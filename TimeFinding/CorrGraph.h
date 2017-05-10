#ifndef CORRGRAPH_H
#define CORRGRAPH_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "FFTtools.h"

class Channel;

class CorrGraph : public TObject {
  
public:
  
  CorrGraph();
  CorrGraph(Channel *first_channel, Channel *second_channel);
  CorrGraph(const CorrGraph &other);
  virtual ~CorrGraph();
  
  virtual CorrGraph *getDeepCopy();
  // consider adding an operator= at some point  
  
  // getters
  TGraph *getGraph() const;
  TGraph *getHilbertEnvelope() const;
  Channel *getFirstChannel() const;
  Channel *getSecondChannel() const;
  int getFirstChannelId() const;
  int getSecondChannelId() const;
  double getWeight() const;
  
  int getN() const;
  double *getX() const;
  double *getY() const;
  double getValue(double dt) const;
  
  // setters
  void setChannels(Channel *first_channel, Channel *second_channel);
  
  void calculate();
    
  enum type {CORRELATION=0, HILBERTENV};
  
  static int whichGraph;
  
protected:
  
  void initialize();
  
  TGraph *_grCorr;
  TGraph *_grHilbEnv;
  
  Channel *_chan1;
  Channel *_chan2;
  
  ClassDef(CorrGraph,1);
  
};

#endif