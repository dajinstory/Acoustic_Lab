#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cstdio>
#include <cmath>
#include <stdlib.h>
#include "audio_augmentation.h"
#define CLOCKSIZE 44100 * 2
#define SHORTINT_MAX 32767
#define POWERLEVEL 5
using namespace std;

char name[40];
short int value[CLOCKSIZE*100];
short int increased_value[CLOCKSIZE * 100];
short int changed_value[CLOCKSIZE * 100];

void printWAVHeader(wav_header_t header){
	printf("WAV File Header read:\n");
	printf("File Type: %s\n", header.chunkID);
	printf("File Size: %ld\n", header.chunkSize);
	printf("WAV Marker: %s\n", header.format);
	printf("Format Name: %s\n", header.subchunk1ID);
	printf("Format Length: %ld\n", header.subchunk1Size);
	printf("Format Type: %hd\n", header.audioFormat);
	printf("Number of Channels: %hd\n", header.numChannels);
	printf("Sample Rate: %ld\n", header.sampleRate);
	printf("Sample Rate * Bits/Sample * Channels / 8: %ld\n", header.byteRate);
	printf("Bits per Sample * Channels / 8.1: %hd\n", header.blockAlign);
	printf("Bits per Sample: %hd\n", header.bitsPerSample);
	printf("\n");
}

void divide_audio(int num, int time, int div){
	int idx, samples_count = 0, sample_size;
	int cnt = 0;
	FILE *fin, *headerFile;
	wav_header_t header;
	chunk_t chunk;
	unsigned long chunk_size;

	//load big data
	sprintf(name, "./input/5s_scream/5s_scream (%d).wav", num);
	fin = fopen(name, "rb");
	headerFile = fopen("./input/header_format.wav", "rb");

	//load header and chunk
	fread(&header, sizeof(header), 1, fin);
	printWAVHeader(header);

	fread(&header, sizeof(header), 1, headerFile);
	while (true)
	{
		fread(&chunk, sizeof(chunk), 1, fin);
		printf("%c%c%c%c\t%u\n",chunk.ID[0], chunk.ID[1], chunk.ID[2], chunk.ID[3], chunk.size);
		if (*(unsigned int *)&chunk.ID ==0x61746164)
			break;
		if (chunk.ID[2] == 'd' && chunk.ID[3] == 'a'){
			fseek(fin, -6, SEEK_CUR);
			continue;
		}
		//skip chunk data bytes
		fseek(fin, chunk.size, SEEK_CUR);
	}
	sample_size = header.bitsPerSample / 8;
	printWAVHeader(header);

	//load audio data
	for (idx = 1; feof(fin) == 0; idx++){
		//read (44100 x time) data
		for (; samples_count < CLOCKSIZE * time && feof(fin) == 0; samples_count++){
			fread(&value[samples_count], sample_size, 1, fin);
		}
		
		//if EOF, break;
		if (samples_count != CLOCKSIZE*time){
			break;
		}

		//to avoid overflow, make the audio data to maximum volume, and then decrease.
		short int max_value = 0;
		for (int i = 0; i < samples_count; i++){
			short int tmp = value[i];
			if (tmp < 0)tmp = -tmp;
			if (tmp > max_value)max_value = tmp;
		}
		for (int i = 0; i < samples_count; i++){
			int tmp = (int)value[i];
			tmp *= SHORTINT_MAX;
			tmp /= max_value;
			increased_value[i] = (short int)tmp;
		}
		
		for (int volume = 1; volume <= POWERLEVEL; volume++){
			//load directory to be saved
			FILE *fout;
			sprintf(name, "./output/3s_danger_scream/3s_danger_scream-%d-%d-(%d).wav", num, idx, volume);
			fout = fopen(name, "wb");

			//write header and chunk
			fwrite(&header, sizeof(header), 1, fout);
			fwrite(&chunk, sizeof(chunk), 1, fout);

			//write audio data
			int maxpower = 1, nowpower = 1;
			for (int i = 0; i < POWERLEVEL; i++){
				maxpower *= POWERLEVEL + 1 - i;
			}
			for (int i = 0; i < volume; i++){
				nowpower *= POWERLEVEL + 1 - i;
			}
			for (int i = 0; i < samples_count; i++){
				long long tmp = (long long)increased_value[i];
				tmp *= nowpower;
				tmp /= maxpower;
				changed_value[i] = (short int)tmp;
				fwrite(&changed_value[i], sample_size, 1, fout);
			}

			//Renew number of samples
			header.chunkSize = samples_count * 2 + 36;
			chunk.size = samples_count * 2;
			fseek(fout, 0, SEEK_SET);
			fwrite(&header, sizeof(header), 1, fout);
			fwrite(&chunk, sizeof(chunk), 1, fout);
			fclose(fout);
		}
		//move some data for next duplicated things
		for (int i = 0; i < samples_count - div; i++){
			value[i] = value[i + div];
		}
		samples_count -= div;
	}
	fclose(fin);
	
}

