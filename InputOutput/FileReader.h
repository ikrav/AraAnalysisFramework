#ifndef FILEREADER_H_
#define FILEREADER_H_

#include "CppIncludes.h"
#include "RootIncludes.h"
#include "AraIncludes.h"

#include "StationGeometry.h"
#include "Channel.h"
#include "ChannelCollection.h"
#include "FFTtools.h"
#include "L2Data.h"

/**
   FileReader class \fn
   FileReader.cc \fn
   Purpose: Read L0 files and return graphs and geometry for the station  
   
   adapted from L0FileReader by Guy Nir 
   questions: guy.nir@weizmann.ac.il
   
   @author Christian Bora
   @version 0.1
*/

class FileReader {

public:
  
  FileReader(std::string filenames, std::string vrmsFile="", int useSurfaceAnt=0);
  virtual ~FileReader();
  
  void initialize();
  virtual bool setChains();
  virtual bool setBranches();
  virtual void setupGeometry();
  virtual void clearEvent();
  void clearChannels();

  virtual void printout();
  
  bool loadNextEvent();
  virtual bool loadEvent(int event_number);
  void loadChannels();
  bool loadVRMS();
    
  // getters
  std::string getFileNames() const;
  TChain *getChain();
  UsefulAraStationEvent *getUsefulEvent();

  int getTotalNumEvents();
  int getCurrentEventNumber() const;
  int getAraCurrentEventNumber();
  int getRunNumber();
  int getStationId();
  L2Data *getL2Data();
  
  bool isIcrrEvent();
  bool isAtriEvent();
  virtual bool isCalPulser();
  virtual bool isSimulation() const;
  
  double getInterpolationFactor() const;
  bool isUsingSurfaceAnts() const;

  StationGeometry *getStationGeometry();
  ChannelCollection getChannelCollection() const;

  // setters
  void setMaxNumEvents(int number_events);
  void setInterpolationFactor(double factor);
  void setDefaultInterpolationFactor();    
  virtual void setUseSurfaceAnts(int bit);

  static int debug_bit;
  
protected:
   
  int _num_chans;
  
  int _total_events;
  int _current_event_num;
  double _interpolation_factor;
  int _station_number;

  int _is_atri;
  int _is_icrr;
  int _use_surface_ants;

  int _branches_set;
  
  std::string _file_names;
  std::string _vrms_file;
    
  std::vector<double> _VRMS;
  
  TChain *_chain;
  
  RawAraStationEvent *_raw_ev;
  RawIcrrStationEvent *_raw_icrr_ev;
  RawAtriStationEvent *_raw_atri_ev;
  UsefulAtriStationEvent *_real_atri_ev;
  UsefulIcrrStationEvent *_real_icrr_ev;

  AraStationInfo *_station_info;
  StationGeometry *_geom;
  ChannelCollection _channels;
  L2Data *_l2_data;
    
  ClassDef(FileReader, 1);
  
};


#endif
