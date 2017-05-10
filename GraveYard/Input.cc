#include "Input.h"

Input::Input(){
  
  initialize();
  
}

Input::~Input(){
  
}

void Input::initialize(){
  
  _current_event_num=0;
  _max_num_events = 1000; // maybe change this default at some point... 
  
}

int Input::getCurrentEventNum() const {
  
  return _current_event_num;
  
}

int Input::getMaxNumEvents() const {
  
  return _max_num_events;
  
}

void Input::setMaxNumEvents(int number){
  
  _max_num_events=number;
  
}

void Input::setCurrentEventNum(int event_number){
 
  _current_event_num=event_number;
  
}

