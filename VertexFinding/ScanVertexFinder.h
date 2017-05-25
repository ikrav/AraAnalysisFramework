#ifndef SCANVERTEXFINDER_H
#define SCANVERTEXFINDER_H

////////////////////////////////////////////////////////////////
//								//
//	Finder class that locates nu interaction vertex.	//
//	Doesnt calculate anything, this is just an interface	//
//	for other VertexFinder classes to inherit from. 	//
//								//
//	send any questions to: guy.nir.@weizmann.ac.il		//
//								//
////////////////////////////////////////////////////////////////

/**
   ScanVertexFinder class
   ScanVertexFinder.cc
   Purpose: interface for other VertexFinder 

   @author Guy Nir
   @version 0.1
*/
#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexFinder.h"

class ScanVertexFinder : public VertexFinder {

public:
  
  ScanVertexFinder();
  ScanVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  ScanVertexFinder(std::string commands);
  ScanVertexFinder(ChannelCollection channels, VertexPos *initial_position, std::string commands);
  ScanVertexFinder(const ScanVertexFinder &other);
  virtual ~ScanVertexFinder();
  
  virtual VertexFinder *getDeepCopy();
  
  virtual void findVertex();
  
  virtual std::string getFinderName() const;

  virtual void reset();
  virtual void help();
  virtual void parse(std::string commands);
  
  // setters
  void setNumSteps1(int num);
  void setNumSteps2(int num);
  void setNumSteps3(int num);
  //void setNumSteps(int num1, int num2, int num3);
  
  static const int _iter_2_theta_bins;
  static const int _iter_2_phi_bins;

  static const int _iter_3_R_bins;
  static const int _iter_3_theta_bins;
  static const int _iter_3_phi_bins;

protected:
  
  virtual void initialize();
  
  void initializeHistograms();
  void fillHistogram(Pos position, double chi2);
  
//   virtual void calculateConfidence(double quality_parameter);
  
  int _Nx;
  int _Ny;
  int _Nz;
  
  ClassDef(ScanVertexFinder,1);
  
};

#endif
