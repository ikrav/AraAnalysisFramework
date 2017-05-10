#include "Output.h"

Output::Output(string fileName, string branchName){

  setFileName(fileName);
  setBranchName(branchName);
  initialize();
}

Output::~Output(){
 
 if(_data) delete _data;
  _data =0;
  
  if(_output_tree) delete _output_tree; _output_tree=0;
  _output_file->Close();
}

void Output::initialize(){

  _data =0;
  _output_tree =0;
  _output_file =0;

  _event_number = -1;

  _output_file = new TFile(_output_file_name.c_str(), "RECREATE");

  if(_data == 0) _data = new EventData();

  _output_tree = new TTree("tree", "reco results tree");
  _output_tree->Branch(_branch_name.c_str(), "EventData", &_data, 4000, 4);

  TH1::AddDirectory(kFALSE);

  if(_data) delete _data;  _data =0;
}

void Output::printout(){

  std::cout <<"Number of events written to file: " << _event_number << std::endl;

}

void Output::write(){
  
  _output_file->Write(0, TObject::kOverwrite);
}

void Output::addBranch(TObject *object, string branchName, string className){

  _output_tree->Branch(branchName.c_str(), className.c_str(), &object, 4000, 99);
}

void Output::fill(){

  _event_number++;
  _output_tree->Fill();
}

void Output::clear(){

  if(_data) delete _data;

  _data=0;
}

string Output::getBranchName() const{
  
  return _branch_name;
}

string Output::getFileName() const{

  return _output_file_name;
}

EventData *Output::getEventData(){

  if(_data == 0) _data = new EventData();
  return _data;
}

void Output::setFileName(string name){
  
  _output_file_name = name;
}

void Output::setBranchName(string name){

  _branch_name = name;
}

void Output::setWriteMode(int write_mode){
 
  std::cerr<<__PRETTY_FUNCTION__<<" ERROR: this isn't implemented yet..."<<std::endl;
  
}

void Output::close(){

  _output_file->Close();
}
