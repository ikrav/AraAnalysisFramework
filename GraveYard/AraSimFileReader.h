#ifndef ARASIMFILEREADER_H
#define ARASIMFILEREADER_H

#include "CppIncludes.h"
#include "SimIncludes.h"
#include "RootIncludes.h"

#include "Input.h"

#include "StationGeometry.h"

#include "Channel.h"
#include "ChannelCollection.h"
#include "SimData.h"

/**
   AraSimFileReader class \fn
   AraSimFileReader.cc \fn
   Purpose: Read AraSim output files and return time graphs and geometry

   @author Christian Bora
   @version 0.1
*/

class AraSimFileReader : public Input {

 public:
  AraSimFileReader(std::vector<std::string> files, int stationId=0);
  AraSimFileReader(std::string files, int stationId=0);
  ~AraSimFileReader();

  //getters
  int getTotalNumEvents() const;
  double getInterpolationFactor();
  int getCurrentEventNumber() const;
  int getStationId();
  int getNumberOfStations();
  int getNumberOfAntennasPerStation(int stationId);
  bool getNextEvent();
  virtual bool next();

  int loadEvent(int event_number=-1);
  
  ChannelCollection getChannelCollection();
  bool getSpecificEvent(int eventNumber);

  TChain *getAraTree();
  TChain *getAraTree2();
  TChain *getEventTree();

  StationGeometry *getStationGeometry();
  SimData *getSimData();

  //setters
  void setInterpolationFactor(double factor);
  void setDefaultInterpolationFactor();
  void printout();
  void printout(int mode);


 private:
  vector<string> _file_names;
  TFile *_file_in;
  TChain *_event_tree;
  TChain *_ara_tree;
  TChain *_ara_tree_2;
  

  Detector *_detector;
  Report *_report;
  Settings *_settings;
  Event *_event;
  IceModel *_ice_model;
  Spectra *_spectra;
  Trigger *_trigger;

  StationGeometry *_geom;
  SimData *_sim_data;

  int _total_events;
//   int _current_event;
  double _interpolation_factor;
  int _run_number;
  int _station_number;
  vector<Channel*> _channels;

  void reset();
  void specific_setup();
  void getReadyForNextEvent();
  void initialize();
  void initializeGeometry();
  void open();
  void lookAtEvent(int eventNumber);
  void createGeometry();
  
  ClassDef(AraSimFileReader,1);

};

#endif
