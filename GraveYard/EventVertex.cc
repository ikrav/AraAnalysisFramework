#include "EventVertex.h"

ClassImp(EventVertex);

int EventVertex::debug_bit=0;

EventVertex::EventVertex() : VertexPos() {
  
  initialize();
  
}

EventVertex::EventVertex(ChannelCollection channels, OpticalIce *ice) : VertexPos() {
 
  initialize();
 
  _channels=channels;
  _ice=ice;
  
}

EventVertex::EventVertex(ChannelCollection channels, Pos starting_position, OpticalIce *ice) : VertexPos(starting_position) {
 
  initialize();
 
  _channels=channels;
  _ice=ice;
  
}

EventVertex::EventVertex(ChannelCollection channels, VertexPos *starting_position, OpticalIce *ice) : VertexPos(*starting_position) {
 
  initialize();
  
  _channels=channels;
  _ice=ice;
     
}

EventVertex::~EventVertex(){

//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  clearFinders();

}

void EventVertex::initialize(){
  
  _ice=0;
  
}

void EventVertex::printout(int coordinate_system){
  
  std::cout<<"current position:"<<std::endl; 
  
  VertexPos::printout(coordinate_system);
  
  for(int i=0;i<_finders.size();i++){
    
    std::cout<<"i= "<<std::setw(2)<<i<<std::endl;
    
    _finders[i]->printout();
    
  }// for i
  
}

void EventVertex::addFinder(int type, int coordinate_system, int time_input, int lock_parameter, double par1, double par2, double par3, int variant){
 
  // TODO: add , int variant to any finders that have multiple versions. 
  
  if(getNumChannels()<1){ cerr<<"ERROR: _channels is empty, use setChannels().\n"; return; }
  
  VertexFinder *new_finder=0;

  VertexPos *_this = this;

  switch(type){
    
    case VertexFinder::REAL_VERTEX : new_finder=new RealVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;
    
    case VertexFinder::SCAN : new_finder=new ScanVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;
    
    case VertexFinder::MINUIT : new_finder=new MinuitVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;

    case VertexFinder::ASM : new_finder=new AnalyticVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;
    
    case VertexFinder::MCMC : new_finder=new MCMCVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;
    
    case VertexFinder::INTMAP : new_finder=new IntMapVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;

  case VertexFinder::EARLIEST : new_finder=new EarliestVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;

  case VertexFinder::ANYWHERE : new_finder=new AnyWhereVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;
    
  case VertexFinder::UNLKUMINUIT : new_finder=new MinuitUNLKUVertexFinder(_channels, this, coordinate_system, time_input, lock_parameter, _ice, par1, par2, par3); break;
    
    default : std::cerr<<"ERROR: no such vertex finder "<<type<<"\n"; return;
    
  }
  
  if(hasRealPos()) new_finder->setRealPosition(getRealPosition());
  
//   new_finder->setupScanHist();
  
  _finders.push_back(new_finder);
  
}

void EventVertex::findVertex(int starting_position){
  
  std::cout << __PRETTY_FUNCTION__ << " " << starting_position << std::endl;
  VertexPos *initial;
  
  switch(starting_position){
  
  case EventVertex::DEFAULT :

    initial = new VertexPos();
    break;
  
  case EventVertex::PREVIOUS:


    if(_finders.size() == 0){

      std::cerr << "FINDERS SIZE == 0. Using default starting position"<< std::endl;
      initial = new VertexPos();
    }
    else {
      int last_index=_finders.size() -1; // to keep track of last executed method
      
      for(int i= _finders.size()-1; i>-1; i--){
	if( _finders[i]->hasExecuted() ) {
	  last_index = i;
	  break;
	}
      }
      
      initial = new VertexPos(*(VertexPos*)_finders[last_index]);
      
    }
    initial->printout();
    break;
  
  case EventVertex::AVERAGE:
    {
      std::vector<VertexPos*> positions;
      positions.reserve(_finders.size());

      for(int i=0;i<_finders.size();i++){ 
	if( !_finders[i]->hasExecuted() ) continue; // to make sure we do not add methods that have not executed
	positions.push_back(_finders[i]);
      }
      if(positions.size() == 0) {
	std::cerr <<" FINDERS SIZE == 0. Using default starting position" << std::endl;
	initial = new VertexPos();
      }
      else {
	initial = meanVertexPos(positions, VertexPos::AVERAGE);
      }
    } 
    break;
    
  case EventVertex::BEST:
    {
      std::vector<VertexPos*> positions;
      positions.reserve(_finders.size());

      for(int i=0;i<_finders.size();i++){
	if( !_finders[i]->hasExecuted() ) continue; // to make sure we do not add methods that have not executed
	positions.push_back(_finders[i]);
      }
      
      if(positions.size() == 0) {
	std::cerr <<" FINDERS SIZE == 0. Using default starting position" << std::endl;
	initial = new VertexPos();
      }
      else {
	initial = meanVertexPos(positions, VertexPos::AVERAGE);
      }
      
    }
    break;
      
  default:
   
    initial = new VertexPos();
    break;
   
  }

  for(int i=0; i<_finders.size(); i++){

    if(_finders[i]->hasExecuted()) continue;
    std::cout << "Executing : "<< _finders[i]->getFinderName() << std::endl;
    if(hasRealPos()) _finders[i]->setRealPosition(getRealPosition());
    _finders[i]->findVertex(initial);
    if(debug_bit) _finders[i]->printout();
    
  }


  if(initial) delete initial;
  initial = 0;
 
}

void EventVertex::findVertex(){
 
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  for(int i=0;i<_finders.size();i++){

    if(_finders[i]->hasExecuted()) continue;

    if(hasRealPos()) _finders[i]->setRealPosition(getRealPosition());
    
    _finders[i]->findVertex();
    
    if(debug_bit) _finders[i]->printout();
    
  }// for i
  
  std::vector<VertexPos*> positions;
  positions.reserve(_finders.size());
  for(int i=0;i<_finders.size();i++){ 
    
    positions.push_back(_finders[i]);
    
  }
  
  VertexPos *pos= meanVertexPos(positions);
  //  adoptPreviousPos(meanVertexPos(positions));
  adoptPreviousPos(pos);

  if(pos) delete pos;
  pos = 0;
  //positions.clear();
  
  pushFinders();
  
}

void EventVertex::pushFinders(){
 
  _prev_finders.push_back(_finders);
  //  _finders.clear();
    
}

void EventVertex::clearFinders(){
  
  for(int i=0;i<_finders.size();i++){
    
    if(_finders[i]) {
      delete _finders[i];

    }
    
    _finders[i]=0;
    
  }
  
  _finders.clear();  
 
  clearPrevFinders();
  
}
  
void EventVertex::clearPrevFinders(){

  for(int i=0;i<_prev_finders.size();i++){
    
    for(int j=0;j<_prev_finders[i].size(); j++){

      _prev_finders[i][j] = 0;
    
    }
    _prev_finders[i].clear();
    
  }
  // we simply clear the vector, since this vector has a pointer to the finders
  _prev_finders.clear();

}

// getters
OpticalIce *EventVertex::getOpticalIce() const {
  
  return _ice;
  
}

int EventVertex::getNumChannels() const {
  
  return _channels.getNumChans();
  
}

int EventVertex::getNumFinders() const {
  
  return _finders.size();
}

int EventVertex::getNumPrevFinders() const {
  
  return _prev_finders.size();
}

ChannelCollection EventVertex::getChannels() const {
  
  return _channels;
  
}

std::vector<VertexFinder*> EventVertex::getFinderVector() const {
  
  return _finders;
}

std::vector< std::vector<VertexFinder*> > EventVertex::getPrevFinderVector() const {
  
  return _prev_finders;
}

VertexFinder *EventVertex::getFinder(int index) const {
 
  if(index<0 || index>_finders.size()) return 0;
  
  return _finders[index];
  
}

VertexFinder *EventVertex::getPrevFinder(int level, int index) const {
 
  if(level<0 || level>=_prev_finders.size() || index<0 || index>= _prev_finders[level].size()) return 0;
  
  return _prev_finders[level][index];
  
}

// setters
void EventVertex::setOpticalIce(OpticalIce *ice){
 
  _ice=ice;
  
}

void EventVertex::setChannels(ChannelCollection channels){
 
  _channels=channels;
  
}

