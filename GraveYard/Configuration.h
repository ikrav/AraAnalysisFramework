#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>
#include <string>
#include "ConfigVertex.h"
#include "TimeFinder.h"
#include "StationGeometry.h"
/*****************
   Data structure for a command

**********/


class Configuration {

public:
  
  Configuration();
  
 ~Configuration();

  ConfigVertex *operator[](int index) const;  

  void initialize();
  void printout();

  void setLabel(string label);
  void setPol(string pol);
  void setTimeFinder(string finder);
  void setTimeFinderParameters(double a, double b, double c);
  void addVertex(ConfigVertex *vertex);
  void setFilter(string value);
  void setDelta(string value);

  void setDeltaType();
  void setTimeType();
  void setPolType();
  void setFilterType();


  string getLabel() const;
  string getPol() const;
  string getTimeFinder() const;
  string getFilter() const;
  vector<double> getTimeFinderParameters() const;
  vector<ConfigVertex* > getVertices();
  string getDelta() const;

  int getDeltaType() const;
  int getTimeType() const;
  int getPolType() const;
  int getFilterType() const;
  
protected:

  string _label;
  string _pol;  
  string _time_finder;
  string _delta;
  string _filter;

  int _pol_type;  
  int _time_finder_type;
  int _delta_type;
  int _filter_type;

  double _time_parameters[3];

  vector<ConfigVertex* > _vertex_finders;
  
};

#endif
