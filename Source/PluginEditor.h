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
    
    IconicCompressor_betaAudioProcessorEditor (IconicCompressor_betaAudioProcessor&, AudioProcessorValueTreeState&);
    ~IconicCompressor_betaAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    void buttonClicked (Button* button) override;
    void sliderValueChanged(Slider* slider) override;
    void timerCallback() override;
    
    int guiWidth = 825;
    int guiHeight = 420;
    int guiCenter = guiWidth/2;
    
    int largeSliderSize = 100;
    int smallSliderSize = 75;
    int knobSpacing = 100;
    
    int ratioKnobWidth = 45;
    int ratioKnobHeight = 30;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioProcessorValueTreeState& valueTreeState;
    IconicCompressor_betaAudioProcessor& processor;
    
    HackAudio::Selector testSelector;
    
    HackAudio::Slider inputKnob;
    HackAudio::Slider thresholdKnob;
    HackAudio::Slider crossoverKnob;
    HackAudio::Slider ratioKnob;
    HackAudio::Slider outputKnob;
    HackAudio::Slider releaseKnob;
    HackAudio::Slider toneKnob;
    HackAudio::Slider attackKnob;
    HackAudio::Slider mixKnob;
    
    HackAudio::Meter grMeter;
    
    juce::Label inputLabel;
    juce::Label thresholdLabel;
    juce::Label crossoverLabel;
    juce::Label ratioLabel;
    juce::Label outputLabel;
    juce::Label overEasyButtonLabel;
    
    juce::Label lowCutLabel;
    juce::Label highCutLabel;
    juce::Label mixLabel;
    
    juce::Label toneLabel;
    juce::Label releaseLabel;
    juce::Label attackLabel;
    
    HackAudio::Button fetCompToggle;
    HackAudio::Button opticalCompToggle;
    HackAudio::Button tubeCompToggle;
    HackAudio::Button vcaCompToggle;

    HackAudio::Button overEasyButton;

    HackAudio::Button ratioFourToggle;
    HackAudio::Button ratioEightToggle;
    HackAudio::Button ratioTwelveToggle;
    HackAudio::Button ratioTwentyToggle;
    HackAudio::Button ratioAllToggle;
    
    HackAudio::FlexBox flexButtons;
    HackAudio::FlexBox flexKnobsCompressor;
    HackAudio::FlexBox flexRatioToggle;
    HackAudio::FlexBox flexMeter;
    
    ////// Block Diagram Area /////////
    HackAudio::Viewport port;
    //HackAudio::Diagrams::Compressor compressorDiagram;
    
public:
    std::unique_ptr<SliderAttachment> thresholdAttachment;
    std::unique_ptr<SliderAttachment> outputAttachment;
    std::unique_ptr<SliderAttachment> ratioAttachment;
    std::unique_ptr<SliderAttachment> inputAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconicCompressor_betaAudioProcessorEditor)
};
