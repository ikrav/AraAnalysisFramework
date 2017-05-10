#ifndef RANDOMFILTER_H
#define RANDOMFILTER_H


#include "Filter.h"
#include "TRandom3.h"

class RandomFilter : public Filter {


 public :
  RandomFilter();
  RandomFilter(EventData *event);
  virtual ~RandomFilter();


  void initialize();
  virtual bool pass();


 private:

  TRandom3 *_rand;

};
  
#endif
