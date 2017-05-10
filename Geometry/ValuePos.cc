#include "ValuePos.h"

ClassImp(ValuePos);

ValuePos::ValuePos() : Pos() {

  initialize();
  
}

ValuePos::ValuePos(double value) : Pos() {
 
  initialize();
  
  setValue(value);
  
}

ValuePos::ValuePos(double x, double y, double z, double value) : Pos(x,y,z) {
   
  initialize();
  
  setValue(value);
    
}

ValuePos::ValuePos(Pos &other_pos, double value) : Pos(other_pos) {
 
  initialize();
  setValue(value);
  
}

ValuePos::~ValuePos(){
 
  
  
}

void ValuePos::initialize(){
 
  _value = 0;
  _val_type=ValuePos::CHI;
  _probability = -1;
  _power_of_qp_for_prob = 1;
  
  
}

void ValuePos::printout(){
  
  Pos::printout();
  std::cout<<"Value= "<<getValue()<<" type: "<<getTypeString()<<" Probability= "<<getProbability();
  if(_val_type==CHI) std::cout<<" Power of QP= "<<getPowerForQP();
  std::cout<<std::endl;
  
}

void ValuePos::help() const {
  
}

int ValuePos::getType() const {
  
  return _val_type;
  
}

std::string ValuePos::getTypeString() const {
 
  std::string str="";
  
  switch(_val_type){
   
    case CHI:
      str.append("Chi Square"); break;
    case QUAL:
      str.append("Quality Parameter"); break;
    case PROB:
      str.append("Probability"); break;
    default:
      str.append("unknown type"); break;
          
  }
    
  return str;
  
}

double ValuePos::getValue() const {
  
  return _value;
  
}

double ValuePos::getRawProbability() const{
  
  return _probability;
  
}

double ValuePos::getProbability(){
  
  if(_probability<0) _probability = calculateProbability();
  
  return _probability;
  
}

double ValuePos::getPowerForQP() const {
  
  return _power_of_qp_for_prob;
  
}

void ValuePos::setValue(double value){
 
  _value = value;
  _probability = -1;
  
}

void ValuePos::setType(int type){
 
  _val_type=type;
  _probability = -1;
  
}

void ValuePos::setPowerForQP(double power){
  
  _power_of_qp_for_prob = power;
  _probability = -1;
  
}

void ValuePos::scaleProbability(double scale){
  
  _probability = getProbability()/scale;
  
}

double ValuePos::calculateProbability(){
 
  switch(_val_type){
   
    case CHI:
      return std::exp(-0.5*(_value));
    case QUAL:
      return std::pow(_value, _power_of_qp_for_prob);
    case PROB:
      return _value;
    default:
      return -1;
      std::cerr<<__PRETTY_FUNCTION__<<" ERROR: unknown value type: "<<_val_type<<std::endl;
    
  }
  
}













