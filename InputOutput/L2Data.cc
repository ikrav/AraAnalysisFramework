#include "L2Data.h"

ClassImp(Hks);
ClassImp(Triggers);
ClassImp(Times);
ClassImp(Waveforms);
ClassImp(TimeStamp);
ClassImp(Headers);
ClassImp(RunHeader);
ClassImp(L2Data);


L2Data::L2Data(){

}

L2Data::L2Data(UsefulAraStationEvent *event){
  fill(event);
}

L2Data::L2Data(const L2Data &other) {
  
  _header = other._header;
  _hk = other._hk;
  _wf = other._wf;
  _trigger = other._trigger;
  _time_header = other._time_header;
}

L2Data::~L2Data(){}

void L2Data::printout(int mode){

  cout <<"Header: "<< endl;
  cout <<"\t unixTime "<< _header.unixTime.epoch << " eventNumber "<< _header.eventNumber << endl;
  
  cout <<"\n WF: "<< endl;
  for (int ch=0; ch< 16; ch++){
    cout <<"\t Power: "<< _wf.power[ch]<< " max frequency "<<   _wf.freqMax[ch] << endl;
  }
  cout << "\n"<< endl;
  
  cout << "Trigger: " << endl;
  cout <<"\t Event type " << _trigger.EventType << endl;
  
  cout <<"Run header: "<< endl;
  cout << "\t stationId " << _run_header.stationId <<" tot events "<< _run_header.NumberOfEvents << " number of RF " << _run_header.NumberOfRFEvents << " number of pulser "<< _run_header.NumberOfPulserEvents << " number of Forced " << _run_header.NumberOfForcedEvents << " unknown "<< _run_header.NumberOfUnknownEvents <<endl; 
  // " start "<< _run_header.RunStartTime << " end "< _run_header.RunEndTime << " duration " << _run_header.RunDuration << endl;

}

void L2Data::fill(UsefulAraStationEvent *event){

  UsefulIcrrStationEvent *derivedIcrr = dynamic_cast<UsefulIcrrStationEvent*>(event);
  if(derivedIcrr){
    fillIcrr(derivedIcrr);
  }
 
  //  if(derivedIcrr) delete derivedIcrr;

  UsefulAtriStationEvent *derivedAtri = dynamic_cast<UsefulAtriStationEvent*>(event);
  if(derivedAtri){
    fillAtri(derivedAtri);
  }
  //  if(derivedAtri) delete derivedAtri;

  _n_events++;
}


void L2Data::fillIcrr(UsefulIcrrStationEvent *event){

  fillIcrrEventData(event);
  fillIcrrRunData(event);
}

void L2Data::fillAtri(UsefulAtriStationEvent *event){

  fillAtriEventData(event);
  fillAtriRunData(event);
}

void L2Data::fillIcrrRunData(UsefulIcrrStationEvent *event){

  _endT = _header.unixTime.epoch;
  _run_header.stationId = event->getStationId();
  if(_n_events == 0){
    _startT = event->head.unixTime;
    _run_header.RunStartTime = TimeStamp(_startT);

  }
  _run_header.RunEndTime = TimeStamp(_endT);
  _run_header.RunDuration = _endT - _startT;
  _run_header.ErrorFlag = 0;
  _run_header.RunType  = 0;

  
  if( _trigger.EventType == PULSER){
    _run_header.NumberOfPulserEvents++;
  }
  else if( _trigger.EventType == FORCED) {
    _run_header.NumberOfForcedEvents++;
  }
  else if( _trigger.EventType == RF){
    _run_header.NumberOfRFEvents++;
  }
  else {
    _run_header.NumberOfUnknownEvents++;
  }
  
  _run_header.NumberOfEvents = _n_events;
  //  _run_header.Nall = _n_events; // this should correspond to(globalEventNumber) lastEvent - firstEvent, -- tessting the meaning of it
  
}

void L2Data::fillAtriRunData(UsefulAtriStationEvent *event){

  _endT = _header.unixTime.epoch;
  _run_header.stationId = event->getStationId();
  
  if(_n_events == 0){
    _startT = event->unixTime;
    _run_header.RunStartTime = TimeStamp(_startT);
  }

  _run_header.RunEndTime = TimeStamp(_endT);
  _run_header.RunDuration = _endT - _startT;
  
  _run_header.ErrorFlag = 0;
  _run_header.RunType = 0;
  
  if(_trigger.EventType == PULSER){
    _run_header.NumberOfPulserEvents++;
  }else if(_trigger.EventType == FORCED){
    _run_header.NumberOfForcedEvents++;
  }else if(_trigger.EventType == RF){
    _run_header.NumberOfRFEvents++;
  }else {
    _run_header.NumberOfUnknownEvents++;
  }
  
  _run_header.NumberOfEvents = _n_events;


}

void L2Data::fillIcrrEventData(UsefulIcrrStationEvent *event){
  
  // triggers
  _trigger.TriggerType = event->trig.trigType;
  _trigger.TriggerPattern = event->trig.trigPattern;
  _trigger.RbClock = event->trig.getRubidiumTriggerTimeInSec();
  _trigger.DeadTime = event->trig.getDeadtime();


  _trigger.EventType = 0;
  if(_trigger.TriggerType == 68) _trigger.EventType = FORCED; // forced
  else if(fabs(_trigger.RbClock-20.7e-6)<1e-7) _trigger.EventType = PULSER; // pulser
  else if(_trigger.TriggerType == 1) _trigger.EventType = RF; // RF

  
  for(int ch=0; ch<8; ch++) _hk.temperature[ch] = event->hk.getTemperature(ch);
  for(int ch=0; ch<8; ch++) _hk.RFPower[ch] = event->hk.getRFPowerBatwing(ch);
  for(int ch=0; ch<8; ch++) _hk.RFPower[ch+8] = event->hk.getRFPowerDiscone(ch);
  
  _hk.sclGlobal = _hk.sclGlobal;
  
  for(int i=0; i<12; i++) _hk.sclL1[i] = event->hk.sclTrigL1[i];
  for(int ch=0; ch<8; ch++) {
    _hk.scl[ch] = event->hk.sclBatMinus[ch];
    _hk.scl[ch+8] = event->hk.sclBatPlus[ch];
    _hk.scl[ch+16] = event->hk.sclDiscone[ch];
  }

  _header.unixTime = TimeStamp(event->head.unixTime);
  _header.unixTimeusec = event->head.unixTimeUs;
  _header.eventNumber = event->head.eventNumber;
  
  _header.gpsSubTime = event->head.gpsSubTime;
  _header.calibStatus = event->head.calibStatus;
  _header.priority = event->head.priority;
  _header.errorFlag = event->head.errorFlag;
  _header.RunNumber = _run_header.RunNumber;
  _header.stationId = event->getStationId();
  
  TH1::AddDirectory(kFALSE);
  //power stuff 
  TH1D *histFFTPower = new TH1D("histFFTPower", "histFFTPower", N_POWER_BINS, FREQ_POWER_MIN -((FREQ_POWER_MAX - FREQ_POWER_MIN)/N_POWER_BINS/2.), FREQ_POWER_MAX + ((FREQ_POWER_MAX - FREQ_POWER_MIN)/N_POWER_BINS/2.));
  
  TH1D *histFFTPowerLow = new TH1D("histFFTPowerLow", "histFFTPowerLow", N_POWER_BINS_L, FREQ_POWER_MIN_L - ((FREQ_POWER_MAX_L - FREQ_POWER_MIN_L)/N_POWER_BINS_L/2.), FREQ_POWER_MAX_L + ((FREQ_POWER_MAX_L - FREQ_POWER_MIN_L)/N_POWER_BINS_L/2.));
  
  for(int ch=0; ch<16; ch++){
    //    TGraph *grFFT = event->getFFTForRFChan(chan);    
    TGraph *gWf = event->getGraphFromRFChan(ch);
    double totPower = fillFFTHistoForRFChanL2(ch, histFFTPower, event->getFFTForRFChan(ch));
    double totPowerLow = fillFFTHistoForRFChanL2(ch, histFFTPowerLow, event->getFFTForRFChan(ch));
    
    _wf.power[ch] = totPower;
    _wf.freqMax[ch] = histFFTPower->GetBinCenter(histFFTPower->GetMaximumBin());
    
    _wf.freqMaxVal[ch] = pow(10, histFFTPower->GetBinContent(histFFTPower->GetMaximumBin())/10.) / pow(10,totPower/10.);

    for(int b=1; b<N_POWER_BINS_L+1; b++){
      _wf.powerBin[ch][b-1] = histFFTPower->GetBinContent(b);
    }
    
    double fInterp = 0.5;
    TGraph *gVt0;

    if(gWf->GetN() > 10) 
      gVt0 = FFTtools::getInterpolatedGraph(gWf, fInterp);
    else
      gVt0 = gWf;

    _wf.v2[ch] = FFTtools::integrateVoltageSquared(gWf, -1, -1);
    _wf.maxV[ch] =0;
    double *wfV = gWf->GetY();

    for(int iy=0; iy<gWf->GetN(); iy++){
      if(fabs(wfV[iy]) > _wf.maxV[ch]){
	_wf.maxV[ch] = fabs(wfV[iy]);
      }

      _wf.mean[ch] =gWf->GetMean(2);
      _wf.rms[ch] = gWf->GetRMS(2);

      _wf.isInTrigPattern[ch] = event->trig.isInTrigPattern(ch);
    }

      delete gVt0;
      delete gWf;
      
    }


    delete histFFTPowerLow;
    delete histFFTPower;
}


void L2Data::fillAtriEventData(UsefulAtriStationEvent *event){

  /*** MISSING TRIGGER INFORMATION
       - trigger.TriggerPattern
       - trigger.RbClock
       - trigger.DeadTime
  **/

  _trigger.EventType = 0;
  if(event->isRFTrigger()) _trigger.EventType = RF;
  else if(event->isSoftwareTrigger()) _trigger.EventType = FORCED;
  else if(event->isCalpulserEvent()) _trigger.EventType = PULSER;

  /**** MISSING HK INFO
	- hk.temperature
	- hk.RFPower
	- hk.sclGlobal
	- hk.sclL1
	- hk.scl
  ***/


  _header.unixTime = TimeStamp(event->unixTime);
  _header.unixTimeusec = event->unixTimeUs;
  _header.eventNumber = event->eventNumber;
  _header.stationId = event->getStationId();


  /*** MISSING HEADER INFO
       _header.gpsSubTime
       _header.calibStatus
       _header.priority
       _header.errorFlag
       _header.RunNumber
  **/
  
  TH1::AddDirectory(kFALSE);
  TH1D * histFFTPower = new TH1D("histFFTPower", "histFFTPower", N_POWER_BINS, FREQ_POWER_MIN - ((FREQ_POWER_MAX - FREQ_POWER_MIN)/N_POWER_BINS/2.), FREQ_POWER_MAX + ((FREQ_POWER_MAX - FREQ_POWER_MIN)/N_POWER_BINS/2.));

  TH1D *histFFTPowerLow = new TH1D("histFFTPowerLow", "histFFTPowerLow", N_POWER_BINS_L, FREQ_POWER_MIN_L - ((FREQ_POWER_MAX_L - FREQ_POWER_MIN_L)/N_POWER_BINS_L/2.), FREQ_POWER_MAX_L + ((FREQ_POWER_MAX_L - FREQ_POWER_MIN_L)/N_POWER_BINS_L/2.));

  for(int ch=0; ch<16; ch++){
    
    TGraph *gWf = event->getGraphFromRFChan(ch);
    TGraph *fftpowergraph =  event->getFFTForRFChan(ch);
    double totPower = fillFFTHistoForRFChanL2(ch, histFFTPower, fftpowergraph);
    double totPowerLow = fillFFTHistoForRFChanL2(ch, histFFTPowerLow, fftpowergraph);
    delete fftpowergraph;
    
    _wf.power[ch] = totPower;
    _wf.freqMax[ch] = histFFTPower->GetBinCenter(histFFTPower->GetMaximumBin());

    _wf.freqMaxVal[ch] = pow(10, histFFTPower->GetBinContent(histFFTPower->GetMaximumBin())/10.) / pow(10,totPower/10.);

    for(int b=1; b<N_POWER_BINS_L+1; b++){
      _wf.powerBin[ch][b-1] = histFFTPower->GetBinContent(b);
    }

    double fInterp = 0.5;
    TGraph *gVt0;

    if(gWf->GetN() > 10)
      gVt0 = FFTtools::getInterpolatedGraph(gWf, fInterp);
    else
      gVt0 = gWf;

    
    _wf.v2[ch] = FFTtools::integrateVoltageSquared(gWf, -1, -1);

    
    _wf.maxV[ch] = 0;
    double *wfV = gWf->GetY();

    for(int iy=0; iy<gWf->GetN(); iy++){

      if(fabs(wfV[iy]) > _wf.maxV[ch]){
        _wf.maxV[ch] = fabs(wfV[iy]);
      }

      _wf.mean[ch] = gWf->GetMean(2);
      _wf.rms[ch] = gWf->GetRMS(2);

      //      _wf.isInTrigPattern[ch] = event->trig.isInTrigPattern(ch);  // MISSING FOR ATRI 
    }
    
    delete gVt0;
    delete gWf;
    
  }

  delete histFFTPowerLow;
  delete histFFTPower;
}

double L2Data::fillFFTHistoForRFChanL2(int chan, TH1D *histFFT, TGraph *grRF){
  
  double tot=0;
  for( int b=0; b<histFFT->GetNbinsX()+1; b++){
    histFFT->SetBinContent(b,0);
  }

  TGraph *grFFT =(TGraph*)grRF->Clone();
  
  if(!grFFT) return -1;
  double *xVals = grFFT->GetX();
  double *yVals = grFFT->GetY();
  int numPoints = grFFT->GetN();
  
  for(int i=0; i<numPoints; i++){
    int bin = histFFT->FindBin(xVals[i]);
    double currentVal = histFFT->GetBinContent(bin);
    double newVal = 10.*log10( pow(10,(currentVal/10.))+pow(10,(yVals[i]/10.)));
    
    tot = tot+pow(10,(yVals[i]/10.));
    histFFT->SetBinContent(bin, newVal);
  }
  
  tot = 10.*log10(tot);
  delete grFFT;
	         
  return tot;
}

int L2Data::_n_events =0;
unsigned int L2Data::_startT =0;
unsigned int L2Data::_endT =0;
RunHeader L2Data::_run_header = RunHeader();
