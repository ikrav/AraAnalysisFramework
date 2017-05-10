#include "EventData.h"

EventData::EventData(){
  
  initialize();
  
}

EventData::EventData(int combine_method){
 
  initialize();
  
  _combine_method=combine_method;
  
}

EventData::EventData(const EventData &other){
 
  initialize();
  
  setChannels(other.getChannels());
  setFindersVector(other.getFindersVector());
  setHousekeeping(other.getHousekeeping());
  setSimData(other.getSimData());
  
  setCombineMethod(other.getCombineMethod());
  
  _final_position = other._final_position;
}

EventData::~EventData(){
 
  clearChannels();
  clearFinders();
  clearHousekeeping();
  clearSimData();
  
  delete _final_position;
  
}

void EventData::clearChannels(){
 
  _channels.clear(); // this is not a deep delete!!!
  
}

void EventData::clearFinders(){
  
  _finders_vector.clear(); // this is not a deep delete!!!
  
}

void EventData::clearHousekeeping(){
  
  _housekeeping_data=0; // this is not a deep delete!!!
  
}

void EventData::clearSimData(){
  
  _simulation_data=0; // this is not a deep delete!!!
  
}

void EventData::clearFinalPosition(){
 
  delete _final_position;
  
  _final_position=0;
  
}

void EventData::initialize(){
 
  _combine_method=0;
  _is_simulated=0;
  
  _final_position=0;
  
  _housekeeping_data=0;
  _simulation_data=0;
    
}

void EventData::printout(){
 
  if(_final_position) _final_position->printout();
  
  _channels.printout();
  
  for(int i=0;i<_finders_vector.size();i++) _finders_vector[i]->printout();
  
  if(_housekeeping_data) _housekeeping_data->printout();
  if(_simulation_data) _simulation_data->printout();
  
}

// getters
int EventData::getCombineMethod() const {
  
  return _combine_method;
  
}

ChannelCollection EventData::getChannels() const {
 
  return getChannelCollection();
  
}

ChannelCollection EventData::getChannelCollection() const {
  
  return _channels;
  
}

VertexFinder *EventData::getFinder(int number) const {
 
  if(number<0 || number>=_finders_vector.size()){ 
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: the number given "<<number<<" is outside the size of the finders vector: "<<_finders_vector.size()<<std::endl;
    return 0;
    
  }
  
  return _finders_vector[number];
  
}

std::vector<VertexFinder*> EventData::getFindersVector() const {
 
  return _finders_vector;
  
}

L2Data *EventData::getHousekeeping() const {
  
  return _housekeeping_data;
  
}

SimData *EventData::getSimData() const {
  
  return _simulation_data;
  
}

VertexPos *EventData::getFinalPosition(){
 
  if(_finders_vector.empty()) return 0;
  
  if(_final_position==0) calculate();
  
  return _final_position;
  
}

// setters
void EventData::setCombineMethod(int method){
  
  _combine_method=method;
  
}

void EventData::setChannels(ChannelCollection chans){
  
  setChannelCollection(chans);
  
}

void EventData::setChannelCollection(ChannelCollection chans){
 
  _channels = chans;
  
}

void EventData::addFinder(VertexFinder *finder){
 
  if(finder==0) return; // silently ignore empty finders!
  
  _finders_vector.push_back(finder);
  
  clearFinalPosition();
  
}

void EventData::setFindersVector(std::vector<VertexFinder*> finders){
  
  _finders_vector=finders;

  clearFinalPosition();
  
}

void EventData::setHousekeeping(L2Data *data){
  
  _housekeeping_data=data;
  
}

void EventData::setSimData(SimData *data){
  
  _simulation_data=data;
  _is_simulated=1;
  
}

void EventData::calculate(){
  
  if(_finders_vector.empty()){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot calculate() without any vertex finders!"<<std::endl;
    return;
    
  }
  
  clearFinalPosition();

  _final_position = VertexPos::meanVertexPos(_finders_vector, _combine_method);

}


ClassImp(EventData);