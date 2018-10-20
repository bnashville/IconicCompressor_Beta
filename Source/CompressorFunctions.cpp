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

        inputLevel = 20.f * log10(abs(inputSample)/1.f);

    }
    
    // return linear value of sidechain
    else if (detectionType == 1) {
        inputLevel = abs(inputSample);
       
    }
    
     return inputLevel;
    
}

// Gain Computer

