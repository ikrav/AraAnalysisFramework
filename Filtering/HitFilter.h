#ifndef HITFILTER_H
#define HITFILTER_H

#include "Filter.h"

class HitFilter : public Filter {

 public:
  
  HitFilter(EventData *event);
  virtual ~HitFilter();

  void initialize();
  virtual bool pass();

};

#endif
