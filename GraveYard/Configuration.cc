#include <Configuration.h>

Configuration::Configuration() {

  initialize();

}

Configuration::~Configuration() {

  for(int i=0; i<_vertex_finders.size(); i++){
    if(_vertex_finders[i]) delete _vertex_finders[i];
  }

  _vertex_finders.clear();
}

void Configuration::initialize(){

  _vertex_finders.clear();
}

ConfigVertex *Configuration::operator[](int index) const {

  return _vertex_finders[index];
}

void Configuration::printout(){

  std::cout <<"Run 1: "<< std::endl;
  std::cout <<"\t Label: " << _label << std::endl;
  std::cout <<"\t Pol: "<< _pol << "\t type: "<< _pol_type << std::endl;
  std::cout <<"\t TimeFinder: "<< _time_finder << " (" << _time_parameters[0] <<", "<< _time_parameters[1] <<", "<< _time_parameters[2] <<") type "<< _time_finder_type <<  std::endl;


  for(int i=0; i<_vertex_finders.size(); i++){
    
    _vertex_finders[i]->printout();
  }
}
  
string Configuration::getLabel() const {
  
  return _label;
}

string Configuration::getPol() const {

  return _pol;
}

string Configuration::getTimeFinder() const {

  return _time_finder;
}

vector<double> Configuration::getTimeFinderParameters() const{

  vector<double> params;
  params.clear();
  params.reserve(3);

  for(int i=0; i<3; i++){
    params.push_back(_time_parameters[i]);
  }
  
  return params;
}

vector<ConfigVertex*> Configuration::getVertices(){
  
  return _vertex_finders;
}

int Configuration::getPolType() const{
  
  return _pol_type;
}
int Configuration::getTimeType() const{
  
  return _time_finder_type;
}

int Configuration::getDeltaType() const{

  return _delta_type;
}

string Configuration::getDelta() const{
  
  return  _delta;
}

void Configuration::setLabel(string label){

  _label = label;
}

void Configuration::setPol(string pol){

  _pol = pol;
  setPolType();
}

void Configuration::setTimeFinder(string finder){

  _time_finder = finder;
  setTimeType();
}

void Configuration::setDelta(string type){
  
  _delta = type;
  setDeltaType();
}

void Configuration::setTimeFinderParameters(double a, double b, double c){

  _time_parameters[0] = a;
  _time_parameters[1] = b;
  _time_parameters[2] = c;
}

void Configuration::addVertex(ConfigVertex *vertex){
  
  _vertex_finders.push_back(vertex);
}

void Configuration::setPolType(){

  if(_pol == "VPOL")
    _pol_type = StationGeometry::VPOL;
  else if(_pol == "HPOL")
    _pol_type = StationGeometry::HPOL;
  else
    _pol_type = StationGeometry::BOTH;
}

void Configuration::setTimeType(){

  if(_time_finder == "THRESH")
    _time_finder_type = TimeFinder::THRESH;
  else if(_time_finder == "GAUS")
    _time_finder_type = TimeFinder::GAUS;
  else if(_time_finder == "REAL_TIMES")
    _time_finder_type = TimeFinder::REAL_TIMES;
  else if(_time_finder == "SIMPLE")
    _time_finder_type = TimeFinder::SIMPLE;
  else if(_time_finder == "CSW")
    _time_finder_type = TimeFinder::CSW;
  else if(_time_finder == "CROS")
    _time_finder_type = DeltaFinder::CROS;
  else if(_time_finder == "SUBTRACT")
    _time_finder_type = DeltaFinder::SUBTRACT;
  else if(_time_finder == "REAL_DELTAS")
    _time_finder_type = DeltaFinder::REAL_DELTAS;
  else if(_time_finder == "EXPECTED")
    _time_finder_type = DeltaFinder::EXPECTED;
  else
    _time_finder_type = TimeFinder::SIMPLE; // in case of type run Simple
}

void Configuration::setDeltaType(){
  
  if(_delta == "TIMES" || _delta == "TIME")
    _delta_type = VertexFinder::TIMES;
  else if(_delta == "DELTA")
    _delta_type = VertexFinder::PAIRS;
  else 
    _delta_type = VertexFinder::TIMES;
}
