#include "DeltaFinder.h"
#include "Channel.h"

ClassImp(DeltaFinder);

DeltaFinder::DeltaFinder() : TimeFinder(){
 
  initialize();
  
}

DeltaFinder::DeltaFinder(Channel *chan1, Channel *chan2, double par1, double par2, int stationID, int is_simulated) : TimeFinder(chan1->getWaveform(), par1, par2, chan1->getChannelId(), stationID, is_simulated) {
 
  initialize();
  
  setFirstChannel(chan1);
  setSecondChannel(chan2);
  
//   _channel_id=channelID1;
//   _second_channel_id=channelID2;
  
  
}

DeltaFinder::DeltaFinder(const DeltaFinder& other) : TimeFinder(other) {
   
//   _second_waveform=other.getSecondWaveform();
//   _second_channel_id=other.getSecondChannelId();
  setFirstChannel(other.getFirstChannel());
  setSecondChannel(other.getSecondChannel());

  _finder_type=other.getFinderType();
  
}

DeltaFinder::~DeltaFinder(){
  
  _chan1=0;
  _chan2=0;
  
}

void DeltaFinder::initialize(){
 
//   _second_channel_id=0;
//   _second_waveform=0;

  _chan1=0;
  _chan2=0;
  
}

void DeltaFinder::inverse(){
 
  // switch the channel id's
  Channel *temp_chan=getFirstChannel();
  setFirstChannel(getSecondChannel());
  setSecondChannel(temp_chan);
  
  _time=-_time;
  if(hasRealTime()) _real_time=-_real_time;
  
  
  
}

// getters
Channel *DeltaFinder::getFirstChannel() const {
  
  return _chan1;
  
}

Channel *DeltaFinder::getSecondChannel() const {
 
  return _chan2;
  
}

int DeltaFinder::getFirstChannelId() const {
  
  return getFirstChannel()->getChannelId();
  
}

int DeltaFinder::getSecondChannelId() const {
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  return getSecondChannel()->getChannelId();
  
}

TGraph *DeltaFinder::getFirstWaveform() const {
  
  return getFirstChannel()->getWaveform();

}

TGraph *DeltaFinder::getSecondWaveform() const {
  
  return getSecondChannel()->getWaveform();
  
}

DeltaFinder &DeltaFinder::operator=(const DeltaFinder &other){
 
  _chan1=0; // some else will have to reset these channel pointers to the new owners...
  _chan2=0; // some else will have to reset these channel pointers to the new owners...
  
  TimeFinder::operator=(other); // call the superclass assignment operator too
  
}

// setters
void DeltaFinder::setFirstChannel(Channel *chan){
 
  _chan1=chan;
  
}

void DeltaFinder::setSecondChannel(Channel *chan){
 
  _chan2=chan;
  
}


bool DeltaFinder::operator<(const DeltaFinder &other) const {
 
  return ( getSecondChannelId() < other.getSecondChannelId() );
    
}

bool DeltaFinder::operator<(DeltaFinder *other) const{
  
  return ( getSecondChannelId() < other->getSecondChannelId() );
  
}

const std::string DeltaFinder::name_delta_type(int index){
 
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  const int size=3;
  const std::string names[size]={"SUBTRACT", "CROS", "REAL_DELTAS"};
  
  if(index<0 || index>=size) return "undefined index";
  
  return names[index];
  
}




