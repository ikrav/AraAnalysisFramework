#include "SimpleTimeFinder.h"

ClassImp(SimpleTimeFinder);

SimpleTimeFinder::SimpleTimeFinder() : TimeFinder() { }

SimpleTimeFinder::SimpleTimeFinder(TGraph *waveform, double par1, double par2, int channelID, int stationID, int isSimulated) : TimeFinder(waveform, par1, par2, channelID, stationID, isSimulated){

  initialize();
  _waveform = waveform;
  scanWaveform();

}

SimpleTimeFinder::SimpleTimeFinder(const SimpleTimeFinder &other) : TimeFinder(other){
 
    // this is needed if only to make sure there's a call to DeltaFinder's copy constructor
  
}

SimpleTimeFinder::~SimpleTimeFinder(){

}

TimeFinder *SimpleTimeFinder::getDeepCopy(){
  
  return new SimpleTimeFinder(*this);
  
}

void SimpleTimeFinder::initialize(){
  _finder_name = "SIMPLE";
  _finder_type = 1;
   
}

void SimpleTimeFinder::calculateTime(){

  int N = _waveform->GetN();
  double *x = _waveform->GetX();
  double *y = _waveform->GetY();
  double max = 0;
  for(int i=0; i < N; i++){
    if(std::fabs(y[i])>max){
	max = std::fabs(y[i]);
	setTime(x[i]);
      }
  }

}

void SimpleTimeFinder::calculateError(){
  setError(3);
}

void SimpleTimeFinder::calculateWeight(){
  
  setWeight(0.5);
}
