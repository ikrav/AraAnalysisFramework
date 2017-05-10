#ifndef TIMINGMATRIX_H
#define TIMINGMATRIX_H

#include "CppIncludes.h"
#include "RootIncludes.h"

#include "PairTiming.h"


class TimingMatrix : public TObject{
  
public:
  
  TimingMatrix();
  virtual ~TimingMatrix();
  
  void printout() const;
  void printResiduals() const;
  void printWeights() const;
  void printErrors() const;
  int size() const;
  
  // getter methods
  int getNumChans() const;
  int getNumCombinations() const;
  int getNumUsefulPairs() const;
  std::vector<PairTiming> &operator[](int index);
  
  PairTiming getPair(int channel_id1, int channel_id2);
      
  // add methods
  void addTimingVector(std::vector<PairTiming> timing_vector);
  void fillResiduals(TH1D *hist); 
  
protected:
  
  std::vector< std::vector<PairTiming> > _matrix;
  
  ClassDef(TimingMatrix,1);
  
};

#endif
