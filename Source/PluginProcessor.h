/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filters.h"
#include "Compressor.h"


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

    //int compressorAlgorithm = 0;
    //int rateSourceAlgorithm = 1;
    int defaultKnobView = 0;
    //int sideChainAlgorithm = 3;
    
    //float multiplierAlgorithm = 8;
    //int tremTypeAlgorithm = 0;
    //int systemDesign = 0;
    //int systemOrder = 0;
    
    //block diagram
    int algorithm = 0;
    int typeModifier = 0;

    float compressorOutput = 1;
    float adjustedInput = 0;
    float adjustedOutput = 0;
    float gainChange_meter;
    
    AudioProcessorValueTreeState& getState();
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState treeState;
    
    float* inputParameter = nullptr;
    float* outputParameter  = nullptr;
    float* thresholdParameter = nullptr;
    float* attackParameter = nullptr;
    float* releaseParameter  = nullptr;
    float* ratioParameter = nullptr;
    float* mixParameter = nullptr;
    
private:
    compressor* thisCompressor;
        
    double Fs = 48000;
    float pi = 3.141592653589793238;

    AudioPlayHead* playHead;
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;

    float randNum = rand()%(1-0 + 1) + 0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconicCompressor_betaAudioProcessor)
};

