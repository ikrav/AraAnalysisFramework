#include "RealVertexFinder.h"

ClassImp(RealVertexFinder);

RealVertexFinder::RealVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameter, ice_ptr, par1, par2, par3){
  
  initialize();
  
}

RealVertexFinder::RealVertexFinder(const std::string commands) : VertexFinder(){
  
  initialize();
  
  parse(commands);
  
}

RealVertexFinder::RealVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands) : VertexFinder(channels, initial_position){
  
  initialize();
  
  parse(commands);
  
}

RealVertexFinder::RealVertexFinder(const RealVertexFinder &other) : VertexFinder(other) {

  // this is here because we need to explicitly call the VertexFinder copy constructor....

}

RealVertexFinder::~RealVertexFinder(){
  
}

VertexFinder *RealVertexFinder::getDeepCopy(){
 
  return new RealVertexFinder(*this);
  
}

void RealVertexFinder::initialize(){ 

  if(hasRealPos()==0){ std::cerr<<"ERROR: called 'RealVertexFinder' when initial_position doesn't have real times...\n"; }
  
    
}

void RealVertexFinder::help(){
  
  VertexFinder::help();
  
}

void RealVertexFinder::parse(const std::string commands){
 
  VertexFinder::parse(commands);
  
}

std::string RealVertexFinder::getFinderName() const {
  
  return "real";
  
}

void RealVertexFinder::findVertex(){
 
  if(checkIfSomethingMissing()) {
    setStatusFailed();
    return;
  }
  
  Pos real_pos=getRealPosition();
  
  TRandom3 rand(0);
  
  if( _parameter1>0 || _parameter2>0 || _parameter3>0 ){// random error parameter...

    if(_coordinate_system==VertexFinder::CARTESIAN){

      real_pos.setX(rand.Gaus(real_pos.getX(), _parameter1));
      real_pos.setY(rand.Gaus(real_pos.getY(), _parameter2));
      real_pos.setZ(rand.Gaus(real_pos.getZ(), _parameter3));

      this->setErrorsCartesian(_parameter1, _parameter2, _parameter3);
    
    }
    
    if(_coordinate_system==VertexFinder::SPHERICAL){
     
      real_pos.setR(rand.Gaus(real_pos.getR(), _parameter1));
      real_pos.setTheta(rand.Gaus(real_pos.getTheta(), _parameter2));
      real_pos.setPhi(rand.Gaus(real_pos.getPhi(), _parameter3));

      this->setErrorsSpherical(_parameter1, _parameter2, _parameter3);
     
    }
  }
  else{
   
    this->setErrorsCartesian(0,0,0);
    this->setErrorsSpherical(0,0,0);
    
  }
  
  *((Pos*) this)=real_pos;
  
  calculateConfidence(_parameter1*_parameter2*_parameter3);
  
  setStatusFound();
  
}

void RealVertexFinder::calculateConfidence(double quality_parameter){
   
  setConfidence(1000/quality_parameter);
  
}
