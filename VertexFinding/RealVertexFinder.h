#ifndef REALVERTEXFINDER_H
#define REALVERTEXFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexFinder.h"

class RealVertexFinder : public VertexFinder {

public:
  
  //RealVertexFinder();
  RealVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  RealVertexFinder(const std::string commands);
  RealVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands);
  
  RealVertexFinder(const RealVertexFinder &other);
  virtual ~RealVertexFinder(); 
  
  void initialize();
  virtual void help();
  virtual void parse(const std::string commands);
  
  virtual void findVertex();
  
  virtual VertexFinder *getDeepCopy();
  
  virtual std::string getFinderName() const;
  
protected:
  
  virtual void calculateConfidence(double quality_parameter);

  ClassDef(RealVertexFinder,1);
  
};

#endif
