#ifndef VERTEXFINDER_H
#define VERTEXFINDER_H

////////////////////////////////////////////////////////////////
//								//
//	Finder class that locates nu interaction vertex.	//
//	Base class only, used a interface for other Finders	//
//								//
//	send any questions to: guy.nir.@weizmann.ac.il		//
//								//
////////////////////////////////////////////////////////////////

/**
   VertexFinder class
   VertexFinder.cc
   Purpose: Base class for all vertex finder methods

   @author Guy Nir
   @version 0.1
*/
#include "CppIncludes.h"
#include "RootIncludes.h"

#include "CurvedRay.h"
#include "OpticalIce.h"

#include "Channel.h"
#include "ChannelCollection.h"
#include "TimingMatrix.h"
#include "Pos.h"
#include "VertexPos.h"
#include "CorrGraph.h"
#include "THX.h"

class VertexFinder : public VertexPos {

public:
  
//   VertexFinder();
  VertexFinder(int coordinate_system=0, int time_input=0, int lock_parameter=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0, int variant=0);
  VertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameter=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0, int variant=0);
  VertexFinder(const std::string commands);
  VertexFinder(ChannelCollection channels, VertexPos *initial_position, std::string commands);
  VertexFinder(const VertexFinder &other);
  virtual ~VertexFinder();
  
  virtual void reset(); // get ready for new channels and initial_position and setup for new call to findVertex
  virtual void printout() const;
  virtual void printout(int coordinate_system) const;
  virtual void printTitle() const;
  virtual void printPos(int coordinate_system) const;
  static void help();
  void parse(const std::string commands);
  
  virtual VertexFinder *getDeepCopy()=0;
  
  virtual std::string getFinderName() const;
  int isSpherical() const;
  int isCartesian() const;
  
  int getCoordinateSystem() const;
  int getLockParameters() const;
  
  int isUsingHitTimes() const;
  int isUsingPairDelays() const;
  int getInputTimeType() const;
  
  int hasFillHist() const;
  int hasExecuted() const;
  
  double getParameter1() const;
  double getParameter2() const;
  double getParameter3() const;

  inline int getStatus() {return _status;}
  
  int getVariant() const;
   
  VertexPos *getVertexPos() const;
  VertexPos *getInitialPosition() const;
  ChannelCollection getChannels() const;
  VertexFinder *getPreviousFinder() const;
  
  int getDegFreedom() const;

  THX  *getHistXD() const;
  
  int checkIfSomethingMissing();
  
  void setPreviousFinder(VertexFinder *previousFinder);
  
  // setters...
  void setChannels(ChannelCollection channels);
  void setInitialPosition(VertexPos *position);
  void setInitialPosition(VertexPos position);
  void setFoundPosition(VertexPos *position);
  void setFoundPosition(VertexPos position);
    
  void setStatusFound();
  void setStatusFailed();
  void setUndefinedSolution();
  
  virtual void setCoordinates(int coordinates);
  virtual void setCoordinates(const std::string input);
  virtual void setTimeInputs(int times);
  virtual void setTimeInputs(const std::string input);
  virtual void setLockedParameter(int par_number);
  virtual void setLockedParameter(const std::string input);
  virtual void setUnlockedParameter(int par_number);
  virtual void setUnlockedParameter(const std::string input);
  virtual void setHistFill(int fill);
  virtual void setHistFill(const std::string input);
  
  // calculation methods
  virtual void findVertex();
  void findVertex(ChannelCollection channels, VertexPos initial_position);
  virtual void findVertex(ChannelCollection channels, VertexPos *initial_position=0);
  void findVertex(VertexPos *initial_position);
  
  virtual double getInteractionTime(Pos position);///<brief * @param source position. get hit times between antenna position
  virtual double estimateErrorR() const;
  virtual double estimateErrorAngles() const;
  
  virtual double myChi(Pos position, double time);// use hit times + the time calculated from the best channel
  virtual double myChi(Pos position);// use time delays from pairs... 
  
  virtual double findInterference(CorrGraph *graph);
  virtual double findInterference(CorrGraph *graph, Pos source);
  virtual double findSumInterference(Pos source);
  
  enum Type {SCAN=0, MINUIT, ASM, MCMC, INTMAP, 
	     EARLIEST, ANYWHERE, UNLKUMINUIT,
	     REAL_VERTEX=999};
	     
  enum Coord {CARTESIAN=0, SPHERICAL};
  
  enum Input {TIMES=0, PAIRS};

  enum Status {READY=0, FOUND, 
	       FAILED=-1};
	       
  enum Lock {UNLOCK=0, LOCK0, LOCK1, LOCK2, LOCK01, LOCK02, LOCK12}; // use this flag to lock parameters in scan/minuit
  
  static const std::string name_vertex_type(int index);
  static const std::string name_coordinates(int index);
  static const std::string name_lock(int index);
  static const std::string name_vertex_status(int index);
  static const std::string name_input(int index);
  
  static int debug_bit;
  
  static bool check_str(const std::string input, const std::string compare);
  static bool check_str(const std::string input, const std::string compare1, const std::string compare2);
  static bool check_str(const std::string input, const std::string compare1, const std::string compare2, const std::string compare3);
  static bool check_str(const std::string input, const std::string compare1, const std::string compare2, const std::string compare3, const std::string compare4);
  static bool isdiv(const char input);
  static bool isletter(const char input);
  static std::vector< std::vector<std::string> > splitCommands(const std::string commands);
  static int stoi(std::string str);
  static double stof(std::string str);
  
protected:
   
  virtual void initialize();
  
  void updateLockedParameters(); // reads the _lock_parameters field and locks the _par_locked_bit[] accordingly
  
  virtual void calculateConfidence(double quality_parameter);
  
  static void minuitChiSquareOldMethod(int &npar, double *gin, double &f, double *par, int iflag);// the wrapper function for TFitter. actual chi2 calculation is done in myChi.
  static void minuitChiSquare(int &npar, double *gin, double &f, double *par, int iflag);
  static VertexFinder *_this_finder;// why do I need this static pointer to trampoline back to "this"? well... because minuitChiSquare is static so Minuit can get its void* pointer and myChi is non-static so...
  
  // these need to be input before (or during) a call to findVertex()
  ChannelCollection _channels;
  TimingMatrix _matrix;
  VertexPos *_initial_position;
  VertexFinder *_previous_finder;
    
  // setup parameters
  int _coordinate_system;
  int _time_input_type;
  int _lock_parameters;
  
  int _par_locked_bit[3];// if 1, then this parameter is locked to a fixed value (set by the lower bound). 
    
  int _fill_hist;
    
  double _parameter1;
  double _parameter2;
  double _parameter3;
  
  int _variant;
   
  OpticalIce *_ice; //! tell root not to save this object (it's saved through reco)
  
  // transient parameters (changed on call to reset())
  THX *_hScan;
 
  int _status;

  int _has_executed;
  
  ClassDef(VertexFinder,1);
  
};

#endif


