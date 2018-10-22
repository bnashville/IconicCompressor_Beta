/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filters.h"


//==============================================================================
/**
*/
class IconicCompressor_betaAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    IconicCompressor_betaAudioProcessor();
    ~IconicCompressor_betaAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
    //my variables
    
    // Wave Type Variable
    // This variable can be used to change between different types of algorithms

    
    // 0 - Tube, 1 - Optical, 2 - Fet, 3 - VCA, 4 - perceptual, 5 - other
    int compressorAlgorithm = 0;
    int rateSourceAlgorithm = 1;
    int defaultKnobView = 0;
    int sideChainAlgorithm = 0;
    
    float multiplierAlgorithm = 8;
    int tremTypeAlgorithm = 0;


    AudioProcessorValueTreeState& getState();
    
private:
    RBJFilter* trebleOutput;
    RBJFilter* bassOutput;
    
    double Fs = 48000;
    float pi = 3.141592653589793238;
    float adjustedInput = 0;
    float inputLevel = 0;
    float inputUnipolar = 0;
    
    float trebleOut[2] = {0};
    float bassOut[2] = {0};
    
    float gainSideChain[2] = {0};
    float gainChange_dB[2] = {0};
    float gainSmooth[2] = {0};
    float linA[2] = {0};
    
    float alphaA = 0;
    float alphaR = 0;
    float gainSmoothPrev[2] = {0};
    
    //More EQ
    float recombineSignal = 1;
    float modulatedTreble = 1;
    float modulatedBass = 1;

    AudioPlayHead* playHead;
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
    ScopedPointer<AudioProcessorValueTreeState> state;
    
    float randNum = rand()%(1-0 + 1) + 0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconicCompressor_betaAudioProcessor)
};


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



