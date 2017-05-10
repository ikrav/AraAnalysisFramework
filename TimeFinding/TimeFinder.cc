#include "TimeFinder.h"

ClassImp(TimeFinder);

TimeFinder::TimeFinder() : Timing() { 
  
  initialize();
  
}

TimeFinder::TimeFinder(TGraph *waveform, double par1, double par2, int channelID, int stationID, int is_simulated) : Timing() {
  
//   std::cout<<"TimeFinder::TimeFinder(...)"<<std::endl;
  
  initialize();
  
  setParameters(par1, par2);
  
  _waveform=waveform;
  
  _channel_id = channelID;
  
  
}

TimeFinder::TimeFinder(const TimeFinder &other) : _channel_id(other.getChannelId()), _station_id(other.getStationId()), _is_simulated(other.isSimulated()), Timing(other){ 
 
  _waveform=other.getWaveform();
  setScale(other.getScale());
  setThresh(other.getThresh());
  
  setParameters(other.getPar1(), other.getPar2());
  
  
  // we don't copy _quality_parameter and _signal_width because they are only ever used internally. if that changes, must add them here... 
    
}

TimeFinder::~TimeFinder(){

  // don't delete the waveform, it isn't yours to destroy!
  _waveform =0;
  
}

void TimeFinder::initialize(){
   
//   std::cout<<"TimeFinder::initialize()"<<std::endl;
  
  _quality_parameter=0;
  _signal_width=0;
  
  _thresh=0;
  _scale=1;
  
  _channel_id=0;
  _station_id=0;
  _is_simulated=1;// assume all data is simulation unless otherwise specified...
  
  _waveform=0;
  
  setParameters();
  
}

void TimeFinder::printout(){
 
  std::cout<<"thresh= "<<std::setw(6)<<getThresh()<<"  scale= "<<std::setw(6)<<getScale()<<std::endl;
  Timing::printout();
  
}

void TimeFinder::scanWaveform(){

//   std::cout<<"TimeFinder::scanWaveform()"<<std::endl;
  
  if(_waveform==0){ std::cerr<<"no waveform given to this finder...\n"; return; } // check there is actually a waveform to work with... 
  
  calculateTime();
  calculateWeight();
  calculateError();
  
  
}

// getters
double TimeFinder::getScale() const {
  
  return _scale;
  
}

double TimeFinder::getThresh() const {
  
  return _thresh;
  
}

int TimeFinder::getChannelId() const { return _channel_id; }

int TimeFinder::getStationId() const { return _station_id; }

int TimeFinder::isSimulated() const { return _is_simulated; }

int TimeFinder::isMeasured() const { return !_is_simulated; }

TGraph *TimeFinder::getWaveform() const {
  
  return _waveform;
  
}

double TimeFinder::getPar1() const { 
  
  return _parameter1;
  
}

double TimeFinder::getPar2() const {
 
  return _parameter2;
  
}

int TimeFinder::isUsingParameters(double par1, double par2) const {

  if( fabs(par1-getPar1())<=epsilon && fabs(par2-getPar2())<=epsilon) return 1;
  
  return 0;
  
}

TimeFinder& TimeFinder::operator=(const TimeFinder &other){
 
  if(this==&other) return *this; // don't self copy!

  _channel_id = other._channel_id;
  _station_id = other._station_id;
  _is_simulated = other._is_simulated;
  _scale = other._scale;
  _thresh = other._thresh;
  _quality_parameter = other._quality_parameter;
  _signal_width = other._signal_width;
  _parameter1 = other._parameter1;
  _parameter2 = other._parameter2;
  
  _waveform = 0; // This is a tricky one. should we copy the pointer to the wf even if the new finder lives in a new channel with a new graph??
  
  return *this;
  
}

// setters
void TimeFinder::setChannelId(int channel_id){
  
  _channel_id=channel_id;
  
}

void TimeFinder::setStationId(int station_id){
  
  _station_id=station_id;
  
}

void TimeFinder::setScale(double scale){
  
  _scale=scale;
  
}

void TimeFinder::setThresh(double threshold){
  
  _thresh=threshold;
  
}

void TimeFinder::setWaveform(TGraph *waveform){
  
  _waveform=waveform; // not a deep copy, that one belongs to Channel!
  
}

void TimeFinder::setSimulated(int is_simulated){ _is_simulated=is_simulated; }

void TimeFinder::setParameters(double par1, double par2){
 
  _parameter1=par1;
  _parameter2=par2;
  
}

void TimeFinder::setGeometry(StationGeometry *geom){

  _geom = geom;
}

void TimeFinder::calculateWeight(){
 
//   std::cout<<"TimeFinder::calculateWeight()"<<std::endl;
  
  // the test for under 0 and below 1 is done at Timing class  
  setWeight( (_quality_parameter-_thresh)/_scale);
  
}

void TimeFinder::calculateError(){
 
//   std::cout<<"TimeFinder::calculateError()"<<std::endl;
  
  _error=3; // default value of error. consider putting it in some data member? don't see much reason. just change this function if needed... 
  
}

std::vector< std::vector<double> > TimeFinder::fill_static_vector(){
 
//   std::cout<<"TimeFinder::fill_static_vector"<<std::endl;
  
  {
  
  // setup the testbed RMS figures (station 0)
  double array[]={58.6373, 44.6564, 32.6612, 39.7439, 48.5373, 60.3161, 37.9067, 49.2996, 47.1186, 35.8446, 46.7825, 46.9619, 30.9856, 46.7562}; 
  int size=sizeof(array)/sizeof(double);
  std::vector<double> vector(&array[0], &array[0]+size);// this is ugly but that's the best I could find to do this... 
   
  _measured_rms.push_back(vector);
  
  }// assign measured RMS for testbed 
  
  
}

std::vector< std::vector<double> > TimeFinder::_measured_rms=fill_static_vector();

const std::string TimeFinder::name_time_type(int index){
 
  const int size=5;
  const std::string names[size]={"THRESH", "GAUS", "CSW", "SIMPLE", "REAL_TIMES"};
  
  if(index>=size) return names[size-1];
  
  return names[index];
  
}

const double TimeFinder::epsilon=1e-6;


StationGeometry *TimeFinder::_geom=0;
