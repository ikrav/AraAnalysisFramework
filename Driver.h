#ifndef DRIVER_H_
#define DRIVER_H_

#include "OpticalIce.h"
#include "StationGeometry.h"
#include "EventData.h"
#include "L0FileReader.h"
#include "AraSimFileReader.h"
#include "EventGenerator.h"
#include "RootIncludes.h"
#include "Parser.h"
#include "EventData.h"

#include "VertexFinder.h"
#include "Output.h"
#include "SimpleFilter.h"
#include "RandomFilter.h"
#include "PassFilter.h"

class Driver : public Parser {

public:
  
  Driver(int maxEvents=-1, string output="results.root");
  virtual ~Driver();
  
  void initialize();
  void zeroPointers();
  
  void printout();

  void setupL0FileReader(L0FileReader *reader);
  void setupAraSimFileReader(AraSimFileReader *reader);
  void setupEventGenerator(EventGenerator *generator);
  void setOutputFileName(string fileName="results.root");

  void addCommand(string command);
  void setConfigurationFile(); 

  void loop();
  void loadEvent();

  void setOpticalIce(OpticalIce *ice);
  void setStationGeometry(StationGeometry *geom);

  void processEvent();
 
  OpticalIce *getOpticalIce();
  StationGeometry *getStationGeometry();
  EventData *getEventData();
  
  
  void clearUnneeded();
  void setMaxEvents(int num=0);
  void applyFilter();
  
  static void setFilterType(int filter=0, int pol=0);
  enum reader {L0=0, ARASIM=1, GENERATOR=2};
  
  enum pols {VPOL=0, HPOL=1, BOTH=2};

private:
 
  EventData *_event_data; // a pointer to the output's eventData
  
  L0FileReader *_l0_reader;
  AraSimFileReader *_sim_reader;
  EventGenerator *_generator;

  StationGeometry *_geometry;
  OpticalIce *_ice;
 
  Output *_output;
  
  int _max_events;
  int _current_event_number;
  
  int _reader_type;
  int _bool_filter;
  
  string _output_file;
  
  static int _filter_type;
  static int _filter_pol;

};

#endif
