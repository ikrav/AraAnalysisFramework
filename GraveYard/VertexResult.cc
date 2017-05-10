#include "VertexResult.h"

int VertexResult::debug_bit=0;

VertexResult::VertexResult(){
  
  initialize();
  
}

VertexResult::VertexResult(int polarization, int time_input_type, int time_finder_type, double par1, double par2, std::vector<int> channel_mask){

  initialize();
  
  setPolarization(polarization);
  setInputTimeChoices(time_input_type, time_finder_type, par1, par2);
  setChannelMask(channel_mask);
  
}

VertexResult::VertexResult(const VertexResult &other){

  initialize();
  
  setInputTimeChoices(other.getTimeInput(), other.getTimeFinderType(), other.getTimeParameter1(), other.getTimeParameter2());
  setPolarization(other.getPolarization());
  setChannelMask(other.getChannelMask());
    
  setVertexFinder(other.getVertexFinder());
  setCombineResultsOption(other.getCombineResultsOption());
  
  setStartingPos(other.getStartingPos());
  setThisPos(other.getThisPos());
  
  // make a deep copy of the linked daughters/sisters... 
  addDaughterResultsVector(other.getDaughterResultsVector());
  addSisterResultsVector(other.getSisterResultsVector());
  
}
  
VertexResult::~VertexResult(){
 
  clear_temp_results();
  _channel_mask.clear();
  
  if(_finder) delete _finder; 
  if(_this_pos) delete _this_pos;
  if(_starting_pos) delete _starting_pos;
    
  // note: this is not a deep clear. If you want that you should do recursiveDelete()
  _daughters.clear();
  _sisters.clear();
  
}

void VertexResult::recursiveDelete(){
 
  for(int i=0;i<_sisters.size(); i++){
    
    if(_sisters[i]){
     
      _sisters[i]->recursiveDelete();
      delete _sisters[i];
      _sisters[i]=0;
      
    }
    
  }// for i
  
  _sisters.clear();
  
  
  for(int i=0;i<_daughters.size(); i++){
    
    if(_daughters[i]){
     
      _daughters[i]->recursiveDelete();
      delete _daughters[i];
      _daughters[i]=0;
      
    }
    
  }// for i
  
  _daughters.clear();
  
}

void VertexResult::clear_temp_results(){
 
  for(int i=0;i<_temp_pos_results.size();i++){
   
    if(_temp_pos_results[i]) delete _temp_pos_results[i];
    
  }
  
  _temp_pos_results.clear();
  
}

void VertexResult::initialize(){
 
  _which_time_input=0;
  _which_time_finder=0;
  _time_par1=0;
  _time_par2=0;
  _use_polarization=0;
  _channel_mask.clear();
  
  _finder=0;
  _starting_pos=0;// this only gets a value when calculate() is called
  _this_pos=0; // this gets filled upon successful call to calculate (if fails, stays zero)
  
  _temp_pos_results.clear();
  
  _combine_results=0;
  
  _sisters.clear();
  _daughters.clear();
    
  
}

void VertexResult::printout(){
 
  std::cout<<"Input: "<<getTimeInputString()<<" "<<getTimeFinderTypeString()<<" finder with pars: "<<getTimeParameter1()<<" "<<getTimeParameter2()<<" pol: "<<getPolString();
  if(_channel_mask.size()) std::cout<<" masking channels: ";
  for(int i=0;i<_channel_mask.size();i++) std::cout<<_channel_mask[i]<<" ";
  std::cout<<std::endl;
  
  if(_finder) _finder->printout();
  if(_starting_pos){ 
    
    std::cout<<"starting pos: ";
    _starting_pos->printout();
    
  }
  
  if(_this_pos){ 
    
    std::cout<<"this pos: ";
    _this_pos->printout();
    
  }
  
  
}

void VertexResult::printRecursive(){
 
  printout();
  
  for(int i=0;i<_sisters.size();i++) {
    
    std::cout <<"Sisters: "<< std::endl;
    _sisters[i]->printRecursive();
  }
    
  for(int i=0;i<_daughters.size();i++){
    
    std::cout <<"Daughter: \t"<< std::endl; 
    _daughters[i]->printRecursive();
  }
  
}

void VertexResult::help(){
 
//   std::cerr<<__PRETTY_FUNCTION__<<" this isn't implemented yet...\n";
  std::cout<<"HELP VertexResult.\nThis object holds one VertexFinder and some preset choices for time finding:";
  std::cout<<"\n *The choice of HitTime/Deltas in setTimeInput(),\n *The type of finder in setTimeFinderType()";
  std::cout<<"\n *Also the polarization to use in setPolarization(), and\n *additional channel masking in setChannelMask()";
  std::cout<<"\n\nEach object can be linked to \'daughter\' and \'sister\' VertexResult objects\n";
  std::cout<<"All sisters get the same starting position as the first object, and their results are averaged.\n";
  std::cout<<"(instead of averaging, you can define each VertexResult to choose the best result instead, using setCombineResultsOption()\n";
  std::cout<<"All daughters get the current object\'s (+sisters\') calculated result as starting pos.\n";
  std::cout<<"When you call calculate the objects do the work, give their result to their daughters\n and receive from them their own calculation (recursively)\n";
  std::cout<<"\nNOTE: copying a VertexResult object results in a recursive copy of all sisters/daughters.\n";
  std::cout<<"When deleting a VertexResult object, by default, the sisters/daughters are not recursively deleted. Use recursiveDelete() on the top-level object.\n";
  std::cout<<"Adding sisters/daughters makes a deep, recursive copy. Using a null pointer creates a new VertexResult with the same time finding options as the calling object.\n";
  std::cout<<"The initial and final position results, as well as intermediate results, are never copied, and are reset with each call to calculate().\n";
  
}

VertexPos *VertexResult::calculate(ChannelCollection channels, VertexPos *starting_pos){
 
  if(_starting_pos) delete _starting_pos;
  _starting_pos=new VertexPos(*starting_pos); // make a deep copy! 
  
  if(_finder){
  
    ChannelCollection new_channels;
    switch(_use_polarization){
   
      case ALL : new_channels=channels; break;
      case VPOL : new_channels=channels.getVpolChannelCollection(); break;
      case HPOL : new_channels=channels.getHpolChannelCollection(); break;
      default : new_channels=channels; cerr<<__PRETTY_FUNCTION__<<" ERROR: undefined polarization choice: "<<_use_polarization<<" using all channels instead\n";
      
    }
  
    new_channels=new_channels.maskChannels(_channel_mask);
  
    _finder->findVertex(channels, _starting_pos);
    
    _temp_pos_results.push_back(new VertexPos(*(VertexPos*) _finder));
    
  }
  
  clear_temp_results();
  
  for(int i=0;i<_sisters.size();i++){// recursively calculate for all sisters...
   
    _temp_pos_results.push_back(_sisters[i]->calculate(channels, starting_pos));
    
  }
  
  if(_this_pos) delete _this_pos;
  _this_pos=VertexPos::meanVertexPos(_temp_pos_results, _combine_results);
  
  clear_temp_results();
  
  if(_this_pos && _daughters.size()){// make sure we can move forward
  
    for(int i=0;i<_daughters.size();i++){// recursively calculate for all daughters based on this result
    
      _temp_pos_results.push_back(_daughters[i]->calculate(channels, _this_pos));

    }
  
    if(_this_pos) delete _this_pos;
    _this_pos=VertexPos::meanVertexPos(_temp_pos_results, _combine_results);
  
  }
  
  return new VertexPos(*_this_pos);// make a deep copy to be returned up the recursive chain! 
  
}

// getters:
int VertexResult::getTimeInput() const {
  
  return _which_time_input;
  
}

std::string VertexResult::getTimeInputString() const {
 
  return name_time_input(getTimeInput());
  
}

int VertexResult::getTimeFinderType() const {
  
  return _which_time_finder;
  
}

std::string VertexResult::getTimeFinderTypeString() const {
  
  if(getTimeInput()==TIMES) return TimeFinder::name_time_type(getTimeFinderType());
  if(getTimeInput()==DELTAS) return DeltaFinder::name_delta_type(getTimeFinderType());
  
}

double VertexResult::getTimeParameter1() const {
  
  return _time_par1;
  
}

double VertexResult::getTimeParameter2() const {
  
  return _time_par2;
  
}

int VertexResult::getPolarization() const {
 
  return _use_polarization;
   
}

std::string VertexResult::getPolString() const {
 
  return name_polarization(getPolarization());
    
}

std::vector<int> VertexResult::getChannelMask() const{
  
  return _channel_mask;
  
}

VertexFinder *VertexResult::getVertexFinder() const {
  
  return _finder;
  
}

VertexPos *VertexResult::getThisPos() const {
  
  return _this_pos;
  
}

VertexPos *VertexResult::getStartingPos() const {
  
  return _starting_pos;
  
}

std::vector<VertexResult*> VertexResult::getDaughterResultsVector() const{
  
  return _daughters;
  
}

int VertexResult::getNumDaughters() const {
  
  return _daughters.size();
  
}

VertexResult *VertexResult::getDaughterResult(int which_daughter) const {
 
  if(getNumDaughters()==0) return 0;// if there are no sisters just quietly return a fail value (null pointer)  
  if(which_daughter<0){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access index "<<which_daughter<<std::endl; return 0; }
  if(which_daughter>=_daughters.size()){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access index "<<which_daughter<<" when _daughters.size()= "<<getNumDaughters()<<std::endl; return 0; }
  
  return _daughters[which_daughter];
  
}

std::vector<VertexResult*> VertexResult::getSisterResultsVector() const{
  
  return _sisters;
  
}

int VertexResult::getNumSisters() const {
  
  return _sisters.size();
  
}

VertexResult *VertexResult::getSisterResult(int which_sister) const {
 
  if(getNumSisters()==0) return 0; // if there are no sisters just quietly return a fail value (null pointer)  
  if(which_sister<0){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access index "<<which_sister<<std::endl; return 0; }
  if(which_sister>=_sisters.size()){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot access index "<<which_sister<<" when _sisters.size()= "<<getNumSisters()<<std::endl; return 0; }
  
  return _daughters[which_sister];
  
}

int VertexResult::getCombineResultsOption() const{
  
  return _combine_results;
  
}

void VertexResult::chooseTimeInput(int which_time_input){
  
  _which_time_input=which_time_input;
  
}

void VertexResult::chooseTimeFinderType(int finder_type){
  
  _which_time_finder=finder_type;
  
}

void VertexResult::setCombineResultsOption(int combine_results_option){
  
  _combine_results=combine_results_option;
  
}

void VertexResult::addDaughterResult(VertexResult *daughter){
 
  if(daughter==0){
   
    daughter=new VertexResult();
    daughter->setInputTimeChoices(getTimeInput(), getTimeFinderType(), getTimeParameter1(), getTimeParameter2());
    daughter->setPolarization(getPolarization());
    daughter->setChannelMask(getChannelMask());
    daughter->setCombineResultsOption(getCombineResultsOption());
    
  }
    
  _daughters.push_back(new VertexResult(*daughter));
  
}

void VertexResult::addDaughterResultsVector(std::vector<VertexResult*> daughters){
  
  for(int i=0;i<daughters.size(); i++){
   
    addDaughterResult(daughters[i]);
    
  }
  
}

void VertexResult::addSisterResult(VertexResult *sister){
  
  if(sister==0){
   
    sister=new VertexResult();
    sister->setInputTimeChoices(getTimeInput(), getTimeFinderType(), getTimeParameter1(), getTimeParameter2());
    sister->setPolarization(getPolarization());
    sister->setChannelMask(getChannelMask());
    sister->setCombineResultsOption(getCombineResultsOption());
    
  }
  
  _sisters.push_back(new VertexResult(*sister));
  
}

void VertexResult::addSisterResultsVector(std::vector<VertexResult*> sisters){
  
  for(int i=0;i<sisters.size(); i++){
   
    addSisterResult(sisters[i]);
    
  }
  
}  
















