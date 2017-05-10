#ifndef INPUT_H
#define INPUT_H

class Input {
 
public:
  
  Input();
  virtual ~Input();
  
  void initialize();
  virtual void printout()=0;
  
  // getters
  int getCurrentEventNum() const;
  int getMaxNumEvents() const;
  virtual int getTotalNumEvents()const=0;
  
  // commands
  virtual int loadEvent(int event_number=-1)=0;
  virtual bool next()=0;
  
  // setters 
  void setMaxNumEvents(int number);
  void setCurrentEventNum(int event_number);
  
protected:
  
  int _current_event_num;
  int _max_num_events;
  
  
};

#endif
