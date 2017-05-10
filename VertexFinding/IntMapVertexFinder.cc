#include "IntMapVertexFinder.h"
#include "DeltaFinder.h"

int IntMapVertexFinder::debug_bit=0;

IntMapVertexFinder::IntMapVertexFinder(int coordinate_system, int time_input, int lock_parameters, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(coordinate_system, time_input, lock_parameters, ice_ptr, par1, par2, par3){
  
  initialize();
 
}

IntMapVertexFinder::IntMapVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameters, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameters, ice_ptr, par1, par2, par3){
  
  initialize();
 
}

IntMapVertexFinder::IntMapVertexFinder(const std::string commands) : VertexFinder(){
 
  initialize();
  
  parse(commands);
  
}

IntMapVertexFinder::IntMapVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands) : VertexFinder(channels, initial_position){
  
  initialize();
  
  parse(commands);
  
}

IntMapVertexFinder::IntMapVertexFinder(const IntMapVertexFinder &other) : VertexFinder(other) {
 
  _num_bins_R=other._num_bins_R;
  _num_bins_theta=other._num_bins_theta;
  _num_bins_phi=other._num_bins_phi;
  
  _best_bin_R=other._best_bin_R;
  _best_bin_theta=other._best_bin_theta;
  _best_bin_phi=other._best_bin_phi;
  _best_corr=other._best_corr;
  
  _R_axis=other._R_axis;
  _theta_axis=other._theta_axis;
  _phi_axis=other._phi_axis;
 
  _maps = other._maps;
  
  for(int i=0;i<_maps.size();i++){ // make a deep copy of the maps! 
   
    _maps[i]=new TH2D(*other._maps[i]);
    
  }
  
}

IntMapVertexFinder::~IntMapVertexFinder(){
 
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  clear_maps();
  _R_axis.clear();
  
}

VertexFinder *IntMapVertexFinder::getDeepCopy(){
 
  return new IntMapVertexFinder(*this);
  
}

void IntMapVertexFinder::initialize(){
 
  _coordinate_system = VertexFinder::SPHERICAL; // this does not change for this finder...
  _time_input_type = VertexFinder::PAIRS; // this does not change for this finder...
  for(int a=0;a<3;a++) _par_locked_bit[a]=0; // this does not change for this finder...
  
  _num_bins_R=1;
  _num_bins_theta=36;
  _num_bins_phi=72;
  
  TH1::AddDirectory(kFALSE);
  TH2::AddDirectory(kFALSE);
  TH3::AddDirectory(kFALSE);
  
  if(_parameter1>1) _num_bins_R=(int) _parameter1;
  if(_parameter2>10) _num_bins_theta=(int) _parameter2;
  if(_parameter3>10) _num_bins_phi=(int) _parameter3;
  
  switch(_lock_parameters){
   
    case VertexFinder::LOCK0 : _num_bins_R=1; break;
    case VertexFinder::LOCK1 : _num_bins_theta=1; break;
    case VertexFinder::LOCK2 : _num_bins_phi=1; break;
    
    case VertexFinder::LOCK01 : _num_bins_R=1; _num_bins_theta=1; break;
    case VertexFinder::LOCK02 : _num_bins_R=1; _num_bins_phi=1; break;
    case VertexFinder::LOCK12 : _num_bins_theta=1; _num_bins_phi=1; break;
      
  }
 
}

void IntMapVertexFinder::reset(){
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  _best_bin_R=0;
  _best_bin_theta=0;
  _best_bin_phi=0;
  _best_corr=0;
  
  _R_axis.clear();
  _theta_axis.clear();
  _phi_axis.clear();
    
  makeLogAxis();
    
  _theta_axis.reserve(_num_bins_theta);
  double max_theta=_initial_position->getUpperBoundTheta();
  double min_theta=_initial_position->getLowerBoundTheta();  
  double step_theta=(max_theta-min_theta)/_num_bins_theta;
  for(int i=0;i<_num_bins_theta;i++) _theta_axis.push_back(min_theta+i*step_theta);
  
  _phi_axis.reserve(_num_bins_phi);
  double max_phi=_initial_position->getUpperBoundPhi();
  double min_phi=_initial_position->getLowerBoundPhi();  
  double step_phi=(max_phi-min_phi)/_num_bins_phi;
  for(int i=0;i<_num_bins_phi;i++) _phi_axis.push_back(min_phi+i*step_phi);
  
  VertexFinder::reset();
  
}

void IntMapVertexFinder::printout() const {
  
  VertexFinder::printout();
  if(_R_axis.size()) std::cout<<"R=";
  for(int i=0;i<_R_axis.size();i++) std::cout<<" "<<_R_axis[i];
  if(_R_axis.size()) std::cout<<std::endl;
  
}

void IntMapVertexFinder::help(){
  
}

void IntMapVertexFinder::parse(const std::string commands){
 
  VertexFinder::parse(commands); // make sure to pass any relevant commands into the base class as well...
      
  std::vector< std::vector< std::string> > parsed = splitCommands(commands);
  
  for(int i=0;i<parsed.size();i++){
   
    if(check_str(parsed[i][0], "r_bins") || check_str(parsed[i][0],"r_num_bins") || check_str(parsed[i][0], "Rbins")) setNumBinsR(parsed[i][1]);
    if(check_str(parsed[i][0], "theta_bins") || check_str(parsed[i][0], "theta_num") || check_str(parsed[i][0], "thetaBins")) setNumBinsTheta(parsed[i][1]);
    if(check_str(parsed[i][0], "phi_bins") || check_str(parsed[i][0], "phi_num") || check_str(parsed[i][0], "phiBins")) setNumBinsPhi(parsed[i][1]);
    
  }
  
}

void IntMapVertexFinder::clear_maps(){
 
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
 
  for(int i=0;i<_maps.size();i++) if(_maps[i]) delete _maps[i];
  _maps.clear();
    
}

std::string IntMapVertexFinder::getFinderName() const {
  
  return "IntMap";
  
}

// getters
TH2D *IntMapVertexFinder::getMap(int index) const {
 
  if(index<0 || index>= _maps.size()) return 0;
  
  return _maps[index];
  
}

TH2D *IntMapVertexFinder::getBestMap() const {
  
  return getMap(_best_bin_R);
  
}

void IntMapVertexFinder::makeLogAxis(){
  
  _R_axis.clear();
  
  if(_num_bins_R==1){
    
    _R_axis.push_back(_initial_position->getR());
    return;
   
  }
  
  if(_num_bins_R==2){
    
    _R_axis.push_back(_initial_position->getR());
    _R_axis.push_back(_initial_position->getUpperBoundR());
    return;
    
  }
  
  double min_R=30;
  if(_initial_position->getLowerBoundR()>min_R) min_R=_initial_position->getLowerBoundR();
  
  double max_R=_initial_position->getUpperBoundR();
  
  double current_R=50;
  if(_initial_position->getR()>current_R) current_R=_initial_position->getR();
  
  if(_num_bins_R==3){
   
    _R_axis.push_back(min_R);
    _R_axis.push_back(current_R);
    _R_axis.push_back(_initial_position->getUpperBoundR());
    return;
    
  }
  
  std::vector<double> lower_axis;
  std::vector<double> upper_axis;
  
  int numLower=_num_bins_R/2+_num_bins_R%2;// add any odd points to the lower part
  int numUpper=_num_bins_R/2-1; // don't count the current R for both
  
  lower_axis.reserve(numLower);
  double min_R_log=std::log(min_R);
  double max_R_log=std::log(current_R);
  double step_log=(max_R_log-min_R_log)/(numLower);
  for(int i=0;i<=numLower;i++) lower_axis.push_back(std::exp(min_R_log+i*step_log));
  
  upper_axis.reserve(numUpper);
  min_R_log=std::log(current_R);
  max_R_log=std::log(max_R);
  step_log=(max_R_log-min_R_log)/(numUpper);
  for(int i=1;i<=numUpper;i++) upper_axis.push_back(std::exp(min_R_log+i*step_log));
  
  _R_axis=lower_axis;
  _R_axis.insert(_R_axis.end(), upper_axis.begin(), upper_axis.end());
  
}

void IntMapVertexFinder::findVertex(){

  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  reset();
  
  _channels.applyDeltaFinder(DeltaFinder::CROS);
  
  if(checkIfSomethingMissing()) { 
    setStatusFailed();
    return;
  }
  
  double r2d=StationGeometry::RADTODEG;
    
  std::vector<CorrGraph*> corrs=_channels.getCorrelationGraphVector();
  
  clear_maps();
  _maps.reserve(_num_bins_R);
    
  for(int i=0;i<_num_bins_R;i++){
    
    TH2D *hist_pointer=new TH2D(Form("hist%4.2lf", _R_axis[i]), Form("Interference map R= %lf; #phi [deg]; #theta [deg]", _R_axis[i]),  _num_bins_phi+1, _initial_position->getLowerBoundPhi()*r2d, _initial_position->getUpperBoundPhi()*r2d, _num_bins_theta+1, _initial_position->getLowerBoundTheta()*r2d, _initial_position->getUpperBoundTheta()*r2d);
    
    hist_pointer->SetStats(0);
    
    _maps.push_back(hist_pointer);
    
    for(int j=0;j<_num_bins_theta;j++){
      
      for(int k=0;k<_num_bins_phi;k++){
	
	Pos source;
	source.setSpherical(_R_axis[i], _theta_axis[j], _phi_axis[k]);
	double corr=findSumInterference(source);
	if(corr>_best_corr){
	 
	  _best_corr=corr;
	  _best_bin_R=i;
	  _best_bin_theta=j;
	  _best_bin_phi=k;
	  
	}
	
	_maps[i]->Fill(_phi_axis[k]*r2d, _theta_axis[j]*r2d, corr);
	
      }// for k
      
    }// for j
    
  }// for i
  
  
  calculateConfidence(_best_corr);
  setSpherical(_R_axis[_best_bin_R], _theta_axis[_best_bin_theta], _phi_axis[_best_bin_phi]);
  // still need to take care of errors...
  
  if(_hScan){ delete _hScan; _hScan=0; }

//   _maps[_best_bin_R]->Draw("colz");
    
//   _hScan=new THX(_maps[_best_bin_R]);// put the best result in THX
  
  estimateErrors();
  
  setStatusFound();
  
}

// setters
void IntMapVertexFinder::setCoordinates(int coordinates){
 
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" is not relevant for IntMapVertexFinder, it is SPHERICAL only..."<<std::endl; }
  
}

void IntMapVertexFinder::setTimeInputs(int times){ 
  
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" is not relevant for IntMapVertexFinder, it is PAIRWISE only..."<<std::endl; }

}

void IntMapVertexFinder::setNumBinsR(int num_bins){
  
  if(num_bins<1){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot have number of bins: "<<num_bins<<" below 1..."<<std::endl;
    return;
    
  }
  
  _num_bins_R=num_bins;
  
}

void IntMapVertexFinder::setNumBinsR(const std::string number){
 
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" number= "<<number<<std::endl; }
  
  setNumBinsR(atoi(number.c_str()));
  
}

void IntMapVertexFinder::setNumBinsTheta(int num_bins){
  
  if(num_bins<1){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot have number of bins: "<<num_bins<<" below 1..."<<std::endl;
    return;
    
  }
  
  _num_bins_theta=num_bins;
  
}

void IntMapVertexFinder::setNumBinsTheta(const std::string number){
  
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" number= "<<number<<std::endl; }
  
  setNumBinsTheta(atoi(number.c_str()));
  
}

void IntMapVertexFinder::setNumBinsPhi(int num_bins){
  
  if(num_bins<1){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot have number of bins: "<<num_bins<<" below 1..."<<std::endl;
    return;
    
  }
  
  _num_bins_phi=num_bins;
  
}

void IntMapVertexFinder::setNumBinsPhi(const std::string number){
    
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" number= "<<number<<std::endl; }
  
  setNumBinsPhi(atoi(number.c_str()));
  
}

void IntMapVertexFinder::calculateConfidence(double quality_parameter){
   
//   if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  double threshold=50;
  double scale=20;
  
  setConfidence((std::sqrt(quality_parameter)-threshold)/scale);
  
  
  
}

void IntMapVertexFinder::estimateErrors(){
  
  Pos source;
  source.setSpherical(_R_axis[_best_bin_R], _theta_axis[_best_bin_theta], _phi_axis[_best_bin_phi]);
  double best_corr=findSumInterference(source);
  double thresh_corr=best_corr*0.5;
  
  double FWHMtoSigma=1.0/2.355;
//   double FWHMtoSigma=1.0; // if we take top 80% of peak height, it is no longer the FWHM...
  
  
  if(_num_bins_R>1){
    
    // calculate the FWHM equivalent area in R
    int low_bin_R=_best_bin_R;
    
    for(int i=1;i<_num_bins_R;i++){ // try going down in R

      if(_best_bin_R-i<0) break;
      
      source.setSpherical(_R_axis[_best_bin_R-i], _theta_axis[_best_bin_theta], _phi_axis[_best_bin_phi]);
      double corr=findSumInterference(source);
      
      if(corr<thresh_corr) break;
      
      low_bin_R=_best_bin_R-i;
      
    }
    
    double low_R=_R_axis[low_bin_R];
    
    int high_bin_R=_best_bin_R;
    
    for(int i=1;i<_num_bins_R;i++){ // try going up in R

      if(_best_bin_R+i>=_num_bins_R) break;
      
      source.setSpherical(_R_axis[_best_bin_R+i], _theta_axis[_best_bin_theta], _phi_axis[_best_bin_phi]);
      double corr=findSumInterference(source);
      
      if(corr<thresh_corr) break;
      
      high_bin_R=_best_bin_R+i;
      
    }
    
    double high_R=_R_axis[high_bin_R];
    
    setErrorR((high_R-low_R)/2*FWHMtoSigma);// for now just use the average error and set symmetric errors...
    
  }
  // calculate the FWHM equivalent area in theta
  int low_bin_theta=_best_bin_theta;
  
  for(int i=1;i<_num_bins_theta;i++){ // try going down in theta

    if(_best_bin_theta-i<0) break;
    
    source.setSpherical(_R_axis[_best_bin_R], _theta_axis[_best_bin_theta-i], _phi_axis[_best_bin_phi]);
    double corr=findSumInterference(source);
    
    if(corr<thresh_corr) break;
    
    low_bin_theta=_best_bin_theta-i;
    
  }
  
  double low_theta=_theta_axis[low_bin_theta];
  
  int high_bin_theta=_best_bin_theta;
  
  for(int i=1;i<_num_bins_theta;i++){ // try going up in theta

    if(_best_bin_theta+i>=_num_bins_theta) break;
    
    source.setSpherical(_R_axis[_best_bin_R], _theta_axis[_best_bin_theta+i], _phi_axis[_best_bin_phi]);
    double corr=findSumInterference(source);
    
    if(corr<thresh_corr) break;
    
    high_bin_theta=_best_bin_theta+i;
    
  }
  
  double high_theta=_theta_axis[high_bin_theta];
    
  setErrorTheta((high_theta-low_theta)/2*FWHMtoSigma);// for now just use the average error and set symmetric errors...
  
  // calculate the FWHM equivalent area in phi
  int low_bin_phi=_best_bin_phi;
  
  for(int i=1;i<_num_bins_phi;i++){ // try going down in phi

    if(_best_bin_phi-i<0) break;
    
    source.setSpherical(_R_axis[_best_bin_R], _theta_axis[_best_bin_theta], _phi_axis[_best_bin_phi-i]);
    double corr=findSumInterference(source);
    
    if(corr<thresh_corr) break;
    
    low_bin_phi=_best_bin_phi-i;
    
  }
  
  double low_phi=_phi_axis[low_bin_phi];
  
  int high_bin_phi=_best_bin_phi;
  
  for(int i=1;i<_num_bins_phi;i++){ // try going up in phi

    if(_best_bin_phi+i>=_num_bins_phi) break;
    
    source.setSpherical(_R_axis[_best_bin_R], _theta_axis[_best_bin_theta], _phi_axis[_best_bin_phi+i]);
    double corr=findSumInterference(source);
    
    if(corr<thresh_corr) break;
    
    high_bin_phi=_best_bin_phi+i;
    
  }
  
  double high_phi=_phi_axis[high_bin_phi];
  
//   std::cout<<"low_bin_phi= "<<low_bin_phi<<" low_phi= "<<low_phi*StationGeometry::RADTODEG<<" high_bin_phi= "<<high_bin_phi<<" high_phi= "<<high_phi*StationGeometry::RADTODEG<<std::endl;
  
  setErrorPhi((high_phi-low_phi)/2*FWHMtoSigma);// for now just use the average error and set symmetric errors...
    
}












  
