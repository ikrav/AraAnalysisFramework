#include "CalibrationDatabase.h"

CalibrationDatabase::CalibrationDatabase(string dbFile){

  initialize();
  _db_name = dbFile;
  
  open();
}

CalibrationDatabase::~CalibrationDatabase(){

  close();
}

void CalibrationDatabase::initialize(){

  _is_db_open = false;

}

void CalibrationDatabase::open(){

  char *_error_msg;
  
  int rc;
  
  //open database 
  rc = sqlite3_open(_db_name.c_str(), &_db);
  
  if(rc){
    std::cerr <<"Cannot open database: " << sqlite3_errmsg(_db) << std::endl;
    exit(-1);
  }
  else {
    std::cerr <<"opened database! "<< std::endl;
  }

  _is_db_open = true;

}

void CalibrationDatabase::close(){

  sqlite3_close(_db);
}

bool CalibrationDatabase::isConnected(){

  return _is_db_open;
}

void CalibrationDatabase::insert(DbStation *station){

  string sql_statement = "INSERT INTO STATION (STATION_NUMBER, DESCRIPTION, TAG)" \
    "VALUES (1, 'testing', 1.0)";
  
  int rc;
  rc = sqlite3_exec(_db, sql_statement.c_str(), callback, 0, &_error_msg);
  
  if (rc != SQLITE_OK){
    std::cerr <<"SQL error: "<< _error_msg << std::endl;
    exit(-1);
  }
  else {
    std::cout <<"Record added successfully!"<< std::endl;
  }
 
}

void CalibrationDatabase::insert(DbChannel *channel){
    
  string sql_statement = "INSERT INTO CHANNELS(CHANNEL_INDEX, CHANNEL_NAME, DESCRIPTION, CALIBRATION_DX, CALIBRATION_DY, CALIBRATION_DZ, CALIBRATION_DT, STATION_ID, VALIDITY_RANGE_START, VALIDITY_RANGE_END, VALIDITY_RANGE_ID)" \
    "VALUES(";
  
  sql_statement += channel->getData() + ")";
   

  int rc;
  rc = sqlite3_exec(_db, sql_statement.c_str(), callback, 0, &_error_msg);
    
  if( rc != SQLITE_OK){
      
    std::cerr<<"SQL error: "<< _error_msg << std::endl;
    exit(-1);
  }
  else{
    std::cout <<"Record added successfully!"<< std::endl;
  }

}

void CalibrationDatabase::insert(vector<DbChannel*> channels){

  for(int i=0; i< channels.size(); i++){  

    insert( channels[i]);
  }//for i channels
}

vector<DbChannel*> CalibrationDatabase::readRecord(int stationID){

  return vector<DbChannel*>();
}

int callback(void *NotUsed, int argc, char **argv, char **azColName){
  
  int i =0;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  
  printf("\n");
  return 0;
}


