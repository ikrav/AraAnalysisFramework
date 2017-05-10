#ifndef SIMDATA_H
#define SIMDATA_H


#include "CppIncludes.h"
#include "RootIncludes.h"

#include "Pos.h"
#include "StationGeometry.h"

class SimData : public Pos {

public:
  
  SimData();
  SimData(const SimData &other);
  virtual ~SimData();
  
  void printout();
  
  // getters
  double getEnergy() const;
  double getFieldAtSource() const;
  double getPolarizationRatio() const;
  double getDirTheta() const;
  double getDirPhi() const;
  int getNuFlavor() const;
  
  
  // setters
  void setEnergy(double energy);
  void setFieldAtSource(double field);  
  void setPolarizationRatio(double pol_ratio);
  void setDirTheta(double theta);
  void setDirPhi(double phi);
  void setNuFlavor(int flavor);
  
protected:
  
  void initialize();
  
  double _energy;
  double _field_at_source;
  double _pol_ratio;
  
  double _nu_dir_theta;
  double _nu_dir_phi;
  
  int _nu_flavor;

  ClassDef(SimData,1);

};

#endif
