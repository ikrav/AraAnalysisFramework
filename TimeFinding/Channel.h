#ifndef CHANNEL_H
#define CHANNEL_H

#include "CppIncludes.h"
#include "RootIncludes.h"
#include "FFTtools.h"

#include "Pos.h"
#include "Timing.h"
#include "PairTiming.h"
#include "StationGeometry.h"
#include "CurvedRay.h"
#include "CorrGraph.h"

#include "TimeFinder.h"
#include "RealTimeFinder.h"
#include "ThreshTimeFinder.h"
#include "CSWTimeFinder.h"
#include "SimpleTimeFinder.h"
#include "GausTimeFinder.h"

#include "DeltaFinder.h"
#include "SubtractDeltaFinder.h"
#include "CrosDeltaFinder.h"
#include "ExpectedDeltaFinder.h"


#include "TGraph.h"

class Channel : public Timing {
  
public: 

  Channel();
  Channel(int channel_id, TGraph *waveform=0, int station_id=-1, int is_simulated=1);
  Channel(const Channel &other);
  virtual ~Channel();
  
  void printout();
  
  void deepCopyTimeFinders(std::vector<TimeFinder*> finders);
  void deepCopyDeltaFinders(std::vector< std::vector<DeltaFinder*> > finders);
  void deepCopyCorrGraphs(std::vector<CorrGraph*> graphs);
  
  void clearFinders();// clears the list of finders and other intermediary data
  void clearWaveform();
  
  // getters
  int getChannelId() const;
  int getStationId() const;
  int isSimulated() const;

  void getFFT(std::vector<double> &real, std::vector<double> &imag, double &freqStep);
  
  double getTimeOffsetFromRay() const;
    
  double getX() const;
  double getY() const;
  double getZ() const;
  std::vector<double> getCartesian() const;
  
  double getR() const;
  double getPhi() const;
  double getTheta() const;
  std::vector<double> getSpherical() const;
  
  Pos getPosition() const;
  
  int getPolarization() const;
  TGraph *getWaveform() const;
  static StationGeometry *getGeometry();
  CurvedRay *getRay();
  
  std::vector<TimeFinder*> getFindersVector() const;
  std::vector< std::vector<DeltaFinder*> > getDeltaFindersMatrix() const;
  std::vector<CorrGraph*> getCorrelationGraphVector() const;
  std::vector<CorrGraph*> getCorrelationGraphVector(std::vector<Channel*> channels);
  CorrGraph *getCorrGraph(Channel *channel); 
  
  int getRecentDeltaFinderIndex() const;

  std::vector<PairTiming> getTimingVector(std::vector<Channel*> channels);
  std::vector<PairTiming> getTimingVector(std::vector<Channel*> channels, int finder_type, double par1=0, double par2=0);
  
  
  // setters
  void setWaveform(const TGraph *waveform); // this creates a deep copy of the waveform, and Channel also destroys it when it dies...
  
  static void setGeometry(StationGeometry *geometry_ptr);
  
  void applyFinder(int finder_type=0, double par1=0, double par2=0);
  int applyDeltaFinder(std::vector<Channel*> channels, int finder_type=0, double par1=0, double par2=0);
  void eraseDeltaFinder(int finder_number, int pair_number); // deletes the finder and erases the vector entry (reallocation ensues)
  void eraseCorrGraph(int pair_number); // deletes the graph and erases the vector entry (reallocation ensues)
  
  bool operator> (const Channel &other) const;
  bool operator> (Channel *other) const;
    
  bool operator< (const Channel &other) const;
  bool operator< (Channel *other) const;
  void useDataFromFinder(int index);

  TimeFinder* operator[](int index) const;
  TGraph *applyCSW(TGraph *csw, int finder_type, double par1, double par2);
  
  void draw();

protected:
  
  void initialize();
  int checkForFinder(int finder_type, double par1=0, double par2=0);
  int addFinder(int finder_type, double par1=0, double par2=0);
  
  void fillDeltaFinder(std::vector<Channel*> channels, int index, int finder_type=0, double par1=0, double par2=0);
  int checkForDeltaFinder(int finder_type, double par1=0, double par2=0);
  int addDeltaFinder(int finder_type, double par1=0, double par2=0);
  DeltaFinder *getDeltaFinder(int second_channel_id, int finder_type=0, double par1=0, double par2=0);
    
  std::vector<TimeFinder*> _finders;

  std::vector< std::vector<DeltaFinder*> > _delta_finders;// the first index is for different types of DeltaFinder's and the second index is for channels found

  int _recent_delta_finder;

  TGraph *_waveform; 
  
  std::vector<CorrGraph*> _corr_graphs;
  
  const int _channel_id;
  const int _station_id;
  const int _is_simulated;
  
  CurvedRay *_ray;
  
  static StationGeometry *_geom;

  ClassDef(Channel, 1);
  
};

#endif
