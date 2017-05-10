#include "ScanVertexFinder.h"

ClassImp(ScanVertexFinder);

ScanVertexFinder::ScanVertexFinder() : VertexFinder() {}

ScanVertexFinder::ScanVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameter, ice_ptr, par1, par2, par3){
 
  
  initialize();
  
}

ScanVertexFinder::ScanVertexFinder(std::string commands) : VertexFinder(){
 
  initialize();
  
  parse(commands);
  
}

ScanVertexFinder::ScanVertexFinder(ChannelCollection channels, VertexPos *initial_position, std::string commands) : VertexFinder(channels, initial_position){

  initialize();
  
  parse(commands);
  
}

ScanVertexFinder::ScanVertexFinder(const ScanVertexFinder &other) : VertexFinder(other) {
 
  _Nx=other._Nx;
  _Ny=other._Ny;
  _Nz=other._Nz;
   
}

ScanVertexFinder::~ScanVertexFinder(){

  
  
}

VertexFinder *ScanVertexFinder::getDeepCopy(){
 
  return new ScanVertexFinder(*this);
  
}

void ScanVertexFinder::initialize(){
   
  // use parameters to set the number of steps in each axis...
  if(_parameter1<1) _Nx=100; else _Nx=_parameter1;
  if(_parameter2<1) _Ny=100; else _Ny=_parameter2;
  if(_parameter3<1) _Nz=100; else _Nz=_parameter3;
  
  initializeHistograms();
  
}

void ScanVertexFinder::reset(){
  
  initializeHistograms();
  
}

void ScanVertexFinder::help(){
 
  VertexFinder::help();
  
  std::cout<<"use num_x, num_y, and num_z to set number of steps for scan"<<std::endl;
  
}

void ScanVertexFinder::parse(const std::string commands){
    
  VertexFinder::parse(commands);
  
  std::vector< std::vector< std::string> > parsed = splitCommands(commands);
  
  for(int i=0;i<parsed.size();i++){
    
    if(check_str(parsed[i][0], "nx","num_x","steps_x","num_steps_x") || check_str(parsed[i][0],"nr","num_r","steps_r","num_steps_r")){ setNumSteps1(stoi(parsed[i][1]));}
    if(check_str(parsed[i][0], "ny","num_y","steps_y","num_steps_y") || check_str(parsed[i][0],"ntheta","num_theta","steps_theta","num_steps_theta")){ setNumSteps2(stoi(parsed[i][1]));}
    if(check_str(parsed[i][0], "nz","num_z","steps_z","num_steps_z") || check_str(parsed[i][0],"nphi","num_phi","steps_phi","num_steps_phi")){ setNumSteps3(stoi(parsed[i][1]));}
           
  }
  
}

std::string ScanVertexFinder::getFinderName() const {
  
  return "scan";
  
}

// setters
void ScanVertexFinder::setNumSteps1(int num){
 
  _Nx=num;
  
}

void ScanVertexFinder::setNumSteps2(int num){
 
  _Ny=num;
  
}

void ScanVertexFinder::setNumSteps3(int num){
 
  _Nz=num;
  
}

void ScanVertexFinder::findVertex(){
 
  if(checkIfSomethingMissing()) {
    setStatusFailed();
    return;
  }

//   _this_finder=this;// why do I need this static pointer to trampoline back to "this"? well... because minuitChiSquare is static so Minuit can get its void* pointer and myChi is non-static so...
  
  double expand=2;
  
  double x_step, y_step, z_step;
  double base; // for R search
  VertexPos::printout();
  Pos::printout();
  std::cout <<" R, theta, phi "<< getR() <<" "<< getTheta() <<" " << getPhi() << std::endl;
  if(_coordinate_system == VertexFinder::CARTESIAN){
  
    x_step=(getUpperBoundX(expand) - getLowerBoundX(expand))/_Nx;
    y_step=(getUpperBoundY(expand) - getLowerBoundY(expand))/_Ny;
    z_step=(getUpperBoundZ(expand) - getLowerBoundZ(expand))/_Nz;
    
  }
  
  if(_coordinate_system == VertexFinder::SPHERICAL){
  
    x_step=(getUpperBoundR(expand) - getLowerBoundR(expand))/_Nx;
    y_step=(getUpperBoundTheta(expand) - getLowerBoundTheta(expand))/_Ny;
    z_step=(getUpperBoundPhi(expand) - getLowerBoundPhi(expand))/_Nz;

    base = TMath::Exp(TMath::Log(getUpperBoundR(expand)/getLowerBoundR(expand))/_Nx); //helps to compute R steps
    std::cout <<"Base: "<< base <<std::endl;
    std::cout <<"Expand -"<< getLowerBoundR(expand) << " /+" << getUpperBoundR(expand) << std::endl;
    std::cout <<"Expand -"<< getLowerBoundR(0) << " /+" << getUpperBoundR(0) << std::endl;
    
    std::cout <<"Theta Expand -"<< getLowerBoundTheta(expand) << " /+" << getUpperBoundTheta(expand) << std::endl;
    std::cout <<"Theta Expand -"<< getLowerBoundTheta(0) << " /+" << getUpperBoundTheta(0) << std::endl;
    std::cout <<"Phi Expand -"<< getLowerBoundPhi(expand) << " /+" << getUpperBoundPhi(expand) << std::endl;
    std::cout <<"Phi Expand -"<< getLowerBoundPhi(0) << " /+" << getUpperBoundPhi(0) << std::endl;
    
   
  }
    
  if(_par_locked_bit[0]) _Nx=0; // fixed parameter 1
  if(_par_locked_bit[1]) _Ny=0; // fixed parameter 2
  if(_par_locked_bit[2]) _Nz=0;// fixed parameter 3

  int best_x_bin=0;
  int best_y_bin=0;
  int best_z_bin=0;
  
  double bestChi=1e36;
  Pos position;
  Pos best_pos;
  
  if(_coordinate_system==VertexFinder::CARTESIAN){
    
    for(int i=0;i<_Nx+1; i++){
    
      if(_par_locked_bit[0]) position.setX(getX()); 
      else position.setX(getLowerBoundX(expand) + (i)*x_step);
        
      if(_lock_parameters==VertexFinder::UNLOCK) std::cout<<"i ="<<i<<" x= "<<position.getX()<<std::endl;
        
      for(int j=0;j<_Ny+1; j++){
      
	if(_par_locked_bit[1]) position.setY(getY());
	else position.setY(getLowerBoundY(expand) + (j)*y_step);
    
	for(int k=0;k<_Nz+1; k++){
      
	  if(_par_locked_bit[2]) position.setZ(getZ());
	  else position.setZ(getLowerBoundZ(expand) + (k)*z_step);
	
	  double chi=0;
	
	  if(_time_input_type==VertexFinder::TIMES){ 
	  
	    double t0=getInteractionTime(position);
	    chi=myChi(position, t0);
	  }
	  
	  if(_time_input_type==VertexFinder::PAIRS){ 
	    chi=myChi(position);
	  }
	    
	  fillHistogram(position, chi);
	
	  if(chi<bestChi){
	    bestChi=chi;
	    best_pos=position;
	  }
      
	}// for k
      }// for j 
    }// for i  
    
    setCartesian(best_pos.getCartesian());
        
    if(_par_locked_bit[0]==0) setErrorX(x_step, 0);
    if(_par_locked_bit[1]==0) setErrorY(y_step, 0);
    if(_par_locked_bit[2]==0) setErrorZ(z_step, 0);
    
    calculateSpherical();
    
    
    
  } // if CARTESIAN
  
  if(_coordinate_system==VertexFinder::SPHERICAL){
    double R, theta, phi;

    if(_par_locked_bit[0]) R = getR();
    else R = getR();
    
    for(int j=0; j<_Ny+1; j++){
      if(_par_locked_bit[1]) theta = getTheta();
      else theta = getLowerBoundTheta(expand) + (j)*y_step;
      
      for(int k=0;k<_Nz+1; k++){
	if(_par_locked_bit[2]) phi = getPhi();
	else phi = getLowerBoundPhi(expand) + (k)*z_step;
	
	position.setSpherical(R, theta, phi);
	
	double chi =0;
	
	if(_time_input_type==VertexFinder::TIMES){

	  double t0 = getInteractionTime(position);	 
	  chi = myChi(position, t0);
	}
	if(_time_input_type==VertexFinder::PAIRS){
	  chi = myChi(position);
	}

	fillHistogram(position, chi);

	if(chi < bestChi){
	  bestChi = chi;
	  best_pos = position;
	}

      }
    }

    best_pos.printout();
    _Ny = ScanVertexFinder::_iter_2_theta_bins;
    _Nz = ScanVertexFinder::_iter_2_phi_bins;

    for(int i=0; i<_Nx+1; i++){
      if(_par_locked_bit[0]) R = getR();
      else R = getLowerBoundR(expand)*pow(base, i);
      //      std::cout
      for(int j=0; j<_Ny+1; j++){
	if(_par_locked_bit[1]) theta = best_pos.getTheta();
	else theta = (best_pos.getTheta() - _Ny*TMath::DegToRad()/2) + (j*TMath::DegToRad());
	
	for(int z=0; z<_Nz+1; z++){
	  if(_par_locked_bit[2]) phi = best_pos.getPhi();
	  else phi = (best_pos.getPhi() - _Nz*TMath::DegToRad()/2) + (z*TMath::DegToRad());
		 
	  position.setSpherical(R, theta, phi);
	  
	  double chi = 0;
	  
	  if(_time_input_type==VertexFinder::TIMES){
	    
	    double t0 = getInteractionTime(position);
	    chi = myChi(position, t0);
	  }
	  if(_time_input_type==VertexFinder::PAIRS){

	    chi = myChi(position);
	  }

	  fillHistogram(position, chi);

	  if(chi < bestChi){

	    bestChi = chi;
	    best_pos = position;
	  }
	  
	}
      }
    }


    _Nx = ScanVertexFinder::_iter_3_R_bins;
    _Ny = ScanVertexFinder::_iter_3_theta_bins;
    _Nz = ScanVertexFinder::_iter_3_phi_bins;
    
    for(int i=0; i<_Nx+1; i++){
      if(_par_locked_bit[0]) R = best_pos.getR();
      else R = best_pos.getR() - _Nx/2 + i;
      
      for(int j=0; j<_Ny+1; j++){
	if(_par_locked_bit[1]) theta = best_pos.getTheta();
	else theta = best_pos.getTheta() - _Ny*TMath::DegToRad()/2 + j*TMath::DegToRad();

	for(int z=0; z<_Nz+1; z++){
	  if(_par_locked_bit[2]) phi = best_pos.getPhi();
	  else phi = best_pos.getPhi() - _Nz*TMath::DegToRad()/2 + z*TMath::DegToRad();
	  
	  
	  position.setSpherical(R, theta, phi);
	  
	  double chi = 0;
	  
	  if(_time_input_type==VertexFinder::TIMES){
	    
	    double t0 = getInteractionTime(position);
	    
	    chi = myChi(position, t0);
	  }
	  if(_time_input_type==VertexFinder::PAIRS){

	    chi = myChi(position);
	  }

	  fillHistogram(position, chi);

	  if(chi < bestChi){
	    bestChi = chi;
	    best_pos = position;
	  }
	  
	}
      }
    }    

    setCartesian(best_pos.getCartesian());

    double error_R=estimateErrorR();
    if(error_R<x_step) error_R=x_step; // it is never smaller than the step size...
    
    double error_theta=estimateErrorAngles();
    double error_phi=error_theta;
    
    if(error_theta<y_step) error_theta=y_step;
    if(error_phi<z_step) error_phi=z_step;
    

    if(_par_locked_bit[0]==0) setErrorR(error_R, 0);
    if(_par_locked_bit[1]==0) setErrorTheta(error_theta, 0); // for angles, the step size is good enough usually...
    if(_par_locked_bit[2]==0) setErrorPhi(error_phi, 0); // for angles, the step size is good enough usually...
    
    calculateCartesian();
    
  }// if SPHERICAL
  
  calculateConfidence(bestChi);
  
  std::cout<<"best chi2= "<<bestChi<<std::endl; // debugging only
  
//   _status=VertexFinder::FOUND;
  setStatusFound();
  
  
}

void ScanVertexFinder::initializeHistograms(){
 
//   if(_hScan1D){ delete _hScan1D; _hScan1D=0; }
//   if(_hScan2D){ delete _hScan2D; _hScan2D=0; }
//   
//   // 1D histograms, cartesian
//   if( _lock_parameters==VertexFinder::LOCK12 && _coordinate_system==VertexFinder::CARTESIAN) _hScan1D=new TH1D("hScan", Form("Scan #chi^{2} using %s finder; x [m]; #chi^{2}", getFinderName().c_str()), _Nx+1, getLowerBoundX(), getUpperBoundX());
//   if( _lock_parameters==VertexFinder::LOCK02 && _coordinate_system==VertexFinder::CARTESIAN) _hScan1D=new TH1D("hScan", Form("Scan #chi^{2} using %s finder; y [m]; #chi^{2}", getFinderName().c_str()), _Ny+1, getLowerBoundY(), getUpperBoundY());
//   if( _lock_parameters==VertexFinder::LOCK01 && _coordinate_system==VertexFinder::CARTESIAN) _hScan1D=new TH1D("hScan", Form("Scan #chi^{2} using %s finder; z [m]; #chi^{2}", getFinderName().c_str()), _Nz+1, getLowerBoundZ(), getUpperBoundZ());
//      
//   // 1D histograms, spherical
//   if( _lock_parameters==VertexFinder::LOCK12 && _coordinate_system==VertexFinder::SPHERICAL) _hScan1D=new TH1D("hScan", Form("Scan #chi^{2} using %s finder; R [m]; #chi^{2}", getFinderName().c_str()), _Nx+1, getLowerBoundR(), getUpperBoundR());
//   if( _lock_parameters==VertexFinder::LOCK02 && _coordinate_system==VertexFinder::SPHERICAL) _hScan1D=new TH1D("hScan", Form("Scan #chi^{2} using %s finder; #theta [deg]; #chi^{2}", getFinderName().c_str()), _Ny+1, getLowerBoundTheta()*VertexPos::RADTODEG, getUpperBoundTheta()*VertexPos::RADTODEG);
//   if( _lock_parameters==VertexFinder::LOCK01 && _coordinate_system==VertexFinder::SPHERICAL) _hScan1D=new TH1D("hScan", Form("Scan #chi^{2} using %s finder; #phi [deg]; #chi^{2}", getFinderName().c_str()), _Nz+1, getLowerBoundPhi()*VertexPos::RADTODEG, getUpperBoundPhi()*VertexPos::RADTODEG);
//  
//   // 2D histograms, cartesian
//   if( _lock_parameters==VertexFinder::LOCK0 && _coordinate_system==VertexFinder::CARTESIAN) _hScan2D=new TH2D("hScan", Form("Scan #chi^{2} using %s finder; y [m]; z [m]; #chi^{2}", getFinderName().c_str()), _Ny+1, getLowerBoundY(), getUpperBoundY(), _Nz+1, getLowerBoundZ(), getUpperBoundZ());
//   if( _lock_parameters==VertexFinder::LOCK1 && _coordinate_system==VertexFinder::CARTESIAN) _hScan2D=new TH2D("hScan", Form("Scan #chi^{2} using %s finder; x [m]; z [m]; #chi^{2}", getFinderName().c_str()), _Nx+1, getLowerBoundX(), getUpperBoundX(), _Nz+1, getLowerBoundZ(), getUpperBoundZ());
//   if( _lock_parameters==VertexFinder::LOCK2 && _coordinate_system==VertexFinder::CARTESIAN) _hScan2D=new TH2D("hScan", Form("Scan #chi^{2} using %s finder; x [m]; y [m]; #chi^{2}", getFinderName().c_str()), _Nx+1, getLowerBoundX(), getUpperBoundX(), _Ny+1, getLowerBoundY(), getUpperBoundY());  
//  
//   // 2D histograms, spherical
//   if( _lock_parameters==VertexFinder::LOCK0 && _coordinate_system==VertexFinder::SPHERICAL) _hScan2D=new TH2D("hScan", Form("Scan #chi^{2} using %s finder; #theta [deg]; #phi [deg]; #chi^{2}", getFinderName().c_str()), _Ny+1, getLowerBoundTheta()*VertexPos::RADTODEG, getUpperBoundTheta()*VertexPos::RADTODEG, _Nz+1, getLowerBoundPhi()*VertexPos::RADTODEG, getUpperBoundPhi()*VertexPos::RADTODEG);
//   if( _lock_parameters==VertexFinder::LOCK1 && _coordinate_system==VertexFinder::SPHERICAL) _hScan2D=new TH2D("hScan", Form("Scan #chi^{2} using %s finder; R [m]; #phi [deg]; #chi^{2}", getFinderName().c_str()), _Nx+1, getLowerBoundR(), getUpperBoundR(), _Nz+1, getLowerBoundPhi()*VertexPos::RADTODEG, getUpperBoundPhi()*VertexPos::RADTODEG);
//   if( _lock_parameters==VertexFinder::LOCK2 && _coordinate_system==VertexFinder::SPHERICAL) _hScan2D=new TH2D("hScan", Form("Scan #chi^{2} using %s finder; R [m]; #theta [deg]; #chi^{2}", getFinderName().c_str()), _Nx+1, getLowerBoundR(), getUpperBoundR(), _Ny+1, getLowerBoundTheta()*VertexPos::RADTODEG, getUpperBoundTheta()*VertexPos::RADTODEG);
// 
//   if(_hScan1D) _hScan1D->SetStats(0);
//   if(_hScan2D) _hScan2D->SetStats(0);


  
}

void ScanVertexFinder::fillHistogram(Pos position, double chi2){
 
//   std::cout<<"ScanVertexFinder::fillHistogram("<<position.getX()<<","<<position.getY()<<","<<position.getZ()<<")  chi = "<<chi2<<std::endl;
  
  // fill 1D cartesian
//   if( _lock_parameters==VertexFinder::LOCK12 && _coordinate_system==VertexFinder::CARTESIAN) _hScan1D->Fill(position.getX(), chi2);
//   if( _lock_parameters==VertexFinder::LOCK02 && _coordinate_system==VertexFinder::CARTESIAN) _hScan1D->Fill(position.getY(), chi2);
//   if( _lock_parameters==VertexFinder::LOCK01 && _coordinate_system==VertexFinder::CARTESIAN) _hScan1D->Fill(position.getZ(), chi2);
//   
//   // fill 1D spherical
//   if( _lock_parameters==VertexFinder::LOCK12 && _coordinate_system==VertexFinder::SPHERICAL) _hScan1D->Fill(position.getR(), chi2);
//   if( _lock_parameters==VertexFinder::LOCK02 && _coordinate_system==VertexFinder::SPHERICAL) _hScan1D->Fill(position.getTheta()*VertexPos::RADTODEG, chi2);
//   if( _lock_parameters==VertexFinder::LOCK01 && _coordinate_system==VertexFinder::SPHERICAL) _hScan1D->Fill(position.getPhi()*VertexPos::RADTODEG, chi2);
//   
//   // fill 2D cartesian
//   if( _lock_parameters==VertexFinder::LOCK0 && _coordinate_system==VertexFinder::CARTESIAN) _hScan2D->Fill(position.getY(), position.getZ(), chi2);
//   if( _lock_parameters==VertexFinder::LOCK1 && _coordinate_system==VertexFinder::CARTESIAN) _hScan2D->Fill(position.getX(), position.getZ(), chi2);
//   if( _lock_parameters==VertexFinder::LOCK2 && _coordinate_system==VertexFinder::CARTESIAN) _hScan2D->Fill(position.getX(), position.getY(), chi2);
//   
//   // fill 2D spherical
//   if( _lock_parameters==VertexFinder::LOCK0 && _coordinate_system==VertexFinder::SPHERICAL) _hScan2D->Fill(position.getTheta()*VertexPos::RADTODEG, position.getPhi()*VertexPos::RADTODEG, chi2);
//   if( _lock_parameters==VertexFinder::LOCK1 && _coordinate_system==VertexFinder::SPHERICAL) _hScan2D->Fill(position.getR(), position.getPhi()*VertexPos::RADTODEG, chi2);
//   if( _lock_parameters==VertexFinder::LOCK2 && _coordinate_system==VertexFinder::SPHERICAL) _hScan2D->Fill(position.getR(), position.getTheta()*VertexPos::RADTODEG, chi2);
  
}


const int ScanVertexFinder::_iter_2_theta_bins=18;
const int ScanVertexFinder::_iter_2_phi_bins=10;

const int ScanVertexFinder::_iter_3_R_bins=16;
const int ScanVertexFinder::_iter_3_theta_bins=23;
const int ScanVertexFinder::_iter_3_phi_bins=18;
