/*
  ==============================================================================

    CompressorFunctions.cpp
    Created: 20 Oct 2018 3:41:47pm
    Author:  Brian Cofer

  ==============================================================================
*/
//#include "PluginProcessor.h"
#include "CompressorFunctions.h"




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
