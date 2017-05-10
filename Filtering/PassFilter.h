#ifndef PASSFILTER_H
#define PASSFILTER_H

#include "Filter.h"

/*****
      Dummy filter to pass all events, (will always return true)

****/

class PassFilter : public Filter {

 public:
  
  PassFilter();
  PassFilter(EventData *event);
  virtual ~PassFilter();

  void initialize();
  virtual bool pass();

};

#endif
