#ifndef CHANNELCOLLECTION_H
#define CHANNELCOLLECTION_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "Channel.h"
#include "TimingMatrix.h"
#include "CorrGraph.h"
#include <string>
#include "DrawTime.h"
#include "TGraph.h"

class ChannelCollection : public TObject {
 
public:
  
  ChannelCollection();
  ChannelCollection(std::vector<Channel*> channels);
  ChannelCollection(const ChannelCollection &other);
  virtual ~ChannelCollection();
  
  ChannelCollection *getDeepCopy();// makes a deep copy and returns a poionter... 
  ChannelCollection &assignDeepCopy(const ChannelCollection &other); // like operator= only it makes a deep copy
  ChannelCollection& operator=(const ChannelCollection &other);
  
  void printout() const;
  void drawWaveforms() const;
  void drawTimeFound() const;
  void clear();// notice this DOES NOT delete the channels themselves, only clears the vector! deleting is done using clear_channels().
  void delete_channels(); 
  void clearFinders();
  void clearWaveforms();
  
  
  // getters
  int size() const;
  int empty() const;
  Channel *back(); 
  int getNumChans() const;
  int getNumUsefulChannels() const;
  Channel *getChannel(int index) const;
  Channel *getChannelById(int channel_id) const;
  std::vector<Channel*> getVector() const;
  Channel *getBestChannel() const;
  
  Channel *getEarliestChannel() const;
  
  Channel *operator[](int index) const;
  
  ChannelCollection getVpolChannelCollection() const;
  ChannelCollection getHpolChannelCollection() const;
  ChannelCollection getPolarizedChannelCollection(int polarization=0) const;
  ChannelCollection getSubset(std::vector<int> channel_ids) const;
  ChannelCollection maskChannels(std::vector<int> channel_ids) const;  
  
  TimingMatrix getTimingMatrix();
  TimingMatrix getTimingMatrix(int finder_type, double par1=0, double par2=0);
  std::vector<CorrGraph*> getCorrelationGraphVector();
  
  // setters
  void reserve(int number);
  void push_back(Channel *channel);
  void setVector(std::vector<Channel*> channels);
  
  // apply stuff
  //apply either time/ delta
  void applyTimeDeltaFinder(int finder_type, int delta=0, double par1=0, double par2=0);
  void applyFinder(int finder_type, double par1=0, double par2=0);
  void applyDeltaFinder(int finder_type, double par1=0, double par2=0);
  
  TGraph *getZeroMeanGraph(TGraph *gr);
  void maxCorrOrderer(int *results, int iter);
  TGraph *getCSW();
  
protected:
  
  // this object doesn't own these resources, unless it is the very first copy (or if we got it from getDeepCopy())
  std::vector<Channel*> _channels;
  TGraph *_csw; 

  ClassDef(ChannelCollection, 1);
  
};

#endif
