//compile using: g++ -Wl,--no-as-needed delta_times_simulation.cxx AntennaPair.cc ChannelTime.cc DeltaTimes.cc DelayFinder.cc SimpleDelayFinder.cc GausDelayFinder.cc -I$SIM -I$ARA_UTIL_INSTALL_DIR/include `root-config --cflags --libs` -L$SIM -lSim -L$ARA_UTIL_INSTALL_DIR/lib -lAraEvent  -o sim_times.exe 

#include "EventGenerator.h"
#include "EventData.h"

#include "DeltaTimes.h"
#include "DelayFinder.h"
#include "GausDelayFinder.h"
#include "SimpleDelayFinder.h"

#include "Reconstruction.h"
#include "VertexFinder.h"
#include "ScanVertexFinder.h"
#include "SphericalScanVertexFinder.h"
#include "MinuitVertexFinder.h"

#include "Input.h"

#include "Headers.h"

#include "FFTtools.h"

// only use AraSim code if 
#ifdef ARASIM_EXISTS
#include "SimIncludes.h"
#endif

/////////////////////////////////////////////////
//						//
// 	this reads some AraSim outputs		//
// 	and extracts the time delays 		//
//	using DeltaTimes and using		//
//	AraSim's own raytrace informtaion	//
//	send any questions to:			//
//	guynir@weizmann.ac.il			//
//						//
/////////////////////////////////////////////////


TH1D *getLogBinHist(const char *name, const char *title, const int numBins, double xmin, double xmax){
 
    //  variable width histogram for gPad->SetLogx() taken from http://root.cern.ch/root/roottalk/roottalk01/3817.html
   double logxmin = log10(xmin);
   double logxmax = log10(xmax);
   double binwidth = (logxmax-logxmin)/numBins;
   double xbins[numBins+1];
   xbins[0] = xmin;
   for (int i=1;i<=numBins;i++) {
      xbins[i] = xmin + pow(10,logxmin+i*binwidth);
   }
   
   TH1D *hist=new TH1D(name,title,numBins, xbins);
   
   return hist;
  
}

TH2D *getLogLogBinHist(const char *name, const char *title, const int numBinsX, double xmin, double xmax, const int numBinsY, double ymin, double ymax){
     
   //  variable width histogram for gPad->SetLogx(), gPad->SetLogy() (2D histogram with log bins
   double logXmin = log10(xmin);
   double logXmax = log10(xmax);
   double binwidthX = (logXmax-logXmin)/numBinsX;
   
   
   double xbins[numBinsX+1];
   xbins[0] = xmin;
   for (int i=1;i<=numBinsX;i++) {
      xbins[i] = xmin + pow(10,logXmin+i*binwidthX);
   }
   
   double logYmin = log10(ymin);
   double logYmax = log10(ymax);
   double binwidthY = (logYmax-logYmin)/numBinsY;
   
   
   double ybins[numBinsY+1];
   ybins[0] = ymin;
   for (int i=1;i<=numBinsY;i++) {
      ybins[i] = ymin + pow(10,logYmin+i*binwidthY);
   }
 
   TH2D *hist=new TH2D(name, title, numBinsX, xbins, numBinsY, ybins);
  
   return hist;
   
}

double pi=3.141592655358979312;

int main(int argc, char **argv){
 
  double step_size=0.5;
  int debug_bit=1;
  int numChan=8;
  int N=512;
  int width=20;// number of bins of signal width
  double amplitude=100;// signal amplitude
  double freq=2.0;// frequency of cos term on the gaussian envelope 
  
  int numEvents=100;
  int numSuccessfulEvents=0; // only counts events where at least 4 pairs have weight>0
  int numTriggeredEvents=0;
//   char strategy[10]="gaus";
  char strategy[10]="naive";
  double strategy_scale=0.7;
  double strategy_thresh=2.1;
  string output_filename="results.root";
  
  int numberOfSampleWfs=3;
  
  
  Input input;
  input.setOption('d', "debug mode",0);
  input.setOption('N', "Number of channels",1);
  input.setOption('l', "length of waveform",1);
  input.setOption('A', "signal Amplitude",1);
  input.setOption('W', "signal Width",1);
  input.setOption('E', "number of Events",1);
  input.setOption('S', "Strategy name",1);
  input.setOption('t', "strategy threshold",1);
  input.setOption('s', "strategy scale", 1);
  input.setOption('f', "cosine frequency",1);
  input.setOption('o', "output filename", 1);
  input.setOption('e', "error on real times", 1);
  input.setOption('L', "Lock parameters 0,1, and 2", 3); 
  input.setOption('V', "Vertex finder strategy", 1);
  input.scanArguments(argc, argv);
  
  if(input.checkParameter('h')){ input.displayHelp(argv); return 0;}
  
  if(input.checkParameter('N')) numChan=atoi(input.getParameter('N'));
  if(input.checkParameter('l')) N=atoi(input.getParameter('l'));
  if(input.checkParameter('A')) amplitude=atof(input.getParameter('A'));
  if(input.checkParameter('W')) width=atoi(input.getParameter('W'));
  if(input.checkParameter('E')) numEvents=atoi(input.getParameter('E'));
  if(input.checkParameter('S')) strcpy(strategy, input.getParameter('S'));
  if(input.checkParameter('t')) strategy_thresh=atof(input.getParameter('t'));
  if(input.checkParameter('s')) strategy_scale=atof(input.getParameter('s'));
  if(input.checkParameter('f')) freq=atof(input.getParameter('f'));
  if(input.checkParameter('o')) output_filename=input.getParameter('o');
  
  
  debug_bit=input.checkParameter('d');
  
  TRandom3 *rand=new TRandom3(0);
//   TH1D *hRes=new TH1D("hRes","Residuals from hit time finder; time residuals; number of events (8 channels)",1000,0,200);
  TH1D *hRes=getLogBinHist("hRes", "Residuals from hit time finder; time residuals [ns]; number of events (8 channels)", 1000, 1e-3, 1.1*N*0.5);
  TH1D *hWeights=new TH1D("hWeights", Form("Weights from hit time finder; weights; number of events (%d channels)", numChan), 300, 0, 1);
//   TH2D *hResWeights=new TH2D("hResWeights", "Weights vs. residuals from hit finder; residuals; weights", 1000, 0, 1.1*N*0.5, 300, 0,1);
  TH2D *hResWeights=getLogLogBinHist("hResWeights", "Weights vs. residuals from hit finder; time residuals [ns]; weights", 1000, 1e-3, 1.1*N*0.5, 300, 1e-3 ,1);
    
  double averageRes=0;
  double event_energy=0;
  double event_distance=0;
  
  
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  
  #ifdef ARASIM_EXISTS
  TChain *araTree2;
  TChain *araTree;
  TChain *eventTree;
  Report *report=0;
  Event *event=0;
  Settings *settings=0;
  Detector *detector=0;
  #endif
  
  double energy;
  
  #ifdef ARASIM_EXISTS
  if(input.numNames>0){
  
  araTree2=new TChain("AraTree2");
  araTree=new TChain("AraTree");
  eventTree=new TChain("eventTree");
  
  char **filenames=input.getNames();
  
  if(debug_bit){
  
    cout<<"numNames= "<<input.numNames<<" ";
  
    for(int i=0;i<input.numNames;i++) cout<<"filename: "<<filenames[i]<<" ";
    cout<<endl;
    
  }
  
  for(int i=0;i<input.numNames;i++){
   
    araTree->Add(filenames[i]);
    araTree2->Add(filenames[i]);
    eventTree->Add(filenames[i]);
    
  }
  

  
  araTree2->SetBranchAddress("report", &report);
  araTree2->SetBranchAddress("event", &event);
  
  
  araTree->SetBranchAddress("settings", &settings);
  araTree->SetBranchAddress("detector", &detector);
  
  numEvents=araTree2->GetEntries();
  
  araTree->GetEntry(0);
  
  cout<<"number of entries in \"AraTree2\"= "<<numEvents<<endl;
  
  }// if using AraSim
  #endif
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  
  // output file we write to
  TFile *fout=new TFile(output_filename.c_str(), "recreate");
  TTree *time_tree=new TTree("time_tree","A tree with DeltaTimes objects");
  DeltaTimes *deltas=0;
  time_tree->Branch("dts", "DeltaTimes", &deltas);
  
  TTree *real_tree=new TTree("real_tree","A tree with EventData objects holding truth data");
  EventData *real_data=0;
  real_tree->Branch("real_data", "EventData", &real_data);
  
  TTree *event_tree=new TTree("event_tree","A tree with EventData objects holding reco data");
  EventData *event_data=0;
  event_tree->Branch("data", "EventData", &event_data);
  
  
  
  EventGenerator gen;


  gen.getGeometry()->twistGeometry();// this switches the string numbering to fit the AraSim convention. which is twisted. 
  gen.setSignalWidth(width);
  gen.setAmplitudeLog(std::log10(amplitude));
  
  vector<TGraph*> gr;
  vector<double> real_times;
  vector<double> real_bins;
  
  int divider=pow(10,(int)log10(numEvents)-1);
  if(divider<1) divider=1;
  
  
    for(int e=0;e<numEvents;e++){
  
      gr.clear();
      real_bins.clear();
      
      #ifdef ARASIM_EXISTS
      if(input.numNames>0){ // if using AraSim input
      
	
	
	araTree2->GetEntry(e);
        
	if(e%divider==0) cout<<"e= "<<setw((int)log10(numEvents))<<e<<" / "<<numEvents<<endl;

	if(report->stations[0].Global_Pass){
	
	  event_data=new EventData();
	  
	  double x=event->Nu_Interaction[0].posnu.GetX()-detector->stations[0].GetX();
	  double y=event->Nu_Interaction[0].posnu.GetY()-detector->stations[0].GetY();
	  double z=event->Nu_Interaction[0].posnu.GetZ()-detector->stations[0].GetZ();
	  
	  event_data->setPositionCartesian(x,y,z);
	  
	  event_data->setEnergy(event->pnu);
	
	  gr=report->getWaveformVectorVpol(detector, 0, e);
	  real_times=report->getHitTimesVectorVpol(detector);
	  
	  
	  
	  for(int i=0;i<16;i++) {
	    
	    if(report->stations[0].strings[i/4].antennas[i%4].arrival_time.size()){
	      
	      event_data->addHitTime(1e9*(report->stations[0].strings[i/4].antennas[i%4].arrival_time[0]),detector-> stations[0].strings[i/4].antennas[i%4].type);
	      
	    }
	    else{
	      
	      event_data->addHitTime(-1000, detector->stations[0].strings[i/4].antennas[i%4].type);// negative value that represents the fact that there is no real times...
	      
	    }
	    
	  }// for i
		  
	}// global pass
	else continue;// if it didn't trigger just move to the next event
	
      }// AraSim stuff
      #else
      if(0){}      
      #endif
      
      else{// using simple generator instead of AraSim
	
	if(e%divider==0) cout<<"e= "<<setw((int)log10(numEvents))<<e<<" / "<<numEvents<<endl;
	
	gen.generateEvent();

	real_data=gen.getEvent();

	gr=gen.getWaveformsVpol();
	real_times=real_data->getHitTimesVpol();
	
      }// else (if not using AraSim output files

      if(numberOfSampleWfs>0){ // sample waveforms with signal
	
	for(int ch=0;ch<numChan;ch++) gr[ch]->Write();
	
	numberOfSampleWfs--;
	
      }// if numberOfSampleWfs>0
      gr[0]->Print();
      // actual work done by DeltaTimes class...
      deltas=new DeltaTimes(8,0.5);
      deltas->readGraphs(gr);

      deltas->replaceStrategy("naive", 0);
      deltas->replaceStrategy(strategy,1);

      //cbora adds 
      cout << "\n strateegy " << strategy << "n *********** t \t "<< atof(input.getParameter('t')) << " \t s \t "<< atof(input.getParameter('s')) << endl;
      if(input.checkParameter('t')) deltas->setStrategyThreshold(atof(input.getParameter('t')),1);
      if(input.checkParameter('s')) deltas->setStrategyScale(atof(input.getParameter('s')),1);
//       deltas->addStrategy("gaus",1);
      
      cout<<"real_times: ";
      for(int ch=0;ch<real_times.size();ch++) cout<<real_times[ch]<<" ";
      cout<<endl;
      
      deltas->setRealTimes(real_times);
	
      if(input.checkParameter('e')) deltas->setTimeFromRealTimesPlusError(atof(input.getParameter('e')));
      
      deltas->fillHistResiduals(hRes);  
      deltas->fillHistWeights(hWeights);
      deltas->fillHistResWeights(hResWeights);
      
      deltas->runDeltas();
      deltas->printTimes();
      if(debug_bit) if(e%divider==0){ 
	
	deltas->printout(4);
	
      }
      
      Reconstruction reco;
      reco.setSerialNumber(e);
      reco.useObjectsFromGenerator(gen, 0);
      debug_bit = 1; // cbora add
      if(debug_bit) reco.getGeometry()->printout();

      reco.setRealEvent(real_data);
      reco.getEvent()->setR(reco.getRealEvent()->getR()+100);
      cout<<"real R= "<<reco.getRealEvent()->getR()<<endl;
      reco.getEvent()->setTheta(reco.getRealEvent()->getTheta());
      reco.getEvent()->setPhi(reco.getRealEvent()->getPhi());
      
      reco.setDeltaTimes(deltas);
      if(input.checkParameter('V')) reco.replaceStrategy(input.getParameter('V'), 0); 
      else reco.defaultStrategy();

      
      if(input.checkParameter('L')){
	
	int sum=0;
	for(int a=0;a<3;a++){ if(atoi(input.getParameter('L', a))) sum++;}
	
	if(sum<3) for(int a=0;a<3;a++) if(atoi(input.getParameter('L', a))) reco.setParameterReal(a,0);
	
      }
      
      //      reco.printout();
      
      reco.writeEvent();
      event_data=reco.getEvent();
      
      if(deltas->getNumPairsUsed()>=4) numSuccessfulEvents++;
      deltas->clearStrategies();
      time_tree->Fill();
      delete deltas;
      
      real_tree->Fill();
      if(input.numNames>0) delete real_data;
            
      event_tree->Fill();
      
      for(int ch=0;ch<numChan;ch++) delete gr[ch];
      gr.clear();
      

      
      numTriggeredEvents++;
	  
    }// for e
    
    //////////////////////////////////////////////////////////////////////////////////
    
    
  hRes->SetStats(0);
  hRes->Write();
  hWeights->SetStats(0);
  hWeights->Write();
  hResWeights->SetStats(0);
  hResWeights->Write();
  
  cout<<"succesfully found times for "<<numSuccessfulEvents<<" out of "<<numTriggeredEvents<<" events"<<endl;
  
  time_tree->Write();
  event_tree->Write();
  real_tree->Write();
  
  fout->Close();
  
  delete hRes;
  delete hWeights;
  delete hResWeights;
  
  delete rand;
    
//   cout<<"Im out!"<<endl;
}// main
  
  


