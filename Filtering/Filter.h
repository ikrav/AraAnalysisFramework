#ifndef FILTER_H
#define FILTER_H


#include "EventData.h"


class Filter {
  
 public :
  
  Filter();
  Filter(EventData *event);
  virtual ~Filter();
  
  
  void initialize();
  
  virtual bool pass()=0;
  
  EventData *getEventData();
  
  void setEventData(EventData *event);

  enum Type {PASS=0, RANDOM, SIMPLE, SURFACE};
  static const std::string name_input(int index);

 protected:
  
  EventData *_data;

};

#endif
