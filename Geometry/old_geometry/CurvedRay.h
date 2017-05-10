#ifndef CURVEDRAY_H
#define CURVEDRAY_H

////////////////////////////////////////////////////////////////
//								//
//	Ray trace finder. Works on strait lines as well.	//
//	Remembers last calculation so raytracing is easier	//
//	when adjusting positions in the minimization.		//
//								//
//	send any questions to: guy.nir.@weizmann.ac.il		//
//								//
////////////////////////////////////////////////////////////////

/**
   CurvedRay class
   CurvedRay.cc
   Purpose: Ray trace finder(not yet implemented), and straight line 

   @author Guy Nir
   @version 0.1
*/
#include "OpticalIce.h"

class CurvedRay {
    
public:
  
  CurvedRay(OpticalIce *ice_ptr, double antenna_x=0, double antenna_y=0, double antenna_z=0, double source_x=200, double source_y=0, double source_z=-100, int use_raytrace=0);

  CurvedRay(OpticalIce *ice_ptr, vector<double> antenna_pos, vector<double> source_pos, int use_raytrace=0);
  
  void reset();  
  void printout();
  
  double getHitTime();
  double getDistance();
  double getAttenuation();


  /*! \brief 
   * @return Ice Model
   */
  OpticalIce *getIce();
  
  void adjustSourcePosition(double x, double y, double z);
  void adjustSourcePosition(vector<double> position_cartesian);
  void setSourcePosition(double x, double y, double z);
  void setSourcePosition(vector<double> position_cartesian);
  

  /*! \brief 
   * replace or set new Ice Model
   */
  void setNewIce(OpticalIce *ice);
    
  /*! \brief
   * calculate signal flight time 
   */
  void calculate();
  
  static const double C_SPEED;
  static const double pi;
  static const double RADTODEG;
  static const double DEGTORAD;
  
protected:
  
  vector<double> _antenna_pos_vector;
  vector<double> _source_pos_vector;
  
  int _use_curved_raytrace_bit;
  
  OpticalIce *_ice;
  
  double _depth_step;
  double _hor_step;
  double _theta_step;
  
  double _flight_time_total;
  double _distance_total;
  double _attenuation_total;
  
  double _launch_theta_angle;
  double _launch_phi_angle;
  
  void findThetaPhi();
  void calculateUniformIndexTrace();
  void calculateStraightLineApprox();
  void calculateCurvedRaytrace();
  
  void initialize(OpticalIce *ice_ptr, double antenna_x, double antenna_y, double antenna_z, double source_x, double source_y, double source_z, int use_raytrace);
  void clear_all();
  
};

#endif
