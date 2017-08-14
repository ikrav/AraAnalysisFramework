#ifndef HITFILTER_H
#define HITFILTER_H

#include "Filter.h"

class HitFilter : public Filter {

 public:
  
  HitFilter();
  HitFilter(EventData *event);
  virtual ~HitFilter();

  inline int getNumberOfHits() {return _hits;}
  void getNeededMultiplierVector(vector<double> &neededV);

  void initialize();
  virtual bool pass();
  int findNumberOfHits();

  inline void setThresholdMultiplier(double val) { _thresholdMultiplier = val; }

 private:

  double _thresholdMultiplier;
  int _hits;

};

#endif
