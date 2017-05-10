#include "RealTimeFinder.h"

ClassImp(RealTimeFinder);

RealTimeFinder::RealTimeFinder() : TimeFinder() {

  initialize();
  
}

RealTimeFinder::RealTimeFinder(TGraph *waveform, double par1, double par2, int channelID, int stationID, int is_simulated) : TimeFinder(waveform, par1, par2, channelID, stationID, is_simulated) {
  
  initialize();
  
  _waveform=waveform;
  
  scanWaveform();
  
}

RealTimeFinder::RealTimeFinder(const RealTimeFinder &other) : TimeFinder(other) {
  
  _rand = new TRandom3(*other._rand);
  
  TimeFinder::operator=(other);
  
}

RealTimeFinder::~RealTimeFinder(){
  
  delete _rand;
  
}

TimeFinder *RealTimeFinder::getDeepCopy(){
 
  return new RealTimeFinder(*this);
  
}

void RealTimeFinder::initialize(){
      
  _quality_parameter=0;
  
  _waveform=0;
  
  setFinderName("real");
  setFinderType(TimeFinder::REAL_TIMES);
  
  _rand=new TRandom3(0);
  
}

void RealTimeFinder::calculateTime(){
 
  if(hasRealTime()){
    
    if(_parameter1>0) setTime(_rand->Gaus(getRealTime(), getPar1()));
    else setTime(getRealTime());
    
  }
  else setTime(0);
  
}

void RealTimeFinder::calculateWeight(){
 
  if(hasRealTime()) setWeight(1);
  else setWeight(0);
  
  if(_parameter2>0 && _rand->Rndm() > _parameter2) setWeight(0); // used to make less than all channels have useful weight. 
  
}

void RealTimeFinder::calculateError(){
  
  setError(getPar1());
  
}

