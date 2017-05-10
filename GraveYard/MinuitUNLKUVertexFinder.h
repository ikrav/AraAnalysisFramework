#ifndef MINUITUNLKUVERTEXFINDER_H_
#define MINUITUNLKUVERTEXFINDER_H_

#include "VertexFinder.h"
#include "TMinuit.h"

#include "RootIncludes.h"
#include "CppIncludes.h"

class MinuitUNLKUVertexFinder : public VertexFinder {

public:
  
  MinuitUNLKUVertexFinder();
  MinuitUNLKUVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameters=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  MinuitUNLKUVertexFinder(int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice, double par1, double par2, double par3);
  virtual ~MinuitUNLKUVertexFinder();
  
  virtual void findVertex();
  virtual void findVertex(ChannelCollection channels, VertexPos *initial_position);
  
  void reset();
  void initialize();
  
  virtual string getFinderName() const;
  
 
protected:

  TMinuit *_minimizer;
  int _error_flag[6];
  double _arglist[2];
  int _ierflag;

  ClassDef(MinuitUNLKUVertexFinder, 1);

};

#endif

