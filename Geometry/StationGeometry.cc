#include "StationGeometry.h"

ClassImp(StationGeometry);

StationGeometry::StationGeometry(std::vector<AntPos> positions, double originX, double originY, double originZ) : _string_radius(0), _vertical_separation(0){
  
  initialize();
  setChannels(positions);
  double origin[3] = { originX, originY, originZ};
  setOrigin(origin);
  
}

StationGeometry::StationGeometry(double radius, double vertical, int pol): _string_radius(radius), _vertical_separation(vertical){

  initialize();
  defaultGeometry(pol);
  
}

StationGeometry::StationGeometry(const StationGeometry &other){
 
  _vertical_separation=other._vertical_separation;
  _string_radius=other._string_radius;
  _num_strings=other._num_strings;
  _ant_per_string=other._ant_per_string;
  
  _antenna_positions=other._antenna_positions;
  _calpulser_positions=other._calpulser_positions;
  
  setOrigin(other._origin_x, other._origin_y, other._origin_z);
  
  _has_twisted_geometry_bit=other._has_twisted_geometry_bit;
  
}

StationGeometry::~StationGeometry(){

  clear_all();
  
}

AntPos StationGeometry::operator[](int index) const{

  return _antenna_positions[index];
}

void StationGeometry::initialize(){

  clear_all();
  
}

void StationGeometry::reset(int pol){

  clear_all();
  defaultGeometry(pol);
}

void StationGeometry::clear_all(){

  _antenna_positions.clear();
}

void StationGeometry::printout(){

  for(int i =0; i<_antenna_positions.size(); i++){
    _antenna_positions[i].printout();
  }

  std::cout<<std::endl<<"origin: x= "<<_origin_x<<" y= "<<_origin_y<<" z= "<<_origin_z<<std::endl;
}

void StationGeometry::defaultGeometry(int pol){
 
  double origin[3]={0};
  
  _num_strings=4;
  _ant_per_string=4; // the default for Vpol/Hpol
  if(pol==VPOL || pol==HPOL) _ant_per_string=2; // when using only one polarization
  
  std::vector<double> v_sep=std::vector<double>(4,0);
  std::vector<double> phi=std::vector<double>(_num_strings,0);
  
  double turn_angle=360.0/_num_strings;
  
  v_sep[0]=0;
  v_sep[1]=2;
  v_sep[2]=2+_vertical_separation;
  v_sep[3]=4+_vertical_separation;
   
  for(int i=0;i<_num_strings; i++){
   
    phi[i]=DEGTORAD*(3*turn_angle/2-i*turn_angle);
    
    for(int j=0;j<_ant_per_string;j++){// make Vpol and Hpol (default)
      
      _antenna_positions.push_back(AntPos());
      
      //_antenna_positions_matrix[_ant_per_string*i+j][0]=_string_radius*std::cos(phi[i]);
      _antenna_positions.back().setX(_string_radius*std::cos(phi[i]));
      origin[0]+=_string_radius*std::cos(phi[i]);
      
      //_antenna_positions_matrix[_ant_per_string*i+j][1]=_string_radius*std::sin(phi[i]);
      _antenna_positions.back().setY(_string_radius*std::sin(phi[i]));
      origin[1]+=_string_radius*std::sin(phi[i]);
      
      if(pol==BOTH){
	
	//_antenna_positions_matrix[_ant_per_string*i+j][2]=-200+v_sep[j];
	_antenna_positions.back().setZ(-200+v_sep[j]);
        //_polarization_vector[_ant_per_string*i+j]=(j%2);// even antennas are pol 0 (Vpol)
	_antenna_positions.back().setPolarization(j%2);
        origin[2]+=-200+v_sep[j];
	
      }
      
      if( pol==VPOL || pol == HPOL ){
	
	//_antenna_positions_matrix[_ant_per_string*i+j][2]=-200+v_sep[2*j+pol];
	_antenna_positions.back().setZ(-200+v_sep[2%j+pol]);
	//_polarization_vector[_ant_per_string*i+j]=pol;
	_antenna_positions.back().setPolarization(pol);
	origin[2]+=-200+v_sep[2*j+pol];
	
      }
      
    }// for j
      
  }// for i
  
  
  
  setOrigin(origin[0]/getNumChan(), origin[1]/getNumChan(), origin[2]/getNumChan());
   
 _has_twisted_geometry_bit=0;
 
}

void StationGeometry::twistGeometry(){

  //testing other features, will be implemented
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this hasn't been implemented yet!"<<std::endl;
  
  return;
}

int StationGeometry::getNumChan(int pol){

  //for both
  return _antenna_positions.size();
  
}

std::vector<double> StationGeometry::getAntPos(int ch){

  return _antenna_positions[ch].getCartesian();
}

AntPos StationGeometry::getPosition(int channelID) const {

  if(channelID<0 || channelID>=_antenna_positions.size()){
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access ID "<<channelID<<" number of channels is "<<_antenna_positions.size()<<std::endl;
    return AntPos();
  }
  
  return _antenna_positions[channelID];

}

AntPos StationGeometry::getPulserPos(int pulserID) const {
 
  if(pulserID<0 || pulserID>=_calpulser_positions.size()){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access ID "<<pulserID<<" number of pulsers is "<<_calpulser_positions.size()<<std::endl;
    
    return AntPos();
    
  }
  
  return _calpulser_positions[pulserID];
  
}

int StationGeometry::getPolarization(int ch) const {
    
  if(ch<0 || ch>=_antenna_positions.size()){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access ID "<<ch<<" number of channels is "<<_antenna_positions.size()<<std::endl;
    
    return -1;
    
  }
  
  return _antenna_positions[ch].getPolarization();
}

int StationGeometry::getPulserPolarization(int pol_id) const {
  
  if(pol_id<0 || pol_id>=_calpulser_positions.size()){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access ID "<<pol_id<<" number of pulsers is "<<_calpulser_positions.size()<<std::endl;
    
    return -1;
    
  }
  
  return _calpulser_positions[pol_id].getPolarization();
  
}

std::vector<AntPos> StationGeometry::getChannels() const {

  return _antenna_positions;
  
}

std::vector<AntPos> StationGeometry::getPulsers() const {
  
  return _calpulser_positions;
  
}

StationGeometry *StationGeometry::getVpolGeometry(){

  return getSubSet(VPOL);
}

StationGeometry *StationGeometry::getHpolGeometry(){

  return getSubSet(HPOL);
}

StationGeometry *StationGeometry::getSubSet(int pol){

  std::vector<AntPos> newPos;
  for(int i=0; i<_antenna_positions.size(); i++){
    if(_antenna_positions[i].getPolarization() == pol){
      newPos.push_back(_antenna_positions[i]);
    }
  }
  
  return (new StationGeometry(newPos, _origin_x, _origin_y, _origin_z));
}

std::vector<double> StationGeometry::getOrigin(){

  std::vector<double> tmp_origin;
  tmp_origin.push_back(_origin_x);
  tmp_origin.push_back(_origin_y);
  tmp_origin.push_back(_origin_z);

  return tmp_origin;
}

Pos StationGeometry::getOriginPosition() const{

  return (Pos(_origin_x, _origin_y, _origin_z));
}

double StationGeometry::getStringRadius(){ 
  
  return _string_radius; 
}

double StationGeometry::getVerticalSeparation(){ 
  
  return _vertical_separation;   
}

void StationGeometry::setOrigin(double origin_x, double origin_y, double origin_z){
 
  double origin[3];
  
  origin[0]=origin_x;
  origin[1]=origin_y;
  origin[2]=origin_z;
  
  setOrigin(origin);
  
}

void StationGeometry::setOrigin(double *origin){

  _origin_x = origin[0];
  _origin_y = origin[1];
  _origin_z = origin[2];
  
  Pos::setOriginStatic(_origin_x, _origin_y, _origin_z);
  
}

void StationGeometry::setChannels(std::vector<AntPos> positions){
  
  _antenna_positions = positions;
}

void StationGeometry::addChannel(AntPos pos){

  _antenna_positions.push_back(pos);
}

void StationGeometry::setPulsers(std::vector<AntPos> cal_pos){
 
  _calpulser_positions = cal_pos;
  
}

void StationGeometry::addPulser(AntPos cal_pos){
 
  _calpulser_positions.push_back(cal_pos);
  
}


const double StationGeometry::PI=3.14159265358979312;
const double StationGeometry::RADTODEG=57.2957795130823229;
const double StationGeometry::DEGTORAD=0.0174532925199432955;
