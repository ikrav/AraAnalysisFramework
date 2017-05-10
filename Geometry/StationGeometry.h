#ifndef STATIONGEOMETRY_H
#define STATIONGEOMETRY_H

////////////////////////////////////////////////////////////////
//								//
//	Holds position of antennas in station coordinates.	//
//	Used to generate and reconstruct events to the same	//
//	relative position of antennas/axis origin. 		//
//								//
//	send any questions to: guy.nir.@weizmann.ac.il		//
//								//
////////////////////////////////////////////////////////////////

/**
   StationGeometry class
   StationGeometry.cc
   Purpose: Holds station's geometry in Cartesian and Spherical coordinates

   @author Guy Nir
   @version 0.1
*/

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "Pos.h"
#include "AntPos.h"

class StationGeometry : public TObject {

public:

  enum Pol { VPOL, HPOL, BOTH,
	     UNKNOWN=-1 };
  
  StationGeometry(std::vector<AntPos> positions, double originX=0, double originY=0, double originZ=0);
  StationGeometry(double radius=10, double vertical=15, int pol=BOTH);
  StationGeometry(const StationGeometry &other);
  
  ~StationGeometry();

  AntPos operator[](int index) const;

  void reset(int pol=-1);
  void printout();
  
  // getter methods
  int getNumChan(int pol=BOTH);
  
  double getStringRadius();
  double getVerticalSeparation();
  
  std::vector<double> getAntPos(int channelID);
  AntPos getPosition(int channelID) const;
  AntPos getPulserPos(int pulserID) const;

  int getPolarization(int ch) const;
  int getPulserPolarization(int pol_id) const;
  
  StationGeometry *getVpolGeometry();
  StationGeometry *getHpolGeometry();
  std::vector<AntPos> getChannels() const; 
  std::vector<AntPos> getPulsers() const;

  std::vector<double> getOrigin();
  Pos getOriginPosition() const;
  
  // setters and modifiers
  void twistGeometry();// why do I need this? because the strings in AraSim output files are twisted that's why
  void setOrigin(double origin_x, double origin_y, double origin_z);
  void setOrigin(double *pos);

  void setChannels(std::vector<AntPos> pos);
  void addChannel(AntPos pos);
  
  void setPulsers(std::vector<AntPos> cal_pos);
  void addPulser(AntPos cal_pos);
  
  static const double PI;
  static const double RADTODEG;
  static const double DEGTORAD;
  
  
protected:
  
  void initialize();
  void clear_all();
  void defaultGeometry(int pol=-1);
  StationGeometry *getSubSet(int pol);
    
//   const int _num_chan;
  int _num_strings;
  int _ant_per_string;
  double _vertical_separation;
  double _string_radius;
  
  double _origin_x;
  double _origin_y;
  double _origin_z;
  
  int _has_twisted_geometry_bit;// knows if this station has had a twist of the two last strings (to fit AraSim's geometry).

  std::vector<AntPos> _antenna_positions;
  std::vector<AntPos> _calpulser_positions;

  ClassDef(StationGeometry,1);
  
};

#endif
