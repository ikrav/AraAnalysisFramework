#include "AntPos.h"

ClassImp(AntPos);

AntPos::AntPos() : Pos(){}

AntPos::AntPos(std::vector<double> cartesian_pos, int pol, double vrms) : Pos(cartesian_pos){

  setPolarization(pol);
  setVRMS(vrms);
}

AntPos::AntPos(double x, double y, double z, int pol, double vrms) : Pos(x, y, z){

  setPolarization(pol);
  setVRMS(vrms);
}

AntPos::AntPos(const AntPos &other) : Pos(other){

  setPolarization(other.getPolarization());
  setChanID(other.getChanID());
  setVRMS(other.getVRMS());
}

AntPos::~AntPos(){}

void AntPos::printout(){

  Pos::printout();
  std::cout <<" \t pol : "<< _polarization <<" VRMS: " << _vrms << std::endl;
}

int AntPos::getPolarization() const{

  return _polarization;
}

int AntPos::getChanID() const{

  return _chan_id;
}

double AntPos::getVRMS() const{
  
  return _vrms;
}

void AntPos::setPolarization(int pol){

  _polarization = pol;
}

void AntPos::setChanID(int id){

  _chan_id = id;
}

void AntPos::setVRMS(double value){

  _vrms = value;
}
