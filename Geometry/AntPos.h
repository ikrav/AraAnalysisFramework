#ifndef ANTPOS_H
#define ANTPOS_H


#include "Pos.h"

class AntPos : public Pos {

 public :

  AntPos();
  AntPos(std::vector<double> cartesian_pos, int pol=-1, double vrms=50.0);
  AntPos(double x, double y, double z, int pol=-1, double vrms=50.0);
  AntPos(const AntPos &other);
  ~AntPos();

  void printout();

  int getPolarization() const;
  int getChanID() const;
  double getVRMS() const;

  void setPolarization(int pol);
  void setChanID(int id);
  void setVRMS(double value);

  enum Type { X, Y, Z,
	      R, PHI, THETA,
	      UNKNOW=-1 };

 protected:
  int _polarization;
  int _chan_id;
  double _vrms;

  ClassDef(AntPos,1);
  

};

#endif


  
