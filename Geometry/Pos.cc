#include "Pos.h"

ClassImp(Pos);

Pos::Pos(){
 
//   _x=_origin_x;
//   _y=_origin_y;
//   _z=_origin_z;
  
  initialize();
     
//   updateSpherical();
  
}

Pos::Pos(std::vector<double> cartesian_pos){
  
  initialize();
  
  setCartesian(cartesian_pos);
    
}

Pos::Pos(double x, double y, double z){
  
  initialize();
 
  setCartesian(x,y,z);
  
}

Pos::Pos(const Pos &other){
 
  initialize();
  
  setCartesian(other.getCartesian());
  
}

Pos::~Pos(){
  
}

Pos& Pos::operator=(const Pos &other){
 
  Pos temp(other); //  create a copy using the copy ctor
  
  swap(temp, *this);// only if construction worked, swap out the data. 
  
  return *this; // return the new data in *this and discard temp naturally
  
}

void Pos::swap(Pos &first, Pos &second){
 
  Pos temp(first.getCartesian()); // put first into temp
  first.setCartesian(second.getCartesian()); // put second into first
  second.setCartesian(temp.getCartesian()); // put temp (holding the original first) into second 
  
}

void Pos::initialize(){
   
//   std::cout<<"Pos::initialize()"<<std::endl;
  
  _x=0;
  _y=0;
  _z=0;
  
//   _x0=_origin_x;
//   _y0=_origin_y;
//   _z0=_origin_z;
  
}

void Pos::printout() const{
 
    int width=10;
  
    std::cout<<"x= "<<std::setw(width)<<getX()<<"  y= "<<std::setw(width)<<getY()<<"  z= "<<std::setw(width)<<getZ();
    std::cout<<"  R= "<<std::setw(width)<<getR()<<"  theta= "<<std::setw(width)<<rad2deg(getTheta())<<"  phi= "<<std::setw(width)<<rad2deg(getPhi())<<std::endl;
  
}

// void Pos::printHeader() const{
//   
// }

// getters
double Pos::getX() const { return _x; };

double Pos::getY() const { return _y; }

double Pos::getZ() const { return _z; }

std::vector<double> Pos::getCartesian() const {
 
  std::vector<double> pos(3,0);
  
  pos[0]=_x;
  pos[1]=_y;
  pos[2]=_z;
  
  return pos; 
  
}

double Pos::getR() const { 
  
  return sqrt(pow(_x-_origin_x,2) + pow(_y-_origin_y,2) + pow(_z-_origin_z,2)); 
    
}

double Pos::getTheta() const { 
  
  return atan2(sqrt(pow(_x-_origin_x,2) + pow(_y-_origin_y,2)), _z-_origin_z);
  
}

double Pos::getPhi() const { 

  return atan2(_y-_origin_y,_x-_origin_x); 
  
}

std::vector<double> Pos::getSpherical() const {
 
  std::vector<double> pos(3,0);
  
  pos[0]=getR();
  pos[1]=getTheta();
  pos[2]=getPhi();
  
  return pos;
  
}

double Pos::getOriginX() const {
 
  return _origin_x;
  
}

double Pos::getOriginY() const {
 
  return _origin_y;
  
}

double Pos::getOriginZ() const {
 
  return _origin_z;
  
}

Pos Pos::getOrigin(){
 
  return Pos(_origin_x, _origin_y, _origin_z);
  
}

// setters
void Pos::setX(double x){ 
  
  _x=x;
  
//   updateSpherical();
  
}

void Pos::setY(double y){ 
  
  _y=y;
  
//   updateSpherical();
  
}

void Pos::setZ(double z){ 
  
  _z=z;
  
//   updateSpherical();
  
}

void Pos::setCartesian(std::vector<double> pos){
  
  _x=pos[0];
  _y=pos[1];
  _z=pos[2];
  
//   updateSpherical();
  
}

void Pos::setCartesian(double x, double y, double z){
 
  setCartesian(valuesToVector(x,y,z));
 
}

void Pos::setR(double R){
  
  double theta=getTheta();
  double phi=getPhi();

  setSpherical(R, theta, phi);

}

void Pos::setTheta(double theta){
    
  double R=getR();
  double phi=getPhi();
  
  setSpherical(R, theta, phi);
  
}

void Pos::setPhi(double phi){
    
  double R=getR();
  double theta=getTheta();
    
  setSpherical(R, theta, phi);
  
}

void Pos::setSpherical(std::vector<double> pos){
 
  _x=pos[0]*cos(pos[2])*sin(pos[1]) + _origin_x;
  _y=pos[0]*sin(pos[2])*sin(pos[1]) + _origin_y;
  _z=pos[0]*cos(pos[1]) + _origin_z;
    
}

void Pos::setSpherical(double R, double theta, double phi){
 
  setSpherical(valuesToVector(R, theta, phi));
  
}

void Pos::setSphericalDegrees(std::vector<double> pos){

  setSphericalDegrees(pos[0], pos[1], pos[2]);

}

void Pos::setSphericalDegrees(double R, double theta, double phi){
 
  setSpherical(valuesToVector(R, deg2rad(theta), deg2rad(phi)));
  
}

std::vector<double> Pos::valuesToVector(double val1, double val2, double val3){
 
  std::vector<double> vec(3,0);
  
  vec[0]=val1;
  vec[1]=val2;
  vec[2]=val3;
  
  return vec;
  
}

std::vector<double> Pos::sphericalToCartesian(std::vector<double> spherical_pos){
  
  std::vector<double> cartesian_pos(3,0);
  
  cartesian_pos[0]=spherical_pos[0]*cos(spherical_pos[1])*sin(spherical_pos[2]) + _origin_x;
  cartesian_pos[1]=spherical_pos[0]*sin(spherical_pos[1])*sin(spherical_pos[2]) + _origin_y;
  cartesian_pos[2]=spherical_pos[0]*cos(spherical_pos[2]) + _origin_z;
  
  return cartesian_pos;
  
}

std::vector<double> Pos::sphericalToCartesian(double R, double theta, double phi){
  
  return sphericalToCartesian(valuesToVector(R, phi, theta));
  
}

std::vector<double> Pos::cartesianToSpherical(std::vector<double> cartesian_pos){
  
  double x= cartesian_pos[0] - _origin_x;
  double y= cartesian_pos[1] - _origin_y;
  double z= cartesian_pos[2] - _origin_z;
  
  double H=sqrt(x*x + y*y);
  
  std::vector<double> spherical_pos(3,0);
  
  spherical_pos[0]=sqrt(z*z + H*H);
  spherical_pos[1]=atan2(y,x);
  spherical_pos[2]=atan2(z,H); 
  
  return spherical_pos;
  
}

std::vector<double> Pos::cartesianToSpherical(double x, double y, double z){
  
  return cartesianToSpherical(valuesToVector(x,y,z));
  
}

double Pos::rad2deg(double radians){
  
  return 180.0/PI*radians;
  
}

double Pos::deg2rad(double degrees){
 
  return PI/180.0*degrees;
  
}

double Pos::getDistance(const Pos &other) const {
 
  return Pos::getDistance(*this, other);
  
}

double Pos::getDistance(const Pos &first, const Pos &second){
  
  double deltaX=second.getX() - first.getX();
  double deltaY=second.getY() - first.getY();
  double deltaZ=second.getZ() - first.getZ();
  
  return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);
  
}

Pos Pos::getMidPoint(const Pos &other) const {
 
  return getMidPoint(*this, other);
  
}

Pos Pos::getMidPoint(const Pos &first, const Pos &second){
 
  std::vector<Pos> positions;
  
  positions.push_back(first);
  positions.push_back(second);
  
  return getMidPoint(positions);
  
}

Pos Pos::getMidPoint(const std::vector<Pos> positions){
 
  double sum_x=0;
  double sum_y=0;
  double sum_z=0;
  
  for(int i=0;i<positions.size();i++){
    
    sum_x+=positions[i].getX();
    sum_y+=positions[i].getY();
    sum_z+=positions[i].getZ();
    
  }// for i
  
  sum_x/=positions.size();
  sum_y/=positions.size();
  sum_z/=positions.size();
  
  Pos mid_point(sum_x, sum_y, sum_z);
  
  return mid_point;
  
}

void Pos::setOriginStatic(double x0, double y0, double z0){
 
  _origin_x=x0;
  _origin_y=y0;
  _origin_z=z0;
  
}

void Pos::setOriginStatic(const Pos &origin_position){
 
  _origin_x=origin_position.getX();
  _origin_y=origin_position.getY();
  _origin_z=origin_position.getZ();
  
  
}

// math operators
Pos Pos::operator+(const Pos &other){
  
  double x=getX() + other.getX();
  double y=getY() + other.getY();
  double z=getZ() + other.getZ();
  
  return Pos(x,y,z);
  
}

Pos Pos::operator-(const Pos &other){
  
  double x=getX() - other.getX();
  double y=getY() - other.getY();
  double z=getZ() - other.getZ();
  
  return Pos(x,y,z);
  
}

std::ostream &operator<<( std::ostream &out, const Pos &pos){
    
  if(1){ // Pos::print_spherical==0
  
    out<<"( "<<std::setw(6)<<pos.getX()<<"  "<<std::setw(6)<<pos.getY()<<"  "<<std::setw(6)<<pos.getZ()<<"  )";
    
  }  
  else{
    
    out<<"( "<<std::setw(6)<<pos.getR()<<"  "<<std::setw(6)<<Pos::rad2deg(pos.getPhi())<<"  "<<std::setw(6)<<Pos::rad2deg(pos.getTheta())<<"  )";
      
  }
  
}

// bool Pos::print_spherical=0;

double Pos::_origin_x=0;
double Pos::_origin_y=0;
double Pos::_origin_z=-192; // check this number!
const double Pos::PI=3.14159265358979312;

