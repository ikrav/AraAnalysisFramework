#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "Filter.h"
#include "StationGeometry.h"

class SimpleFilter : public Filter {

 public:
  SimpleFilter();
  SimpleFilter(EventData *event, int pol=2);
  virtual ~SimpleFilter();

  
  void initialize();
  virtual bool pass();

 private:

  int _pol;

};

#endif
