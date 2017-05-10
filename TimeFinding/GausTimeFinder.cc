#include "GausTimeFinder.h"

ClassImp(GausTimeFinder);

GausTimeFinder::GausTimeFinder() : TimeFinder() {}

GausTimeFinder::GausTimeFinder(TGraph *waveform, double par1, double par2, int channelID, int stationID, int isSimulated): TimeFinder(waveform, par1, par2, channelID, stationID, isSimulated) { 
  
  initialize();
  _waveform = waveform;
  scanWaveform();
}

GausTimeFinder::GausTimeFinder(const GausTimeFinder &other) : TimeFinder(other){
 
  _average_error=other._average_error;
  
}

GausTimeFinder::~GausTimeFinder(){}

TimeFinder *GausTimeFinder::getDeepCopy(){
 
  return new GausTimeFinder(*this);
  
}

void GausTimeFinder::initialize(){

  setFinderName("gaus");
  setFinderType(TimeFinder::GAUS);
  
  double max_clarity=12;
  _average_error=3;
  _thresh=4;
  
  if(_thresh>=max_clarity) _scale=1;
  _scale=(max_clarity-_thresh);
  
  
}

void GausTimeFinder::calculateTime(){
  double clarity=0;
  double time=0;
    
  getPeak(_waveform, time, clarity);

  _quality_parameter=clarity;
  
  //  double weight=calculateWeight(clarity);
  double error=_average_error;
  
  //  ChannelTime channel;

  //  channel.setTimeWeightError(time, weight, error);
  //  channel.setQualityParameter(clarity);// keep the unnormalized QP as well...
  
  setTime(time);
  calculateWeight();
  calculateError();
  
//   std::cout<<"clarity= "<<clarity<<" weight= "<<_weight<<std::endl;
  
  
}

void GausTimeFinder::getPeak(TGraph *gr, double &time, double &clarity, int numBins){
  
  TGraph *grs=gausmooth(gr, 5, 5.0);
  
  int N=grs->GetN();
  double *Y=grs->GetY();
  double *X=grs->GetX();
  
  getPeak(N,X,Y, time, clarity, numBins);
  
  delete grs;
}// getPeak (_graphs_vectoraph)

void GausTimeFinder::getPeak(int N, double *X, double *Y, double &time, double &clarity, int numBins){

  int max_bin=0;
  double max_position=0;
  double max_value=0;
  
  double mean=0;
  
  // get the mean:
  for(int i=0;i<N;i++) mean+=Y[i];
  mean=mean/N;
  
  for(int i=0;i<N;i++){
    
    if(Y[i]>max_value){// find maximum
     
      max_bin=i;
      max_position=X[i];
      max_value=Y[i];
      
    }
    
  }// for i
  
  for(int j=0;j<numBins;j++){// flatten out area around the peak

    if(max_bin+j<N)  Y[max_bin+j]=mean;
    if(max_bin-j>=0) Y[max_bin-j]=mean;
    

  }// for j

  mean=0; // recalculate the new mean
  for(int i=0;i<N;i++) mean+=Y[i];
  mean=mean/N;
  
  time=max_position;
  clarity=max_value/mean;
}// getPeak (array)

TGraph *GausTimeFinder::gausmooth(TGraph *gr, int numBins, double sigma){
  
  int N=gr->GetN();
  double *X=gr->GetX();
  double *Y=gr->GetY();

  double mean=std::fabs(gr->GetMean(2));// to put into the edges of the waveform that don't get smoothed
  
  // make the gaussian shape
  double *gaus=new double[numBins];

  for(int i=0;i<numBins;i++){

    gaus[i]=std::exp(-0.5*std::pow((double)i/sigma,2))/std::sqrt(2*3.141592565358979312)/sigma;
    
  }
  
  double *newY=new double[N];
  double *absY=new double[N];
  for(int i=0;i<N;i++) absY[i]=std::fabs(Y[i]);
  
  for(int i=0;i<N;i++){
   
    if(i<numBins||i>=N-numBins) newY[i]=mean;// edges can't get smoothed by rolling average (put mean instead)
    else{// use weighted rolling average
     
      newY[i]=absY[i];
      for(int j=1;j<numBins;j++) newY[i]+=(gaus[j]*absY[i+j]+gaus[j]*absY[i-j]);
      
      
    }
    
  }// for i
  
  TGraph *grsmooth=new TGraph(N,X,newY);
  grsmooth->SetLineColor(kGreen);
  
  delete [] gaus;
  delete [] absY;
  delete [] newY;
  
  return grsmooth;  
}// gausmooth
