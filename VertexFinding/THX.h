#ifndef THX_H
#define THX_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "ValuePos.h"

class THX {

public:
  
  THX();  
  THX(const THX &other);
  virtual ~THX();
  
  void initialize();
  void resetPoints(); // just empty the contents of the histogram
  void printout() const;
  void help() const;
  
  // getters
//   std::string getName() const;
//   std::string getTitle() const;
  
  int getDimReduction() const;
  int getCoordinates() const;
  std::string getCoordinatesString() const;
   
  int getValueType() const;
  double getPowerForQP() const;
  
  std::vector<ValuePos> getPointsVector() const;
  TH1D *getNewHist1D(int axis=0, int coordinates=-1, int show_prob=0) const;
  TH2D *getNewHist2D(int axis1=1, int axis2=2,int coordinates=-1, int show_prob=0) const;
  TH3D *getNewHist3D(int coordinates=-1, int show_prob=0) const;
  
  int getNbins(int axis, int coordinates=-1) const;
  double getMin(int axis, int coordinates=-1) const;
  double getMax(int axis, int coordinates=-1) const;
  int getNumDimensions(int coordinates=-1) const;
  
  // setters
//   void SetName(const char* name);
//   void SetTitle(const char* title="");
//   void SetNameTitle(const char* name, const char* title);
  
  void setPoint(Pos position, double value);
  void setPointsVector(std::vector<ValuePos> points, int append=0);
  void setValueType(int type);
  void setPowerForQP(double power);
  void reserve(int size);
    
  enum Reduce {PROJECT=0, SLICE};
  
  static int debug_bit;
  
protected:
  
//   std::string _name;
//   std::string _title;
  //   void specidic_setup();

  int _reduce_dims; // what to do with reduced dimensions? project/slice
  int _coordinates;
  
  int _x_nbins;
  double _x_min;
  double _x_max;
  
  int _y_nbins;
  double _y_min;
  double _y_max;
  
  int _z_nbins;
  double _z_min;
  double _z_max;
  
  int _R_nbins;
  double _R_min;
  double _R_max;
  
  int _theta_nbins;
  double _theta_min;
  double _theta_max;
  
  int _phi_nbins;
  double _phi_min;
  double _phi_max;
    
  std::vector<ValuePos> _points;
      
  ClassDef(THX,1);
  
};

#endif