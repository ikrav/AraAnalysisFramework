#ifndef EVENTDATA_H
#define EVENTDATA_H

#include "CppIncludes.h"
#include "ChannelCollection.h"
#include "VertexFinder.h"
#include "VertexPos.h"
#include "L2Data.h"
#include "SimData.h"
#include "FileReader.h"
#include "SimFileReader.h"

class EventData {
  
public:
  
  EventData();
  EventData(int combine_method);
  EventData(const EventData &other);
  virtual ~EventData();
    
  void clearChannels();
  void clearFinders();
  void clearHousekeeping();
  void clearSimData();
  void clearFinalPosition();
  
  void initialize();
  void printout();

  void fill(FileReader *reader);
  void fill(SimFileReader *reader);

  // get methods
  int getCombineMethod() const;
  ChannelCollection getChannels() const;
  ChannelCollection getChannelCollection() const;
  VertexFinder *getFinder(int number=0) const;
  std::vector<VertexFinder*> getFindersVector() const;
  L2Data *getHousekeeping() const;
  SimData *getSimData() const;
  VertexPos *getFinalPosition();
    
  // setters
  void setCombineMethod(int method);
  void setChannels(ChannelCollection chans);
  void setChannelCollection(ChannelCollection chans);
  void addFinder(VertexFinder *finder);
  void setFindersVector(std::vector<VertexFinder*> finders);
  void setHousekeeping(L2Data *data);
  void setSimData(SimData *data);
  
  void calculate();
    
protected:
      
  int _combine_method;
  int _is_simulated; 
  
  ChannelCollection _channels; // these are NOT deep copies
  
  std::vector<VertexFinder*> _finders_vector; // these are NOT deep copies
  
  VertexPos *_final_position; // this is a deep copy, created by averaging/selecting the best finder
      
  L2Data *_housekeeping_data; // these are NOT deep copies
  SimData *_simulation_data; // these are NOT deep copies
  
  ClassDef(EventData,1);
  
};

#endif
