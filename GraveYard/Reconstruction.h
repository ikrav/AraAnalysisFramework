#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include "CppIncludes.h"

// #include "Input.h"
#include "EventGenerator.h"
// #include "L0FileReader.h"
#include "FileReader.h"
#include "AraSimFileReader.h"
// #include "Output.h"
// #include "Parser.h"

#include "StationGeometry.h"
#include "OpticalIce.h"
#include "CurvedRay.h"

// #include "EventVertex.h"
#include "VertexFinder.h"
#include "VertexPos.h"

// #include "EventData.h"
#include "SimData.h"

class Reconstruction {
  
public:
  
  Reconstruction();
  Reconstruction(StationGeometry *geom, OpticalIce *ice);
  virtual ~Reconstruction();
  
  void initialize();
  void printout();
  void help();

  // getters
  // get the various objects stored in Reco
  StationGeometry *getStationGeometry() const;
  OpticalIce *getOpticalIce() const;
  EventGenerator *getEventGenerator() const;
//   L0FileReader *getL0FileReader() const;
  AraSimFileReader *getAraSimFileReader() const;
  
//   EventData *getEventData() const;
//   Output *getOutput() const;
//   Parser *getParser() const;
   
  // get information ditectly from reader
//   Input *getCurrentInput() const;
//   int getCurrentEventNum() const;
  int getMaxNumEvents() const;
//   int getTotalNumEvents() const;
  
  // setters
  // set the persistent classes
  void setStationGeometry(StationGeometry *geom);
  void setOpticalIce(OpticalIce *ice);
  
  // parser control
//   void setupParser(Parser *parser=0); // defualt 0 creates his own parser. 
  // TODO: add direct calls to the parser like read command etc.
  
  // setup inputs
  void setChannels(ChannelCollection channels); // if you have the channels from somewhere (don't use loadEvent())
//   void setupL0FileReader(L0FileReader *reader); // if you already initialized a file reader (use loadEvent() or loop())
//   void setupL0FileReader(string filenames);// Reco will create a file reader and feed it the filenames. (use loadEvent() or loop())
  void setupAraSimFileReader(AraSimFileReader *reader);// if you already initialized a file reader (use loadEvent() or loop())
  void setupAraSimFileReader(std::string filenames); // Reco will create a file reader and feed it the filenames. (use loadEvent() or loop())
  void setupAraSimFileReader(std::vector<std::string> filenames);
  void setupEventGenerator(EventGenerator *generator=0, int max_num_events=0);// if you already initialized a generator (use loadEvent() or loop()). if generator=0 Reco will create one. 
  void chooseInputType(int input_type); // if no such input (reader/generator) exists, it will create one... 
    
  // setup output
//   void setOutput(Output *output);// if you already initialized an Output object.
//   void setOutput(string filename, int write_mode=0);// specify an output filename, Reco will create an Output object
//   void setWriteMode(int write_mode);// specify the writing mode. 
  // TODO: add direct calls to Output to set what to save etc. 
  
  
  // control reader/generator
  int loadEvent(int event_number=-1); // loads from file/generator. use event_number if you don't want to use the current serial number
//   void processEvent(); // proceeds with processing.  use event_number if you don't want to use the current serial number
//   void loop(int max_num_events=-1); // loop over all events. max number is all events in file (or _max_num_events, if it is smaller/using generator)
  
  // set analysis parameters
  void setMaxNumEvents(int max_num_events);
//   void setCurrentEventNum(int event_number);
//   void useAveragePolResults(); // set result using average of polarizations
//   void useBestPolOrCombined(); // set result using the best reco of all options
    
  // operational / processing commands
  void applyTimeFinder(int finder_type=0, double par1=0, double par2=0);// apply time finder methods
  void applyDeltaFinder(int finder_type=0, double par1=0, double par2=0);// apply delta finder methods
  void addVertexFinder(int type=0, int coordinate_system=0, int time_input=0, int lock_parameter=0, double par1=0, double par2=0, double par3=0, int variant=0); // add vertex finders to the stack in each EventVertex object
  void runVertexFinder(); // run the finders that have been stacked to any of the EventVertex objects
  
  static void setAllDebugBits(int reco_debug=0, int data_debug=0, int readers_debug=0, int output_debug=0,
			      int time_finder_debug=0, int vertex_finder_debug=0, int parser_debug=0, 
			      int geometry_ice_debug=0, int channels_debug=0, int timing_debug=0, 
			      int position_debug=0, int ray_debug=0); // used to quickly setup the debug level for all classes. 
  
  static int debug_bit; // choose level of debugging output. default 0 no outputs. 
  
  enum Input_Type {NONE=0, DATA_READER, SIM_READER, GENERATOR}; // types of input/reader/generator for analysis
  static const std::string name_input_type(int index); // to output the enum as string. 
  
protected:
  
  StationGeometry *_geometry; // not copied into Reconstruction, not deleted on deconstruction
  OpticalIce *_ice; // not copied into Reconstruction, not deleted on deconstruction
  
//   L0FileReader *_data_reader; // are these going to be deep copied into reco??
  AraSimFileReader *_sim_reader; // are these going to be deep copied into reco??
  EventGenerator *_generator; // are these going to be deep copied into reco??
//   Output *_output;  
  
//   Parser *_parser;
  
//   EventData *_data;// created and destroyed by reco...
      
  int _which_input;// which input method used (data reader, sim reader, generator). default -1 gives an error. 
  int _max_num_events;
  
};

#endif