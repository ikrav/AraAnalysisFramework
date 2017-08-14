#ifndef VERTEXPOS_H
#define VERTEXPOS_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "Pos.h"

class VertexFinder;

class VertexPos : public Pos {
  
public:
  
  VertexPos();
  VertexPos(std::vector<double> cartesian_pos);
  VertexPos(std::vector<double> cartesian_pos, std::vector<double> errors_cartesian);
//   VertexPos(std::vector<double> cartesian_pos, std::vector<double> lower_errors_cartesian, std::vector<double> upper_errors_cartesian);
  VertexPos(double x, double y, double z);
  VertexPos(double x, double y, double z, double error_x, double error_y, double error_z);
//   VertexPos(double x, double y, double z, double lower_error_x, double lower_error_y, double lower_error_z, double upper_error_x, double upper_error_y, double upper_error_z);
  VertexPos(const Pos &other);
  VertexPos(const VertexPos &other);
  virtual ~VertexPos();
  
  VertexPos &operator=(const VertexPos &other);
  void adoptPreviousPos(VertexPos *other, double expand=1);
  
  static VertexPos *meanVertexPos(std::vector<VertexPos*> positions, int combine_method=0);
  static VertexPos *meanVertexPos(std::vector<VertexFinder*> positions, int combine_method=0);
  
  virtual void printout(int coordinate_system=0) const;
  
  // getters (cartesian)
  virtual std::vector< std::vector<double> > getCovMatCartesian(double expansion_factor=1) const;
//   virtual std::vector<double> getAsymmetryCartesian() const;
  
  virtual double getErrorX(double expand=1) const;
  virtual double getErrorY(double expand=1) const;
  virtual double getErrorZ(double expand=1) const;
  virtual std::vector<double> getErrorsCartesian(double expand=1) const;
  
//   double getLowerErrorX(double expand=1) const;
//   double getLowerErrorY(double expand=1) const;
//   double getLowerErrorZ(double expand=1) const;
//   std::vector<double> getLowerErrorsCartesian(double expand=1) const;
    
  virtual double getLowerBoundX(double expand=1) const;
  virtual double getLowerBoundY(double expand=1) const;
  virtual double getLowerBoundZ(double expand=1) const;
  virtual std::vector<double> getLowerBoundsCartesian(double expand=1) const;

//   double getUpperErrorX(double expand=1) const;
//   double getUpperErrorY(double expand=1) const;
//   double getUpperErrorZ(double expand=1) const;
//   std::vector<double> getUpperErrorsCartesian(double expand=1) const;
    
  virtual double getUpperBoundX(double expand=1) const;
  virtual double getUpperBoundY(double expand=1) const;
  virtual double getUpperBoundZ(double expand=1) const;
  virtual std::vector<double> getUpperBoundsCartesian(double expand=1) const;
  
  virtual std::vector<Pos> getBoundsBoxSides(double expand=1) const;
  virtual std::vector<Pos> getBoundsBoxCorners(double expand=1) const;
  
  // getters (spherical)
  std::vector< std::vector<double> > getCovMatSpherical(double expansion_factor=1) const;
//   std::vector<double> getAsymmetrySpherical() const;
  
  virtual double getErrorR(double expand=1) const;
  virtual double getErrorTheta(double expand=1) const;
  virtual double getErrorPhi(double expand=1) const;
  virtual std::vector<double> getErrorsSpherical(double expand=1) const;
  
//   double getLowerErrorR(double expand=1) const;
//   double getLowerErrorTheta(double expand=1) const;
//   double getLowerErrorPhi(double expand=1) const;
//   std::vector<double> getLowerErrorsSpherical(double expand=1) const;
    
  virtual double getLowerBoundR(double expand=1) const;
  virtual double getLowerBoundTheta(double expand=1) const;
  virtual double getLowerBoundPhi(double expand=1) const;
  virtual std::vector<double> getLowerBoundsSpherical(double expand=1) const;
  
//   double getUpperErrorR(double expand=1) const;
//   double getUpperErrorTheta(double expand=1) const;
//   double getUpperErrorPhi(double expand=1) const;
//   std::vector<double> getUpperErrorsSpherical(double expand=1) const;
    
  virtual double getUpperBoundR(double expand=1) const;
  virtual double getUpperBoundTheta(double expand=1) const;
  virtual double getUpperBoundPhi(double expand=1) const;
  virtual std::vector<double> getUpperBoundsSpherical(double expand=1) const;
  
  virtual std::vector<Pos> getBoundsTrapezoidSides(double expand=1) const;
  virtual std::vector<Pos> getBoundsTrapezoidCorners(double expand=1) const; 
  
  // getters (real)
  int hasRealPos() const;
  
  double getRealX() const;
  double getRealY() const;
  double getRealZ() const;
  
  double getRealR() const;
  double getRealTheta() const;
  double getRealPhi() const;
  
  double getResidualX() const;
  double getResidualY() const;
  double getResidualZ() const;
  
  double getResidualR() const;
  double getResidualTheta() const;
  double getResidualPhi() const;
  
  Pos getRealPosition() const;
  
  double getConfidence() const; 
  
  // setters (cartesian)
  void setCovMatCartesian(std::vector< std::vector<double> > covMatrix);
  void setErrorInCartesianMatrix(double error, int position);
  void setErrorInCartesianMatrix(double e_x, double e_y, double e_z);
//   void setAsymmetryCartesian(std::vector<double> asymmetry_factors);
  
  void calculateCartesian();
  void calculateCartesianCovMatrix();
  
  void setErrorX(double ex, int calculate=1);
  void setErrorY(double ey, int calculate=1);
  void setErrorZ(double ez, int calculate=1);
  void setErrorsCartesian(std::vector<double> errors_cartesian, int calculate=1);
  void setErrorsCartesian(double ex, double ey, double ez, int calculate=1);
  
//   void setLowerErrorX(double ex, int calculate=1);
//   void setLowerErrorY(double ey, int calculate=1);
//   void setLowerErrorZ(double ez, int calculate=1);
//   void setLowerErrorsCartesian(std::vector<double> lower_errors_cartesian, int calculate=1);
//   void setLowerErrorsCartesian(double ex, double ey, double ez, int calculate=1);
//   
//   void setUpperErrorX(double ex, int calculate=1);
//   void setUpperErrorY(double ey, int calculate=1);
//   void setUpperErrorZ(double ez, int calculate=1);
//   void setUpperErrorsCartesian(std::vector<double> upper_errors_cartesian, int calculate=1);
//   void setUpperErrorsCartesian(double ex, double ey, double ez, int calculate=1);

  void setLowerBoundX(double ex, int calculate=1);
  void setLowerBoundY(double ey, int calculate=1);
  void setLowerBoundZ(double ez, int calculate=1);
  void setLowerBoundsCartesian(std::vector<double> lower_bounds_cartesian, int calculate=1);
  void setLowerBoundsCartesian(double ex, double ey, double ez, int calculate=1);

  
  void setUpperBoundX(double ex, int calculate=1);
  void setUpperBoundY(double ey, int calculate=1);
  void setUpperBoundZ(double ez, int calculate=1);
  void setUpperBoundsCartesian(std::vector<double> upper_bounds_cartesian, int calculate=1);
  void setUpperBoundsCartesian(double ex, double ey, double ez, int calculate=1);

  
  // setters (spherical)
  void setCovMatSpherical(std::vector< std::vector<double> > covMatrix, int calculate=1);
  void setErrorInSphericalMatrix(double error, int position);
  void setErrorInSphericalMatrix(double e_R, double e_Theta, double e_Phi);
  void calculateSphericalCovMatrix();
  void calculateSpherical();
//   void setAsymmetrySpherical(std::vector<double> asymmetry_factors);
  
  void setErrorR(double e_R, int calculate=1);
  void setErrorTheta(double e_theta, int calculate=1);
  void setErrorPhi(double e_phi, int calculate=1);
  void setErrorsSpherical(std::vector<double> errors_spherical, int calculate=1);
  void setErrorsSpherical(double e_R, double e_theta, double e_phi, int calculate=1);
/*  
  void setLowerErrorR(double e_R, int calculate=1);
  void setLowerErrorTheta(double e_theta, int calculate=1);
  void setLowerErrorPhi(double e_phi, int calculate=1);
  void setLowerErrorsSpherical(std::vector<double> lower_errors_spherical, int calculate=1);
  void setLowerErrorsSpherical(double e_R, double e_theta, double e_phi, int calculate=1);
  
  void setUpperErrorR(double e_R, int calculate=1);
  void setUpperErrorTheta(double e_theta, int calculate=1);
  void setUpperErrorPhi(double e_thi, int calculate=1);
  void setUpperErrorsSpherical(std::vector<double> upper_errors_spherical, int calculate=1);
  void setUpperErrorsSpherical(double e_R, double e_theta, double e_thi, int calculate=1);*/
    
  void setLowerBoundR(double e_R, int calculate=1);
  void setLowerBoundTheta(double e_theta, int calculate=1);
  void setLowerBoundPhi(double e_phi, int calculate=1);
  void setLowerBoundsSpherical(std::vector<double> lower_bounds_spherical, int calculate=1);
  void setLowerBoundsSpherical(double e_R, double e_theta, double e_phi, int calculate=1);
  
  void setUpperBoundR(double e_R, int calculate=1);
  void setUpperBoundTheta(double e_theta, int calculate=1);
  void setUpperBoundPhi(double e_phi, int calculate=1);
  void setUpperBoundsSpherical(std::vector<double> upper_bounds_spherical, int calculate=1);
  void setUpperBoundsSpherical(double e_R, double e_theta, double e_phi, int calculate=1);
  
  void setRealPosition(Pos position);
  
  void setConfidence(double confidence);
  
  int isOriginInBounds() const; // check if the coordinates origin is inside the error bounds. 
  
  static const double PI;
  static const double DEGTORAD;
  static const double RADTODEG;
  
  enum ChooseResult { BEST=0, AVERAGE};
  
  static const std::string name_choose_result(int index);

protected:
  
  void initialize();
  
  double _error_x;
  double _error_y;
  double _error_z;
  
  double _error_R;
  double _error_theta;
  double _error_phi;

  double _upper_bound_theta;
  double _upper_bound_R;
  double _upper_bound_phi;

  double _lower_bound_theta;
  double _lower_bound_R;
  double _lower_bound_phi;
  
//   double _lower_error_x;
//   double _lower_error_y;
//   double _lower_error_z;
//   
//   double _upper_error_x;
//   double _upper_error_y;
//   double _upper_error_z;
//   
//   double _lower_error_R;
//   double _lower_error_Theta;
//   double _lower_error_Phi;
//   
//   double _upper_error_R;
//   double _upper_error_Theta;
//   double _upper_error_Phi;
  
  std::vector< std::vector<double> > _covMatSpherical;
  std::vector< std::vector<double> > _covMatCartesian;
  
//   double _asymmetry_x;
//   double _asymmetry_y;
//   double _asymmetry_z;
//   
//   double _asymmetry_R;
//   double _asymmetry_Theta;
//   double _asymmetry_Phi;
  
  double _confidence;
  
  Pos _real_position;
  
  int _has_real_pos;
  
  ClassDef(VertexPos,1);
  
};

#endif
