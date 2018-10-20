/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CompressorFunctions.cpp"


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
    Fs = getSampleRate();
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    
    //load in values from the GUI knobs
    float thresholdValue = *state->getRawParameterValue("threshold");
    float crossoverValue = *state->getRawParameterValue("crossover");
    float inputValue = *state->getRawParameterValue("input");
    float outputValue = *state->getRawParameterValue("output");
    float attackValue = *state->getRawParameterValue("attack");
    float releaseValue = *state->getRawParameterValue("release");
    float ratioValue = *state->getRawParameterValue("ratio");
    float lowCutValue = *state->getRawParameterValue("lowCut");
    float highCutValue = *state->getRawParameterValue("highCut");


    attackValue = attackValue / 1000;
    releaseValue = releaseValue / 1000;
    
    alphaA = exp(-log(9) / (Fs * attackValue));
    alphaR = exp(-log(9) / (Fs * releaseValue));
    
    //loop through each channel, I.E. Left & Right, or 5.1, etc. -------------------------------------------------------------------
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
       
        
        
        //----------------------SAMPLE BUFFER ----------------------------------------------------------------------
        
        //loop throught all samples in the buffer
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
       
            //apply input scalar here
            adjustedInput = buffer.getReadPointer(channel)[sample] * pow(10.f,inputValue/20.f);
            
            // get level of current sample, either linear or dB based on toggle button
          
            inputLevel = levelDetection(adjustedInput, sideChainAlgorithm);

            
            // we have the current input level, now we need to consider our algorithm
            
            
            if(tremTypeAlgorithm == 0) { // NOT multiband
                
                if (inputLevel < -96) {
                    inputLevel = -96;
                }
            
                //compare to threshold
                if(inputLevel > thresholdValue) {
                    //perform compression
                    gainSideChain[channel] = thresholdValue + (inputLevel - thresholdValue) / ratioValue;
                }
                else {
                    // no compression
                    gainSideChain[channel] = inputLevel;
                }
                
                gainChange_dB[channel] = gainSideChain[channel] - inputLevel;
                
                // smooth over the gainChange
                if(gainChange_dB[channel] < gainSmoothPrev[channel]) {
                    //attack
                    gainSmooth[channel] = ((1-alphaA) * gainChange_dB[channel]) + (alphaA*gainSmoothPrev[channel]);
                    
                }
                else {
                    //release
                    gainSmooth[channel] = ((1-alphaR) * gainChange_dB[channel]) + (alphaR*gainSmoothPrev[channel]);
                }
                
                // convert to linear amplitude scalar
                linA[channel] = pow(10,gainSmooth[channel]/20);
                
                //apply linear amplitude to input sample
                recombineSignal = linA[channel] * adjustedInput;
                
                //update gainSmoothPrev
                gainSmoothPrev[channel] = gainSmooth[channel];
                
                //create our next LFO value based on which wave type is selected
                switch(compressorAlgorithm) {
                        
                    case 0 :
                        //tube
                        break;
                    case 1:    //do stuff; //optical
                        break;
                    case 2:  //do stuff; //fet
                        break;
                    case 3:  //do stuff; //vca
                        
                        break;
                    case 4:  //do stuff; //perceptual
                        break;
                    case 5:  //do stuff; //other
                        break;
                } //end of switch
               
                //recombineSignal =  buffer.getReadPointer(channel)[sample];
                
            }
            else {
                //for all other cases, need to split signal into 'bass' and 'treble' parts to modify seperately.
                
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
                
                //Slide the samples over one position; a circular buffer of sorts?
                PrevSample[2][channel]  = PrevSample[1][channel] ;
                PrevSample[1][channel]  = PrevSample[0][channel] ;
                PrevSample[0][channel]  = buffer.getReadPointer(channel)[sample];
                
                //y[n] = (b0LPF/a0LPF)*x[n] + (b1LPF/a0LPF)*x[n-1] + (b2LPF/a0LPF)*x[n-2] - (a1LPF/a0LPF)*y[n-1] - (a2LPF/a0LPF)*y[n-2];
                bassOutput[0][channel]  = (b0LPF/a0LPF)*PrevSample[0][channel] +
                (b1LPF/a0LPF)*PrevSample[1][channel]  +
                (b2LPF/a0LPF)*PrevSample[2][channel]  -
                (a1LPF/a0LPF)*bassOutput[1][channel]  -
                (a2LPF/a0LPF)*bassOutput[2][channel] ;
                
                //TREBLE
                //HPF: H(s) = s^2 / (s^2 + s/Q + 1)
                b0HPF =  (1 + cos(w0))/2;
                b1HPF = -(1 + cos(w0));
                b2HPF =  (1 + cos(w0))/2;
                a0HPF =   1 + alpha;
                a1HPF =  -2*cos(w0);
                a2HPF =   1 - alpha;
                
                trebleOutput[0][channel]  = (b0HPF/a0HPF)*PrevSample[0][channel]  +
                (b1HPF/a0HPF)*PrevSample[1][channel]  +
                (b2HPF/a0HPF)*PrevSample[2][channel]  -
                (a1HPF/a0HPF)*trebleOutput[1][channel]  -
                (a2HPF/a0HPF)*trebleOutput[2][channel] ;
                
                //---------BUFFERS FOR ALL EQ FUNCTIONS-------------------------
                
                //here is the buffer to keep track of previous values
                
                bassOutput[2][channel]  = bassOutput[1][channel] ;
                bassOutput[1][channel]  = bassOutput[0][channel] ;
                
                trebleOutput[2][channel]  = trebleOutput[1][channel] ;
                trebleOutput[1][channel]  = trebleOutput[0][channel] ;
                
                //--------APPLY COMPRESSION TO BASS OR TREBLE ONLY--------------
                if (tremTypeAlgorithm == 1) {
                    //APPLY TREBLE COMPRESSION HERE only
                    modulatedTreble = trebleOutput[0][channel] ;
                    modulatedBass = bassOutput[0][channel] ;
                }
                else if (tremTypeAlgorithm == 2) {
                    //bass only
                    modulatedBass = bassOutput[0][channel] ;
                    modulatedTreble = trebleOutput[0][channel] ;
                    
                    
                }

                //processing done, combine bass & treble back into one signal
                recombineSignal = modulatedTreble + modulatedBass;
                
            } // end else (crossover)
            
            //write the output buffer.
            
            buffer.getWritePointer(channel)[sample] = recombineSignal * pow(10.f,outputValue/20.f);
            
            //-----------------------SET PARAMATERS FOR NEXT ITERATION OF LOOP-----------
            
           
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
