#include "VertexFinder.h"

ClassImp(VertexFinder);

int VertexFinder::debug_bit=0;

VertexFinder::VertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameters, OpticalIce *ice_ptr, double par1, double par2, double par3, int variant) :_coordinate_system(coordinate_system), _lock_parameters(lock_parameters), _time_input_type(time_input), _parameter1(par1), _parameter2(par2), _parameter3(par3), _variant(variant),  VertexPos()  { 
    
  initialize();
  setInitialPosition(initial_position);
  //this = initial_position;
  //  adoptPreviousPos(initial_position);
  setChannels(channels);
  _ice=ice_ptr;
    
}

VertexFinder::VertexFinder(int coordinate_system, int time_input, int lock_parameters, OpticalIce *ice_ptr, double par1, double par2, double par3, int variant) :_coordinate_system(coordinate_system), _lock_parameters(lock_parameters), _time_input_type(time_input), _parameter1(par1), _parameter2(par2), _parameter3(par3), _variant(variant), VertexPos()  { 
    
  initialize();
  _ice=ice_ptr;
    
}

VertexFinder::VertexFinder(const VertexFinder &other) : _coordinate_system(other.getCoordinateSystem()), _time_input_type(other.getInputTimeType()), _lock_parameters(other.getLockParameters()), _parameter1(other.getParameter1()), _parameter2(other.getParameter2()), _parameter3(other.getParameter3()), _variant(other.getVariant()) {
 
  initialize();
  
  setChannels(other.getChannels());
  setInitialPosition(other.getInitialPosition());
  _matrix=_channels.getTimingMatrix();
  
  _hScan=other.getHistXD();
  
}

VertexFinder::VertexFinder(const std::string commands){
 
  initialize();

  parse(commands);
  
}

VertexFinder::VertexFinder(ChannelCollection channels, VertexPos *initial_position, std::string commands){
 
  initialize();
  
  _channels=channels;
  _initial_position=initial_position;
  
  parse(commands);
  
}

VertexFinder::~VertexFinder(){
 
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(_initial_position){ delete _initial_position; _initial_position=0; }

  _channels.clear();

  if(_hScan != 0){ delete _hScan; _hScan=0; }

  _previous_finder =0; // we only have a pointer
  
}

void VertexFinder::initialize(){
  
//   std::cout<<"VertexFinder::initialize()"<<std::endl;
  _initial_position=new VertexPos();
  _ice=0;
  
  _previous_finder =0;

  for(int a=0;a<3;a++) _par_locked_bit[a]=0; // no locked parameters
    
  _status=0;// READY

  _has_executed=0;

  _hScan=0;
  _fill_hist=0;
  
//   _parameter1=0;
//   _parameter2=0;
//   _parameter3=0;
  
  updateLockedParameters();
  
  TH1::AddDirectory(kFALSE);
  TH2::AddDirectory(kFALSE);
  TH3::AddDirectory(kFALSE);
  
}

void VertexFinder::reset(){
  
   _status=READY;
   _has_executed=0;
   
   if(_hScan){ delete _hScan; _hScan=0; }
      
  
}

void VertexFinder::printout() const {

  printTitle();
  printPos(getCoordinateSystem());
  
}

void VertexFinder::printout(int coordinate_system) const {
   
  printTitle();
  printPos(coordinate_system);
  
}
 
void VertexFinder::printTitle() const {
  
  if(_coordinate_system==CARTESIAN) std::cout<<" cartesian ";
  if(_coordinate_system==SPHERICAL) std::cout<<" spherical ";

  if(_time_input_type==TIMES) std::cout<<" hit time ";
  if(_time_input_type==PAIRS) std::cout<<" pairwise ";
  
  std::cout<<getFinderName();

  std::cout<<" finder ";
  if(_par_locked_bit[0] || _par_locked_bit[1] || _par_locked_bit[2]) std::cout<<" with locked pars: ";
  
  if(_par_locked_bit[0] && _coordinate_system==CARTESIAN) std::cout<<" X ";
  if(_par_locked_bit[1] && _coordinate_system==CARTESIAN) std::cout<<" Y ";
  if(_par_locked_bit[2] && _coordinate_system==CARTESIAN) std::cout<<" Z ";
  
  if(_par_locked_bit[0] && _coordinate_system==SPHERICAL) std::cout<<" R ";
  if(_par_locked_bit[1] && _coordinate_system==SPHERICAL) std::cout<<" Theta ";
  if(_par_locked_bit[2] && _coordinate_system==SPHERICAL) std::cout<<" Phi ";

}

void VertexFinder::printPos(int coordinate_system) const {
   
  if(debug_bit && _initial_position){ 
    
    std::cout<<"...starting pos: "<<std::endl;
    _initial_position->printout(coordinate_system);
    
  }
  
  if(_status==FOUND){ 
    
    std::cout<<"...found pos: (confidence= "<<getConfidence()<<")"<<std::endl; 
    ((VertexPos*)this)->VertexPos::printout(coordinate_system);
    
  }
  
  if((debug_bit==0 || _initial_position==0) && _status!=FOUND) std::cout<<std::endl; // make sure there's a new line! 

  
}

void VertexFinder::help(){
 
  
  
}

void VertexFinder::parse(const std::string commands){
    
  std::vector< std::vector< std::string> > parsed = splitCommands(commands);
  
  for(int i=0;i<parsed.size();i++){
   
//     std::cout<<parsed[i][0]<<" = "<<parsed[i][1]<<std::endl;
    
    if(check_str(parsed[i][0], "coordinates")){ std::cout<<"coordinates!!"<<std::endl; setCoordinates(parsed[i][1]);}
    if(check_str(parsed[i][0], "times") || check_str(parsed[i][0], "inputs")) setTimeInputs(parsed[i][1]);
    if(check_str(parsed[i][0], "locked")) setLockedParameter(parsed[i][1]);
    if(check_str(parsed[i][0], "unlocked")) setUnlockedParameter(parsed[i][1]);
    if(check_str(parsed[i][0], "histogram")) setHistFill(parsed[i][1]);
           
  }
  
}

// getter methods
std::string VertexFinder::getFinderName() const { 
  
  return "NULL";
  
}
  
int VertexFinder::isSpherical() const {
  
  if(_coordinate_system==SPHERICAL) return 1;
  else return 0;
  
}

int VertexFinder::isCartesian() const {

  if(_coordinate_system==CARTESIAN) return 1;
  else return 0;
  
}
 
int VertexFinder::getCoordinateSystem() const {
 
  return _coordinate_system;
  
}

int VertexFinder::getLockParameters() const {
  
  return _lock_parameters;
  
}

int VertexFinder::isUsingHitTimes() const {
 
  if(_time_input_type==TIMES) return 1;
  else return 0;
  
}

int VertexFinder::isUsingPairDelays() const {
 
  if(_time_input_type==PAIRS) return 1;
  else return 0;
  
}

int VertexFinder::getInputTimeType() const {
 
  return _time_input_type;
  
}

int VertexFinder::hasFillHist() const {
  
  return _fill_hist;
  
}

int VertexFinder::hasExecuted() const {
  
  return _has_executed;

}

double VertexFinder::getParameter1() const {
  
  return _parameter1;
  
}

double VertexFinder::getParameter2() const {
  
  return _parameter2;
  
}

double VertexFinder::getParameter3() const {
  
  return _parameter3;
  
}

int VertexFinder::getVariant() const {
  
  return _variant;
  
}

VertexPos *VertexFinder::getVertexPos() const {
 
  return (VertexPos*) this;
  
}

VertexPos *VertexFinder::getInitialPosition() const {
  
  return _initial_position;
  
}

ChannelCollection VertexFinder::getChannels() const {
  
  return _channels;
  
}

VertexFinder *VertexFinder::getPreviousFinder() const {
  
  return _previous_finder;

}

int VertexFinder::getDegFreedom() const {
 
  int numPars=3;
  for(int a=0; a<3;a++) numPars-=_par_locked_bit[a];
  
  return _channels.getNumUsefulChannels() - numPars;
  
}

THX *VertexFinder::getHistXD() const {
  
  return _hScan;
  
}

int VertexFinder::checkIfSomethingMissing(){

  _has_executed = 1; // to make sure the finder is not executed twice
 
  if(isUsingHitTimes()){
   
    if(_channels.getNumUsefulChannels()<3){
          
      std::cerr<<__PRETTY_FUNCTION__<<"  ERROR: channels has less than 3 useful channels...\n";
      return 1;
      
    }
    
  }
  
  if(isUsingPairDelays()){
  
    for(int i=0;i<_channels.size();i++){
    
      if(_channels[i]->getRecentDeltaFinderIndex()<0){
      
	std::cerr<<__PRETTY_FUNCTION__<<"  ERROR: channel["<<i<<"] has not been applied a DeltaFinder...\n";
	return 1;
      
      }
      
    }// for i

    _matrix=_channels.getTimingMatrix();
//     _matrix.printErrors();
//     _matrix.printResiduals();

    if(_matrix.getNumUsefulPairs()<3){
      
      std::cerr<<__PRETTY_FUNCTION__<<"  ERROR: matrix has less than 3 useful pairs...\n";
      return 1;
      
    }
  }
  
  
  
  
  
}

// setter methods
void VertexFinder::setChannels(ChannelCollection channels){
  
  _channels=channels;
  
}

void VertexFinder::setPreviousFinder(VertexFinder *previous){

  _previous_finder = previous;

  setInitialPosition((VertexPos*)_previous_finder);
  
}

void VertexFinder::setInitialPosition(VertexPos *position){
   
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(_initial_position){ delete _initial_position; _initial_position=0; }
  
  if(position==0){
   
    _initial_position=new VertexPos();
    
  }
  else _initial_position=new VertexPos(*position);
    
  VertexPos::operator=((VertexPos)*_initial_position);
  
  if(_initial_position->hasRealPos()) this->setRealPosition(_initial_position->getRealPosition());
  
}

void VertexFinder::setInitialPosition(VertexPos position){

//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  setInitialPosition(&position);
  
}

void VertexFinder::setFoundPosition(VertexPos *position){
 
  if(position==0){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cant have a null position here!\n"; }
  else setFoundPosition(*position);
  
}

void VertexFinder::setFoundPosition(VertexPos position){
  
  *((VertexPos*)this)=position;
  if(_initial_position->hasRealPos()) this->setRealPosition(_initial_position->getRealPosition());
  setStatusFound();
  
//   std::cout<<"_initial_position->hasRealPos()= "<<_initial_position->hasRealPos()<<" position.hasRealPos()= "<<position.hasRealPos()<<" this->hasRealPos()= "<<this->hasRealPos()<<std::endl;
  
}

void VertexFinder::setStatusFound(){
  
  _status=FOUND;
  _has_executed=1;

}

void VertexFinder::setStatusFailed(){
 
  _status=FAILED;
  _has_executed=1;

}

void VertexFinder::setUndefinedSolution(){
//   _current_event->setX(-999);
//   _current_event->setY(-999);
//   _current_event->setZ(-999);
//   _current_event->setAmplitudes(_times->getTimes());
//   _current_event->setConfidence(0);


}

void VertexFinder::setCoordinates(int coordinates){

  if(coordinates<0 || coordinates>=2){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: can't input coordinates: "<<coordinates<<"... try CARTESIAN or SPHERICAL"<<std::endl;
    return;
    
  }
  
  _coordinate_system=coordinates;
  
}

void VertexFinder::setCoordinates(const std::string input){

  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" input= "<<input<<std::endl; }
  
  int coordinates;
  
  if(check_str(input, "CARTESIAN")) coordinates=VertexFinder::CARTESIAN;
  else if(check_str(input, "SPHERICAL")) coordinates=VertexFinder::SPHERICAL;
  else{ 
    
    std::cerr<<__PRETTY_FUNCTION__<<" WARNING: unrecognized coordinates: "<<input<<" try CARTESIAN or SPHERICAL..."<<std::endl;
    return;
    
  }
  
  setCoordinates(coordinates);
    
}

void VertexFinder::setTimeInputs(int times){
   
  if(times<0 || times>=2){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: can't use time inputs: "<<times<<"... try PAIRS or HITS"<<std::endl;
    return;
    
  }
  
  _time_input_type = times;
  
}

void VertexFinder::setTimeInputs(const std::string input){
   
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" input= "<<input<<std::endl; }

  int times;
  
  if(check_str(input, "TIMES") || check_str(input, "HITS")) times=VertexFinder::TIMES;
  else if(check_str(input, "PAIRS") || check_str(input,"DELAYS")) times=VertexFinder::PAIRS;
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" WARNING: unrecognized time inputs: "<<input<<" try HITS or PAIRS"<<std::endl;
    return;
    
  }
  
  setTimeInputs(times);
  
}

void VertexFinder::setLockedParameter(int par_number){
 
  if(par_number<0 || par_number>=3){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: can't input parameter number: "<<par_number<<" outside 0-2 range..."<<std::endl;
    return;
    
  }
  
  _par_locked_bit[par_number]=1;
  
}

void VertexFinder::setLockedParameter(const std::string input){
  
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" input= "<<input<<std::endl; }

  int par;
  
  if(check_str(input, "X") || check_str(input, "R") || atoi(input.c_str())==0) par=0;
  else if(check_str(input, "Y") || check_str(input,"THETA") || atoi(input.c_str())==1) par=1;
  else if(check_str(input, "Z") || check_str(input,"PHI") || atoi(input.c_str())==2) par=2;
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" WARNING: unrecognized parameter to lock: "<<input<<" try X or R or Theta"<<std::endl;
    return;
    
  }
  
  setLockedParameter(par);
  
}

void VertexFinder::setUnlockedParameter(int par_number){
 
  if(par_number<0 || par_number>=3){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: can't input parameter number: "<<par_number<<" outside 0-2 range..."<<std::endl;
    return;
    
  }
  
  _par_locked_bit[par_number]=0;
  
}

void VertexFinder::setUnlockedParameter(const std::string input){
   
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" input= "<<input<<std::endl; }

  int par;
  
  if(check_str(input, "X") || check_str(input, "R") || atoi(input.c_str())==0) par=0;
  else if(check_str(input, "Y") || check_str(input,"THETA") || atoi(input.c_str())==1) par=1;
  else if(check_str(input, "Z") || check_str(input,"PHI") || atoi(input.c_str())==2) par=2;
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" WARNING: unrecognized parameter to lock: "<<input<<" try X or R or Theta"<<std::endl;
    return;
    
  }
  
  setUnlockedParameter(par);
  
}

void VertexFinder::setHistFill(int fill){
 
  
  if(fill<0 || fill>=2){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: can't use fill= "<<fill<<"try 0 or 1"<<std::endl;
    return;
    
  }
    
  _fill_hist=fill;
  
}

void VertexFinder::setHistFill(const std::string input){
   
  if(debug_bit){ std::cout<<__PRETTY_FUNCTION__<<" input= "<<input<<std::endl; }

  int fill;
  
  if(check_str(input, "FILL") || check_str(input, "TRUE") || atoi(input.c_str())==1) fill=1;
  else if(check_str(input, "NOFILL") || check_str(input, "FALSE") || atoi(input.c_str())==0) fill=0;
  else{ 
    
    std::cerr<<__PRETTY_FUNCTION__<<" WARNING: unrecognized hist fill command: "<<input<<" try TRUE or FALSE..."<<std::endl;
    return;
    
  }
  
  setHistFill(fill);
    
  
  
}

void VertexFinder::updateLockedParameters(){
 
  switch(_lock_parameters){
   
    case LOCK0 :  _par_locked_bit[0]=1; break;
    case LOCK1 :  _par_locked_bit[1]=1; break;
    case LOCK2 :  _par_locked_bit[2]=1; break;
    
    case LOCK01 : _par_locked_bit[0]=1; _par_locked_bit[1]=1; break;
    case LOCK02 : _par_locked_bit[0]=1; _par_locked_bit[2]=1; break;
    case LOCK12 : _par_locked_bit[1]=1; _par_locked_bit[2]=1; break;
    
  }
  
}

// calculate methods
void VertexFinder::findVertex(){
 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this should be a purely virtual function!\n";
  
}

void VertexFinder::findVertex(ChannelCollection channels, VertexPos initial_position){
 
//   std::cerr<<__PRETTY_FUNCTION__<<std::endl;
  
  findVertex(channels, &initial_position);
  
}

void VertexFinder::findVertex(ChannelCollection channels, VertexPos *initial_position){
 
//   std::cerr<<__PRETTY_FUNCTION__<<std::endl;
  
  setChannels(channels);
  
  setInitialPosition(initial_position);
  
  findVertex();
  
//   std::cout<<"done finding vertex"<<std::endl;
  
}

void VertexFinder::findVertex(VertexPos *initial_position){

  setInitialPosition(initial_position);
  
  findVertex();

}

double VertexFinder::getInteractionTime(Pos position){
  
  _channels.getBestChannel()->getRay()->adjustSourcePosition(position); // make sure this CurvedRay is pointing at the right spot... 
  
  double t0=_channels.getBestChannel()->getTimeOffsetFromRay();// first guess of event time using the highest weight channel
  
  return t0;
  
}

double VertexFinder::estimateErrorR() const {
 
  double station_size=10; // rough estimate. we could somehow get this from channels...
  double timing_precision=0.05; // rough estimate. we could somehow get this from channels...
  double c_speed=0.2; // in meters per nanosecond
  
  return 2*c_speed*timing_precision/std::pow(station_size, 2)*std::pow(getR(), 2);
  
}

double VertexFinder::estimateErrorAngles() const {
  
  double station_size=10; // rough estimate. we could somehow get this from channels...
  double timing_precision=0.05; // rough estimate. we could somehow get this from channels...
  double c_speed=0.2; // in meters per nanosecond
  
  return c_speed*timing_precision/station_size;
  
}

double VertexFinder::myChi(Pos position, double time){ // used with hit times + initial time
 
  // must assume this finder has a ChannelCollection with some useful channels, and that they all have: timing data, curved rays, geometry pointer
  
  double chi=0;
  double sum_w=0;
  double chisquare =0.0;
  
  for(int i=0;i<_channels.getNumChans(); i++){ 
    
    if(_channels[i]->getWeight()>0) {
    
      double error=_channels[i]->getError();
      if(error<0.01) error=0.01;// minimal error to avoid dividing by zero
    
      double weight=_channels[i]->getWeight();
//       weight=1; // debugging
    
      _channels[i]->getRay()->adjustSourcePosition(position);
    
      //      chi =weight*pow( (_channels[i]->getTimeOffsetFromRay() - time) / error, 2);
      chi = time - _channels[i]->getTimeOffsetFromRay();
      
      chisquare += chi*chi;
      
      sum_w+=weight;
      
    }
    
  }// for i

  return chisquare;
 
}

double VertexFinder::myChi(Pos position){ // used with pair delays
    
  // since each PairTiming in _matrix has a pointer to the right channels, updating the channel rays is enough to update the pair flight time differences... 
  
  for(int i=0;i<_channels.getNumChans();i++) _channels[i]->getRay()->adjustSourcePosition(position);
  
  double chi=0;
  
  for(int i=0;i<_channels.getNumChans()-1;i++){
    
    for(int j=i+1; j<_channels.getNumChans(); j++){ 
      
      if(_matrix[i][j].getWeight()>0){// don't use weight zero pairs
   
	double error=_matrix[i][j].getError();
	//	if(error<0.1) error=0.03; // minimal error to avoid dividing by zero...
	if(error<0.01) error=0.01; //to avoid dividing by zero
    
	double weight=_matrix[i][j].getWeight();
	weight=1;

	chi+=weight*pow( (_matrix[i][j].getRayTimeDifference() - _matrix[i][j].getTime()) / error, 2);
 
      }
      
    }
    
  }
  

  // chi/=getDegFreedom();
  
  return chi;
  
}

void VertexFinder::minuitChiSquareOldMethod(int &npar, double *gin, double &f, double *par, int iflag){ 
  
  Pos position;
  
  if(_this_finder->_coordinate_system==SPHERICAL){
   
     position.setSpherical(par[0], par[1], par[2]);
    
  }
  
  if(_this_finder->_coordinate_system==CARTESIAN){
   
    position.setCartesian(par[0], par[1], par[2]);
    
  }
  
  if(_this_finder->isUsingHitTimes()){
   
    par[3]=_this_finder->getInteractionTime(position); 
    f=_this_finder->myChi(position, par[3]); // use this to calculate chi2 using hit times + initial time
    
  }
  else {
  
    f=_this_finder->myChi(position); // use this to calculate chi2 using time delays only
    
  }
  
}

void VertexFinder::minuitChiSquare(int &npar, double *gin, double &f, double *par, int iflag){
  
  Pos position;
  
  if(_this_finder->_coordinate_system==SPHERICAL){

    position.setSpherical(par[0], par[1], par[2]);

  }
  if(_this_finder->_coordinate_system==CARTESIAN){
    
    position.setCartesian(par[0], par[1], par[2]);

  }

  if(_this_finder->isUsingHitTimes()){
    
    f = _this_finder->myChi(position, par[3]);
  }
  
  else {
    
    f = _this_finder->myChi(position);
  }
    

}

double VertexFinder::findInterference(CorrGraph *graph){
  
  double dt=graph->getFirstChannel()->getRay()->getHitTime() - graph->getSecondChannel()->getRay()->getHitTime();
  
  return graph->getValue(dt)*graph->getWeight(); 
  
}

double VertexFinder::findInterference(CorrGraph *graph, Pos source){
 
  graph->getFirstChannel()->getRay()->adjustSourcePosition(source);
  graph->getSecondChannel()->getRay()->adjustSourcePosition(source);

  return findInterference(graph);
  
}

double VertexFinder::findSumInterference(Pos source){
 
  for(int ch=0;ch<_channels.size();ch++){
   
    _channels[ch]->getRay()->adjustSourcePosition(source);
    
  }
  
  double sum=0;
  
  std::vector<CorrGraph*> vec=_channels.getCorrelationGraphVector();
  
  for(int i=0;i<vec.size();i++) sum+=findInterference(vec[i]);
  
  return sum;
  
}

void VertexFinder::calculateConfidence(double quality_parameter){
 
  int locked=0;
  for(int a=0;a<3;a++) locked+=_par_locked_bit[a];
  
  if(_time_input_type==VertexFinder::TIMES) quality_parameter/=(_channels.getNumChans()-1-locked); // divide by degrees of freedom minus parameters...
  if(_time_input_type==VertexFinder::PAIRS) quality_parameter/=(_matrix.getNumCombinations()-locked);
  
  // assumes QP > 0 
  if(quality_parameter>1) setConfidence(1/quality_parameter);
  else setConfidence(1);
  
}
  
VertexFinder *VertexFinder::_this_finder=0;

bool VertexFinder::check_str(const std::string input, const std::string compare){
 
//   std::cout<<__PRETTY_FUNCTION__<<" input= "<<input<<" ("<<input.size()<<") | compare= "<<compare<<" ("<<compare.size()<<")"<<std::endl;
  
  std::string new_input=input;
    
  for(int i=new_input.size()-1;i>=0;i--){ // remove trailing \0's
    
    if(input[i]!='\0') break;
    
    new_input.erase(new_input.size()-1);
    
  } 
  
  for(int i=0;i<new_input.size();i++){ // remove leading spaces
    
    if(new_input[0]!=' ') break;
    else new_input.erase(0);
    
  }
  
  for(int i=new_input.size()-1;i>=0;i--){ // remove trailing spaces
    
    if(input[i]!=' ') break;
    
    new_input.erase(new_input.size()-1);
    
  }
  
  for(int i=0;i<new_input.size();i++){
   
//     std::cout<<"i= "<<i<<" "<<input[i]<<" "<<compare[i]<<std::endl;
    
    if(i>=compare.size() || tolower(new_input[i])!=tolower(compare[i])) return false;
    
  }
    
  return true;
  
}

bool VertexFinder::check_str(const std::string input, const std::string compare1, const std::string compare2){
 
  return check_str(input, compare1) || check_str(input, compare2);
  
}

bool VertexFinder::check_str(const std::string input, const std::string compare1, const std::string compare2, const std::string compare3){

  return check_str(input, compare1) || check_str(input, compare2) || check_str(input, compare3);
  
}

bool VertexFinder::check_str(const std::string input, const std::string compare1, const std::string compare2, const std::string compare3, const std::string compare4){
 
  return check_str(input, compare1) || check_str(input, compare2) || check_str(input, compare3) || check_str(input, compare4);
  
}

bool VertexFinder::isdiv(const char input){
 
    char div[]={',','|'};
    const int Ndiv=sizeof(div);
    
    for(int j=0;j<Ndiv;j++) if(input==div[j]) return true; 
    
    return false;
  
}

bool VertexFinder::isletter(const char input){
  
  if(input>='a' && input<='z') return true;
  if(input>='A' && input<='Z') return true;
  
  return false;
  
}

int VertexFinder::stoi(std::string str){
 
  return atoi(str.c_str());
  
}

double VertexFinder::stof(std::string str){
 
  return atof(str.c_str());
  
}

std::vector< std::vector<std::string> > VertexFinder::splitCommands(const std::string commands){
  
  std::vector< std::vector<std::string> > output;

  int read_letters=0;
  int which=0;
  
  for(int i=0;i<commands.size();i++){
   
//     printf("i= %d commands= %c\n",i, commands[i]);
    
    if(commands[i]=='\0') break;
    
    if(isdiv(commands[i])){ // if you find a dividing character...
      
      if(read_letters==0) continue; // skip dividers if you haven't read any letters yet...
      else{
	
	read_letters=0; // once a dividor comes up, get ready to push back another vector
	which=0; // now go back to first of the two outputs
	continue;
	
      }

    }
    
    if(commands[i]=='='){
      
      if(read_letters==0 || which==1) continue; // ignore '=' if not in the right place
      else if(which==0)
	
	which=1;
	continue;
	
      }

    if(read_letters==0 && isletter(commands[i])){ // if this is the first letter to be read in (after a dividor)

      output.push_back(std::vector<std::string>());
  
      output.back().push_back(std::string()); // first output
      output.back().push_back(std::string()); // second output
      
      which=0; // read into first output
      read_letters=1; // now we can just read the whole thing... 

    }
      
//     std::cout<<"which= "<<which<<" output.size()= "<<output.size()<<"output.back().size()= "<<output.back().size()<<std::endl;
      
    // if non of the above gives a break or continue...
    output[output.size()-1][which].push_back(commands[i]); // input the command letter by letter... 
    
  }// for i
  
  for(int i=0;i<output.size();i++){ // verify string endings...
   
    if(output[i][0][output[i][0].size()-1] != '\0') output[i][0].push_back('\0');
    if(output[i][1][output[i][0].size()-1] != '\0') output[i][1].push_back('\0');
    
  }
  
   // debugging...
//   for(int i=0;i<output.size();i++) std::cout<<output[i][0]<<" = "<<output[i][1]<<std::endl;
  
  return output;
  
}

const std::string VertexFinder::name_vertex_type(int index){
  
  const int size=8;
  const std::string names[size]={"SCAN", "MINUIT", "ASM", "MCMC", "INTMAP", "EARLIEST", "ANYWHERE", "REAL_VERTEX"};
    
  if(index>=size) return names[size-1]; // return last element in case of large index
  
  return names[index];


}

const std::string VertexFinder::name_coordinates(int index){
  
  const std::string names[2]={"SPHERICAL", "CARTESIAN"};
  
  return names[index];
  
}

const std::string VertexFinder::name_lock(int index){
  
  const std::string names[7]={"UNLOCK", "LOCK0", "LOCK1", "LOCK2", "LOCK01", "LOCK02", "LOCK12"};
  
  return names[index];
  
}

const std::string VertexFinder::name_vertex_status(int index){
  
  const int size=3;
  const std::string names[size]={"READY", "FOUND", "FAILED"};
  
  if(index<0 || index>=size) return names[size-1];
  
  return names[index];
  
}

const std::string VertexFinder::name_input(int index){
 
  const std::string names[2]={"TIMES", "PAIRS"};
  return names[index];
  
}
  




