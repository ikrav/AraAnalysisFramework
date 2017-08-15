
#include "HitFilter.h"

HitFilter::HitFilter() : Filter() {
  initialize();
}

HitFilter::HitFilter(EventData *event) : Filter(event) {
  initialize();
}

HitFilter::~HitFilter() {}

void HitFilter::initialize() {

  _thresholdMultiplier = 2.5;

}

bool HitFilter::pass(){
  
  bool answer = false;
  _hits = findNumberOfHits();
  if(_hits>=4) answer = true;
  return answer;
  
}

int HitFilter::findNumberOfHits() {

  int hits = 0;
  int nChl = _data->getChannelCollection().getNumChans();

  for(int ichl=0; ichl<nChl; ichl++) {
    double rms = _data->getChannelCollection().getChannel(ichl)->getGeometry()->getPosition(ichl).getVRMS();
    bool hit = false;
    int npnt = _data->getChannelCollection().getChannel(ichl)->getWaveform()->GetN();
    for(int ipnt=0; ipnt<npnt && !hit; ipnt++) {
      double val = _data->getChannelCollection().getChannel(ichl)->getWaveform()->GetY()[ipnt];
      if( val > (rms*_thresholdMultiplier) ) { 
	hit = true; 
	hits++;
      }
    }
  }
  
  return hits;
  
}

void HitFilter::getNeededMultiplierVector(vector<double> &neededV) {
  neededV.clear();
  int nChl = _data->getChannelCollection().getNumChans();
  for(int ichl=0; ichl<nChl; ichl++) {
    TGraph *hgra = FFTtools::getHilbertEnvelope(_data->getChannelCollection().getChannel(ichl)->getWaveform());
    double rms = _data->getChannelCollection().getChannel(ichl)->getGeometry()->getPosition(ichl).getVRMS();
    int npnt = _data->getChannelCollection().getChannel(ichl)->getWaveform()->GetN();
    double maxmult = 0.0;
    for(int ipnt=0; ipnt<npnt; ipnt++) {
      double val = _data->getChannelCollection().getChannel(ichl)->getWaveform()->GetY()[ipnt];
      double mult = val/rms;
      if(maxmult<mult) maxmult = mult;
    }
    delete hgra;
    neededV.push_back(maxmult);
  }
}
