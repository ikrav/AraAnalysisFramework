#ifndef TREEWRITER_H
#define TREEWRITER_H

#include "CppIncludes.h"

#include "TTree.h"
#include "TFile.h"

#include "EventData.h"

#include "SimData.h"
#include "L2Data.h"
#include "VertexPos.h"
#include "VertexFinder.h"

////////////////////////////////////////////
//                                        //
// this class was adapted from Output     //
// class by Christian Bora, its job is	   //
// to save an EventData object		   //
//                                        //
//	questions: guy.nir@weizmann.ac.il //
//                                        //
////////////////////////////////////////////

class TreeWriter {

public:
  
  TreeWriter(std::string filename="results.root", std::string branchname="reco_results");
  virtual ~TreeWriter(); // in case someone extends it
  
  void close();  // closes file
  void reset(); // clears the EventData object and makes a new one... 
  void clearChannels();
  void clearFinders();
  
  void initialize();
  void printout() const;
  
  // getters
  std::string getFileName() const;
  std::string getTreeName() const;
  
  int getSaveChannelsBit() const;
  int getSaveVertexBit() const;
  int getSaveFindersBit() const;
  int getSaveHousekeepingBit() const;
  
  //int getEventNumber() const; //This method was introduced but has never been implemented
  
  VertexFinder *getFinder(int index=0) const;
  std::vector<VertexFinder*> getFindersVector() const;
  VertexPos *getFinalPosition();
  L2Data *getHousekeepingData() const;
  SimData *getSimulationData() const;
  EventData *getEventData() const;
  
  // setters
  //void setFileName(std::string name); //This method was introduced but has never been implemented
  //void setBranchName(std::string name); //This method was introduced but has never been implemented
    
  void setSaveChannelsBit(int bit);
  void setSaveFindersBit(int bit);
  void setSaveVertexBit(int bit);
  void setSaveHousekeepingBit(int bit);
  
  void setWriteMode(int write_mode);

  void setChannels(ChannelCollection channels);
  void addFinder(VertexFinder *finder);
  //void setFindersVector(std::vector<VertexFinder*> finders); //This method was introduced but has never been implemented
  //void setHousekeeping(L2Data *data); //This method was introduced but has never been implemented
  //void setSimData(SimData *data); //This method was introduced but has never been implemented
  
  void fill(); // fills the tree and calls clear()
  void write();  // writes event to file
  
  enum WriteMode {OBJECT=0, FLAT, TEXT};
  
protected:
  
  void calculateFinalPosition();
  
  TTree *_output_tree;
  TFile *_output_file;
  
  const std::string _file_name;
  const std::string _tree_name;
  
  int _event_number;
  
  int _write_mode;
  
  int _save_channels_bit;
  int _save_finders_bit;
  int _save_vertex_bit;
  int _save_housekeeping_bit;

  int _combine_method;
  
  EventData *_data;
  
};

#endif


