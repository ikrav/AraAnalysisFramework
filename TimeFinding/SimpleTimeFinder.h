#ifndef SIMPLETIMEFINDER_H
#define SIMPLETIMEFINDER_H

#include "CppIncludes.h"
#include "TimeFinder.h"
#include <cmath>

// just finds the maximum of the waveform

class SimpleTimeFinder : public TimeFinder {

 public :

  SimpleTimeFinder();
  SimpleTimeFinder(TGraph *waveform, double par1=0, double par2=0, int channelID=0, int stationID=-1, int isSimulated=1);
  SimpleTimeFinder(const SimpleTimeFinder &other);
  ~SimpleTimeFinder();

  virtual TimeFinder *getDeepCopy();
  
 protected:
  virtual void initialize();
  
  virtual void calculateTime();
  virtual void calculateError();
  virtual void calculateWeight();

  ClassDef(SimpleTimeFinder,2);
  
};

#endif 
  
