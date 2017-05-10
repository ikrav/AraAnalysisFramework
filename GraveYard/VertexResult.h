#ifndef VERTEXRESULT_H
#define VERTEXRESULT_H

#include "CppIncludes.h"
#include "VertexPos.h"
#include "VertexFinder.h"


class VertexResult {

public:
  
  VertexResult();
  VertexResult(int polarization, int time_input_type, int time_finder_type, double par1=0, double par2=0, std::vector<int> channel_mask=std::vector<int>());
  VertexResult(const VertexResult &other);
  virtual ~VertexResult();
  
  void recursiveDelete();
  
  void initialize();
  void printout();
  void printRecursive();
  void help();
  
  // commands:
  VertexPos *calculate(ChannelCollection channels, VertexPos *starting_pos=0); // run this finder, run sisters' finders, combine, and give result to daughter, return the daughter's results after it did calculate() itself (recursively). 
  
  // getters
  int getTimeInput() const;
  std::string getTimeInputString() const; 
  int getTimeFinderType() const;
  std::string getTimeFinderTypeString() const;
  double getTimeParameter1() const;
  double getTimeParameter2() const;
  int getPolarization() const;
  std::string getPolString() const; 
  std::vector<int> getChannelMask() const;
  
  VertexFinder *getVertexFinder() const;
  VertexPos *getThisPos() const;
  VertexPos *getStartingPos() const;
  
  std::vector<VertexResult*> getDaughterResultsVector() const;
  int getNumDaughters() const;
  VertexResult *getDaughterResult(int which_daughter=0) const;
  std::vector<VertexResult*> getSisterResultsVector() const;
  int getNumSisters() const;
  VertexResult *getSisterResult(int which_sister=0) const;
  
  int getCombineResultsOption() const; 
  
  // time analysis choices:
  void chooseTimeInput(int which_time_input=0);
  void chooseTimeFinderType(int finder_type=0);
  void setInputTimeParameters(double par1=0, double par2=0);
  void setInputTimeChoices(int which_time_input, int which_finder_type, double par1=0, double par2=0);
  void setPolarization(int pol); // choose to mask channels (ALL=0, VPOL=1, HPOL=2);
  void setChannelMask(std::vector<int> channel_mask);
  void addToChannelMask(int channel_id);
  
  // fill the objects inside this
  void setVertexFinder(VertexFinder *finder);
  // TODO: add methods to generate the finder internally (given parameters).
  void addDaughterResult(VertexResult *daughter=0); // any null pointer is filled with a default VertexResult with mother's parameters. non-null pointers make a deep copy! 
  void addDaughterResultsVector(std::vector<VertexResult*> daughters);
  void addSisterResult(VertexResult *sister=0);// any null pointer is filled with a default VertexResult with elder sister's parameters. non-null pointers make a deep copy! 
  void addSisterResultsVector(std::vector<VertexResult*> sisters);
  void setCombineResultsOption(int combine_results_option); // can be AVERAGE=0, BEST=1 etc... 
  
  // set positions
  void setStartingPos(VertexPos *position);
  void setThisPos(VertexPos *position);
  
  enum TimeInput { TIMES=0, DELTAS};
  enum Polarization {ALL=0, VPOL, HPOL};
  
  static int debug_bit;
  
protected:
  
  // time analysis choices (these are copied from the mother result, but can be altered.
  int _which_time_input;// to use TimeFinder, DeltaFinder, or other.
  int _which_time_finder;// can be any of the TimeFinder or DeltaFinder sub-types.
  double _time_par1; // parameter for the time input.
  double _time_par2; // parameter for the time input.
  int _use_polarization; // to mask off some of the channels. 
  std::vector<int> _channel_mask;// to mask off any channels you want. 
  
  VertexFinder *_finder; // can be null, in which case use only daughter/sister's results. 
    
  // VertexPos results
  VertexPos *_starting_pos;
  VertexPos *_this_pos;
  std::vector<VertexPos*> _temp_pos_results; // this vector hold temporary return values from the sisters, then they are combined into one result... 
  int _combine_results; // can be AVERAGE=0 or BEST=1

  void clear_temp_results();// delete the VErtexPos objects and clear the vectors
  
  // other VertexResult objects connected to this one
  std::vector<VertexResult*> _sisters;
  std::vector<VertexResult*> _daughters;
  
  static const std::string name_time_input(int index);
  static const std::string name_polarization(int index);
  
};

#endif