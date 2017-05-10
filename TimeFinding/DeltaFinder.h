#ifndef DELTAFINDER_H
#define DELTAFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "TimeFinder.h"

class Channel;

class DeltaFinder : public TimeFinder {

public:
  
  DeltaFinder();
  DeltaFinder(Channel *chan1, Channel *chan2, double par1=0, double par2=0, int stationID=-1, int is_simulated=1);
  DeltaFinder(const DeltaFinder& other);
  virtual ~DeltaFinder();
  
//   static DeltaFinder* createInverse(const DeltaFinder* other);// generator for finders with inverted time (and chanID and waveforms)
  void inverse();
  
  // getters
  Channel *getFirstChannel() const;
  Channel *getSecondChannel() const;
  
  int getFirstChannelId() const;
  int getSecondChannelId() const;
  TGraph *getFirstWaveform() const;
  TGraph *getSecondWaveform() const;
      
  virtual DeltaFinder *getDeepCopy()=0;
  DeltaFinder& operator=(const DeltaFinder &other);
  
  // setters
  void setFirstChannel(Channel *chan);
  void setSecondChannel(Channel *chan);
  
  enum Type { SUBTRACT=0, CROS, EXPECTED,
	      REAL_DELTAS};
  
	      
  bool operator<(const DeltaFinder &other) const; // note this checks the SECOND CHANNEL ID for comparison!
  bool operator<(DeltaFinder *other) const;
  
  static const std::string name_delta_type(int index);
  
protected:
  
  Channel *_chan1;
  Channel *_chan2;
  
//   int _second_channel_id;
//   TGraph *_second_waveform;
  
  void initialize();

  ClassDef(DeltaFinder,1);
  
};

#endif
