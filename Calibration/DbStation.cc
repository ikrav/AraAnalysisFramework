#include "DbStation.h"

DbStation::DbStation(){

  initialize();
}

DbStation::~DbStation(){

}

void DbStation::initialize(){

  //default values
  _station_number = -1;
  _description = "NULL";
  _tag = 0.0;

}

int DbStation::getStationNumber(){
  
  return _station_number;
}

string DbStation::getDescription(){
  
  return _description;
}

double DbStation::getTag(){

  return _tag;
}

string DbStation::getData(){
  
  string value = toString(_station_number) + ", " + _description +", " +  toString(_tag);
  return value;
}

void DbStation::setStationNumber(int stationID){
  
  _station_number = stationID;
}

void DbStation::setDescription(string description){
  
  _description = description;
}

void DbStation::setTag(double tag){

  _tag = tag;
}

string DbStation::toString(int value){

  stringstream convert;
  convert << value;
  return convert.str();
}

string DbStation::toString(double value){

  stringstream convert;
  convert << value;
  return convert.str();
}
