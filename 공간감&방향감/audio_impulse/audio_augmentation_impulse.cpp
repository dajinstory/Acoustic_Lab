#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cstdio>
#include <cmath>
#include <stdlib.h>
#include "audio_augmentation_impulse.h"
#define CLOCKSIZE 44100 * 2

using namespace std;
int ratio[44100];
char name[40];
short int value[CLOCKSIZE * 5];
int value_int[CLOCKSIZE * 5];
short int changed_value[CLOCKSIZE * 5];

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

void make_impulse(int num, int len, int power){
	int idx, samples_count = 0, sample_size;
	int cnt = 0;
	FILE *fin, *fout;
	wav_header_t header;
	chunk_t chunk;
	unsigned long chunk_size;

	//load big data
	sprintf(name, "./input/audio/audio (%d).wav", num);
	fin = fopen(name, "rb");
	sprintf(name, "./output/impulse_sound_%d-%d-%d.wav", num, len, power);
	fout = fopen(name, "wb");
	//load header and chunk
	fread(&header, sizeof(header), 1, fin);
	printWAVHeader(header);

	//go to fin1's audio data
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

	fwrite(&header, sizeof(header), 1, fout);
	fwrite(&chunk, sizeof(chunk), 1, fout);

	//load audio data
	for (idx = 0; idx < CLOCKSIZE * 3; idx++){
		fread(&value[idx], sample_size, 1, fin);
		value_int[idx] = (int)value[idx];
	}
	
	//check ratio array
	for (int i = 0, rate = 1; rate <= 16384 * 2; i += rate, rate *= 2){
		if (i == 1){
			i = 10000;
			rate *= 2;
		}
		for (int k = 0; k < rate; k++){
			ratio[i + k] = 16384 / rate;
		}
	}
	int before = 1;
	for (int i = 0; i < 1024*32 - 1; i++){
//		if (ratio[i] != before)printf("\n");
//		printf("%d ", ratio[i]);
//		before = ratio[i];
	}

	for (idx = 0; idx < CLOCKSIZE * 3; idx++){
		////////////////////////////////////////////////////郊蚊操醤廃陥郊蚊操醤廃陥. len葵戚櫛 power戚櫛 100生稽 蟹寛爽澗依旭精 雫室廃蕉級 陥 郊蚊操醤廃雁原び;っしい原びっかい戚ったいけし虞びっし
		/*
		if (idx < len){
			changed_value[idx] = value[idx];
		}
		else{
			changed_value[idx] = value[idx] + value[idx - len] * power / 100;
		}
		*/
		long long sum = 0;
		long long div = 0;
		/*
		for (int j = 0; idx - j >= 0 && j < 44100;j++){// 1024 * 32 - 1; j++){
			sum += ratio[j] * value_int[idx - j];
			div += ratio[j];
		}
		*/
		sum += 1000 * value_int[idx];
		div += 1000;
		if (idx > 3000){
			sum += 600 * value_int[idx - 3000];
			div += 600;
			if (idx > 25000){
				sum += 100 * value_int[idx - 25000];
				div += 100;
				if (idx > 80000){
					sum += 30 * value_int[idx - 80000];
					div += 30;
				}
			}
		}
		changed_value[idx] = (short int)(sum / div);
		if (idx % 44100 == 0)printf("processing..");
		//changed_value[idx] = (changed_value[idx - 1] + value[idx] * 15) / 16;
	}
	for (idx = 0; idx < CLOCKSIZE * 3; idx++){
		fwrite(&changed_value[idx], sample_size, 1, fout);
	}
	fclose(fin);
	fclose(fout);
}