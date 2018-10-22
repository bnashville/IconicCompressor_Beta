/*
  ==============================================================================

    CompressorFunctions.cpp
    Created: 20 Oct 2018 3:41:47pm
    Author:  Brian Cofer

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "CompressorFunctions.h"

// Level Detection Algorithms
float levelDetection(float inputSample, int detectionType) {
    
    //return db value of sidechain
    if (detectionType == 0) {

        return 20.f * log10(abs(inputSample)/1.f);

    }
    
    // return linear value of sidechain
    else if (detectionType == 1) {
        return abs(inputSample);
       
    }
    else {
        return inputSample;
    }

}

//Smoothing Function
float gainSmoothFunction(float currentSample, float previousSample, float alphaAttack, float alphaRelease) {
    
    
    // smooth over the gainChange
    if(currentSample < previousSample) {
        //attack
        return  ((1-alphaAttack) * currentSample) + (alphaAttack*previousSample);
        
    }
    else {
        //release
        return ((1-alphaRelease) * currentSample) + (alphaRelease*previousSample);
    }

}
// Gain Computer

class crossoverFilter {
    
public:
    float crossoverFrequency;
    float Fs;
    float PrevSample[3][2] = {0};
    float currentSample[2];
    float treble[3][2] = {0};
    float bass[3][2] = {0};
    int chan = 0;
    
    
private:
    float pi = 3.141592653589793238;
    float f0 = crossoverFrequency;
    float w0 = 2*pi*f0/Fs;
    float Q = 0.707;
    float alpha = sin(w0)/(2*Q);
    
    //BASS
    //LPF: H(s) = 1 / (s^2 + s/Q + 1)
    float b0LPF =  (1 - cos(w0))/2;
    float b1LPF =   1 - cos(w0);
    float b2LPF =  (1 - cos(w0))/2;
    float a0LPF =   1 + alpha;
    float a1LPF =  -2*cos(w0);
    float a2LPF =   1 - alpha;
    
    crossoverFilter() {
    //Slide the samples over one position
    PrevSample[2][chan]  = PrevSample[1][chan] ;
    PrevSample[1][chan]  = PrevSample[0][chan] ;
    PrevSample[0][chan]  = currentSample[chan];
    
    //y[n] = (b0LPF/a0LPF)*x[n] + (b1LPF/a0LPF)*x[n-1] + (b2LPF/a0LPF)*x[n-2] - (a1LPF/a0LPF)*y[n-1] - (a2LPF/a0LPF)*y[n-2];
    bass[0][chan]  = (b0LPF/a0LPF)*PrevSample[0][chan] +
    (b1LPF/a0LPF)*PrevSample[1][chan]  +
    (b2LPF/a0LPF)*PrevSample[2][chan]  -
    (a1LPF/a0LPF)*bass[1][chan]  -
    (a2LPF/a0LPF)*bass[2][chan] ;
    
    //TREBLE
    //HPF: H(s) = s^2 / (s^2 + s/Q + 1)
    float b0HPF =  (1 + cos(w0))/2;
    float b1HPF = -(1 + cos(w0));
    float b2HPF =  (1 + cos(w0))/2;
    float a0HPF =   1 + alpha;
    float a1HPF =  -2*cos(w0);
    float a2HPF =   1 - alpha;
    
    treble[0][chan]  = (b0HPF/a0HPF)*PrevSample[0][chan]  +
    (b1HPF/a0HPF)*PrevSample[1][chan]  +
    (b2HPF/a0HPF)*PrevSample[2][chan]  -
    (a1HPF/a0HPF)*treble[1][chan]  -
    (a2HPF/a0HPF)*treble[2][chan] ;
    
    //---------BUFFERS FOR ALL EQ FUNCTIONS-------------------------
    
    //here is the buffer to keep track of previous values
    
    bass[2][chan]  = bass[1][chan] ;
    bass[1][chan]  = bass[0][chan] ;
    
    treble[2][chan]  = treble[1][chan] ;
    treble[1][chan]  = treble[0][chan] ;
    }
};


/*
 the original code that we are replacing with a class
 f0 = crossoverValue;
 w0 = 2*pi*f0/Fs;
 Q = 0.707;
 alpha = sin(w0)/(2*Q);
 
 //BASS
 //LPF: H(s) = 1 / (s^2 + s/Q + 1)
 b0LPF =  (1 - cos(w0))/2;
 b1LPF =   1 - cos(w0);
 b2LPF =  (1 - cos(w0))/2;
 a0LPF =   1 + alpha;
 a1LPF =  -2*cos(w0);
 a2LPF =   1 - alpha;
 
 //Slide the samples over one position
 PrevSample[2][chan]  = PrevSample[1][chan] ;
 PrevSample[1][chan]  = PrevSample[0][chan] ;
 PrevSample[0][chan]  = buffer.getReadPointer(chan)[sample];
 
 //y[n] = (b0LPF/a0LPF)*x[n] + (b1LPF/a0LPF)*x[n-1] + (b2LPF/a0LPF)*x[n-2] - (a1LPF/a0LPF)*y[n-1] - (a2LPF/a0LPF)*y[n-2];
 bassOutput[0][chan]  = (b0LPF/a0LPF)*PrevSample[0][chan] +
 (b1LPF/a0LPF)*PrevSample[1][chan]  +
 (b2LPF/a0LPF)*PrevSample[2][chan]  -
 (a1LPF/a0LPF)*bassOutput[1][chan]  -
 (a2LPF/a0LPF)*bassOutput[2][chan] ;
 
 //TREBLE
 //HPF: H(s) = s^2 / (s^2 + s/Q + 1)
 b0HPF =  (1 + cos(w0))/2;
 b1HPF = -(1 + cos(w0));
 b2HPF =  (1 + cos(w0))/2;
 a0HPF =   1 + alpha;
 a1HPF =  -2*cos(w0);
 a2HPF =   1 - alpha;
 
 trebleOutput[0][chan]  = (b0HPF/a0HPF)*PrevSample[0][chan]  +
 (b1HPF/a0HPF)*PrevSample[1][chan]  +
 (b2HPF/a0HPF)*PrevSample[2][chan]  -
 (a1HPF/a0HPF)*trebleOutput[1][chan]  -
 (a2HPF/a0HPF)*trebleOutput[2][chan] ;
 
 //---------BUFFERS FOR ALL EQ FUNCTIONS-------------------------
 
 //here is the buffer to keep track of previous values
 
 bassOutput[2][chan]  = bassOutput[1][chan] ;
 bassOutput[1][chan]  = bassOutput[0][chan] ;
 
 trebleOutput[2][chan]  = trebleOutput[1][chan] ;
 trebleOutput[1][chan]  = trebleOutput[0][chan] ;
 
 */
