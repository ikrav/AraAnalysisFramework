#ifndef CONFIGVERTEX_H_
#define CONFIGVERTEX_H_

#include <string>
#include <iostream>
#include <vector>
#include "VertexFinder.h"
//using namespace std;
using std::string;
using std::vector;

class ConfigVertex {

public:
  
  ConfigVertex();
  ConfigVertex(string vertexFinder, double a=0.0, double b=0.0, double c=0.0, string coordinates="CARTESIAN");
  ~ConfigVertex();

  void initialize();
  void printout();
  
  void setVertexFinder(string finder);
  void setParameters(double a, double b, double c);
  void setCoordinates(string coord);
  void setPreviousFinder(ConfigVertex *vertex);

  string getVertexFinder() const;
  vector<double> getParameters();
  string  getCoordinate() const;
  ConfigVertex* getPreviousFinder();

  int getCoordinateType();
  int getVertexType();
  void setCoordinateType();
  void setVertexType();
  void setLockType();

  int getPolType();

  double getParameter1();
  double getParameter2();
  double getParameter3();
  
  int getLockType();
 
    
  //  static vector<double> getParamValues(string value);
  //  static string getFinderName(string value);
  //  static vector<string> getFinderVector(string value);
  //  static string getCoordName(string value);
  

protected:
  
  string _vertex_finder;
  int _vertex_type;
  double _param[3];
  
  string _coordinates;
  int _coordinate_type;
  
  int _lock_type;
  string _lock;

  ConfigVertex *_previous_finder;


};

#endif
