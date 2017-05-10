#ifndef DBSTATION_H_
#define DBSTATION_H_

#include <sstream>
#include <string>

using namespace std;

class DbStation {

 public:
  
  DbStation();
  ~DbStation();

  void initialize();

  
  int getStationNumber();
  string getDescription();
  double getTag();
  

  //setters
  void setStationNumber(int stationID);
  void setDescription(string description);
  void setTag(double tag);
  
  string getData();
  void populate(string data);
  
  string toString(int value);
  string toString(double value);
 private:

  int _station_number;
  string _description;
  double _tag;

};
#endif
