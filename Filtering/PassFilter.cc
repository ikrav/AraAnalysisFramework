#include "PassFilter.h"

PassFilter::PassFilter() : Filter() {}

PassFilter::PassFilter(EventData *event) : Filter(event) {}

PassFilter::~PassFilter() {}

bool PassFilter::pass() {
  
  return true;
}
