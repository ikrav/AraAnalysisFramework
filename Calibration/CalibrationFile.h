#ifndef CALIBRATIONFILE_H_
#define CALIBRATIONFILE_H_

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "DbChannel.h"
#include "DbStation.h"

using namespace std;

class CalibrationFile {

 public:

  CalibrationFile(string calibFile="calibration.txt");
  ~CalibrationFile();

  void initialize();

  vector<DbChannel*> getChannels();
  DbStation* getStation();
  
  void readFile(string fileName);
  void readDtFile(string fileName);
  void readGeomFile(string fileName);
  void setStation(DbStation *station);

private:
  
  void clearChannels();  
  void clear();
  
  vector<DbChannel*> _channels;
  DbStation *_station;
  
  string _file_name;
  string _dt_file_name;
  string _geom_file_name;

};

#endif
