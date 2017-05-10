#include "SimData.h"

ClassImp(SimData);

SimData::SimData() : Pos() {
 
  initialize();
  
}

SimData::SimData(const SimData &other) : Pos(other) {

  setEnergy(other.getEnergy());
  setFieldAtSource(other.getFieldAtSource());
  setPolarizationRatio(other.getPolarizationRatio());
  setDirTheta(other.getDirTheta());
  setDirPhi(other.getDirPhi());
  setNuFlavor(other.getNuFlavor());
    
}

SimData::~SimData(){
  
}

void SimData::initialize(){
 
  _energy=0;
  _field_at_source=0;
  _pol_ratio=0;
  
  _nu_dir_theta=0;
  _nu_dir_phi=0;
  
  _nu_flavor=0;
  
}

void SimData::printout(){
 
  std::cout<<"E= "<<getEnergy()<<" [eV]  F= "<<getFieldAtSource()<<" [V/m]  P= "<<getPolarizationRatio()<<std::endl;
  std::cout<<"dir_th= "<<getDirTheta()*StationGeometry::RADTODEG<<" [deg]  dir_phi= "<<getDirPhi()*StationGeometry::RADTODEG<<" [deg]  ";
  Pos::printout();
  
}

// getters
double SimData::getEnergy() const {
  
  return _energy;
  
}

double SimData::getFieldAtSource() const {
  
  return _field_at_source;
  
}

double SimData::getDirTheta() const {
  
  return _nu_dir_theta;
  
}

double SimData::getDirPhi() const {
  
  return _nu_dir_phi;
  
}

double SimData::getPolarizationRatio() const {
  
  return _pol_ratio;
  
}

int SimData::getNuFlavor() const {
  
  return _nu_flavor;
  
}

// setters
void SimData::setEnergy(double energy){
  
  _energy=energy;
  
}

void SimData::setFieldAtSource(double field){
  
  _field_at_source=field;
  
}

void SimData::setPolarizationRatio(double pol_ratio){ 
  
  _pol_ratio=pol_ratio;
  
}

void SimData::setDirTheta(double theta){
  
  _nu_dir_theta=theta;
  
}

void SimData::setDirPhi(double phi){
  
  _nu_dir_phi=phi;
  
}

void SimData::setNuFlavor(int flavor){
  
  _nu_flavor=flavor;
  
}








