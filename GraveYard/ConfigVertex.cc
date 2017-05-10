#include "ConfigVertex.h"

ConfigVertex::ConfigVertex(){

  initialize();
 
}

ConfigVertex::ConfigVertex(string vertexFinder, double a, double b, double c, string coord){

  initialize();
  setVertexFinder(vertexFinder);
  setParameters(a, b, c);
  setCoordinates(coord);
  
}

ConfigVertex::~ConfigVertex(){

  _previous_finder = 0;
}

void ConfigVertex::initialize(){

  _previous_finder = 0;
}

void ConfigVertex::printout(){

  std::cout <<"\t Vertex "<< _vertex_finder <<" type: "<< _vertex_type << " params: ("<< _param[0] << ", "<< _param[1] <<", "<< _param[2]<<")" <<" coord: "<< _coordinates << " coord type: "<< _coordinate_type << std::endl;
  if(_previous_finder){
    std::cout << " \t \t with previous finder ";
    _previous_finder->printout();
  }
}

string ConfigVertex::getVertexFinder() const{
  
  return _vertex_finder;
}

vector<double> ConfigVertex::getParameters() {

  vector<double> params;
  params.clear();
  params.reserve(3);
  
  for(int i=0; i<3; i++){
    
    params.push_back(_param[i]);
  }

  return params;
}

string ConfigVertex::getCoordinate() const{
  
  return _coordinates;
}

ConfigVertex *ConfigVertex::getPreviousFinder() {
  
  return _previous_finder;
}

int ConfigVertex::getVertexType(){
  
  return _vertex_type;
}

int ConfigVertex::getCoordinateType(){

  return _coordinate_type;
}

double ConfigVertex::getParameter1(){

  return _param[0];
}

double ConfigVertex::getParameter2(){

  return _param[1];
}

double ConfigVertex::getParameter3(){

  return _param[2];
}

int ConfigVertex::getLockType(){

  return _lock_type;
}
void ConfigVertex::setVertexFinder(string finder){

  _vertex_finder = finder;
  setVertexType();
  setLockType();
}

void ConfigVertex::setParameters(double a, double b, double c){
  
  _param[0] = a;
  _param[1] = b;
  _param[2] = c;
}

void ConfigVertex::setCoordinates(string coord){
  
  _coordinates = coord;
  setCoordinateType();
}

void ConfigVertex::setPreviousFinder(ConfigVertex *vertex){
  
  _previous_finder = vertex;
}

void ConfigVertex::setVertexType(){

  if(_vertex_finder == "SCAN")
    _vertex_type = VertexFinder::SCAN;
  else if(_vertex_finder == "MINUIT")
    _vertex_type = VertexFinder::MINUIT;
  else if(_vertex_finder == "ASM")
    _vertex_type = VertexFinder::ASM;
  else if(_vertex_finder == "MCMC")
    _vertex_type = VertexFinder::MCMC;
  else if(_vertex_finder == "INTMAP")
    _vertex_type = VertexFinder::INTMAP;
  else if(_vertex_finder == "REAL_VERTEX")
    _vertex_type = VertexFinder::REAL_VERTEX;
  else if(_vertex_finder == "EARLIEST")
    _vertex_type = VertexFinder::EARLIEST;
  else if(_vertex_finder == "ANYWHERE")
    _vertex_type = VertexFinder::ANYWHERE;
  else if(_vertex_finder == "UNLKUMINUIT")
    _vertex_type = VertexFinder::UNLKUMINUIT;
  else{
    std::cerr<< "UNKNOWN: "<< _vertex_finder <<  " setting Minuit as default MINUIT "<< std::endl;
    _vertex_type = VertexFinder::MINUIT; //minuit by default
  }
}

void ConfigVertex::setCoordinateType(){
  
  if(_coordinates == "CARTESIAN")
    _coordinate_type = VertexFinder::CARTESIAN;
  else if (_coordinates == "SPHERICAL")
    _coordinate_type = VertexFinder::SPHERICAL;
  else {
    std::cerr <<"UNKNOWN: " <<_coordinates <<" setting SPHERICAL as default"<< std::endl;
    _coordinate_type = VertexFinder::SPHERICAL;
  }

}

void ConfigVertex::setLockType(){

  if(_lock == "UNLOCK")
    _lock_type = VertexFinder::UNLOCK;
  //to be completed

}
