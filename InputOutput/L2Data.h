#ifndef L2DATA_H_
#define L2DATA_H_

#include "CppIncludes.h"
#include "RootIncludes.h"
#include "AraIncludes.h"
#include "FFTtools.h"
#include "L2Structure.h"

using namespace std;

/****
     classes to store L2Data
     constants defined in AraRoot::L2Structure.h

***/

class Hks {

public:
  Double_t temperature[8];
  Double_t RFPower[ANTS_PER_ICRR];
  UShort_t sclGlobal;
  UShort_t sclL1[12];
  UShort_t scl[24];

  ClassDef(Hks,1);
};

class Triggers {

public:
  Double_t RbClock;  // Rubidium time in seconds. (divided by 280M)
  Double_t DeadTime; // Dead time as a fraction
  Int_t TriggerType;// 
  Int_t EventType; //   
  Int_t TriggerPattern; //
  
  ClassDef(Triggers,1);
};


class Times {

public:
  Int_t hour,minute,sec,usec,dayoy;
  UInt_t time;
  Float_t dt,dtEvent;
  Double_t rbtime;

  ClassDef(Times, 1);
};



class Waveforms {

public:
  Double_t mean[ANTS_PER_ICRR];
  Double_t rms[ANTS_PER_ICRR];
  Double_t v2[ANTS_PER_ICRR];
  Double_t power[ANTS_PER_ICRR];
  Double_t maxV[ANTS_PER_ICRR];
  Float_t freqMax[ANTS_PER_ICRR];
  Float_t freqMaxVal[ANTS_PER_ICRR];
  Float_t powerBin[ANTS_PER_ICRR][N_POWER_BINS];
  Int_t isInTrigPattern[ANTS_PER_ICRR];
  Double_t trackR, trackPhi, trackTheta;
  
  ClassDef(Waveforms,1);
};



class TimeStamp {

public:
  TimeStamp(){};
  TimeStamp(unsigned int unixTime) {TTimeStamp ts = TTimeStamp(unixTime); epoch=unixTime; DOY=ts.GetDayOfYear();
    int dat=ts.GetTime(); hour=(int) dat/10000; minute=(int) dat/100 - hour*100; second=dat-hour*10000-minute*100; DOW=ts.GetDayOfWeek();
};
  ~TimeStamp(){};
  
  unsigned int epoch;
  Int_t DOY;
  Int_t hour;
  Int_t minute;
  Int_t second;
  Int_t DOW;
  
  ClassDef(TimeStamp,1);
};
  
class Headers {
  
public:
  unsigned int RunNumber;
  unsigned int stationId;
  TimeStamp unixTime;   //-i               
  unsigned int unixTimeusec;//-i  
  unsigned int eventNumber;
  int gpsSubTime;
  UShort_t calibStatus;
  UChar_t priority;
  UChar_t errorFlag; 

  ClassDef(Headers,1);
};



class RunHeader {

public:
  Int_t RunNumber;
  Int_t stationId;
  TimeStamp RunStartTime;
  TimeStamp RunEndTime;
  Int_t RunDuration;
  Int_t ErrorFlag;
  Int_t RunType;
  Int_t NumberOfEvents;
  Int_t NumberOfRFEvents;
  Int_t NumberOfPulserEvents;
  Int_t NumberOfForcedEvents;
  Int_t NumberOfUnknownEvents;
  Int_t Nall;

  ClassDef(RunHeader,1);
};


class L2Data : public TObject {

 public: 

  L2Data();
  L2Data(UsefulAraStationEvent *event);

  ~L2Data();

  L2Data(const L2Data &other);
  enum Type { UNKNOWN =0, RF =1, FORCED=2, PULSER = 3};


  void printout(int mode=1);
  void fill(UsefulAraStationEvent *event);


  Headers _header;
  Hks _hk;
  Waveforms _wf;
  Triggers _trigger;
  Times _time_header;

  static unsigned int _startT;
  static unsigned int _endT;
  static RunHeader _run_header;
  static int _n_events;

 private :

  void fillAtri(UsefulAtriStationEvent *Atrievent);
  void fillIcrr(UsefulIcrrStationEvent *Icrrevent);
  
  void fillAtriEventData(UsefulAtriStationEvent *event);
  void fillAtriRunData(UsefulAtriStationEvent *event);

  
  void fillIcrrEventData(UsefulIcrrStationEvent *event);
  void fillIcrrRunData(UsefulIcrrStationEvent *event);
  double fillFFTHistoForRFChanL2(int chan, TH1D *histFFT, TGraph *grRF);
  
  ClassDef(L2Data,1);

};


#endif

