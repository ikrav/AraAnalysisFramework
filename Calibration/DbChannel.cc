#include "DbChannel.h"

DbChannel::DbChannel(DbStation *station){
  
  initialize();
  _station = station;
}

DbChannel::~DbChannel(){
  
  _station = 0;
}

void DbChannel::initialize(){
  
  //initialize all values to default

  _channel_index =-1;
  _channel_name = "NULL";
  _description = "NULL";
  
  _dX = 0;
  _dY = 0;
  _dZ = 0;
  
  _dT = 0;

  _station =0;
  
  _validity_range_start ="default";
  _validity_range_end ="default";
  
  _validity_range_id =-1;
    
}

int DbChannel::getChannelIndex(){
  
  return _channel_index;
}

string DbChannel::getChannelName(){
  
  return _channel_name;
}

string DbChannel::getChannelDescription(){

  return _description;
}

double DbChannel::getdX(){

  return _dX;
}

double DbChannel::getdY(){

  return _dY;
}

double DbChannel::getdZ(){

  return _dZ;
}

double DbChannel::getdT(){
  
  return _dT;
}

DbStation *DbChannel::getStation(){

  return _station;
}

string DbChannel::getValidityRangeStart(){
  
  return _validity_range_start;
}

string DbChannel::getValidityRangeEnd(){

  return _validity_range_end;
}

int DbChannel::getValidityRangeID(){

  return _validity_range_id;
}

string DbChannel::getData(){
  
  string value = toString(_channel_index) + ", " + _channel_name + ", " + _description + ", " + toString(_dX) + ", " + toString(_dY) + ", " + toString(_dZ) + ", " + ", " + toString(_dT) + ", " + _validity_range_start + ", " + _validity_range_end + ", " + toString( _validity_range_id);

  return value;
}

void DbChannel::setChannelIndex(int index){
  
  _channel_index = index;
}

void DbChannel::setChannelName(string name){
  
  _channel_name = name;
}

void DbChannel::setChannelDescription(string description){
  
  _description = description;
}

void DbChannel::setdX(double value){

  _dX = value;
}

void DbChannel::setdY(double value){
  
  _dY = value;
}

void DbChannel::setdZ(double value){

  _dZ = value;
}

void DbChannel::setdT(double value){

  _dT = value;
}

void DbChannel::setStation(DbStation *station){
  
  _station = station;
}

void DbChannel::setValidityRangeStart(string range){
  
  _validity_range_start = range;
}

void DbChannel::setValidityRangeEnd(string range){
  
  _validity_range_end = range;
}

void DbChannel::setValidityRangeID(int id){

  _validity_range_id = id;
}

string DbChannel::toString(int value){
  
  stringstream convert;
  convert << value;
  return convert.str();
}

string DbChannel::toString(double value){
  
  stringstream convert;
  convert << value;
  return convert.str();
}
