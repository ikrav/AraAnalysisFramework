#ifndef ANALYTICVERTEXFINDER_H
#define ANALYTICVERTEXFINDER_H

#include "CppIncludes.h"
#include "VertexFinder.h"

/**
   AnalyticVertexFinder class
   AnalyticVertexFinder.cc
   
   @author Andrew Shultz, Christian Bora
   @version 0.1
*/

class AnalyticVertexFinder : public VertexFinder {

 public:
  AnalyticVertexFinder();
  AnalyticVertexFinder(ChannelCollection channels, VertexPos *initial_position=0, int coordinate_system=0, int time_input=0, int lock_parameter=0, OpticalIce *ice_ptr=0, double par1=0, double par2=0, double par3=0);
  AnalyticVertexFinder(int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3);
  AnalyticVertexFinder(const std::string commands);
  AnalyticVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands);
  AnalyticVertexFinder(const AnalyticVertexFinder &other);  

  virtual ~AnalyticVertexFinder();

  void initialize();
  void reset();
  void help();
  void parse(const std::string commands);
  
  virtual std::string getFinderName() const;
  virtual VertexFinder *getDeepCopy();
  
  virtual void findVertex(ChannelCollection channels, VertexPos *initial_pos);
  virtual void findVertex();

  static int debug_bit;

 protected:
  //  virtual void calculateConfidence(double quality_parameter);

 private:
  double _x;
  double _y;
  double _z;
  double _t;
  double _n; //index of refraction
  void setAnt(double x, double y, double z, double t);
  double dist3D(double x1, double y1, double z1, double x2, double y2, double z2);
  double _ant_info[4][4];
  int _n_ant_set;
  bool solve();
  double _indx_refraction1, _indx_refraction2, _indx_refraction3;
  void orderer(double *values, int *order, int n);
  void quad(double a, double b, double c, double &pos, double &neg);
  double C();
  double sQ(double value);
  float nTimeExpected(float z1, float z2, float dtot);
  double nOfZ(double z);  

  ClassDef(AnalyticVertexFinder,1);
  
};

#endif
