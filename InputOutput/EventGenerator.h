#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

////////////////////////////////////////////////////////////////
//								//
//	Simple event generator for testing reconstruction.	//
//	Generates a vertex in the ice and makes waveforms	//
//	with timing data based on the ice model and geometry.	//
//								//
//	send any questions to: guy.nir@weizmann.ac.il		//
//								//
////////////////////////////////////////////////////////////////

/**
   EventGenerator class
   EventGenerator.cc
   Purpose: Simple event generator for testing reconstruction

   @author Guy Nir
   @version 0.1
*/

#include "CppIncludes.h"

#include "SimData.h"
// #include "EventData.h"
// #include "Input.h"

#include "StationGeometry.h"
#include "CurvedRay.h"
#include "OpticalIce.h"

#include "Pos.h"

#include "Channel.h"
#include "ChannelCollection.h"

class EventGenerator : public TNamed {
  
public:
  
  EventGenerator();
  EventGenerator(const EventGenerator &other);
  ~EventGenerator();
  
  void reset();
  void printout();
     
  void generateEvent(int print_bit=0);
  int loadEvent(int event_number=-1);
  virtual bool next();
  
  void deleteChannels(); // only use if you are not giving the channels to Reco
  
  // getters
  //  Channel **getChannelArray() const;
  Channel **getChannelArray();

  //  ChannelCollection getChannelCollection() const;
  ChannelCollection getChannelCollection();
  
  // getters
  int getCurrentEventNum() const;
  int getMaxNumEvents() const;
  double getAmpAtSource(double amp_at_station, Pos source_pos);
  double getEnergyFromAmplitdue(double amplitude);// gives the hypothetical energy required to make the mean amplitde given. 
  double getAmplitudeFromEnergy(double energy_eV);
  
  StationGeometry *getGeometry();
  StationGeometry *getStationGeometry();

  OpticalIce *getOpticalIce();
  SimData *getSimData();
  Pos getSourcePosition() const;
  Pos getRealPosition() const;
  
   
  double getXmin() const;
  double getXmax() const;
  double getYmin() const;
  double getYmax() const;
  double getZmin() const;
  double getZmax() const;
  double getRmin() const;
  double getRmax() const;
  
  double getAmplitudeLog() const;
  double getAmpSpread() const;
  double hasAmpDecay() const;
  
  double getMinPolarization() const;
  double getMaxPolarization() const; 
  double getSignalWidth() const; 
  int getNumSignalChannels() const;
  
  int getTotalNumEvents() const;
  
  // setters
  void setMaxNumEvents(int num_events);
  void setCurrentEventNum(int event_number);

  void setStationGeometry(double string_radius=10, double vertical_separation=15);
  
  void setXrange(double min_x, double max_x=-10000);
  void setYrange(double min_y, double max_y=-10000);
  void setZrange(double min_z, double max_z=-10000);
  
  void setVertexRadius(double min_radius, double max_radius);
  void setAmplitudeLog(double amplitude_log, double amp_spread_log=0.1, int amp_decay=0);
  void setPolarizationRange(double min_vertical, double max_vertical=-1);
  void setAttenuationDecaysBit(int att_bit_state);
  void setSignalWidth(double width);
  
  void setNumSignalChannels(int numChannels);
  
  // output methods
  TCanvas *drawWaveforms();// draws the wavefroms on a canvas (expects 16 waveforms)
//   void writeEvent();// writes SimData and the waveform graphs to an open root file. 
  
  static const double PI;
  static const double FREQ; // waveform oscillation frequency (envelope is muliplied by cos(FREQ*t)
 
  
  
protected:

    
  void initialize();
  void clear_channels();
  
  void calculateConversionFactor();// numerical integral for finding the conversion btw nu energy and signal amplitde at the source. 
  
  // internal methods used in the calculations... 
  void selectVertexPosition();
  void selectPolarizationRatio();
  void setSourceAmp();// used internally by generateEvent()
  
  void fillGraphWithNoise(TGraph *gr, double RMS);// expects an empty graph
  void addSignalToGraph(TGraph *gr, double time, double amplitude, double width);
  std::vector<int> selectAntennas(int polarization);
  
  int _current_event_num;
  int _max_num_events;
  
  std::vector<Channel*> _channels;
  
  StationGeometry *_geom; // station geometry data
  OpticalIce *_ice; //! don't save to root file
  SimData *_data; // event position, hit times and amplitudes
  TRandom3 *_rand;
  std::vector<CurvedRay> _rays;
  
  int _num_points_waveform;// number of bins in a wavefrom
  double _step_size;// nanoseconds per bin
  
  int _numEvents;
  
  double _min_x_pos;
  double _max_x_pos;
  
  double _min_y_pos;
  double _max_y_pos;
  
  double _min_z_pos;// max depth
  double _max_z_pos;
  
  double _min_radius;
  double _max_radius;
  
  // for full cherenkov cone generation
  int _build_cherenkov_bit; // if 0: use simple isotropic amplitude. if 1: use advanced tools to make emission from shower profile
  double _min_energy_log;
  double _max_energy_log;
  
  double _theta_central_value;
  double _theta_spread_width;
  
  double _phi_central_value;
  double _phi_spread_width;
  
  double _conversion_factor;
  
  // for simple event generation
  int _use_decaying_amplitude_bit;// if 0: set same amplitude for all antennas, if 1: set amplitude at source position and let it decay with distance
  int _use_curved_raytrace_bit; // if 0: use straight line or uniform ice approximation, if 1: use full raytracing algorithms
 
  double _signal_amplitude_log;
  double _signal_amplitude_spread;
  double _signal_envelope_width;
  double _noise_RMS;
  double _polarization_ratio;
  
  double _min_polarization_vertical;
  double _max_polarization_vertical;
  
  double _signal_width;
  
  int _num_signal_channels;
  
  ClassDef(EventGenerator, 1);
  
};

#endif
