#ifndef INTMAPVERTEXFINDER_H
#define INTMAPVERTEXFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexFinder.h"

class IntMapVertexFinder : public VertexFinder {
  
public:
  
  IntMapVertexFinder(int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  IntMapVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  IntMapVertexFinder(const std::string commands);
  IntMapVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands);
  IntMapVertexFinder(const IntMapVertexFinder &other);
  virtual ~IntMapVertexFinder();
  
  virtual VertexFinder *getDeepCopy();
  
  void initialize();
  void reset();
  virtual void printout() const;
  static void help();
  void parse(const std::string commands);
    
  void clear_maps();
  
  // getters
  TH2D *getMap(int index=0) const;
  TH2D *getBestMap() const;
  
  inline double getBestCorrelation() {return _best_corr;}
  inline double getLogStep() {return _step_log;}
  inline double getPhiStep() {return _step_phi;}
  inline double getThetaStep() {return _step_theta;}

  virtual void findVertex();
  
  virtual std::string getFinderName() const;
    
  // setters
  virtual void setCoordinates(int coordinates);
  virtual void setTimeInputs(int times);
  virtual void setNumBinsR(int num_bins);
  virtual void setNumBinsR(const std::string number);
  virtual void setNumBinsTheta(int num_bins);
  virtual void setNumBinsTheta(const std::string number);
  virtual void setNumBinsPhi(int num_bins);
  virtual void setNumBinsPhi(const std::string number);
  
  void makeLogAxis();
  
  static int debug_bit;
  
protected:
    
  virtual void calculateConfidence(double quality_parameter);
  virtual void estimateErrors();
  
  // user inputs
  int _num_bins_R;
  int _num_bins_theta;
  int _num_bins_phi;
  
  int _best_bin_R;
  int _best_bin_theta;
  int _best_bin_phi;
  double _best_corr;

  double _step_theta;
  double _step_phi;
  double _step_log;
  
  std::vector<double> _R_axis;
  std::vector<double> _theta_axis;
  std::vector<double> _phi_axis;
 
  std::vector<TH2D*> _maps;

  ClassDef(IntMapVertexFinder,1);
  
};

#endif
