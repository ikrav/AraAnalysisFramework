#ifndef SUBTRACTDELTAFINDER_H
#define SUBTRACTDELTAFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "DeltaFinder.h"

class SubtractDeltaFinder : public DeltaFinder {

public:
  
  SubtractDeltaFinder();
  SubtractDeltaFinder(Channel *chan1, Channel *chan2, double par1=0, double par2=0, int stationID=-1, int is_simulated=1);
  SubtractDeltaFinder(const SubtractDeltaFinder &other);
  virtual ~SubtractDeltaFinder();
  
  virtual DeltaFinder *getDeepCopy();
  
  void initialize();
  
//   virtual void calculateTime(Timing first, Timing second);

protected:
  
  virtual void calculateTime();
  virtual void calculateWeight();
  virtual void calculateError();
  
  ClassDef(SubtractDeltaFinder,1);
  
};

#endif
