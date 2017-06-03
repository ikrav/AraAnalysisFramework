#ifndef TIMESEQUENCEFILTER_H
#define TIMESEQUENCEFILTER_H

#include "Filter.h"

class TimeSequenceFilter : public Filter {

 public:

  TimeSequenceFilter();
  TimeSequenceFilter(EventData *event, int pol=2);
  virtual ~TimeSequenceFilter();

  
  void initialize();
  virtual bool pass();
 
 private:


  
};

#endif
