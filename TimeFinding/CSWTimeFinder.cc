#include "CSWTimeFinder.h"

ClassImp(CSWTimeFinder);

CSWTimeFinder::CSWTimeFinder() : TimeFinder() {

  _csw_gr=0;
  _zero_mean_waveform=0;
}

CSWTimeFinder::CSWTimeFinder(TGraph *waveform, TGraph *csw, double par1, double par2, int channelID, int stationID, int isSimulated) : TimeFinder(waveform, par1, par2, channelID, stationID, isSimulated){

  _csw_gr=0;
  _zero_mean_waveform=0;

  initialize();

  if(csw){
    _csw_gr = (TGraph*)csw->Clone();
  }
  scanWaveform();  
}

CSWTimeFinder::CSWTimeFinder(const CSWTimeFinder &other) : TimeFinder(other) {
 
  _csw_gr = new TGraph(*(other._csw_gr));
  _zero_mean_waveform = new TGraph(*(other._zero_mean_waveform));
  
}

CSWTimeFinder::~CSWTimeFinder(){

  if(_csw_gr) delete _csw_gr;
  _csw_gr=0;
 
  if(_zero_mean_waveform) delete _zero_mean_waveform;
  _zero_mean_waveform = 0;
  
}

TimeFinder *CSWTimeFinder::getDeepCopy(){
 
  return new CSWTimeFinder(*this);
  
}

CSWTimeFinder& CSWTimeFinder::operator=(const CSWTimeFinder &other){
 
  if(this==&other) return *this;
  
  if(other._csw_gr) _csw_gr = new TGraph(*(other._csw_gr));
  if(other._zero_mean_waveform) _zero_mean_waveform = new TGraph(*(other._zero_mean_waveform));
  
  TimeFinder::operator=(other);
  
}

void CSWTimeFinder::initialize(){
  _csw_gr = 0;  
  _finder_name = "CSW";
  _finder_type = 1;

  int maxNumSamples = 480;
  int sampleFactor = 1;
  //  TGraph *temp = CSWTimeFinder::getGraphWithLength(_waveform, maxNumSamples*sampleFactor);
  _zero_mean_waveform = getZeroMeanGraph(_waveform);
  //  if(temp) delete temp;
  //  temp=0;
}

void CSWTimeFinder::calculateTime(){
  double dT =0;
  
  double corr=0;
  
  if(getPar1() == 0.0){ 
    /*1st iteration of the CSW*/
    if( getPar2() == 0 ){
      /* if its the first channel set its dT to 0, and add it to CSW */
      dT = 0;
      _csw_gr = (TGraph*)_zero_mean_waveform->Clone();
    }else{
      /* for first iteration of the CSW */
      dT = processCSW();
    }
  }else {
    dT = processCSW2ndIter();
  }

  setTime(dT);

}

void CSWTimeFinder::calculateError(){

  setError(0.1);
}

void CSWTimeFinder::calculateWeight(){

  //will be changed to correlation value
  setWeight(0.1);
}

TGraph *CSWTimeFinder::getZeroMeanGraph(TGraph *gr){
  
  double *yVals = gr->GetY();
  double *xVals = gr->GetX();
  int numPoints = gr->GetN();
  
  double *yValsNew = new double[numPoints];
  double mean = gr->GetMean(2);
  
  for(int i=0; i < numPoints; i++){
    yValsNew[i] = yVals[i] - mean;
  }
  
  TGraph *grZM = new TGraph(numPoints, xVals, yValsNew);
  delete [] yValsNew;
  return grZM;
}

double CSWTimeFinder::processCSW(){
  /* take X-correlation with CSW and take the resulting time as hit time for this channel
     then add the waveform to the CSW, with the time as the offset */
  
  //this functions assumes both of the graphs to be zero meaned
  TGraph *grCorr = FFTtools::getNormalisedCorrelationGraph(_csw_gr, _zero_mean_waveform);
  
  int bin=0;
  double corr = FFTtools::getPeakVal(grCorr, &bin);
  double x,y;
  grCorr->GetPoint(bin, x, y);
  
  double dT =x; //this may need to be -x

  TGraph *grTemp = (TGraph*)_csw_gr->Clone();
  delete _csw_gr;
  _csw_gr = 0;
  
  _csw_gr = CSWTimeFinder::addGraphs(grTemp, _zero_mean_waveform, dT);

  if(grCorr) delete grCorr;
  grCorr = 0;

  if(grTemp) delete grTemp;
  grTemp =0;
  
  return dT;
}

double CSWTimeFinder::processCSW2ndIter(){
  /** This runs after all channels have done the 1st iteration
      implying -> we have the CSW with all the channels included
      
      1. we remove the waveform of this channel from the CSW
      2. take X-correlation between the waveform of this channel and the CSW without this channel
      3. hit time of this channel = time from cross correlation
      4. add this back to the CSW with the hit time as dT found on step 3
  **/
  
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  double dT =0;
  int numSamples = _zero_mean_waveform->GetN();
  double *xOld = _zero_mean_waveform->GetX();
  double *yOld = _zero_mean_waveform->GetY();
  double *yNew = new double[numSamples];
  
  for(int i=0; i < numSamples; i++){
    yNew[i] = -1*yOld[i];
  }

  TGraph *thisWaveMinus = new TGraph(numSamples, xOld, yNew);
  //  TGraph *grCSW_Minus_this_chan = addGraphs(thisWaveMinus, _csw_gr, getTime());
  TGraph *grCSW_Minus_this_chan = addGraphs(_csw_gr, thisWaveMinus, getTime());
  
  TGraph *grCorr = FFTtools::getNormalisedCorrelationGraph(grCSW_Minus_this_chan, _zero_mean_waveform);
  
  int bin=0;
  double cropFactor = 1.0;
  double corr = CSWTimeFinder::getPeakVal(grCorr, &bin, cropFactor);
  double x=0, y=0;
  grCorr->GetPoint(bin, x, y);
  

  dT = x - CSWTimeFinder::deltaT2ndIter;
  
  TGraph *grTemp = (TGraph*)_csw_gr->Clone();
  delete _csw_gr;
  _csw_gr = 0;
  
  _csw_gr = CSWTimeFinder::addGraphs(grTemp, _zero_mean_waveform, dT);

  delete [] yNew;
  
  if(grCorr) delete grCorr;
  grCorr =0;

  if(thisWaveMinus) delete thisWaveMinus;
  thisWaveMinus = 0;

  if(grCSW_Minus_this_chan) delete grCSW_Minus_this_chan;
  grCSW_Minus_this_chan = 0;

  if(grTemp) delete grTemp;
  grTemp =0;
  
  return dT;
  
}

TGraph *CSWTimeFinder::getCSW(){

  return _csw_gr;
}

TGraph *CSWTimeFinder::addGraphs(TGraph *gr1, TGraph *gr2, double deltaT){
  /*Function to add two waveforms with an offset dT */

  double *xVals1 = gr1->GetX();
  double *xVals2 = gr2->GetX();
  double *yVals1 = gr1->GetY();
  double *yVals2 = gr2->GetY();
  
  int grSize1 = gr1->GetN();
  int grSize2 = gr2->GetN();
  
  double *yValsNew = new double[grSize1];


  double firstX1 = xVals1[0];
  double firstX2 = xVals2[0];
  double dTGr2Gr1 = firstX2 - firstX1;
  int binOffset = 0;
  double binOffsetTime =0;

  if(deltaT + dTGr2Gr1 > 0){

    for(int samp =0; samp <grSize2; samp++){
      double x, y;
      gr2->GetPoint(samp, x, y);
    
      if(x >= (firstX2 + deltaT + dTGr2Gr1)){
	binOffset = -1*samp;
	binOffsetTime = x;
	break;
      }
    }
  }
  else {
    
    for(int samp=0; samp<grSize1;samp++){
      double x, y;
      gr1->GetPoint(samp, x, y);
    
      if(x >= (firstX1 - deltaT - dTGr2Gr1)){
	binOffset = +1*samp;
	binOffsetTime = x;
	break;
      }
    }
  }

  for(int samp=0; samp < grSize1; samp++){
    yValsNew[samp] = yVals1[samp];
    int newSamp = samp + binOffset;
    
    if(newSamp < 0 || newSamp >= grSize2) continue;
    
    yValsNew[samp] += yVals2[newSamp];
  }

  TGraph *grSum = new TGraph(grSize1, xVals1, yValsNew);
  
  delete [] yValsNew;
  yValsNew=0;

  return grSum;

}

double CSWTimeFinder::getPeakVal(TGraph *gr, int *bin, double cropFactor){
 
  if(cropFactor > 1) cropFactor=1;
  double x, y;
  int numPoints = gr->GetN();
  int newNumPoints = int(cropFactor*numPoints);
  int deltaNumPoints = numPoints - newNumPoints;
  
  gr->GetPoint(deltaNumPoints/2, x, y);
  double peakVal = y;
  int peakBin = deltaNumPoints/2;
  
  for(int i=1; i<newNumPoints; i++){
    gr->GetPoint(i+deltaNumPoints/2, x,y);
    if(peakVal < y){
      peakVal = y;
      peakBin = i;
    } 
  }
  if(bin){
    *bin = peakBin;
  }
  
  return peakVal;

}

TGraph *CSWTimeFinder::getGraphWithLength(TGraph *gr, int newLength ){

  int length = gr->GetN();
  double *xOld = gr->GetX();
  double *yOld = gr->GetY();

  double *xNew = new double[newLength];
  double *yNew = new double[newLength];
  
  TGraph *grNew;
  
  if(length<newLength){
    
    grNew = FFTtools::padWaveToLength(gr, newLength);
  }
  else if(length > newLength){
    
    int deltaLength = length - newLength;
    for(int i=0; i<newLength; i++){
      yNew[i] = yOld[i+deltaLength/2];
      xNew[i] = xOld[i+deltaLength/2];
    }
    
    grNew = new TGraph(newLength, xNew, yNew);
  }
  else {
    grNew = new TGraph(newLength, xOld, yOld);
  }

  delete [] xNew;
  delete [] yNew;
  return grNew;
}

int CSWTimeFinder::deltaT2ndIter=0;
