#include "CalibrationFile.h"

CalibrationFile::CalibrationFile(string FileName){

  initialize();
}

CalibrationFile::~CalibrationFile(){

  if(_station) delete _station;
  _station = 0;
  
  clearChannels();
}

void CalibrationFile::clear(){
 
  clearChannels();
}

void CalibrationFile::clearChannels(){

  for(int i=0; i< _channels.size(); i++){
    if(_channels[i]) delete _channels[i];
    _channels[i] = 0;
  }
  _channels.clear();
}

void CalibrationFile::initialize(){
  
  _station=0;
  clearChannels();
}

void CalibrationFile::readDtFile(string fileName){
 
  clearChannels();

  _dt_file_name = fileName;

  ifstream _dt_file;
  _dt_file.open(_dt_file_name.c_str());
  double value;
  
  while(!_dt_file.eof()){
    
    if(_dt_file.eof() || !_dt_file.good()) break;
    
    _dt_file >> value;
    DbChannel *channel = new DbChannel(_station);
    channel->setdT(value);
    _channels.push_back(channel);
  }

}

void CalibrationFile::readGeomFile(string fileName){
  
  clearChannels();

  _geom_file_name = fileName;
  ifstream _geom_file;
  
  _geom_file.open(_geom_file_name.c_str());
  
  double dx, dy, dz;
  int i=0;
  string separator;
  while(!_geom_file.eof()){

    if(_geom_file.eof() || !_geom_file.good()) break;
    
    if(i==0) _geom_file >> separator;
    if(i==1) _geom_file >> dx;
    if(i==2) _geom_file >> dy;
    if(i==3) {
      _geom_file >> dz;

      DbChannel *channel = new DbChannel(_station);
      channel->setdX(dx);
      channel->setdY(dy);
      channel->setdT(dz);
      _channels.push_back(channel);
      i=-1;
    } 
    i++;
  }
}    

void CalibrationFile::readFile(string fileName){

  clearChannels();

  _file_name = fileName;
  ifstream _file;
  
  _file.open(_file_name.c_str());
  
  double dx, dy, dz, dt;
  int i=0;
  string separator;
  while(!_file.eof()){

    if(_file.eof() || !_file.good()) break;
    
    if(i==0) _file >> separator;
    if(i==1) _file >> dx;
    if(i==2) _file >> dy;
    if(i==3) _file >> dz;
    if(i==4) {
      _file >> dt;
      DbChannel *channel = new DbChannel(_station);
      channel->setdX(dx);
      channel->setdY(dy);
      channel->setdZ(dz);
      channel->setdT(dt);
      _channels.push_back(channel);
      i=-1;
    }
    i++;
  }
}
					     
vector<DbChannel*> CalibrationFile::getChannels(){
  
  return _channels;
}

DbStation *CalibrationFile::getStation(){
  
  return _station;
}

void CalibrationFile::setStation(DbStation *station){

  if(_station) delete _station;
  _station = 0;
  _station = station;
}
