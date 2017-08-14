
#include "TimeSequenceFilter.h"

TimeSequenceFilter::TimeSequenceFilter() : Filter() {
  initialize();
}

TimeSequenceFilter::TimeSequenceFilter(EventData *event) : Filter(event) {
  initialize();
}

TimeSequenceFilter::~TimeSequenceFilter() {

  if(_triggerPattern) delete _triggerPattern;

}

void TimeSequenceFilter::initialize() {

  _thFactor = 4.0;
  _sumTime = 10;
  _triggerPattern = 0;
  _minQuality = 2.0;

}

bool TimeSequenceFilter::pass(){

  bool answer = false;
  getQuality();
  if(_quality>_minQuality) answer = true;
  return answer;

}

void TimeSequenceFilter::getQuality() {

  _nChl = _data->getChannelCollection().getNumChans();
  for(int i=0;i<_nChl;i++) {
    _waves.push_back( FFTtools::getInterpolatedGraph(_data->getChannelCollection().getChannel(i)->getWaveform(),0.5));
  }
  
  double timeLength = _waves[0]->GetN()/2.0; //this is the duration of the waveform
  _blockCount = timeLength/20.0*4.0; //actually the count of blocks. Factor 4 is historical. Just a scaling...
  		
  getHits();
  patternCheck();

  for(int i=0;i<_nChl;i++) {
    delete _waves[i];
  }
  _waves.clear();

}


void TimeSequenceFilter::getHits() {
  
  Double_t *times = 0;
  Double_t *volts = 0;
  double en_average=0;
  double en_var=0;
  double en_std_dev=0;
  
  double sum[4] = {0};
  double sumSQ[4] = {0};
  double minSum[2]; minSum[0] = 9e6; minSum[1] = 9e6;
  double minSumSQ[2]; minSumSQ[0] = 9e6; minSumSQ[1] = 9e6;
  double energy = 0;
  int bin;

  _var.resize(_nChl);
  _stdDev.resize(_nChl);
  _mean.resize(_nChl);

  int pattern_bin_no = 500/(_sumTime/2.0);
  if(_triggerPattern) delete _triggerPattern;
  _triggerPattern = new TH2D("_triggerPattern", "The event trigger pattern", pattern_bin_no,0,500, _nChl,0,_nChl);
  
  for(int rfchan=0;rfchan<_nChl;rfchan++) {
    
    vector<double> energyV;
    vector<double> timeV;

    for(int i=0; i<4; i++) {
      sum[i] = 0.0;
      sumSQ[i] = 0.0;
    }
    
    for(int i=0; i<2; i++) {
      minSum[i] = 9e6;
      minSumSQ[i] = 9e6;
    }

    vector<double> times;
    vector<double> volts;
    bin = _waves[rfchan]->GetN()-1; 
    for(int i=0; i<bin+1; i++) {
      times.push_back(_waves[rfchan]->GetX()[i]);
      volts.push_back(_waves[rfchan]->GetY()[i]);
    }
    int lim[4] = {0,bin/4-1,bin/2-1,bin/4*3.0-1};
    int division = bin/4;
    int quad = 0;    
    
    for (int l=0; l<bin; l++) {

      energy = 0.0;

      if(l>_sumTime) {

	for(int ie =0; ie<_sumTime;ie++) energy += volts[l-_sumTime+ie]*volts[l-_sumTime+ie];
	energy = TMath::Sqrt(energy/_sumTime);
	
	for(int j=0; j<4; j++) if(l>lim[j]) quad = j;

	sum[quad] += energy;
	sumSQ[quad] += energy*energy;
	
	energyV.push_back(energy);
	timeV.push_back(times[l]);

      }
      
    }

    sum[0] /= ((bin/4.0)-_sumTime);
    sumSQ[0] /= ((bin/4.0)-_sumTime);
    for(int i=1; i<4; i++) {
      sum[i] /= (bin/4.0);
      sumSQ[i] /= (bin/4.0);
    }

    for(int i=0; i<4; i++) {

      if(sum[i]<minSum[0]) {
	minSum[1] = minSum[0];
	minSum[0] = sum[i];
      }
      else if(sum[i]<minSum[1]) {
	minSum[1] = sum[i];
      }

      if(sumSQ[i]<minSumSQ[0]) {
	minSumSQ[1] = minSumSQ[0];
	minSumSQ[0] = sumSQ[i];
      }
      else if(sumSQ[i]<minSumSQ[1]) {
	minSumSQ[1] = sumSQ[i];
      }

    }

    _mean[rfchan] = (double)(minSum[0] + minSum[1])/2.0;
    _var[rfchan] = (minSumSQ[0] + minSumSQ[1])/2.0 - _mean[rfchan]*_mean[rfchan];
    _stdDev[rfchan] = TMath::Sqrt(_var[rfchan]);
   
    for(int i=0; i<energyV.size(); i++) {
      if(energyV[i]>( _thFactor*_stdDev[rfchan] + _mean[rfchan] ) && rfchan!=15 ) {
	_triggerPattern->Fill(timeV[i],rfchan);
      }
    }
					    
  }
  
}

void TimeSequenceFilter::patternCheck() {

  vector<int> string;
  vector< vector<int> > chanstringID; chanstringID.resize(4);
  vector<int> plane;
  double stringbase[2] = {_data->getChannelCollection().getChannel(0)->getX(),_data->getChannelCollection().getChannel(0)->getY()};
  double centerx = 0.0;
  double centery = 0.0;

  for(int ichl=0;ichl<_nChl;ichl++) {
    centerx += _data->getChannelCollection().getChannel(ichl)->getX()/_nChl;
    centery += _data->getChannelCollection().getChannel(ichl)->getY()/_nChl;
  }

  for(int ichl=0;ichl<_nChl;ichl++) {
    
    plane.push_back(0);
    
    double antx = _data->getChannelCollection().getChannel(ichl)->getX();
    double anty = _data->getChannelCollection().getChannel(ichl)->getY();
        
    if( antx-centerx>0.0 && anty-centery>0.0 ) {string.push_back(1); chanstringID[0].push_back(ichl);}
    else if( antx-centerx>0.0 && anty-centery<0.0 ) {string.push_back(2); chanstringID[1].push_back(ichl);}
    else if( antx-centerx<0.0 && anty-centery>0.0 ) {string.push_back(3); chanstringID[2].push_back(ichl);}
    else {string.push_back(4); chanstringID[3].push_back(ichl);}
    
  }
  
  for(int istr=0; istr<4; istr++) {
    for(int ichl1=0; ichl1<chanstringID[istr].size(); ichl1++) {
      double ant1z = _data->getChannelCollection().getChannel(chanstringID[istr][ichl1])->getZ();
      for(int ichl2=0; ichl2<chanstringID[istr].size(); ichl2++) {
	if(ichl1!=ichl2) {
	  double ant2z = _data->getChannelCollection().getChannel(chanstringID[istr][ichl2])->getZ();
	  if(ant1z<ant2z) plane[chanstringID[istr][ichl1]]++;
	}
      }
    }
  }

  int max_bin = _triggerPattern->GetNbinsX();				//The number of bins
  double timeStep = _triggerPattern->GetXaxis()->GetBinWidth(1);	//the binsize
  double timeRange = _triggerPattern->GetXaxis()->GetXmax();		//the time range
  
  vector<int> antnum;
  vector<double> hittime;

  for(int i=1;i<max_bin+1;i++) {
    for(int j=0;j<_nChl;j++) {
      if(_triggerPattern->GetBinContent(i,j+1)>0) {
	hittime.push_back(double(i)*timeStep);
	antnum.push_back(j);
      }
    }
  }
  
  int same_speed_t[800] = {0};
  int same_speed_v[800] = {0};
  int same_speed_d1[800] ={0};
  int same_speed_d2[800] = {0};
  int same_speed_h[800] = {0};
  int count = hittime.size();
  double distance = 0;

  for(int gg=0; gg<count;gg++) {

    int string1 = string[antnum[gg]];
    double ant1x = _data->getChannelCollection().getChannel(antnum[gg])->getX();
    double ant1y = _data->getChannelCollection().getChannel(antnum[gg])->getY();
    double ant1z = _data->getChannelCollection().getChannel(antnum[gg])->getZ();
    
    for(int kk=gg+1;kk<count;kk++) {

      if(antnum[kk]==antnum[gg]) continue;

      int string2 = string[antnum[kk]];
      double ant2x = _data->getChannelCollection().getChannel(antnum[kk])->getX();
      double ant2y = _data->getChannelCollection().getChannel(antnum[kk])->getY();
      double ant2z = _data->getChannelCollection().getChannel(antnum[kk])->getZ();
      
      //First check if pair has the same plane number, means they are at the same depth level (or close to).
      if(plane[antnum[kk]]==plane[antnum[gg]]) {
	
	distance = sqrt( pow(ant1x-ant2x,2) + pow(ant1y-ant2y,2) );
	if( (string1-string2) < 0) distance = -distance;
	int innerbin = 400 + TMath::Nint(15.0/timeStep*(hittime[kk] - hittime[gg])/distance);
	
	//Now the string numbers are used to determine the horizontal direction:
	// lateral 1) string 1-2 and string 3-4
	if(TMath::Abs(string2-string1)==1 && string2+string1!=5) same_speed_t[innerbin]++;
	// diagonal 1) string 2-3 
	else if(TMath::Abs(string2-string1)==1 && string2+string1==5) same_speed_d1[innerbin]++;	
	// lateral 2) string 2-4 and string 1-3 
	else if(TMath::Abs(string2-string1)==2) same_speed_v[innerbin]++;
	// diagonal 2) string 1-4
	else if(TMath::Abs(string2-string1)==3 && string2+string1==5) same_speed_d2[innerbin]++;
	
      }
      
      //And here we get to the vertical pairs:
      if(string1==string2 &&  plane[antnum[kk]]%2==plane[antnum[gg]]%2) {
	distance = ant1z-ant2z;
	int innerbin = 400 + TMath::Nint(18.0/timeStep*(hittime[kk] - hittime[gg])/distance);
	same_speed_h[innerbin]++;
      }

    }
  }
  
  int speed_t_max=0;
  int speed_v_max=0;
  int speed_h_max=0;
  int speed_d_max1=0;
  int speed_d_max2=0;
  
  int temp_speed_t_max=0;
  int temp_speed_v_max=0;
  int temp_speed_h_max=0;
  int temp_speed_d1_max=0;
  int temp_speed_d2_max=0;
        
  for(int tt=1; tt<799; tt++) {

    temp_speed_t_max = same_speed_t[tt]+same_speed_t[tt-1]+same_speed_t[tt+1];
    temp_speed_v_max = same_speed_v[tt]+same_speed_v[tt-1]+same_speed_v[tt+1];
    temp_speed_h_max = same_speed_h[tt]+same_speed_h[tt-1]+same_speed_h[tt+1];
    temp_speed_d1_max = same_speed_d1[tt]+same_speed_d1[tt-1]+same_speed_d1[tt+1];
    temp_speed_d2_max = same_speed_d2[tt]+same_speed_d2[tt-1]+same_speed_d2[tt+1];
    
    if(temp_speed_t_max>speed_t_max)   speed_t_max = temp_speed_t_max;
    if(temp_speed_v_max>speed_v_max)   speed_v_max = temp_speed_v_max;
    if(temp_speed_h_max>speed_h_max)   speed_h_max = temp_speed_h_max;
    if(temp_speed_d1_max>speed_d_max1) speed_d_max1 = temp_speed_d1_max;
    if(temp_speed_d2_max>speed_d_max2) speed_d_max2 = temp_speed_d2_max;

  }
  
  _quality = (speed_t_max+speed_v_max+speed_h_max)/_blockCount;
  
}
