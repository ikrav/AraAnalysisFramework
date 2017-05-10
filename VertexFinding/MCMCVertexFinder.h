#ifndef MCMCMVERTEXFINDER_H
#define MCMCMVERTEXFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexFinder.h"

class MCMCVertexFinder : public VertexFinder {
  
public:
  
  MCMCVertexFinder(int coordinate_system=0, int time_input=0, int lock_parameter=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0, int variant=0);
  MCMCVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0, int variant=0);
  MCMCVertexFinder(const std::string commands);
  MCMCVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands);  
  MCMCVertexFinder(const MCMCVertexFinder &other);
  virtual ~MCMCVertexFinder();
  
  virtual VertexFinder *getDeepCopy();
  
  void initialize();
  void reset();
  virtual void help();
  virtual void parse(const std::string commands);
  
  // getters
  int getNumIterations() const;
  int getNumSteps() const;
  int getNumBurn() const;
  double getStepSize() const;
  
  // setters
  void setNumIterations(int num);
  void setNumSteps(int num);
  void setNumBurn(int num);
  void setStepSize(double size);
  
  virtual void findVertex();
  virtual Pos estimatePosition();
  virtual std::vector<std::vector<double> > estimateErrors();
  
  // getters
  virtual std::string getFinderName() const;
  
  TH3D *getDistHist() const;
  
protected:

  virtual void takeRandomStep();
  virtual void takeRandomStep(int axis);
  
  Pos _this_point;
  Pos _next_point;
  
  int _num_iterations;
  int _num_steps;
  int _num_burn;
  double _step_size;
  
  std::vector<Pos> _points;
  
  TRandom3 *_rand;
  
  static int debug_bit;
  
  ClassDef(MCMCVertexFinder,1);
   
};

#endif
