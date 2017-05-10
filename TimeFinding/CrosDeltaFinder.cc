#include "CrosDeltaFinder.h"
#include "Channel.h"

ClassImp(CrosDeltaFinder);

CrosDeltaFinder::CrosDeltaFinder() : DeltaFinder () { initialize(); }

CrosDeltaFinder::CrosDeltaFinder(Channel *chan1, Channel *chan2, double par1, double par2, int stationID, int is_simulated) : DeltaFinder(chan1, chan2, par1, par2, stationID, is_simulated){
  
  initialize();
  
  scanWaveform();
  
}

CrosDeltaFinder::CrosDeltaFinder(const CrosDeltaFinder &other) : DeltaFinder(other) {
 
  // this is needed if only to make sure there's a call to DeltaFinder's copy constructor
  
}

CrosDeltaFinder::~CrosDeltaFinder() {}

void CrosDeltaFinder::initialize(){
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  setFinderName("cros");
  setFinderType(DeltaFinder::CROS);

  _thresh=1;
  _scale=4;
  
}

DeltaFinder *CrosDeltaFinder::getDeepCopy(){
 
  return new CrosDeltaFinder(*this);
  
}

void CrosDeltaFinder::calculateTime(){
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  CorrGraph *grCorr=getFirstChannel()->getCorrGraph(getSecondChannel()); // this checks if the relevant CorrGraph exists, and creates it if it doesn't
  
  int N=grCorr->getN();
  double *Y=grCorr->getY();
  double *X=grCorr->getX();
  
  int best_bin=0;
  double best_amp=0;
  
  for(int i=0;i<N;i++) if(fabs(Y[i])>best_amp){
       
    best_amp=fabs(Y[i]);
    best_bin=i;
    
  }
  
  setTime(-X[best_bin]);// we need a minus to make it second channel - first channel

  calculateSignalWidthAmp(grCorr->getGraph(), best_bin, best_amp);
  
}
  
void CrosDeltaFinder::calculateSignalWidthAmp(TGraph *gr, int best_bin, double best_amp){
  
  TGraph *grSmooth=GausTimeFinder::gausmooth(gr, 50, 50);
      
  int N=grSmooth->GetN();
  double *Y=grSmooth->GetY();
  double *X=grSmooth->GetX();
  
  // calculate the QP and errors
  int FWHM_above=best_bin;
  int FWHM_below=best_bin;
  int peak_above=best_bin;
  int peak_below=best_bin;
  int edge_above=best_bin;
  int edge_below=best_bin;
    
  for(int j=1;j<N;j++){ // go above the peak to 50% height

    FWHM_above=j+best_bin;
    if(j+best_bin>=N) break;
    if(fabs(Y[j+best_bin])<0.5*best_amp) break;
    
  }
        
  for(int j=1;j<N;j++){ // go above the peak to 90% height

    peak_above=j+best_bin;
    if(j+best_bin>=N) break;
    if(fabs(Y[j+best_bin])<0.9*best_amp) break;
        
  }
      
  for(int j=1;j<N;j++){ // go above the peak to 20% height

    edge_above=j+best_bin;
    if(j+best_bin>=N) break;
    if(fabs(Y[j+best_bin])<0.2*best_amp) break;
        
  }
  
  for(int j=1;j<N;j++){ // go below the peak to 50% height

    FWHM_below=best_bin-j;
    if(best_bin-j<0) break;
    if(fabs(Y[best_bin-j])<0.5*best_amp) break;
        
  }
        
  for(int j=1;j<N;j++){ // go below the peak to 90% height

    peak_below=best_bin-j;
    if(best_bin-j<0) break;
    if(fabs(Y[best_bin-j])<0.9*best_amp) break;
    
  }
      
  for(int j=1;j<N;j++){ // go below the peak to 90% height

    edge_below=best_bin-j;
    if(best_bin-j<0) break;
    if(fabs(Y[best_bin-j])<0.2*best_amp) break;
    
  }
  
  _signal_width=fabs(X[FWHM_above]-X[FWHM_below]);
  
  double noise_var=0;
  
  for(int i=0;i<edge_below;i++) noise_var+=Y[i]*Y[i];
  if(edge_above<N) for(int i=edge_above+1;i<N;i++) noise_var+=Y[i]*Y[i];
  
  noise_var/=(edge_above-edge_below); // normalize by the number of samples...
  
  if(noise_var>0) _quality_parameter=best_amp/std::sqrt(noise_var);
  else _quality_parameter=best_amp/1000; // hopefully we never need this default value...
  
//   printf("V= %8.1f | QP= %8.6f | edges: %04d %04d | peaks: %04d %04d | width= %6.4f\n", noise_var, _quality_parameter, edge_below, edge_above, peak_below, peak_above, _signal_width);
  
}

void CrosDeltaFinder::calculateError(){
 
  setError(_signal_width/2.355); // convert FWHM to 1sigma error
  
}
