#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "TTree.h"
#include "TFile.h"
#include "EventData.h"

class Output {

public :
  Output(string fileName="results.root", string branchName="DataBranch");
  virtual ~Output(); // in case someone extends it

  void write();  // writes event to file
  
  string getBranchName() const;
  string getFileName() const;
  EventData *getEventData(); // makes new event data before returning it.

  void fill(); // fills, deletes event data, and creates a new eventData
  void clear(); // deletes eventData
  
  void setBranchName(string name);
  void setFileName(string name);
  void addBranch(TObject *object, string branchName, string className); // can add customized branch and tree will fill the, user is responsible to delete object and make new one
  
  void setWriteMode(int write_mode);
  
  void close();  // closes file, //Deconstructor does the same too
  void printout();
  
protected:

  TTree *_output_tree;
  TFile *_output_file;
  string _output_file_name;
  string _branch_name;  // branch name for EventData
  
  int _event_number;

  EventData *_data;
  
  void initialize();
};

#endif


