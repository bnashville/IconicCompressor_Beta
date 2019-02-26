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
                  ),
#endif
treeState (*this, nullptr, Identifier ("AnalogCompressor"), createParameterLayout() )

{
    
    //thisBiquad = new RBJFilter();
    
    inputParameter = treeState.getRawParameterValue("input");
    outputParameter = treeState.getRawParameterValue("output");
    thresholdParameter = treeState.getRawParameterValue("threshold");
    mixParameter = treeState.getRawParameterValue("mix");
    attackParameter = treeState.getRawParameterValue("attack");
    releaseParameter = treeState.getRawParameterValue("release");
    ratioParameter = treeState.getRawParameterValue("ratio");
    
    thisCompressor = new compressor();
    
}

AudioProcessorValueTreeState::ParameterLayout IconicCompressor_betaAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> params;
    
    auto inputParams = std::make_unique<AudioParameterFloat> ("input","Input",-48,12,0);
    auto outputParams = std::make_unique<AudioParameterFloat> ("output","Output",    -48,12,0);
    auto thresholdParams = std::make_unique<AudioParameterFloat> ("threshold","Threshold",-48,12,0);
    auto mixParams = std::make_unique<AudioParameterFloat> ("mix","Mix",    0,100,100);
    auto attackParams = std::make_unique<AudioParameterFloat> ("attack","Attack",  NormalisableRange<float> (0.00002, 0.0008), .0004);
    //0.00002, 0.0008, .0004);
    auto releaseParams = std::make_unique<AudioParameterFloat> ("release","Release",0.05, 1.1, .7);
    auto ratioParams = std::make_unique<AudioParameterFloat> ("ratio","Ratio",    1, 20, 3);
    
    params.push_back(std::move(inputParams));
    params.push_back(std::move(outputParams));
    params.push_back(std::move(thresholdParams));
    params.push_back(std::move(mixParams));
    params.push_back(std::move(attackParams));
    params.push_back(std::move(releaseParams));
    params.push_back(std::move(ratioParams));
    
    return { params.begin(), params.end() };
    
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
    
    auto thresholdValue = *thresholdParameter;
    auto inputValue = *inputParameter;
    auto outputValue = *outputParameter;
    auto attackValue = *attackParameter;
    auto releaseValue = *releaseParameter;
    auto ratioValue = *ratioParameter;
    auto wetDryValue = *mixParameter;
    
    //send all these values to the compressor class. Would make sense to be able to send all in one function
    thisCompressor->setAttack(attackValue);
    thisCompressor->setRelease(releaseValue);
    thisCompressor->setThreshold(thresholdValue);
    thisCompressor->setRatio(ratioValue);
    
    
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
            
            //grMeter.setSource(channel, adjustedInput);
            
            compressorOutput = thisCompressor->tick(adjustedInput, channel);
            
            adjustedOutput = compressorOutput * pow(10.f,outputValue/20.f);
            
            gainChange_meter = thisCompressor->getGainChange();
            
            //gainChange_meter = abs(adjustedInput - compressorOutput);
            //apply output scalar, write the output buffer.
            buffer.getWritePointer(channel)[sample] = adjustedOutput ;
            
            //next line is to activate wet/dry mix
            //buffer.getWritePointer(channel)[sample] = (compressorOutput * (wetDryValue/100)) + (adjustedInput * ( (100-wetDryValue) /100 ));
            
        } // sample loop
        
        // ..do something to the data...
    }
}

//==============================================================================
bool IconicCompressor_betaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* IconicCompressor_betaAudioProcessor::createEditor()
{
    return new IconicCompressor_betaAudioProcessorEditor (*this, treeState);
}

//==============================================================================
void IconicCompressor_betaAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void IconicCompressor_betaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (treeState.state.getType()))
            treeState.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IconicCompressor_betaAudioProcessor();
}



