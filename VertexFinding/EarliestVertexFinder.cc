#include "EarliestVertexFinder.h"

ClassImp(EarliestVertexFinder);

EarliestVertexFinder::EarliestVertexFinder() : VertexFinder() {}

EarliestVertexFinder::EarliestVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameter, ice_ptr, par1, par2, par3){

  initialize();

}

EarliestVertexFinder::EarliestVertexFinder(const std::string commands) : VertexFinder(){
  
  initialize();
  
  parse(commands);
  
}

EarliestVertexFinder::EarliestVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands) : VertexFinder(channels, initial_position){
  
  initialize();
  
  parse(commands);
  
}

EarliestVertexFinder::EarliestVertexFinder(const EarliestVertexFinder &other) : VertexFinder(other){ 
  
  // this is here because we need to explicitly call the VertexFinder copy constructor....
  
}

EarliestVertexFinder::~EarliestVertexFinder() {}

VertexFinder *EarliestVertexFinder::getDeepCopy(){
 
  return new EarliestVertexFinder(*this);
  
}

void EarliestVertexFinder::initialize(){}

void EarliestVertexFinder::help(){
  
  VertexFinder::help();
  
}

void EarliestVertexFinder::parse(const std::string commands){
 
  VertexFinder::parse(commands);
  
}

std::string EarliestVertexFinder::getFinderName() const {
  
  return "earliest";
}

void EarliestVertexFinder::findVertex(){

  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  //set the results to the earliest channel
  setCartesian(_channels.getEarliestChannel()->getPosition().getX(), _channels.getEarliestChannel()->getPosition().getY(), _channels.getEarliestChannel()->getPosition().getZ());

  setConfidence(0.9);
  setStatusFound();
}

