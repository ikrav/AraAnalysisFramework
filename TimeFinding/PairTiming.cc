#include "PairTiming.h"
#include "Channel.h"

ClassImp(PairTiming);

PairTiming::PairTiming() : _chan1(0), _chan2(0), Timing() {}

PairTiming::PairTiming(Channel *chan1, Channel *chan2, const Timing &timing) : _chan1(chan1), _chan2(chan2), Timing(timing) {
 
  
}

PairTiming::PairTiming(const PairTiming &other) : _chan1(other.getFirstChannel()), _chan2(other.getSecondChannel()), Timing(other) {
  
}

PairTiming::~PairTiming(){
  
}

// getters:
Channel *PairTiming::getFirstChannel() const { 
  
  return _chan1;
  
}

Channel *PairTiming::getSecondChannel() const {
 
  return _chan2;
  
}

int PairTiming::getChannelId() const { return _chan1->getChannelId(); }

int PairTiming::getFirstChannelId() const { return _chan1->getChannelId(); }

int PairTiming::getSecondChannelId() const { return _chan2->getChannelId(); }

double PairTiming::getRayTimeDifference() const {
 
  return (_chan2->getRay()->getHitTime() - _chan1->getRay()->getHitTime());
  
}
