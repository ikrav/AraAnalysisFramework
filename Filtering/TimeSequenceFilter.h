#ifndef TIMESEQUENCEFILTER_H
#define TIMESEQUENCEFILTER_H

#include "Filter.h"
#include "FFTtools.h"

class TimeSequenceFilter : public Filter {

 public:
  
  TimeSequenceFilter();
  TimeSequenceFilter(EventData *event, int pol=2);
  virtual ~TimeSequenceFilter();

  inline double getQualityParameter() {return _quality;}
  inline void setNeededQualityParameter(double qual) {_minQuality = qual;}
  inline void setThresholdFactor(double factor) {_thFactor=factor;}
  
  void initialize();
  virtual bool pass();
 
 private:

  std::vector<TGraph*> _waves;

  int _nChl;
  int _pol;
  int _sumTime;

  double _thFactor;
  double _blockCount;
  double _minQuality;
  double _quality;
  
  vector<double> _mean;
  vector<double> _stdDev;
  vector<double> _var;

  TH2D * _triggerPattern;

  void getQuality();
  void getHits();
  void patternCheck();

};

#endif
