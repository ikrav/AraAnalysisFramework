#ifndef CSWTIMEFINDER_H
#define CSWTIMEFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "FFTtools.h"
#include "TimeFinder.h"
#include <iostream>

class CSWTimeFinder : public TimeFinder {

public :
  
  CSWTimeFinder();
  CSWTimeFinder(TGraph *thisWaveform, TGraph *csw, double par1=0, double par2=0, int channelID=0, int stationID=-1, int isSimulated=1);
  CSWTimeFinder(const CSWTimeFinder &other);
  virtual ~CSWTimeFinder();

  virtual TimeFinder *getDeepCopy();
  CSWTimeFinder &operator=(const CSWTimeFinder &other);

  void initialize();

  // getters
  TGraph *getCSW();
  TGraph *getZeroMeanGraph(TGraph *gr);

  virtual void calculateTime();
  virtual void calculateError();
  virtual void calculateWeight();

  double processCSW();
  double processCSW2ndIter();
  
  TGraph *addGraphs(TGraph *wave1, TGraph *wave2, double dt);
  double getPeakVal(TGraph *gr, int *bin, double cropFactor);
  
  static TGraph *getGraphWithLength(TGraph *gr, int newLength);
  static int deltaT2ndIter;

protected:
  
  /* to hold the CSW
     we can't make it static, since HPol and VPol have independent CSW */
  TGraph *_csw_gr;
  TGraph *_zero_mean_waveform;
  
  ClassDef(CSWTimeFinder,1);
  
};

#endif
