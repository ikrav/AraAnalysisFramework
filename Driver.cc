#include "Driver.h"

Driver::Driver(int maxEvents, string output) : Parser() {

  _output_file = output;
  zeroPointers();
  initialize();
  //  setMaxEvents(maxEvents);
  _max_events = maxEvents;
}

Driver::~Driver() {

  if(_output) delete _output; _output=0;

  _event_data =0; // we only have a pointer
  
  if(_l0_reader) delete _l0_reader; _l0_reader=0;
  
  
  if(_sim_reader) delete _sim_reader; _sim_reader=0;

  if(_ice) delete _ice; _ice=0;
  


}

void Driver::zeroPointers(){

  _event_data=0;
  _l0_reader=0;
  _sim_reader=0;
  _output=0;
  _geometry=0;
  _ice=0;

}

void Driver::initialize(){

  //objects
  //  _output = new Output("/data/user/cbora/practice/results.root", "DataBranch");
  _output = new Output(_output_file.c_str(), "DataBranch");

  _ice = new OpticalIce();
  CurvedRay::setOpticalIce(_ice);

  _current_event_number =0;
  _max_events =0;
}

void Driver::setupL0FileReader(L0FileReader *reader){

  _l0_reader = reader;
  _geometry = _l0_reader->getStationGeometry();
  _reader_type = Driver::L0;
  setMaxEvents();
}

void Driver::setupAraSimFileReader(AraSimFileReader *reader){

  _sim_reader = reader;
  _geometry = _sim_reader->getStationGeometry();
  _reader_type = Driver::ARASIM;
  setMaxEvents();
}

void Driver::setupEventGenerator(EventGenerator *generator){
  
  _generator = generator;
  _reader_type = Driver::GENERATOR;
  setMaxEvents();
}

void Driver::addCommand(string command){

  Parser::addCommand(command);
}

void Driver::loop(){
  //  _max_events = 3;
  for(int ievt=0; ievt<_max_events; ievt++){

    _current_event_number = ievt;

    loadEvent();
    processEvent(); 

    applyFilter(); // pass filter by default


    if(_bool_filter) { //check if filter passed 

      _output->fill();

    }

    _output->clear();
    _event_data=0;
  }

  _output->write();
}


void Driver::loadEvent(){

  _event_data = _output->getEventData();

  switch(_reader_type){

  case Driver::L0:
    _l0_reader->getNextEvent();
    _event_data->setData(_l0_reader->getChannelCollection(), _geometry, _ice);
    break;

  case Driver::ARASIM:
    _sim_reader->getNextEvent();
    _event_data->setData(_l0_reader->getChannelCollection(), _geometry, _ice);
    break;
   
  case Driver::GENERATOR:
    _generator->generateEvent();
    _event_data->setData(_l0_reader->getChannelCollection(), _geometry, _ice);
    break;

  default:
    break;
  }  
    
}


void Driver::processEvent(){
  
  int pol;
  int i;
  for(i=0; i< _config.size(); i++) { //loop through methods given in the configuration file

    switch(_config[i]->getPolType()){
      //check the delta flag
    case VPOL:
      pol = 0;
      getEventData()->getChannels().getVpolChannelCollection().applyTimeDeltaFinder(_config[i]->getTimeType(), _config[i]->getDeltaType(), _config[i]->getTimeFinderParameters()[0], _config[i]->getTimeFinderParameters()[1]);
      break;
    
    case HPOL:
      pol = 1;
      getEventData()->getChannels().getVpolChannelCollection().applyTimeDeltaFinder(_config[i]->getTimeType(), _config[i]->getDeltaType(), _config[i]->getTimeFinderParameters()[0], _config[i]->getTimeFinderParameters()[1]);
      break;

    case BOTH:
      pol = 2;
      //for both vpol
      getEventData()->getChannels().getVpolChannelCollection().applyTimeDeltaFinder(_config[i]->getTimeType(), _config[i]->getDeltaType(), _config[i]->getTimeFinderParameters()[0], _config[i]->getTimeFinderParameters()[1]);

      getEventData()->getChannels().getHpolChannelCollection().applyTimeDeltaFinder(_config[i]->getTimeType(), _config[i]->getDeltaType(), _config[i]->getTimeFinderParameters()[0], _config[i]->getTimeFinderParameters()[1]);
      break;

    default:
      pol = 0;
      break;
    }
    std::cout << __PRETTY_FUNCTION__ << " pol "<< pol << std::endl;
    pol = 0;
    
    //for each vertex, in a given reco
    for(int j=0; j<_config[i]->getVertices().size(); j++){

      getEventData()->addVertexFinder(pol,_config[i]->getVertices()[j]->getVertexType(), _config[i]->getVertices()[j]->getCoordinateType(), _config[i]->getDeltaType(), _config[i]->getVertices()[j]->getLockType(), _config[i]->getVertices()[j]->getParameter1(), _config[i]->getVertices()[j]->getParameter2(), _config[i]->getVertices()[j]->getParameter3());
    
    //if method does not have previous finder, just add the methods and if its not the last in the array of finders
      if(_config[i]->getVertices()[j]->getPreviousFinder() == 0 && _config[i]->getVertices().size()-1 != j){
	continue;
      }
      getEventData()->findVertex(pol);
    }
  }
}

StationGeometry *Driver::getStationGeometry(){

  return _geometry;
}

OpticalIce *Driver::getOpticalIce(){

  return _ice;
}

void Driver::setOpticalIce(OpticalIce *ice){

  if(_ice) delete _ice;
  _ice = new OpticalIce(*ice);
  CurvedRay::setOpticalIce(_ice);
}

void Driver::setStationGeometry(StationGeometry *geom){

  if(_geometry) delete _geometry;
  _geometry = new StationGeometry(*geom);
}

void Driver::printout(){
  
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  Parser::printout();
}

EventData *Driver::getEventData(){

  return _event_data;
}

void Driver::clearUnneeded(){

  //clear histograms
  


  //clear waveforms
  
}

void Driver::setMaxEvents(int max){

  if(_max_events == -1){ // user did not specify max events
    
    switch(_reader_type){
      
    case Driver::L0:
	_max_events = _l0_reader->getTotalNumEvents();
	break;
	
    case Driver::ARASIM:
      _max_events= _sim_reader->getTotalNumEvents();
      break;
    case Driver::GENERATOR:
      ///      _max_events = 
      break;
    default:
      break;
    }
  }
  std::cout<< _max_events <<" \t "<< __PRETTY_FUNCTION__ << std::endl;
}


void Driver::setFilterType(int filter, int pol){

  _filter_type=filter;
  _filter_pol=pol;
}

void Driver::applyFilter(){

  Filter *filter=0;

  switch(_filter_type){
    
  case Filter::PASS:
    filter = new PassFilter(_event_data);
    break;

  case Filter::RANDOM:
    filter = new RandomFilter(_event_data);
    break;

  case Filter::SIMPLE:
    filter = new SimpleFilter(_event_data, _filter_pol);
    break;

  default:
    filter = new PassFilter(_event_data);
    break;
  }

  _bool_filter = filter->pass();
  
  if(filter) delete filter;
  filter=0;
  
}

int Driver::_filter_type=0;
int Driver::_filter_pol=0;
