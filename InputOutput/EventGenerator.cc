#include "EventGenerator.h"

ClassImp(EventGenerator);
  
EventGenerator::EventGenerator(){ 
  
//   std::cout<<"EventGenerator default constructor."<<std::endl;
  
  initialize(); 
  
}

EventGenerator::EventGenerator(const EventGenerator &other){
 
  initialize();
  
  setXrange(other.getXmin(), other.getXmax());
  setYrange(other.getYmin(), other.getYmax());
  setZrange(other.getZmin(), other.getZmax());
  
  setVertexRadius(other.getRmin(), other.getRmax());
  
  setAmplitudeLog(other.getAmplitudeLog(), other.getAmpSpread(), other.hasAmpDecay());
  setPolarizationRange(other.getMinPolarization(), other.getMaxPolarization());
  setSignalWidth(other.getSignalWidth());
  setNumSignalChannels(other.getNumSignalChannels());
  
}

EventGenerator::~EventGenerator(){ 
 
  deleteChannels(); // note this deletes the copies of the channels... 
  
  if(_rand) { delete _rand;  _rand=0; }
  if(_geom) { delete _geom;  _geom=0; }
//   if(_ice)  { delete _ice;   _ice=0;  }
  if(_data){ delete _data; _data=0;}
  
}

void EventGenerator::initialize(){
  
//   std::cout<<"EventGenerator::initialize()"<<std::endl;
  
  _data=0;
  _ice=0;
  _geom=0;
  _rand=0;
  
  reset();
  
  _rand=new TRandom3(0);
  setStationGeometry(10, 15);
  _ice=new OpticalIce(1.76, 0, 0, 1.4, 170);
   
  calculateConversionFactor();
  
  CurvedRay::setOpticalIce(_ice);
  
  generateEvent();
  
  _current_event_num=0;
  
}

void EventGenerator::reset(){
 
//   std::cout<<"EventGenerator::reset()"<<std::endl;
  
  _num_points_waveform=512;// number of bins
  _step_size=0.5; // nanoseconds per bin
  
  _numEvents=0;
  
  _min_x_pos=-3000;
  _max_x_pos= 3000;
  
  _min_y_pos=-3000;
  _max_y_pos= 3000;
  
  _min_z_pos=-3000;
  _max_z_pos= 0;
  
  _min_radius=0;
  _max_radius=1e9;
  
  
  _build_cherenkov_bit=0;
  _min_energy_log=16;
  _max_energy_log=20;
  
  _theta_central_value=180; // straight down
  _theta_spread_width=90; // on either side
  
  _phi_central_value=0;
  _phi_spread_width=180;
  
  _use_decaying_amplitude_bit=0;// if 0, use amplitude at station center, if 1, use amp at source
  _signal_amplitude_log=2.5;
  _signal_amplitude_spread=0.1;
  
  _signal_envelope_width=4; // in nanoseconds 
  _noise_RMS=40;
  
  // set both to 0.5 to get unpolarized amplitude at all antennas
  _min_polarization_vertical=0.5;
  _max_polarization_vertical=0.5;
  
  _use_curved_raytrace_bit=0;
  
  _num_signal_channels=3;
  
}

void EventGenerator::calculateConversionFactor(){
 
  double Amp=1e4; // 80 dB amplification factor
  double length=1; // antenna length in m (to convert from electric field to volts)
  double unit_conversion=1e-3*1e-12; // to go from V to mV and from TeV to eV
  double typical_frequency=1000; // in MHz
  double Azero= 2.53e-7; // empirical constant
  double delta= 1.44; // empirical constant
  double epsilon= 0.5;
  double max_freq=850; // antenna frequency in MHz
  double min_freq=150; // antenna frequency in MHz
  
  
  int numPoints=1e6;
  double step_size=(max_freq-min_freq)/(numPoints);
   
  double integral=0;
  
  for(int i=0; i<numPoints; i++){
   
    double freq=min_freq+i*step_size;
    
    integral+=freq/(1+0.4*std::pow(freq, delta))*step_size;
    
  }
  
  _conversion_factor=Amp*length*unit_conversion*typical_frequency*Azero*epsilon*integral;
  
}

void EventGenerator::printout(){
 
  std::cout<<"simulation parameters:\n x:["<<_min_x_pos<<":"<<_max_x_pos<<"]  y:["<<_min_y_pos<<":"<<_max_y_pos<<"]  z:["<<_min_z_pos<<":"<<_max_z_pos<<"]"<<std::endl;
  std::cout<<"signal: 10^"<<_signal_amplitude_log<<" +- "<<_signal_amplitude_spread<<"  width= "<<_signal_envelope_width<<"ns  noise RMS= "<<_noise_RMS<<"mV"<<std::endl<<std::endl;
  
  _data->printout();

}

// getter methods
int EventGenerator::getCurrentEventNum() const {
 
  return _current_event_num;
  
}

int EventGenerator::getMaxNumEvents() const {
  
  return _max_num_events;
  
}

Channel **EventGenerator::getChannelArray() {
 
  Channel **array=new Channel*[_channels.size()];
  
  for(int i=0;i<_channels.size();i++) array[i]=_channels[i];
  
  return array;
  
}

ChannelCollection EventGenerator::getChannelCollection() { // const {
 
  return ChannelCollection(_channels);
  
}

double EventGenerator::getAmpAtSource(double amp_at_station, Pos source_pos){
   
  CurvedRay::setOpticalIce(_ice);
  CurvedRay::useCurvedRays(_use_curved_raytrace_bit);
  
  CurvedRay ray(Pos::getOrigin(), source_pos);
  
  return amp_at_station*ray.getDistance()/ray.getAttenuation();
  
}

double EventGenerator::getEnergyFromAmplitdue(double amplitude){

  return amplitude/_conversion_factor;

}

double EventGenerator::getAmplitudeFromEnergy(double energy_eV){
  
  return energy_eV*_conversion_factor;
  
}

StationGeometry *EventGenerator::getGeometry(){ 
  
  return _geom; 
  
}

StationGeometry *EventGenerator::getStationGeometry(){ 
  
  return getGeometry();
  
}
  
OpticalIce *EventGenerator::getOpticalIce(){ 
  
  return _ice; 
  
}

SimData *EventGenerator::getSimData(){ 
  
  return _data; 
  
}

Pos EventGenerator::getSourcePosition() const {
  
  return *((Pos*)_data);
  
}

Pos EventGenerator::getRealPosition() const {
  
  return getSourcePosition();
  
}

double EventGenerator::getXmin() const {
  
  return _min_x_pos;
  
}

double EventGenerator::getXmax() const {
  
  return _max_x_pos;
  
}

double EventGenerator::getYmin() const {
  
  return _min_y_pos;
  
}

double EventGenerator::getYmax() const {
  
  return _max_y_pos;
  
}

double EventGenerator::getZmin() const {
  
  return _min_z_pos;
  
}

double EventGenerator::getZmax() const {
  
  return _max_z_pos;
  
}

double EventGenerator::getRmin() const {
  
  return _min_radius;
  
}

double EventGenerator::getRmax() const {
  
  return _max_radius;
  
}

double EventGenerator::getAmplitudeLog() const {
  
  return _signal_amplitude_log;
  
}

double EventGenerator::getAmpSpread() const {
  
  return _signal_amplitude_spread;
  
}

double EventGenerator::hasAmpDecay() const {
  
  return _use_decaying_amplitude_bit;
  
}

double EventGenerator::getMinPolarization() const {
  
  return _min_polarization_vertical;
  
}

double EventGenerator::getMaxPolarization() const {
  
  return _max_polarization_vertical;
  
}

double EventGenerator::getSignalWidth() const {
  
  return _signal_envelope_width;
  
}

int EventGenerator::getNumSignalChannels() const {
  
  return _num_signal_channels;
  
}

int EventGenerator::getTotalNumEvents() const {
 
  return getMaxNumEvents();
  
}

// setter methods
void EventGenerator::setMaxNumEvents(int num_events){
 
  _max_num_events = num_events;
  
}

void EventGenerator::setCurrentEventNum(int event_number){
  
  _current_event_num = event_number;
  
}

void EventGenerator::setStationGeometry(double string_radius, double vertical_separation){
  
  if(_geom) delete _geom;
  
  _geom=new StationGeometry(string_radius, vertical_separation);
  
}

void EventGenerator::setXrange(double min_x, double max_x){

  if(min_x>max_x) max_x=min_x;
  
  _min_x_pos=min_x; 
  _max_x_pos=max_x;
  
}

void EventGenerator::setYrange(double min_y, double max_y){

  if(min_y>max_y) max_y=min_y;
  
  _min_y_pos=min_y; 
  _max_y_pos=max_y;
  
}

void EventGenerator::setZrange(double min_z, double max_z){

  if(min_z>max_z) max_z=min_z;
  
  _min_z_pos=min_z; 
  _max_z_pos=max_z;
  
}

void EventGenerator::setVertexRadius(double min_radius, double max_radius){
 
  if(max_radius>0&&min_radius>max_radius) max_radius=min_radius;
  
  _min_radius=min_radius;
  _max_radius=max_radius;
  
}

void EventGenerator::setAmplitudeLog(double amplitude_log, double amp_spread_log, int amp_decay){ 
  
  _signal_amplitude_log=amplitude_log; 
  _signal_amplitude_spread=amp_spread_log; 
  _use_decaying_amplitude_bit=amp_decay; 
  
}
 
void EventGenerator::setPolarizationRange(double min_vertical, double max_vertical){
 
  min_vertical=fabs(min_vertical);
   
  if(min_vertical>max_vertical) max_vertical=min_vertical;
  
  _min_polarization_vertical=min_vertical; 
  _max_polarization_vertical=max_vertical;
  
}
  
void EventGenerator::setAttenuationDecaysBit(int att_bit_state){ 
  
  _use_decaying_amplitude_bit=att_bit_state; 
  
}

void EventGenerator::setSignalWidth(double width){
 
  _signal_envelope_width=width;
  
}

void EventGenerator::setNumSignalChannels(int numChannels){
 
  _num_signal_channels=numChannels;
  
}

void EventGenerator::deleteChannels(){
  
  for(int i=0; i<_channels.size();i++){
    
    if(_channels[i]) delete _channels[i];
    
  }
  
  _channels.clear();
  
}

// calculate and output methods
TCanvas *EventGenerator::drawWaveforms(){
  
  TCanvas *c1=new TCanvas();
  c1->Divide(4,4,0,0);
  
  
  // find the highest/lowest point for all graphs.
  double min[4]={0};
  double max[4]={0};
    
  for(int ch=0;ch<_channels.size();ch++){
    
    double *v=_channels[ch]->getWaveform()->GetY();
    
    for(int i=0;i<_channels[ch]->getWaveform()->GetN(); i++){
      
      if(v[i]<min[ch/4]) min[ch/4]=v[i];
      if(v[i]>max[ch/4]) max[ch/4]=v[i];
      
    }// for i
    
  }// for ch
  
  
  for(int ch=0;ch<_channels.size();ch++){
   
    c1->cd(ch+1);
    _channels[ch]->getWaveform()->SetMaximum(max[ch/4]*1.1);
    _channels[ch]->getWaveform()->SetMinimum(min[ch/4]*1.1);
    
    _channels[ch]->getWaveform()->Draw("al");
    
  }
  
  return c1;
  
}

// NOTE: this can be restored by replacing EventData with SimData 
// void EventGenerator::writeEvent(){
// 
//   EventData event(getChannelCollection(), getStationGeometry(), getOpticalIce());
//   
//   event.setSimData(getSimData());
//   event.SetNameTitle(Form("event%d", _numEvents),Form("Real data about event number %d", _numEvents));
//   
//   event.Write();
//   
//   
// }

void EventGenerator::selectVertexPosition(){
  
  if(_min_radius>0 || _max_radius>0){// check minimal/maximal radius requirements can be met
    
    double furthest_x=_max_x_pos;
    double closest_x=_min_x_pos;
    if(fabs(_min_x_pos)>fabs(_max_x_pos)){ furthest_x=_min_x_pos; closest_x=_max_x_pos; }
    double furthest_y=_max_y_pos;
    double closest_y=_min_y_pos;
    if(fabs(_min_y_pos)>fabs(_max_y_pos)){ furthest_y=_min_y_pos; closest_y=_max_y_pos; }
    double furthest_z=_max_z_pos;
    double closest_z=_min_z_pos;
    if(fabs(_min_z_pos)>fabs(_max_z_pos)){ furthest_z=_min_z_pos; closest_z=_max_z_pos; }
    
    double furthest_distance=std::sqrt(std::pow(furthest_x,2)+std::pow(furthest_y,2)+std::pow(furthest_z, 2));
    double closest_distance=std::sqrt(std::pow(closest_x,2)+std::pow(closest_y,2)+std::pow(closest_z, 2));
    
    if(_min_radius>0&&_min_radius>furthest_distance){ std::cerr<<"ERROR: _min_radius= "<<_min_radius<<" is larger than min/max range.\n"; return; }
    if(_max_radius>0&&_max_radius<closest_distance){ std::cerr<<"ERROR: _max_radius= "<<_max_radius<<" is smaller than min/max range.\n"; return; }
    
    
  }
  
  
  
  if(_min_x_pos>=_max_x_pos) _data->setX(_min_x_pos);
  else _data->setX(_min_x_pos + (_max_x_pos - _min_x_pos)*_rand->Rndm());
  
  if(_min_y_pos>=_max_y_pos) _data->setY(_min_y_pos);
  else _data->setY(_min_y_pos + (_max_y_pos - _min_y_pos)*_rand->Rndm());
  
  if(_min_z_pos>=_max_z_pos) _data->setZ(_min_z_pos);
  else _data->setZ(_min_z_pos + (_max_z_pos - _min_z_pos)*_rand->Rndm());
  
  // still need to add the loop and check against radius
  
}

void EventGenerator::selectPolarizationRatio(){
    
  _polarization_ratio=_min_polarization_vertical + (_max_polarization_vertical - _min_polarization_vertical)*_rand->Rndm();
  _data->setPolarizationRatio(_polarization_ratio);
  
}

void EventGenerator::setSourceAmp(){// used internally by generateEvent()
 
  if(_use_decaying_amplitude_bit==0){
    
    Pos point(*_data);
    
    _data->setFieldAtSource(getAmpAtSource(std::pow(10,_signal_amplitude_log), point));
   
  }
  
  if(_use_decaying_amplitude_bit==1){
   
    _data->setFieldAtSource(std::pow(10, _signal_amplitude_log+_signal_amplitude_spread*(2*_rand->Rndm()-1)));
    
  }
  
}

void EventGenerator::generateEvent(int print_bit){

  if(_data){ delete _data; }
  deleteChannels();
  
  _data=new SimData();
  
  // select a vertex position in the ice 
  selectVertexPosition();
    
  // select the polarization of this signal:
  selectPolarizationRatio();
  
  // find the hit time for all channels, and attenuation
  
  // setup the amplitude at source.
  setSourceAmp();
  _data->setEnergy(getEnergyFromAmplitdue(_data->getFieldAtSource()));
  
  _channels.clear();
  _channels.reserve(_geom->getNumChan());
  
  _rays.clear();
  _rays.reserve(_geom->getNumChan());
  
  CurvedRay::setOpticalIce(_ice);
  CurvedRay::useCurvedRays(_use_curved_raytrace_bit);
  
  for(int ch=0;ch<_geom->getNumChan(); ch++){
        
    _rays.push_back(CurvedRay(_geom->getAntPos(ch), *_data));
    
//     std::cout<<"ch = "<<std::setw(2)<<ch<<"  hit time= "<<_rays.back().getHitTime()<<std::endl;
    
    _channels.push_back(new Channel(ch));
    _channels[ch]->setRealTime(_rays[ch].getHitTime());
    
    
  }// for ch
  
  // normalize hit times to 0-256 ns range
  int closest_index=0;
  double shortest_time=_channels[0]->getRealTime();
  
  for(int ch=1;ch<_channels.size(); ch++){ 
    
    if(_channels[ch]->getRealTime()<shortest_time){ 
      
      shortest_time=_channels[ch]->getRealTime(); 
      closest_index=ch; 
      
    }
    
  }
  
  std::vector<int> chooseVpol=selectAntennas(0);
  std::vector<int> chooseHpol=selectAntennas(1);
  
    
  for(int ch=0;ch<_geom->getNumChan(); ch++){
    
    _channels[ch]->setRealTime(_channels[ch]->getRealTime()-shortest_time+30);
    
    double amplitude;
    double polarization;// factor that multiplies the amplitude
    
    if(_geom->getPolarization(ch)) polarization=std::sin(PI/2*_polarization_ratio);
    else polarization=std::cos(PI/2*_polarization_ratio);
    
    if(_use_decaying_amplitude_bit==0){//  just set amplitude based on given values
     
      amplitude=polarization*std::pow(10,_signal_amplitude_log+_signal_amplitude_spread*(2*_rand->Rndm()-1)); // logarithmic variance

    }
    if(_use_decaying_amplitude_bit==1){// set amplitude at source and use attenuation and 1/r^2 law to get the value at the antennas

      amplitude=polarization*_data->getFieldAtSource()/_rays[ch].getDistance()*_rays[ch].getAttenuation();
      
    }
    
    TGraph *gr=new TGraph();
    gr->SetNameTitle(Form("WF%dC%02d", _numEvents, ch),Form("Simulated WF %d, ch %02d, A= %5.1lf mV, W= %5.1lf ns, t= %5.1lf ns; time [ns]; amplitude [mV];", _numEvents, ch, amplitude, _signal_envelope_width, _channels[ch]->getRealTime()));
        
    fillGraphWithNoise(gr, _noise_RMS);
    
    for(int i=0;i<chooseVpol.size();i++){ // only add signal if ch index matches the list in chooseVpol
      
      if(chooseVpol[i]==ch) addSignalToGraph(gr, _channels[ch]->getRealTime(), amplitude, _signal_envelope_width);
      if(chooseHpol[i]==ch) addSignalToGraph(gr, _channels[ch]->getRealTime(), amplitude, _signal_envelope_width);
    
    }
    
    _channels[ch]->setWaveform(gr);
    
    
  }// for ch
  
  _numEvents++;
  
  if(print_bit) printout();
  
}

int EventGenerator::loadEvent(int event_number){
 
  generateEvent();
  _current_event_num++;
  
}

bool EventGenerator::next(){

  std::cerr<<"NOT implemented yet!!!! "<< std::endl;
  return false;
}

void EventGenerator::fillGraphWithNoise(TGraph *gr, double RMS){
   
//   double mean_noise_level=std::sqrt(PI/2)*RMS;// sqrt(pi/2)*sigma is the rayleigh distribution mean
  
//   for(int i=0;i<_num_points_waveform;i++) gr->SetPoint(i, i*_step_size, (std::sqrt(fabs(rand->Gaus(0,RMS)*rand->Gaus(0,RMS)))-mean_noise_level));
  
  for(int i=0;i<_num_points_waveform;i++) gr->SetPoint(i, i*_step_size, _rand->Gaus(0,RMS)); // just make gaussian noise!
  
}

void EventGenerator::addSignalToGraph(TGraph *gr, double time, double amplitude, double width){

  int width_bins=width/_step_size;
  int central_bin=time/_step_size;

  if(central_bin<0) central_bin=0;
  if(central_bin>=gr->GetN()) central_bin=gr->GetN()-1;
  
  for(int j=0;j<2*width_bins;j++){

    int bin;
    double x,y;
    
    bin=central_bin-j;
      
    if(bin>=0){// verify we are still inside the array

      gr->GetPoint(bin, x, y);
      gr->SetPoint(bin, x, y+cos(FREQ*2*PI*(x-time))*amplitude*exp(-0.5*pow((x-time)/width,2)));// add gaussian envelope with cos modulation
      
    }
      
    if(j>0){// do above AND below only for j>0
	
      bin=central_bin+j;

      if(bin<gr->GetN()){// verify we are still inside the array

	gr->GetPoint(bin, x, y);
	gr->SetPoint(bin, x, y+cos(FREQ*2*PI*(x-time))*amplitude*exp(-0.5*pow((x-time)/width,2)));// add gaussian envelope with cos modulation
      
      }
      
    }
    

    
  }// for j
  
}

std::vector<int> EventGenerator::selectAntennas(int polarization){
  
  std::vector<int> chan;
  chan.reserve(_geom->getNumChan());
  
  for(int ch=0;ch<_geom->getNumChan(); ch++){
   
    if(_geom->getPolarization(ch)==polarization) chan.push_back(ch);
        
  }
  
  std::vector<int> choose;
  choose.reserve(_num_signal_channels);
  
  if(_num_signal_channels<=0.5*chan.size()){
   
    for(int i=0;i<10000;i++){
     
      int number=(int)(chan.size()*_rand->Rndm()); // choose random in interval and round down to nearest bin.
      
      if(chan[number]>-1){
	
	choose.push_back(chan[number]);// choose this channel
	chan[number]=-1;// no picking the same channel twice...
	
      }
      
      if(choose.size()>=_num_signal_channels) break;// quit when there's enough channels. 
      
    }// for i
    
  }
  
  else{
   
    int counter=chan.size()-_num_signal_channels; // number of channels to kick output
    
    for(int i=0;i<10000;i++){
      
      int number=(int)(chan.size()*_rand->Rndm()); // choose random in interval and round down to nearest bin.

      if(chan[number]>-1){
	
	chan[number]=-1;
	counter--;
	
      }
      
      if(counter==0) break; // once enough channels are kicked out... 
      
    }// for i
    
    for(int ch=0;ch<chan.size();ch++){// copy the remaining channels
     
      if(chan[ch]>-1) choose.push_back(chan[ch]);
      
    }
    
  }
  
  return choose;
  
}

const double EventGenerator::PI=3.14159265358979312;
const double EventGenerator::FREQ=1.2;// frequence in units of 1/ns
