#ifndef VALUEPOS_H
#define VALUEPOS_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "Pos.h"

class ValuePos : public Pos {

public:
  
  ValuePos();
  ValuePos(double value);
  ValuePos(double x, double y, double z, double value);
  ValuePos(Pos &other_pos, double value=0);
  ~ValuePos();
  
  void printout();
  void help() const; 
  
  enum ValType { CHI=0, QUAL, PROB}; // what does the value parameter represent? chisqure (defualt), generic quality parameter, or something proportional to probability
  
  // getters
  int getType() const;
  std::string getTypeString() const;
  double getValue() const;
  double getRawProbability() const;
  double getProbability();
  double getPowerForQP() const;
  
  // setters
  void setValue(double value); // generic value to be added to this position
  void setType(int type); 
  void setPowerForQP(double power); // how to turn QP into prob? prob=QP^X where X can be nagative or fractional (if type!=CHI)
  void scaleProbability(double scale); // used to multiply by 1 over normalization
  
protected:
  
  void initialize();
  
  int _val_type;
  
  double _value; 
  double _probability; // if negative, requires lazy load from the chisqure function
  double _power_of_qp_for_prob; // what power to raise the quality parameter to get probability (can be negative!)
  
  double calculateProbability();
  
  ClassDef(ValuePos,1);
};

#endif