#include "ExpectedDeltaFinder.h"

ClassImp(ExpectedDeltaFinder);

ExpectedDeltaFinder::ExpectedDeltaFinder() : DeltaFinder() {

  initialize();
}

ExpectedDeltaFinder::ExpectedDeltaFinder(Channel *chan1, Channel *chan2, double par1, double par2, int stationID, int is_simulated) : DeltaFinder(chan1, chan2, par1, par2, stationID, is_simulated) {
  
  initialize();
  scanWaveform();
  calculateWeight();
}

ExpectedDeltaFinder::ExpectedDeltaFinder(const ExpectedDeltaFinder &other) : DeltaFinder(other){ 
 
    // this is needed if only to make sure there's a call to DeltaFinder's copy constructor
  
}

ExpectedDeltaFinder::~ExpectedDeltaFinder() {}

DeltaFinder *ExpectedDeltaFinder::getDeepCopy(){
 
  return new ExpectedDeltaFinder(*this);
  
}

void ExpectedDeltaFinder::initialize(){
  
  setFinderName("expected");
  setFinderType(DeltaFinder::EXPECTED);
  
  //  _thresh=10000;
  //  _scale = 5000;
}

void ExpectedDeltaFinder::calculateTime(){

  getFirstChannel()->getRay()->adjustSourcePosition(getSecondChannel()->getPosition());
  double dt = getFirstChannel()->getRay()->getHitTime();

  setTime(dt);
}

void ExpectedDeltaFinder::calculateError(){

  setError(1);
}

void ExpectedDeltaFinder::calculateWeight(){
  
  setWeight(0.9); // dummy value
}
