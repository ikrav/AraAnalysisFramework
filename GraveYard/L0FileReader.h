#ifndef L0FILEREADER_H_
#define L0FILEREADER_H_

#include "CppIncludes.h"
#include "RootIncludes.h"
#include "AraIncludes.h"

#include "Input.h"

#include "StationGeometry.h"
#include "Channel.h"
#include "ChannelCollection.h"
#include "FFTtools.h"
//#include "L2Data.h"

/**
   L0FileReader class \fn
   L0FileReader.cc \fn
   Purpose: Read L0 files and return graphs and geometry for the station    

   @author Christian Bora
   @version 0.1
*/

class L0FileReader : public Input {

 public:
  L0FileReader(std::string l0File, std::string vrmsFile="", int useSurfaceAnt=0);
  ~L0FileReader();

  //getters
  
  void printout();
  void printout(int mode);
  
  StationGeometry *getStationGeometry();
  int getTotalNumEvents() const;

  
  double getInterpolationFactor();

  int getCurrentEventNumber() const;
  int getAraCurrentEventNumber();

  int getRunNumber();

  int getStationId();
  bool getNextEvent();
  
  virtual bool next();

  int loadEvent(int event_number=-1);
  

  bool isUsingSurfaceAnts();
  TTree *getTree();

  UsefulAraStationEvent *getUsefulEvent();

  ChannelCollection getChannelCollection();
  bool getSpecificEvent(int eventNumber);
  
  bool isIcrrEvent();
  bool isAtriEvent();
  int isCalPulser();
  
  void setInterpolationFactor(double factor);
  void setDefaultInterpolationFactor();    
  void setUseSurfaceAnts(int bit);

  void loadVRMS();


 private:
  int _total_events;
//   int _current_event;
  double _interpolation_factor;
  int _run_number;
  int _station_number;
  std::string _file_name;
  int _ara_event_number;
  int _is_atri;
  int _is_icrr;
  int _is_cal_pulser;
  std::string _vrms_file;
  int _use_surface_ants;

  std::vector<double> _VRMS;
  TFile *_file_in;
  TTree *_event_tree;
  
  RawAraStationEvent *_raw_ev;
  RawIcrrStationEvent *_raw_icrr_ev;
  RawAtriStationEvent *_raw_atri_ev;
  UsefulAtriStationEvent *_real_atri_ev;
  UsefulIcrrStationEvent *_real_icrr_ev;

  AraStationInfo *_station_info;
  StationGeometry *_geom;
  StationGeometry *_v_geom;
  std::vector<Channel*> _channels;
  ChannelCollection _channel_collection;
  //L2Data *_l2_data;

  void reset();
  void specificInitialization();
  void getReadyForNextEvent();
  void initialize();
  void open();
  void lookAtEvent(int eventNumber);
  void createGeometry();
  
  ClassDef(L0FileReader, 1);
  
};


#endif
