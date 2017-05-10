#ifndef CROSDELTAFINDER_H
#define CROSDELTAFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "DeltaFinder.h"

#include "FFTtools.h"

class CrosDeltaFinder : public DeltaFinder {

public:
  
  CrosDeltaFinder();
  CrosDeltaFinder(Channel *chan1, Channel *chan2, double par1=0, double par2=0, int stationID=-1, int is_simulated=1);
  CrosDeltaFinder(const CrosDeltaFinder &other);
  virtual ~CrosDeltaFinder();
    
  void initialize();
  
  virtual DeltaFinder *getDeepCopy();
    
protected:
  
  virtual void calculateTime();
  virtual void calculateError();
  virtual void calculateSignalWidthAmp(TGraph *gr, int best_bin, double best_amp);
  
  ClassDef(CrosDeltaFinder,1);
  
};

#endif
