#ifndef TIMEFINDER_H
#define TIMEFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "Timing.h"
#include "StationGeometry.h"

class TimeFinder : public Timing {
  
public:
  
  TimeFinder();
  TimeFinder(TGraph *waveform, double par1=0, double par2=0, int channelID=0, int stationID=-1, int is_simulated=1);
  TimeFinder(const TimeFinder &other);
  virtual ~TimeFinder();

  void printout();
    
  virtual void scanWaveform();

  // getters
  double getScale() const;
  double getThresh() const;
  
  int getChannelId() const;
  int getStationId() const;
  
  int isSimulated() const;
  int isMeasured() const;
  
  TGraph *getWaveform() const;
  
  virtual double getPar1() const;
  virtual double getPar2() const;
  
  virtual int isUsingParameters(double par1, double par2) const;
  
  virtual TimeFinder *getDeepCopy()=0;
  
  TimeFinder& operator=(const TimeFinder &other);
  
  // setters
  void setChannelId(int channel_id);
  void setStationId(int station_id);
  
  void setScale(double scale);
  void setThresh(double threshold);
    
  void setWaveform(TGraph *waveform);
  
  void setSimulated(int is_simulated);
  
  static void setGeometry(StationGeometry *geom);
  
  virtual void setParameters(double par1=0, double par2=0);
  
  enum Type { THRESH=0, GAUS=1, CSW=2, SIMPLE=3,
	      REAL_TIMES=10};
  
protected:
    
  int _channel_id;
  int _station_id;
  
  int _is_simulated;
    
  double _scale; // used to scale the quality parameter into weight
  double _thresh;// used as baseline for quality parameter when switching to weight
  
  double _quality_parameter;// single parameter from scan of waveform (depends on method used. may add other paramers for complicated finders)  
  double _signal_width; // secondary parameter used to store the width of the signal (for error estimates)
  
  double _parameter1;
  double _parameter2;
  
  TGraph *_waveform;
      
  void initialize();
  
  virtual void calculateTime()=0;
  virtual void calculateWeight(); // convert from specific method's quality parameter to a 0-1 weight range
  virtual void calculateError(); // use the results of calculations to derive the error (in nanosec) 
  
  static const int MAX_NUM_STATIONS=1;// currently have measurements... 
  static const int MAX_NUM_CHANS=16;

  static std::vector< std::vector<double> > fill_static_vector();
  
  static const double epsilon;

  static StationGeometry *_geom;
    
  
public:
  
  static std::vector< std::vector<double> > _measured_rms;// if any finder needs to know the baseline RMS for this specific channel (make sure you have the right stationID/channelID
  static const std::string name_time_type(int index);

  ClassDef(TimeFinder,1);
  
};

#endif
