#ifndef EXPECTEDDELTAFINDER_H
#define EXPECTEDDELTAFINDER_H


#include "CppIncludes.h"
#include "RootIncludes.h"

#include "DeltaFinder.h"
#include "CurvedRay.h"
#include "Pos.h"
#include "Channel.h"

class ExpectedDeltaFinder : public DeltaFinder {
  
 public:
  ExpectedDeltaFinder();
  ExpectedDeltaFinder(Channel *chan1, Channel *chan2, double par1=0, double par2=0, int stationID=-1, int is_simulated=1);
  ExpectedDeltaFinder(const ExpectedDeltaFinder &other);
  
  virtual ~ExpectedDeltaFinder();
  
  void initialize();
  
  virtual DeltaFinder *getDeepCopy();
  
 protected:
  
  virtual void calculateTime();
  virtual void calculateError();
  virtual void calculateWeight();
  
  ClassDef(ExpectedDeltaFinder, 1);
  
};

#endif
 
