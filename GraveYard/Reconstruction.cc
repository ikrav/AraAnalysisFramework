#include "Reconstruction.h"

int Reconstruction::debug_bit=0;

Reconstruction::Reconstruction(){
  
  initialize();
    
  // these also set the static members inside Channels and CurvedRay
  setStationGeometry(new StationGeometry());
  setOpticalIce(new OpticalIce());
  
  
}

Reconstruction::Reconstruction(StationGeometry *geometry, OpticalIce *ice){
  
  initialize();
  
  // these also set the static members inside Channels and CurvedRay
  setStationGeometry(geometry);
  setOpticalIce(ice);
  
}

Reconstruction::~Reconstruction(){
 
  if(_geometry)    delete _geometry;
  if(_ice)         delete _ice;
//   if(_data_reader) delete _data_reader;
  if(_sim_reader)  delete _sim_reader;
  if(_generator)   delete _generator;
//   if(_output)      delete _output;
//   if(_parser)      delete _parser;
//   if(_data)        delete _data;
  
  
}

void Reconstruction::initialize(){
 
  _geometry=0;
  _ice=0;
//   _data_reader=0;
  _sim_reader=0;
  _generator=0;
//   _output=0;
//   _parser=0;
//   _data=0;
  
  _which_input=NONE;
  _max_num_events=1e9; // for safety maybe use a lower default maximum??
  
}

void Reconstruction::printout(){
 
  std::cout<<"\nStationGeometry and OpticalIce objects:"<<std::endl;
  if(_geometry) _geometry->printout();
  else std::cerr<<__PRETTY_FUNCTION__<<" ERROR: no geometry defined for Reconstruction class\n";
  
  if(_ice) _ice->printout();
  else std::cerr<<__PRETTY_FUNCTION__<<" ERROR: no ice model defined for Reconstruction class\n";
  
  std::cout<<"\nInput method is "<<name_input_type(_which_input)<<std::endl;
  
//   if(_which_input==DATA_READER && _data_reader) _data_reader->printout();
  if(_which_input==SIM_READER && _sim_reader)  _sim_reader->printout();
  if(_which_input==GENERATOR && _generator)   _generator->printout();
    
//   if(_output){ cout<<"\nOutput object:"<<std::endl; _output->printout(); }
//   else std::cout<<__PRETTY_FUNCTION__<<" WARNING: no output defined for Reconstruction class\n";
//     
//   if(_parser){ 
//     std::cout<<"Parser is applied: "<<std::endl;
//     _parser->printout();
//   
//   }
//   
//   if(_data){ 
//       
//     std::cout<<"EventData object:"<<std::endl;
//     _data->printout();
//   
//   }
}

void Reconstruction::help(){
 
  std::cout<<"HELP: Reconstruction "<<std::endl;
  std::cout<<"SETTING UP:\n Make sure you setup one input method: \n  *setupEventGenerator()\n  *setupAraSimFileReader(string filenames)\n  *setupL0FileReader(string filenames)"<<std::endl;
  std::cout<<" make sure you also use setOutput(string filename, int write_mode) so the results would be saved to file. "<<std::endl;
  std::cout<<"\nLOOPING ON EVENTS:\n For each event make sure you use applyTimeFinder(int finder_type,...) or applyDeltaFinder(int finder_type,...).\n Also use addVertexFinder(int finder_type,...) to add finders in parallel. \n Once you added at least one vertex finder, use runVertexFinder() to apply them. \n Additional rounds of add- or runVertexFinder will improve on previous results."<<std::endl;
//   std::cout<<"use setMaxNumEvents(int max_num_events) to break from the loop if 
  
}

// getters
StationGeometry *Reconstruction::getStationGeometry() const {

  return _geometry;
  
}

OpticalIce *Reconstruction::getOpticalIce() const {
  
  return _ice;
  
}

EventGenerator *Reconstruction::getEventGenerator() const {
  
  return _generator;
  
}

// L0FileReader *Reconstruction::getL0FileReader() const {
// 
//   return _data_reader;
//   
// }

AraSimFileReader *Reconstruction::getAraSimFileReader() const {
 
  return _sim_reader;
  
}

// EventData *Reconstruction::getEventData() const {
//   
//   return _data; 
//   
// }
// 
// Output *Reconstruction::getOutput() const {
//   
//   return _output;
//   
// }
// 
// Parser *Reconstruction::getParser() const {
//   
//   return _parser;
//   
// }

// Input *Reconstruction::getCurrentInput() const {
//   
//   if(_which_input==NONE) return 0;
// //   if(_which_input==DATA_READER) return _data_reader;
//   if(_which_input==SIM_READER) return _sim_reader;
//   if(_which_input==GENERATOR) return _generator;
//   
// }

// int Reconstruction::getCurrentEventNum() const {
//  
//   int number=0;
//   
//   if(getCurrentInput()) number=getCurrentInput()->getCurrentEventNum();
//   else std::cerr<<__PRETTY_FUNCTION__<<" ERROR: there is no current input set...\n";
//     
//   return number;
//   
// }

int Reconstruction::getMaxNumEvents() const {
 
  return _max_num_events;
  
}

// int Reconstruction::getTotalNumEvents() const {
//  
//   int number=0;
//   
//   if(getCurrentInput()) number=getCurrentInput()->getTotalNumEvents();
//   else std::cerr<<__PRETTY_FUNCTION__<<" ERROR: there is no current input set...\n";
//     
//   return number;
//   
// }

// setters
void Reconstruction::setStationGeometry(StationGeometry *geometry){
  
  _geometry=geometry;
  Channel::setGeometry(geometry);
  std::vector<double> origin=_geometry->getOrigin();
  
  Pos::setOriginStatic(origin[0], origin[1], origin[2]);
    
}

void Reconstruction::setOpticalIce(OpticalIce *ice){
  
  _ice=ice;
  CurvedRay::setOpticalIce(ice);
  
}

// void Reconstruction::setupParser(Parser *parser){
//  
//   std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this method not yet implemented...\n";
//  
//   
// }

void Reconstruction::setChannels(ChannelCollection channels){
  
  // creates a single EventData object. 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this method not yet implemented...\n";
  
}

// void Reconstruction::setupL0FileReader(L0FileReader *reader){
//   
//   if(reader==0){
//     std::cerr<<__PRETTY_FUNCTION__<<" ERROR: the pointer given is null...\n";
//     return;
//   }
//   
//   if(_data_reader) delete _data_reader;
//   _data_reader = new L0FileReader(*reader); // must have a working copy constructor! 
//   setStationGeometry(_data_reader->getStationGeometry());
//   _data_reader->setMaxNumEvents(getMaxNumEvents());
//   
//   if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<" setting maximum number of events to loop over to "<<getMaxNumEvents()<<endl;
//   
//   _which_input = DATA_READER; 
//   
// }

// void Reconstruction::setupL0FileReader(std::string filenames){
//   
//   if(_data_reader) {
//     delete _data_reader;
//     _data_reader = 0;
//   }
//   
//   _data_reader = new L0FileReader(filenames);
// 
//   setStationGeometry(_data_reader->getStationGeometry());
// 
//   _data_reader->setMaxNumEvents(getMaxNumEvents());
//   
//   if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<" setting maximum number of events to loop over to "<<getMaxNumEvents()<<endl;
//   
//   _which_input = DATA_READER; 
//   
// }

void Reconstruction::setupAraSimFileReader(AraSimFileReader *reader){
 
    
  if(reader==0){
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: the pointer given is null...\n";
    return;
  }
  
  if(_sim_reader) delete _sim_reader;
  _sim_reader = new AraSimFileReader(*reader); // must have a working copy constructor! 
  setStationGeometry(_sim_reader->getStationGeometry());
  _sim_reader->setMaxNumEvents(getMaxNumEvents());
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<" setting maximum number of events to loop over to "<<getMaxNumEvents()<<endl;
  
  _which_input = SIM_READER; 
  
}

void Reconstruction::setupAraSimFileReader(std::string filenames){

  if(_sim_reader) delete _sim_reader;
  
  _sim_reader = new AraSimFileReader(filenames);

  setStationGeometry(_sim_reader->getStationGeometry());

  _sim_reader->setMaxNumEvents(getMaxNumEvents());
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<" setting maximum number of events to loop over to "<<getMaxNumEvents()<<endl;
  
  _which_input = SIM_READER; 
    
}

void Reconstruction::setupAraSimFileReader(std::vector<std::string> filenames){

  if(_sim_reader) delete _sim_reader;
  
  _sim_reader = new AraSimFileReader(filenames);

  setStationGeometry(_sim_reader->getStationGeometry());

  _sim_reader->setMaxNumEvents(getMaxNumEvents());
  
  if(debug_bit) std::cout<<__PRETTY_FUNCTION__<<" setting maximum number of events to loop over to "<<getMaxNumEvents()<<endl;
  
  _which_input = SIM_READER; 
    
}

void Reconstruction::setupEventGenerator(EventGenerator *generator, int max_num_events){

  if(generator) _generator=new EventGenerator(*generator);
  else{ 
    
    _generator=new EventGenerator();
  
    // some default setups for easy reconstruction... 
    _generator->setXrange(-1600,1600);
    _generator->setYrange(-1600,1600);
    _generator->setAmplitudeLog(3);
    _generator->setSignalWidth(10);
    
    
  }
  
  if(max_num_events>0) setMaxNumEvents(max_num_events);
  else _generator->setMaxNumEvents(getMaxNumEvents());
  
  _which_input=GENERATOR;
   
  
}

// void Reconstruction::setOutput(Output *output){
//  
//   if(_output) delete _output;
//   
//   _output = new Output(*output); // there needs to be a copy constructor!
//    
//   
// }
// 
// void Reconstruction::setOutput(std::string filename, int write_mode){
//   
//   _output = new Output(filename);
//   
//   setWriteMode(write_mode);
// 
// }
// 
// void Reconstruction::setWriteMode(int write_mode){
//  
//   if(_output==0){
//     std::cerr<<__PRETTY_FUNCTION__<<" ERROR: there is no defined output object! "<<endl;
//     return;
//     
//   }
//   
//   _output->setWriteMode(write_mode);
//   
// }

// int Reconstruction::loadEvent(int event_number){
//   
//   if(event_number>-1) setCurrentEventNum(event_number);
//     
//   if(!getCurrentInput()){ std::cerr<<__PRETTY_FUNCTION__<<" ERROR: there is no input set for Reconstruction...\n"; }
//   
//   getCurrentInput()->loadEvent();
//   
// }

// void Reconstruction::processEvent(){
//   
//   if(_data==0){ 
//     std::cerr<<__PRETTY_FUNCTION__<<" ERROR: there is no event loaded!\n";
//     return;
//     
//   }
//   int pol;
//   int i;
//   
//   for(i=0; i< _parser->_config.size(); i++){
// 
//     switch(_parser->_config[i]->getPolType()){
// 
//     case StationGeometry::VPOL:
//       pol=0;
//       getEventData()->getChannels().getVpolChannelCollection().applyTimeDeltaFinder(_parser->_config[i]->getTimeType(), _parser->_config[i]->getDeltaType(), _parser->_config[i]->getTimeFinderParameters()[0], _parser->_config[i]->getTimeFinderParameters()[1]);
//       break;
// 
//     case StationGeometry::HPOL:
//       pol=1;
//       getEventData()->getChannels().getHpolChannelCollection().applyTimeDeltaFinder(_parser->_config[i]->getTimeType(), _parser->_config[i]->getDeltaType(), _parser->_config[i]->getTimeFinderParameters()[0], _parser->_config[i]->getTimeFinderParameters()[1]);
//       break;
//       
//     case StationGeometry::BOTH:
//       pol =2;
//       getEventData()->getChannels().applyTimeDeltaFinder(_parser->_config[i]->getTimeType(), _parser->_config[i]->getDeltaType(), _parser->_config[i]->getTimeFinderParameters()[0], _parser->_config[i]->getTimeFinderParameters()[1]);
//       break;
//       
//     default:
//       std::cerr<< "NO FINDER EXECUTED!!! Check if the polarazition is right.... "<< std::endl;
//       pol=0;
//     }
//       
//     //for each vertex, in a given reco
//     for(int j=0; j<_parser->_config[i]->getVertices().size(); j++){
// 
//       getEventData()->addVertexFinder(pol, _parser->_config[i]->getVertices()[j]->getVertexType(), _parser->_config[i]->getVertices()[j]->getCoordinateType(), _parser->_config[i]->getDeltaType(), _parser->_config[i]->getVertices()[j]->getLockType(), _parser->_config[i]->getVertices()[j]->getParameter1(), _parser->_config[i]->getVertices()[j]->getParameter2(), _parser->_config[i]->getVertices()[j]->getParameter3());
//     
// 
//     //if method does not have a previous finder, just add the methods and it its not the last in the array of finders
//     
//       if(_parser->_config[i]->getVertices()[j]->getPreviousFinder() == 0 && _parser->_config[i]->getVertices().size() -1 != j){
// 	continue;
//       }
//     
//       getEventData()->findVertex(pol);
//     }
//   }
// 										    
//   
//   // continue to run everything.
//   std::cerr<<__PRETTY_FUNCTION__<<" no yet implemented..."<<endl;
//   
// }
// 
// void Reconstruction::loop(int max_num_events){
//  
//   if(max_num_events>0) setMaxNumEvents(max_num_events);
//   
//   if(!getCurrentInput()){ 
//     std::cerr<<__PRETTY_FUNCTION__<<" ERROR: there is no input set for Reconstruction...\n";
//     return;
//   }
//   
//   getCurrentInput()->setCurrentEventNum(0);
//   
//   for(int e=0;e<_max_num_events; e++){
//    
//     loadEvent();
//     processEvent();
//     
//   }
//   
// 
// }

void Reconstruction::setMaxNumEvents(int max_num_events){
 
  _max_num_events=max_num_events;
  
//   if(_data_reader) _data_reader->setMaxNumEvents(max_num_events);
  if(_sim_reader) _sim_reader->setMaxNumEvents(max_num_events);
  if(_generator) _generator->setMaxNumEvents(max_num_events);
  
}

// void Reconstruction::setCurrentEventNum(int event_number){
//  
//   if(event_number<0 || event_number>=_max_num_events || event_number >= getTotalNumEvents()){
//     
//     std::cerr<<__PRETTY_FUNCTION__<<" ERROR: can't input event number "<<event_number<<endl;
//     return;
//     
//   }
//   
//   getCurrentInput()->setCurrentEventNum(event_number);
//   
// }

// void Reconstruction::useAveragePolResults(){
//  
//   _data->chooseResults(EventData::AVERAGE);
//     
// }
// 
// void Reconstruction::useBestPolOrCombined(){
//  
//   _data->chooseResults(EventData::BEST);
//   
// }

void Reconstruction::applyTimeFinder(int finder_type, double par1, double par2){
  
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet...\n";
  
}

void Reconstruction::applyDeltaFinder(int finder_type, double par1, double par2){
 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet...\n";
  
}

void Reconstruction::addVertexFinder(int type, int coordinate_system, int time_input, int lock_parameter, double par1, double par2, double par3, int variant){
  
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet...\n";
  
}

void Reconstruction::runVertexFinder(){
  
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet...\n";
  
}

void Reconstruction::setAllDebugBits(int reco_debug, int data_debug, int readers_debug, int output_debug,
			      int time_finder_debug, int vertex_finder_debug, int parser_debug, 
			      int geometry_ice_debug, int channels_debug, int timing_debug, 
			      int position_debug, int ray_debug){
 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet...\n";
  
}

const std::string Reconstruction::name_input_type(int index){
 
  switch(index){
   
    case NONE: return "NONE";
    case DATA_READER : return "DATA_READER";
    case SIM_READER : return "SIM_READER";
    case GENERATOR : return "GENERATOR";
    default : return "--unknown input type--";
    
  }
  
}
























