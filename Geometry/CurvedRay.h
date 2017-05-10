#ifndef CURVEDRAY_H
#define CURVEDRAY_H

#include "CppIncludes.h"

#include "Pos.h"
#include "OpticalIce.h"

#include "TMultiGraph.h"
#include "assert.h"


class CurvedRay : public TObject {

public:
  
  CurvedRay();
  CurvedRay(Pos antenna_pos);
  CurvedRay(Pos antenna_pos, Pos source_pos);
  CurvedRay(std::vector<double> antenna_pos);
  CurvedRay(std::vector<double> antenna_pos, std::vector<double> source_pos);
  CurvedRay(double antenna_x, double antenna_y, double antenna_z);
  CurvedRay(double antenna_x, double antenna_y, double antenna_z, double source_x, double source_y, double source_z);
  CurvedRay(const CurvedRay &other);
  ~CurvedRay();
  
  void printout();
  
  // getters
  double getHitTime() const;
  double getDistance() const;
  double getAttenuation() const;
  
  Pos getAntennaPos() const;
  Pos getSourcePos() const;
  
  static double getDepthStep();
  static double getHorStep();
  double getThetaStep() const;
  
  double getPhiLaunchAngle() const;
  double getThetaLaunchAngle() const;
  
  TMultiGraph *getDebugGraph() const;
  static OpticalIce *getOpticalIce();
   
  // setters
  void setSourcePosition(Pos position);
  void setSourcePosition(std::vector<double> cartesian_pos);
  void setSourcePosition(double x, double y, double z);
  
  void adjustSourcePosition(Pos position);
  void adjustSourcePosition(std::vector<double> cartesian_pos);
  void adjustSourcePosition(double x, double y, double z);
  
  static void setOpticalIce(OpticalIce *ice);
  static void useCurvedRays(int use_curved_rays);
  
  static const double C_SPEED;
  static const double PI;
  static const double RADTODEG;
  static const double DEGTORAD;
  
  void dump_debug_graph();

  enum Type { UNIFORM=0, STRAIGHT, NTIME, CURVED };
		  
 protected:

  //ray trace stuff
  
  struct rayStruct {
    double x[2];
    double y[2];
    double z[2];
    double trajectoryAngle;
    double trajectoryDistance;
    double travelTime;
    double finalPosition[3];
    double min;
    double absMin;
    bool reflect;
    bool good;
  };


  //Ray trace helper functions
  int findPointsOfInterest(double *minAry, double *angAry);
  int rayPath(double trajectoryAngle, double XYDist, rayStruct *rs);
  void rayTrace();
  
  double getRefractionAngle(double incidentAngle, double incidentIndex, double refractionIndex, bool &refCheck);
  void convertCartesian2Spherical(double origx, double origy, double origz, double vecx, double vecy, double vecz, double &theta, double &phi, double &R);

  double binarySearch(double minTheta, double maxTheta, int nSolution, int trialNum);
  double distance3D(double x1, double y1, double z1, double x2, double y2, double z2);
  double distance2D(double x1, double y1, double x2, double y2);
  double getRayTraceLowestTime();

  double _xy_dist;
  double _trajectory_angle;
  double _n_solution;
  int _success;
  int _n_cuts;
  int _max_solution;
  bool _good;
  double _min;
  double _tolerance;

  
  std::vector<double> _min_angle_vector;
  std::vector<double> _max_angle_vector;
  std::vector<double> _travel_time;
  
  std::vector<int> _mode;
  std::vector<double> _found_angle_vector;
  std::vector<double> _trajectory_distance;
  
  void initializeRayTrace();
  
  
  void initialize(Pos antenna_pos, Pos source_pos);
  
  
  void calculate();   
  
  void findThetaPhi();
  void calculateUniformIndexTrace();
  void calculateStraightLineApprox();
  void calculateCurvedRaytrace();
  void nTimeExpected();

  Pos _antenna_pos;
  Pos _source_pos;
  
  // should these be static???
  double _theta_step;// in radians
  
  double _flight_time_total;// in nanoseconds
  double _distance_total;// in meters
  double _attenuation_total;// fraction of 1
  
  double _launch_theta_angle;// in radians
  double _launch_phi_angle;// in radians
  
  TMultiGraph *_debug_graph;
  
  static OpticalIce *_ice;
  static int _use_curved_rays;
  static double _small_step; // what counts as a small adjustment
  static double _depth_step;// in meters
  static double _hor_step;// in meters
  
  static double _dist_step;
  
  ClassDef(CurvedRay, 1);
  
};

#endif
