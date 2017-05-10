#ifndef PAIRTIMING_H
#define PAIRTIMING_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "Timing.h"

class Channel;

class PairTiming : public Timing {

public:
  
  PairTiming();
  PairTiming(Channel *chan1, Channel *chan2, const Timing &timing);
  PairTiming(const PairTiming &other);
  virtual ~PairTiming();
  
  Channel *getFirstChannel() const;
  Channel *getSecondChannel() const;
  
  int getChannelId() const;
  int getFirstChannelId() const;
  int getSecondChannelId() const;
  
  double getRayTimeDifference() const; 
  // add methods for getting the position/polarization of the antennas in the pair... 
  
protected:
  
  Channel *_chan1;
  Channel *_chan2;

  ClassDef(PairTiming,1);
  
};

#endif
