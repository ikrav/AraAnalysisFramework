#ifndef EVENTDATA_H
#define EVENTDATA_H

////////////////////////////////////////////////////////////////
//								//
//	Container for all useful data on the event		//
//	Holds VertexPos, channels etc...		 	//
//	used to save reconstruction results...		 	//
//								//
//	send any questions to: guy.nir.@weizmann.ac.il		//
//								//
////////////////////////////////////////////////////////////////

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexPos.h"
#include "VertexFinder.h"
// #include "EventVertex.h"

#include "ChannelCollection.h"
#include "Channel.h"

#include "OpticalIce.h"
#include "StationGeometry.h"

#include "SimData.h"
#include "L2Data.h"

class EventData : public TNamed {
  
public:

  EventData();
  EventData(ChannelCollection channels, StationGeometry *geometry, OpticalIce *ice);
  
  virtual ~EventData();
  
  void printout(int print_level=0);
  void printVertexVpol(int coordinate_system=0);
  void printVertexHpol(int coordinate_system=0);
  void printVertexCombined(int coordinate_system=0);

  void setData(ChannelCollection channels, StationGeometry *geometry, OpticalIce *ice);

  // getters
  VertexPos *getVertexResult() const;
//   EventVertex *getVertexCombined() const;
//   EventVertex *getVertexVpol() const;
//   EventVertex *getVertexHpol() const;
//   EventVertex *getVertexArbitrary() const;
//   EventVertex *getEventVertex() const; // gives the default vertex being used (Vpol/Hpol/Combined)
  ChannelCollection getChannels() const;
  
  OpticalIce *getOpticalIce() const;
  StationGeometry *getStationGeometry() const;
  SimData *getSimData() const;
  //  L2Data *getL2Data() const;

  // setters
  void setSimData(SimData *data); // make a deep copy of simulated data from AraSim or from EventGenerator
  //  void setL2Data(L2Data *data);

  void addVertexFinderVpol(int finder_type, int coordinate_system=0, int time_input=0, int lock_parameter=0, double par1=0, double par2=0, double par3=0);
  void addVertexFinderHpol(int finder_type, int coordinate_system=0, int time_input=0, int lock_parameter=0, double par1=0, double par2=0, double par3=0);
  void addVertexFinderCombined(int finder_type, int coordinate_system=0, int time_input=0, int lock_parameter=0, double par1=0, double par2=0, double par3=0);
  void addVertexFinder(int pol, int finder_type, int coordinate_system=0, int time_input=0, int lock_parameter=0, double par1=0, double par2=0, double par3=0);

  void findVertex(int pol, int starting_position=0);
  void chooseResults(int choice);// can choose between BEST and AVERAGE for the choice btw all EventVertex objects. 
  
  // clear data methods
  void clearChannels(); // to save space and also in destructor
  void clearWaveforms(); // to save disk space
  void clearTimeFinders();
  void clearVertices(); // clears the intermidiate vertex finding results to save space
  void clearPrevVertexFinders();// clears the previously found vertex finder objects
  void clearVertexFinders();
  
  enum Calc_Pol {CALC_ALL=0, CALC_VPOL, CALC_HPOL, CALC_BOTH, CALC_COMBINED};
  enum Result_Pol {AVERAGE=0, BEST};
  
protected:
  
  void initialize();
  
//   EventVertex *_vertexVpol;
//   EventVertex *_vertexHpol;
//   EventVertex *_vertexCombined;
//   EventVertex *_vertexArbitrary;
  
  VertexPos *_vertexResult; // best/average result, by which-ever method used
  ChannelCollection _channels; // this is where all channels are saved. EventData is responsible for deleting the Channel objects.
  
  OpticalIce *_ice;//! don't save to root file
  StationGeometry *_geometry;
  
  SimData *_sim_data;
  //  L2Data *_l2_data;
  
  ClassDef(EventData,1);
  
};

#endif
