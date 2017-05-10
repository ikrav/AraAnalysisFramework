#include "CurvedRay.h"

CurvedRay::CurvedRay(OpticalIce *ice_ptr, double antenna_x, double antenna_y, double antenna_z, double source_x, double source_y, double source_z, int use_raytrace){
 
  initialize(ice_ptr, antenna_x, antenna_y, antenna_z, source_x, source_y, source_z, use_raytrace);
  
  
}

CurvedRay::CurvedRay(OpticalIce *ice_ptr, vector<double> antenna_pos, vector<double> source_pos, int use_raytrace){
 
  initialize(ice_ptr, antenna_pos[0], antenna_pos[1], antenna_pos[2], source_pos[0], source_pos[1], source_pos[2], use_raytrace);
  
}

void CurvedRay::initialize(OpticalIce *ice_ptr, double antenna_x, double antenna_y, double antenna_z, double source_x, double source_y, double source_z, int use_raytrace){
  
  reset();
  
  _ice=ice_ptr;
      
  _source_pos_vector.push_back(source_x);
  _source_pos_vector.push_back(source_y);
  _source_pos_vector.push_back(source_z);
  
  _antenna_pos_vector.push_back(antenna_x);
  _antenna_pos_vector.push_back(antenna_y);
  _antenna_pos_vector.push_back(antenna_z);
  
 _use_curved_raytrace_bit=use_raytrace;
 
 findThetaPhi();
 
 calculate();
  
}

void CurvedRay::reset(){
 
  _depth_step=0.1;
  _hor_step=0.1;
  _theta_step=0.05;
  
  _flight_time_total=0;
  _distance_total=0;
  _attenuation_total=0;
    
}

void CurvedRay::clear_all(){
 
  _antenna_pos_vector.clear();
  _source_pos_vector.clear();
  
}

void CurvedRay::printout(){
   
  cout<<"use curved= "<<_use_curved_raytrace_bit<<" ice index= "<<_ice->getIndex(1000)<<endl;
  for(int a=0;a<3;a++) cout<<"source["<<a<<"]= "<<setw(5)<<_source_pos_vector[a]<<"     ant["<<a<<"]= "<<setw(5)<<_antenna_pos_vector[a]<<endl;
  cout<<"theta    = "<<setw(5)<<_launch_theta_angle*RADTODEG<<"  phi   = "<<setw(5)<<_launch_phi_angle*RADTODEG<<endl;
  cout<<"dist     = "<<setw(5)<<_distance_total<<"   time  = "<<setw(5)<<_flight_time_total<<endl;
  
}

// getter methods
  
double CurvedRay::getHitTime(){ 
  
  return _flight_time_total; 
  
}

double CurvedRay::getDistance(){ 
  
  return _distance_total; 
  
}

double CurvedRay::getAttenuation(){ 
  
  return _attenuation_total; 
  
}

OpticalIce *CurvedRay::getIce(){
  
  return _ice;
  
}

// setter methods (and adjust position methods)

void CurvedRay::adjustSourcePosition(double x, double y, double z){
 
  if( (_use_curved_raytrace_bit&&!_ice->isUniform()) || (std::sqrt(std::pow(x-_source_pos_vector[0],2) + std::pow(y-_source_pos_vector[1],2) + std::pow(z-_source_pos_vector[2],2)) > 1) ) setSourcePosition(x,y,z); // if the adjustment is very large, recalculate theta and phi
  
  _source_pos_vector[0]=x;
  _source_pos_vector[1]=y;
  _source_pos_vector[2]=z;
  
  calculate();

}

void CurvedRay::adjustSourcePosition(vector<double> position_cartesian){ 
  
  adjustSourcePosition(position_cartesian[0], position_cartesian[1], position_cartesian[2]); 
  
}

void CurvedRay::setSourcePosition(double x, double y, double z){
   
  _source_pos_vector[0]=x;
  _source_pos_vector[1]=y;
  _source_pos_vector[2]=z;
 
  findThetaPhi();
  
  calculate();

}

void CurvedRay::setSourcePosition(vector<double> position_cartesian){ 
  
  setSourcePosition(position_cartesian[0], position_cartesian[1], position_cartesian[2]); 
  
}

void CurvedRay::setNewIce(OpticalIce *ice){
 
  if(ice==0){ cerr<<"ERROR: null pointer given to setNewIce().\n"; return;}
  
  if(_ice) delete _ice;
  _ice=ice;
  
}

void CurvedRay::calculate(){
 
  if(_ice->isUniform()) calculateUniformIndexTrace();// if ice is uniform do the simplest calculation
  else if(_use_curved_raytrace_bit==0) calculateStraightLineApprox();// if ice is non-uniform use straight line approx
  else calculateCurvedRaytrace(); // if you ask specifically for curved ray tracing algorithm.
  
}

// calculation methods (all protected):
void CurvedRay::findThetaPhi(){
  
  double delta_x=_source_pos_vector[0]-_antenna_pos_vector[0];
  double delta_y=_source_pos_vector[1]-_antenna_pos_vector[1];
  double delta_z=_source_pos_vector[2]-_antenna_pos_vector[2];
  double delta_R=std::sqrt(std::pow(delta_x,2)+std::pow(delta_y,2));
  
  _launch_phi_angle=std::atan2(delta_y, delta_x);
  
  _launch_theta_angle=std::atan2(delta_R, delta_z);
  
}

void CurvedRay::calculateUniformIndexTrace(){
  
  double delta_x=_source_pos_vector[0]-_antenna_pos_vector[0];
  double delta_y=_source_pos_vector[1]-_antenna_pos_vector[1];
  double delta_z=_source_pos_vector[2]-_antenna_pos_vector[2];
  
  _distance_total=std::sqrt(std::pow(delta_x,2) + std::pow(delta_y,2) + std::pow(delta_z,2));
  _flight_time_total=_distance_total*_ice->getIndex(_source_pos_vector[2])/C_SPEED;
  _attenuation_total=_ice->getAttenuation(_source_pos_vector[2], _distance_total);
  
}

void CurvedRay::calculateStraightLineApprox(){

  calculateUniformIndexTrace();
  
  if(fabs(_source_pos_vector[2]-_antenna_pos_vector[2])<_depth_step) return; // if vertical separation is less than one step, just use uniform index.
  
  double z=_source_pos_vector[2];
  double z_finish=_antenna_pos_vector[2];
  
  if(_antenna_pos_vector[2]<_source_pos_vector[2]){
   
    z=_antenna_pos_vector[2];
    z_finish=_source_pos_vector[2];
    
  }
  
  _flight_time_total=0;
  _attenuation_total=1;
  
  for(int i=0; i<1e6; i++){
   
    // recall that in straight line approx the launch angle is the ONLY angle
    _flight_time_total+=_depth_step/std::cos(_launch_theta_angle)*_ice->getIndex(z)/C_SPEED;
    _attenuation_total*=_ice->getAttenuation(z, _depth_step/std::cos(_launch_theta_angle));
    
    z+=_depth_step;
    
    if(z>=z_finish) break;
    
  }
  
  
  
}

void CurvedRay::calculateCurvedRaytrace(){
 
  cerr<<"calculateCurvedRaytrace() not yet implemented...\n";
  
  // if horizontal separation smaller than _hor_step just do straight line approximation (ray goes straight up).
  
}

// useful constants
const double CurvedRay::C_SPEED=0.3; // in meters per nanosecond
const double CurvedRay::pi=3.14159265358979312;
const double CurvedRay::RADTODEG=57.2957795130823229;
const double CurvedRay::DEGTORAD=0.0174532925199432955;


