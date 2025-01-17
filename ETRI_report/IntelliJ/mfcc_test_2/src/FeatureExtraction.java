import Feature.Energy;
import Feature.MFCC;
import Feature.MagnitudeSpectrum;
import Feature.ZeroCrossing;
import SignalProcess.WaveIO;
import Distance.CityBlock;
import Distance.Cosine;
import Distance.Euclidean;
import Player.SoundEffectDemo;

/**
 * Created by workshop on 9/18/2015.
 * Show examples for different feature extraction methods, including Energy, MagnitudeSpectrum, MFCC and Zero-Crossing;
 */
public class FeatureExtraction {
    public static void main(String[] args){
        new SoundEffectDemo();
          WaveIO waveIO1 = new WaveIO();
          short[] signals1 = waveIO1.readWave("data/input/office1.wav");
//
//        WaveIO waveIO2 = new WaveIO();
//        short[] signals2 = waveIO2.readWave("data/input/bus2.wav");
//
//        Energy energy1 = new Energy();
//        double[] eFeature1 = energy1.getFeature(signals1);
//
//        Energy energy2 = new Energy();
//        double[] eFeature2 = energy2.getFeature(signals2);
//
//        MagnitudeSpectrum ms1 = new MagnitudeSpectrum();
//        double[] msFeature1 = ms1.getFeature(signals1);
//
//        MagnitudeSpectrum ms2 = new MagnitudeSpectrum();
//        double[] msFeature2 = ms2.getFeature(signals2);
//
          MFCC mfcc = new MFCC();
          double[][] MFCC = mfcc.process(signals1);//13-d mfcc
//
          double[] mean = mfcc.getMeanFeature();
          for(int i=0;i<1000;i++){
              System.out.println(mean[i] + " ");
          }
//
//
//
//        ZeroCrossing zc = new ZeroCrossing();
//        double[] zero1 = zc.getFeature(signals1);
//        double[] zero2 = zc.getFeature(signals2);
//
//        Cosine cosine = new Cosine();
//        double ss = cosine.getDistance(msFeature1, msFeature2);
//
//        Euclidean euclidean = new Euclidean();
//        double ee = euclidean.getDistance(msFeature1, msFeature2);
//
//        CityBlock cityBlock = new CityBlock();
//        double cc = cityBlock.getDistance(msFeature1, msFeature2);
//
//        System.out.println(ss);
//        System.out.println(ee);
//        System.out.println(cc);
    }
}
