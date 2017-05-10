#include "AraSimFileReader.h"

ClassImp(AraSimFileReader);

AraSimFileReader::AraSimFileReader(std::vector<std::string> files, int stationId){

  _station_number = stationId;
  _file_names = files;
  open();
  initialize();
  initializeGeometry();
  createGeometry();

}

AraSimFileReader::AraSimFileReader(std::string files, int stationId){
 
  _station_number = stationId;
  _file_names = std::vector<std::string>();
  _file_names.push_back(files);
  open();
  initialize();
  initializeGeometry();
  createGeometry();
  
}

AraSimFileReader::~AraSimFileReader(){

  if(_detector) delete _detector; _detector=0;
  if(_report) delete _report; _report=0;
  if(_settings) delete _settings; _settings=0;
  if(_geom) delete _geom; _geom=0;
  if(_event_tree) delete _event_tree; _event_tree = 0;
  if(_ara_tree) delete _ara_tree; _ara_tree = 0;
  if(_ara_tree_2) delete _ara_tree_2; _ara_tree_2 = 0;
  if(_sim_data) delete _sim_data; _sim_data =0;
}

void AraSimFileReader::open(){  

  _ara_tree_2 = new TChain("AraTree2");
  _ara_tree = new TChain("AraTree");
  _event_tree = new TChain("eventTree");

  for(int i=0; i<_file_names.size(); i++){
    _ara_tree->Add(_file_names[i].c_str());
    _ara_tree_2->Add(_file_names[i].c_str());
    _event_tree->Add(_file_names[i].c_str());
  }

  if(_event_tree == NULL) cerr <<"No eventTree found!!! "<< endl;
  if(_ara_tree == NULL) cerr <<"No AraTree found!!!" << endl;
  if(_ara_tree_2 == NULL) cerr <<"No AraTree2 found!!!"<< endl;
}


void AraSimFileReader::initialize(){

  _sim_data = 0;
  _ara_tree->SetBranchAddress("settings", &_settings);
  _ara_tree->SetBranchAddress("detector", &_detector);
  _ara_tree->SetBranchAddress("spectra", &_spectra);
  _ara_tree->SetBranchAddress("icemodel", &_ice_model);
  _ara_tree->SetBranchAddress("trigger", &_trigger);
   
  _ara_tree_2->SetBranchAddress("report", &_report);
  _ara_tree_2->SetBranchAddress("event", &_event);
  
  _total_events = _ara_tree_2->GetEntries();
  _current_event_num = -1;
  _ara_tree->GetEntry(0);
}

void AraSimFileReader::initializeGeometry(){
  _geom = new StationGeometry(_detector->stations[_station_number].number_of_antennas);
}

int AraSimFileReader::getTotalNumEvents() const {
  return _total_events;
}

double AraSimFileReader::getInterpolationFactor(){
  return _interpolation_factor;
}

int AraSimFileReader::getCurrentEventNumber() const{
  return _current_event_num;
}

int AraSimFileReader::getStationId(){
  return _station_number;
}

StationGeometry* AraSimFileReader::getStationGeometry(){
  return _geom;
}

bool AraSimFileReader::getNextEvent(){

  if((_total_events - _current_event_num) > 1){
    getReadyForNextEvent();
    _current_event_num++;
    lookAtEvent(_current_event_num);
  }else{
    return false;
  }

  return true;

}

bool AraSimFileReader::next(){

  std::cerr<<"NOT implemented yet!!!"<< std::endl;
  return false;
}

int AraSimFileReader::loadEvent(int event_number){
 
  if(event_number>-1 && event_number<getTotalNumEvents()) _current_event_num=event_number;
    
  getReadyForNextEvent();
  lookAtEvent(_current_event_num);
  
  _current_event_num++;
  
  return _current_event_num;
  
}

SimData *AraSimFileReader::getSimData(){
  
  return _sim_data;
}

TChain *AraSimFileReader::getEventTree(){
  return _event_tree;
}

TChain *AraSimFileReader::getAraTree(){
  return _ara_tree;
}

TChain *AraSimFileReader::getAraTree2(){
  return _ara_tree_2;
}

ChannelCollection AraSimFileReader::getChannelCollection(){

  return (ChannelCollection(_channels));
}

bool AraSimFileReader::getSpecificEvent(int eventNumber){

  if(_total_events > eventNumber){
    getReadyForNextEvent();
    lookAtEvent(eventNumber);
    return true;
  }else{
    return false;
  }

  return true;
}

int AraSimFileReader::getNumberOfStations(){
  return _detector->params.number_of_stations;
}

int AraSimFileReader::getNumberOfAntennasPerStation(int stationId){
  return _detector->stations[stationId].number_of_antennas;
}

void AraSimFileReader::setInterpolationFactor(double factor){
  _interpolation_factor = factor;
}

void AraSimFileReader::setDefaultInterpolationFactor(){
  setInterpolationFactor(0.5);
}

void AraSimFileReader::getReadyForNextEvent(){

  for(int i=0; i< _channels.size(); i++){
    if(_channels[i]) delete _channels[i];
    _channels[i] = 0;
  }
  _channels.clear();
  if(_sim_data) delete _sim_data;
  _sim_data = 0;
}

void AraSimFileReader::lookAtEvent(int eventNumber){

  _ara_tree_2->GetEntry(eventNumber);
  cout <<"Event Number "<< eventNumber << endl;
  while(!_report->stations[_station_number].Global_Pass){
    _current_event_num++;
    _ara_tree_2->GetEntry(_current_event_num);
    // what happens if the last events don't trigger, and _current_event_num goes over the _max_num_events ??
  }

  int string, ant;
  for(int iChan=0; iChan<_detector->stations[_station_number].number_of_antennas; iChan++){

    _detector->GetSSAfromChannel(0, iChan+1, &ant, &string, _settings);
    double getx[_settings->NFOUR/2];
    double gety[_settings->NFOUR/2];

    for(int iBin=0; iBin<_settings->NFOUR/2; iBin++){
      getx[iBin] = _report->stations[_station_number].strings[string].antennas[ant].time_mimic[iBin];
      gety[iBin] = _report->stations[_station_number].strings[string].antennas[ant].V_mimic[iBin];
    }
    TGraph *gr = new TGraph(_settings->NFOUR/2, getx, gety);    
    _channels.push_back(new Channel(iChan, gr));
    _channels.back()->setRealTime(_report->stations[_station_number].strings[string].antennas[ant].arrival_time[0]);

    _sim_data = new SimData();

    /***
	THESE STILL NEED TO BE UPDATED WITH THE NEW VERSION OF ARASIM

    double x = _detector->stations[_station_number].strings[string].antennas[ant].posnu.GetX();
    double y = _detector->stations[_station_number].strings[string].antennas[ant].posnu.GetY();
    double z = _detector->stations[_station_number].strings[string].antennas[ant].posnu.GetZ();
    _sim_data->setCartesian(x,y,z);

    _sim_data->setEnergy();
    _sim_data->setFieldAtSource();
    _sim_data->setPolarizationRatio();
    _sim_data->setDirTheta();
    _sim_data->setNuFlavor();
    ***/
    delete gr;
  }//for chans
  _channels.back()->setGeometry(_geom);

}

void AraSimFileReader::printout(){
  
  printout(1);
  
}

void AraSimFileReader::printout(int mode){

  if(mode != 100){
    //    _geom->printout();
    cout <<"Misc info.... "<< endl;
    printf("Current event: %d / %d \n \t StationID: %d \n \t Interpolation factor: %.2f \n \n",_current_event_num, _total_events, _station_number, _interpolation_factor);

  }
  printf("------------------------------------------------------------------------------\n");

}

void AraSimFileReader::createGeometry(){

  double origin[3];  //to compute reference frame
  //for each string
  int string, ant;
  vector<AntPos> positions;
  for(int iChan=0; iChan<_detector->stations[_station_number].number_of_antennas; iChan++){
    _detector->GetSSAfromChannel(0, iChan+1, &ant, &string, _settings);
    //get the position
    double locationXYZ[3];
    locationXYZ[0] = _detector->stations[_station_number].strings[string].antennas[ant].GetX();
    locationXYZ[1] = _detector->stations[_station_number].strings[string].antennas[ant].GetY();
    locationXYZ[2] = _detector->stations[_station_number].strings[string].antennas[ant].GetZ();
    origin[0] += locationXYZ[0];
    origin[1] += locationXYZ[1];
    origin[2] += locationXYZ[2];

    positions.push_back(AntPos(locationXYZ[0], locationXYZ[1], locationXYZ[2], _detector->stations[_station_number].strings[string].antennas[ant].type));
  }//for chans
  
  //reference frame
  origin[0] /= _detector->stations[_station_number].number_of_antennas;
  origin[1] /= _detector->stations[_station_number].number_of_antennas;
  origin[2] /= _detector->stations[_station_number].number_of_antennas;
  _geom->setOrigin(origin);
  _geom->setChannels(positions);
}
