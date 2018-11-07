//
//  Compressor.h
//  IconicCompressor_Beta
//
//  Created by Brian Cofer on 10/26/18.
//
#include "../JuceLibraryCode/JuceHeader.h"

#ifndef Compressor_h
#define Compressor_h
/*
 4 classes defined:
 multiband: takes input sample and if desired, splits into a treble part and bass part. Still in development.
 levelDetector: determines input level, default unit is dB, can also determine linear. Will also (I think) have options for various detector types
 gainComputer: applies attack/release parameters
 compressor: pulls everything together. Future versions will allow the level detector and gain computer blocks to be reversed. Still thinking this through.
 

 */


/*! Multiband class; splits the input signal into treble and bass parts to be processed individually. Combines back together after compression. */
class multiband
{
public:
    
    /** An enum type.
     *  Various options for splitting the signal by frequency.
     */
    enum bandType
    {
        NORMAL, /**< Default - No multiband processing */
        BASS,   /**< Compress only the bass. */
        TREBLE, /**< Compress only the treble. */
        MULTI   /**< Future option for a true multiband compressor */
    };
    
    multiband(bandType type = bandType::NORMAL) {
        //initialize RBJFILTER for splitting the signal into multiple parts.
        trebleOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::HIGHPASS), 1, 44100);
        bassOutput = new RBJFilter(RBJFilter::FilterType(RBJFilter::LOWPASS), 1, 44100);
    }
    ~multiband() {}
    
    //! Split signal into treble and bass parts.
    /*!
     Should be the first function called of a compressor. Splits the sidechain into a bass and treble part based on a user-defined crossover frequency.
     This allows only half of the signal to be compressed, while the other half is stored to be recombined after processing.
     Q is adjustable but in the current version, is static at .707.
     */
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
    
    //! Combine compressed treble or bass parts.
    /*!
     Should be called after compression has occured to return a full spectrum signal to the processor.
     */
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

// --------------------------------------------------------------------------------------------------------------------
/*! Gain Smoothing Class; Takes in a detector type, attack, and release values and returns a smoothed gain value.
 This is called from the Level Detector Class.
 */
class gainSmoothing
{
public:
    
    enum detectorType
    {
        PEAK,   /**< Peak Detection  */
        RMS,    /**< RMS Detection  */
        LCP,    /**< Level-Corrected Peak Detection  */
        SMOOTH /**< Default: Smooth Peak Detection  */
    };
    
    
    gainSmoothing() {}
    ~gainSmoothing(){}
    
    float process(float gainChange_dB, int channel){
        
        setGainChange_dB(gainChange_dB);
        setChannel(channel);
        
        if (gainChange_dB ==0){
            gainSmooth = 0;
        }
        else{
            if (dType == PEAK){
                gainSmooth = peakDetector();
            }
            else if (dType == RMS){
                gainSmooth = rmsDetector();
            }
            else if (dType == LCP){
                gainSmooth = lcpDetector();
            }
            else {
                gainSmooth = smoothDetector();
            }
        }
        
        if (isnan(gainSmooth)){
            gainSmooth = 0;
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
    void setDetectorType(detectorType t){
        dType = t;
    }
    void setGainChange_dB(float c){
        gainChange_dB = c;
    }
    void setChannel(int c){
        channel = c;
    }
    float peakDetector(){
        return (alphaR*gainSmoothPrevious[channel]) + ((1-alphaA) * jmax((gainChange_dB - gainSmoothPrevious[channel]), 0.f));
    }
    float rmsDetector(){
        if(gainChange_dB < gainSmoothPrevious[channel]) {
            //attack
            return -sqrt(((1.f-alphaA)*pow(gainChange_dB,2.f)) + (alphaA*pow(gainSmoothPrevious[channel],2.f)));
            
        }
        else {
            //release
            return -sqrt(((1.f-alphaR)*pow(gainChange_dB,2.f)) + (alphaR*pow(gainSmoothPrevious[channel],2.f) ));
        }
    }
    float lcpDetector(){
        if(gainChange_dB > gainSmoothPrevious[channel]) {
            //attack
            return (  ((1-alphaA) * gainChange_dB) + (alphaA*gainSmoothPrevious[channel]) ) ;
            
        }
        else {
            //release
            return (alphaR*gainSmoothPrevious[channel]) ;
        }
    }
    float smoothDetector(){
        if(gainChange_dB > gainSmoothPrevious[channel]) {
            //attack
            return (  ((1-alphaA) * gainChange_dB) + (alphaA*gainSmoothPrevious[channel]) ) ;
            
        }
        else {
            //release
            return ( ((1-alphaR) * gainChange_dB) + (alphaR*gainSmoothPrevious[channel]) ) ;
        }
    }
    
protected:
    float alphaA;
    float alphaR;
    float sampleRate;
    float gainSmooth;
    float gainChange_dB;
    int channel;
    float gainSmoothPrevious[2] = {0}; // _TODO make the channel count dynamic in case > 2 channels.
    detectorType dType = detectorType(detectorType::SMOOTH);
};



//-------------------------------------LEVEL DETECTOR------------------------------------------------------------------
/*! Level Detector Class; Takes in a detectorUnit, detectorTopology and detectorPlacement. Then calculates the appropriate level in DB or Linear amplitude,
 and sends this value to the gain smoothing class.
 */
class levelDetector
{
public:
    enum detectorUnit
    {
        LINEAR,
        DB
    };

    enum detectorTopology{
        FEEDFORWARD,
        FEEDBACK,
        HYBRID
    };
    
    enum detectorPlacement{
        RETURNTOZERO,
        RETURNTOTHRESH,
        LOGDOMAIN
        
    };

    
    levelDetector(detectorUnit u = detectorUnit::DB) : u(u) {
         thisGainSmooth = new gainSmoothing();
    }
    
    ~levelDetector(){
        delete thisGainSmooth;
    }

    
    float process(float inputSample) {
        float inputLevel;
        
        switch(topology){
            case FEEDFORWARD:
            {
                // inputSample = inputSample;
            }break;
            case FEEDBACK:
            {
                inputSample = previousOutput;
            }break;
            case HYBRID:
            {
                //inputSample = inputSample;
            }break;
        }
        
        switch (u)
        {
            case LINEAR:
            {
                inputLevel =  abs(inputSample);
                
                if (inputLevel > 1) {
                    inputLevel = 1;
                }
                
            } break;
                
            case DB:
            {
               // inputLevel =  20.f * log10(abs(inputSample)/1.f);
                
                if (inputSample < -96) {
                    inputLevel = -96;
                }
                else {
                    inputLevel = inputSample;
                }
            } break;
        }

        return thisGainSmooth->process(inputLevel, channel); // apply attack/release times

    }
    
    void setDetectorUnit(detectorUnit unit){
        u = unit;
    }
    void setDetectorTopology(detectorTopology t){
        topology = t;
    }
    detectorTopology getDetectorTopology(){
        return topology;
    }
    void setDetectorPlacement(detectorPlacement dp){
        dPlacement = dp;
    }
    detectorPlacement getDetectorPlacement(){
        return dPlacement;
    }
    void setPreviousOutput(float po){
        previousOutput = po;
    }
    float getPreviousOutput(){
        return previousOutput;
    }
    void setChannel(int c){
        channel = c;
    }
    void setSamplingRate(float Fs){
        thisGainSmooth->setSamplingRate(Fs);
    }
    void setDetectorType(gainSmoothing::detectorType newType){
        thisGainSmooth->setDetectorType(newType);
    }
    void setAttack(float a){
        thisGainSmooth->setAttack(a);
    }
    void setRelease(float r){
        thisGainSmooth->setRelease(r);
    }
    
protected:
    gainSmoothing* thisGainSmooth;
    detectorUnit u = detectorUnit::DB;
    detectorTopology topology = detectorTopology::FEEDFORWARD;
    detectorPlacement dPlacement = detectorPlacement::LOGDOMAIN;
    float previousOutput = 0.f;
    int channel;
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


// ------------------------------------------------------------------------------------------------------------------------

/*! SidechainEQ Class; Takes in the linear input value from the side chain, and if user has defined low or high cut options, eqs the sidechain.
 Additional eq options could easily be added in here.
 */
class sidechainEQ {
    
public:
    sidechainEQ(){
        lowCut = new RBJFilter(RBJFilter::FilterType(RBJFilter::HIGHPASS), 1, 44100);
        highCut = new RBJFilter(RBJFilter::FilterType(RBJFilter::LOWPASS), 1, 44100);
      
        lowCut->SetQValue(.707);
        highCut->SetQValue(.707);
     
    }
    ~sidechainEQ(){
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

        return high;
       
    }
   
// _TODO many of these functions will need to be elsewhere or removed
        void setSamplingRate(float s){
            lowCut->SetSamplingRate(s);
            highCut->SetSamplingRate(s);
        }

    void setThreshold(float t){
        threshold = t;
    }
    
protected:

    RBJFilter* highCut;
    RBJFilter* lowCut;
    
    levelDetector::detectorUnit dUnit;
    float threshold;
    
};

/*! Compressor Class; The "root" of the compressor.
 Takes in various compressor options, e.g. attack, release, threshold, etc.
 Functionality based on this paper:
 Giannoulis, D., Massberg, M., & Reiss, J. D. (2012). Digital Dynamic Range Compressor Design. Journal of the Audio Engineering Society, 60(6), 399–408.
 and
 Tarr, Eric. Hack Audio: An Introduction to Computer Programming and Digital Signal Processing in MATLAB. Routledge, 2018.
 with additional options such as the multiband and sidechain eq added by Brian Cofer.
 This and all sub classes created by Brian Cofer. Unrestricted use and modification is granted.
 */
class compressor {
    
public:
    
    //Compressor Initilization
    compressor(){
        //initialize required subclasses
        thisSidechain = new sidechainEQ();
        thisMultiband = new multiband(); //can we "if" this somehow _TODO
        thisLevelDetector = new levelDetector();
        thisGainComputer = new gainComputer();
       
    }
    ~compressor(){
        delete thisMultiband;
        delete thisGainComputer;
        delete thisSidechain;
        delete thisLevelDetector;
    }
    
    /*! The main function.
     
    1) Takes an input sample, splits bass and treble if needed.
    2) EQ the sidechain if requested.
    3) Process the signal according to system design options, either the level detector first, or the gain computer.
    4) If the sample was split into bass and treble, recombine the compressed band with the uncompressed band.
    5) Save the output to the level detector for the next run
    6) Consider detector topology again if needed. Hybrid topology returns the previousOutput * input. Others simply return output * input.
     
     *Input gain, output gain and wet/dry mix are not a part of this class, though this may be added later. Those are currently set before/after calling the class.
     */
    float tick(float input, int channel){
    
        float inputSample = thisMultiband->splitInputSample(input, channel); //send sample to the multiband processor. Returns input if no multiband option
        
        //FUTURE USE: Placeholder to see if user has specified a different sidechain.
        if (sidechainInput == 0) {
            setSidechainInput(inputSample);
        }
        else {
            setSidechainInput(inputSample);
        }
        

        //send the input to the sidechainEQ class. This class handles any EQ done to the sidechain. Output is linear.
        float processedSidechain = abs(thisSidechain->process(sidechainInput, channel));
        
        float gainSmooth;
        float sidechainOutput;
        float linA;
        
        
        // Different signal-chain options depending on system design / topology
        if (thisLevelDetector->getDetectorPlacement() == levelDetector::detectorPlacement::RETURNTOZERO){
            
           //abs>levelDetector>dB>GainComp>+?>Output*Input
            gainSmooth = 20.f * log10(abs(thisLevelDetector->process(processedSidechain))); //convert linear to dB
            sidechainOutput = thisGainComputer->process(gainSmooth); //determine necessary gain change
             linA = pow(10.0f,sidechainOutput/20.0f);
        }
        else if  (getDetectorPlacement() == levelDetector::detectorPlacement::RETURNTOTHRESH){

            //abs>-Threshold>levelDetector>+Thresh>dB>gainComp...

            float ldMinusThresh = thisLevelDetector->process(abs(processedSidechain) - pow(10.f,thresholdValue/20.f)); //subtract threshold from input & send to detector
            float addThresh = ldMinusThresh+pow(10.f,thresholdValue/20.f); //add threshold back in
            gainSmooth = 20.f * log10(addThresh); // convert to dB
            sidechainOutput = thisGainComputer->process(gainSmooth); //send to gain computer
             linA = pow(10.0f,sidechainOutput/20.0f);
        }
        else {
            // log domain: abs>dB>gainComp>levelDetector>lin
             float gC_dB = thisGainComputer->process(20.f*log10(processedSidechain)); //determine necessary gain change
            sidechainOutput = (thisLevelDetector->process((20.f * log10(processedSidechain)) - gC_dB  ));
             linA = -(pow(10.0f,sidechainOutput/20.0f));
            //PEAK and RMS not working correctly
            //START THERE ^
        }
        

        //send to multiband to re-combine compressed treble/bass with uncompressed treble/bass. Returns input if no MB option set
        float outputSample = thisMultiband->combineIO(linA);
        
        //Save our output into the levelDetector for the next run
        thisLevelDetector->setPreviousOutput(outputSample * input);
        
        if (getDetectorTopology() == levelDetector::HYBRID){ //(I think) this is how to implement the HYBRID topology as described
            return thisLevelDetector->getPreviousOutput() * input;
        }
        else {
            return outputSample * input; //final step; multiply the calculated output value with the input sample to get compressed output
        }
    }

    void setCurrentSample(float currentSample) { inputSample = currentSample; }
    float getCurrentSample() { return inputSample; }
    void SetSamplingRate(float Fs) {
        thisLevelDetector->setSamplingRate(Fs);
        thisMultiband->setSamplingRate(Fs);
        thisSidechain->setSamplingRate(Fs);
    }
    void setBandType(multiband::bandType newTypeB) {
        bType = newTypeB;
    }
    void setDetectorType(gainSmoothing::detectorType newType){
        thisLevelDetector->setDetectorType(newType);
        
    }
    void setTopology(levelDetector::detectorTopology newTopology){
        thisLevelDetector->setDetectorTopology(newTopology);
    }
    
    //void setBandCount(int c) { bandCount = c; }
    void setCrossoverFrequency(float f) {
        crossoverFrequency = f;
        thisMultiband->setCrossoverFrequency(f);
    }
    
    void setThreshold(float t){
        thresholdValue = t;
        thisGainComputer->setThreshold(t);
        thisSidechain->setThreshold(t);
    }
    void setRatio(float r){
        ratioValue = r;
        thisGainComputer->setRatio(r);
    }

    void setAttack(float attackTime){
        thisLevelDetector->setAttack(attackTime);
    }
    void setRelease(float releaseTime){
        thisLevelDetector->setRelease(releaseTime);
    }
    void setCurrentChannel(int c) {
        channel = c;
        thisLevelDetector->setChannel(c);
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
    void setDetectorPlacement(levelDetector::detectorPlacement dPlacement){
        thisLevelDetector->setDetectorPlacement(dPlacement);
        
        if (dPlacement != levelDetector::LOGDOMAIN) {
        thisLevelDetector->setDetectorUnit(levelDetector::detectorUnit::LINEAR);
        }
        else{
        thisLevelDetector->setDetectorUnit(levelDetector::detectorUnit::DB);
        }
        
    }

//------------------------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------------------
    void setDetectorTopology(levelDetector::detectorTopology newTopology){
        thisLevelDetector->setDetectorTopology(newTopology);
    }
    levelDetector::detectorTopology getDetectorTopology(){
        return thisLevelDetector->getDetectorTopology();
    }
    
    levelDetector::detectorPlacement getDetectorPlacement(){
        return thisLevelDetector->getDetectorPlacement();
    }
  
protected:
    multiband* thisMultiband;
    gainComputer* thisGainComputer;
    levelDetector* thisLevelDetector;
    sidechainEQ* thisSidechain;
    
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
