#include "RandomFilter.h"

RandomFilter::RandomFilter() : Filter() { }

RandomFilter::RandomFilter(EventData *event) : Filter(event) {

  initialize();
}

RandomFilter::~RandomFilter(){
  
  if(_rand) delete _rand;
  _rand = 0;
}

void RandomFilter::initialize(){

  _rand = new TRandom3();
}

bool RandomFilter::pass(){
  
  return _rand->Binomial(1, _rand->Rndm());
}
