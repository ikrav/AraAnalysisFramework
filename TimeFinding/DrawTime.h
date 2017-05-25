#ifndef DRAWTIME_H
#define DRAWTIME_H

#include "TGraph.h"
#include "CppIncludes.h"
#include "TMultiGraph.h"
#include "Channel.h"

/********************************************
    Simple class to draw the time found on the WF
    Use:
          Draw("AL*")  -- for nice visual
*****************************************/

class DrawTime {

  
 public:
  DrawTime();
  DrawTime(Channel *chan, int hasTime=1);
  ~DrawTime();
  
  TMultiGraph *getGraph() const;
  
  //int getN() const;
  //double *getX() const;
  //double getTime() const;
  Channel *getChannel() const;

  void setTime(double t);
  void setChannel(Channel *chan);
  void calculate();
  double getVoltage();

  
 protected:
  void initialize();

  
  TGraph *_gr;
  TGraph *_hit_time;
  TMultiGraph *_mg;
  Channel *_chan;
  double _time_found;
  
  ClassDef(DrawTime,1);

};

#endif
  
 
