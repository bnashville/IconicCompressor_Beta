//
//  Compressor.h
//  IconicCompressor_Beta
//
//  Created by Brian Cofer on 10/23/18.
//

#ifndef Compressor_h
#define Compressor_h

#include "PluginProcessor.h"
class levelDetector
{
public:
   
    enum detectorUnit
    {
        LINEAR,
        DB
    };
    
    enum detectorType
    {
        PEAK,
        RMS,
        LCP,
        SMOOTH
    };

    levelDetector(detectorUnit unit = detectorUnit::DB, float inputSample = 0) : u(unit) {
        setInputLevel(inputSample);
    }
    
    ~levelDetector() {}
    
    void setDetectorUnit(detectorUnit newUnit)
    {
        u = newUnit;
    }
    
    detectorUnit GetType()
    {
        return u;
    }
    
    void setInputLevel(float s) {

        switch (u)
        {
            case LINEAR:
            {
                inputLevel =  abs(s);
                
                if (inputLevel < -1) {
                    inputLevel = -1;
                }
            } break;
                
            case DB:
            {
                inputLevel =  20.f * log10(abs(s)/1.f);
                
                if (inputLevel < -96) {
                    inputLevel = -96;
                }
            } break;
        }
        

    }
    
    float getInputLevel() {
        return inputLevel;
    }
    
    private:
        detectorUnit u;
        float inputLevel;
    
        
    };



class gainComputer
{
public:
    gainComputer() {}
    ~gainComputer() {}
    
     void setThreshold(float t){
     thresholdValue = t;
     }
     void setRatio(float r){
     ratioValue = r;
     }

    void setInputLevel(float i) {
        inputLevel = i;
        
    }
    void setStaticChar(){
        if (inputLevel > thresholdValue) {
            gainSC = thresholdValue + (inputLevel - thresholdValue) / ratioValue; //Perform Downwards Compression
        }
        else {
            gainSC = inputLevel; // Do not perform compression
        }
        
        gainChange_dB = gainSC - inputLevel;
        
    }
    float getGainChange(){
        return gainChange_dB;
    }
protected:
    float gainSC;
    
    
private:
    float thresholdValue; // can this be inherited from compressor?
    float ratioValue;
    float inputLevel;
    float gainChange_dB;
};


class gainSmoothing
{
public:
    
    
    gainSmoothing() {

            // smooth over the gainChange
     
    }
    
    ~gainSmoothing() {}
    
    void setAlphaA(float alphaA){
        alphaAttack = alphaA;
    }
    void setAlphaR(float alphaR){
        alphaRelease = alphaR;
    }
    
    void setGainSmooth(float gainChange_dB = 0, float thisSample = 0) {
        
        if(gainChange_dB < gainSmoothPrevious) {
            //attack
            
            gainSmooth = (  ((1-alphaAttack) * thisSample) + (alphaAttack*gainSmoothPrevious) ) ;
            
        }
        else {
            //release
            gainSmooth = ( ((1-alphaRelease) * thisSample) + (alphaRelease*gainSmoothPrevious) ) ;
        }
        
        //Convert to linear amplitude scalar _TODO not sure if I want to do this here
        linA = pow(10,gainSmooth/20);

       //Update gainSmoothPrev used in the next sample of the loop
        gainSmoothPrevious = gainSmooth;
        
    }
    float getLinA(){
        return linA;
    }
    
protected:
    float gainSmooth;
    float gainSmoothPrevious = 0;
    float alphaAttack;
    float alphaRelease;
    float linA;
};


class compressor {
    
    enum bandType
    {
        NORMAL,
        BASS,
        TREBLE,
        MULTI
    };
    
public:
    compressor() {
        //standard vs multiband compressor
        setSplitInput(getBandType());

    }
    
    ~compressor() {
        
    }
    
    void process() {
        
    }
    
    float tick(float input) {

        //if bandtype is split, need to reassign my input to only one band, then sum later
        setCurrentSample(input);
        getSplitInput(inputSample);
        
        setInputLevel(inputSample);
        setGainComputer();
        setGainSmoothing();
        compressedOutput = thisGainSmooth->getLinA() * getCurrentSample();
        combineSplitOutput();
        return outputSample;
    }
    void combineSplitOutput(){
        switch(type)
        {
            case NORMAL:
            {
                
            }break;
            case BASS:
            {
                outputSample = compressedOutput + inputNotCompressed;
            }break;
            case TREBLE:
            {
                outputSample = compressedOutput + inputNotCompressed;
            }break;
            case MULTI:
            {
                
            }break;
        }
    }
    void setSplitInput(bandType type = bandType::NORMAL){
        
        switch (type)
        {
            case NORMAL:
            {
               //do nothing
                
            } break;
            case BASS:
            {
                //run the signal through a filter to split into halves
                trebleOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::HIGHPASS), 1, 44100);
                bassOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::LOWPASS), 1, 44100);
                bassOutput->SetCutoff(getCrossoverFrequency());
                trebleOutput->SetCutoff(getCrossoverFrequency());
            } break;
            case TREBLE:
            {
                trebleOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::HIGHPASS), 1, 44100);
                bassOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::LOWPASS), 1, 44100);
                bassOutput->SetCutoff(getCrossoverFrequency());
                trebleOutput->SetCutoff(getCrossoverFrequency());
            } break;
            case MULTI:
            {
                
            } break;
                
        }
        

    }
    
    void getSplitInput(float i) {
        
        
        switch(type)
        {
            case NORMAL:
            {
                
            }break;
            case BASS:
            {
                inputNotCompressed = trebleOutput-> Tick(i, channel); //TODO 1 is not correct
                inputSample = bassOutput->Tick(i, channel);
            }break;
            case TREBLE:
            {
                inputSample = trebleOutput-> Tick(i,channel); //TODO 1 is not correct
                inputNotCompressed = bassOutput->Tick(i,channel);
            }break;
            case MULTI:
            {
                
            }break;
        }
        
    }
    
    void setInputLevel(float inputLevel){
        thisLevelDetector->setInputLevel(inputLevel);
        xdB = thisLevelDetector->getInputLevel();
        thisGainComputer->setInputLevel(thisLevelDetector->getInputLevel());
    }
    
    void setGainComputer() {
        thisGainComputer->setStaticChar();
        
    }
    void setGainSmoothing(){
        thisGainSmooth->setGainSmooth(thisGainComputer->getGainChange(), xdB);
    }

    void setAlphaA(float attackTime){
        alphaA = exp(-log(9) / (sampleRate * (attackTime / 1000) ));
        thisGainSmooth->setAlphaA(alphaA);
    }
    float getAlphaA() {
        return alphaA;
    }
    void setAlphaR(float releaseTime){
        alphaR = exp(-log(9) / (sampleRate * (releaseTime / 1000) ));
        thisGainSmooth->setAlphaR(alphaR);
    }
    float getAlphaR(){
        return alphaR;
    }
    void setThreshold(float threshold){
        thresholdValue = threshold; // do I need the threshold anywhere other than static char?
        thisGainComputer->setThreshold(threshold);
    }
    float getThreshold(){
        return thresholdValue;
    }
    void setRatio(float ratio){
        ratioValue = ratio; // do I need the ratio anywhere other than static char?
        thisGainComputer->setRatio(ratio);
    }
    float getRatio() {
        return ratioValue;
    }
    void setDetectorUnit(levelDetector::detectorUnit newUnit) {

        thisLevelDetector->setDetectorUnit(newUnit);

    }

    float getInputLevel() {
        return thisLevelDetector->getInputLevel();
        
    }
  
  void setCurrentSample(float currentSample){
        inputSample = currentSample;
    }
    float getCurrentSample() {
        return inputSample;
    }
  
    void SetSamplingRate(float Fs)
    {
        sampleRate = Fs;
        
    }
    
    void setBandType(bandType newType){
        type = newType;
    }
    bandType getBandType(){
        return type;
    }
    void setBandCount(int c) {
        bandCount = c;
    }
    
    void setCrossoverFrequency(float f){
        crossoverFrequency = f;
    }
    float getCrossoverFrequency(){
        return crossoverFrequency;
    }
    void setChannelCount(int c){
        channelCount = c;
    }
    void setCurrentChannel(int c) {
        channel = c;
    }
    
protected:
    float alphaA;
    float alphaR;
    float thresholdValue;
    float ratioValue;
    
    float inputSample;
    float previousSample;
    float sampleRate;
    float gainChange_dB;
    float inputLevel;
    float outputSample;
    float compressedOutput;
    
    float xdB;
    float trebleOut;
    float bassOut;
    int channelCount = 1;
    int channel = 1;
    float inputNotCompressed;
    
    levelDetector* thisLevelDetector;
    gainComputer* thisGainComputer;
    gainSmoothing* thisGainSmooth;
    
    bandType type;
    int bandCount;
    float crossoverFrequency;
    RBJFilter* trebleOutput;
    RBJFilter* bassOutput;
    
};


#endif /* Compressor_h */
