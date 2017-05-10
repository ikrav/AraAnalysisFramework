#include "SubtractDeltaFinder.h"
#include "Channel.h"

ClassImp(SubtractDeltaFinder);

SubtractDeltaFinder::SubtractDeltaFinder() : DeltaFinder() {}

SubtractDeltaFinder::SubtractDeltaFinder(Channel *chan1, Channel *chan2, double par1, double par2, int stationID, int is_simulated) : DeltaFinder(chan1, chan2, par1, par2, stationID, is_simulated){
  
  initialize();
  
  scanWaveform();
  
}

SubtractDeltaFinder::SubtractDeltaFinder(const SubtractDeltaFinder &other) : DeltaFinder(other){
  
    // this is needed if only to make sure there's a call to DeltaFinder's copy constructor

}

SubtractDeltaFinder::~SubtractDeltaFinder(){}

DeltaFinder *SubtractDeltaFinder::getDeepCopy(){
  
  return new SubtractDeltaFinder(*this);
  
}

void SubtractDeltaFinder::initialize() {
 
  setFinderName("subtract");
  setFinderType(DeltaFinder::SUBTRACT);
  
}

void SubtractDeltaFinder::calculateTime() {
    
  setTime(getSecondChannel()->getTime() - getFirstChannel()->getTime());   
  
}

void SubtractDeltaFinder::calculateWeight(){

  setWeight(getFirstChannel()->getWeight() * getSecondChannel()->getWeight() );
  
}

void SubtractDeltaFinder::calculateError(){

  setError(sqrt( pow(getFirstChannel()->getError(),2) + pow(getSecondChannel()->getError(),2) ));
}
