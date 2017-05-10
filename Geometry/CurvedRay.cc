#include "CurvedRay.h"

ClassImp(CurvedRay);

CurvedRay::CurvedRay(){
 
  initialize(Pos(), Pos(10,10,-210));
  
}

CurvedRay::CurvedRay(Pos antenna_pos){
  
  initialize(antenna_pos, Pos());
  
}

CurvedRay::CurvedRay(Pos antenna_pos, Pos source_pos){
 
  initialize(antenna_pos, source_pos);
  
}

CurvedRay::CurvedRay(std::vector<double> antenna_pos){
 
  Pos antenna(antenna_pos);
  
  initialize(antenna, Pos());
  
}

CurvedRay::CurvedRay(std::vector<double> antenna_pos, std::vector<double> source_pos){
 
  Pos antenna(antenna_pos);
  Pos source(source_pos);
  
  initialize(antenna, source);
  
}

CurvedRay::CurvedRay(double antenna_x, double antenna_y, double antenna_z){
 
  Pos antenna(antenna_x, antenna_y, antenna_z);
  
  initialize(antenna, Pos());
  
}

CurvedRay::CurvedRay(double antenna_x, double antenna_y, double antenna_z, double source_x, double source_y, double source_z){
 
  Pos antenna(antenna_x, antenna_y, antenna_z);
  Pos source(source_x, source_y, source_z);
  
  initialize(antenna, source);
  
}

CurvedRay::CurvedRay(const CurvedRay &other) : TObject(other) {
 
  _xy_dist=other._xy_dist;
  _trajectory_distance=other._trajectory_distance;
  _n_solution=other._n_solution;
  _success=other._success;
  _n_cuts=other._n_cuts;
  _max_solution=other._max_solution;
  _good=other._good;
  _min=other._min;
  _tolerance=other._tolerance;
  
  _min_angle_vector=other._min_angle_vector;
  _max_angle_vector=other._max_angle_vector;
  _travel_time=other._travel_time;
  _mode=other._mode;
  _found_angle_vector=other._found_angle_vector;
  _trajectory_distance=other._found_angle_vector;
  
  _antenna_pos=other.getAntennaPos();
  _source_pos=other.getSourcePos();
  
//   _depth_step=other.getDepthStep(); // static
//   _hor_step=other.getHorStep(); // static
  _theta_step=other.getThetaStep(); // not static
  
  _flight_time_total=other.getHitTime();
  _distance_total=other.getDistance();
  _attenuation_total=other.getAttenuation();
  
  _launch_phi_angle=other.getPhiLaunchAngle();
  _launch_theta_angle=other.getThetaLaunchAngle();
  
  if(other.getDebugGraph()) _debug_graph=(TMultiGraph*) other.getDebugGraph()->Clone();
  else _debug_graph=0;
  
}

CurvedRay::~CurvedRay(){
 
//   std::cout<<__PRETTY_FUNCTION__<<"  _debug_graph= "<<_debug_graph<<std::endl;
  
  dump_debug_graph();
  
}

void CurvedRay::initialize(Pos antenna_pos, Pos source_pos){
 
  _antenna_pos=antenna_pos;
  _source_pos=source_pos;
  
  _debug_graph=0;
    
  _theta_step=0.01;// in radians
  
  _flight_time_total=0;// in nanoseconds
  _distance_total=0;// in meters
  _attenuation_total=1;// fraction of 1
  
  _launch_theta_angle=0;
  _launch_phi_angle=0;

  initializeRayTrace(); // for ray trace initialization
  
  if(_ice) calculate();
  
}

void CurvedRay::initializeRayTrace(){
  
  _min = 999.;
  _good = false;
  _n_cuts = 10;
  _max_solution = 6;
  _found_angle_vector.resize(_max_solution);
  _min_angle_vector.resize(_max_solution);
  _mode.resize(_max_solution);
  _max_angle_vector.resize(_max_solution);
  _trajectory_distance.resize(_max_solution);
  _travel_time.resize(_max_solution);
  _tolerance = _dist_step;

}

void CurvedRay::dump_debug_graph(){
 
  if(_debug_graph){ delete _debug_graph; }
  
}

void CurvedRay::printout(){
  //dummy call so compiler doesnt complain
  std::cout <<__PRETTY_FUNCTION__ << "not implemented "<<std::endl;
}

// getters
double CurvedRay::getHitTime() const {
 
  return _flight_time_total;
  
}

double CurvedRay::getDistance() const {
  
  return _distance_total;
  
}

double CurvedRay::getAttenuation() const {
 
  return _attenuation_total;
  
}

OpticalIce *CurvedRay::getOpticalIce(){
  
  return _ice;
  
}

Pos CurvedRay::getAntennaPos() const {
  
  return _antenna_pos;
  
}

Pos CurvedRay::getSourcePos() const {
  
  return _source_pos;
  
}

double CurvedRay::getDepthStep(){

  return _depth_step;
  
}

double CurvedRay::getHorStep(){
  
  return _hor_step;
  
}

double CurvedRay::getThetaStep() const {
  
  return _theta_step;
  
}

double CurvedRay::getPhiLaunchAngle() const {
 
  return _launch_phi_angle;
  
}

double CurvedRay::getThetaLaunchAngle() const {
  
  return _launch_theta_angle;
  
}

TMultiGraph *CurvedRay::getDebugGraph() const {
  
  return _debug_graph;
  
}

// setter
void CurvedRay::setSourcePosition(Pos position){
 
  _source_pos=position;
  
  findThetaPhi();
  
  calculate();
  
}

void CurvedRay::setSourcePosition(std::vector<double> cartesian_pos){
  
  Pos position(cartesian_pos);
  
  setSourcePosition(position);
  
}

void CurvedRay::setSourcePosition(double x, double y, double z){
  
  Pos position(x, y, z);
  
  setSourcePosition(position);
}
  
void CurvedRay::adjustSourcePosition(Pos position){
  
  if(_source_pos.getDistance(position)<=_small_step){ setSourcePosition(position); return; }
  
  _source_pos=position;
    
  calculate();
  
}
  
void CurvedRay::adjustSourcePosition(std::vector<double> cartesian_pos){
  
  Pos position(cartesian_pos);
  
  adjustSourcePosition(position);
  
}

void CurvedRay::adjustSourcePosition(double x, double y, double z){ 
  
  Pos position(x,y,z);
  
  adjustSourcePosition(position);
  
}

void CurvedRay::setOpticalIce(OpticalIce *ice){
  
  _ice=ice;
  
}

void CurvedRay::useCurvedRays(int use_curved_rays){
  
  _use_curved_rays=use_curved_rays;
  
}

// calculators
void CurvedRay::calculate(){

  switch(_use_curved_rays) {

  case UNIFORM:   
    calculateUniformIndexTrace(); break;   // if ice is uniform do the simplest calculation
    
  case STRAIGHT:
    calculateStraightLineApprox(); break; // if ice is non-uniform use straight line approx

  case NTIME:
    nTimeExpected(); break;  // n time expected
  
  case CURVED:
   calculateCurvedRaytrace(); break; // if you ask specifically for curved ray tracing algorithm.
   
  default: 
    calculateUniformIndexTrace(); break;
  }


}

void CurvedRay::findThetaPhi(){
  
  Pos delta=_source_pos - _antenna_pos;
  
  _launch_phi_angle=delta.getPhi();
  _launch_theta_angle=delta.getTheta();
  
}

void CurvedRay::calculateUniformIndexTrace(){

  _distance_total=_antenna_pos.getDistance(_source_pos);
  _flight_time_total=_distance_total * _ice->getIndex(_source_pos.getZ())/C_SPEED;
  _attenuation_total=_ice->getAttenuation(_source_pos.getZ(), _distance_total);
  
}

void CurvedRay::calculateStraightLineApprox(){

  
  double z_start=_antenna_pos.getZ();
  double z_finish=_source_pos.getZ();
  
  if(fabs(z_start - z_finish)<_depth_step){  // if vertical separation is less than one step, just use uniform index.
    
    calculateUniformIndexTrace();
    return;
    
  }
  
  if(z_start>z_finish){// make sure z_start is lower down than z_finish
    
    double z_temp=z_start;
    z_start=z_finish;
    z_finish=z_temp;
    
  }
  
  findThetaPhi();  
    
  _flight_time_total=0;
  _attenuation_total=0;
  
  double z=z_start;
  
  for(int i=0;i<1e6;i++){
   
    double line_length=_depth_step/cos(_launch_theta_angle)*_ice->getIndex(z);
    _flight_time_total+=line_length/C_SPEED;
    _attenuation_total*=_ice->getAttenuation(z, line_length);
    
    z+=_depth_step;
    if(z>=z_finish) break;
    
  }// for i
  
  _distance_total=(z_finish-z_start)/cos(_launch_theta_angle);
  
  
}

void CurvedRay::nTimeExpected(){
 
  //  std::cout << __PRETTY_FUNCTION__<< std::endl;
  double A= _ice->getDeepIceIndex();
  //  double B=A*-0.24;
  double B = _ice->getFirnIceIndex() - _ice->getDeepIceIndex();
    //  double C=16.;
  double C = _ice->getExpSlope()*1000;

  //in km
  double zmin = _antenna_pos.getZ()/1000;
  double zmax = _source_pos.getZ()/1000;
  
  double distanceTotal = _antenna_pos.getDistance(_source_pos)/1000;
  

  
  if(zmin > zmax){
    double temp = zmin;
    zmin = zmax;
    zmax = temp;
  }
  
  double cTheta;
  if(distanceTotal > 0){
    cTheta = fabs((_antenna_pos.getZ()/1000) - (_source_pos.getZ()/1000))/distanceTotal;
    
  }else {
    _flight_time_total = 0; 
    return;
  }
  
  _flight_time_total = distanceTotal /C_SPEED;
  
  if(cTheta!=0){
    if(zmax<0. && zmin<0.)
      _flight_time_total = (A*(zmax-zmin)+(B/C)*(exp(C*zmax)-exp(C*zmin)))/(C_SPEED*cTheta);
    if(zmax>=0. && zmin<0.)
      _flight_time_total = (A*(0.-zmin)+(B/C)*(exp(C*0.)-exp(C*zmin))+zmax)/(C_SPEED * cTheta);
    if(zmax>=0. && zmin>=0.)
      _flight_time_total=(zmax-zmin)/(C_SPEED*cTheta);
    if(zmax<0. && zmin>0.){
      std::cerr<<"Condition not covered in NTimeZ!!! using T = d/C "<< std::endl;
      //      calculateUniformIndexTrace(); //use this method
    }
  }
    _flight_time_total *=1000;  // km2m
    _distance_total = 1000*fabs((_antenna_pos.getZ()/1000) - (_source_pos.getZ()/1000)) / cTheta;
}

void CurvedRay::calculateCurvedRaytrace(){

  //  std::cerr<<"calculateCurvedRaytrace() not yet implemented...\n";
  rayTrace();
  _flight_time_total = getRayTraceLowestTime();

}
double CurvedRay::getRayTraceLowestTime(){
  
  if(_n_solution == 0) return 0;
  double min = _travel_time[0];

  for(int i=0; i< _n_solution; i++){
    if(_travel_time[i] < min )
      min = _travel_time[i];
  }

  return min;
}

double CurvedRay::binarySearch(double minTheta, double maxTheta, int nSolution, int trialNum){
  
  rayStruct *rs = new rayStruct();
  
  double tempMinTheta = minTheta;
  double tempMaxTheta = maxTheta;
  
  double halfAngle = (tempMinTheta+tempMaxTheta)/2;
  
  for(int i=0; i<1000; i++){
    
    halfAngle = (tempMinTheta+tempMaxTheta)/2;
    rayPath(halfAngle, _xy_dist, rs);
 

    if( fabs(tempMaxTheta - tempMinTheta) < .00000000001){
      if(rs->absMin < _dist_step){
	_trajectory_distance[nSolution] = rs->trajectoryDistance;
	_travel_time[nSolution] = rs->travelTime;
	delete rs;
	return halfAngle;
      }
      else {
	delete rs;
	return 999;
      }
    }
    if(rs->absMin< _tolerance){
      _trajectory_distance[nSolution] = rs->trajectoryDistance;
      _travel_time[nSolution] = rs->travelTime;
      delete rs;
      return halfAngle;
    }
    
    if(_mode[trialNum] == 1){
      if(rs->min < 0) 
	tempMaxTheta = halfAngle;
      
      else if(rs->min > 0 )
	tempMinTheta = halfAngle;
      else {
	printf("Unresolved situation, exiting... \n");
	assert(0);
      }
    }
    else if(_mode[trialNum]  == 2){
      if(rs->min > 0) tempMaxTheta = halfAngle;
      else if(rs->min > 0) tempMinTheta = halfAngle;
      else {
	printf("Unresolved situation, exiting.... \n");
	assert(0);
      }
    }

    else {
      printf("Unresolved situation, exiting.... \n");
      assert(0);
    }

  }
  delete rs;
  return 999;
}

void CurvedRay::convertCartesian2Spherical(double origx, double origy, double origz, double vecx, double vecy, double vecz, double &theta, double &phi, double &R){
 

  //R = distance3D(origx, origy, origz, vecx, vecy, vecz);
  R = _antenna_pos.getDistance(_source_pos);
  
  if(R == 0){
    phi = nan("");
    theta = nan("");
    return;
  }

  else {
    theta = acos((vecz - origz) / R);
    double top = (vecy - origy);
    double bottom = (vecx - origx);
    
    phi = atan2(top, bottom);

    if(phi < 0){
      phi += 2*TMath::Pi();
    }
  }
}

void CurvedRay::rayTrace(){

  _success = 0;
  rayStruct *rs = new rayStruct();
  
  //set origin and destination
  rs->x[0] = _source_pos.getX();
  rs->x[1] = _antenna_pos.getX();
  
  rs->y[0] = _source_pos.getY();
  rs->y[1] = _antenna_pos.getY();
   
  rs->z[0] = _source_pos.getZ();
  rs->z[1] = _antenna_pos.getZ();

  double theta, phi, r;

  //convert cartesian to spherical to theta angle(and treat it as the max angle)
  // get theta angle between source and origin
  convertCartesian2Spherical(_source_pos.getX(), _source_pos.getY(), _source_pos.getZ(), _antenna_pos.getX(), _antenna_pos.getY(), _antenna_pos.getZ(), theta, phi, r);


  double maxTheta = theta;
  double minTheta = 0;

  int tempI = 0;
  while((r/(pow(1000,tempI))) > 1000) {
    tempI++;
    _tolerance = _dist_step*pow(10, tempI); // what is tolerance
  }
  
  int nCuts = _n_cuts;
  int iCase = 0;

  //angles are in radians

  //case 1 (source above ice, destination below ice)
  if((_source_pos.getZ() > _antenna_pos.getZ()) && ((_source_pos.getZ() > 0) && (_antenna_pos.getZ() < 0)) ) {
    minTheta = theta;
  }
  
  // case 2 (both below ice, source above destination)
  else if(  ((_source_pos.getZ() < 0) && (_antenna_pos.getZ() < 0)) && (_source_pos.getZ() > _antenna_pos.getZ()) ) {

    minTheta = asin(1/_ice->getIndex(_source_pos.getZ()));
    }
  // case 3 (source below ice destination above ice)
  else if ( (_source_pos.getZ() < _antenna_pos.getZ()) && (( _source_pos.getZ() < 0) && (_antenna_pos.getZ() > 0))) {
    minTheta = (sin(maxTheta))/_ice->getIndex(_source_pos.getZ());

  }

  //case 4 both below ice, source below destination)
  else if (((_source_pos.getZ() < 0 ) && (_antenna_pos.getZ() < 0) && (_source_pos.getZ() < _antenna_pos.getZ()))){
    
    // minTheta = (sin(maxTheta))/_ice->getIndex(_source_pos.getZ());
    minTheta = asin((sin(maxTheta))/_ice->getIndex(_source_pos.getZ()));
  }
  // case 5 both above ice
  else {
    minTheta = maxTheta;
    printf("Case 5 Used. Min/Max Theta = %.3f/%.3f \n",minTheta*(180/(TMath::Pi())),maxTheta*(180/(TMath::Pi())));
    nCuts=-1;
    iCase = 5;
  }

  double cutStep = (maxTheta - minTheta)/nCuts;

  double *minAry = new double[nCuts+3];
  double *angAry = new double[nCuts+3];
  
  double XYDist = distance2D(_source_pos.getX(), _source_pos.getY(), _antenna_pos.getX(), _antenna_pos.getY());
  
  _xy_dist = XYDist;
  
  for(int iCut=0; iCut<=nCuts && iCase!=5; iCut++){
    
    double testTheta = minTheta + (iCut*cutStep);   // testtheta is the trajectory angle
    
    rayPath(testTheta, XYDist, rs); // to get the path of the ray
    
    minAry[iCut] = rs->min; 
    angAry[iCut] = testTheta;
  
  }  
    int nPointsFound = findPointsOfInterest(minAry, angAry);
    int nSol = 0;

    for(int i=0; i<nPointsFound && iCase!=5; i++){
      
      _found_angle_vector[nSol] = binarySearch(_min_angle_vector[i], _max_angle_vector[i], nSol, i);
      
      if(_found_angle_vector[nSol] != 999){
	_success = 1;
	nSol++;
      }
    }
    _n_solution = nSol;
 
    if(iCase == 5){
      _n_solution = 1;
      rayPath(maxTheta, XYDist, rs);
      _trajectory_distance[0] = rs->trajectoryDistance;
      _travel_time[0] = rs->travelTime;
      _found_angle_vector[0] = minTheta;
    }
     
    delete rs;
}
    

int CurvedRay::rayPath(double trajectoryAngle, double XYDist, rayStruct *rs){
    
  //same as volley

  bool good = (XYDist >0);
  bool topref = false;
  double Z0 = _source_pos.getZ();
  double zTraveled = 0;
  double NxtAng = trajectoryAngle;
  double Distance =0;
  double TTime = 0;
  double nextZ0;
  
  topref = false;
  
  bool topReflection = false;
  
  int printcnt = 0;
  
  for(int i=0; good; i++){
    
    double xyStep = _dist_step*sin(NxtAng);
    double zStep = _dist_step*cos(NxtAng);
    
    nextZ0 = Z0+zStep;
    
    //if the source position is below ice, and next step is and above, OR
    // if the source position is above ice, and the next step is below ice
    //    if( ((Z0<0) && (nextZ0 > 0 ) || ((Z0>0) && (nextZ0 < 0) ) ){
    if( ((Z0 < 0) && (nextZ0 > 0)) || ((Z0 > 0) && (nextZ0 < 0) )){
	
	if( (Z0< 0) && (nextZ0 > 0 ) ){
	  //if the source is below ice and next step is above ice
	  // set the next step to 0
	  nextZ0 = 0;
	  zStep = fabs(Z0);
	}

	if( (Z0>0) && (nextZ0 < 0) ) {
	  // if the source is above ice 
	  // and the the next step is below ice
	  nextZ0 = -1e-100;
	  zStep = -fabs(Z0);
	}
	
	//don't know what he is doing here
	xyStep = fabs(zStep)*tan(NxtAng);
	double tempDstep = fabs(zStep)/cos(NxtAng);
	Distance += tempDstep;
	TTime += tempDstep/(C_SPEED/_ice->getIndex(Z0));
      }
      else {
	Distance += _dist_step;
	TTime += _dist_step/(C_SPEED/_ice->getIndex(Z0));
      }

      printcnt++;
      
      if( (Z0==0) && (topref) ) {
	Z0 = -1e-100;
      }

      NxtAng = getRefractionAngle(NxtAng, _ice->getIndex(Z0), _ice->getIndex(nextZ0), topref);
      
      Z0 = nextZ0;
      XYDist -= xyStep;
      zTraveled += zStep;
      good = (XYDist> 0);
      
      
      if(topref){
	topReflection = true;
      }
      
  }

  rs->trajectoryDistance = Distance;
  rs->travelTime = TTime;
  rs->min = (Z0-_antenna_pos.getZ());
  rs->absMin = fabs(Z0 - _antenna_pos.getZ());
  rs->reflect = topReflection;
    
  if(topref && Z0<rs->z[1]) return 1;
  else if(topref && Z0 > rs->z[1]) return 2;
  else if(Z0 > rs->z[1]) return 1;
  else if(Z0 < rs->z[1]) return 2;
  else return 0;
  
}


double CurvedRay::getRefractionAngle(double incidentAngle, double incidentIndex, double refractionIndex, bool &refCheck){
  
  double criticalAngle = asin(refractionIndex/incidentIndex);
  bool critNan = TMath::IsNaN(criticalAngle);
  bool adjusted = false;
    
  double adjustedAngle = incidentAngle;
  double outAngle;
  refCheck = false;

  if(adjustedAngle>(TMath::Pi()/2)) {
    adjustedAngle = (TMath::Pi()) - incidentAngle;
    adjusted = true;
  }

  if( (adjustedAngle > (TMath::Pi()/2) ) || (adjustedAngle < 0 )){
    printf("GetRefractionAngle() (code 0): Problems finding proper incident angle \n");
    assert(0);
  }
    
  if(adjustedAngle > criticalAngle && !critNan && !adjusted) {
    
    outAngle = (TMath::Pi()) - adjustedAngle;
    refCheck = true;
  }

  else {
    double nextAngle = asin( (incidentIndex/refractionIndex)*sin(adjustedAngle));
    if(adjusted) {
      outAngle = (TMath::Pi()) - nextAngle;
    }else {
      outAngle = nextAngle;
    }
    
  }
  return outAngle;
}
      
int CurvedRay::findPointsOfInterest(double *minAry, double *angAry){
    
  int nPoints = 0;
    
  //_nCuts
  for(int i=0; i<_n_cuts; i++){
    double first = minAry[i];
    double second = minAry[i+1];
      
    if( ((first<0) && (second > 0)) || ((first > 0) && (second < 0)) ){
      if((first > 0) && (second < 0))
	_mode[nPoints] = 1;
      
      else if((first < 0) && (second > 0)) 
	_mode[nPoints] = 2;
      else 
	_mode[nPoints] = 0;
      
      
      _min_angle_vector[nPoints] = angAry[i];
      _max_angle_vector[nPoints] = angAry[i+1];
	
      nPoints++;
    }
    
  }

  return nPoints;
}

double CurvedRay::distance2D(double x1, double y1, double x2, double y2){
   
  return sqrt( ((x1-x2)*(x1-x2)) + ((y1-y2)*(y1-y2)));
}

OpticalIce *CurvedRay::_ice=0;
int CurvedRay::_use_curved_rays=0;
double CurvedRay::_small_step=1;
double CurvedRay::_hor_step=1;
double CurvedRay::_depth_step=1;
double CurvedRay::_dist_step = 0.01;

//const double CurvedRay::C_SPEED=0.3; // meters/n
const double CurvedRay::C_SPEED=2.998e-1; // meters/n
const double CurvedRay::PI=3.14159265358979312;
const double CurvedRay::RADTODEG=57.2957795130823229;
const double CurvedRay::DEGTORAD=0.0174532925199432955;

