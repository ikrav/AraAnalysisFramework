#ifndef THRESHTIMEFINDER_H
#define THRESHTIMEFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "TimeFinder.h"

// finds the number of bins in the waveform above a threshold
// and calculates the time (first bin above thresh) and the
// error (number of bins above thresh).

class ThreshTimeFinder : public TimeFinder{
  
public:
  
  ThreshTimeFinder();
  ThreshTimeFinder(TGraph *waveform, double par1=0, double par2=0, int channelID=0, int stationID=-1, int isSimulated=1);
  ThreshTimeFinder(const ThreshTimeFinder &other);
  virtual ~ThreshTimeFinder();
  
  virtual TimeFinder *getDeepCopy();
  
protected:
    
  double _threshold_multiplier;

  virtual void initialize();
  
  virtual void calculateTime();
  virtual void calculateError();
  
  virtual void useMeasuredThreshold();
  
  ClassDef(ThreshTimeFinder,1);
  
};

#endif
