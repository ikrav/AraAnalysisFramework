#include "VertexPos.h"
#include "VertexFinder.h"

ClassImp(VertexPos);

VertexPos::VertexPos() : Pos(Pos::sphericalToCartesian(2500, 90*DEGTORAD, 180*DEGTORAD)){
// VertexPos::VertexPos() : Pos(Pos::getOrigin().getX(), Pos::getOrigin().getY(), Pos::getOrigin().getZ()){
  
  initialize();
  
}

VertexPos::VertexPos(std::vector<double> cartesian_pos) : Pos(cartesian_pos) {
 
  initialize();
  
}

VertexPos::VertexPos(std::vector<double> cartesian_pos, std::vector<double> errors_cartesian) : Pos(cartesian_pos) {
 
  initialize();
  
  setErrorsCartesian(errors_cartesian);
  
}

// VertexPos::VertexPos(std::vector<double> cartesian_pos, std::vector<double> lower_errors_cartesian, std::vector<double> upper_errors_cartesian) : Pos(cartesian_pos) {
//  
//   initialize();
//   
//   setLowerErrorsCartesian(lower_errors_cartesian);
//   setUpperErrorsCartesian(upper_errors_cartesian);
//   
// }

VertexPos::VertexPos(double x, double y, double z) : Pos(x,y,z){ 
  
  initialize();
  
}

VertexPos::VertexPos(double x, double y, double z, double error_x, double error_y, double error_z) : Pos(x,y,z){
 
  initialize();
  
  setErrorsCartesian(error_x, error_y, error_z);
  
}

// VertexPos::VertexPos(double x, double y, double z, double lower_error_x, double lower_error_y, double lower_error_z, double upper_error_x, double upper_error_y, double upper_error_z) : Pos(x,y,z){
//  
//   initialize();
//   
//   setLowerErrorsCartesian(lower_error_x, lower_error_y, lower_error_z);
//   setUpperErrorsCartesian(upper_error_x, upper_error_y, upper_error_z);
//   
// }

VertexPos::VertexPos(const Pos &other) : Pos(other){ 
  
  initialize();
  
}

VertexPos::VertexPos(const VertexPos &other) : Pos(other){
 
//   std::cout<<"VertexPos copy constructor..."<<std::endl;
  
  initialize();
      
//   setLowerErrorsSpherical(other.getLowerErrorsSpherical(), 0);
//   setUpperErrorsSpherical(other.getUpperErrorsSpherical(), 0);
  
//   setLowerErrorsCartesian(other.getLowerErrorsCartesian(), 0);
//   setUpperErrorsCartesian(other.getUpperErrorsCartesian(), 0);

  setCovMatSpherical(other.getCovMatSpherical());
//   setAsymmetrySpherical(other.getAsymmetrySpherical());
  calculateCartesian();
  
  setConfidence(other.getConfidence());
  
  if(other.hasRealPos()) setRealPosition(other.getRealPosition());
  
}
  
VertexPos::~VertexPos(){
 
  _covMatSpherical.clear();
  
}

VertexPos &VertexPos::operator=(const VertexPos &other){
 
  VertexPos temp(other);
  
  swap(*this, temp);
  
  return *this;
  
}

void VertexPos::adoptPreviousPos(VertexPos *other, double expand_bounds){
 
  initialize();
  
  if(other==0) return;
  
  *this=*((Pos*) other); // use copy constructor for Pos part of "this"
      
//   setLowerErrorsSpherical(other->getLowerErrorsSpherical(expand_bounds), 0);
//   setUpperErrorsSpherical(other->getUpperErrorsSpherical(expand_bounds), 0);
  
//   setLowerErrorsCartesian(other->getLowerErrorsCartesian(expand_bounds), 0);
//   setUpperErrorsCartesian(other->getUpperErrorsCartesian(expand_bounds), 0);
  
  setCovMatSpherical(other->getCovMatSpherical(expand_bounds));
  calculateCartesian();
  
  setConfidence(other->getConfidence());
  
  if(other->hasRealPos()) setRealPosition(other->getRealPosition());

  
}

void swap(VertexPos &first, VertexPos &second){
 
  VertexPos temp(first);
  
  first.setCartesian(second.getCartesian());
//   first.setUpperErrorsSpherical(second.getUpperErrorsSpherical(), 0);
//   first.setLowerErrorsSpherical(second.getLowerErrorsSpherical(), 0);
//   first.setUpperErrorsCartesian(second.getUpperErrorsCartesian(), 0);
//   first.setLowerErrorsCartesian(second.getLowerErrorsCartesian(), 0);
  first.setCovMatSpherical(second.getCovMatSpherical());
//   first.setAsymmetrySpherical(second.getAsymmetrySpherical());
  first.setConfidence(second.getConfidence());
  if(second.hasRealPos()) first.setRealPosition(second.getRealPosition());
  
  second.setCartesian(temp.getCartesian());
//   second.setUpperErrorsSpherical(temp.getUpperErrorsSpherical(), 0);
//   second.setLowerErrorsSpherical(temp.getLowerErrorsSpherical(), 0);
//   second.setUpperErrorsCartesian(temp.getUpperErrorsCartesian(), 0);
//   second.setLowerErrorsCartesian(temp.getLowerErrorsCartesian(), 0);
  second.setCovMatSpherical(temp.getCovMatSpherical());
//   second.setAsymmetrySpherical(temp.getAsymmetrySpherical());
  second.setConfidence(temp.getConfidence());
  if(temp.hasRealPos()) second.setRealPosition(temp.getRealPosition());
   
}

void VertexPos::initialize(){ 
  
  // default error values... 
  double r_error=2500;
  double theta_error=90*DEGTORAD;
  double phi_error=180*DEGTORAD;
  
  _covMatSpherical.clear();
  _covMatCartesian.clear();
  
  for(int a=0; a<3;a++){ 
    
    _covMatSpherical.push_back(std::vector<double>(3,0));
    _covMatCartesian.push_back(std::vector<double>(3,0));
    
  }
  
//   _covMatSpherical[0][0]=std::pow(r_error,2);
//   _covMatSpherical[0][1]=0;
//   _covMatSpherical[0][2]=0;
//   _covMatSpherical[1][0]=0;
//   _covMatSpherical[1][1]=std::pow(theta_error,2);
//   _covMatSpherical[1][2]=0;
//   _covMatSpherical[2][0]=0;
//   _covMatSpherical[2][1]=0;
//   _covMatSpherical[2][2]=std::pow(phi_error,2);

  setErrorsSpherical(r_error, theta_error, phi_error);
  
  calculateCartesian();
  
  _has_real_pos=0;
  
  _confidence=0;
  
}
    
void VertexPos::printout(int coordinate_system) const {
 
//   Pos::printout();
  
  int w=11;
  
  std::vector< std::vector<double> > matrix;
  
  if(coordinate_system==VertexFinder::CARTESIAN){
  
    matrix=getCovMatCartesian();
    
//     std::cout<<" x= "<<std::setw(w)<<getX()<<" +- "<<std::setw(w)<<getLowerErrorX()<<" / "<<std::setw(w)<<getUpperErrorX();
    std::cout<<" x= "<<std::setw(w)<<getX()<<" +- "<<std::setw(w)<<getErrorX();
    std::cout<<"   | ";
    for(int a=0;a<3;a++) std::cout<<"  "<<std::setw(w)<<matrix[0][a];
    std::cout<<" |   ";
    if(hasRealPos()) std::cout<<"  residual= "<<std::setw(w)<<getResidualX();
    std::cout<<std::endl;
    
//     std::cout<<" y= "<<std::setw(w)<<getY()<<" +- "<<std::setw(w)<<getLowerErrorY()<<" / "<<std::setw(w)<<getUpperErrorY();
    std::cout<<" y= "<<std::setw(w)<<getY()<<" +- "<<std::setw(w)<<getErrorY();
    std::cout<<"   | ";
    for(int a=0;a<3;a++) std::cout<<"  "<<std::setw(w)<<matrix[1][a];
    std::cout<<" |   ";
    if(hasRealPos()) std::cout<<"  residual= "<<std::setw(w)<<getResidualY();
    std::cout<<std::endl;
    
//     std::cout<<" z= "<<std::setw(w)<<getZ()<<" +- "<<std::setw(w)<<getLowerErrorZ()<<" / "<<std::setw(w)<<getUpperErrorZ();
    std::cout<<" z= "<<std::setw(w)<<getZ()<<" +- "<<std::setw(w)<<getErrorZ();
    std::cout<<"   | ";
    for(int a=0;a<3;a++) std::cout<<"  "<<std::setw(w)<<matrix[2][a];
    std::cout<<" |   ";
    if(hasRealPos()) std::cout<<"  residual= "<<std::setw(w)<<getResidualZ();
    std::cout<<std::endl;
  
  }
  
  if(coordinate_system==VertexFinder::SPHERICAL){
    
    matrix=getCovMatSpherical();
    
//     std::cout<<"     R= "<<std::setw(w)<<getR()<<" +- "<<std::setw(w)<<getLowerErrorR()<<" / "<<std::setw(w)<<getUpperErrorR();
    std::cout<<"     R= "<<std::setw(w)<<getR()<<" +- "<<std::setw(w)<<getErrorR();
    std::cout<<"   | ";
    std::cout<<"  "<<std::setw(w)<<matrix[0][0];
    std::cout<<"  "<<std::setw(w)<<matrix[0][1]*RADTODEG;
    std::cout<<"  "<<std::setw(w)<<matrix[0][2]*RADTODEG;
    std::cout<<" |   ";
    if(hasRealPos()) std::cout<<"  residual= "<<std::setw(w)<<getRealR()-getR();
    std::cout<<std::endl;
    
//     std::cout<<" theta= "<<std::setw(w)<<getTheta()*RADTODEG<<" +- "<<std::setw(w)<<getLowerErrorTheta()*RADTODEG<<" / "<<std::setw(w)<<getUpperErrorTheta()*RADTODEG;
    std::cout<<" theta= "<<std::setw(w)<<getTheta()*RADTODEG<<" +- "<<std::setw(w)<<getErrorTheta()*RADTODEG;
    std::cout<<"   | ";
    std::cout<<"  "<<std::setw(w)<<matrix[1][0]*RADTODEG;
    std::cout<<"  "<<std::setw(w)<<matrix[1][1]*RADTODEG*RADTODEG;
    std::cout<<"  "<<std::setw(w)<<matrix[1][2]*RADTODEG*RADTODEG;
    std::cout<<" |   ";
    if(hasRealPos()) std::cout<<"  residual= "<<std::setw(w)<<(getRealTheta()-getTheta())*RADTODEG;
    std::cout<<std::endl;
    
//     std::cout<<"   phi= "<<std::setw(w)<<getPhi()*RADTODEG<<" +- "<<std::setw(w)<<getLowerErrorPhi()*RADTODEG<<" / "<<std::setw(w)<<getUpperErrorPhi()*RADTODEG;
    std::cout<<"   phi= "<<std::setw(w)<<getPhi()*RADTODEG<<" +- "<<std::setw(w)<<getErrorPhi()*RADTODEG;
    std::cout<<"   | ";
    std::cout<<"  "<<std::setw(w)<<matrix[2][0]*RADTODEG;
    std::cout<<"  "<<std::setw(w)<<matrix[2][1]*RADTODEG*RADTODEG;
    std::cout<<"  "<<std::setw(w)<<matrix[2][2]*RADTODEG*RADTODEG;
    std::cout<<" |   ";
    if(hasRealPos()) std::cout<<"  residual= "<<std::setw(w)<<(getRealPhi()-getPhi())*RADTODEG;
    std::cout<<std::endl;
    
  }

    
  }
 
VertexPos *VertexPos::meanVertexPos(std::vector<VertexPos*> positions, int combine_method){
 
  int numPositions=positions.size();
  
  if(numPositions==0){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: the positions given are empty!"<<std::endl;
    return 0;
    
  }
  
  if(numPositions==1){ 
    
    return new VertexPos(*positions[0]);
    
  }
  
  if(combine_method==BEST){
  
    double best_confidence=0;
    int best_index=-1;
    
    for(int p=0;p<numPositions;p++){
     
      if(positions[p]->getConfidence()>best_confidence){
	
	best_confidence=positions[p]->getConfidence();
	best_index=p;
	
      }
      
    }// for p
    
    if(best_index<0){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: all positions have confidence zero!\n"; return 0; }
    
    else return new VertexPos(*positions[best_index]);
    
  }// if BEST
  
  else if(combine_method==AVERAGE){
    
    double sumConfidences=0;
  
    for(int p=0;p<numPositions; p++){
    
      sumConfidences+=positions[p]->getConfidence();
    
    }// for p
  
    if(sumConfidences==0){ std::cerr<<__PRETTY_FUNCTION__<<"  ERROR: all positions have confidence=0.\n"; return 0; }
  
    double x=0;
    double y=0;
    double z=0;
  
    double sumCovariances[3][3]={0};
  
    std::vector< std::vector<double> > matrix;
  
    for(int p=0; p<positions.size(); p++){
    
      double conf=positions[p]->getConfidence();
        
      x+=positions[p]->getX()*conf;
      y+=positions[p]->getY()*conf;
      z+=positions[p]->getZ()*conf;
    
      matrix=positions[p]->getCovMatSpherical();
    
      for(int i=0;i<3;i++){
      
	for(int j=0;j<3;j++){
	
	  sumCovariances[i][j]+=matrix[i][j]*conf;
	
	}// for j
    
      }// for i
       
    }// for p
  
    x/=sumConfidences;
    y/=sumConfidences;
    z/=sumConfidences;
  
    for(int i=0;i<3;i++){
    
      for(int j=0;j<3;j++){
      
	sumCovariances[i][j]/=sumConfidences;
      
	matrix[i][j]=sumCovariances[i][j];
      
      }// for j
    
    }// for i
  
    VertexPos *new_pos=new VertexPos(x,y,z);
    new_pos->setCovMatSpherical(matrix);
  
    for(int p=0;p<numPositions;p++){
    
      if(positions[p]->hasRealPos()){ 
      
	new_pos->setRealPosition(positions[p]->getRealPosition());
      
	break;
      
      }
     
    
    }// for p
  
    return new_pos;
  
  }// if AVERAGE
  
  else{
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: unrecognized combine_method="<<combine_method<<" use AVERAGE or BEST etc...\n";
    return 0;
    
  }
    
}

VertexPos *VertexPos::meanVertexPos(std::vector<VertexFinder*> positions, int combine_method){

  std::vector<VertexPos*> new_positions;
  positions.reserve(positions.size());
  
  for(int i=0;i<positions.size();i++) new_positions.push_back(positions[i]);
  
  return meanVertexPos(new_positions, combine_method);
  
}

// getters (cartesian)
std::vector< std::vector<double> > VertexPos::getCovMatCartesian(double expansion_factor) const {

  std::vector< std::vector<double> > matrix=_covMatCartesian;
  
  for(int i=0; i<3; i++){ 
    
    for(int j=0; j<3; j++){
      
      matrix[i][j]*=std::pow(expansion_factor,2);
      
    }
    
  }
  
  return matrix;
  
}

// std::vector<double> VertexPos::getAsymmetryCartesian() const{
//   
//   std::vector<double> vec;
//   vec.push_back(_asymmetry_x);
//   vec.push_back(_asymmetry_y);
//   vec.push_back(_asymmetry_z);
//   
//   return vec;
//   
// }

double VertexPos::getErrorX(double expand) const {
 
//   return expand*(_lower_error_x + _upper_error_x)/2;
  return expand*_error_x;
  
}

double VertexPos::getErrorY(double expand) const {
    
//   return expand*(_lower_error_y + _upper_error_y)/2;
  return expand*_error_y;
  
}

double VertexPos::getErrorZ(double expand) const {
    
//   return expand*(_lower_error_z + _upper_error_z)/2;
  return expand*_error_z;
  
}
  
std::vector<double> VertexPos::getErrorsCartesian(double expand) const {
  
  std::vector<double> vec(3,0);
  
  vec[0]=getErrorX(expand);
  vec[1]=getErrorY(expand);
  vec[2]=getErrorZ(expand);
  
  return vec;
  
}
  
// double VertexPos::getLowerErrorX(double expand) const {
//  
//   return expand*_lower_error_x;
//   
// }
// 
// double VertexPos::getLowerErrorY(double expand) const {
//   
//   return expand*_lower_error_y;
//   
// }
//   
// double VertexPos::getLowerErrorZ(double expand) const {
//   
//   return expand*_lower_error_z;
//   
// }
// 
// std::vector<double> VertexPos::getLowerErrorsCartesian(double expand) const {
//   
//   std::vector<double> vec(3,0);
//   
//   vec[0]=getLowerErrorX(expand);
//   vec[1]=getLowerErrorY(expand);
//   vec[2]=getLowerErrorZ(expand);
//   
//   return vec;
//   
// }
//     
double VertexPos::getLowerBoundX(double expand) const {
  
  return getX() - getErrorX(expand);
  
}
  
double VertexPos::getLowerBoundY(double expand) const {
  
  return getY() - getErrorY(expand);
  
}
  
double VertexPos::getLowerBoundZ(double expand) const {
  
  return getZ() - getErrorZ(expand);
  
}

std::vector<double> VertexPos::getLowerBoundsCartesian(double expand) const {
  
  std::vector<double> vec(3,0);
  
  vec[0]=getLowerBoundX(expand);
  vec[1]=getLowerBoundY(expand);
  vec[2]=getLowerBoundZ(expand);
  
  return vec;
  
}
  
// double VertexPos::getUpperErrorX(double expand) const {
//   
//   return expand*_upper_error_x;
//   
// }
// 
// double VertexPos::getUpperErrorY(double expand) const {
//   
//   return expand*_upper_error_y;  
//   
// }
// 
// double VertexPos::getUpperErrorZ(double expand) const{
//   
//   return expand*_upper_error_z;
//   
// }
//   
// std::vector<double> VertexPos::getUpperErrorsCartesian(double expand) const {
//   
//   std::vector<double> vec(3,0);
//   
//   vec[0]=getUpperErrorX(expand);
//   vec[1]=getUpperErrorY(expand);
//   vec[2]=getUpperErrorZ(expand);
//   
//   return vec;
//   
// }
    
double VertexPos::getUpperBoundX(double expand) const {
  
  return getX() + getErrorX(expand);
  
}
  
double VertexPos::getUpperBoundY(double expand) const {
  
  return getY() + getErrorY(expand);
  
}

double VertexPos::getUpperBoundZ(double expand) const {
  
  return getZ() + getErrorZ(expand);
  
}
  
std::vector<double> VertexPos::getUpperBoundsCartesian(double expand) const {
  
  std::vector<double> vec(3,0);
  
  vec[0]=getUpperBoundX(expand);
  vec[1]=getUpperBoundY(expand);
  vec[2]=getUpperBoundZ(expand);
  
  return vec;
  
}
  
std::vector<Pos> VertexPos::getBoundsBoxSides(double expand) const {
  
  std::cout<<__PRETTY_FUNCTION__<<" This isn't implemented yet!"<<std::endl;
  
  std::vector<Pos> vec;
  
  return vec;
  
}

std::vector<Pos> VertexPos::getBoundsBoxCorners(double expand) const {
  
  std::cout<<__PRETTY_FUNCTION__<<" This isn't implemented yet!"<<std::endl;
  
  std::vector<Pos> vec;
  
  return vec;
  
}
  
// getters (spherical)
std::vector< std::vector<double> > VertexPos::getCovMatSpherical(double expansion_factor) const {
  
  std::vector< std::vector<double> > matrix=_covMatSpherical;
  
  for(int i=0; i<3; i++){ 
    
    for(int j=0; j<3; j++){
      
      matrix[i][j]*=std::pow(expansion_factor,2);
      
    }
    
  }
  
  return matrix;
  
}

// std::vector<double> VertexPos::getAsymmetrySpherical() const{
//  
//   std::vector<double> vec;
//   vec.push_back(_asymmetry_R);
//   vec.push_back(_asymmetry_Theta);
//   vec.push_back(_asymmetry_Phi);
//   
//   return vec;
//   
// }

double VertexPos::getErrorR(double expand) const {
  
//   return expand*(_lower_error_R + _upper_error_R)/2;
  return expand*_error_R;
  
}
  
double VertexPos::getErrorTheta(double expand) const {
  
//   return expand*(_lower_error_Theta + _upper_error_Theta)/2;
  return expand*_error_theta;
  
}
  
double VertexPos::getErrorPhi(double expand) const {
  
//   return expand*(_lower_error_Phi + _upper_error_Phi)/2;
  return expand*_error_phi;
  
}

std::vector<double> VertexPos::getErrorsSpherical(double expand) const {
  
  std::vector<double> vec(3,0);
  vec[0]=getErrorR(expand);
  vec[1]=getErrorTheta(expand);
  vec[2]=getErrorPhi(expand);
  
  return vec;
  
}
  
// double VertexPos::getLowerErrorR(double expand) const {
//   
//   return expand*_lower_error_R;
//   
// }
// 
// double VertexPos::getLowerErrorTheta(double expand) const {
//   
//   return expand*_lower_error_Theta;
//   
// }
// 
// double VertexPos::getLowerErrorPhi(double expand) const {
//   
//   return expand*_lower_error_Phi;
//   
// }
// 
// std::vector<double> VertexPos::getLowerErrorsSpherical(double expand) const {
//     
//   std::vector<double> vec(3,0);
//   vec[0]=getLowerErrorR(expand);
//   vec[1]=getLowerErrorTheta(expand);
//   vec[2]=getLowerErrorPhi(expand);
//   
//   return vec;
//   
// }
    
double VertexPos::getLowerBoundR(double expand) const {
  
  double low_r=getR() - getErrorR(expand);
  
  if(low_r<10) low_r=10; // minimal R bound...
  
  return low_r;
  
}

double VertexPos::getLowerBoundTheta(double expand) const {
  
  double low_theta=getTheta() - getErrorTheta(expand);
  
  // add checks here...
  
  return low_theta;
  
}

double VertexPos::getLowerBoundPhi(double expand) const {
  
  double low_phi=getPhi() - getErrorPhi(expand);
  
  // add checks here...
  
  return low_phi;
  
}

std::vector<double> VertexPos::getLowerBoundsSpherical(double expand) const{
      
  std::vector<double> vec(3,0);
  vec[0]=getLowerBoundR(expand);
  vec[1]=getLowerBoundTheta(expand);
  vec[2]=getLowerBoundPhi(expand);
  
  return vec;
  
}
  
// double VertexPos::getUpperErrorR(double expand) const {
// 
//   return expand*_upper_error_R;
//   
// }
// 
// double VertexPos::getUpperErrorTheta(double expand) const {
// 
//   return expand*_upper_error_Theta;
//   
// }
//   
// double VertexPos::getUpperErrorPhi(double expand) const {
//   
//   return expand*_upper_error_Phi;
//   
// }
//   
// std::vector<double> VertexPos::getUpperErrorsSpherical(double expand) const {
//       
//   std::vector<double> vec(3,0);
//   vec[0]=getUpperErrorR(expand);
//   vec[1]=getUpperErrorTheta(expand);
//   vec[2]=getUpperErrorPhi(expand);
//   
//   return vec;
//   
// }
    
double VertexPos::getUpperBoundR(double expand) const {
      
  double high_r=getR() + getErrorR(expand);
  
  // add checks here...
  
  return high_r;
  
}

double VertexPos::getUpperBoundTheta(double expand) const {
    
  double high_theta=getTheta() + getErrorTheta(expand);
  
  // add checks here...
  
  return high_theta;
  
}

double VertexPos::getUpperBoundPhi(double expand) const {
  
  double high_phi=getPhi() + getErrorPhi(expand);
  
  // add checks here...
  
  return high_phi;
  
}

std::vector<double> VertexPos::getUpperBoundsSpherical(double expand) const {
  
  std::vector<double> vec(3,0);
  vec[0]=getUpperBoundR(expand);
  vec[1]=getUpperBoundTheta(expand);
  vec[2]=getUpperBoundPhi(expand);
  
  return vec;
  
  
}
  
std::vector<Pos> VertexPos::getBoundsTrapezoidSides(double expand) const {
  
  std::cout<<__PRETTY_FUNCTION__<<" This isn't implemented yet!"<<std::endl;
  
}

std::vector<Pos> VertexPos::getBoundsTrapezoidCorners(double expand) const {
  
  std::cout<<__PRETTY_FUNCTION__<<" This isn't implemented yet!"<<std::endl;
  
}

// get real
int VertexPos::hasRealPos() const {
  
  return _has_real_pos; 
  
}

double VertexPos::getRealX() const {
  
  if(_has_real_pos) return _real_position.getX();
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getRealY() const {
  
  if(_has_real_pos) return _real_position.getY();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getRealZ() const {
  
  if(_has_real_pos) return _real_position.getZ();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getRealR() const {
   
  if(_has_real_pos) return _real_position.getR();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getRealTheta() const {
  
  if(_has_real_pos) return _real_position.getTheta();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getRealPhi() const {
  
  if(_has_real_pos) return _real_position.getPhi();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}double VertexPos::getResidualX() const {
  
  if(_has_real_pos) return _real_position.getX() - getX();
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getResidualY() const {
  
  if(_has_real_pos) return _real_position.getY() - getY();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getResidualZ() const {
  
  if(_has_real_pos) return _real_position.getZ() - getZ();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getResidualR() const {
   
  if(_has_real_pos) return _real_position.getR() - getR();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getResidualTheta() const {
  
  if(_has_real_pos) return _real_position.getTheta() - getPhi();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

double VertexPos::getResidualPhi() const {
  
  if(_has_real_pos) return _real_position.getPhi() - getPhi();
  
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this VertexPos does not have a real position!"<<std::endl;
    return 0;
    
  }
  
}

Pos VertexPos::getRealPosition() const {
  
  return _real_position;
  
}

double VertexPos::getConfidence() const {
  
  return _confidence;
  
}

// setters (cartesian)
void VertexPos::setCovMatCartesian(std::vector< std::vector<double> > covMatrix){
  
  _covMatCartesian=covMatrix;
  
  _error_x=sqrt(covMatrix[0][0]);
  _error_y=sqrt(covMatrix[1][1]);
  _error_z=sqrt(covMatrix[2][2]);
  
//   _lower_error_x=sqrt(covMatrix[0][0]);
//   _lower_error_y=sqrt(covMatrix[1][1]);
//   _lower_error_z=sqrt(covMatrix[2][2]);
//   
//   _upper_error_x=sqrt(covMatrix[0][0]);
//   _upper_error_y=sqrt(covMatrix[1][1]);
//   _upper_error_z=sqrt(covMatrix[2][2]);
    
  calculateSpherical();
  
}

void VertexPos::setErrorInCartesianMatrix(double error, int position){
  
  // enter the value (squared) into the right diagonal place, zero out the off diagonals (assumes error is uncorrelated)
  
  for(int i=0;i<3;i++){
    
    for(int j=0;j<3;j++){
      
      if(i==position || j==position) _covMatCartesian[i][j]=0;
      if(i==position && j==position) _covMatCartesian[i][j]=std::pow(error, 2);
      
    }
    
  }
  
}

void VertexPos::setErrorInCartesianMatrix(double ex, double ey, double ez){
 
  for(int i=0;i<3;i++){
    
    for(int j=0;j<3;j++){
      
      if(i==j && i==0) _covMatCartesian[i][j]=std::pow(ex, 2);
      if(i==j && i==1) _covMatCartesian[i][j]=std::pow(ey, 2);
      if(i==j && i==2) _covMatCartesian[i][j]=std::pow(ez, 2);
      if(i!=j) _covMatCartesian[i][j]=0;
      
    }
    
  }
  
}

// void VertexPos::setAsymmetryCartesian(std::vector<double> asymmetry_factors){
//   
//   _asymmetry_x=asymmetry_factors[0];
//   _asymmetry_y=asymmetry_factors[1];
//   _asymmetry_z=asymmetry_factors[2];
//   
//   // what about the spherical asymmetry??
//   
// }
 
void VertexPos::calculateCartesian(){
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
//   double factor=TMath::ChisquareQuantile(0.68,1);
  
  calculateCartesianCovMatrix();
  
  _error_x=sqrt(_covMatCartesian[0][0]);
  _error_y=sqrt(_covMatCartesian[1][1]);
  _error_z=sqrt(_covMatCartesian[2][2]);
  
//   _lower_error_x=sqrt(_covMatCartesian[0][0]);
//   _lower_error_y=sqrt(_covMatCartesian[1][1]);
//   _lower_error_z=sqrt(_covMatCartesian[2][2]);
//   
//   _upper_error_x=sqrt(_covMatCartesian[0][0]);
//   _upper_error_y=sqrt(_covMatCartesian[1][1]);
//   _upper_error_z=sqrt(_covMatCartesian[2][2]);
  
  // what to do with asymmetry??
  
}
 
void VertexPos::calculateCartesianCovMatrix(){
 
//   std::cout<<"VertexPos::calculateCartesianCovMatrix()"<<std::endl;
  
  double cost=cos(getTheta());
  double cost2=pow(cost,2);
  
  double sint=sin(getTheta());
  double sint2=pow(sint,2);
  
  double cosp=cos(getPhi());
  double cosp2=pow(cosp,2);
  
  double sinp=sin(getPhi());
  double sinp2=pow(sinp,2);
  
  double r=getR();
  
  double R[3][3]; // Jacobian transformation matrix
  R[0][0]=sint*cosp;
  R[0][1]=r*cost*cosp;
  R[0][2]=-r*sint*sinp;
  R[1][0]=sint*sinp;
  R[1][1]=r*cost*sinp;
  R[1][2]=r*sint*cosp;
  R[2][0]=cost;
  R[2][1]=-r*sint;
  R[2][2]=0;

  
  double RT[3][3]; // R transpose
  for(int i=0;i<3;i++){
    
    for(int j=0;j<3;j++){
      
      RT[i][j]=R[j][i];
      
    }
    
  }

  double temp[3][3]={0};
  
  for(int i=0;i<3;i++){
    
    for(int j=0;j<3;j++){
      
      for(int k=0;k<3;k++){
	
	temp[i][j]+=R[i][k]*_covMatSpherical[k][j];// multiply temp by R
	
      }// for k
      
    }// for j
    
  }// for i
    
  for(int i=0;i<3;i++){
    
    for(int j=0;j<3;j++){
      
      _covMatCartesian[i][j]=0;// initialize matrix
      
      for(int k=0;k<3;k++){
	
	_covMatCartesian[i][j]+=temp[i][k]*RT[k][j];// multiply temp by the transverse of R
	
      }// for k
            
    }// for j
    
  }// for i

}

void VertexPos::setErrorX(double ex, int calculate){
  
  _error_x=ex;
  
//   setLowerErrorX(ex);
//   setUpperErrorX(ex);
  
  setErrorInCartesianMatrix(ex, 0);
  
  if(calculate){ calculateSpherical(); }
  
}

void VertexPos::setErrorY(double ey, int calculate){ 
  
  _error_y=ey;
  
//   setLowerErrorY(ey);
//   setUpperErrorY(ey);
  
  setErrorInCartesianMatrix(ey, 1);
  
  if(calculate){ calculateSpherical(); }
  
}
  
void VertexPos::setErrorZ(double ez, int calculate){
  
  _error_z=ez;
//   setLowerErrorZ(ez);
//   setUpperErrorZ(ez);
  
  setErrorInCartesianMatrix(ez, 2);
    
  if(calculate){ calculateSpherical(); }
  
}
  
void VertexPos::setErrorsCartesian(std::vector<double> errors_cartesian, int calculate){ 

  setErrorsCartesian(errors_cartesian[0], errors_cartesian[1], errors_cartesian[2], calculate);
  
}

void VertexPos::setErrorsCartesian(double ex, double ey, double ez, int calculate){
 
  _error_x=ex;
  _error_y=ey;
  _error_z=ez;
  
//   _lower_error_x=ex;
//   _upper_error_x=ex;
//   _lower_error_y=ey;
//   _upper_error_y=ey;
//   _lower_error_z=ez;
//   _upper_error_z=ez;
  
  setErrorInCartesianMatrix(ex, ey, ez);
      
  if(calculate){ calculateSpherical(); }
  
}

// void VertexPos::setLowerErrorX(double ex, int calculate){
//   
//   _lower_error_x=fabs(ex);
//   
//   setErrorInCartesianMatrix(ex, 0); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateSpherical(); }
//   
// }
//   
// void VertexPos::setLowerErrorY(double ey, int calculate){
//   
//   _lower_error_y=fabs(ey);
//   
//   setErrorInCartesianMatrix(ey, 1); // this needs to be improved for asymmetry
// 
//   
//   if(calculate){ calculateSpherical(); }
//   
// }
//   
// void VertexPos::setLowerErrorZ(double ez, int calculate){
//   
//   _lower_error_z=fabs(ez);
//   
//   setErrorInCartesianMatrix(ez, 2); // this needs to be improved for asymmetry
// 
//   if(calculate){ calculateSpherical(); }
//   
// }
//   
// void VertexPos::setLowerErrorsCartesian(std::vector<double> lower_errors_cartesian, int calculate){
//   
//   setLowerErrorsCartesian(lower_errors_cartesian[0], lower_errors_cartesian[1], lower_errors_cartesian[2], calculate);
//   
// }
// 
// void VertexPos::setLowerErrorsCartesian(double ex, double ey, double ez, int calculate){
//  
//   _lower_error_x=ex;
//   _lower_error_y=ey;
//   _lower_error_z=ez;
//   
//   setErrorInCartesianMatrix(ex, ey, ez); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateSpherical(); }
//     
// }
// 
// void VertexPos::setUpperErrorX(double ex, int calculate){
//   
//   _upper_error_x=fabs(ex);
//   
//   setErrorInCartesianMatrix(ex, 0); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateSpherical(); }
//   
// }
// 
// void VertexPos::setUpperErrorY(double ey, int calculate){
//   
//   _upper_error_y=fabs(ey);
//   
//   setErrorInCartesianMatrix(ey, 1); // this needs to be improved for asymmetry
// 
//   if(calculate){ calculateSpherical(); }
//   
// }
// 
// void VertexPos::setUpperErrorZ(double ez, int calculate){
//   
//   _upper_error_z=fabs(ez);
// 
//   setErrorInCartesianMatrix(ez, 2); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateSpherical(); }
//   
// }
// 
// void VertexPos::setUpperErrorsCartesian(std::vector<double> upper_errors_cartesian, int calculate){
//   
//   setUpperErrorsCartesian(upper_errors_cartesian[0], upper_errors_cartesian[1], upper_errors_cartesian[2], calculate);
//   
// }
// 
// void VertexPos::setUpperErrorsCartesian(double ex, double ey, double ez, int calculate){
// 
//   _upper_error_x=ex;
//   _upper_error_y=ey;
//   _upper_error_z=ez;
// 
//   setErrorInCartesianMatrix(ex, ey, ez); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateSpherical(); }
//   
// }

void VertexPos::setLowerBoundX(double ex, int calculate){
  
  if(ex>getX()){ std::cerr<<"ERROR: setLowerBoundX("<<ex<<") bigger than getX()= "<<getX()<<"\n"; return; }// check that bound doesn't exceed actual position
  
  setErrorX(getX()-ex, calculate);
  
}

void VertexPos::setLowerBoundY(double ey, int calculate){
    
  if(ey>getY()){ std::cerr<<"ERROR: setLowerBoundY("<<ey<<") bigger than getY()= "<<getY()<<"\n"; return; }// check that bound doesn't exceed actual position
  
  setErrorY(getY()-ey, calculate);
  
}

void VertexPos::setLowerBoundZ(double ez, int calculate){
    
  if(ez>getZ()){ std::cerr<<"ERROR: setLowerBoundZ("<<ez<<") bigger than getZ()= "<<getZ()<<"\n"; return; }// check that bound doesn't exceed actual position
  
  setErrorZ(getZ()-ez, calculate);
  
}
  
void VertexPos::setLowerBoundsCartesian(std::vector<double> lower_bounds_cartesian, int calculate){
     
    setLowerBoundsCartesian(lower_bounds_cartesian[0], lower_bounds_cartesian[1], lower_bounds_cartesian[2], calculate);
   
}

void VertexPos::setLowerBoundsCartesian(double ex, double ey, double ez, int calculate){
     
  if(ex>getX()){ std::cerr<<"ERROR: setLowerBoundX("<<ex<<") bigger than getX()= "<<getX()<<"\n"; return; }// check that bound doesn't exceed actual position
  if(ey>getY()){ std::cerr<<"ERROR: setLowerBoundY("<<ey<<") bigger than getY()= "<<getY()<<"\n"; return; }// check that bound doesn't exceed actual position
  if(ez>getZ()){ std::cerr<<"ERROR: setLowerBoundZ("<<ez<<") bigger than getZ()= "<<getZ()<<"\n"; return; }// check that bound doesn't exceed actual position

  setErrorsCartesian(getX()- ex, getY() - ey, getZ() -ez, calculate);

  if(calculate){ calculateSpherical(); }
   
}

void VertexPos::setUpperBoundX(double ex, int calculate){
    
  if(ex<getX()){ std::cerr<<"ERROR: setUpperBoundX("<<ex<<") smaller than getX()= "<<getX()<<"\n"; return; }// check that bound doesn't exceed actual position
  
  setErrorX(ex-getX(), calculate);
  
}
  
void VertexPos::setUpperBoundY(double ey, int calculate){
  
  if(ey<getY()){ std::cerr<<"ERROR: setUpperBoundY("<<ey<<") smaller than getY()= "<<getY()<<"\n"; return; }// check that bound doesn't exceed actual position
  
  setErrorY(ey-getY(), calculate);
  
}

void VertexPos::setUpperBoundZ(double ez, int calculate){
  
  if(ez<getZ()){ std::cerr<<"ERROR: setUpperBoundZ("<<ez<<") smaller than getZ()= "<<getZ()<<"\n"; return; }// check that bound doesn't exceed actual position
  
  setErrorZ(ez-getZ(), calculate);
  
}  

void VertexPos::setUpperBoundsCartesian(std::vector<double> upper_bounds_cartesian, int calculate){
        
  setUpperBoundsCartesian(upper_bounds_cartesian[0],upper_bounds_cartesian[1], upper_bounds_cartesian[2], calculate);
   
}

void VertexPos::setUpperBoundsCartesian(double ex, double ey, double ez, int calculate){
       
  if(ex<getX()){ std::cerr<<"ERROR: setUpperBoundX("<<ex<<") smaller than getX()= "<<getX()<<"\n"; return; }// check that bound doesn't exceed actual position
  if(ey<getY()){ std::cerr<<"ERROR: setUpperBoundY("<<ey<<") smaller than getY()= "<<getY()<<"\n"; return; }// check that bound doesn't exceed actual position
  if(ez<getZ()){ std::cerr<<"ERROR: setUpperBoundZ("<<ez<<") smaller than getZ()= "<<getZ()<<"\n"; return; }// check that bound doesn't exceed actual position

  setErrorsCartesian(ex - getX(), ey - getY(), ez - getZ(), calculate); 
    
  if(calculate){ calculateSpherical(); }
 
}

  // setters (spherical)
void VertexPos::setCovMatSpherical(std::vector< std::vector<double> > covMatrix, int calculate){
  
  _covMatSpherical=covMatrix;
    
  _error_R=sqrt(covMatrix[0][0]);
  _error_theta=sqrt(covMatrix[1][1]);
  _error_phi=sqrt(covMatrix[2][2]);
  
//   _lower_error_R=sqrt(covMatrix[0][0]);
//   _lower_error_Theta=sqrt(covMatrix[1][1]);
//   _lower_error_Phi=sqrt(covMatrix[2][2]);
//   
//   _upper_error_R=sqrt(covMatrix[0][0]);
//   _upper_error_Theta=sqrt(covMatrix[1][1]);
//   _upper_error_Phi=sqrt(covMatrix[2][2]);
  
  calculateCartesian();
  
}

void VertexPos::setErrorInSphericalMatrix(double error, int position){
   
  // enter the value (squared) into the right diagonal place, zero out the off diagonals (assumes error is uncorrelated)
  
  for(int i=0;i<3;i++){
    
    for(int j=0;j<3;j++){
      
      if(i==position || j==position) _covMatSpherical[i][j]=0;
      if(i==position && j==position) _covMatSpherical[i][j]=std::pow(error, 2);
      
    }
    
  }
  
}

void VertexPos::setErrorInSphericalMatrix(double e_R, double e_Theta, double e_Phi){
   
  for(int i=0;i<3;i++){
    
    for(int j=0;j<3;j++){
      
      if(i==j && i==0) _covMatSpherical[i][j]=std::pow(e_R, 2);
      if(i==j && i==1) _covMatSpherical[i][j]=std::pow(e_Theta, 2);
      if(i==j && i==2) _covMatSpherical[i][j]=std::pow(e_Phi, 2);
      if(i!=j) _covMatSpherical[i][j]=0;
      
    }
    
  }
  
}

// void VertexPos::setAsymmetrySpherical(std::vector<double> asymmetry_factors){
//   
//   _asymmetry_R=asymmetry_factors[0];
//   _asymmetry_Theta=asymmetry_factors[1];
//   _asymmetry_Phi=asymmetry_factors[2];
//   
// }
  
void VertexPos::calculateSpherical(){
 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this is not yet implemented..."<<std::endl;
  
}

void VertexPos::calculateSphericalCovMatrix(){
  
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this is not yet implemented..."<<std::endl;
  
}

void VertexPos::setErrorR(double e_R, int calculate){
  
  _error_R=e_R;
//   _lower_error_R=e_R;
//   _upper_error_R=e_R;
  
  setErrorInSphericalMatrix(e_R, 0);
  
  if(calculate){ calculateCartesian(); }
  
}

void VertexPos::setErrorTheta(double e_theta, int calculate){
  
  _error_theta=e_theta;
//   _lower_error_Theta=e_theta;
//   _upper_error_Theta=e_theta;
  
  setErrorInSphericalMatrix(e_theta, 1);
  
  if(calculate){ calculateCartesian(); }
  
}

void VertexPos::setErrorPhi(double e_phi, int calculate){
  
  _error_phi=e_phi;
//   _lower_error_Phi=e_phi;
//   _upper_error_Phi=e_phi;
  
  setErrorInSphericalMatrix(e_phi, 2);
  
  if(calculate){ calculateCartesian(); }
  
}

void VertexPos::setErrorsSpherical(std::vector<double> errors_spherical, int calculate){
  
  setErrorsSpherical(errors_spherical[0], errors_spherical[1], errors_spherical[2], calculate);
  
}
  
void VertexPos::setErrorsSpherical(double e_R, double e_theta, double e_phi, int calculate){

  _error_R=e_R;
//   _lower_error_R=e_R;
//   _upper_error_R=e_R;
  
  _error_theta=e_theta;
//   _lower_error_Theta=e_theta;
//   _upper_error_Theta=e_theta;
   
  _error_phi=e_phi;
//   _lower_error_Phi=e_phi;
//   _upper_error_Phi=e_phi;
  
  setErrorInSphericalMatrix(e_R, e_theta, e_phi);
  
  if(calculate){ calculateCartesian(); }
  
}

// void VertexPos::setLowerErrorR(double e_R, int calculate){
//   
//   _lower_error_R=e_R;
//   
//   setErrorInSphericalMatrix(e_R, 0); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateCartesian(); }
//   
// }
// 
// void VertexPos::setLowerErrorTheta(double e_theta, int calculate){
//   
//   _lower_error_Theta=e_theta;
//   
//   setErrorInSphericalMatrix(e_theta, 1); // this needs to be improved for asymmetry_factors
//   
//   if(calculate){ calculateCartesian(); }
//   
// }
// 
// void VertexPos::setLowerErrorPhi(double e_phi, int calculate){
//   
//   _lower_error_Phi=e_phi;
//   
//   setErrorInSphericalMatrix(e_phi, 2); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateCartesian(); }
//   
// }
//   
// void VertexPos::setLowerErrorsSpherical(std::vector<double> lower_errors_spherical, int calculate){
//   
//   setLowerErrorsSpherical(lower_errors_spherical[0], lower_errors_spherical[1], lower_errors_spherical[2], calculate);
//   
// }
//   
// void VertexPos::setLowerErrorsSpherical(double e_R, double e_theta, double e_phi, int calculate){
//   
//   _lower_error_R=e_R;
//   _lower_error_Theta=e_theta;
//   _lower_error_Phi=e_phi;
//   
//   setErrorInSphericalMatrix(e_R, e_theta, e_phi); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateCartesian(); }
//   
// }
//   
// void VertexPos::setUpperErrorR(double e_R, int calculate){
//   
//   _upper_error_R=e_R;
//   
//   setErrorInSphericalMatrix(e_R, 0); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateCartesian(); }
//   
// }
//   
// void VertexPos::setUpperErrorTheta(double e_theta, int calculate){
//   
//   _upper_error_Theta=e_theta;
//   
//   setErrorInSphericalMatrix(e_theta, 1); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateCartesian(); }
//   
// }
// 
// void VertexPos::setUpperErrorPhi(double e_phi, int calculate){
//   
//   
//   _upper_error_Phi=e_phi;
//   
//   setErrorInSphericalMatrix(e_phi, 2); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateCartesian(); }
//   
// }
// 
// void VertexPos::setUpperErrorsSpherical(std::vector<double> upper_errors_spherical, int calculate){
//   
//   setUpperErrorsSpherical(upper_errors_spherical[0], upper_errors_spherical[1], upper_errors_spherical[2], calculate);
//   
// }
//  
// void VertexPos::setUpperErrorsSpherical(double e_R, double e_theta, double e_phi, int calculate){
// 
//   _upper_error_R=e_R;
//   _upper_error_Theta=e_theta;
//   _upper_error_Phi=e_phi;
//   
//   setErrorInSphericalMatrix(e_R, e_theta, e_phi); // this needs to be improved for asymmetry
//   
//   if(calculate){ calculateCartesian(); }
//   
// }

void VertexPos::setLowerBoundR(double e_R, int calculate){
  
  if(e_R>getR()){ std::cerr<<"ERROR: setLowerBoundR("<<e_R<<") larger than getR()= "<<getR()<<"\n"; return; }
  
  setErrorR(getR() - e_R, calculate);
  
}
  
void VertexPos::setLowerBoundTheta(double e_theta, int calculate){
    
  if(e_theta>getTheta()){ std::cerr<<"ERROR: setLowerBoundTheta("<<e_theta<<") larger than getTheta()= "<<getTheta()<<"\n"; return; }
  
  setErrorTheta(getTheta() - e_theta, calculate);
  
}
  
void VertexPos::setLowerBoundPhi(double e_phi, int calculate){
    
  if(e_phi>getPhi()){ std::cerr<<"ERROR: setLowerBoundPhi("<<e_phi<<") larger than getPhi()= "<<getPhi()<<"\n"; return; }
  
  setErrorPhi(getPhi() - e_phi, calculate);
  
}
  
void VertexPos::setLowerBoundsSpherical(std::vector<double> lower_bounds_spherical, int calculate){
    
  setLowerBoundsSpherical(lower_bounds_spherical[0], lower_bounds_spherical[1], lower_bounds_spherical[2], calculate);
  
}
 
void VertexPos::setLowerBoundsSpherical(double e_R, double e_theta, double e_phi, int calculate){
      
  if(e_R>getR()){ std::cerr<<"ERROR: setLowerBoundR("<<e_R<<") larger than getR()= "<<getR()<<"\n"; return; }
  if(e_theta>getTheta()){ std::cerr<<"ERROR: setLowerBoundTheta("<<e_theta<<") larger than getTheta()= "<<getTheta()<<"\n"; return; }
  if(e_phi>getPhi()){ std::cerr<<"ERROR: setLowerBoundPhi("<<e_phi<<") larger than getPhi()= "<<getPhi()<<"\n"; return; }

  setErrorsSpherical(getR() - e_R, getTheta() - e_theta, getPhi() - e_phi, calculate);
  
  if(calculate){ calculateCartesian(); }
  
}
  
void VertexPos::setUpperBoundR(double e_R, int calculate){
      
  if(e_R<getR()){ std::cerr<<"ERROR: setUpperBoundR("<<e_R<<") smaller than getR()= "<<getR()<<"\n"; return; }
  
  setErrorTheta(e_R - getR(), calculate);
  
}
  
void VertexPos::setUpperBoundTheta(double e_theta, int calculate){
        
  if(e_theta<getTheta()){ std::cerr<<"ERROR: setUpperBoundTheta("<<e_theta<<") smaller than getTheta()= "<<getTheta()<<"\n"; return; }
  
  setErrorTheta(e_theta - getTheta(), calculate);
  
}

void VertexPos::setUpperBoundPhi(double e_phi, int calculate){
        
  if(e_phi<getPhi()){ std::cerr<<"ERROR: setUpperBoundPhi("<<e_phi<<") smaller than getPhi()= "<<getPhi()<<"\n"; return; }
  
  setErrorPhi(e_phi - getPhi(), calculate);
  
}

void VertexPos::setUpperBoundsSpherical(std::vector<double> upper_bounds_spherical, int calculate){
  
  setUpperBoundsSpherical(upper_bounds_spherical[0], upper_bounds_spherical[1], upper_bounds_spherical[2], calculate);
  
}
  
void VertexPos::setUpperBoundsSpherical(double e_R, double e_theta, double e_phi, int calculate){
    
  if(e_R<getR()){ std::cerr<<"ERROR: setUpperBoundR("<<e_R<<") smaller than getR()= "<<getR()<<"\n"; return; }
  if(e_theta<getTheta()){ std::cerr<<"ERROR: setUpperBoundTheta("<<e_theta<<") smaller than getTheta()= "<<getTheta()<<"\n"; return; }
  if(e_phi<getPhi()){ std::cerr<<"ERROR: setUpperBoundPhi("<<e_phi<<") smaller than getPhi()= "<<getPhi()<<"\n"; return; }

  setErrorsSpherical(e_R - getR(), e_theta - getTheta(), e_phi - getPhi(), calculate);
  
  if(calculate){ calculateCartesian(); }
  
}

void VertexPos::setRealPosition(Pos position){
  
//   std::cout<<__PRETTY_FUNCTION__<<" position passed"<<std::endl;
//   position.printout();
  
  _real_position=position;
  
  _has_real_pos=1;
  
}

void VertexPos::setConfidence(double confidence){
  
  if(confidence>1) _confidence=1;
  else if(confidence<0) _confidence=0;
  else _confidence=confidence;
  
}

int VertexPos::isOriginInBounds() const {
  
  if( getOriginX()>=getLowerBoundX() && getOriginX()<=getUpperBoundX() &&
      getOriginY()>=getLowerBoundY() && getOriginY()<=getUpperBoundY() &&
      getOriginZ()>=getLowerBoundZ() && getOriginZ()<=getUpperBoundZ() ) return 1;
  
  return 0; 
  
}

const std::string VertexPos::name_choose_result(int index){
 
  std::string names[]={"BEST","AVERAGE"};
  
  return names[index];
  
}

const double VertexPos::PI=3.14159265358979312;
const double VertexPos::DEGTORAD=1.74532925199432955e-02;
const double VertexPos::RADTODEG=5.72957795130823229e+01;
 
 
 
 
 
 
 
 
 
