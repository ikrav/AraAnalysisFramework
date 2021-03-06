#ifndef SIMFILEREADER_H
#define SIMFILEREADER_H

#include "CppIncludes.h"
#include "RootIncludes.h"
#include "SimIncludes.h"

#include "FileReader.h"

#include "StationGeometry.h"
#include "Channel.h"
#include "ChannelCollection.h"
#include "SimData.h"

/**
   SimFileReader class \fn
   SimFileReader.cc \fn
   Purpose: Read AraSim output files and return time graphs and geometry

   adapted from AraSimFileReader by Guy Nir
   questions: guy.nir@weizmann.ac.il
   
   @author Christian Bora
   @version 0.1
*/

class SimFileReader : public FileReader {

public:
  
  SimFileReader(std::string filenames);
  virtual ~SimFileReader();
  
  void initialize();
  virtual bool setChains();
  virtual bool setBranches();
  virtual void setupGeometry();
  virtual void loadChannelsReport();
  void clearEvent();
  
  virtual void printout();
  
  virtual bool loadEvent(int eventNumber);
  
  // getters
  virtual bool isCalPulser() const;
  virtual bool isSimulation() const;
  virtual Pos getRealPosition();
  virtual Pos getSourcePosition();
  virtual SimData *getSimData();
  
  static int debug_bit;
  
private:
  
  int isFromReport() const;// check where we get the wavefroms
  
  TChain *_sim_chain1;
  TChain *_sim_chain2;
  
  Detector *_detector;
  Settings *_settings;
  
  Report *_report;
  Event *_event;
  
  Pos _real_pos;

  
};

#endif