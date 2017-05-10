#include "Timing.h"

ClassImp(Timing);

Timing::Timing(){

//   std::cout<<"Timing: default constructor called..."<<std::endl;
  
  initialize();
  
}

Timing::Timing(double time, double weight, double error){
 
  initialize();
  
  setTimingData(time, weight, error);
    
}

Timing::Timing(const Timing &other){
 
//   std::cout<<"Timing copy constructor called."<<std::endl;
  
  initialize();
  
  setTimingData(other.getTime(), other.getWeight(), other.getError());
  
  if(other.hasRealTime()) setRealTime(other.getRealTime());
  
  setFinderName(other.getFinderName());
  setFinderType(other.getFinderType());
}

Timing::~Timing(){
  
}

void Timing::initialize(){
 
//   std::cout<<"Timing::initialize()"<<std::endl;
  
  _time=0;
  _weight=0;
  _error=0;
  
  _has_real_time=0;
  _real_time=0;
  
  _residual=0;
  
  _finder_name="";
  _finder_type=-1;
  
}

Timing& Timing::operator=(const Timing &other){
  
  Timing temp(other);
  
  swap(temp, *this);
  
  return *this;
  
}

void Timing::swap(Timing &first, Timing &second){
  
  Timing temp(first.getTime(), first.getWeight(), first.getError());
  if(first.hasRealTime()) temp.setRealTime(first.getRealTime());
  temp.setFinderName(first.getFinderName());
  
  first.setTimingData(second.getTime(), second.getWeight(), second.getError());
  if(second.hasRealTime()) first.setRealTime(second.getRealTime());
  first.setFinderName(second.getFinderName());
  
  second.setTimingData(temp.getTime(), temp.getWeight(), temp.getError());
  if(temp.hasRealTime()) second.setRealTime(temp.getRealTime());
  second.setFinderName(temp.getFinderName());
  
}

void Timing::printout() const {
  
  std::cout<<"T= "<<std::setw(10)<<getTime()<<"  W= "<<std::setw(10)<<getWeight()<<"  E= "<<std::setw(10)<<getError();
  if(hasRealTime()) std::cout<<"  residual= "<<std::setw(10)<<getResidual();
  
  if(getFinderName().size()) std::cout<<"  (\""<<getFinderName()<<"\")  ";
  
  std::cout<<std::endl;
  
}

// getters
double Timing::getTime() const { return _time; }

double Timing::getWeight() const { return _weight; }

double Timing::getError() const { return _error; }

int Timing::hasRealTime() const { return _has_real_time; }

double Timing::getRealTime() const { return _real_time; }

double Timing::getResidual() const { return _residual; }

std::string Timing::getFinderName() const { return _finder_name; }

int Timing::getFinderType() const { return _finder_type; }

// setters
void Timing::setTime(double time){ 
  
  _time=time; 
  
  if(hasRealTime()) _residual=_time - _real_time;
  
}

void Timing::setWeight(double weight){ 
  
  if(weight<0) _weight=0;
  else if(weight>1) _weight=1;
  else _weight=weight; 
    
}

void Timing::setError(double error){ _error=error; }

void Timing::setTimingData(double time, double weight, double error){
  
  setTime(time);
  setWeight(weight);
  setError(error);
  
//   std::cout<<__PRETTY_FUNCTION__<<"  time= "<<getTime()<<std::endl;
  
}

void Timing::setRealTime(double time){

  _real_time=time;
  
  _residual=_time - _real_time;
  
  _has_real_time=1;
  
}

void Timing::setFinderName(std::string name){
  
  _finder_name=name;
  
}

void Timing::setFinderType(int finder_type){
  
  _finder_type=finder_type;
  
}