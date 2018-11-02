//
//  Compressor.h
//  IconicCompressor_Beta
//
//  Created by Brian Cofer on 10/26/18.
//

#ifndef Compressor_h
#define Compressor_h
/*
 4 classes defined:
 multiband: takes input sample and if desired, splits into a treble part and bass part. Still in development.
 levelDetector: determines input level, default unit is dB, can also determine linear. Will also (I think) have options for various detector types
 gainComputer: applies attack/release parameters
 compressor: pulls everything together. Future versions will allow the level detector and gain computer blocks to be reversed. Still thinking this through.
 

 */

class multiband
{
public:
    enum bandType
    {
        NORMAL,
        BASS,
        TREBLE,
        MULTI
    };
    
    multiband(bandType type = bandType::NORMAL) {
        //initialize RBJFILTER for splitting the signal into multiple parts.
        trebleOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::HIGHPASS), 1, 44100);
        bassOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::LOWPASS), 1, 44100);
    }
    ~multiband() {}
    
    float splitInputSample(float inputSample, float channel){
        
        if (type == NORMAL){
            // do nothing
            return inputSample;
        }
        else if  (type == BASS)  {
            //run the signal through a filter to split into halves
            
            bassOutput->SetCutoff(crossoverFreq);
            trebleOutput->SetCutoff(crossoverFreq);
            bassOutput->SetQValue(.707);
            trebleOutput->SetQValue(.707);
            trebleOutput->SetSamplingRate(samplingRate);
            bassOutput->SetSamplingRate(samplingRate);
            
            inputNotCompressed = trebleOutput-> Tick(inputSample, channel); //TODO 1 is not correct
            return bassOutput->Tick(inputSample, channel);
        }
        else if (type == TREBLE) {
            
            bassOutput->SetCutoff(crossoverFreq);
            trebleOutput->SetCutoff(crossoverFreq);
            bassOutput->SetQValue(.707);
            trebleOutput->SetQValue(.707);
            trebleOutput->SetSamplingRate(samplingRate);
            bassOutput->SetSamplingRate(samplingRate);
            
            return trebleOutput-> Tick(inputSample,channel); //TODO 1 is not correct
            inputNotCompressed = bassOutput->Tick(inputSample,channel);
        }
        else { //true MULTIBAND options will go here in future versions
            return inputSample;
        }
        
    }
    
    
    float combineIO(float linA){
        switch(type)
        {
            case NORMAL:
            {
                return linA;
            }break;
            case BASS:
            {
                return linA + inputNotCompressed;
            }break;
            case TREBLE:
            {
                return linA + inputNotCompressed;
            }break;
            case MULTI:
            {
                return linA;
            }break;
        }
    }
    
    void setSamplingRate(float Fs){
        samplingRate = Fs;
    }
    void setCrossoverFrequency(float crossoverFrequency){
        crossoverFreq = crossoverFrequency;
    }
protected:
    RBJFilter* trebleOutput;
    RBJFilter* bassOutput;
    bandType type = bandType::NORMAL;
    float crossoverFreq;
    float samplingRate;
    float inputNotCompressed;
    
};

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
    
    
    levelDetector(detectorUnit u = detectorUnit::DB) : u(u) {
        
    }
    
    ~levelDetector(){}

    
    float process(float inputSample) {
        float inputLevel;
        
        switch (u)
        {
            case LINEAR:
            {
                inputLevel =  abs(inputSample);
                
                if (inputLevel < -1) {
                    inputLevel = -1;
                }
            } break;
                
            case DB:
            {
                inputLevel =  20.f * log10(abs(inputSample)/1.f);
                
                if (inputLevel < -96) {
                    inputLevel = -96;
                }
            } break;
        }
        return inputLevel;
    }
    
    void setDetectorUnit(detectorUnit unit){
        u = unit;
    }
    void setDetectorType(detectorType type){
        t = type;
    }
    
protected:
    detectorUnit u;
    detectorType t;
};

class gainComputer
{
public:
    gainComputer() {}
    ~gainComputer() {}
    
    float process(float inputLevel){
        float gainSC;
        
        if (inputLevel > thresholdValue) {
            gainSC = thresholdValue + (inputLevel - thresholdValue) / ratioValue; //Perform Downwards Compression
        }
        else {
            gainSC = inputLevel; // Do not perform compression
        }
        
        return gainSC - inputLevel;
        
    }
    
    void setThreshold(float t){
        thresholdValue = t;
    }
    void setRatio(float r){
        ratioValue = r;
    }
    
protected:
    float thresholdValue;
    float ratioValue;
  
};

class gainSmoothing
{
public:
    gainSmoothing() {}
    ~gainSmoothing(){}
    
    float process(float gainChange_dB, int channel){
        
        if(gainChange_dB < gainSmoothPrevious[channel]) {
            //attack
            gainSmooth = (  ((1-alphaA) * gainChange_dB) + (alphaA*gainSmoothPrevious[channel]) ) ;
            
        }
        else {
            //release
            gainSmooth = ( ((1-alphaR) * gainChange_dB) + (alphaR*gainSmoothPrevious[channel]) ) ;
        }
        
        //Update gainSmoothPrev for the next iteration
        gainSmoothPrevious[channel] = gainSmooth;
        
        return gainSmooth;
        
    }
    
    void setAttack(float attackTime){
        alphaA = exp(-log(9) / (sampleRate * (attackTime / 1000) ));
    }
    void setRelease(float releaseTime){
        alphaR = exp(-log(9) / (sampleRate * (releaseTime / 1000) ));
    }
    void setSamplingRate(float Fs){
        sampleRate = Fs;
    }

protected:
    float alphaA;
    float alphaR;
    float sampleRate;
    float gainSmooth;
    float gainSmoothPrevious[2]; // _TODO make the channel count dynamic in case > 2 channels.
    
};

class sidechain {
    
public:
    sidechain(){
        lowCut = new RBJFilter(RBJFilter::FilterType(RBJFilter::HIGHPASS), 1, 44100);
        highCut = new RBJFilter(RBJFilter::FilterType(RBJFilter::LOWPASS), 1, 44100);
      
        lowCut->SetQValue(.707);
        highCut->SetQValue(.707);
        
        thisLevelDetector = new levelDetector();
        
    }
    ~sidechain(){
        delete thisLevelDetector;
        delete highCut;
        delete lowCut;
    }
    
    void setHighCutoff(float f){
        highCut->SetCutoff(f);
    }
    void setLowCutoff(float f){
        lowCut->SetCutoff(f);
    }
    void setQ(float q){

    }
    
    float process(float inputSample, int channel){
        //take in the side chain
        //apply high & low cut

        float low = lowCut->Tick(inputSample, channel);
        float high =  highCut->Tick(low, channel);
        
        return thisLevelDetector->process(high); //get input level, default is DB. Can be set to linear manually.
        
    }
   

        void setSamplingRate(float s){
            lowCut->SetSamplingRate(s);
            highCut->SetSamplingRate(s);
        }
    void setDetectorUnit(levelDetector::detectorUnit newUnit){
        dUnit = newUnit;
    }
    void setDetectorType(levelDetector::detectorType newType){
        dType = newType;
    }
    
protected:

    RBJFilter* highCut;
    RBJFilter* lowCut;
    levelDetector* thisLevelDetector;
    levelDetector::detectorUnit dUnit;
    levelDetector::detectorType dType;
    
};


class compressor {
    
public:
    
    compressor(){
        //initialize required subclasses
        thisSidechain = new sidechain();
        thisMultiband = new multiband(); //can we "if" this somehow _TODO
      
        thisGainComputer = new gainComputer();
        thisGainSmooth = new gainSmoothing();
    }
    ~compressor(){
        delete thisMultiband;
        delete thisGainComputer;
        delete thisGainSmooth;
        delete thisSidechain;
    }
    
    float tick(float input, int channel){
    
        float inputSample = thisMultiband->splitInputSample(input, channel); //send sample to the multiband processor. Returns input if no multiband option
        
        if (sidechainInput == 0) { //this should maybe be set a different way. Just a check to see if user has specified a different sidechain.
            setSidechainInput(inputSample);
        }
        else {
            setSidechainInput(inputSample);
        }
        
        //maybe some of this should be called within the sidechain class? START HERE
        
        //send the input to the sidechain class. This class handles any EQ done to the sidechain, then outputs the input level in dB or linear.
        float sidechainOutput = thisSidechain->process(sidechainInput, channel);
        
        float gainChange_dB = thisGainComputer->process(sidechainOutput); //determine necessary gain change (db)
        float gainSmooth = thisGainSmooth->process(gainChange_dB, channel); // apply attack/release times
        float linA = pow(10.0f,gainSmooth/20.0f); //convert back to linear. This will probably change in the future as blocks become moveable.
        float outputSample = thisMultiband->combineIO(linA); //send to multiband; returns input if no MB option set, otherwise, combine signals as needed.
        
        return outputSample * input; //final step; multiply the calculated output value with the input sample to get compressed output
   
    }

    void setCurrentSample(float currentSample) { inputSample = currentSample; }
    float getCurrentSample() { return inputSample; }
    void SetSamplingRate(float Fs) {
        thisGainSmooth->setSamplingRate(Fs);
        thisMultiband->setSamplingRate(Fs);
        thisSidechain->setSamplingRate(Fs);
    }
    void setBandType(multiband::bandType newTypeB) {
        bType = newTypeB;
    }
    void setDetectorUnit(levelDetector::detectorUnit newUnit){
        //dUnit = newUnit;
        thisSidechain->setDetectorUnit(newUnit);
    }
    void setDetectorType(levelDetector::detectorType newType){
        thisSidechain->setDetectorType(newType);
    }
    
    //void setBandCount(int c) { bandCount = c; }
    void setCrossoverFrequency(float f) {
        crossoverFrequency = f;
        thisMultiband->setCrossoverFrequency(f);
    }
    
    void setThreshold(float t){
        thresholdValue = t;
        thisGainComputer->setThreshold(t);
    }
    void setRatio(float r){
        ratioValue = r;
        thisGainComputer->setRatio(r);
    }

    void setAttack(float attackTime){
        thisGainSmooth->setAttack(attackTime);
    }
    void setRelease(float releaseTime){
        thisGainSmooth->setRelease(releaseTime);
    }
    void setCurrentChannel(int c) {
        channel = c;
    }
    void setSidechainInput(float i){
        sidechainInput = i;
    }
    void setHighCutoff(float hc){
        thisSidechain->setHighCutoff(hc);
    }
    void setLowCutoff(float lc){
        thisSidechain->setLowCutoff(lc);
    }

protected:
    multiband* thisMultiband;
    gainComputer* thisGainComputer;
    gainSmoothing* thisGainSmooth;
    sidechain* thisSidechain;
    
    float sampleRate;
    float thresholdValue;
    float ratioValue;
    
    float inputSample;
    float outputSample;
    
    float crossoverFrequency;
    int channel;
    float sidechainInput = 0;
    
    multiband::bandType bType;
    
    
};
#endif /* Compressor_h */
