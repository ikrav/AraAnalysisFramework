#include "THX.h"
#include "VertexFinder.h"

int THX::debug_bit=0;

ClassImp(THX);

THX::THX(){
  
  initialize();
  
}

THX::THX(const THX &other){
  
  
}

THX::~THX(){
 
  resetPoints();
  
}

void THX::initialize(){

//   _name="";
//   _title="Scan results";
  
  resetPoints();
  
  _reduce_dims = 0; // by default lower dimensions get projected. 
  _coordinates;
  
  _x_nbins=100;
  _x_min=-2000;
  _x_max=+2000;
  
  _y_nbins=100;
  _y_min=-2000;
  _y_max=+2000;
  
  _z_nbins=100;
  _z_min=-2000;
  _z_max=+2000;
  
  _R_nbins=100;
  _R_min=0;
  _R_max=5000;
  
  _theta_nbins=180;
  _theta_min=-90;
  _theta_max=+90;
  
  _phi_nbins=180;
  _phi_min=-180;
  _phi_max=+180;
  
}

void THX::resetPoints(){

  _points.clear();
  
}

void THX::printout() const {
    
  std::cout<<__PRETTY_FUNCTION__<<" this is not yet implemented!"<<std::endl;

}

void THX::help() const {

    std::cout<<__PRETTY_FUNCTION__<<" this is not yet implemented!"<<std::endl;

}

// getters
int THX::getDimReduction() const{
  
  return _reduce_dims;
  
}

int THX::getCoordinates() const {
  
  return _coordinates;
  
}

std::string THX::getCoordinatesString() const {
 
  std::string str="";
  
  if(_coordinates==VertexFinder::CARTESIAN) str.append("Cartesian");
  if(_coordinates==VertexFinder::SPHERICAL) str.append("Spherical");
  
  return str;
  
}

int THX::getValueType() const{
 
  if(_points.empty()) return 0;
  else return _points[0].getType();
    
}

double THX::getPowerForQP() const{
  
  if(_points.empty()) return 1;
  else return _points[0].getPowerForQP();
   
}

std::vector<ValuePos> THX::getPointsVector() const{
 
  return _points;
  
}

TH1D *THX::getNewHist1D(int axis, int coordinates, int show_prob) const {
      
  if(coordinates<0) coordinates = _coordinates;

  std::cout<<__PRETTY_FUNCTION__<<" this is not yet implemented!"<<std::endl;
  
  return 0;
  
}

TH2D *THX::getNewHist2D(int axis1, int axis2, int coordinates, int show_prob) const {
      
  if(coordinates<0) coordinates = _coordinates;

  std::cout<<__PRETTY_FUNCTION__<<" this is not yet implemented!"<<std::endl;
  
  return 0;
  
}

TH3D *THX::getNewHist3D(int coordinates, int show_prob) const {
         
  if(coordinates<0) coordinates = _coordinates;

  std::cout<<__PRETTY_FUNCTION__<<" this is not yet implemented!"<<std::endl;
  
  return 0;
  
}

int THX::getNbins(int axis, int coordinates) const {
   
  if(coordinates<0) coordinates = _coordinates;

  if(coordinates==VertexFinder::CARTESIAN){
   
    switch(axis){
     
      case 0: return _x_nbins;
      case 1: return _y_nbins;
      case 2: return _z_nbins;
      
    }
    
  }
  
  if(coordinates==VertexFinder::SPHERICAL){
   
    switch(axis){
     
      case 0: return _R_nbins;
      case 1: return _theta_nbins;
      case 2: return _phi_nbins;
      
    }
    
  }
  
}

double THX::getMin(int axis, int coordinates) const {
   
  if(coordinates<0) coordinates = _coordinates;

  if(coordinates==VertexFinder::CARTESIAN){
   
    switch(axis){
     
      case 0: return _x_min;
      case 1: return _y_min;
      case 2: return _z_min;
      
    }
    
  }
  
  if(coordinates==VertexFinder::SPHERICAL){
   
    switch(axis){
     
      case 0: return _R_min;
      case 1: return _theta_min;
      case 2: return _phi_min;
      
    }
    
  }
  
}

double THX::getMax(int axis, int coordinates) const {
   
  if(coordinates<0) coordinates = _coordinates;

  if(coordinates==VertexFinder::CARTESIAN){
   
    switch(axis){
     
      case 0: return _x_max;
      case 1: return _y_max;
      case 2: return _z_max;
      
    }
    
  }
  
  if(coordinates==VertexFinder::SPHERICAL){
   
    switch(axis){
     
      case 0: return _R_max;
      case 1: return _theta_max;
      case 2: return _phi_max;
      
    }
    
  }
  
}

int THX::getNumDimensions(int coordinates) const {
  
  if(coordinates<0) coordinates = _coordinates;
  
  int dims=0;
  
  if(coordinates==VertexFinder::CARTESIAN){
  
    if(_x_nbins>1) dims++;
    if(_y_nbins>1) dims++;
    if(_z_nbins>1) dims++;
    
  }
  
  if(coordinates==VertexFinder::SPHERICAL){
  
    if(_R_nbins>1) dims++;
    if(_theta_nbins>1) dims++;
    if(_phi_nbins>1) dims++;
    
  }
  
  return dims;
  
}

// setters
// void THX::SetName(const char *name){
//   
//   TNamed::SetName(name);
//     
// }

// void THX::SetTitle(const char *title){
//  
//   TNamed::SetTitle(title);
//     
// }

// void THX::SetNameTitle(const char *name, const char *title){
// 
//   TNamed::SetName(name);
//   TNamed::SetTitle(title);
//     
// }
  
void THX::setPoint(Pos position, double value){
  
  _points.push_back(ValuePos(position, value));
  
}

void THX::setPointsVector(std::vector<ValuePos> points, int append){
 
  if(append==0){
    
    _points.clear();
    _points = points;
    
  }
  else{
    
    _points.insert(_points.end(), points.begin(), points.end());
        
  }
  
}
  
void THX::setValueType(int type){
 
  for(int i=0;i<_points.size();i++) _points[i].setType(type);
  
}

void THX::setPowerForQP(double power){
  
  for(int i=0;i<_points.size();i++) _points[i].setPowerForQP(power);
  
}

void THX::reserve(int size){
  
  _points.reserve(size);
  
}


  
  
  
  
  
  
  