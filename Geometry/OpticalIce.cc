#include "OpticalIce.h"

ClassImp(OpticalIce);

OpticalIce::OpticalIce(double n_deep, int variable_bit, int attenuation_bit, double n_shallow, double firn_start_depth, double slope){ 
    
    initialize(n_deep, variable_bit, attenuation_bit, n_shallow, firn_start_depth, slope); 
    
}

OpticalIce::OpticalIce(const OpticalIce &other){
 
  initialize(other.getDeepIceIndex(), other.isVariable(), other.hasAttenuation(), other.getFirnIceIndex(), other.getFirnDepth(), other.getExpSlope());
  
  setAttenuationFactor(other.getDeepIceAttenuation(), other.getFirnIceAttenuation());
  
}

OpticalIce::~OpticalIce(){
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
}

void OpticalIce::initialize(double n_deep, int variable_bit, int attenuation_bit, double n_shallow, double firn_start_depth, double slope){
  
  _variable_ice_bit=variable_bit;
  _attenuation_bit=attenuation_bit;
  
  setIndex(n_deep, n_shallow, firn_start_depth, slope);
  
  setAttenuationFactor();// add the parameters to the constructor???
    
}

// void OpticalIce::reset(){
//       
//   setIndex();
//   
//   setAttenuationFactor();
//   
//   _variable_ice_bit=0;
//   _attenuation_bit=0;
//   
// }

void OpticalIce::printout(){
 
  std::cout<<" ice model: n_s - (n_s - n_d) exp[-la * z ] with:  n_d= "<<_deep_ice_index<<" n_s= "<<_firn_ice_index<<" la= "<<_exp_slope<<std::endl;
  std::cout<<"use variable ice: "<<_variable_ice_bit<<" use ice attenuation: "<<_attenuation_bit;
  if(_attenuation_bit) std::cout<<" att_d= "<<_deep_ice_att<<" att_s= "<<_shallow_ice_att;
  std::cout<<std::endl;
  
}

// getter methods
double OpticalIce::getIndex(double z){
 
  if(z >= 0) return 1;  // above surface
  if(_variable_ice_bit==0) return _deep_ice_index; // constant ice case
  double result = _deep_ice_index + (_firn_ice_index - _deep_ice_index)*std::exp(-_exp_slope*fabs(z));
  return result;

}

double OpticalIce::getAttenuation(double z, double path_length){
  
  if(!_attenuation_bit) return 1;
  
  double attenuation;
  
  if(_variable_ice_bit) attenuation=_deep_ice_att + (_shallow_ice_att - _deep_ice_att)*std::exp(-_exp_slope*fabs(z));
  else attenuation=_deep_ice_att;
  
  return std::exp(-attenuation*path_length);
  
}

double OpticalIce::getDeepIceIndex() const {
    //  return _deep_ice_att;
    return _deep_ice_index;
  
}

double OpticalIce::getFirnIceIndex() const {
  
  return _firn_ice_index;
  
}

double OpticalIce::getFirnDepth() const {
  
  return _depth_of_firn;
  
}

double OpticalIce::getExpSlope() const {
  
  return _exp_slope;
  
}

double OpticalIce::getDeepIceAttenuation() const {
  
  return _deep_ice_att;
  
}

double OpticalIce::getFirnIceAttenuation() const {
  
  return _shallow_ice_att;
  
}

int OpticalIce::isUniform() const {

  return !_variable_ice_bit;
  
}

int OpticalIce::isVariable() const {
  
  return _variable_ice_bit;
  
}

int OpticalIce::hasAttenuation() const {
  
  return _attenuation_bit;
  
}



// setter methods  
void OpticalIce::setIndex(double n_deep, double n_shallow, double firn_start_depth, double slope){
 
  _deep_ice_index=n_deep;
  _firn_ice_index=n_shallow;
  _depth_of_firn=firn_start_depth;
  
  if(slope==0) _exp_slope=(double)1/(3*_depth_of_firn); 
  else _exp_slope=slope;
  
}

void OpticalIce::setAttenuationFactor(double deep_attenuation_factor, double shallow_attenuation_factor){
  
  _deep_ice_att=deep_attenuation_factor; 
  
  _shallow_ice_att=shallow_attenuation_factor; 
  
}

void OpticalIce::setIceVariablityBit(int bit_status) { 
  
  _variable_ice_bit=bit_status; 
  
}

void OpticalIce::setIceAttenuationBit(int bit_status){ 
  
  _attenuation_bit=bit_status; 
  
}



  
