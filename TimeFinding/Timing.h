#ifndef TIMING_H
#define TIMING_H

#include "CppIncludes.h"
#include "RootIncludes.h"

class Timing : public TObject{
  
public:
  
  Timing();
  Timing(double time, double weight=1, double error=3);
  Timing(const Timing &other);
  virtual ~Timing();
  
  virtual Timing &operator=(const Timing &other);
  static void swap(Timing &first, Timing &second);
  
  void printout() const;
  
  // getters
  double getTime() const;
  double getWeight() const;
  double getError() const;
  
  int hasRealTime() const;
  double getRealTime() const;
  double getResidual() const;
  
  std::string getFinderName() const;
  int getFinderType() const;
  
  // setters
  void setTime(double time);
  void setWeight(double weight);
  void setError(double error);
  void setTimingData(double time, double weight, double error);
  
  void setRealTime(double time);
  
  void setFinderName(std::string name);
  void setFinderType(int finder_type);
  
protected:
  
  void initialize();
  
  double _time;
  double _weight;
  double _error;
  
  int _has_real_time;
  double _real_time;
  double _residual;

  std::string _finder_name;
  int _finder_type;
  
  ClassDef(Timing,1);
  
};

#endif
