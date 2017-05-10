#include "EventData.h"

ClassImp(EventData);

EventData::EventData(){
 
  initialize();
  
}

EventData::EventData(ChannelCollection channels, StationGeometry *geometry, OpticalIce *ice){
 
  initialize();

  _channels=channels;   
  _geometry=geometry;
  _ice=ice;
  
}

EventData::~EventData(){
  std::cout <<__PRETTY_FUNCTION__<<std::endl;
 
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  Clear();
  
  if(_vertexResult) delete _vertexResult;
  if(_sim_data) delete _sim_data;
  //  if(_l2_data) delete _l2_data;

  clearVertices();
  clearChannels();
  
}

void EventData::initialize(){
 
  _vertexResult=0; // final result 
//   _vertexVpol=0;
//   _vertexHpol=0;
//   _vertexCombined=0;
//   _vertexArbitrary=0;
    
  _sim_data=0;
  _ice =0;
  _geometry=0;
  
}

void EventData::setData(ChannelCollection channels, StationGeometry *geom, OpticalIce *ice){
  
  _channels = channels;
  _geometry = geom;
  _ice = ice;
}

void EventData::clearChannels(){

  if(_vertexVpol) _vertexVpol->getChannels().clear();
  if(_vertexHpol) _vertexHpol->getChannels().clear();
  if(_vertexCombined) _vertexCombined->getChannels().clear();
  if(_vertexArbitrary) _vertexArbitrary->getChannels().clear();
  
}

void EventData::clearTimeFinders(){
 
  _channels.clearFinders();
  
}

void EventData::clearWaveforms(){
 
  _channels.clearWaveforms();
  
}

void EventData::clearVertices(){

  if(_vertexVpol) delete _vertexVpol; _vertexVpol=0;
  if(_vertexHpol) delete _vertexHpol; _vertexHpol=0;
  if(_vertexCombined) delete _vertexCombined; _vertexCombined=0;
  if(_vertexArbitrary) delete _vertexArbitrary; _vertexArbitrary=0;
  
}

void EventData::clearPrevVertexFinders(){
 
  if(_vertexVpol) _vertexVpol->clearPrevFinders();
  if(_vertexHpol) _vertexHpol->clearPrevFinders();
  if(_vertexCombined) _vertexCombined->clearPrevFinders();
  if(_vertexArbitrary) _vertexArbitrary->clearPrevFinders();
  
}

void EventData::clearVertexFinders(){
 
  if(_vertexVpol) _vertexVpol->clearFinders();
  if(_vertexHpol) _vertexHpol->clearFinders();
  if(_vertexCombined) _vertexCombined->clearFinders();
  if(_vertexArbitrary) _vertexArbitrary->clearFinders();
  
}

void EventData::printout(int print_level){
 
  if(getVertexResult()){
  

    std::cout<<"Vertex result:"<<std::endl;
    getVertexResult()->printout();
      
  }
    
  std::cout<<"Channels: "<<std::endl;
  
  getChannels().printout();
  
}

void EventData::printVertexVpol(int coordinate_system){
  
  if(getVertexVpol()) getVertexVpol()->printout(coordinate_system);
 
}

void EventData::printVertexHpol(int coordinate_system){  
  
  if(getVertexHpol()) getVertexHpol()->printout(coordinate_system);
  
}
  
void EventData::printVertexCombined(int coordinate_system){
  
  if(getVertexCombined()) getVertexCombined()->printout(coordinate_system);
  
}

// getters
VertexPos *EventData::getVertexResult() const {
  
  return _vertexResult;
  
}

EventVertex *EventData::getVertexCombined() const {
  
  return _vertexCombined; 
  
}

EventVertex *EventData::getVertexVpol() const {
  
  return _vertexVpol;
  
}

EventVertex *EventData::getVertexHpol() const {
  
  return _vertexHpol;
  
}

EventVertex *EventData::getVertexArbitrary() const {
 
  return _vertexArbitrary;
  
}

EventVertex *EventData::getEventVertex() const {
 
  if(_vertexCombined) return _vertexCombined;
  if(_vertexVpol) return _vertexVpol;
  if(_vertexHpol) return _vertexHpol;
  if(_vertexArbitrary) return _vertexArbitrary;
  
  return 0; 
  
}

ChannelCollection EventData::getChannels() const {
  
  return _channels;
  
}

OpticalIce *EventData::getOpticalIce() const {
  
  return _ice;
  
}

StationGeometry *EventData::getStationGeometry() const {
 
  return _geometry;
  
}

SimData *EventData::getSimData() const {
  
  return _sim_data;
  
}

/**
L2Data *EventData::getL2Data() const {

  //  return _l2_data;
}
**/
// setters
void EventData::setSimData(SimData *data){
 
  if(data){

    if(_sim_data) delete _sim_data;
    _sim_data=new SimData(*data);
    
  }
  
  else _sim_data=0;
  
}
/***
void EventData::setL2Data(L2Data *data){

  if(data){
    if(_l2_data) delete _l2_data;
    _l2_data = new L2Data(*data);
  }

  else _l2_data = 0;
    
}

**/
void EventData::addVertexFinder(int pol, int finder_type, int coordinate_system, int time_input, int lock_parameter,  double par1, double par2, double par3){
  
  if(pol == 0)
    addVertexFinderVpol(finder_type, coordinate_system, time_input, lock_parameter, par1, par2, par3);
  else if(pol == 1)
    addVertexFinderHpol(finder_type, coordinate_system, time_input, lock_parameter, par1, par2, par3);
  else 
    addVertexFinderCombined(finder_type, coordinate_system, time_input, lock_parameter, par1, par2, par3);

}
  
void EventData::findVertex(int pol, int starting_position){

  if(pol == 0)

    getVertexVpol()->findVertex(starting_position);
  
  else if(pol == 1)

    getVertexHpol()->findVertex(starting_position);

  else 

    getVertexCombined()->findVertex(starting_position);
  
}

void EventData::addVertexFinderVpol(int finder_type, int coordinate_system, int time_input, int lock_parameter,  double par1, double par2, double par3){
 
  if(_vertexVpol==0){ 
    
    _vertexVpol=new EventVertex(getChannels().getVpolChannelCollection(), getOpticalIce());
    if(getSimData()) _vertexVpol->setRealPosition(*(Pos*)getSimData());
    
  }
  
  _vertexVpol->addFinder(finder_type, coordinate_system, time_input, lock_parameter, par1, par2, par3);
    
}

void EventData::addVertexFinderHpol(int finder_type, int coordinate_system, int time_input, int lock_parameter,  double par1, double par2, double par3){
 
  if(_vertexHpol==0){ 
    
    _vertexHpol=new EventVertex(getChannels().getHpolChannelCollection(), getOpticalIce());
    if(getSimData()) _vertexHpol->setRealPosition(*(Pos*)getSimData());
    
  }
  
  _vertexHpol->addFinder(finder_type, coordinate_system, time_input, lock_parameter, par1, par2, par3);
  
}

void EventData::addVertexFinderCombined(int finder_type, int coordinate_system, int time_input, int lock_parameter,  double par1, double par2, double par3){
 
  if(_vertexCombined==0){ 
    
    _vertexCombined=new EventVertex(getChannels(), getOpticalIce());
    if(getSimData()) _vertexCombined->setRealPosition(*(Pos*)getSimData());
    
  }
  
  _vertexCombined->addFinder(finder_type, coordinate_system, time_input, lock_parameter, par1, par2, par3);
  
}









