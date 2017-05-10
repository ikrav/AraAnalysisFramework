#ifndef EVENTVERTEX_H
#define EVENTVERTEX_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexPos.h"
#include "VertexFinder.h"

// add all kinds of xxxVertexFinder includes here:
#include "ScanVertexFinder.h"
#include "MinuitVertexFinder.h"
#include "RealVertexFinder.h"
#include "MCMCVertexFinder.h"
#include "IntMapVertexFinder.h"
#include "AnalyticVertexFinder.h"
#include "AnyWhereVertexFinder.h"
#include "EarliestVertexFinder.h"
#include "MinuitUNLKUVertexFinder.h"

class EventVertex : public VertexPos {

public:
  
  EventVertex();
  EventVertex(ChannelCollection channels, OpticalIce *ice=0);
  EventVertex(ChannelCollection channels, Pos starting_position, OpticalIce *ice=0);
  EventVertex(ChannelCollection channels, VertexPos *starting_position, OpticalIce *ice=0);
  virtual ~EventVertex();
  
  void printout(int coordinate_system=0);
    
  void addFinder(int type=0, int coordinate_system=0, int time_input=0, int lock_parameter=0, double par1=0, double par2=0, double par3=0, int variant=0);
  void findVertex();
  
  void findVertex(int starting_pos);
  
  void pushFinders(); // move current _finders into _prev_finders 
  void clearFinders();
  void clearPrevFinders();
  
  // getters
  OpticalIce *getOpticalIce() const;
  int getNumChannels() const;
  int getNumFinders() const;
  int getNumPrevFinders() const;
  ChannelCollection getChannels() const;

  std::vector<VertexFinder*> getFinderVector() const;
  std::vector< std::vector<VertexFinder*> > getPrevFinderVector() const;
  VertexFinder *getFinder(int index=0) const;
  VertexFinder *getPrevFinder(int level=0, int index=0) const;
  
  // setters
  void setOpticalIce(OpticalIce *ice);
  void setChannels(ChannelCollection channels);
  
  // cleanup
  void deleteChannels();
  void deleteWaveforms();
  void deleteVertices();
  
  enum type {DEFAULT=0, PREVIOUS=1, AVERAGE=2, BEST=3};
  
  static int debug_bit;
  
protected:
  
  void initialize();
  
  ChannelCollection _channels;
  
  OpticalIce *_ice;//! don't save to file
  
  std::vector<VertexFinder*> _finders;
  std::vector< std::vector<VertexFinder*> > _prev_finders;
  
  ClassDef(EventVertex,1);
  
};

#endif
