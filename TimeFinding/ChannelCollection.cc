#include "ChannelCollection.h"

ClassImp(ChannelCollection);

ChannelCollection::ChannelCollection(){
   
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;

  _csw=0;
  
}

ChannelCollection::ChannelCollection(std::vector<Channel*> channels){

  _csw=0;
  setVector(channels);
  
}

ChannelCollection::ChannelCollection(const ChannelCollection &other){
   
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;

  // this creates a shallow copy. if you want, use getDeepCopy()
  _csw = other._csw;
  _channels=other._channels;
  
}

ChannelCollection::~ChannelCollection(){
    
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;

  //  delete_channels();
  clear();
}

ChannelCollection *ChannelCollection::getDeepCopy(){
   
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;

  ChannelCollection *new_chans=new ChannelCollection();
  
  new_chans->assignDeepCopy(*this);
  
//   std::cout<<"returning new channel collection"<<std::endl;
  
  return new_chans;
  
}

ChannelCollection &ChannelCollection::operator=(const ChannelCollection &other){
 
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(this==&other) return *this; // don't self assign!
    
  // this class has surprisingly little members!   
  _channels=other._channels;
  _csw=other._csw;

  
}

ChannelCollection &ChannelCollection::assignDeepCopy(const ChannelCollection &other){
  
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  if(this==&other) return *this; // don't self assign!
  
  if(other._csw) _csw=new TGraph(*other._csw);
  else _csw=0;
    
  reserve(other.size());
  
  for(int ch=0;ch<other.size();ch++){// first, copy the channels (deep copy)
   
    push_back(new Channel(*other[ch]));
    
  }// for ch
  
  for(int ch=0;ch<other.size();ch++){// for each channel make sure the delta finders and corr graphs have the right Channel* pointers
   
//     std::cout<<"ch= "<<ch<<std::endl;
        
    // now make sure that all pair-wise channel pointers are updated to the new objects (in DeltaFinder)
    std::vector< std::vector<DeltaFinder*> > delta_finders=getChannel(ch)->getDeltaFindersMatrix();
    std::vector< std::vector<DeltaFinder*> > other_delta_finders=other[ch]->getDeltaFindersMatrix();

    for(int i=0;i<delta_finders.size();i++){ // this goes over the types of delta finders
           
//       std::cout<<"i= "<<i<<std::endl;

      for(int j=delta_finders[i].size()-1;j>=0;j--){ // this goes over the pairs of each finder type with all other channels.
	
// 	std::cout<<"j= "<<j<<std::endl;

	// these are all pointers to the deeply copied finders, so we can change them here and it applies inside the channel
	delta_finders[i][j]->setFirstChannel(getChannel(ch));
	int chanID=other_delta_finders[i][j]->getSecondChannelId();// the delta finders are one-to-one with old delta finders
	Channel *ptr=getChannelById(chanID); // make sure you are getting the channel pointer for the right ID (from THIS collection!)
	
// 	std::cout<<"chanID= "<<chanID<<" ptr= "<<ptr<<std::endl;
	
	if(ptr)	delta_finders[i][j]->setSecondChannel(ptr); // if the channel exists, adopt the new pointer.
	else{
	  
	  getChannel(ch)->eraseDeltaFinder(i,j); // delete the finder, remove the pointer from the channel's own version of the finder matrix
	  delta_finders[i][j]=0; // just to make sure there are no dangling pointers...
	  
	}
	
      }// for j
      
    }// for i
        
    // now make sure that all pair-wise channel pointers are updated to the new objects (in CorrGraph)
    std::vector<CorrGraph*> corr_graphs=getChannel(ch)->getCorrelationGraphVector();
    std::vector<CorrGraph*> other_corr_graphs=other[ch]->getCorrelationGraphVector();

    for(int j=0;j<corr_graphs.size();j++){ // this goes over the pairs of each graph type with all other channels.

      // these are all pointers to the deeply copied graphs, so we can change them here and it applies inside the channel
      int index=other_corr_graphs[j]->getSecondChannelId();// the corr graphs are one-to-one with old corr graphs
      Channel *ptr=getChannelById(index); // make sure you are getting the channel pointer for the right ID (from THIS collection!)

      if(ptr) corr_graphs[j]->setChannels(getChannel(ch), ptr); // if the channel exists, adopt the new pointer.
      else{
  
	getChannel(ch)->eraseCorrGraph(j); // delete the graph, remove the pointer from the channel's own version of the graph vector
	corr_graphs[j]=0; // just to make sure there are no dangling pointers...
	  
      }
	
    }// for j
        
  }// for ch
  
//   std::cout<<"done with assignDeepCopy()"<<std::endl;
  
  return *this;
  
}

void ChannelCollection::printout() const {
 
  for(int i=0;i<getNumChans();i++) _channels[i]->printout();
  
}

void ChannelCollection::drawWaveforms() const {
 
  if(getNumChans()<1) return;
  
  TCanvas *cWaveforms=new TCanvas("cWaveforms", "channel collection waveforms");
  
  int numCols=4;
  int numRows=(getNumChans()-1)/numCols+1; // for rounding up
  
  cWaveforms->Divide(numCols, numRows);
  
  int ch=0;
  
  for(int i=0;i<numCols;i++){
    
    for(int j=0;j<numRows;j++){
      
      if(ch>=getNumChans()) break;
      
      cWaveforms->cd((i)*numRows+j+1);
//       _channels[ch]->getWaveform()->Draw("al"); //like this it won't crash in case we select only vpols...
      _channels[ch]->draw();
      
      ch++;
      
    }
  }
}

void ChannelCollection::drawTimeFound() const {

  /***
  if(getNumChans() < 1) return;
  
  TCanvas *cTimes = new TCanvas("cTimes", "channel collection times");
  
  int numCols = 4;
  int numRows = (int) ceil((getNumChans()*1.0)/ (numCols*1.0));

  DrawTime *dr[getNumChans()];
  cTimes->Divide(numCols, numRows);
  int ch=0;
  
  for(int i=0; i<numRows; i++){
    for(int j=0; j<numCols; j++){
      
      if((ch - getNumChans()) == 0) break;
      dr[ch] = new DrawTime(_channels[ch]);
      cTimes->cd((i)*4+j+1);
      stringstream ss;
      ss << _channels[ch]->getChannelId();
      string s = "Chan: ";
      s.append(ss.str());
      if(_channels[ch]->getPolarization() == 0) s.append(" / VPOL");
      else s.append(" / HPOL");
      dr[ch]->getGraph()->SetTitle(s.c_str());
      dr[ch]->getGraph()->Draw("AL*");
  
      ch++;
      
    }
    
  }

  ***/
} 

void ChannelCollection::clear(){
 
  _channels.clear();
  
}

void ChannelCollection::delete_channels(){
 
  for(int i=0;i<getNumChans();i++){
   
    if(_channels[i]) delete _channels[i];
    _channels[i] = NULL;
    
  }
  
  _channels.clear();
  
}

void ChannelCollection::clearFinders(){
 
  for(int i=0;i<getNumChans();i++) _channels[i]->clearFinders();
  
}

void ChannelCollection::clearWaveforms(){
 
  for(int i=0;i<getNumChans();i++) _channels[i]->clearWaveform();
  
}

// getters
int ChannelCollection::size() const {
  
  return getNumChans();
  
}

int ChannelCollection::empty() const {
 
  return _channels.empty();
  
}

Channel* ChannelCollection::back(){
 
  return _channels.back();
  
}

int ChannelCollection::getNumChans() const {
  
  return _channels.size();
  
}

int ChannelCollection::getNumUsefulChannels() const {
 
  int num=0;
  
  for(int i=0;i<getNumChans(); i++){
    
    if(_channels[i]->getWeight() > 0){ 
      
      num++;
      
    }
    
  }
  
  return num;
  
}

Channel *ChannelCollection::getChannel(int index) const{
 
  return _channels[index];
  
}

Channel *ChannelCollection::getChannelById(int channel_id) const {
 
  Channel *ptr=0;
  
  for(int i=0;i<getNumChans();i++) if(_channels[i]->getChannelId()==channel_id) ptr=_channels[i];
  
  return ptr;
  
}

Channel *ChannelCollection::getBestChannel() const {
 
  Channel *ptr=0;
  
  for(int i=0;i<getNumChans(); i++){
   
    if(ptr==0 || _channels[i]->getWeight() > ptr->getWeight()) ptr=_channels[i];
    
  }
  
  return ptr;
  
}

Channel *ChannelCollection::getEarliestChannel() const {
 
  Channel *ptr=0;
  
  for(int i=0;i<getNumChans(); i++){
   
    if(ptr==0 || _channels[i]->getTime() < ptr->getTime()) ptr=_channels[i];
    
  }
  
  return ptr;
  
}

std::vector<Channel*> ChannelCollection::getVector() const {

  return _channels;
  
}

Channel *ChannelCollection::operator[](int index) const{
 
  return _channels[index];
  
}

ChannelCollection ChannelCollection::getVpolChannelCollection() const {
 
  std::vector<Channel*> new_vec;
  
  for(int i=0;i<getNumChans();i++) if(_channels[i]->getPolarization()==0) new_vec.push_back(_channels[i]);
  
  return ChannelCollection(new_vec);
  
}

ChannelCollection ChannelCollection::getHpolChannelCollection() const {
 
  std::vector<Channel*> new_vec;
  
  for(int i=0;i<getNumChans();i++) if(_channels[i]->getPolarization()==1) new_vec.push_back(_channels[i]);
  
  return ChannelCollection(new_vec);
  
}

ChannelCollection ChannelCollection::getPolarizedChannelCollection(int polarization) const {
 
  if(polarization<0 || polarization>1) std::cerr<<"wrong polarization "<<polarization<<" used. try 0 (Vpol) or 1 (Hpol)\n";
  
  std::vector<Channel*> new_vec;
  
  for(int i=0;i<getNumChans();i++) if(_channels[i]->getPolarization()==polarization) new_vec.push_back(_channels[i]);
  
  return ChannelCollection(new_vec);
  
}

ChannelCollection ChannelCollection::getSubset(std::vector<int> channel_ids) const {
 
  std::vector<Channel*> new_vec;
  
  for(int i=0;i<getNumChans(); i++) for(int j=0;j<channel_ids.size(); j++) if(_channels[i]->getChannelId()==channel_ids[j]) new_vec.push_back(_channels[i]);
  
  return ChannelCollection(new_vec);
  
}

ChannelCollection ChannelCollection::maskChannels(std::vector<int> channel_ids) const {
 
  std::vector<Channel*> new_vec;
  
  for(int i=0;i<getNumChans(); i++) for(int j=0;j<channel_ids.size(); j++) if(!_channels[i]->getChannelId()==channel_ids[j]) new_vec.push_back(_channels[i]);
  
  return ChannelCollection(new_vec);
  
}

TimingMatrix ChannelCollection::getTimingMatrix(){
 
  TimingMatrix matrix;
   
  for(int i=0;i<getNumChans();i++) matrix.addTimingVector(_channels[i]->getTimingVector(_channels));
  
  return matrix;
  
}

TimingMatrix ChannelCollection::getTimingMatrix(int finder_type, double par1, double par2){
 
  TimingMatrix matrix;
   
  for(int i=0;i<getNumChans();i++) matrix.addTimingVector(_channels[i]->getTimingVector(_channels, finder_type, par1, par2));
  
  return matrix;
  
}

std::vector<CorrGraph*> ChannelCollection::getCorrelationGraphVector(){
 
  std::vector<CorrGraph*> vec;
  
  for(int ch=0;ch<getNumChans();ch++){
   
    std::vector<CorrGraph*> temp_vec=_channels[ch]->getCorrelationGraphVector(_channels);
    
    vec.insert(vec.end(), temp_vec.begin(), temp_vec.end());
    
  }
  
  return vec;
  
}

// setters
void ChannelCollection::reserve(int number){
  
  _channels.reserve(number);
  
}

void ChannelCollection::push_back(Channel *channel){
  
  _channels.push_back(channel);
  
}

void ChannelCollection::setVector(std::vector<Channel*> channels){
  
  _channels=channels;
  
}

void ChannelCollection::applyTimeDeltaFinder(int finder_type, int delta, double par1, double par2){

  if(finder_type > 100) {  // apply Time finders, see deltaFinder enums & timeFinder
    applyDeltaFinder(finder_type, par1, par2);
  }
  else{
    applyFinder(finder_type, par1, par2);

    if( delta == 1) { // to make pairs
      applyDeltaFinder(DeltaFinder::SUBTRACT, par1,par2); // explicitly call subtract
    }
  }
}

void ChannelCollection::applyFinder(int finder_type, double par1, double par2){
        
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;

  if(finder_type != TimeFinder::CSW){
    for(int i=0;i<getNumChans();i++) _channels[i]->applyFinder(finder_type, par1, par2);
  }else{
    
    /* CSW requires a non-straight forward iteration, here it is */
    int channels_order[getNumChans()];
    //    int channels_order[6] = {0, 1, 3, 2, 4, 5};
    TGraph *csw=0;
    
    int iterations = 2;
    /** loop on the channels twice */
    for(int iter=0; iter< iterations; iter++){   
      maxCorrOrderer(channels_order, iter); // returns an array ordered wrt correlation

      std::cout << __PRETTY_FUNCTION__ << std::endl;    
      for(int i=0; i<getNumChans(); i++)    
	std::cout << channels_order[i] << std::endl;//for testing

      for(int i=0; i< getNumChans(); i++){
	par1 = (double) iter; //tells us if its the 1st iteration or not
	par2 = (double) i;  // tells us if its the first channel in the CSW
	int ch_index = channels_order[i];
	if(iter > 0)
	  ch_index = i;
      /* see CSWTimeFinder for how par1 and par2 are interpreted */	
	if(_channels[ch_index]->getWaveform() == 0) {std::cerr<<"ERROR: no waveform in this channels" << _channels[ch_index]->getChannelId() << " to apply a finder to...\n"; continue;}
	/* return the CSW and feed it to the next channel */
	TGraph *grTemp= (TGraph*) _channels[ch_index]->applyCSW(csw, finder_type, par1, par2)->Clone();
	
	if(iter == 1 && i == 0) {
	  CSWTimeFinder::deltaT2ndIter = _channels[ch_index]->getTime();
	  //CSWTimeFinder::deltaT2ndIter = -31.5923;
	  //CSWTimeFinder::deltaT2ndIter =0;
	  std::cout << "delta T 2nd Iter "<< CSWTimeFinder::deltaT2ndIter << std::endl;
	}
	if(csw) delete csw;
	csw =0;
	csw = (TGraph*) grTemp->Clone();
	if(grTemp) delete grTemp;
	grTemp = 0;
      }//numChans
    }//iterations

    _csw = (TGraph*) csw->Clone();
    if(csw) delete csw;
    csw=0;
    
  }
  
}

void ChannelCollection::applyDeltaFinder(int finder_type, double par1, double par2){
 
//   std::cout<<__PRETTY_FUNCTION__<<std::endl;
  for(int i=0;i<getNumChans();i++) _channels[i]->applyDeltaFinder(_channels, finder_type, par1, par2);
  
}

void ChannelCollection::maxCorrOrderer(int *results, int iteration){

  //will loop through all channels and return array in the order of max correlated channels    

  struct theCorr{
    int ch1;
    int ch2;
    double corr;
  };


  double theMaxCorr;

  TGraph *grCorr;
  double maxCorr;

  int dummyIndex=0;

  /* variables to help make sure surface ants go in last */
  int surfaceAntIndex[32]; // 32 just in case
  double surfaceAntMax = -10.0; // surface ant criteria
  int numberOfSurfaceAnts = 0;
  int goodAnts[32];
  int totalChans = 0;

  int pairN=0;


  if(iteration < 1){
    //meaning first iteration
    //find indices of last antennas
    for(int i=0; i<_channels.size(); i++){

      if(_channels[i]->getPosition().getZ() > surfaceAntMax){ 
	surfaceAntIndex[numberOfSurfaceAnts] = i;
	numberOfSurfaceAnts++;
      }else{
	goodAnts[i] = i;
	totalChans++;
      }

    }
  }else {
    for(int i=0; i< _channels.size(); i++)
      goodAnts[i] = i;
    totalChans = _channels.size();
  }


  int nPairs = (totalChans * (totalChans-1))/2;
  theCorr myCorr[nPairs];

  for(int i=0; i< totalChans; i++){
    
    int ant1 = goodAnts[i];

    for(int j=i+1; j<totalChans; j++){

      int ant2 = goodAnts[j];

      TGraph *gr1 = getZeroMeanGraph(_channels[ant1]->getWaveform());
      TGraph *gr2 = getZeroMeanGraph(_channels[ant2]->getWaveform());
      grCorr = FFTtools::getNormalisedCorrelationGraph(gr1, gr2);
      double maxCorr = FFTtools::getPeakVal(grCorr, &dummyIndex);

      myCorr[pairN].ch1 = ant1;
      myCorr[pairN].ch2 = ant2;
      myCorr[pairN].corr = maxCorr;

      pairN++;
      if(grCorr) delete grCorr; grCorr = 0;
      if(gr1) delete gr1; gr1=0;
      if(gr2) delete gr2; gr2=0;
    }

  }

  //use selection sort and sort by correlation value    
  int i, j, minIndex;
  theCorr temp;
  for(i=0; i < nPairs-1; i++){
    minIndex = i;
    for(j = i+1; j< nPairs; j++){      
      
      if(myCorr[j].corr > myCorr[minIndex].corr){ // descending order   
        minIndex = j;
      }
    }
    if(minIndex != i){
      temp = myCorr[i];
      myCorr[i] = myCorr[minIndex];
      myCorr[minIndex] = temp;
    }
  }

  //copy all channels in the array         
  int n=0;
  int newArr[nPairs*2];
  for(int i=0; i<nPairs; i++){
    newArr[n] = myCorr[i].ch1;
    newArr[n+1] = myCorr[i].ch2;
    n = n+2;
  }


  //boolean array of added channels 
  int arr[totalChans];
  //initialize it to zeros    
  for(int i=0; i<totalChans; i++){
    arr[i] = 0;
  }

  //remove duplicates from results         
  int seen=0;
  for(int i=0; i<n; i++){
    if(arr[newArr[i]] == 0) { // number has not been seen  
      results[seen] = newArr[i];
      arr[newArr[i]] = 1; //mark it as seen       
      seen++;
    }
  }

  if(iteration < 1){
  //now add the surface antennas last
    for(int i=0; i < numberOfSurfaceAnts; i++){
      results[seen] = surfaceAntIndex[i];
      seen++;
    }
  }

}

TGraph *ChannelCollection::getZeroMeanGraph(TGraph *gr){
  
  /* Maybe should be made a static function */
  
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
     
TGraph *ChannelCollection::getCSW(){
  
  if(_csw){
    
    double *xOld = _csw->GetX();
    double *yOld = _csw->GetY();
    int numSamples = _csw->GetN();
    
    double *yNew = new double[numSamples];
    
    for(int i=0;i<numSamples;i++) yNew[i] = yOld[i]/(getNumChans());
    TGraph *scaled = new TGraph(numSamples, xOld, yNew);
    
    return scaled;
  }
    
  return NULL;
}
