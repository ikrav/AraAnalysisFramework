#ifndef EARLIESTVERTEXFINDER_H
#define EARLIESTVERTEXFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexFinder.h"

class EarliestVertexFinder : public VertexFinder {

 public:
  EarliestVertexFinder();
  EarliestVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  EarliestVertexFinder(const std::string commands);
  EarliestVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands);  
  EarliestVertexFinder(const EarliestVertexFinder &other);
  
  virtual ~EarliestVertexFinder();
  
  virtual void findVertex();
  
  virtual VertexFinder *getDeepCopy();
  
  void initialize();
  virtual void help();
  virtual void parse(const std::string commands);
  
  virtual std::string getFinderName() const;
  
 protected:
  
  ClassDef(EarliestVertexFinder, 1);

};

#endif
