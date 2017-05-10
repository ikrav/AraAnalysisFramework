#include "CorrGraph.h"
#include "Channel.h"

ClassImp(CorrGraph);

CorrGraph::CorrGraph() {
 
//   std::cerr<<__PRETTY_FUNCTION__<<" WARNING: don't use this constructor. it doesn't make the correlation graph at all..."<<std::endl;
  
  initialize();
  
}

CorrGraph::CorrGraph(Channel *first_channel, Channel *second_channel) {
  
  initialize();

  setChannels(first_channel, second_channel);
  
  calculate();
  
}

CorrGraph::CorrGraph(const CorrGraph &other){
  
  initialize();
  
//   setChannels(other.getFirstChannel(), other.getSecondChannel());
  
  if(other._grCorr) _grCorr=new TGraph(*other._grCorr);
  if(other._grHilbEnv) _grHilbEnv=new TGraph(*other._grHilbEnv);
  
}

CorrGraph::~CorrGraph(){
  
  if(_grCorr) delete _grCorr; _grCorr=0;
  if(_grHilbEnv) delete _grHilbEnv, _grHilbEnv=0;
}

CorrGraph *CorrGraph::getDeepCopy(){
  
  return new CorrGraph(*this);
  
}

void CorrGraph::initialize(){
 
  _grCorr=0;
  _grHilbEnv=0;
  
  _chan1=0;
  _chan2=0;
    
}

void CorrGraph::calculate(){
 
//   std::cout<<__PRETTY_FUNCTION__<<" chan1= "<<_chan1->getChannelId()<<" chan2= "<<_chan2->getChannelId()<<std::endl;
  
  if(_chan1==0 || _chan2==0 || _chan1->getWaveform()==0 || _chan2->getWaveform()==0){ 
    
    std::cerr<<__PRETTY_FUNCTION__<<" ERROR: no channels/waveforms"<<std::endl;
    return;  
  
  }
  
  _grCorr=FFTtools::getCorrelationGraph(_chan1->getWaveform(), _chan2->getWaveform());
  _grCorr->SetTitle(Form("Correlation graph (ch: % 2d and % 2d); dt (ns); correlation (mV^{2})", getFirstChannelId(), getSecondChannelId()));
  
  _grHilbEnv=FFTtools::getHilbertEnvelope(_grCorr);
  _grHilbEnv->SetTitle(Form("Hilbert Envelope (ch: % 2d and % 2d); dt (ns); correlation (mV^2{2})", getFirstChannelId(), getSecondChannelId()));
  
  
}

// getters
TGraph *CorrGraph::getGraph() const {
  
  return _grCorr;
  
}

TGraph *CorrGraph::getHilbertEnvelope() const {
 
  return _grHilbEnv;
  
}

Channel *CorrGraph::getFirstChannel() const {
  
  return _chan1;
  
}

Channel *CorrGraph::getSecondChannel() const {
  
  return _chan2;
  
}

int CorrGraph::getFirstChannelId() const {
 
  return getFirstChannel()->getChannelId();
  
}

int CorrGraph::getSecondChannelId() const {
 
  return getSecondChannel()->getChannelId();
  
}

double CorrGraph::getWeight() const {
 
  return _chan1->getWeight()*_chan2->getWeight();
  
}

int CorrGraph::getN() const {
  
  if(getGraph()==0) return 0;
  
  return getGraph()->GetN();
  
}

double *CorrGraph::getX() const {
  
  if(getGraph()==0) return 0;
  
  return getGraph()->GetX();
  
}

double *CorrGraph::getY() const {
  
  if(getGraph()==0) return 0;
  
  return getGraph()->GetY();
  
}

double CorrGraph::getValue(double dt) const {
  
  if(whichGraph==CORRELATION) return getGraph()->Eval(dt);
  if(whichGraph==HILBERTENV) return getHilbertEnvelope()->Eval(dt);
  
}

// setters
void CorrGraph::setChannels(Channel *first_channel, Channel *second_channel){
     
  _chan1=first_channel;
  _chan2=second_channel;
  
}

int CorrGraph::whichGraph=1; // 0 is for cross correlation, 1 (default) adds a smoothing with Hilbert envelope
