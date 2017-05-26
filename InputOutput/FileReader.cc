#include "FileReader.h"

ClassImp(FileReader);

FileReader::FileReader(std::string filenames, std::string vrmsFile, int useSurfaceAnt) {

  _file_names = filenames;
  _vrms_file = vrmsFile;
  _use_surface_ants = useSurfaceAnt;
  initialize();

}

FileReader::~FileReader(){
  
  if(_geom) delete _geom; _geom =0;
  if(_station_info) delete _station_info; _station_info=0;
  
  clearChannels(); //to clear channels and events
  
  _VRMS.clear();
  
}

void FileReader::initialize(){
   
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;

  // initialize all pointers!
  _raw_ev=0;
  _raw_icrr_ev=0;
  _raw_atri_ev=0;
  _real_atri_ev=0;  
  _real_icrr_ev=0;
  
  clearEvent();
  
  // initialize other parameters
  _num_chans=0;
  
  _total_events=0;
  _current_event_num=0;
  _interpolation_factor=1;
  _station_number=-1;
  
  _is_atri=0;
  _is_icrr=0;
  _use_surface_ants=0;
        
  _branches_set=0;

  _station_info=0;
  _geom=0;
  
}

bool FileReader::setChains(){
     
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;

  // load the eventTree chain for data and also for simulations
  _chain=new TChain("eventTree");
  if(!_chain->Add(_file_names.c_str())){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: incorrect file name(s): "<<_file_names<<std::endl;
    return false;
    
  }
  
  return true;
  
}

bool FileReader::setBranches(){

  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(!setChains()) return false;
  
  _total_events=_chain->GetEntries();
  
  // get RAW event class just for basic info
  if(_chain->GetListOfBranches()->FindObject("event")){
    
    _chain->SetBranchAddress("event", &_raw_ev);
    
  }
  else{
  
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot locate branch 'event' in chain"<<std::endl;
    return false;
    
  }
    
  _chain->GetEntry(0);
  _station_number = _raw_ev->stationId;

  setupGeometry(); // generate both AraStationInfo and StationGeometry objects
  
  // reset this so we can point to the more specific event type
  _chain->ResetBranchAddresses();
  delete _raw_ev;
  _raw_ev=0;

  if(_station_number==0){
    
    if(_chain->SetBranchAddress("event", &_raw_icrr_ev)!=TTree::kMatch){
    
      std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot locate branch 'event' in chain"<<std::endl;
      return false;
    
    }
   
    _is_icrr = 1;
    _is_atri = 0;
    
  }
  
  else{
    
    _is_atri =1;
    _is_icrr =0;
    if(_chain->SetBranchAddress("event", &_raw_atri_ev)!=TTree::kMatch){
      
      std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot locate branch 'event' in chain"<<std::endl;
      return false;
    
    }
      
    _is_icrr = 1;
    _is_atri = 0;
    
  }
 
  setDefaultInterpolationFactor();

  _branches_set=1;
  return true;
  
}

void FileReader::setupGeometry(){
    
  if(_station_number<0){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: wrong station number "<<_station_number<<std::endl;
    return;
    
  }
  
  _station_info = new AraStationInfo(_station_number);

  if(!_use_surface_ants){ // don't include surface antennas
    _station_info->setNumRFChans(_station_info->getNumAntennasByPol(AraAntPol::kVertical)+_station_info->getNumAntennasByPol(AraAntPol::kHorizontal));
    _num_chans=_station_info->getNumRFChans();
  }
  
  _VRMS.clear();
  if(_vrms_file[0] != ""[0]){
    loadVRMS();
  }
  else{
    //fill VRMS with 50.0 -- default VRMS value
    for(int i=0; i<_num_chans; i++){
      _VRMS.push_back(50.0);
    }
  }
  
  
  
  //Create geometry with the station number and make a stationGeometry object 
  double origin[3]; // for reference frame
  std::vector<AntPos> positions;
  for(int i=0; i<_num_chans; i++){
    double *locationXYZ = _station_info->getAntennaInfo(i)->getLocationXYZ();

    origin[0] += locationXYZ[0];
    origin[1] += locationXYZ[1];
    origin[2] += locationXYZ[2];
    
    positions.push_back(AntPos(locationXYZ[0], locationXYZ[1], locationXYZ[2], _station_info->getAntennaInfo(i)->polType, _VRMS[i]));

  }
 
  //use all antennas for reference frame
  origin[0] /=_num_chans;
  origin[1] /=_num_chans;
  origin[2] /=_num_chans;

  _geom = new StationGeometry(positions, origin[0], origin[1], origin[2]); 
  
  Pos::setOriginStatic(origin[0], origin[1], origin[2]);
  
  // add the pulser information too
  std::vector<AntPos> cal_positions;
  
  for(int i=0;i<_station_info->getNumCalAnts();i++){
  
    double *pulserXYZ= _station_info->getCalAntennaInfo(i)->getLocationXYZ();
   
    cal_positions.push_back(AntPos(pulserXYZ[0], pulserXYZ[1], pulserXYZ[2], _station_info->getCalAntennaInfo(i)->polType,0));
    
  }
  
  _geom->setPulsers(cal_positions);
    

  Channel::setGeometry(_geom);
  
}

void FileReader::clearEvent(){

  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  clearChannels();
    
//   if(_raw_ev){ delete _raw_ev; _raw_ev=0; }
//   if(_raw_icrr_ev){ delete _raw_icrr_ev; _raw_icrr_ev=0; }
//   if(_raw_atri_ev){ delete _raw_atri_ev; _raw_atri_ev=0; }
  
  if(_real_icrr_ev){ delete _real_icrr_ev; _real_icrr_ev=0; }
  if(_real_atri_ev){ delete _real_atri_ev; _real_atri_ev=0; }
  
}

void FileReader::clearChannels(){
  
    for(int i=0; i<_channels.size(); i++){
    
    if(_channels[i]){ delete _channels[i]; }// _channels[i]=0; }
    
  }
  
  _channels.clear();
  
}

void FileReader::printout(){
  
  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  std::cout<<"filenames: "<<getFileNames()<<std::endl;
  
  std::cout<<"Event number: "<<getCurrentEventNumber()<<" out of "<<getTotalNumEvents()<<std::endl;
  
  // need to expand this a little...
  
}

bool FileReader::loadNextEvent(){
    
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;

  if(_current_event_num >= _total_events) { printf("Fail 1: %i \t %i\n",_current_event_num,_total_events); return false; }
  else{
    
    if(loadEvent(_current_event_num)) _current_event_num++;
    else {printf("Fail 2\n"); return false;} // if we fail to load event! 
    
  }
  
  return true;
  
}

bool FileReader::loadEvent(int eventNumber){

  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(_branches_set==0) setBranches(); // lazy load the branches!

  if(eventNumber>=_total_events){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: event number given ("<<eventNumber<<") exceeds maximum number of events ("<<_total_events<<")"<<std::endl;
    return false;
    
  }
  
  clearEvent();
      
  _chain->GetEntry(eventNumber);
  
  int isPulser=0; // we need to find something smart to do with this result... 
  
  if(_is_icrr){
    
    _real_icrr_ev = new UsefulIcrrStationEvent(_raw_icrr_ev, AraCalType::kLatestCalib);
    
    isPulser = _real_icrr_ev->isCalPulserEvent();
    
    
  }
  else if(_is_atri){
    
    _real_atri_ev = new UsefulAtriStationEvent(_raw_atri_ev, AraCalType::kLatestCalib);
    // how do we know it is calpulser?!
    
  }
  
  loadChannels();
   
  return true;

}

void FileReader::loadChannels(){
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  clearChannels();
        
  _channels.reserve(_num_chans);
  
  for(int ch = 0; ch < _num_chans; ch++){
    
    TGraph *gVt;
    if(_is_icrr) gVt = _real_icrr_ev->getGraphFromRFChan(ch);
    else if(_is_atri) gVt = _real_atri_ev->getGraphFromRFChan(ch);
    
    TGraph *gVt0 = FFTtools::getInterpolatedGraph(gVt, _interpolation_factor);
    
//     std::cout<<"ch= "<<ch<<" _real_icrr= "<<_real_icrr_ev<<" gVt= "<<gVt<<" gVt0= "<<gVt0<<std::endl;
    
    _channels.push_back(new Channel(ch, gVt0));
    
    delete gVt;
    delete gVt0;
    
  }//end of loop over channels
  
}

bool FileReader::loadVRMS(){

  ifstream VRMSfile;
  VRMSfile.open(_vrms_file.c_str());
  
  if(!VRMSfile.is_open()){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot open VRMS file: "<<_vrms_file<<std::endl;
    
    return false;
    
  }
  
  double value;
  
  int chan = 0;
  
  while(!VRMSfile.eof()){
    
    if(VRMSfile.eof() || !VRMSfile.good()) break;
    
    if(chan >= _num_chans) break;
  
    VRMSfile >> value;
    _VRMS.push_back(value);
  
    chan++;
    
  }
  
  VRMSfile.close();
  
  return true;
  
}

// getters
std::string FileReader::getFileNames() const {
  
  return _file_names;
  
}

TChain *FileReader::getChain() {
  
  return _chain;
  
}

UsefulAraStationEvent *FileReader::getUsefulEvent(){
  
  if(_is_icrr) return _real_icrr_ev;
  else return _real_atri_ev;
  
}

int FileReader::getTotalNumEvents(){
  
  if(_branches_set==0) setBranches(); // lazy load the branches
  
  return _total_events;
  
}

int FileReader::getCurrentEventNumber() const {
  
  return _current_event_num;
  
}

int FileReader::getAraCurrentEventNumber(){
  
  if(_branches_set==0) setBranches(); // lazy load the branches

  if(_is_icrr) return _real_icrr_ev->head.eventNumber;
//   if(_is_atri) return _real_atri_ev->head.eventNumber; // check this....

  return -1;
  
}

int FileReader::getRunNumber(){
    
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this is not yet implemented"<<std::endl;
  
  return -1;
  
  if(_branches_set==0) setBranches(); // lazy load the branches
  
}

int FileReader::getStationId(){
  
  if(_branches_set==0) setBranches(); // lazy load the branches

  return _station_number;
  
}

bool FileReader::isAtriEvent(){
    
  if(_branches_set==0) setBranches(); // lazy load the branches
  
  return _is_atri;
  
}

bool FileReader::isIcrrEvent(){
  
  if(_branches_set==0) setBranches(); // lazy load the branches

  return _is_icrr;
  
}

bool FileReader::isCalPulser(){
  
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet!"<<std::endl;
  
  return false;
  
  if(_branches_set==0) setBranches(); // lazy load the branches

}

bool FileReader::isSimulation() const {
  
  return false;
  
}

double FileReader::getInterpolationFactor() const {
  
  return _interpolation_factor;
  
}

bool FileReader::isUsingSurfaceAnts() const {
  
  return _use_surface_ants;
  
}

StationGeometry *FileReader::getStationGeometry(){
  
  if(_branches_set==0) setBranches(); // lazy load the branches

  return _geom;
  
}

ChannelCollection FileReader::getChannelCollection() const {
  
  return _channels;
  
}

// setters
void FileReader::setMaxNumEvents(int number_events){
 
  if(number_events>=getChain()->GetEntries()){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: cannot set max of "<<number_events<<" events, file(s) contain only "<<getChain()->GetEntries()<<" events!"<<std::endl;
    return;
    
  }
  
  _total_events = number_events;
  
}

void FileReader::setInterpolationFactor(double factor){  
  
  _interpolation_factor = factor;
  
}

void FileReader::setDefaultInterpolationFactor(){
  
  setInterpolationFactor(0.5);
  
}

void FileReader::setUseSurfaceAnts(int bit){
  
  _use_surface_ants = bit;
  
}

int FileReader::debug_bit=0;
