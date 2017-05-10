#include "MinuitUNLKUVertexFinder.h"

ClassImp(MinuitUNLKUVertexFinder);

MinuitUNLKUVertexFinder::MinuitUNLKUVertexFinder() : VertexFinder() {

  _minimizer=0;
}

MinuitUNLKUVertexFinder::MinuitUNLKUVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameter, ice_ptr, par1, par2, par3){

  initialize();
}


MinuitUNLKUVertexFinder::MinuitUNLKUVertexFinder(int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(coordinate_system, time_input, lock_parameter, ice_ptr, par1, par2, par3){

  initialize();

}

MinuitUNLKUVertexFinder::~MinuitUNLKUVertexFinder(){

  if(_minimizer)
    delete _minimizer;
  
  _minimizer = 0;

}

void MinuitUNLKUVertexFinder::initialize(){

  _arglist[0] = 1000;
  _arglist[1] = 1;

  if(isUsingHitTimes()){
    _minimizer = new TMinuit(4);
  }
  else {
    _minimizer = new TMinuit(3);
  }
  _minimizer->SetPrintLevel(-1);
  _minimizer->SetMaxIterations(100);
  _minimizer->SetFCN(VertexFinder::minuitUNLKUChiSquare);
  _minimizer->mnexcm("SET ERR", _arglist, 1, _ierflag);

  for(int i=0; i<6; i++){
      _error_flag[i] = 0;
  }
}

string MinuitUNLKUVertexFinder::getFinderName() const {
  
  return "minuitUNLKU";
}

void MinuitUNLKUVertexFinder::reset(){
  
  if(_minimizer) delete _minimizer; _minimizer=0;

  initialize();
}

void MinuitUNLKUVertexFinder::findVertex(ChannelCollection channels, VertexPos *initial_position){

  reset();

  setChannels(channels);
  setInitialPosition(initial_position);

  findVertex();

}

void MinuitUNLKUVertexFinder::findVertex(){

  if(checkIfSomethingMissing()){
    setStatusFailed();
    return;
  }

  _this_finder = this;
  
  double t_lower_bound = -512*1000;
  double t_upper_bound = 512*1000;
  double t_step = 20;
  
  if(_coordinate_system==VertexFinder::CARTESIAN){

    double hor_step =4;
    double depth_step = 5;
    double expand_bounds =3;
    
    _minimizer->DefineParameter(0, "X", _initial_position->getX(), hor_step, _initial_position->getLowerBoundX(expand_bounds), _initial_position->getUpperBoundX(expand_bounds));
    _minimizer->DefineParameter(1, "Y", _initial_position->getY(), hor_step, _initial_position->getLowerBoundY(expand_bounds), _initial_position->getUpperBoundY(expand_bounds));
    _minimizer->DefineParameter(2, "Z", _initial_position->getZ(), hor_step, _initial_position->getLowerBoundZ(expand_bounds), _initial_position->getUpperBoundZ(expand_bounds));
      
    if(isUsingHitTimes()){
      _minimizer->DefineParameter(3, "T", _channels.getEarliestChannel()->getTime() - 256, t_step, t_lower_bound, t_upper_bound);
    }
    
  }

  if(_coordinate_system==VertexFinder::SPHERICAL){
    std::cout << __PRETTY_FUNCTION__ << "  SPHERICAL "<< std::endl;
    double r_step =10;
    double angle_step = VertexPos::DEGTORAD*1;
    double expand_bounds = 3;
    
    _minimizer->DefineParameter(0, "R", _initial_position->getR(), r_step, _initial_position->getLowerBoundR(expand_bounds), _initial_position->getUpperBoundR(expand_bounds));
    _minimizer->DefineParameter(1, "TH", _initial_position->getTheta(), angle_step, _initial_position->getLowerBoundTheta(expand_bounds), _initial_position->getUpperBoundTheta(expand_bounds));
    _minimizer->DefineParameter(2, "PH", _initial_position->getPhi(), angle_step, _initial_position->getLowerBoundPhi(expand_bounds), _initial_position->getUpperBoundPhi(expand_bounds));

    if(isUsingHitTimes()){
      _minimizer->DefineParameter(3, "T", _channels.getEarliestChannel()->getTime(), t_step, t_lower_bound, t_upper_bound);
    }

  }

  _minimizer->mnexcm("SIMPLEX", _arglist, 2, _error_flag[4]);  
  _minimizer->mnexcm("MIGRAD", _arglist, 2, _error_flag[5]);
  
  double found_pos[3];
  double found_err[3];
  

  for(int a=0; a<3; a++){
    _minimizer->GetParameter(a, found_pos[a], found_err[a]);
  }

  if(_coordinate_system==VertexFinder::CARTESIAN){
    setCartesian(found_pos[0], found_pos[1], found_pos[2]);
    setSymmetricErrorsCartesian(found_err[0], found_err[1], found_err[2]);
  }
  
  if(_coordinate_system==VertexFinder::SPHERICAL){
    
    setSpherical(found_pos[0], found_pos[1], found_pos[2]);
    setSymmetricErrorsCartesian(found_err[0], found_err[1], found_err[2]);
  }

  double bestChi=0;
  if(_time_input_type==VertexFinder::TIMES) bestChi=myChi(*this, getInteractionTime(*this));
  if(_time_input_type==VertexFinder::PAIRS) bestChi=myChi(*this);
  
  calculateConfidence(bestChi);
  setStatusFound();
}
