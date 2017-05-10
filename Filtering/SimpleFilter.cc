#include "SimpleFilter.h"

SimpleFilter::SimpleFilter() : Filter() {}

SimpleFilter::SimpleFilter(EventData *event, int pol) : Filter(event) {

  _pol = pol;
  initialize();


}

SimpleFilter::~SimpleFilter() {}


void SimpleFilter::initialize(){}


bool SimpleFilter::pass(){
  bool answer = false;
  switch(_pol){  
  case StationGeometry::VPOL:
    
    if(_data->getChannels().getVpolChannelCollection().getNumUsefulChannels() < 4)
      answer = false;
    else 
      answer = true;
    break;

  case StationGeometry::HPOL:
    if(_data->getChannels().getHpolChannelCollection().getNumUsefulChannels() < 4)
      answer = false;
    else 
      answer = true;
    break;

  case StationGeometry::BOTH:
    if(_data->getChannels().getNumUsefulChannels() < 4)
      answer = false;
    else
      answer = true;
    
  default:
    answer = false;
    break;
  }
  
  return answer;
}
