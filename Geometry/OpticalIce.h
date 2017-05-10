#ifndef OPTICALICE_H
#define OPTICALICE_H

////////////////////////////////////////////////////////////////
//								//
//	Simple ice model to use for ray-tracing.		//
//	Can run in uniform or exponential ice mode.	 	//
//								//
//	send any questions to: guy.nir.@weizmann.ac.il		//
//								//
////////////////////////////////////////////////////////////////

/**
   OpticalIce class
   OpticalIce.cc
   Purpose: Simple ice model to use for ray-tracing. Uniform or exponential
   
   @author Guy Nir
   @version 0.1
*/

#include "CppIncludes.h"
#include "RootIncludes.h"

class OpticalIce : public TObject{

public:

  //  OpticalIce(double n_deep=1.76, int variable_bit=0, int attenuation_bit=0, double n_shallow=1.4, double firn_start_depth=170, double slope=0);
  OpticalIce(double n_deep=1.78, int variable_bit=1, int attenuation_bit=1, double n_shallow=1.3528, double firn_start_depth=170, double slope=0.016); //these are the better values according to the paper (2014)
  OpticalIce(const OpticalIce &other);
  virtual ~OpticalIce();
  
  /*! \brief
   * initialization
   */
//   void reset();

  /*! \brief
   * printout class data
   */
  void printout();

  /*! \brief
   * n(z)
   * @param depth of channel
   * @return index of refration
   */
  
  // getters
  double getIndex(double z);
  double getAttenuation(double z, double path_length);
  
  double getDeepIceIndex() const;
  double getFirnIceIndex() const;
  double getFirnDepth() const;
  double getExpSlope() const;
  double getDeepIceAttenuation() const;
  double getFirnIceAttenuation() const;
  
  int isUniform() const;
  int isVariable() const;
  int hasAttenuation() const;
  
  //need to find default values for this function!!
  void setAttenuationFactor(double deep_attenuation_factor=1, double shallow_attenuation_factor=1);
    
  void setIndex(double n_deep=1.76, double n_shallow=1.4, double firn_start_depth=170, double slope=0);
  void setIceVariablityBit(int bit_status);
  void setIceAttenuationBit(int bit_status);
  
  
protected:
  
  int _variable_ice_bit;// if 0, use just _deep_ice_index, if 1: use straight line approx and exponential ice
  int _attenuation_bit;
  
  double _deep_ice_index;
  double _firn_ice_index;
  double _depth_of_firn;
  double _exp_slope;
  
  double _deep_ice_att;
  double _shallow_ice_att;
  
  void initialize(double n_deep, int variable_bit, int attenuation_bit, double n_shallow, double firn_start_depth, double slope);
    
  ClassDef(OpticalIce,1);
  
  
};

#endif
