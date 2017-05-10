#include "DrawTime.h"

ClassImp(DrawTime);

DrawTime::DrawTime(){
  
  initialize();

}

DrawTime::DrawTime(Channel *chan, int hasTime){

  initialize();
  setChannel(chan);
  if(hasTime != 0){
    setTime(chan->getTime());
    calculate();
  }
}

DrawTime::~DrawTime(){
  
  if(_chan)
    delete _chan;
  if(_gr)
    delete _gr;
  if(_hit_time)
    delete _hit_time;
  if(_mg)
    delete _mg;
}

void DrawTime::initialize(){
  
  _chan = 0;
  _gr = 0;
  _mg = 0;
  _hit_time = 0;
}

double DrawTime::getVoltage(){

  int nPoints = _gr->GetN();
  
  double y=0;
  double *xVals = _gr->GetX();
  double *yVals = _gr->GetY();
  
  for(int i=0; i<nPoints; i++){
    if(xVals[i] <= _time_found){
      y = yVals[i];
    }
  }
  //  delete [] xVals;
  //  delete [] yVals;
  
  return y;
}

void DrawTime::calculate(){

  _gr = _chan->getWaveform();
  _gr->SetMarkerSize(0.01);
  double x[1];
  x[0] = _time_found;
  double y[1];
  y[0] = getVoltage();
  
  _hit_time = new TGraph(1, x, y);
  _hit_time->SetMarkerStyle(33);
  _hit_time->SetMarkerColor(2);
  _hit_time->SetMarkerSize(3);
  
  _mg = new TMultiGraph();

  _mg->Add(_gr);
  _mg->Add(_hit_time);
}

TMultiGraph *DrawTime::getGraph() const {
  
  return _mg;
}

Channel *DrawTime::getChannel() const{

  return _chan;
}

void DrawTime::setChannel(Channel *chan){
  
  _chan = chan;
}

void DrawTime::setTime(double t){

  _time_found = t;
}
