/*
 ==============================================================================
 
 This file was auto-geneinputd!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
IconicCompressor_betaAudioProcessorEditor::IconicCompressor_betaAudioProcessorEditor (IconicCompressor_betaAudioProcessor& p, AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    
    // order: smoothing before static charastics, or v/v
    // smoothing method: peak, rms approximation, rms exact, 2nd order smoothing function (biquad), feed forward (straight), feed-back (straight), mixed (ff & fb)
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //Settings for the input
    fetCompToggle.addListener(this);
    fetCompToggle.setButtonText("FET");
    fetCompToggle.setConnectedEdges(Button::ConnectedOnRight);
    fetCompToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    fetCompToggle.setSize(100,30);
    fetCompToggle.setRadioGroupId(4);
    fetCompToggle.setToggleState(true, dontSendNotification);
    addAndMakeVisible(fetCompToggle);
    
    opticalCompToggle.addListener(this);
    opticalCompToggle.setButtonText("Optical");
    opticalCompToggle.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    opticalCompToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    opticalCompToggle.setSize(120,30);
    opticalCompToggle.setRadioGroupId(4);
    addAndMakeVisible(opticalCompToggle);
    
    tubeCompToggle.addListener(this);
    tubeCompToggle.setButtonText("Tube");
    tubeCompToggle.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    tubeCompToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    tubeCompToggle.setSize(90,30);
    tubeCompToggle.setRadioGroupId(4);
    addAndMakeVisible(tubeCompToggle);
    
    vcaCompToggle.addListener(this);
    vcaCompToggle.setButtonText("VCA");
    vcaCompToggle.setConnectedEdges(Button::ConnectedOnLeft);
    vcaCompToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    vcaCompToggle.setSize(90,30);
    vcaCompToggle.setRadioGroupId(4);
    addAndMakeVisible(vcaCompToggle);
    
    // -------------'76 RATIO TOGGLE'---------------------------------------
    
    ratioFourToggle.addListener(this);
    ratioFourToggle.setButtonText("4");
    ratioFourToggle.setConnectedEdges(Button::ConnectedOnRight);
    ratioFourToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    ratioFourToggle.setSize(ratioKnobWidth,ratioKnobHeight);
    ratioFourToggle.setRadioGroupId(1);
    ratioFourToggle.setToggleState(true, dontSendNotification);
    addAndMakeVisible(ratioFourToggle);
    
    ratioEightToggle.addListener(this);
    ratioEightToggle.setButtonText("8");
    ratioEightToggle.setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnLeft);
    ratioEightToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    ratioEightToggle.setSize(ratioKnobWidth,ratioKnobHeight);
    ratioEightToggle.setRadioGroupId(1);
    ratioEightToggle.setToggleState(false, dontSendNotification);
    addAndMakeVisible(ratioEightToggle);
    
    ratioTwelveToggle.addListener(this);
    ratioTwelveToggle.setButtonText("12");
    ratioTwelveToggle.setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnLeft);
    ratioTwelveToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    ratioTwelveToggle.setSize(ratioKnobWidth,ratioKnobHeight);
    ratioTwelveToggle.setRadioGroupId(1);
    ratioTwelveToggle.setToggleState(false, dontSendNotification);
    addAndMakeVisible(ratioTwelveToggle);
    
    ratioTwentyToggle.addListener(this);
    ratioTwentyToggle.setButtonText("20");
    ratioTwentyToggle.setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnLeft);
    ratioTwentyToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    ratioTwentyToggle.setSize(ratioKnobWidth,ratioKnobHeight);
    ratioTwentyToggle.setRadioGroupId(1);
    ratioTwentyToggle.setToggleState(false, dontSendNotification);
    addAndMakeVisible(ratioTwentyToggle);
    
    ratioAllToggle.addListener(this);
    ratioAllToggle.setButtonText("All");
    ratioAllToggle.setConnectedEdges(Button::ConnectedOnLeft);
    ratioAllToggle.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    ratioAllToggle.setSize(ratioKnobWidth,ratioKnobHeight);
    ratioAllToggle.setRadioGroupId(1);
    ratioAllToggle.setToggleState(false, dontSendNotification);
    addAndMakeVisible(ratioAllToggle);
    
    // ---------------------------------------------------------------------
    overEasyButton.addListener(this);
    overEasyButton.setButtonText("overeasy");
    //overEasyButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    overEasyButton.setButtonStyle(HackAudio::Button::ButtonStyle::SlidingToggle);
    overEasyButton.setSize(90,30);
    addAndMakeVisible(overEasyButton);
    
    overEasyButtonLabel.setText("overeasy", dontSendNotification);
    overEasyButtonLabel.setFont(HackAudio::Fonts::NowRegular);
    overEasyButtonLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    overEasyButtonLabel.setJustificationType(Justification::centred);
    overEasyButtonLabel.attachToComponent(&overEasyButton, false); // 'false' is to put it on top
    addAndMakeVisible(overEasyButtonLabel);
    
    //------------------KNOB SETTINGS -----------------
    inputKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    inputKnob.addListener(this);
    inputKnob.setSize(largeSliderSize,largeSliderSize);
    inputKnob.setPipCount(14);
    inputKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(inputKnob);
    
    inputLabel.setText("Input Gain", dontSendNotification);
    inputLabel.setFont(HackAudio::Fonts::NowRegular);
    inputLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    inputLabel.setJustificationType(Justification::centred);
    inputLabel.attachToComponent(&inputKnob, false); // 'false' is to put it on top
    addAndMakeVisible(inputLabel);
    
    //Settings for the ratioKnob
    ratioKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    ratioKnob.addListener(this);
    ratioKnob.setSize(smallSliderSize,smallSliderSize);
    ratioKnob.setPipCount(9);
    ratioKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(ratioKnob);
    
    ratioLabel.setText("Ratio", dontSendNotification);
    ratioLabel.setFont(HackAudio::Fonts::NowRegular);
    ratioLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    ratioLabel.setJustificationType(Justification::centred);
    ratioLabel.attachToComponent(&ratioKnob, false); // 'false' is to put it on top
    addAndMakeVisible(ratioLabel);
    
    //Settings for the threshold knob
    thresholdKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    thresholdKnob.addListener(this);
    thresholdKnob.setSize(largeSliderSize,largeSliderSize);
    thresholdKnob.setPipCount(9);
    thresholdKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(thresholdKnob);
 
    thresholdLabel.setText("Threshold", dontSendNotification);
    thresholdLabel.setFont(HackAudio::Fonts::NowRegular);
    thresholdLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    //gainLabel.setFont(Font & "Raleway");
    thresholdLabel.setJustificationType(Justification::centred);
    thresholdLabel.attachToComponent(&thresholdKnob, false); // 'false' is to put it on top
    addAndMakeVisible(thresholdLabel);
    
    //Settings for the crossoverKnob
    attackKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    attackKnob.addListener(this);
    attackKnob.setSize(smallSliderSize,smallSliderSize);
    attackKnob.setPipCount(9);
    attackKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(attackKnob);
    
    //Settings for attack knob
    attackLabel.setText("Attack", dontSendNotification);
    attackLabel.setFont(HackAudio::Fonts::NowRegular);
    attackLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    attackLabel.setJustificationType(Justification::centred);
    attackLabel.attachToComponent(&attackKnob, false); // 'false' is to put it on top
    addAndMakeVisible(attackLabel);
    
    //Settings for the release switch
    releaseKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    releaseKnob.addListener(this);
    releaseKnob.setSize(smallSliderSize,smallSliderSize);
    releaseKnob.setPipCount(9);
    releaseKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(releaseKnob);
    
    releaseLabel.setText("Release", dontSendNotification);
    releaseLabel.setFont(HackAudio::Fonts::NowRegular);
    releaseLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    releaseLabel.setJustificationType(Justification::centred);
    releaseLabel.attachToComponent(&releaseKnob, false); // 'false' is to put it on top
    addAndMakeVisible(releaseLabel);
    
    
    //Settings for the outputKnob
    outputKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    outputKnob.addListener(this);
    outputKnob.setSize(largeSliderSize,largeSliderSize);
    outputKnob.setPipCount(14);
    outputKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(outputKnob);
    
    outputLabel.setText("Output Gain", dontSendNotification);
    outputLabel.setFont(HackAudio::Fonts::NowRegular);
    outputLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    outputLabel.setJustificationType(Justification::centred);
    outputLabel.attachToComponent(&outputKnob, false); // 'false' is to put it on top
    addAndMakeVisible(outputLabel);
    
    mixKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mixKnob.addListener(this);
    mixKnob.setSize(largeSliderSize,largeSliderSize);
    mixKnob.setPipCount(12);
    mixKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(mixKnob);
    
    mixLabel.setText("Wet/Dry Mix", dontSendNotification);
    mixLabel.setFont(HackAudio::Fonts::NowRegular);
    mixLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    mixLabel.setJustificationType(Justification::centred);
    mixLabel.attachToComponent(&mixKnob, false); // 'false' is to put it on top
    addAndMakeVisible(mixLabel);
    
    //--------------METER----------------
    grMeter.setMeterStyle(HackAudio::Meter::MeterStyle::Horizontal);
    grMeter.setMeterCalibration(HackAudio::Meter::MeterCalibration::VU);
    grMeter.setPeakStatus(false);
    grMeter.setSize(200, 30);
    grMeter.setColour(HackAudio::highlightColourId, juce::Colours::black);
    grMeter.setColour(HackAudio::backgroundColourId, juce::Colours::orange);
    grMeter.setPipState(false);
    grMeter.setSource(1, &processor.gainChange_meter);
    addAndMakeVisible(grMeter);
    
    flexMeter.addComponent(grMeter);
    flexMeter.applyBounds(juce::Rectangle<int>(300,310,210,30));
    
    //-------------------FLEX BOX ITEMS ----------------
    //Flex box for input source button
    flexButtons.addComponent(fetCompToggle);
    flexButtons.addComponent(opticalCompToggle);
    flexButtons.addComponent(tubeCompToggle);
    flexButtons.addComponent(vcaCompToggle);
    
    //flexButtons.setJustifyContent(juce::FlexBox::JustifyContent::center);
    flexButtons.applyBounds(juce::Rectangle<int>(280,60,260,40));
    
    
    // Flex box for ratio toggle
    
    flexRatioToggle.addComponent(ratioFourToggle);
    flexRatioToggle.addComponent(ratioEightToggle);
    flexRatioToggle.addComponent(ratioTwelveToggle);
    flexRatioToggle.addComponent(ratioTwentyToggle);
    flexRatioToggle.addComponent(ratioAllToggle);
    flexRatioToggle.applyBounds(juce::Rectangle<int>(300,260,ratioKnobWidth*5,ratioKnobHeight));
    
    //Flex box for knobs
    flexKnobsCompressor.addComponent(inputKnob);
    flexKnobsCompressor.addComponent(outputKnob);
    flexKnobsCompressor.addComponent(attackKnob);
    flexKnobsCompressor.addComponent(releaseKnob);
    flexKnobsCompressor.addComponent(ratioKnob);
    
    flexKnobsCompressor.addComponent(thresholdKnob);
    flexKnobsCompressor.addComponent(mixKnob);
    
    flexKnobsCompressor.addComponent(overEasyButton);
    
    flexKnobsCompressor.applyBounds(juce::Rectangle<int>(170,140,(3*largeSliderSize)+(3*smallSliderSize),85));

    //Display////////////////////////
    //
    
    //    startTimer(50);
    //
    //    port.setBounds(0,250,guiWidth,350);
    //
    //    port.setDiagram(biquadDiagram);
    //
    //    addAndMakeVisible(port);
    
    //---------------------------- ATTACHMENTS --------------------------------
    
    thresholdAttachment = std::make_unique<SliderAttachment>(processor.treeState,"threshold", thresholdKnob);
    
    ratioAttachment = std::make_unique<SliderAttachment>(processor.treeState,"ratio", ratioKnob);
    
    outputAttachment = std::make_unique<SliderAttachment>(processor.treeState,"output", outputKnob);
    inputAttachment = std::make_unique<SliderAttachment>(processor.treeState,"input", inputKnob);
    
    mixAttachment = std::make_unique<SliderAttachment>(processor.treeState,"mix", mixKnob);
    releaseAttachment = std::make_unique<SliderAttachment>(processor.treeState,"release", releaseKnob);
    attackAttachment = std::make_unique<SliderAttachment>(processor.treeState,"attack", attackKnob);
    
    //updateFilterButtonSelected();
    
    
    //---------------------------- ATTACHMENTS --------------------------------
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (guiWidth, guiHeight);
}

IconicCompressor_betaAudioProcessorEditor::~IconicCompressor_betaAudioProcessorEditor()
{
}

//==============================================================================
void IconicCompressor_betaAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    // htmlcolorcodes.com/color-names/
    g.setColour (Colours::firebrick); //rebeccapurple
    g.setFont (15.0f);
    
    Font titleFont = HackAudio::Fonts::NowBold;
    titleFont.setHeight(30);
    
    g.setFont(titleFont);
    //g.setColour (juce::Colours::blue);
    g.drawFittedText ("iconic",0, 20, 740, 25, Justification::centred, 1.0);
    g.setColour (juce::Colours::orange);//g.setColour (Colours::cyan);
    g.drawFittedText ("analog",0, 20, 970, 25, Justification::centred, 1.0);
    
    guiTab(processor.defaultKnobView);

}

void IconicCompressor_betaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void IconicCompressor_betaAudioProcessorEditor::buttonClicked(Button* button)
{
    
    //---------WAVETYPE VALUES-------------------
    if (button == &fetCompToggle) {
        processor.defaultKnobView = 0;
        guiTab(processor.defaultKnobView);
    }
    if (button == &opticalCompToggle){
        processor.defaultKnobView = 1;
        guiTab(processor.defaultKnobView);
    }
    if(button == &vcaCompToggle){
        processor.defaultKnobView = 2;
        guiTab(processor.defaultKnobView);
    }
    if(button == &tubeCompToggle){
        processor.defaultKnobView = 3;
        guiTab(processor.defaultKnobView);
    }
}
void IconicCompressor_betaAudioProcessorEditor::sliderValueChanged(Slider* slider){
    //-----LABEL CHANGES, UPDATES THE KNOB LABELS WHEN USER CHANGES A PARAMETER---------
    if (!isShowing()) return;
    
    if (slider == &inputKnob) {
        inputLabel.setText(String(inputKnob.getValue(),1),dontSendNotification);
    }
    if (slider == &attackKnob) {
        attackLabel.setText(String(attackKnob.getValue(),1),dontSendNotification);
    }
    if (slider == &ratioKnob) {
        ratioLabel.setText(String(ratioKnob.getValue(),1),dontSendNotification);
    }
    if (slider == &thresholdKnob) {
        thresholdLabel.setText(String(thresholdKnob.getValue(),1),dontSendNotification);
    }
    
    if (slider == &releaseKnob) {
        releaseLabel.setText(String(releaseKnob.getValue(),1),dontSendNotification);
    }
    if (slider == &outputKnob) {
        outputLabel.setText(String(outputKnob.getValue(),1),dontSendNotification);
    }
    
    //Starts a timer, after it expires, resets the labels to default values
    startTimer(1200);
}

void IconicCompressor_betaAudioProcessorEditor::timerCallback()
{
    stopTimer();
    inputLabel.setText("Input", dontSendNotification);
    attackLabel.setText("Attack", dontSendNotification);
    releaseLabel.setText("Release", dontSendNotification);
    thresholdLabel.setText("Threshold", dontSendNotification);
    ratioLabel.setText("Ratio", dontSendNotification);
    outputLabel.setText("Output", dontSendNotification);
    mixLabel.setText("Wet/Dry Mix", dontSendNotification);
    
}

void IconicCompressor_betaAudioProcessorEditor::guiTab(int &selectedView){
    if (selectedView == 0) {
        
        fetCompToggle.setToggleState(true, dontSendNotification);
        opticalCompToggle.setToggleState(false, dontSendNotification);
        tubeCompToggle.setToggleState(false, dontSendNotification);
        vcaCompToggle.setToggleState(false, dontSendNotification);
        
        inputKnob.setColour(HackAudio::foregroundColourId, juce::Colours::azure);
        ratioKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
        attackKnob.setColour(HackAudio::foregroundColourId, juce::Colours::azure);
        releaseKnob.setColour(HackAudio::foregroundColourId, juce::Colours::azure);
        outputKnob.setColour(HackAudio::foregroundColourId, juce::Colours::azure);
        
        inputKnob.setVisible(true);
        outputKnob.setVisible(true);
        
        attackKnob.setVisible(true);
        releaseKnob.setVisible(true);
        
        ratioFourToggle.setVisible(true);
        ratioEightToggle.setVisible(true);
        ratioTwelveToggle.setVisible(true);
        ratioTwentyToggle.setVisible(true);
        ratioAllToggle.setVisible(true);
        
        ratioKnob.setVisible(false);
        
        mixKnob.setVisible(false);
        thresholdKnob.setVisible(false);
        overEasyButton.setVisible(false);
        
    }
    
      if (selectedView == 1) {
          fetCompToggle.setToggleState(false, dontSendNotification);
          opticalCompToggle.setToggleState(true, dontSendNotification);
          tubeCompToggle.setToggleState(false, dontSendNotification);
          vcaCompToggle.setToggleState(false, dontSendNotification);
          
          attackKnob.setVisible(false);
          releaseKnob.setVisible(false);
          ratioKnob.setVisible(false);
          
          mixKnob.setVisible(false);
          overEasyButton.setVisible(false);
          
          inputKnob.setVisible(false);
          thresholdKnob.setVisible(false);
          outputKnob.setVisible(false);
          
          ratioTwentyToggle.setVisible(false);
          ratioTwelveToggle.setVisible(false);
          ratioEightToggle.setVisible(false);
          ratioFourToggle.setVisible(false);
          ratioAllToggle.setVisible(false);
          
          inputKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
          thresholdKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
          outputKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
          
          inputKnob.setVisible(true);
          thresholdKnob.setVisible(true);
          outputKnob.setVisible(true);
          
      }
     if (selectedView == 2) {
         fetCompToggle.setToggleState(false, dontSendNotification);
         opticalCompToggle.setToggleState(false, dontSendNotification);
         vcaCompToggle.setToggleState(true, dontSendNotification);
         tubeCompToggle.setToggleState(false, dontSendNotification);
         
         
         inputKnob.setVisible(false);
         attackKnob.setVisible(false);
         releaseKnob.setVisible(false);
         mixKnob.setVisible(false);
         
         thresholdKnob.setVisible(false);
         overEasyButton.setVisible(false);
         ratioKnob.setVisible(false);
         outputKnob.setVisible(false);
         
         ratioTwentyToggle.setVisible(false);
         ratioTwelveToggle.setVisible(false);
         ratioEightToggle.setVisible(false);
         ratioFourToggle.setVisible(false);
         ratioAllToggle.setVisible(false);
         
         thresholdKnob.setColour(HackAudio::foregroundColourId, juce::Colours::firebrick);
         ratioKnob.setColour(HackAudio::foregroundColourId, juce::Colours::skyblue);
         outputKnob.setColour(HackAudio::foregroundColourId, juce::Colours::seagreen);
         
         thresholdKnob.setVisible(true);
         overEasyButton.setVisible(true);
         ratioKnob.setVisible(true);
         outputKnob.setVisible(true);
     }
    if(selectedView == 3){
        fetCompToggle.setToggleState(false, dontSendNotification);
        opticalCompToggle.setToggleState(false, dontSendNotification);
        vcaCompToggle.setToggleState(false, dontSendNotification);
        tubeCompToggle.setToggleState(true, dontSendNotification);
        
        outputKnob.setVisible(false);
        ratioKnob.setVisible(false);
        mixKnob.setVisible(false);
        overEasyButton.setVisible(false);
        
        inputKnob.setVisible(false);
        thresholdKnob.setVisible(false);
        attackKnob.setVisible(false);
        releaseKnob.setVisible(false);
        
        ratioTwentyToggle.setVisible(false);
        ratioTwelveToggle.setVisible(false);
        ratioEightToggle.setVisible(false);
        ratioFourToggle.setVisible(false);
        ratioAllToggle.setVisible(false);
        
        inputKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
        thresholdKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
        attackKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
        releaseKnob.setColour(HackAudio::foregroundColourId, juce::Colours::black);
        
        inputKnob.setVisible(true);
        thresholdKnob.setVisible(true);
        attackKnob.setVisible(true);
        releaseKnob.setVisible(true);
    }
}
