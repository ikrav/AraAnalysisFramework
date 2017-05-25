#ifndef GAUSTIMEFINDER_H
#define GAUSTIMEFINDER_H

/////////////////////////////////////////////////
// 	derived class from TimeFinder  	        //
//	Uses g-filter for finding peak		//
//	times for each channel.			//
//	questions: guy.nir@weizmann.ac.il	//
/////////////////////////////////////////////////

/**
   GaussTimeFinder class
   GaussTimeFinder.cc
   Purpose: uses Guassian smoothing function to find peaks
   
   @author Guy Nir
   @version 0.1
*/

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "TimeFinder.h"
#include "TGraph.h"
#include <cmath>

class GausTimeFinder : public TimeFinder {
 
public:

  GausTimeFinder();
  GausTimeFinder(TGraph *waveform, double par1=0, double par2=0, int channelID=0, int stationID=-1, int isSimulated=1);
  GausTimeFinder(const GausTimeFinder &other);
  virtual ~GausTimeFinder();
 
  virtual void initialize();
  virtual void calculateTime();
//   virtual void calculateError();
//   virtual void calculateWeight();
  
  virtual TimeFinder *getDeepCopy();
  //GausTimeFinder& operator=(const GausTimeFinder &other);
  
  static TGraph *gausmooth(TGraph *gr, int numBins=12, double sigma=3);
  void getPeak(int N, double *X, double *Y, double &time, double &clarity, int numBins=12);
  void getPeak(TGraph *gr, double &time, double &clarity, int numBins=12);
  //void getPeak(TGraph *gr, double *time_array, double *clarity_array, int iterations, int numBins=12);

protected:
  
  double _average_error;

  ClassDef(GausTimeFinder,1);
  
};

#endif
