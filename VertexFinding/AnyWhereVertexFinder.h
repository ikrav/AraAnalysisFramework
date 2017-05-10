#ifndef ANYWHEREVERTEXFINDER_H
#define ANYWHEREVERTEXFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexFinder.h"

class AnyWhereVertexFinder : public VertexFinder {
  
 public:
  AnyWhereVertexFinder();
  AnyWhereVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  AnyWhereVertexFinder(const std::string commands);
  AnyWhereVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands);  
  AnyWhereVertexFinder(const AnyWhereVertexFinder &other);
  
  virtual ~AnyWhereVertexFinder();
  
  void initialize();
  void help();
  void parse(const std::string commands);
  
  virtual VertexFinder *getDeepCopy();
  virtual std::string getFinderName() const;

  // setters
  void setPosition1(double position);
  void setPosition2(double position);
  void setPosition3(double position);
    
  virtual void findVertex();
  
 protected:
  
  ClassDef(AnyWhereVertexFinder, 1);
 

};

#endif
