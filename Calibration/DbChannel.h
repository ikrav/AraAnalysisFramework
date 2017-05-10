#ifndef DBCHANNEL_H_
#define DBCHANNEL_H_

#include "DbStation.h"
#include <sstream>
#include <string>
using namespace std;

class DbChannel {

 public:
  
  DbChannel(DbStation *station=0);
  ~DbChannel();

  int getChannelIndex();
  string getChannelName();
  string getChannelDescription();
  
  double getdX();
  double getdY();
  double getdZ();
  
  double getdT();

  DbStation *getStation();
  
  string getValidityRangeStart();
  string getValidityRangeEnd();
  int getValidityRangeID();
  

  //setters
  void setChannelIndex(int index);
  void setChannelName(string name);
  void setChannelDescription(string description);
  
  void setdX(double value);
  void setdY(double value);
  void setdZ(double value);
  
  void setdT(double value);
  
  void setStation(DbStation *station);
  
  void setValidityRangeStart(string range);
  void setValidityRangeEnd(string range);
  void setValidityRangeID(int id);
  
  string getData(); // returns all data in a string format (comma separated)

  void populate(string values);
  
  string toString(double value);
  string toString(int value);

 private:

  void initialize();

  int _channel_index; // have to correspond to AraROOT's channel index
  string _channel_name;
  string _description;
  
  double _dX;
  double _dY;
  double _dZ;
  
  double _dT;
  
  DbStation *_station;
  
  string _validity_range_start;
  string _validity_range_end;
  int _validity_range_id;
  
};

#endif
  
  
    
