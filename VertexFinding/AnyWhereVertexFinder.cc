#include "AnyWhereVertexFinder.h"

ClassImp(AnyWhereVertexFinder);

AnyWhereVertexFinder::AnyWhereVertexFinder() : VertexFinder() {}

AnyWhereVertexFinder::AnyWhereVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameter, ice_ptr, par1, par2, par3){

  initialize();
}

AnyWhereVertexFinder::AnyWhereVertexFinder(const std::string commands) : VertexFinder() {
 
  initialize();
  
  parse(commands);
  
}

AnyWhereVertexFinder::AnyWhereVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands) : VertexFinder(channels, initial_position){
 
  initialize();
  
  parse(commands);
  
}

AnyWhereVertexFinder::AnyWhereVertexFinder(const AnyWhereVertexFinder &other) : VertexFinder(other){
 
  // this is here because we need to explicitly call the VertexFinder copy constructor....
  
}

AnyWhereVertexFinder::~AnyWhereVertexFinder(){}

void AnyWhereVertexFinder::initialize() {}

void AnyWhereVertexFinder::help(){
  
  VertexFinder::help();
  
}

void AnyWhereVertexFinder::parse(const std::string commands){
 
  VertexFinder::parse(commands);
        
  std::vector< std::vector< std::string> > parsed = splitCommands(commands);
  
  for(int i=0;i<parsed.size();i++){
  
    if(check_str(parsed[i][0], "x", "r")) setPosition1(stof(parsed[i][1]));
    if(check_str(parsed[i][0], "y", "theta")) setPosition2(stof(parsed[i][1]));
    if(check_str(parsed[i][0], "z", "phi")) setPosition3(stof(parsed[i][1]));
    
  }
  
}

VertexFinder *AnyWhereVertexFinder::getDeepCopy(){
 
  return new AnyWhereVertexFinder(*this);
  
}

std::string AnyWhereVertexFinder::getFinderName() const {

  return "anywhere";
}

void AnyWhereVertexFinder::setPosition1(double position){
 
  _parameter1=position;
  
}

void AnyWhereVertexFinder::setPosition2(double position){
 
  _parameter2=position;
  
}

void AnyWhereVertexFinder::setPosition3(double position){
 
  _parameter3=position;
  
}

void AnyWhereVertexFinder::findVertex(){

  if(_coordinate_system == VertexFinder::CARTESIAN){    

    setCartesian(_parameter1, _parameter2, _parameter3);

  }
  if(_coordinate_system == VertexFinder::SPHERICAL){

    setSphericalDegrees(_parameter1, _parameter2, _parameter3);
  }

  setConfidence(1);
  setStatusFound();
}

