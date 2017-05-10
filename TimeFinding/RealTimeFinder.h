#ifndef REALTIMEFINDER_H
#define REALTIMEFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "TimeFinder.h"

class RealTimeFinder : public TimeFinder{

public:
  
  RealTimeFinder();
  RealTimeFinder(TGraph *waveform, double par1, double par2, int channelID=0, int stationID=-1, int isSimulated=1);
  RealTimeFinder(const RealTimeFinder &other);
  
  virtual ~RealTimeFinder();
  
  virtual TimeFinder *getDeepCopy();
  RealTimeFinder& operator=(const RealTimeFinder &other);
  
protected:
  
  virtual void initialize();
  
  virtual void calculateTime();
  virtual void calculateWeight();
  virtual void calculateError();
  
  TRandom3 *_rand;

  ClassDef(RealTimeFinder,1);
  
};

#endif