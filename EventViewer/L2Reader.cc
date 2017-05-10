#include "L2Reader.h"

L2Reader::L2Reader(string input, string branchName, string treeName){


  setInputFile(input);
  setBranchName(branchName);
  setTreeName(treeName);
  initialize();
}

L2Reader::~L2Reader(){

  _tree=0;
  _file->Close();
}

void L2Reader::initialize(){

  _file=0;
  _tree=0;
  
  _current_event=-1;
  _event_data =0;
  
  open();
}

void L2Reader::open(){

  
  _file = new TFile(_file_name.c_str());
  _tree = (TTree*)_file->Get("tree"); // tree name
  
  _tree->SetBranchAddress(_branch_name.c_str(), &_event_data);
  _num_entries = _tree->GetEntries();
}

void L2Reader::clear(){

  //  if(_current_event<
}

bool L2Reader::next(){

  clear();
  _current_event++;

  if(_current_event < _num_entries){
    processEvent();
    
    return true;
  }
  else 
    return false;
}

void L2Reader::processEvent(){
    
  _tree->GetEntry(_current_event);
}

EventData *L2Reader::getEventData(){

  return _event_data;
}

void L2Reader::setInputFile(string fileName){
  
  _file_name = fileName;
}

void L2Reader::setTreeName(string name){

  _tree_name = name;
}

void L2Reader::setBranchName(string name){

  _branch_name = name;
}


TTree *L2Reader::getTree(){

  return _tree;
}
