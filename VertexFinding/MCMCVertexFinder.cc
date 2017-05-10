#include "MCMCVertexFinder.h"

ClassImp(MCMCVertexFinder);

int MCMCVertexFinder::debug_bit=1;

MCMCVertexFinder::MCMCVertexFinder(int coordinate_system, int time_input, int lock_parameters, OpticalIce *ice_ptr, double par1, double par2, double par3, int variant) : VertexFinder(coordinate_system, time_input, lock_parameters, ice_ptr, par1, par2, par3) {
  
  initialize();
  
  if(par1>100) _num_steps=par1;
  if(par2>100) _num_burn=par2;
  if(par3>0) _step_size=par3;
  
  if(_num_burn > 0.5*_num_steps) _num_burn=0.5*_num_steps; 
  
  
}

MCMCVertexFinder::MCMCVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameters, OpticalIce *ice_ptr, double par1, double par2, double par3, int variant) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameters, ice_ptr, par1, par2, par3,
 variant){
 
  initialize();
  
  if(par1>100) _num_steps=par1;
  if(par2>100) _num_burn=par2;
  if(par3>1) _step_size=par3;
  
  if(_num_burn > 0.5*_num_steps) _num_burn=0.5*_num_steps; 
  
}

MCMCVertexFinder::MCMCVertexFinder(const std::string commands) : VertexFinder() {
 
  initialize();
  
  parse(commands);
  
}

MCMCVertexFinder::MCMCVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands) : VertexFinder(channels, initial_position){
 
  initialize();
  
  _channels=channels;
  _initial_position=initial_position;
  
  parse(commands);
  
}

MCMCVertexFinder::MCMCVertexFinder(const MCMCVertexFinder &other) : VertexFinder(other){
 
  _this_point=other._this_point;
  _next_point=other._next_point;
  
  _num_iterations=other._num_iterations;
  _num_steps=other._num_steps;
  _num_burn=other._num_burn;
  _step_size=other._step_size;
  
  _points=other._points;
  
  _rand=new TRandom3(*other._rand);
  
}

MCMCVertexFinder::~MCMCVertexFinder(){
  
  _points.clear();
  
  if(_rand) delete _rand;
  
}

VertexFinder *MCMCVertexFinder::getDeepCopy(){
 
  return new MCMCVertexFinder(*this);
  
}

void MCMCVertexFinder::initialize(){
 
  _rand=new TRandom3(0);
  
  _num_iterations=1e6;
  _num_steps=1e4;
  _num_burn=2e3;
  _step_size=10;
  
  
//   reset();
  
}

void MCMCVertexFinder::reset(){
 
  _this_point=*_initial_position;
  _points.clear();
  _points.reserve(_num_steps - _num_burn);
  
  // TODO: add step size that adjusts based on the search region size...
  
  VertexFinder::reset();
  
}

void MCMCVertexFinder::help(){
 
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  std::cout<<"Markov Chain Monte Carlo vertex finder.\n Use par1: numSteps. par2: numBurn. par3=step size (angular steps are translated to length, in meters at the location)"<<std::endl;
  std::cout<<" use variant 0 for unbinned unconstrained search, variant 1 for binned statistics, with cyclical bounds."<<std::endl;
  
}

void MCMCVertexFinder::parse(const std::string commands){
 
  VertexFinder::parse(commands);
    
  std::vector< std::vector< std::string> > parsed = splitCommands(commands);
  
  for(int i=0;i<parsed.size();i++){
    
    if(check_str(parsed[i][0], "iterations","num_iterations")){ setNumIterations(stoi(parsed[i][1]));}
    if(check_str(parsed[i][0], "points", "num_points","num_steps")){ setNumSteps(stoi(parsed[i][1]));}
    if(check_str(parsed[i][0], "burn","num_burn")){ setNumBurn(stoi(parsed[i][1]));}    
    if(check_str(parsed[i][0], "step_size")){ setStepSize(stof(parsed[i][1]));}
    
  }
}

// getters
int MCMCVertexFinder::getNumIterations() const {
  
  return _num_iterations;
  
}

int MCMCVertexFinder::getNumSteps() const {
  
  return _num_steps;
  
}

int MCMCVertexFinder::getNumBurn() const {
  
  return _num_burn;
  
}

double MCMCVertexFinder::getStepSize() const {
  
  return _step_size;
  
}


// setters
void MCMCVertexFinder::setNumIterations(int num){
  
  _num_iterations=num;
  
}

void MCMCVertexFinder::setNumSteps(int num){
  
  _num_steps=num;

}

void MCMCVertexFinder::setNumBurn(int num){
  
  _num_burn=num;
  
}

void MCMCVertexFinder::setStepSize(double size){
  
  _step_size=size;
  
}

void MCMCVertexFinder::findVertex(){
 
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  reset();
  
  if(checkIfSomethingMissing()) {
    setStatusFailed();
    return;
  }
  
  int i=0; // in this scope to know how many iterations actually happened before the break...
  int j=0;
  
  double this_log_likelihood;
  double next_log_likelihood;
  
  double bestChi=1e36;
  
  if(isUsingHitTimes()) this_log_likelihood=-0.5*myChi(_this_point, getInteractionTime(_this_point));
  else this_log_likelihood=-0.5*myChi(_this_point);
  
  for(i=0;i<_num_iterations;i++){
    
    takeRandomStep(i%3); // take a random step one axis at a time, more likely to converge this way
    
    double chi;
    
    if(isUsingHitTimes()) chi=myChi(_next_point, getInteractionTime(_next_point));
    else chi=myChi(_next_point);
  
    if(chi<bestChi) bestChi=chi;
    
    next_log_likelihood=-0.5*chi;
        
    double ratio=std::exp(next_log_likelihood - this_log_likelihood);
    //double ratio = this_log_likelihood / next_log_likelihood;

    if( ratio > 1 - _rand->Rndm() ){// if true, move on to next point...

      _this_point=_next_point; // first off, move to next point
      this_log_likelihood=next_log_likelihood; // copy over the likelihood.
      
      j++; // burn sample counter
      
      if(j>_num_burn){
      
	_points.push_back(_next_point);
// 	switch(_lock_parameters){
// 	  
// 	  case VertexFinder::UNLOCK : _hScan->fill(_next_point.getX(), _next_point.getY(), _next_point.getZ()); break;
// 	  case VertexFinder::LOCK0  : _hScan->fill(_next_point.getY(), _next_point.getZ());
// 	  case VertexFinder::LOCK1  : _hScan->fill(_next_point.getX(), _next_point.getZ());
// 	  case VertexFinder::LOCK2  : _hScan->fill(_next_point.getX(), _next_point.getY());
// 	  case VertexFinder::LOCK01 : _hScan->fill(_next_point.getZ());
// 	  case VertexFinder::LOCK02 : _hScan->fill(_next_point.getY());
// 	  case VertexFinder::LOCK12 : _hScan->fill(_next_point.getX());
// 	  
// 	}
	
// 	_hScan->fill(_next_point.getX(), _next_point.getY(), _next_point.getZ());
	
	if(_points.size()>_num_steps) break; // stop after so many successful steps...
	
      }// if burn sample

    }// if move forward 
    
  }// for i
  
  if(debug_bit) std::cout<<"successful steps= "<<_points.size()<<"  total steps= "<<i<<"  ratio= "<<(double)_points.size()/i<<"  step= "<<_step_size<<"  burn= "<<_num_burn<<std::endl;
  
  
  VertexPos position(estimatePosition());
    
  if(_coordinate_system==VertexFinder::CARTESIAN) position.setCovMatCartesian(estimateErrors());
  if(_coordinate_system==VertexFinder::SPHERICAL) position.setCovMatSpherical(estimateErrors());
  
  setFoundPosition(position);
  calculateConfidence(bestChi);
  //  setCartesian(_this_point.getX(), _this_point.getY(), _this_point.getZ());
}

Pos MCMCVertexFinder::estimatePosition(){

  if(_points.size()<1){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: not enough points in the markov chain...\n"; }
  
  return Pos::getMidPoint(_points);
  
}

std::vector< std::vector<double> > MCMCVertexFinder::estimateErrors(){

  // assumes the position in "this" is the mean of the distribution...
  
  int N=_points.size();
  
  if(N<1){ std::cerr<<__PRETTY_FUNCTION__<<"  ERROR: not enough points in the markov chain...\n";}
  
  if(_coordinate_system==VertexFinder::CARTESIAN){
    
    double var_x=0;
    double var_y=0;
    double var_z=0;
    
    double cov_xy=0;
    double cov_xz=0;
    double cov_yz=0;
    
    for(int i=0;i<N;i++){
      
      var_x+=std::pow(_points[i].getX() - getX(), 2);
      var_y+=std::pow(_points[i].getY() - getY(), 2);
      var_z+=std::pow(_points[i].getZ() - getZ(), 2);
      
      cov_xy+=(_points[i].getX() - getX())*(_points[i].getY() - getY());
      cov_xz+=(_points[i].getX() - getX())*(_points[i].getZ() - getZ());
      cov_yz+=(_points[i].getY() - getY())*(_points[i].getZ() - getZ());
      
    }// for i
    
    var_x/=N;
    var_y/=N;
    var_z/=N;
    
    cov_xy/=N;
    cov_xz/=N;
    cov_yz/=N;
     
    std::vector< std::vector<double> > matrix;
    for(int a=0;a<3;a++) matrix.push_back(std::vector<double>(3,0));
    
    matrix[0][0]=var_x;
    matrix[0][1]=cov_xy;
    matrix[0][2]=cov_xz;
    
    matrix[1][0]=cov_xy;
    matrix[1][1]=var_y;
    matrix[1][2]=cov_yz;
    
    matrix[2][0]=cov_xz;
    matrix[2][1]=cov_yz;
    matrix[2][2]=var_z;
    
    return matrix;
    
  }
  
  if(_coordinate_system==VertexFinder::SPHERICAL){
   
    double var_R=0;
    double var_Theta=0;
    double var_Phi=0;
    
    double cov_R_Theta=0;
    double cov_R_Phi=0;
    double cov_Theta_Phi=0;
    
    for(int i=0;i<N;i++){
      
      var_R+=std::pow(_points[i].getR() - getR(), 2);
      var_Theta+=std::pow(_points[i].getTheta() - getTheta(), 2);
      var_Phi+=std::pow(_points[i].getPhi() - getPhi(), 2); // may have trouble with wrap around!
      
      cov_R_Theta+=(_points[i].getR() - getR())*(_points[i].getTheta() - getTheta());
      cov_R_Phi+=(_points[i].getR() - getR())*(_points[i].getPhi() - getPhi());
      cov_Theta_Phi+=(_points[i].getTheta() - getTheta())*(_points[i].getPhi() - getPhi());
      
    }// for i
    
    var_R/=N;
    var_Theta/=N;
    var_Phi/=N;
    
    cov_R_Theta/=N;
    cov_R_Phi/=N;
    cov_Theta_Phi/=N;
    
    std::vector< std::vector<double> > matrix;
    for(int a=0;a<3;a++) matrix.push_back(std::vector<double>(3,0));
    
    matrix[0][0]=var_R;
    matrix[0][1]=cov_R_Theta;
    matrix[0][2]=cov_R_Phi;
    
    matrix[1][0]=cov_R_Theta;
    matrix[1][1]=var_Theta;
    matrix[1][2]=cov_Theta_Phi;
    
    matrix[2][0]=cov_R_Phi;
    matrix[2][1]=cov_Theta_Phi;
    matrix[2][2]=var_Phi;
    
    return matrix;
    
  }
  
}

// getters
std::string MCMCVertexFinder::getFinderName() const {
  
  return "MCMC";
  
}

TH3D *MCMCVertexFinder::getDistHist() const {
 
  if(_points.size()<1){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: not enough points in the markov chain...\n"; return 0; }
  
  double max_x=-10000;
  double max_y=-10000;
  double max_z=-10000;
  
  double min_x= 10000;
  double min_y= 10000;
  double min_z= 10000;
  
  for(int i=0;i<_points.size(); i++){ // find min/max
   
    if(_points[i].getX() > max_x) max_x=_points[i].getX();
    if(_points[i].getY() > max_y) max_y=_points[i].getY();
    if(_points[i].getZ() > max_z) max_z=_points[i].getZ();
        
    if(_points[i].getX() < min_x) min_x=_points[i].getX();
    if(_points[i].getY() < min_y) min_y=_points[i].getY();
    if(_points[i].getZ() < min_z) min_z=_points[i].getZ();
    
  }// for i
  
  TH3D *hist=new TH3D("histMCMC", "Markov Chain Monte Carlo distribution; x [m]; y [m]; z [m]", 300, min_x, max_x, 300, min_y, max_y, 300, min_z, max_z);
  
  for(int i=0;i<_points.size(); i++){
    
    hist->Fill(_points[i].getX(), _points[i].getY(), _points[i].getZ());
    
  }// for i
  
  hist->SetStats(0);
  
  return hist;
  
}

// utilities
void MCMCVertexFinder::takeRandomStep(){

  double mean = 10;
  _step_size = 10;
  if(_coordinate_system==VertexFinder::CARTESIAN){
  
    double step_x=0;// any locked parameter means staying put!
    if(_par_locked_bit[0]==0) step_x=_rand->Gaus(_this_point.getX(), _step_size); 
    double step_y=0;
    if(_par_locked_bit[1]==0) step_y=_rand->Gaus(_this_point.getY(), _step_size);
    double step_z=0;
    if(_par_locked_bit[2]==0) step_z=_rand->Gaus(_this_point.getZ(), _step_size);
       
    _next_point=Pos(_this_point.getX()+step_x, _this_point.getY()+step_y, _this_point.getZ()+step_z);
  
    
    if(_variant==1){// use cyclic bounds
     
      double deltaX=_next_point.getX()-_initial_position->getUpperBoundX();
      if(deltaX>0) deltaX+=_initial_position->getLowerBoundX();// if _next_point goes outside upper bound, bring it back around
      else{ 
	
	deltaX=_next_point.getX() - _initial_position->getLowerBoundX(); 
	if(deltaX<0) deltaX+=_initial_position->getUpperBoundX();
	
      }
      
      
      double deltaY=_next_point.getY()-_initial_position->getUpperBoundY();
      if(deltaY>0) deltaY+=_initial_position->getLowerBoundY();// if _next_point goes outside upper bound, bring it back around
      else{ 
	
	deltaY=_next_point.getY() - _initial_position->getLowerBoundY(); 
	if(deltaY<0) deltaY+=_initial_position->getUpperBoundY();
	
      }
      
      
      double deltaZ=_next_point.getZ()-_initial_position->getUpperBoundZ();
      if(deltaZ>0) deltaZ+=_initial_position->getLowerBoundZ();// if _next_point goes outside upper bound, bring it back around
      else{ 
	
	deltaZ=_next_point.getZ() - _initial_position->getLowerBoundZ(); 
	if(deltaZ<0) deltaZ+=_initial_position->getUpperBoundZ();
	
      }
      
      _next_point=Pos(deltaX, deltaY, deltaZ);// make sure the point is cyclically inside the bounds.
      
      
      
    }// _variant==1
    
  }
  
  if(_coordinate_system==VertexFinder::SPHERICAL){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet...\n";
    
  }
  
}


void MCMCVertexFinder::takeRandomStep(int axis){
  

  double mean = 0;
  if(_coordinate_system==VertexFinder::CARTESIAN){

   double step_x=0;// any locked parameter means staying put!
   if(axis==0 && _par_locked_bit[0]==0) step_x=_rand->Gaus(mean, _step_size); 

   double step_y=0;   
   if(axis==1 && _par_locked_bit[1]==0) step_y=_rand->Gaus(mean, _step_size);

   double step_z=0;
   if(axis==2 && _par_locked_bit[2]==0) step_z=_rand->Gaus(mean, _step_size);
   
   _next_point=Pos(_this_point.getX()+step_x, _this_point.getY()+step_y, _this_point.getZ()+step_z);

    
   if(_variant==1){// use cyclic bounds
     
     double deltaX=_next_point.getX()-_initial_position->getUpperBoundX();
     if(deltaX>0) deltaX+=_initial_position->getLowerBoundX();// if _next_point goes outside upper bound, bring it back around
     else{ 
	
       deltaX=_next_point.getX() - _initial_position->getLowerBoundX(); 
       if(deltaX<0) deltaX+=_initial_position->getUpperBoundX();
	
     }
      
      
     double deltaY=_next_point.getY()-_initial_position->getUpperBoundY();
     if(deltaY>0) deltaY+=_initial_position->getLowerBoundY();// if _next_point goes outside upper bound, bring it back around
     else{ 
	
       deltaY=_next_point.getY() - _initial_position->getLowerBoundY(); 
       if(deltaY<0) deltaY+=_initial_position->getUpperBoundY();
	
     }
      
      
     double deltaZ=_next_point.getZ()-_initial_position->getUpperBoundZ();
     if(deltaZ>0) deltaZ+=_initial_position->getLowerBoundZ();// if _next_point goes outside upper bound, bring it back around
     else{ 
	
       deltaZ=_next_point.getZ() - _initial_position->getLowerBoundZ(); 
       if(deltaZ<0) deltaZ+=_initial_position->getUpperBoundZ();
	
     }
      
     _next_point=Pos(deltaX, deltaY, deltaZ);// make sure the point is cyclically inside the bounds.   
      
   }// _variant==1
    
 }
 if(_coordinate_system==VertexFinder::SPHERICAL){
   
   std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet...\n";
    
 }
}
