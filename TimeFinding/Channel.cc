#include "Channel.h"

ClassImp(Channel);

Channel::Channel() : _channel_id(0), _station_id(-1), _is_simulated(1){
  
  initialize();
  
}

Channel::Channel(int channel_id, TGraph *waveform, int station_id, int is_simulated) : _channel_id(channel_id), _station_id(station_id), _is_simulated(is_simulated){
 
  initialize();
  
  setWaveform(waveform);
  
//   setPolarization(polarization);
  
}

Channel::Channel(const Channel &other) : _channel_id(other.getChannelId()), _station_id(other.getStationId()), _is_simulated(other.isSimulated()), Timing(other) {
 
  // the copy constructor does the right thing and makes a deep copy
  _waveform=0;
  
  if(other._ray) _ray=new CurvedRay(*other._ray);
  else _ray=0;
  
  if(other.getWaveform()) setWaveform(new TGraph(*other.getWaveform())); // this also clears the CorrGraph and finder objects
  
  deepCopyTimeFinders(other._finders);
  deepCopyDeltaFinders(other._delta_finders);
  deepCopyCorrGraphs(other._corr_graphs);  
  
  _recent_delta_finder=other.getRecentDeltaFinderIndex();
 
  // but it doesn't set the finders' pointers to the new Channel object (this and this channel's pair) that is done in ChannelCollection::assignDeepCopy()
  
}

Channel::~Channel(){
  
  clearFinders();
  clearWaveform();
  
  if(_ray) delete _ray;
  
}

void Channel::initialize(){
 
//   std::cout<<"Channel::initialize()"<<std::endl;
  
  _waveform=0;
  
  _ray=0;

  _recent_delta_finder=-1;
  _finders.clear();
  _finders.resize(0);
  
}

void Channel::printout(){
 
  
  std::cout<<"ch= "<<std::setw(2)<<getChannelId();
  if(getStationId()>=0) std::cout<<" /"<<std::setw(2)<<getStationId();
  else std::cout<<"    ";
  std::cout<<" time:  ";
  Timing::printout();
  
  if(_geom) {
    
    for(int i=0;i<18;i++) std::cout<<" ";
    _geom->getPosition(getChannelId()).printout();
    
  }
  
//   for(int i=0;i<_corr_graphs.size();i++) std::cout<<"j= "<<_corr_graphs[i]->getSecondChannelId()<<"  corr_graph= "<<_corr_graphs[i]<<std::endl;
  
//   for(int i=0;i<_delta_finders.size();i++) for(int j=0; j<_delta_finders[i].size(); j++) std::cout<<"delta finder["<<i<<"]["<<j<<"]= "<<_delta_finders[i][j]->getFirstChannelId()<<" / "<<_delta_finders[i][j]->getSecondChannelId()<<std::endl;
  
}

void Channel::deepCopyTimeFinders(std::vector<TimeFinder*> finders){
 
  for(int i=0; i<_finders.size(); i++){
   
    if(_finders[i]) delete _finders[i];
    
  }
  
  _finders.clear();
  _finders.reserve(finders.size());
    
  for(int i=0;i<finders.size();i++){
   
     _finders.push_back(finders[i]->getDeepCopy());
     _finders[i]->setWaveform(getWaveform()); // make sure each finder points to the current waveform object
  }
  
}

void Channel::deepCopyDeltaFinders(std::vector< std::vector<DeltaFinder*> > finders){
  
  _delta_finders.reserve(finders.size());
  
  for(int i=0;i<finders.size();i++){
    
    _delta_finders.push_back(std::vector<DeltaFinder*>());
    _delta_finders[i].reserve(finders[i].size());
        
    for(int j=0;j<finders[i].size();j++){
   
      _delta_finders[i].push_back(finders[i][j]->getDeepCopy());
      _delta_finders[i][j]->setFirstChannel(this); // make sure the finders point to the right channel. the second channel needs to be handled by ChannelCollection
      
    }
    
  }
  
}

void Channel::deepCopyCorrGraphs(std::vector<CorrGraph*> graphs){
    
  for(int i=0;i<_corr_graphs.size(); i++){
    
    if(_corr_graphs[i]) delete _corr_graphs[i];
  
  }
 
  _corr_graphs.clear();
  _corr_graphs.reserve(graphs.size());
  
  for(int i=0;i<graphs.size();i++){
   
    _corr_graphs.push_back(graphs[i]->getDeepCopy());
    // need to set the channels for these objects (done in ChannelCollection)
    
  }
  
  
  
}

void Channel::clearFinders(){
 
  // get rid of finder list and intermediary calculations... 
  for(int i=0; i<_finders.size(); i++) {
   
    if(_finders[i])

      delete _finders[i];
  }
  
  _finders.clear();
  
  for(int i=0;i<_delta_finders.size();i++){ 
    
    for(int j=0;j<_delta_finders[i].size();j++){ 
      
      if(_delta_finders[i][j])
	delete _delta_finders[i][j];
      
    }
    _delta_finders[i].clear();
  }
  
  _delta_finders.clear();
  
}

void Channel::clearWaveform(){
 
  if(_waveform){ delete _waveform; _waveform=0; }

  clearFinders();
  
  for(int i=0;i<_corr_graphs.size(); i++){
    
    if(_corr_graphs[i]) delete _corr_graphs[i];
  
  }
  
  _corr_graphs.clear();
  
}

TimeFinder *Channel::operator[](int index) const {
  return _finders[index];
}

// getters
int Channel::getChannelId() const { return _channel_id; }

int Channel::getStationId() const { return _station_id; }

int Channel::isSimulated() const { return _is_simulated; }

void Channel::getFFT(std::vector<double> &real, std::vector<double> &imag, double &freqStep) {

  real.clear();
  imag.clear();

  int N=_waveform->GetN(); 
  double oldY[N];
  for(int i=0; i<N; i++) {
    oldY[i] = _waveform->GetY()[i];
  }
  
  freqStep=1/(((_waveform->GetX()[1]-_waveform->GetX()[0]))*N);
  //printf("N: %i \t FreqStep = %.6f \t TimeStep: %.6f \n",N,freqStep,_waveform->GetX()[1]-_waveform->GetX()[0]);
  //freqStep *= 1e3;
  FFTWComplex *thefft=FFTtools::doFFT(N,oldY);

  for(int i=0; i<(N/2+1); i++) {
    real.push_back(thefft[i].re);
    imag.push_back(thefft[i].im);
  }

  delete [] thefft;

}

double Channel::getTimeOffsetFromRay() const {
 
  if(_ray==0){ std::cerr<<"ERROR: no ray object has been initialized here...\n"; return 0; }
  
  return getTime() - _ray->getHitTime();
  
}

double Channel::getX() const {
 
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return 0; }
  
  return _geom->getPosition(getChannelId()).getX();
  
}

double Channel::getY() const {
   
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return 0; }
  
  return _geom->getPosition(getChannelId()).getY();
  
}

double Channel::getZ() const {
 
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return 0; }
  
  return _geom->getPosition(getChannelId()).getZ();
  
}

std::vector<double> Channel::getCartesian() const {
 
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return std::vector<double>(3,0); }
  
  return _geom->getPosition(getChannelId()).getCartesian();
  
}

double Channel::getR() const {
 
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return 0; }
  
  return _geom->getPosition(getChannelId()).getR();
  
}

double Channel::getPhi() const {
  
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return 0; }
  
  return _geom->getPosition(getChannelId()).getPhi();
  
}

double Channel::getTheta() const {
  
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return 0; }
  
  return _geom->getPosition(getChannelId()).getTheta();
  
}

std::vector<double> Channel::getSpherical() const {
  
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return std::vector<double>(3,0); }
  
  return _geom->getPosition(getChannelId()).getSpherical();
  
}

Pos Channel::getPosition() const {
  
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return Pos(); }
  
  return _geom->getPosition(getChannelId());
  
}

int Channel::getPolarization() const { 
  
  
  if(!_geom){ std::cerr<<"no StationGeometry assigned. Use Channel::setGeometry()...\n"; return 0; }
  
  return _geom->getPolarization(getChannelId()); 
  
}

TGraph *Channel::getWaveform() const { 
  
  return _waveform; 
  
}

StationGeometry *Channel::getGeometry(){
  
  return _geom;
  
}

CurvedRay *Channel::getRay(){
  
  if(_ray==0) _ray=new CurvedRay(getPosition());
  
  return _ray;
  
}

std::vector<TimeFinder*> Channel::getFindersVector() const{
  
  return _finders;
  
}

std::vector< std::vector<DeltaFinder*> > Channel::getDeltaFindersMatrix() const {
 
  return _delta_finders;
  
}

std::vector<CorrGraph*> Channel::getCorrelationGraphVector() const {
  
  return _corr_graphs;
  
}

std::vector<CorrGraph*> Channel::getCorrelationGraphVector(std::vector<Channel*> channels){
 
  std::vector<CorrGraph*> vec;
  
  // need to make sure we have all the pairs of "this" and "channels" without repetition
  for(int ch=0;ch<channels.size();ch++){
    
    CorrGraph *pointer=getCorrGraph(channels[ch]);
    
    if(pointer) vec.push_back(pointer);
    
  }// for ch
  
  return vec;
  
}

CorrGraph *Channel::getCorrGraph(Channel *channel){
 
      // check it isn't this channels
    if(channel->getChannelId()==getChannelId()) return 0; // if so, don't make a new CorrGraph
    
    // check if "this" already occurs in another channel
    std::vector<CorrGraph*> temp_vec=channel->getCorrelationGraphVector();
    
    for(int i=0;i<temp_vec.size();i++){
      
      if(temp_vec[i]->getSecondChannelId()==getChannelId()) return 0;// if so, don't make a new CorrGraph
      
    }
    
    CorrGraph *pointer=0;

    // check if this channel already exists:
    for(int i=0;i<_corr_graphs.size();i++){
     
      if(_corr_graphs[i]->getSecondChannelId()==channel->getChannelId()) pointer=_corr_graphs[i];
      
    }
    
    if(pointer==0){ // if all else fails, add a new CorrGraph object to this Channel and to the output vector
      
      pointer=new CorrGraph(this, channel); 
      
      _corr_graphs.push_back(pointer);// store for later use
      
    }
    
    return pointer;// in the last two cases, return a valid pointer... 
  
}

// setters
void Channel::setWaveform(const TGraph *waveform){
 
  //  _waveform->Print();
  clearWaveform();
  if(waveform) _waveform=(TGraph*) waveform->Clone();
  
}

void Channel::setGeometry(StationGeometry *geometry_ptr){
  
  _geom=geometry_ptr;
  
}

// TimeFinder methods
void Channel::applyFinder(int finder_type, double par1, double par2){// public: use this to apply a finder to a channel
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  TimeFinder::setGeometry(_geom); // set geometry - might need to be moved where this will only be called once.
  
  if(_waveform==0){ std::cerr<<"ERROR: no waveform in channel "<<getChannelId()<<" to apply a finder to...\n"; return; }
  
  int index=checkForFinder(finder_type, par1, par2);
  
  if(index<0 || index>_finders.size()) index=addFinder(finder_type, par1, par2);

  useDataFromFinder(index);
  
}

TGraph *Channel::applyCSW(TGraph *csw, int finder_type, double par1, double par2){
 
  if(_waveform==0){ std::cerr<<"ERROR: no waveform in channel "<<getChannelId()<<" to apply a finder to...\n"; return NULL; }


  TimeFinder::setGeometry(_geom); // set geometry - might need to be moved where this will only be called once.
    
  /* remember we explictly run CSW twice from ChanCollection, so we can tell if the previous was CSW 1st iteration*/
  if(par1 == 1.0) {
    int csw_index = _finders.size()-1;
    //    _finders.pop_back(); //delete the 1st iteration of CSW
    if(_finders[csw_index])
      delete _finders[csw_index];
    
    _finders[csw_index] =0;
    _finders.pop_back();
			     
  }
  
  _finders.push_back(new CSWTimeFinder(getWaveform(), csw, par1, par2, getChannelId(), getStationId(), isSimulated()));
  
  useDataFromFinder(_finders.size()-1);
  
  CSWTimeFinder *derived = dynamic_cast<CSWTimeFinder*>(_finders.back());

  if(derived){
    return derived->getCSW();
  }
  
  // No need to worry about the NULL, the statement above is always going to hold, because we explicitly call CSWTimeFinder
  return NULL;
}

void Channel::useDataFromFinder(int index){ // copy the timing data from the finder used to "this"
 
//   std::cout<<"Channel::useDataFromFinder()"<<std::endl;

  setFinderName(_finders[index]->getFinderName());
  setFinderType(_finders[index]->getFinderType());
  setTimingData(_finders[index]->getTime(), _finders[index]->getWeight(), _finders[index]->getError());
  
  if(_finders[index]->hasRealTime()) setRealTime(_finders[index]->getRealTime());
  
  
}

int Channel::checkForFinder(int finder_type, double par1, double par2){ // check if there is already such a finder applied
  
//   std::cout<<"Channel::checkForFinder()"<<std::endl;
  
  int index=-1;
  
  for(int i=0;i<_finders.size(); i++){ 
    
    if( _finders[i]->getFinderType()==finder_type && _finders[i]->isUsingParameters(par1, par2) ) index=i;
    
  }
  
  return index;
  
  
}

int Channel::addFinder(int finder_type, double par1, double par2){ // add another finder if necessary. 
 
//   std::cout<<"Channel::addFinder()"<<std::endl;
  
  switch(finder_type){
    case TimeFinder::REAL_TIMES :
    
    _finders.push_back(new RealTimeFinder(getWaveform(), par1, par2, getChannelId(), getStationId(), isSimulated()));
    _finders.back()->setRealTime(getRealTime());// take real time from Channel into finder...
    _finders.back()->scanWaveform();
    break;
  
    case TimeFinder::THRESH :
    _finders.push_back(new ThreshTimeFinder(getWaveform(), par1, par2, getChannelId(), getStationId(), isSimulated()));
    break;
    
    case TimeFinder::SIMPLE :
      _finders.push_back(new SimpleTimeFinder(getWaveform(), par1, par2, getChannelId(), getStationId(), isSimulated()));
    break;
  
    case TimeFinder::GAUS : 
      _finders.push_back(new GausTimeFinder(getWaveform(), par1, par2, getChannelId(), getStationId(), isSimulated()));
      break;
    
    default : 
      std::cerr<<"ERROR: no such finder, use THRESH, GAUS, REAL, etc...\n";
  }
  
  return _finders.size()-1;
    
}

// DeltaFinder methods
int Channel::applyDeltaFinder(std::vector<Channel*> channels, int finder_type, double par1, double par2){

//   std::cout<<__PRETTY_FUNCTION__<<"  type: "<<DeltaFinder::name_delta_type(finder_type)<<" chan= "<<getChannelId()<<std::endl;
  
    TimeFinder::setGeometry(_geom); // set geometry - might need to be moved where this will only be called once.
  if(_waveform==0){ std::cerr<<"ERROR: no waveform in channel "<<getChannelId()<<" to apply a finder to...\n"; return -1; }
  
  int index=checkForDeltaFinder(finder_type, par1, par2);
  
  if(index<0 || index>_delta_finders.size()) index=addDeltaFinder(finder_type, par1, par2);
  
  fillDeltaFinder(channels, index, finder_type, par1, par2);
  
  _recent_delta_finder=index;

  return index;
  
}

void Channel::fillDeltaFinder(std::vector<Channel*> channels, int index, int finder_type, double par1, double par2){
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(finder_type == DeltaFinder::SUBTRACT){// for SUBTRACT type there is an extra check that all channels have timing data from a TimeFinder. 
     
      int check=1;
      for(int i=0;i<channels.size();i++) if(channels[i]->getFindersVector().size()==0) check=0;
    
      if(check==0){ 
      
	std::cerr<<"ERROR: not all channels have a TimeFinder applied. Applying the default type 1 finder.\n"; 
	for(int i=0;i<channels.size();i++) channels[i]->applyFinder(1); 
      
      }
      
  
  }// if SUBTRACT

  for(int i=0; i<channels.size(); i++){ // loop over the number of channels used by current analysis

    int bit=1;

    if(channels[i]->getChannelId()==getChannelId()) continue; // don't calculate the delta between this channel and itself

    for(int j=0;j<_delta_finders[index].size(); j++){// if this finder vector (marked by index) already has a DeltaFinder with this (second) channel.
            
      if(_delta_finders[index][j]->getSecondChannelId()==channels[i]->getChannelId()){ 
	
	bit=0; // this means continue the outer loop
	break;
	
      }
      
    }// for j
    
    if(bit==0) continue;
    
    // add a new finder (either copied from other channel or new)
    DeltaFinder *finder_ptr=channels[i]->getDeltaFinder(getChannelId(), finder_type, par1, par2);// check if the other channel already has a delta finder of this sort (to add inversed), zero if such finder doesn't exist...
    
    switch(finder_type){
    
      case DeltaFinder::SUBTRACT : // just subtract hit times
	
	if(finder_ptr){
	
	_delta_finders[index].push_back(new SubtractDeltaFinder(*((SubtractDeltaFinder*)finder_ptr) ));
	_delta_finders[index].back()->inverse();
	
	}
        else{// if it doesn't exist in channel[i], create a new one
		
	  _delta_finders[index].push_back(new SubtractDeltaFinder(this, channels[i], par1, par2, getStationId(), isSimulated()));
	
	}
	break;
       
      case DeltaFinder::CROS : // for cross correlation delta finder
      
// 	std::cout<<"pushing back another CROS type finder. ch= "<<getChannelId()<<" pair id= "<<i<<std::endl;

	if(finder_ptr){ // if this finder already exists in the other channel (but inverted)

	  _delta_finders[index].push_back(new CrosDeltaFinder(*((CrosDeltaFinder*)finder_ptr) ));
	  _delta_finders[index].back()->inverse();
	
	}
	else{// if it doesn't exist in channel[i], create a new one
	
	  _delta_finders[index].push_back(new CrosDeltaFinder(this, channels[i], par1, par2, getStationId(), isSimulated()));
          
	}
	break;
	
    case DeltaFinder::EXPECTED : // to get expected time between channels
      if(finder_ptr){
	_delta_finders[index].push_back(new ExpectedDeltaFinder(*((ExpectedDeltaFinder*)finder_ptr)));
	_delta_finders[index].back()->inverse();
      }
      else {
	_delta_finders[index].push_back(new ExpectedDeltaFinder(this, channels[i], par1, par2, getStationId(), isSimulated()));
      }
      break;
      
    default:
      
      std::cerr<<__PRETTY_FUNCTION__<<" ERROR: unknown delta finder type: "<<finder_type<<std::endl;
      
    }// switch/case

    if( hasRealTime() && channels[i]->hasRealTime() ) _delta_finders[index].back()->setRealTime(channels[i]->getRealTime() - getRealTime());// always do second minus first

  }// for i
  
}

int Channel::checkForDeltaFinder(int finder_type, double par1, double par2){
  
  int index=-1;
  
  for(int i=0;i<_delta_finders.size(); i++){ 
    
    if( _delta_finders[i].size() && _delta_finders[i][0]->getFinderType()==finder_type && _delta_finders[i][0]->isUsingParameters(par1, par2) ) index=i;
      
  }
  
//   std::cout<<"index= "<<index<<std::endl;
  
  return index;
   
}

DeltaFinder *Channel::getDeltaFinder(int second_channel_id, int finder_type, double par1, double par2){
 
  int index=checkForDeltaFinder(finder_type, par1, par2);
  
  if(index<0 || index>=_delta_finders.size()) return 0; // no such finder type here
       
  for(int i=0; i<_delta_finders[index].size(); i++) if(_delta_finders[index][i]->getSecondChannelId()==second_channel_id) return _delta_finders[index][i];
	
  return 0;
  
}

int Channel::addDeltaFinder(int finder_type, double par1, double par2){
 
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  _delta_finders.push_back(std::vector<DeltaFinder*>());
  
  return _delta_finders.size()-1;
  
}

void Channel::eraseDeltaFinder(int finder_number, int pair_number){

//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(finder_number>_delta_finders.size()){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: input finder number "<<finder_number<<" is larger than _delta_finders.size= "<<_delta_finders.size()<<std::endl;
    return;
    
  }
  
  if(pair_number>_delta_finders[finder_number].size()){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: input pair number "<<pair_number<<" is larger than _delta_finders["<<finder_number<<"].size= "<<_delta_finders[finder_number].size()<<std::endl;
    return;
    
  }
  
  delete _delta_finders[finder_number][pair_number];
  _delta_finders[finder_number].erase(_delta_finders[finder_number].begin()+pair_number);
  
//   std::cout<<"done erasing delta finder. finder num= "<<finder_number<<" pair_number= "<<pair_number<<std::endl;
  
}

void Channel::eraseCorrGraph(int pair_number){
    
  if(pair_number>_corr_graphs.size()){
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: input pair number "<<pair_number<<" is larger than _corr_graphs.size= "<<_corr_graphs.size()<<std::endl;
    return;
    
  }
  
  delete _corr_graphs[pair_number];
  _corr_graphs.erase(_corr_graphs.begin()+pair_number);
  
}

bool compChannels(Channel *first, Channel *second){ 
  
  return (first->getChannelId() < second->getChannelId() ); 
  
}

bool compDeltas(DeltaFinder *first, DeltaFinder *second){ 
  
  return (first->getSecondChannelId() < second->getSecondChannelId() ); 
  
}  

std::vector<PairTiming> Channel::getTimingVector(std::vector<Channel*> channels, int finder_type, double par1, double par2){
    
  applyDeltaFinder(channels, finder_type, par1, par2);

  return getTimingVector(channels);
  
}

std::vector<PairTiming> Channel::getTimingVector(std::vector<Channel*> channels){
  
  int index=getRecentDeltaFinderIndex();
  
  // make sure both vectors are sorted:
  std::sort(_delta_finders[index].begin(), _delta_finders[index].end(), compDeltas);// by their second channel id
  std::sort(channels.begin(), channels.end(), compChannels);// by channel id
  
//   for(int i=0;i<channels.size();i++) std::cout<<"i= "<<i<<" id= "<<channels[i]->getChannelId()<<std::endl;
  
  // after sorting these, add another Timing object with zero timings in the appropriate place:
  int last_index_below_this_ch_num=-1;
  
  for(int i=0;i<_delta_finders[index].size();i++){
    
    if(getChannelId() < _delta_finders[index][i]->getSecondChannelId()) break;
    
    last_index_below_this_ch_num=i;
    
  }// for i
  
  
//   for(int i=0;i<channels.size();i++) std::cout<<"i= "<<i<<"  ch= "<<channels[i]->getChannelId()<<std::endl;
//   for(int j=0;j<_delta_finders[index].size();j++) std::cout<<"j= "<<j<<"  delta finder second id= "<<_delta_finders[index][j]->getSecondChannelId()<<std::endl;

  std::vector<int> placeholder;
  placeholder.reserve(channels.size());
    
  for(int i=0;i<channels.size();i++){
        
    for(int j=0;j<_delta_finders[index].size();j++){
    
      if(_delta_finders[index][j]->getSecondChannelId() == channels[i]->getChannelId() ){ 
	
	placeholder.push_back(j);
	break;
	
      }
      
    }
  
  }
  
  std::vector<PairTiming> timings;

  for(int i=0;i<channels.size();i++) {
    
    if( channels[i]->getChannelId() == getChannelId() ){// if we're at this very same channel...
      
      timings.push_back(PairTiming(this, this, Timing()));

      continue;

    }
    
    for(int k=0;k<placeholder.size();k++){
    
//       std::cout<<"placeholder["<<k<<"]= "<<placeholder[k]<<std::endl;
      
      if( channels[i]->getChannelId() == _delta_finders[index][placeholder[k]]->getSecondChannelId() ){
     
// 	std::cout<<"channel["<<i<<"].id= "<<std::setw(2)<<channels[i]->getChannelId()<<"  --  finder["<<i<<"].second_id= "<<_delta_finders[index][j]->getSecondChannelId()<<std::endl;
	
	timings.push_back(PairTiming(this, channels[i], *_delta_finders[index][placeholder[k]]));
      
	continue;
	
      }
    
    }// for k
    
  }// for i
  
  return timings;
  
}

bool Channel::operator>(const Channel &other) const {
 
  return ( getChannelId() > other.getChannelId() );
  
}

bool Channel::operator<(const Channel &other) const {
 
  return ( getChannelId() > other.getChannelId() );
  
}

bool Channel::operator>(Channel *other) const {
  
  return ( getChannelId() < other->getChannelId() );
  
}

bool Channel::operator<(Channel *other) const {
  
  return ( getChannelId() < other->getChannelId() );
  
}

int Channel::getRecentDeltaFinderIndex() const {

  return _recent_delta_finder;
  
}

void Channel::draw(){
 
  TGraph *gr=getWaveform();
  
  char title[100];
  
  sprintf(title, "Waveform for channel %d; time (ns); voltage (mV)", getChannelId());
  
  gr->SetTitle(title);
    
  gr->Draw("al");
  
}

StationGeometry *Channel::_geom=0;
