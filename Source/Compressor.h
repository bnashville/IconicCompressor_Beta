//
//  Compressor.h
//  IconicCompressor_Beta
//
//  Created by Brian Cofer on 10/26/18.
//
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>
#include <cstdlib>
using namespace std;

#ifndef Compressor_h
#define Compressor_h


/*! Compressor Class; The "root" of the compressor.
 Takes in various compressor options, e.g. attack, release, threshold, etc.
 Some code and functionality adapted from
 Tarr, Eric. Hack Audio: An Introduction to Computer Programming and Digital Signal Processing in MATLAB. Routledge, 2018.
 */
class compressor {
    
public:
    
    //Compressor Initilization
    compressor(){
        //initialize comp settings

    }
    ~compressor(){
       
    }
   
    float tick(float input, int channel){
     
        return computeGain(input);
    }

    void setCurrentSample(float currentSample) { inputSample = currentSample; }
    float getCurrentSample() { return inputSample; }
    void SetSamplingRate(float Fs) {

    }
   
    void setThreshold(float t){
        threshold = t;
        
    }
    void setRatio(float r){
        ratio = r;
       
    }

    void setAttack(float aTime){
        alphaA = exp(-log(9)/(sampleRate * aTime));
    }
    
    void setRelease(float rTime){
       alphaR = exp(-log(9)/(sampleRate * rTime));
    }
    
    void setCurrentChannel(int c) {
        channel = c;
       
    }
    float getGainChange() {
         return linA;
       
    }
    float computeGain(float input){
       //Turn the input signal into a uni-polar signal on the dB scale
        float y_uni = abs(y_prev[channel]);
        
        float y_dB = 20*log10(y_uni/1);
        float gainSC;
        
        // Ensure there are no values of negative infinity
        if (y_dB < -96) {
            y_dB = -96;
        }
        
        // Static Characteristics
        if (y_dB > threshold) {
            // Perform Downwards Compression
            gainSC = threshold + (y_dB - threshold)/ratio;
        
        } else {
            // Do not perform compression
            gainSC = y_dB;
        }
        
        gainChange_dB = gainSC - y_dB;
        
        // smooth over the gainChange
        if (gainChange_dB < gainSmoothPrev[channel]) {
            // attack mode
            gainSmooth = ((1-alphaA)*gainChange_dB) + (alphaA*gainSmoothPrev[channel]);
        } else {
            // release
            gainSmooth = ((1-alphaR)*gainChange_dB)+(alphaR*gainSmoothPrev[channel]);
        }
        
        // Convert to linear amplitude scalar
        linA = pow(10,gainSmooth/20);

        y_prev[channel] = linA * input;
        return y_prev[channel];
        
        // Update gainSmoothPrev used in the next sample of the loop
        gainSmoothPrev[channel] = gainSmooth;
        
    
    }
  
  
protected:
    float sampleRate = 48000;
    int channel = 1;
    
    float gainChange_dB;
    
    float inputSample;
    float outputSample;
    
    float threshold = -15;
    float ratio = 4;
    //float attackTime = .0004; //0.00002 - 0.0008
    //float releaseTime = .05; //.05 - 1.1

    float gainSmooth;
    float gainSmoothPrev[2] = {0};
    float y_prev[2] = {0};
    float linA;
    
    float alphaA;
    float alphaR;
  
};
#endif /* Compressor_h */
