/*
  ==============================================================================

    This file was auto-geneinputd!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
IconicCompressor_betaAudioProcessorEditor::IconicCompressor_betaAudioProcessorEditor (IconicCompressor_betaAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    // order: smoothing before static charastics, or v/v
    // smoothing method: peak, rms approximation, rms exact, 2nd order smoothing function (biquad), feed forward (straight), feed-back (straight), mixed (ff & fb)
    

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //Settings for the input

    sidechainKnobsButton.addListener(this);
    sidechainKnobsButton.setButtonText("Sidechain");
    sidechainKnobsButton.setConnectedEdges(Button::ConnectedOnLeft);
    sidechainKnobsButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    sidechainKnobsButton.setSize(80,30);
    sidechainKnobsButton.setRadioGroupId(4);
    
    addAndMakeVisible(sidechainKnobsButton);
    
    compressorKnobsButton.addListener(this);
    compressorKnobsButton.setButtonText("Compressor");
    compressorKnobsButton.setConnectedEdges(Button::ConnectedOnRight);
    compressorKnobsButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    compressorKnobsButton.setSize(80,30);
    compressorKnobsButton.setRadioGroupId(4);
    compressorKnobsButton.setToggleState(true, dontSendNotification);
    addAndMakeVisible(compressorKnobsButton);

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
    
    //Settings for the crossoverKnob
    crossoverKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    crossoverKnob.addListener(this);
    crossoverKnob.setSize(smallSliderSize,smallSliderSize);
    crossoverKnob.setPipCount(9);
    crossoverKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(crossoverKnob);
    
    crossoverLabel.setText("Crossover", dontSendNotification);
    crossoverLabel.setFont(HackAudio::Fonts::NowRegular);
    crossoverLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    crossoverLabel.setJustificationType(Justification::centred);
    crossoverLabel.attachToComponent(&crossoverKnob, false); // 'false' is to put it on top
    addAndMakeVisible(crossoverLabel);
    
    
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
    
    
    lowCutKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lowCutKnob.addListener(this);
    lowCutKnob.setSize(smallSliderSize,smallSliderSize);
    lowCutKnob.setPipCount(12);
    lowCutKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(lowCutKnob);
    
    lowCutLabel.setText("Low Cut", dontSendNotification);
    lowCutLabel.setFont(HackAudio::Fonts::NowRegular);
    lowCutLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    lowCutLabel.setJustificationType(Justification::centred);
    lowCutLabel.attachToComponent(&lowCutKnob, false); // 'false' is to put it on top
    addAndMakeVisible(lowCutLabel);
    
    highCutKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    highCutKnob.addListener(this);
    highCutKnob.setSize(smallSliderSize,smallSliderSize);
    highCutKnob.setPipCount(12);
    highCutKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(highCutKnob);
    
    highCutLabel.setText("High Cut", dontSendNotification);
    highCutLabel.setFont(HackAudio::Fonts::NowRegular);
    highCutLabel.setColour(juce::Label::textColourId, HackAudio::Colours::White);
    highCutLabel.setJustificationType(Justification::centred);
    highCutLabel.attachToComponent(&highCutKnob, false); // 'false' is to put it on top
    addAndMakeVisible(highCutLabel);
    
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
  
    
   
     // ------------------------TOGGLE BUTTONS, ROW 1, DIGITAL TYPE -----------------
    
    // Various settings for the perceptual wave button
    peakScButton.addListener(this);
    peakScButton.setButtonText("Peak");
    peakScButton.setConnectedEdges(Button::ConnectedOnRight);
    peakScButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    peakScButton.setSize(70,40);
    peakScButton.setRadioGroupId(5);
    peakScButton.setToggleState(true, dontSendNotification);
    addAndMakeVisible(peakScButton);
    
    rmsScButton.addListener(this);
    rmsScButton.setButtonText("RMS");
    rmsScButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    rmsScButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    rmsScButton.setSize(70,40);
    rmsScButton.setRadioGroupId(5);
    addAndMakeVisible(rmsScButton);
    
    levelCorrectedButton.addListener(this);
    levelCorrectedButton.setButtonText("Level-Corrected");
    levelCorrectedButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    levelCorrectedButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    levelCorrectedButton.setSize(120,40);
    levelCorrectedButton.setRadioGroupId(5);
    addAndMakeVisible(levelCorrectedButton);
    
    smoothButton.addListener(this);
    smoothButton.setButtonText("Smooth");
    smoothButton.setConnectedEdges(Button::ConnectedOnLeft);
    smoothButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    smoothButton.setSize(90,40);
    smoothButton.setRadioGroupId(5);
    addAndMakeVisible(smoothButton);

    
    // --------------FF/ FB -----------------
    feedBackScButton.setButtonText("Feed-Back");
    feedBackScButton.setConnectedEdges(Button::ConnectedOnRight);
    feedBackScButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    feedBackScButton.setSize(90,40);
    feedBackScButton.setRadioGroupId(6);
    addAndMakeVisible(feedBackScButton);
    
    feedForwardButton.setButtonText("Feed-Forward");
    feedForwardButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    feedForwardButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    feedForwardButton.setSize(90,40);
    feedForwardButton.setRadioGroupId(6);
    addAndMakeVisible(feedForwardButton);
    
    hybridButton.addListener(this);
    hybridButton.setButtonText("Hybrid");
    hybridButton.setConnectedEdges(Button::ConnectedOnLeft);
    hybridButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    hybridButton.setSize(90,40);
    hybridButton.setRadioGroupId(6);
    addAndMakeVisible(hybridButton);
    
    //----------PLACEMENT ------------------
    ldOrderButton.addListener(this);
    ldOrderButton.setButtonText("Level Detector First");
    ldOrderButton.setConnectedEdges(Button::ConnectedOnRight);
    ldOrderButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    ldOrderButton.setSize(140,40);
    ldOrderButton.setRadioGroupId(7);
    addAndMakeVisible(ldOrderButton);
    
    gcOrderButton.addListener(this);
    gcOrderButton.setButtonText("Gain Computer First");
    gcOrderButton.setConnectedEdges(Button::ConnectedOnLeft);
    gcOrderButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    gcOrderButton.setSize(140,40);
    gcOrderButton.setRadioGroupId(7);
    addAndMakeVisible(gcOrderButton);

    
    // ------------------------TOGGLE BUTTONS, ROW 2, COMPRESSION TYPE   -----------------
    
    
    // Various settings for the multi-band buttons
    normalTremButton.addListener(this);
    normalTremButton.setButtonText("Normal");
    normalTremButton.setConnectedEdges(Button::ConnectedOnRight);
    normalTremButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    normalTremButton.setSize(80,30);
    normalTremButton.setRadioGroupId(3);
    normalTremButton.setToggleState(true, dontSendNotification);
    addAndMakeVisible(normalTremButton);
    
    // Various settings for the normal tremolo treble only button
    trebleTremButton.addListener(this);
    trebleTremButton.setButtonText("Treble Only");
    trebleTremButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    trebleTremButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    trebleTremButton.setSize(100,30);
    trebleTremButton.setRadioGroupId(3);
    addAndMakeVisible(trebleTremButton);
    
    // Various settings for the normal tremolo bass only button
    bassTremButton.addListener(this);
    bassTremButton.setButtonText("Bass Only");
    bassTremButton.setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnLeft);
    bassTremButton.setButtonStyle(HackAudio::Button::ButtonStyle::BarToggle);
    bassTremButton.setSize(90,30);
    bassTremButton.setRadioGroupId(3);
    addAndMakeVisible(bassTremButton);

    
    //-------------------FLEX BOX ITEMS ----------------
    //Flex box for input source button
    flexButtons.addComponent(compressorKnobsButton);
    flexButtons.addComponent(sidechainKnobsButton);
    
    //flexButtons.setJustifyContent(juce::FlexBox::JustifyContent::center);
    flexButtons.applyBounds(juce::Rectangle<int>(380,60,160,40));
   
    // Flex Box for tremolo type Buttons
    flexTremType.addComponent(normalTremButton);
    flexTremType.addComponent(trebleTremButton);
    flexTremType.addComponent(bassTremButton);

    //flexTremType.setJustifyContent(juce::FlexBox::JustifyContent::center);
    flexTremType.applyBounds(juce::Rectangle<int>(500,140,270,40));
    
    //Flex box for knobs
    flexKnobsCompressor.addComponent(inputKnob);
    flexKnobsCompressor.addComponent(thresholdKnob);
    flexKnobsCompressor.addComponent(attackKnob);
    flexKnobsCompressor.addComponent(releaseKnob);
    flexKnobsCompressor.addComponent(ratioKnob);
    flexKnobsCompressor.addComponent(outputKnob);
    flexKnobsCompressor.addComponent(mixKnob);

    flexKnobsCompressor.applyBounds(juce::Rectangle<int>(100,140,(4*largeSliderSize)+(3*smallSliderSize)+knobSpacing,85));
    
    // Flex box for sidechain knobs
    flexKnobsSidechain.addComponent(lowCutKnob);
    flexKnobsSidechain.addComponent(highCutKnob);
    flexKnobsSidechain.addComponent(crossoverKnob);
    flexKnobsSidechain.applyBounds(juce::Rectangle<int>(150,140,(3*smallSliderSize)+knobSpacing,85));
   
    // Flex Box for analog Buttons
    flexLevelDetector.addComponent(peakScButton);
    flexLevelDetector.addComponent(rmsScButton);
    flexLevelDetector.addComponent(levelCorrectedButton);
    flexLevelDetector.addComponent(smoothButton);
    flexLevelDetector.applyBounds(juce::Rectangle<int>(125,250,350,40));

    flexSystemDesign.addComponent(feedBackScButton);
    flexSystemDesign.addComponent(feedForwardButton);
    flexSystemDesign.addComponent(hybridButton);
    flexSystemDesign.applyBounds(juce::Rectangle<int>(525,250,90*3,40));
    
    // flex order
    flexOrder.addComponent(ldOrderButton);
    flexOrder.addComponent(gcOrderButton);
    flexOrder.applyBounds(juce::Rectangle<int>(150,300,140*2,40));
    

    //---------------------------- ATTACHMENTS --------------------------------
    thresholdAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"threshold", thresholdKnob);
    crossoverAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"crossover", crossoverKnob);
    ratioAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"ratio", ratioKnob);
    outputAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"output", outputKnob);
    inputAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"input", inputKnob);
 
    releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"release", releaseKnob);
    attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"attack", attackKnob);
    lowCutAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"lowCut", lowCutKnob);
    highCutAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"highCut", highCutKnob);
    mixAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"mix", mixKnob);
    
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
    g.setColour (Colours::rebeccapurple); //firebrick
    g.setFont (15.0f);
    
    Font titleFont = HackAudio::Fonts::NowBold;
    titleFont.setHeight(30);
    
    g.setFont(titleFont);
    //g.setColour (juce::Colours::blue);
    g.drawFittedText ("iconic",0, 20, 740, 25, Justification::centred, 1.0);
    g.setColour (juce::Colours::mediumseagreen);//g.setColour (Colours::cyan);
    g.drawFittedText ("compressor",0, 20, 970, 25, Justification::centred, 1.0);
    
    if (processor.defaultKnobView == 0) {
        
        inputKnob.setVisible(true);
        thresholdKnob.setVisible(true);
        attackKnob.setVisible(true);
        releaseKnob.setVisible(true);
        ratioKnob.setVisible(true);
        outputKnob.setVisible(true);
        mixKnob.setVisible(true);
        
        
        lowCutKnob.setVisible(false);
        highCutKnob.setVisible(false);
        crossoverKnob.setVisible(false);
        
        normalTremButton.setVisible(false);
        trebleTremButton.setVisible(false);
        bassTremButton.setVisible(false);
        
        processor.defaultKnobView = 1;
    }
   
    if (processor.tremTypeAlgorithm == 0) {
         crossoverKnob.setEnabled(false);
    }

}

void IconicCompressor_betaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void IconicCompressor_betaAudioProcessorEditor::buttonClicked(Button* button)
{
  
    //---------WAVETYPE VALUES-------------------
    if (button == &compressorKnobsButton) {
        
        // need to remove analog toggle buttons & add digital buttons
        
        inputKnob.setVisible(true);
        thresholdKnob.setVisible(true);
        attackKnob.setVisible(true);
        releaseKnob.setVisible(true);
        ratioKnob.setVisible(true);
        outputKnob.setVisible(true);
        mixKnob.setVisible(true);
        
        
        lowCutKnob.setVisible(false);
        highCutKnob.setVisible(false);
        crossoverKnob.setVisible(false);
        
        normalTremButton.setVisible(false);
        trebleTremButton.setVisible(false);
        bassTremButton.setVisible(false);
        
    }
    if (button == &sidechainKnobsButton){
      
        inputKnob.setVisible(false);
        thresholdKnob.setVisible(false);
        attackKnob.setVisible(false);
        releaseKnob.setVisible(false);
        ratioKnob.setVisible(false);
        outputKnob.setVisible(false);
        mixKnob.setVisible(false);
       
        lowCutKnob.setVisible(true);
        highCutKnob.setVisible(true);
        crossoverKnob.setVisible(true);
        
        normalTremButton.setVisible(true);
        trebleTremButton.setVisible(true);
        bassTremButton.setVisible(true);
    }
    
    if (button == &peakScButton) {
        processor.sideChainAlgorithm = 0;
    }
    if (button == &rmsScButton) {
        processor.sideChainAlgorithm = 1;
    }
    if (button == &smoothButton) {
        processor.compressorAlgorithm = 0;
    }
   
    if (button == &feedBackScButton) {
        processor.compressorAlgorithm = 1;
    }
    if (button == &feedForwardButton) {
        processor.compressorAlgorithm = 2;
    }
    if (button == &hybridButton) {
        processor.compressorAlgorithm = 3;
    }

    //------------TREMTYPE VALUES----------------------
    
    if (button == &normalTremButton) {
        processor.tremTypeAlgorithm = 0;
        crossoverKnob.setEnabled(false);
    }
    if (button == &trebleTremButton) {
        processor.tremTypeAlgorithm = 1;
        crossoverKnob.setEnabled(true);
    }
    if (button == &bassTremButton) {
        processor.tremTypeAlgorithm = 2;
        crossoverKnob.setEnabled(true);
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
    if (slider == &crossoverKnob) {
        crossoverLabel.setText(String(crossoverKnob.getValue(),1),dontSendNotification);
    }
    if (slider == &releaseKnob) {
        releaseLabel.setText(String(releaseKnob.getValue(),1),dontSendNotification);
    }
    if (slider == &outputKnob) {
        outputLabel.setText(String(outputKnob.getValue(),1),dontSendNotification);
    }
    if (slider == &lowCutKnob) {
        lowCutLabel.setText(String(lowCutKnob.getValue(),1),dontSendNotification);
        
    }
    if (slider == &highCutKnob) {
        highCutLabel.setText(String(highCutKnob.getValue(),1),dontSendNotification);
        
    }
    //Starts a timer, after it expires, resets the labels to default values
    startTimer(1200);
}

void IconicCompressor_betaAudioProcessorEditor::timerCallback()
{
    stopTimer();
    inputLabel.setText("input", dontSendNotification);
    attackLabel.setText("attack", dontSendNotification);
    releaseLabel.setText("release", dontSendNotification);
    thresholdLabel.setText("threshold", dontSendNotification);
    ratioLabel.setText("ratio", dontSendNotification);
    crossoverLabel.setText("Crossover", dontSendNotification);
    outputLabel.setText("output", dontSendNotification);
    lowCutLabel.setText("Low Cut", dontSendNotification);
    highCutLabel.setText("High Cut", dontSendNotification);
    mixLabel.setText("Wet/Dry Mix", dontSendNotification);
    
}

