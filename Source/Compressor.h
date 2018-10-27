//
//  Compressor.h
//  IconicCompressor_Beta
//
//  Created by Brian Cofer on 10/26/18.
//

#ifndef Compressor_h
#define Compressor_h

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
    
protected:
    detectorUnit u;
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

class compressor {
    
public:
    
    compressor(){
        //initialize required subclasses
        thisMultiband = new multiband(); //can we "if" this somehow _TODO
        thisLevelDetector = new levelDetector();
        thisGainComputer = new gainComputer();
        thisGainSmooth = new gainSmoothing();
    }
    ~compressor(){
        delete thisMultiband;
        delete thisLevelDetector;
        delete thisGainComputer;
        delete thisGainSmooth;
    }
    
    float tick(float input, int channel){
    
        float inputSample = thisMultiband->splitInputSample(input, channel); //send sample to the multiband processor. Returns input if no multiband option
        float inputLevel = thisLevelDetector->process(inputSample); //get input level, default is DB. Can be set to linear manually.
        float gainChange_dB = thisGainComputer->process(inputLevel); //determine necessary gain change (db)
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
    }
    void setBandType(multiband::bandType newTypeB) {
        bType = newTypeB;
    }
    void setDetectorUnit(levelDetector::detectorUnit newUnit){
        dUnit = newUnit;
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
    
protected:
    multiband* thisMultiband;
    levelDetector* thisLevelDetector;
    gainComputer* thisGainComputer;
    gainSmoothing* thisGainSmooth;
    
    float sampleRate;
    float thresholdValue;
    float ratioValue;
    
    float inputSample;
    float outputSample;
    
    float crossoverFrequency;
    int channel;
    multiband::bandType bType;
    levelDetector::detectorUnit dUnit;
    
};
#endif /* Compressor_h */
