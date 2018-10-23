/*
  ==============================================================================

    functions.h
    Created: 23 Oct 2018 7:02:13am
    Author:  Brian Cofer

  ==============================================================================
*/

#pragma once



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


