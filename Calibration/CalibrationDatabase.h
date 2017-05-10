#ifndef CALIBRATIONDATABASE_H_
#define CALIBRATIONDATABASE_H_

#include <stdio.h>
#include "sqlite3.h"
//#include "sqlite3ext.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

#include "DbChannel.h"
#include "DbStation.h"

class CalibrationDatabase {
  
 public:
  
  CalibrationDatabase(string dbfile="calibration.db");
  ~CalibrationDatabase();

  
  void initialize();


  void insert(DbChannel *channel);
  void insert(vector<DbChannel*> channels);
  void insert(DbStation *station);
  

  vector<DbChannel*> readRecord(int stationID);
  void deleteRecord(vector<DbChannel*> dbChannels);
  void deleteRecord(DbStation *station);
  
  bool isConnected();
  
  static int callback(void *NotUsed, int argc, char **argv, char **azColName);


 private:
  
  void open();
  void close();


  sqlite3 *_db;
  bool _is_db_open;
  string _db_name;
  char *_error_msg;
};

#endif


