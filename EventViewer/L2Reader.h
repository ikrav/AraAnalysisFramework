#ifndef L2READER_H_
#define L2READER_H_


#include "EventData.h"
//#include "MethodPlot.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>

using std::string;

class L2Reader {

public:
 
  L2Reader(string input="results.root", string branchName="DataBranch", string treeName="tree");
  virtual ~L2Reader();
  
  
  void initialize();
  
  void readTree();

  void open();

  void setInputFile(string name);
  void setBranchName(string name);
  void setTreeName(string name);

  void clear();
  void processEvent();

  bool next();
  
  EventData *getEventData();
  TTree *getTree();

protected:
  
  TTree *_tree;
  TFile *_file;
  
  EventData *_event_data;
  

  string _file_name;
  string _branch_name;
  string _tree_name;

  int _current_event;
  int _num_entries;
};

#endif


