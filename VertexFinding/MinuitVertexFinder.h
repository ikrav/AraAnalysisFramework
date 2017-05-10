#ifndef MINUITVERTEXFINDER_H
#define MINUITVERTEXFINDER_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "VertexFinder.h"

/**
   MinuitVertexFinder class
   MinuitVertexFinder.cc
   Purpose: Use TMinuit minimizer to find vertex

   @author Guy Nir
   @version 0.1
*/

class MinuitVertexFinder : public VertexFinder {

 public :

  MinuitVertexFinder(int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  MinuitVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  MinuitVertexFinder(const std::string commands);
  MinuitVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands);
  MinuitVertexFinder(const MinuitVertexFinder &other);
  virtual ~MinuitVertexFinder();

  virtual void findVertex();

  virtual VertexFinder *getDeepCopy();
  
  void initialize();
  void reset();
  virtual void help();
  virtual void parse(const std::string commands);
  
  // getters
  virtual std::string getFinderName() const;
      
  static int debug_bit;
  
protected:   
   
  TMinuit *_minimizer;   //!
  
  int _error_flag[6];
  double _arglist[2];
  int _ierflag;

  ClassDef(MinuitVertexFinder,1);
  
};

#endif
