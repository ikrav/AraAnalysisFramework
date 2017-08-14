#include "AnalyticVertexFinder.h"

ClassImp(AnalyticVertexFinder);

int AnalyticVertexFinder::debug_bit=0;

AnalyticVertexFinder::AnalyticVertexFinder() : VertexFinder() {}

AnalyticVertexFinder::AnalyticVertexFinder(ChannelCollection channels, VertexPos *initial_position, int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice_ptr, double par1, double par2, double par3) : VertexFinder(channels, initial_position, coordinate_system, time_input, lock_parameter, ice_ptr, par1, par2, par3){
  
  std::cout <<__PRETTY_FUNCTION__ <<std::endl;

  initialize();
}

AnalyticVertexFinder::AnalyticVertexFinder(int coordinate_system, int time_input, int lock_parameter, OpticalIce *ice, double par1, double par2, double par3) : VertexFinder(coordinate_system, time_input, lock_parameter, ice, par1, par2, par3){

  initialize();

}

AnalyticVertexFinder::AnalyticVertexFinder(const std::string commands) : VertexFinder() {
  
  initialize();
  
  parse(commands);
  
}

AnalyticVertexFinder::AnalyticVertexFinder(ChannelCollection channels, VertexPos *initial_position, const std::string commands) : VertexFinder(channels, initial_position){
  
  initialize();
  
  parse(commands);
  
}

AnalyticVertexFinder::AnalyticVertexFinder(const AnalyticVertexFinder & other) : VertexFinder(other) {
    
  _indx_refraction1=other._indx_refraction1;
  _indx_refraction2=other._indx_refraction2;
  _indx_refraction3=other._indx_refraction3;  
  
  _n_ant_set=other._n_ant_set;
  for(int i=0;i<4;i++) for(int j=0;j<4;j++) _ant_info[i][j]=other._ant_info[i][j];
  
  _x=other._x;
  _y=other._y;
  _z=other._z;
  _t=other._t;
  _n=other._n; 
  
}

AnalyticVertexFinder::~AnalyticVertexFinder(){
  
  
}

void AnalyticVertexFinder::initialize(){
  
  _n_ant_set = 0;
  
  reset();
  
}

void AnalyticVertexFinder::help(){
  
  VertexFinder::help();
  
}

void AnalyticVertexFinder::parse(const std::string commands){
  
  VertexFinder::parse(commands);
  
}

std::string AnalyticVertexFinder::getFinderName() const {

  return "asm";
  
}

VertexFinder *AnalyticVertexFinder::getDeepCopy(){
 
  return new AnalyticVertexFinder(*this);
  
}

void AnalyticVertexFinder::setAnt(double x, double y, double z, double t){
  
  if(_n_ant_set >= 4) return;
  else {
    _ant_info[_n_ant_set][0] = x;
    _ant_info[_n_ant_set][1] = y;
    _ant_info[_n_ant_set][2] = z;
    _ant_info[_n_ant_set][3] = t;
    _n_ant_set++;
  }

}

void AnalyticVertexFinder::reset(){
  
  for(int i=0; i<4; i++){
    for(int j=0; j<4; j++){
      _ant_info[i][j] =0;
    }
  }

  _n_ant_set =0;
  _x=0;
  _y=0;
  _z=0;
}

void AnalyticVertexFinder::findVertex(ChannelCollection channels, VertexPos *initial_position){

  reset();
  setChannels(channels);
  setInitialPosition(initial_position);

  findVertex();
  
}

void AnalyticVertexFinder::findVertex(){

  if(checkIfSomethingMissing()) {
    setStatusFailed();
    return;
  }
  
  if(_coordinate_system != VertexFinder::CARTESIAN){
    std::cerr <<" ASM implementations works with cartesian "<< std::endl;
    //    return;
  }
  
  int count=0;
  double answerX=0;
  double answerY=0;
  double answerZ=0;
  AnalyticVertexFinder::reset();
  std::vector<double> posV;
  for(int ant1=0; ant1 < _channels.getNumChans(); ant1++){
    if(_channels[ant1]->getWeight() == 0) continue;
    for(int ant2=ant1+1; ant2 < _channels.getNumChans(); ant2++){
      if(_channels[ant2]->getWeight() == 0) continue;
      for(int ant3=ant2+1; ant3 < _channels.getNumChans(); ant3++){
	if(_channels[ant3]->getWeight() == 0) continue;
	for(int ant4=ant3+1; ant4 <_channels.getNumChans(); ant4++){
	  if(_channels[ant4]->getWeight() == 0) continue;
	  
	  posV.clear();
	  std::vector<double>().swap(posV);
	  posV = _channels[ant1]->getPosition().getCartesian();
	  setAnt(posV[0], posV[1], posV[2], _channels[ant1]->getTime());
	  
	  posV.clear();
	  std::vector<double>().swap(posV);
	  posV = _channels[ant2]->getPosition().getCartesian();
	  setAnt(posV[0], posV[1], posV[2], _channels[ant2]->getTime());
	  
	  posV.clear();
	  std::vector<double>().swap(posV);
	  posV = _channels[ant3]->getPosition().getCartesian();
	  setAnt(posV[0], posV[1], posV[2], _channels[ant3]->getTime());
	  
	  posV.clear();
	  std::vector<double>().swap(posV);
	  posV = _channels[ant4]->getPosition().getCartesian();
	  setAnt(posV[0], posV[1], posV[2], _channels[ant4]->getTime());
	  
	  //solve
	  bool status = solve();
	  if(status){
	    answerX +=_x;
	    answerY +=_y;
	    answerZ +=_z;
	    count++;
	  }
	}//ant4
      }//ant3
    }//ant2
  }//ant1
  
  answerX /= count;
  answerY /= count;
  answerZ /= count;
  if(count > 0){
    if(debug_bit) printf("ASM X: %f \t Y: %f \t Z: %f \n", answerX, answerY, answerZ);
    setCartesian(answerX, answerY, answerZ);
    Pos position = Pos(answerX, answerY, answerZ);
    double t0 = getInteractionTime(position);
    double chisq = myChi(position, t0);
    calculateConfidence(chisq);
    setStatusFound();
  }else{
    setCartesian(999, 999, 999);
    calculateConfidence(1e36);
    setStatusFailed();
  }
  
}

bool AnalyticVertexFinder::solve() {

  if(_n_ant_set != 4){
    printf("Analytic method needs four antennas with hit times.. Event not reconstructed\n");
    return false;
  }
  _n_ant_set=0;

  double temp[4];
  temp[0] = _ant_info[0][3];
  temp[1] = _ant_info[1][3];
  temp[2] = _ant_info[2][3];
  temp[3] = _ant_info[3][3];

  int order[4];
  orderer(temp, order, 4);

  double x1, y1, z1;
  double x2, y2, z2;
  double x3, y3, z3;
  double x4, y4, z4;

  x1 = _ant_info[order[0]][0];
  y1 = _ant_info[order[0]][1];
  z1 = _ant_info[order[0]][2];

  x2 = _ant_info[order[1]][0];
  y2 = _ant_info[order[1]][1];
  z2 = _ant_info[order[1]][2];

  x3 = _ant_info[order[2]][0];
  y3 = _ant_info[order[2]][1];
  z3 = _ant_info[order[2]][2];

  x4 = _ant_info[order[3]][0];
  y4 = _ant_info[order[3]][1];
  z4 = _ant_info[order[3]][2];

  //find index of refraction
  _indx_refraction1 = (C()/(1e9))/_ice->getIndex((z1+z2)/2);
  _indx_refraction2 = (C()/(1e9))/_ice->getIndex((z1+z3)/2);
  _indx_refraction3 = (C()/(1e9))/_ice->getIndex((z1+z4)/2);
  double deltaD12, deltaD13, deltaD14;
  deltaD12 = (_ant_info[order[1]][3] - _ant_info[order[0]][3])*_indx_refraction1;
  deltaD13 = (_ant_info[order[2]][3] - _ant_info[order[0]][3])*_indx_refraction2;
  deltaD14 = (_ant_info[order[3]][3] - _ant_info[order[0]][3])*_indx_refraction3;

  double RD12, RD13, RD14;
  RD12 = sQ(x2)+sQ(y2)+sQ(z2) - (sQ(x1)+sQ(y1)+sQ(z1)) - sQ(deltaD12);
  RD13 = sQ(x3)+sQ(y3)+sQ(z3) - (sQ(x1)+sQ(y1)+sQ(z1)) - sQ(deltaD13);
  RD14 = sQ(x4)+sQ(y4)+sQ(z4) - (sQ(x1)+sQ(y1)+sQ(z1)) - sQ(deltaD14);

  double alpha0, alpha1, alpha2, alpha3;
  alpha0 =(y1-y4 - (y1-y2)*((x1-x4)/(x1-x2)) )/(y1-y3-(y1-y2)*((x1-x3)/(x1-x2)));
  alpha1 = alpha0*(RD12*((x1-x3)/(x1-x2)) - RD13)+ RD14 - RD12*((x1-x4)/(x1-x2));
  alpha2 = 2 * ( alpha0 * (deltaD13 - deltaD12*((x1-x3)/(x1-x2))) + deltaD12*((x1-x4)/(x1-x2)) - deltaD14);
  alpha3 = 2 * ( alpha0 * ( (z1-z2)*((x1-x3)/(x1-x2)) -z1+z3) + z1 - z4 - (z1-z2)*((x1-x4)/(x1-x2)));

  // X(d) Terms                                                                                    

  double beta0, beta1, beta2, beta3;
  beta0 = ((z1-z4)*((y1-y3)/(y1-y4)) -z1+z3) / (z1-z2-(z1-z4)*((y1-y2)/(y1-y4)));
  beta1 = beta0 * (RD12-RD14*((y1-y2)/(y1-y4))) + RD13 - RD14*((y1-y3)/(y1-y4));
  beta2 = 2 * ( beta0 * (deltaD14*((y1-y2)/(y1-y4)) - deltaD12) + deltaD14*((y1-y3)/(y1-y4)) - deltaD13);
  beta3 = 2 * ( beta0 * (x1-x2-(x1-x4)*((y1-y2)/(y1-y4))) + x1 - x3 - (x1-x4)*((y1-y3)/(y1-y4)) );

  // Y(d) Terms                                                                                    

  double gamma0, gamma1, gamma2, gamma3;
  gamma0 = ((x1-x3)*((z1-z2)/(z1-z3)) -x1+x2) / (x1-x4 - (x1-x3)*((z1-z4)/(z1-z3)));
  gamma1 = gamma0 * (RD14-RD13*((z1-z4)/(z1-z3))) + RD12 - RD13*((z1-z2)/(z1-z3));
  gamma2 = 2 * ( gamma0 * (deltaD13*((z1-z4)/(z1-z3)) - deltaD14) + deltaD13*((z1-z2)/(z1-z3)) - deltaD12);
  gamma3 = 2 * ( gamma0 * (y1-y4-(y1-y3)*((z1-z4)/(z1-z3))) + y1 - y2 - (y1-y3)*((z1-z2)/(z1-z3)));

  double QuadTerms, LinTerms, Cons;
  QuadTerms = sQ(beta2/beta3) + sQ(gamma2/gamma3) + sQ(alpha2/alpha3) - 1;
  LinTerms = 2*( (beta1*beta2)/sQ(beta3) + (gamma1*gamma2)/sQ(gamma3) + (alpha1*alpha2)/sQ(alpha3)+ x1*(beta2/beta3) + y1*(gamma2/gamma3) + z1*(alpha2/alpha3) );
  Cons = sQ(beta1/beta3) + sQ(gamma1/gamma3) + sQ(alpha1/alpha3) + 2*(x1*(beta1/beta3) + y1*(gamma1/gamma3) + z1*(alpha1/alpha3)) +sQ(x1)+ sQ(y1)+sQ(z1);

  // Solve for d with quadratic equation                                                           
  double timepos, timeneg;
  quad(QuadTerms,LinTerms,Cons,timepos,timeneg);
  // Positive solution is taken                                                                    
  if(timepos>0 && timeneg<0) {
    _x = -(beta2*timepos + beta1)/beta3;
    _y = -(gamma2*timepos + gamma1)/gamma3;
    _z = -(alpha2*timepos + alpha1)/alpha3;
  }  else if(timepos<0 && timeneg>0) {
    _x = -(beta2*timeneg + beta1)/beta3;
    _y = -(gamma2*timeneg + gamma1)/gamma3;
    _z = -(alpha2*timeneg + alpha1)/alpha3;
  }else{
    // If both negative or positive returns false                                                   
    return false;
  }
  //solution found
  // Calculate time to antenna 1 based on coordinates found                                   
  Pos position = Pos(_x,_y,_z);
  _channels[0]->getRay()->adjustSourcePosition(position);
  //_t = _channels[0]->getTimeOffsetFromRay();
  _t = _channels[0]->getRay()->getHitTime();
  
  return true;
}

double AnalyticVertexFinder::dist3D(double x1, double y1, double z1, double x2, double y2, double z2){
  
  return sqrt(
	      ((x2-x1)*(x2-x1))
	       +((y2-y1)*(y2-y1))
	      +((z2-z1)*(z2-z1))
	      );
}

double AnalyticVertexFinder::sQ(double value) { 
  
  return value*value;
  
}

void AnalyticVertexFinder::orderer(double *values, int *order, int n){
  
  double valord[n];
  double high=0;

  for (int i=0; i<n; i++) {
    valord[i] = values[i];
    high += fabs(values[i]);
  }

  for(int i = 0;i<n;i++) {
    double checker = high;
    for (int j = 0;j<n;j++) {
      if (valord[j]<=checker) {
        checker = values[j];
        order[i] = j;
      }
    }
    valord[order[i]] = high+1;
  }

}

float AnalyticVertexFinder::nTimeExpected(float z1, float z2, float dtot){
  
  //function not used, will be deleted
  const float km2m=1e3;
  const float cVac=2.998e-1;

  z1 /= 1000;
  z2 /= 1000;
  dtot /= 1000;

  float A=1.78;
  float B=A*-0.24;
  const float C=16.;
  float zmax=z2;
  float zmin=z1;
  if(zmin>zmax){
    zmin=z2;
    zmax=z1;
  }
  float ctheta;
  if(dtot>0)
    ctheta=fabs(z2-z1)/dtot;
  else
    return 0;
  float ftimexp=dtot/cVac;
  if(ctheta!=0){
    if(zmax<0. && zmin<0.)
      ftimexp=(A*(zmax-zmin)+(B/C)*(exp(C*zmax)-exp(C*zmin)))/(cVac * ctheta);
    if(zmax>=0. && zmin<0.)
      ftimexp=(A*(0.-zmin)+(B/C)*(exp(C*0.)-exp(C*zmin))+zmax)/(cVac * ctheta);
    if(zmax>=0. && zmin>=0.)
      ftimexp=(zmax-zmin)/(cVac * ctheta);
    if(zmax<0. && zmin>0.)
      printf("CONDITION NOT COVERED IN NTIMEZ!!!\n");
  }
  return ftimexp*km2m; //km->meters                                                                                                                                                          
}

void AnalyticVertexFinder::quad(double a, double b, double c, double &pos, double &neg){
  
  double determinant  = (b*b) - (4*a*c);
  pos = (-b + sqrt(determinant))/(2*a);
  neg = (-b - sqrt(determinant))/(2*a);
}

double AnalyticVertexFinder::nOfZ(double z){
  //function not used, will be deleted
  z /= 1000;
  const double a = _n;
  const double b = -.24 * a;
  const double c = 16;
  double n = a + b * exp(c*z);
  return n;
  
}

double AnalyticVertexFinder::C(){
   
  return 2.99792458e8; 
  
}
