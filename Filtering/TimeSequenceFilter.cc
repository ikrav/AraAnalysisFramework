
#include "TimeSequenceFilter.h"


TimeSequenceFilter::TimeSequenceFilter() : Filter() {}

TimeSequenceFilter::TimeSequenceFilter(EventData *event, int pol=2) : Filter(event) {

  _pol = pol;
  initialize();

}

TimeSequenceFilter::~TimeSequenceFilter(){}

TimeSequenceFilter::initialize(){}

bool TimeSequenceFilter::pass(){

  bool answer = false;

  return answer;

}
