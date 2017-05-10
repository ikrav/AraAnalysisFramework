#include "ThreshTimeFinder.h"

ClassImp(ThreshTimeFinder);

ThreshTimeFinder::ThreshTimeFinder() : TimeFinder() {
  
  initialize();
  
}

ThreshTimeFinder::ThreshTimeFinder(TGraph *waveform, double par1, double par2, int channelID, int stationID, int is_simulated) : TimeFinder(waveform, par1, par2, channelID, stationID, is_simulated) {
  
//   std::cout<<"ThreshTimeFinder::ThreshTimeFinder(...)"<<std::endl;
  
  initialize();
  
  _waveform=waveform;
  
  scanWaveform();
  
}

ThreshTimeFinder::ThreshTimeFinder(const ThreshTimeFinder& other) : TimeFinder(other) {
  
    // this is needed if only to make sure there's a call to DeltaFinder's copy constructor
  
}

ThreshTimeFinder::~ThreshTimeFinder(){
  
}

TimeFinder *ThreshTimeFinder::getDeepCopy(){
  
  return new ThreshTimeFinder(*this);
  
}

void ThreshTimeFinder::initialize(){
 
//   std::cout<<"ThreshTimeFinder::initialize()"<<std::endl;
  
  double multiplier=6;
    
  _quality_parameter=0;
  
  _waveform=0;

  if(_geom){
    _thresh = _geom->getPosition(_channel_id).getVRMS();
  }
  else {
    _thresh = 50.0;
  }
  
  //_thresh=50;// this is subtracted from the initial QP to cut low intensity events
  _scale=100; // this divides the residual QP to go roughly into the 0-1 range
    
  //  useMeasuredThreshold();// this checks if there is a specific (experimentally measured) threshold to replace _thresh. 
  
  _thresh*=multiplier;
  
  _signal_width=0;
  
  setFinderName("thresh");
  setFinderType(TimeFinder::THRESH);

}

void ThreshTimeFinder::calculateTime(){
  
//   std::cout<<"ThreshTimeFinder::calculateTime()"<<std::endl;
  
  int N=_waveform->GetN();
  double *x=_waveform->GetX();
  double *y=_waveform->GetY();
  
  
//   _thresh*=multiplier;// the baseline RMS is multiplied by some factor to get the real threshold...
  
  int bin_start=-1;
  int bin_end=N-1;
  int num_bins=6;// number of bins signal has to be low to end the width of the signal
  int counter=num_bins;
  double maximum=0;
  
  for(int i=0;i<N;i++){
   
    if(bin_start<0 && fabs(y[i])>=_thresh) bin_start=i;// set the start bin (if it isn't set yet).
        
    if(fabs(y[i])>=_thresh) counter=num_bins; // if we are above threshold, reset the counter
    
    if(bin_start>=0 && bin_end==N-1 && fabs(y[i])<_thresh) counter--; // if the signal goes down (after we marked the start and before we marked the end) reduce the counter...
    
    if(bin_end==N-1 && counter<=0) bin_end=i; // if we have not yet set the bin_end and the counter reaches zero, set the bin end

    if(fabs(y[i])>maximum) maximum=fabs(y[i]);
    
  }// for i
  
  if(bin_start>0){// if there is a signal above the threshold...
    
    setTime(x[bin_start]);

    _quality_parameter=maximum;
  
    _signal_width=x[bin_end]-x[bin_start];
    
  }
  
}

void ThreshTimeFinder::calculateError(){
  
//   std::cout<<"ThreshTimeFinder::calculateError()"<<std::endl;
  
  double min_error=3;
  
  if(_signal_width>min_error) setError(_signal_width);
  else setError(min_error);
  
}

void ThreshTimeFinder::useMeasuredThreshold(){
 
  if(_station_id>=0 && _station_id<_measured_rms.size() && _channel_id>=0 && _channel_id<_measured_rms[_station_id].size()){
   
    _thresh=_measured_rms[_station_id][_channel_id];
    
  }
  
  // if not, just use the default value of _thresh
  
}

