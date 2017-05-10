#include "Filter.h"


Filter::Filter() {

  initialize();

}

Filter::Filter(EventData *event){

  initialize();
  setEventData(event);
}

Filter::~Filter(){

  _data = 0; //don't own the object
}

void Filter::initialize(){

  _data = 0;

}

EventData *Filter::getEventData(){
  
  return _data;
}

void Filter::setEventData(EventData *event){
 
  _data = event;
}

const std::string Filter::name_input(int index){

  const std::string names[3]={"PASS", "RANDOM", "SIMPLE"};
  return names[index];

}
