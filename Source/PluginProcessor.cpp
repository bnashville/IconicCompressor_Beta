/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "functions.h"
#include "Compressor.h"

//==============================================================================
IconicCompressor_betaAudioProcessor::IconicCompressor_betaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    state = new AudioProcessorValueTreeState(*this, nullptr);
    thisCompressor = new compressor();
    
    state->createAndAddParameter("input", "Input", "Input", NormalisableRange<float>(-48, 12, .1), 0, nullptr, nullptr);
    state->createAndAddParameter("output", "Output", "Output", NormalisableRange<float>(-48, 12, .1), 0, nullptr, nullptr);
    state->createAndAddParameter("threshold", "Threshold", "Threshold", NormalisableRange<float>(-48, 12, .1), 0, nullptr, nullptr);
    
    state->createAndAddParameter("attack", "Attack", "Attack", NormalisableRange<float>(10.f, 800, .5), 20, nullptr, nullptr);
     state->createAndAddParameter("release", "Release", "Release", NormalisableRange<float>(50.f, 1100, .5), 50.0, nullptr, nullptr);
    state->createAndAddParameter("ratio", "Ratio", "Ratio", NormalisableRange<float>(0.1, 20, 0.1), 3, nullptr, nullptr);
    state->createAndAddParameter("crossover", "Crossover", "Crossover", NormalisableRange<float>(30, 20000, 1), 2000.0, nullptr, nullptr);
    
    state->createAndAddParameter("lowCut", "LowCut", "LowCut", NormalisableRange<float>(20, 18000, 1), 20.0, nullptr, nullptr);
    state->createAndAddParameter("highCut", "HighCut", "HighCut", NormalisableRange<float>(20, 20000, 1), 20000.0, nullptr, nullptr);
   
    state->state = ValueTree("compressor");
  
}

IconicCompressor_betaAudioProcessor::~IconicCompressor_betaAudioProcessor()
{

    delete thisCompressor;
}

//==============================================================================
const String IconicCompressor_betaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IconicCompressor_betaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IconicCompressor_betaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool IconicCompressor_betaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double IconicCompressor_betaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IconicCompressor_betaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int IconicCompressor_betaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IconicCompressor_betaAudioProcessor::setCurrentProgram (int index)
{
}

const String IconicCompressor_betaAudioProcessor::getProgramName (int index)
{
    return {};
}

void IconicCompressor_betaAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void IconicCompressor_betaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //Fs = getSampleRate();
    //thisCompressor->SetSamplingRate(Fs);
thisCompressor->SetSamplingRate(sampleRate);
     
}

void IconicCompressor_betaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IconicCompressor_betaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void IconicCompressor_betaAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    
    //load in values from the GUI knobs - these were in the channel loop, moved them out. 
    float thresholdValue = *state->getRawParameterValue("threshold");
    float crossoverValue = *state->getRawParameterValue("crossover");
    float inputValue = *state->getRawParameterValue("input");
    float outputValue = *state->getRawParameterValue("output");
    float attackValue = *state->getRawParameterValue("attack");
    float releaseValue = *state->getRawParameterValue("release");
    float ratioValue = *state->getRawParameterValue("ratio");
    
    float lowCutValue = *state->getRawParameterValue("lowCut");
    float highCutValue = *state->getRawParameterValue("highCut");
    
    //send all these values to the compressor class. Would make sense to be able to send all in one function
    thisCompressor->setAlphaA(attackValue);
    thisCompressor->setAlphaR(releaseValue);
    thisCompressor->setThreshold(thresholdValue);
    thisCompressor->setRatio(ratioValue);
    thisCompressor->setCrossoverFrequency(crossoverValue);
    thisCompressor->setChannelCount(totalNumInputChannels);
    thisCompressor->setBandType(compressor::bandType(compressor::bandType::NORMAL));
    //thisCompressor->setSplitInput(compressor::bandType(compressor::bandType::NORMAL));
    
    // set the "units" of the level detector, either 'db' or 'linear'
    thisCompressor->setDetectorUnit(levelDetector::detectorUnit((levelDetector::detectorUnit::DB)));
    
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    


    //loop through each channel, I.E. Left & Right, or 5.1, etc. -------------------------------------------------------------------
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        thisCompressor->setCurrentChannel(channel);
      
        //loop throught all samples in the buffer
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
       
            //apply input scalar here
            adjustedInput = buffer.getReadPointer(channel)[sample] * pow(10.f,inputValue/20.f);
            
            compressorOutput = thisCompressor->tick(adjustedInput);
            
            //apply output scalar, write the output buffer.
            buffer.getWritePointer(channel)[sample] = compressorOutput * pow(10.f,outputValue/20.f);
            
        } // sample loop

        // ..do something to the data...
    }
}

AudioProcessorValueTreeState& IconicCompressor_betaAudioProcessor::getState(){
    
    return *state;
    
}

//==============================================================================
bool IconicCompressor_betaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* IconicCompressor_betaAudioProcessor::createEditor()
{
    return new IconicCompressor_betaAudioProcessorEditor (*this);
}

//==============================================================================
void IconicCompressor_betaAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    MemoryOutputStream stream(destData, false);
    state->state.writeToStream(stream);
    
}

void IconicCompressor_betaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    ValueTree tree = ValueTree::readFromData(data, sizeInBytes);
    
    if(tree.isValid()){
        
        state->state = tree;
    }
    
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IconicCompressor_betaAudioProcessor();
}


