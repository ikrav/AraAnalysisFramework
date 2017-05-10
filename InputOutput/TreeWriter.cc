#include "TreeWriter.h"
#include "VertexPos.h"

TreeWriter::TreeWriter(std::string filename, std::string treename) : _file_name(filename), _tree_name(treename) {
  
  initialize();
  
}

TreeWriter::~TreeWriter(){

  close();  
  delete _data;
  
}

void TreeWriter::close(){

  if(_output_tree){
   
//     delete _output_tree;
    _output_tree=0;
    
  }
  
  if(_output_file){
  
    _output_file->Close();
//     delete _output_file;
    _output_file = 0;
    
  }
  
  _event_number=0; // make sure you can now reset() again
  
}

void TreeWriter::reset(){

  delete _data;
  
  _data=new EventData(_combine_method);
  
}

void TreeWriter::clearChannels(){
    
  _data->clearChannels();
  
}

void TreeWriter::clearFinders(){
 
  _data->clearFinders();
  
}

void TreeWriter::initialize(){
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  _save_channels_bit=1;
  _save_finders_bit=1;
  _save_vertex_bit=1;
  _save_housekeeping_bit=1;
  
  _write_mode = OBJECT; // by default write the tree of objects
  
  _combine_method;
  
  _data=new EventData(_combine_method);
    
  _output_file=new TFile(_file_name.c_str(), "RECREATE");
  _output_tree=new TTree(_tree_name.c_str(), "reconstruction results");
  _output_tree->Branch("data","EventData",&_data, 4000,4);

  _event_number=0;

  TH1::AddDirectory(kFALSE);

}

void TreeWriter::printout() const {
  
  std::cout<<"Number of events written: " << _event_number<<" combine method= "<<VertexPos::name_choose_result(_combine_method)<<std::endl;
  if(_data) _data->printout();
  
}

// getters
std::string TreeWriter::getFileName() const{

  return _file_name;
  
}

std::string TreeWriter::getTreeName() const {
 
  return _tree_name;
  
}

int TreeWriter::getSaveChannelsBit() const {
  
  return _save_channels_bit;
  
}

int TreeWriter::getSaveFindersBit() const {
  
  return _save_finders_bit;
  
}

int TreeWriter::getSaveVertexBit() const {
  
  return _save_vertex_bit;
  
}

int TreeWriter::getSaveHousekeepingBit() const {
  
  return _save_housekeeping_bit;
  
}

VertexFinder *TreeWriter::getFinder(int index) const {
 
  return _data->getFinder(index);
  
}

std::vector<VertexFinder*> TreeWriter::getFindersVector() const {
  
  return _data->getFindersVector();
  
}

VertexPos *TreeWriter::getFinalPosition(){
  
  return _data->getFinalPosition();
  
}

L2Data *TreeWriter::getHousekeepingData() const {
  
  return _data->getHousekeeping();
  
}

SimData *TreeWriter::getSimulationData() const {
  
  return _data->getSimData();
  
}

EventData *TreeWriter::getEventData() const {
  
  return _data;
  
}

// setters
void TreeWriter::setSaveChannelsBit(int bit){
  
  _save_channels_bit = bit;
  
}

void TreeWriter::setSaveFindersBit(int bit){
  
  _save_finders_bit = bit;
  
}

void TreeWriter::setSaveVertexBit(int bit){
  
  _save_vertex_bit = bit;
  
}

void TreeWriter::setSaveHousekeepingBit(int bit){
  
  _save_housekeeping_bit = bit;
  
}

void TreeWriter::setWriteMode(int write_mode){
 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet..."<<std::endl;
  
}

void TreeWriter::setChannels(ChannelCollection channels){
 
  _data->setChannels(channels);
  
}

void TreeWriter::addFinder(VertexFinder *finder){
 
  if(finder==0){
   
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: the finder given is empty..."<<std::endl;
    return;
  }
 
  _data->addFinder(finder);
   
}

void TreeWriter::fill(){
 
  _data->calculate();
  
  _output_tree->Fill();
  
  reset();
  
  _event_number++;
  
}

void TreeWriter::write(){
  
  if(_output_file) _output_file->Write(0, TObject::kOverwrite);
  
}
