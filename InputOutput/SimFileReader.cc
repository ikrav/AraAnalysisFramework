#include "SimFileReader.h"

SimFileReader::SimFileReader(std::string filenames) : FileReader(filenames) {
 
  initialize();
  
}

SimFileReader::~SimFileReader(){
 
  clearEvent();
  
}

void SimFileReader::initialize(){

  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  _detector=0;
  _settings=0;
  _report=0;
  _event=0;
  _station_number=0;
  _evtTreeToAraTree2Translation.clear();

}

void SimFileReader::printout(){

  FileReader::printout();
  
}

bool SimFileReader::setChains(){
          
  _a2cnt = -1;
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  FileReader::setChains();
  
  _sim_chain1=new TChain("AraTree");
  
    if(!_sim_chain1->Add(_file_names.c_str())){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: for _sim_chain1, incorrect file name(s): "<<_file_names<<std::endl;
    return false; // can't go on without detector class right?!
    
  }
  
  if(_sim_chain1->SetBranchAddress("settings", &_settings)==TTree::kMatch){
   
    if(debug_bit) std::cout<<"success to SetBranchAddress for '_settings'..."<<std::endl;
    
  }
  
  if(_sim_chain1->SetBranchAddress("detector", &_detector)==TTree::kMatch){
   
    if(debug_bit) std::cout<<"success to SetBranchAddress for '_detector'..."<<std::endl;
    
  }
  
  _sim_chain2=new TChain("AraTree2");

  if(!_sim_chain2->Add(_file_names.c_str())){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: for _sim_chain2, incorrect file name(s): "<<_file_names<<std::endl;
    return false;
    
  }
  
  return true;
  
}

bool SimFileReader::setBranches(){
          
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(!setChains()) return false;// try to use the filenames to add to the chain for AraTree and AraTree2
  
  // can add another if for FileReader::setBranches to try if it is actually not simulation... not sure if useful though...
  
  if(_chain->GetListOfBranches()->FindObject("UsefulARAStationEvent")){
    
    _chain->SetBranchAddress("UsefulARAStationEvent", &_real_icrr_ev);
    
    if(debug_bit) std::cout<<"setting up the branch based on 'eventTree' and 'UsefulARAStationEvent' - Icrr style..."<<std::endl;
    _total_events=_chain->GetEntries();
    _is_icrr=1;

    
  }
  else if(_chain->GetListOfBranches()->FindObject("UsefulAtriStationEvent")){
    
    _chain->SetBranchAddress("UsefulAtriStationEvent", &_real_atri_ev);
  
    if(debug_bit) std::cout<<"setting up the branch based on 'eventTree' and 'UsefulAtriStationEvent' - Atri style..."<<std::endl;
    _total_events=_chain->GetEntries();
    _is_atri=1;
    
  }
  else if(_sim_chain2->GetListOfBranches()->FindObject("report")){
    
//     we will set the branch address for report and event below
    _sim_chain2->SetBranchAddress("report", &_report); 
    if(debug_bit) std::cout<<"setting up the branch based on 'AraTree2' and 'Reprt'..."<<std::endl;
    for(int i=0; i<_sim_chain2->GetEntries(); i++) {
      _sim_chain2->GetEntry(i);
      if(_report->stations[_station_number].Global_Pass>0) _evtTreeToAraTree2Translation.push_back(i);
    }
    
  }
  else{
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot setup any branch from these files!"<<std::endl;
    return false;
    
  }
  
  if(_sim_chain2->GetListOfBranches()->FindObject("report")){
    _sim_chain2->SetBranchAddress("report", &_report);
    for(int i=0; i<_sim_chain2->GetEntries(); i++) {
      _sim_chain2->GetEntry(i);
      if(_report->stations[_station_number].Global_Pass>0) _evtTreeToAraTree2Translation.push_back(i);
    }
  }
  
  if(_sim_chain2->GetListOfBranches()->FindObject("event")){
    _sim_chain2->SetBranchAddress("event", &_event);
  }
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<"_real_icrr_ev= "<<_real_icrr_ev<<" | _real_atri_ev= "<<_real_atri_ev<<" | _report= "<<_report<<std::endl;
  
  setupGeometry();
  _branches_set=1;
  return true;
  
}

void SimFileReader::setupGeometry(){
    
  _sim_chain1->GetEntry(0);
  
  if(!_detector){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: _detector= "<<_detector<<std::endl;
    return;
  
  }
  
  _station_number=0; // is there going to be a field in AraSim to get this from?
  //int stationID = _detector->stations[_station_number].StationID;
  _station_ID = _settings->DETECTOR_STATION;
  _exponent = _settings->EXPONENT;
  _cylinder_radius = _settings->POSNU_RADIUS;
 
  double origin[3];  //to compute reference frame
  
  double station_pos[3];
  station_pos[0]=_detector->stations[_station_number].GetX();
  station_pos[1]=_detector->stations[_station_number].GetY();
  station_pos[2]=_detector->stations[_station_number].GetZ();
  
  //for each string
  int string, ant;
  
  _num_chans=_detector->stations[_station_number].number_of_antennas;
  
  vector<AntPos> positions;
    
  for(int ch=0; ch<_num_chans; ch++){
    
    _detector->GetSSAfromChannel(_station_ID, ch, &ant, &string, _settings);

    //get the position
    
    double locationXYZ[3];
    
    locationXYZ[0] = _detector->stations[_station_number].strings[string].antennas[ant].GetX() - station_pos[0];
    locationXYZ[1] = _detector->stations[_station_number].strings[string].antennas[ant].GetY() - station_pos[1];
    locationXYZ[2] = _detector->stations[_station_number].strings[string].antennas[ant].GetZ() - station_pos[2];
      
    origin[0] += locationXYZ[0];
    origin[1] += locationXYZ[1];
    origin[2] += locationXYZ[2];

    positions.push_back(AntPos(locationXYZ[0], locationXYZ[1], locationXYZ[2], _detector->stations[_station_number].strings[string].antennas[ant].type));
    
  }//for ch
  
  //reference frame
  origin[0] /= _num_chans;
  origin[1] /= _num_chans;
  origin[2] /= _num_chans;
  
  _geom=new StationGeometry(positions, origin[0], origin[1], origin[2]);
    
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<" geometry set. _num_chans= "<<_num_chans<<" origin= "<<origin[0]<<" "<<origin[1]<<" "<<origin[2]<<std::endl;
  
  Channel::setGeometry(_geom);
  
}

bool SimFileReader::loadEvent(int eventNumber){
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(_branches_set==0) setBranches(); // lazy load the branches!
  
  if(eventNumber>=_total_events){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: event number given ("<<eventNumber<<") exceeds maximum number of events ("<<_total_events<<")"<<std::endl;
    return false;
    
  }
    
  clearEvent();
    
  _chain->GetEntry(eventNumber);
  int passcnt = -1;
  int aratree2evt = 0;
  bool enditall = false;

  _sim_chain2->GetEntry(_evtTreeToAraTree2Translation[eventNumber]);

  if(!isFromReport()) loadChannels(); // call the base class reader function to getGraphFromRFChan
  else loadChannelsReport(); // use the waveforms from Report class...

  if(_event){
    
    double station_pos[3];
    station_pos[0]=_detector->stations[_station_number].GetX();
    station_pos[1]=_detector->stations[_station_number].GetY();
    station_pos[2]=_detector->stations[_station_number].GetZ();
  
    double posnu[3];
  
    posnu[0]=_event->Nu_Interaction[0].posnu.GetX();
    posnu[1]=_event->Nu_Interaction[0].posnu.GetY();
    posnu[2]=_event->Nu_Interaction[0].posnu.GetZ();
  
    _weight = _event->Nu_Interaction[0].weight;
    _flavor = _event->nuflavorint;
    _nu_nubar = _event->nu_nubar;
    _current = _event->Nu_Interaction[0].currentint;
    _cross_sec = _event->Nu_Interaction[0].sigma;

    for(int a=0;a<3;a++) posnu[a]=posnu[a]-station_pos[a];
    _real_pos.setCartesian(posnu[0], posnu[1], posnu[2]);
      
  }

  return true;

}

void SimFileReader::loadChannelsReport(){
       
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;

  clearChannels();
        
  _channels.reserve(_num_chans);
  
  // complete this! 
  
}

void SimFileReader::clearEvent(){
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  FileReader::clearEvent();
  
}

// getters
bool SimFileReader::isCalPulser() const {
 
  return false;
  
}

bool SimFileReader::isSimulation() const { 
  
  return true;
  
}

int SimFileReader::isFromReport() const {
 
  if(_real_atri_ev || _real_icrr_ev) return 0;
  if(_report) return 1;
  
  return 0; // default if all is empty...
  
}

int SimFileReader::getNumberEventsAraTree2() {

  return _sim_chain2->GetEntries();

}

Pos SimFileReader::getRealPosition(){
    
  return _real_pos;
    
}

double SimFileReader::getReceivingAngle(int channel, int solution) {
  if(!_report) {std::cout<<"Report points to nothing, can't get receiving angle"<<std::endl; return 999;}
  if(!_detector) {std::cout<<"Detector points to nothing, can't get receiving angle"<<std::endl; return 999;}
  if(!_settings) {std::cout<<"Settings points to nothing, can't get receiving angle"<<std::endl; return 999;}
  int ant, string;
  _detector->GetSSAfromChannel(_station_ID, channel, &ant, &string, _settings);
  int recsize = _report->stations[_station_number].strings[string].antennas[ant].rec_ang.size();
  if(solution>=recsize) {
    std::cout << "Can't get receiving angle for the "<<solution<<" solution, receiving angle vector has "<<recsize<<" solutions"<<std::endl;
    return 999;
  }
  return _report->stations[_station_number].strings[string].antennas[ant].rec_ang[solution];
}

int SimFileReader::getReceivingAngleVectorSize(int channel) {
  if(!_report) {std::cout<<"Report points to nothing, can't get receiving angle vector size"<<std::endl; return 999;}
  if(!_detector) {std::cout<<"Detector points to nothing, can't get receiving angle vector size"<<std::endl; return 999;}
  if(!_settings) {std::cout<<"Settings points to nothing, can't get receiving angle vector size"<<std::endl; return 999;}
  int ant, string;
  _detector->GetSSAfromChannel(_station_ID, channel, &ant, &string, _settings);
  return _report->stations[_station_number].strings[string].antennas[ant].rec_ang.size();
}

Pos SimFileReader::getSourcePosition(){
  
  return getRealPosition();
  
}

SimData *SimFileReader::getSimData(){
 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this hasn't been implemented yet..."<<std::endl;
  
  return 0;
  
}

int SimFileReader::debug_bit=0;
