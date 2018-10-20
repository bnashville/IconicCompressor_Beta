/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class IconicCompressor_betaAudioProcessorEditor  : public AudioProcessorEditor,
public Button::Listener,
public Slider::Listener,
public Timer
{
public:
    void extracted();
    
    IconicCompressor_betaAudioProcessorEditor (IconicCompressor_betaAudioProcessor&);
    ~IconicCompressor_betaAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked (Button* button) override;
    void sliderValueChanged(Slider* slider) override;
    void timerCallback() override;
    
    int guiWidth = 900;
    int guiHeight = 400;
    int guiCenter = guiWidth/2;
    
    int largeSliderSize = 100;
    int smallSliderSize = 75;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    HackAudio::Selector testSelector;
    
    HackAudio::Slider inputKnob;
    HackAudio::Slider thresholdKnob;
    HackAudio::Slider crossoverKnob;
    HackAudio::Slider ratioKnob;
    HackAudio::Slider outputKnob;
    HackAudio::Slider releaseKnob;
    HackAudio::Slider toneKnob;
    HackAudio::Slider attackKnob;
    
    juce::Label inputLabel;
    juce::Label thresholdLabel;
    juce::Label crossoverLabel;
    juce::Label ratioLabel;
    juce::Label outputLabel;
    
    juce::Label lowCutLabel;
    juce::Label highCutLabel;
    juce::Label mixLabel;
    
    juce::Label toneLabel;
    juce::Label releaseLabel;
    juce::Label attackLabel;
    IconicCompressor_betaAudioProcessor& processor;
    
    HackAudio::Button analogSourceButton;
    HackAudio::Button digitalSourceButton;
    
    HackAudio::Button tubeButton;
    HackAudio::Button opticalButton;
    HackAudio::Button fetButton;
    HackAudio::Button vcaButton;

    HackAudio::Button peakScButton;
    HackAudio::Button rmsScButton;
    HackAudio::Button smoothButton;
    HackAudio::Button levelCorrectedButton;
    
    HackAudio::Button feedBackScButton;
    HackAudio::Button feedForwardButton;
    HackAudio::Button hybridButton;

    HackAudio::Button ldOrderButton;
    HackAudio::Button gcOrderButton;
    
    HackAudio::Button normalTremButton;
    HackAudio::Button trebleTremButton;
    HackAudio::Button bassTremButton;
    HackAudio::Button phaseTremButton;
    
    HackAudio::Slider lowCutKnob;
    HackAudio::Slider highCutKnob;
    HackAudio::Slider mixKnob;
    
    HackAudio::FlexBox flexOrder;
    HackAudio::FlexBox flexButtons;
    HackAudio::FlexBox flexKnobs;
    HackAudio::FlexBox flexAnalog;
    HackAudio::FlexBox flexLevelDetector;
    HackAudio::FlexBox flexSystemDesign;
    
    HackAudio::FlexBox flexNote;
    HackAudio::FlexBox flexTremType;
    HackAudio::FlexBox flexSideChain;
    HackAudio::FlexBox flexMix;
    
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> crossoverAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> rateSourceAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lowCutAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> highCutAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconicCompressor_betaAudioProcessorEditor)
};
