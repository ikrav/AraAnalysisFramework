#ifndef POS_H
#define POS_H

#include "CppIncludes.h"
#include "RootIncludes.h"

class Pos : public TObject {
  
public:
  
  // constructors
  Pos();
  Pos(std::vector<double> cartesian_pos);
  Pos(double x, double y, double z);
  Pos(const Pos &other);// copy constructor
  ~Pos();
  
  Pos & operator=(const Pos &other);
  static void swap(Pos &first, Pos &second);
  
  // generic methods
//   void printHeader() const;
  virtual void printout() const;

  
  // getters
  double getX() const;
  double getY() const;
  double getZ() const;
  std::vector<double> getCartesian() const;
  
  double getR() const;
  double getPhi() const;
  double getTheta() const;
  std::vector<double> getSpherical() const;
  
  double getOriginX() const;
  double getOriginY() const;
  double getOriginZ() const;
  
  static Pos getOrigin(); 
  
  // setters
  void setX(double x);
  void setY(double y);
  void setZ(double z);
  
  void setCartesian(std::vector<double> pos);
  void setCartesian(double x, double y, double z);
  
  void setR(double R);
  void setTheta(double theta);
  void setPhi(double phi);
  
  void setSpherical(std::vector<double> pos);
  void setSpherical(double R, double theta, double phi);  
  
  void setSphericalDegrees(std::vector<double> pos);
  void setSphericalDegrees(double R, double theta, double phi);
  
  // utility methods
  static std::vector<double> valuesToVector(double val1, double val2, double val3);
  
  static std::vector<double> sphericalToCartesian(std::vector<double> spherical_pos);
  static std::vector<double> sphericalToCartesian(double R, double phi, double theta);

  static std::vector<double> cartesianToSpherical(std::vector<double> cartesian_pos);
  static std::vector<double> cartesianToSpherical(double x, double y, double z);
  
  static double rad2deg(double radians);
  static double deg2rad(double degrees);
  
  // calculator methods
  double getDistance(const Pos &other) const;
  static double getDistance(const Pos &first, const Pos &second);
  
  Pos getMidPoint(const Pos &other) const;
  static Pos getMidPoint(const Pos &first, const Pos &second);
  static Pos getMidPoint(const std::vector<Pos> positions);
  
  static const double PI;
  
  static void setOriginStatic(double x0, double y0, double z0);
  static void setOriginStatic(const Pos &origin_position);
 
  // math operators
  Pos operator+(const Pos &other);
  Pos operator-(const Pos &other);
  
  friend std::ostream &operator<<( std::ostream &out, const Pos &pos);
  
//   static bool print_spherical;
  
  
protected:
  
  double _x,_y,_z;
//   double _R,_phi,_theta;
  
//   double _x0,_y0,_z0; // this is saved with the position object (for future reference)
  static double _origin_x, _origin_y, _origin_z;// this is shared by all Pos objects in the run
  
  void initialize();
//   void updateCartesian();
//   void updateSpherical();
//   void updateOrigin();
  
  ClassDef(Pos,1);
  
};

#endif