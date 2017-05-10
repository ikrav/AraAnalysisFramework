#include "L0FileReader.h"

ClassImp(L0FileReader);

L0FileReader::L0FileReader(std::string inputFile, std::string vrmsFile, int useSurfaceAnt) {

  _file_name = inputFile;
  _vrms_file = vrmsFile;
  _use_surface_ants = useSurfaceAnt;
  initialize();

}

L0FileReader::~L0FileReader(){
  _file_in->Close();
  if(_is_icrr) delete _raw_icrr_ev; _raw_icrr_ev=0;
  if(_is_atri) delete _raw_atri_ev; _raw_atri_ev=0;
  if(_geom) delete _geom; _geom =0;
  if(_station_info) delete _station_info; _station_info=0;
  getReadyForNextEvent(); //to clear vectors
}

void L0FileReader::initialize(){

    
  open(); // to open ROOT file
  _VRMS.clear();
  _raw_ev = 0;
  _raw_icrr_ev =0;
  _raw_atri_ev =0;
  //  _l2_data = 0;
  _event_tree->SetBranchAddress("event", &_raw_ev);
  _event_tree->GetEntry(0);
  _station_number = _raw_ev->stationId;

  _station_info = new AraStationInfo(_station_number);

  if(!_use_surface_ants){ // don't include surface antennas
    _station_info->setNumRFChans(_station_info->getNumAntennasByPol(AraAntPol::kVertical)+_station_info->getNumAntennasByPol(AraAntPol::kHorizontal));
  }
    
  _geom = new StationGeometry(_station_info->getNumRFChans());

  _event_tree->ResetBranchAddresses();
  delete _raw_ev;

  if(_station_number == 0){
    _is_icrr = 1;
    _is_atri = 0;
    _event_tree->SetBranchAddress("event", &_raw_icrr_ev);
    _file_in->cd();
  }else{
    _is_atri =1;
    _is_icrr =0;
    _event_tree->SetBranchAddress("event", &_raw_atri_ev);
  }
 
  _total_events=_event_tree->GetEntries();
  _current_event_num=-1;
  setDefaultInterpolationFactor();
  

  if(_vrms_file[0] != ""[0]){
    loadVRMS();
  }else {
    //fill VRMS with 50.0 -- default VRMS value
    for(int i=0; i<_station_info->getNumRFChans(); i++){
      _VRMS.push_back(50.0);
    }
  }
  createGeometry();
}

void L0FileReader::open(){
  _file_in = TFile::Open(_file_name.c_str());
  _file_in->cd();
  _event_tree = (TTree*)gDirectory->Get("eventTree");
  if(_event_tree == NULL) {
    std::cerr <<"No _event_tree found!!!! "<<std::endl;
    _total_events =0;
  }
}

int L0FileReader::getTotalNumEvents() const {
  return _total_events;
}

double L0FileReader::getInterpolationFactor(){
  return _interpolation_factor;
}

int L0FileReader::getCurrentEventNumber() const {
  return _current_event_num;
}

int L0FileReader::getAraCurrentEventNumber(){
  return _ara_event_number;
}

int L0FileReader::getRunNumber(){
  return _run_number;
}

int L0FileReader::getStationId(){
  return _station_number;
}

StationGeometry *L0FileReader::getStationGeometry(){
  return _geom;
}

//L2Data *L0FileReader::getL2Data() {
//  return _l2_data;
//}

TTree* L0FileReader::getTree(){
  return _event_tree;
}

UsefulAraStationEvent *L0FileReader::getUsefulEvent(){
  if(_is_icrr) return _real_icrr_ev;
  else return _real_atri_ev;
}
  
int L0FileReader::isCalPulser(){
  return _is_cal_pulser;
}

bool L0FileReader::isAtriEvent(){
  return _is_atri;
}

bool L0FileReader::isIcrrEvent(){
  return _is_icrr;
}

ChannelCollection L0FileReader::getChannelCollection(){

  _channel_collection = ChannelCollection(_channels);
  //  return (ChannelCollection(_channels));
  return _channel_collection;
}

void L0FileReader::setInterpolationFactor(double factor){  
  _interpolation_factor = factor;
}

void L0FileReader::setDefaultInterpolationFactor(){
  setInterpolationFactor(0.5);
}

bool L0FileReader::next(){

  std::cerr<<"NOT implemented yet!!!"<< std::endl;
  return false;
}

bool L0FileReader::getNextEvent(){
  if((_total_events - _current_event_num) > 1) {
    getReadyForNextEvent();
    _current_event_num++;
    lookAtEvent(_current_event_num);
  }else{

    return false;
  }

  return true;
}

int L0FileReader::loadEvent(int event_number){
 
  if(event_number>0 && event_number<getTotalNumEvents()) _current_event_num=event_number;
  else _current_event_num++;
  getReadyForNextEvent();
  lookAtEvent(_current_event_num);
  
  return _current_event_num;
  
}

bool L0FileReader::getSpecificEvent(int eventNumber){

  if(_total_events > eventNumber){
    getReadyForNextEvent();
    lookAtEvent(eventNumber);
    return true;
  }else {
    return false;
  }

  return true;
}

void L0FileReader::getReadyForNextEvent(){

  //clear channels
  for(int i=0; i< _channels.size(); i++){
    if(_channels[i]) {
      delete _channels[i];
    }
    
    _channels[i] = 0;
  }

  _channels.clear();
  _channel_collection.clear();

  if(_real_icrr_ev) delete _real_icrr_ev; _real_icrr_ev=0;
  if(_real_atri_ev) delete _real_atri_ev; _real_atri_ev=0;
}

void L0FileReader::lookAtEvent(int eventNumber){

  _channels.reserve(_station_info->getNumRFChans());
  _event_tree->GetEntry(eventNumber);
  int isPulser=0;
  if(_is_icrr){
    _real_icrr_ev =0;
    _real_icrr_ev = new UsefulIcrrStationEvent(_raw_icrr_ev, AraCalType::kLatestCalib);
    isPulser = _real_icrr_ev->isCalPulserEvent();
    _ara_event_number = _real_icrr_ev->head.eventNumber;
    //    _l2_data = new L2Data(_real_icrr_ev);
  }else if(_is_atri){
    _real_atri_ev = new UsefulAtriStationEvent(_raw_atri_ev, AraCalType::kLatestCalib);
    //    isPulser = _real_icrr_ev->isCalPulserEvent();
    //    _l2_data = new L2Data(_real_atri_ev);
  }
 
  _is_cal_pulser = isPulser;
 
  for(int ch = 0; ch < _station_info->getNumRFChans(); ch++){
    TGraph *gVt;
    if(_is_icrr){
      gVt = _real_icrr_ev->getGraphFromRFChan(ch);
    }else if(_is_atri){
      gVt = _real_atri_ev->getGraphFromRFChan(ch);
    }
    TGraph *gVt0 = FFTtools::getInterpolatedGraph(gVt, _interpolation_factor);
    
    _channels.push_back(new Channel(ch, gVt0));
    _channels.back()->setGeometry(_geom);
    
    delete gVt;
    delete gVt0;
  }//end of loop over channels


}

void L0FileReader::printout(){
  
  printout(1);
  
}

void L0FileReader::printout(int mode){
  printf(" %s \n \t Current event: %d / %d , \t isCalPulser: %d \n", __PRETTY_FUNCTION__,  _current_event_num, _total_events, _is_cal_pulser);
  if(mode != 1){ // default mode 1 for print current event stuff
    _geom->printout();
    std::cout <<"Misc info...."<<std::endl;
    printf("\t File name: %s \n \t Run number: %d \n \t StationID: %d \n \t Interpolation factor: %.2f \n \t isIcrr %d isAtri %d \n \n", _file_name.c_str(), _run_number, _station_number, _interpolation_factor, _is_icrr, _is_atri);
  }
}

void L0FileReader::createGeometry(){
  std::cout <<__PRETTY_FUNCTION__<< "size " << _VRMS.size() << std::endl;
  //Create geometry with the station number and make a stationGeometry object 
  double origin[3]; // for reference frame
  std::vector<AntPos> positions;
  for(int i=0; i<_station_info->getNumRFChans(); i++){
    double *locationXYZ = _station_info->getAntennaInfo(i)->getLocationXYZ();

    origin[0] += locationXYZ[0];
    origin[1] += locationXYZ[1];
    origin[2] += locationXYZ[2];
    
    positions.push_back(AntPos(locationXYZ[0], locationXYZ[1], locationXYZ[2], _station_info->getAntennaInfo(i)->polType, _VRMS[i]));

  }   
  //use all antennas for reference frame
  origin[0] /=_station_info->getNumRFChans();
  origin[1] /=_station_info->getNumRFChans();
  origin[2] /=_station_info->getNumRFChans();
  _geom->setOrigin(origin);
  _geom->setChannels(positions);

}

void L0FileReader::loadVRMS(){

  ifstream VRMSfile;
  VRMSfile.open(_vrms_file.c_str());
  double value;
  int chan = 0;
  int maxChan = _station_info->getNumRFChans();
  while(!VRMSfile.eof()){
    
    if(VRMSfile.eof() || !VRMSfile.good()) break;
    if(chan >= maxChan) break;
    VRMSfile >> value;
    _VRMS.push_back(value);
    chan++;
  }
  VRMSfile.close();
}
