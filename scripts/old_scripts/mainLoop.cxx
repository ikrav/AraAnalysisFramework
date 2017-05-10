#include "Headers.h"
#include "Definitions.h"
#include "Configuration.h"
#include "ConfigurationTranslator.h"
#include "L0FileReader.h"
#include "StationGeometry.h"
#include "Reconstruction.h"
#include "DeltaTimes.h"
#include "EventData.h"
#include "OpticalIce.h"

int main(int argc, char **argv){

  if(argc < 2){
    cout <<"usage:   "<< argv[0] <<" <input_L0_file> <outputFile> <configFile>"<< endl;
    return -1;
  }

  //output File, this will eventually have its own class
  string output;
  if(argc > 2)
    output = argv[2];
  else 
    output = "output.root";


  //configuration stuff
  Configuration *config = new Configuration("config.txt");
  config->readConfiguration();
  config->printout();


  ConfigurationTranslator *ct = new ConfigurationTranslator(config);
  ct->fill();
  ct->printout();

  //get data structure containing all reconstruction methods
  vector<_pol_run> runs = ct->getRuns();
  if(runs.size() == 0) { cerr<<"No configurations set.. Exiting!!!"<<endl; return -1;}

  //Let's create all the branches we will need
  int numBranches=0;
  string branches[100];
  for(int ii=0; ii<runs.size(); ii++){
    for(int jj=0; jj<runs[ii].fMethod.size(); jj++){
      for(int kk=0; kk<runs[ii].fMethod[jj].vertex.size(); kk++){
	  branches[numBranches] = ct->generateBranchDescription(runs[ii].pol, runs[ii].fMethod[jj].delay.name, runs[ii].fMethod[jj].vertex[kk].name);
	  cout <<" branch # : "<< numBranches << " name "<< branches[numBranches] << endl;
	  numBranches++;
      }
    }
  }
  
  TFile *fOut = new TFile(output.c_str(), "RECREATE");
  TTree *out_tree = new TTree("out_tree", "EventData tree");
  fOut->cd();
  EventData *event_data[numBranches];
  for(int iii=0; iii<numBranches; iii++){
    event_data[iii] = new EventData();
    out_tree->Branch(branches[iii].c_str(), "EventData", &event_data[iii]);
  }
  
  //reading L0 file
  L0FileReader *reader = new L0FileReader(argv[1]);
  DeltaTimes *deltas =0;

  vector<TGraph*> gr;

  StationGeometry *geom[2];
  geom[VPOL] = reader->getVPolGeometry();
  geom[HPOL] = reader->getHPolGeometry();

  OpticalIce *ice = new OpticalIce();
  ice->setIceVariablityBit(1);

  //looping over events
  while(reader->getNextEvent()){
    gr.clear();

    //    reader->printout();
    int addedBranch=0;
    for(int iPol=0; iPol<runs.size(); iPol++){  //for each polarization

      gr.reserve(geom[runs[iPol].pol]->getNumChan());

      //looping like this, will make us avoid looping over a finder method twice
      for(int iDelay=0; iDelay<runs[iPol].fMethod.size(); iDelay++){ //for each delay finder
	gr.clear();
	if(runs[iPol].pol == VPOL)  gr = reader->getVPolTimeGraphs();
	else if(runs[iPol].pol == HPOL) gr = reader->getHPolTimeGraphs();
       
	deltas  = new DeltaTimes(geom[runs[iPol].pol]->getNumChan(), 0.5); //0.5 - step size (for now a dummy value)
	deltas->readGraphs(gr);
	deltas->replaceStrategy(runs[iPol].fMethod[iDelay].delay.method, 0);  // set method to use    
	deltas->runDeltas();

	for(int iVertex=0; iVertex<runs[iPol].fMethod[iDelay].vertex.size(); iVertex++){ //for each vertex of the delay 
	  Reconstruction *reco = new Reconstruction();
	  reco->setOpticalIce(ice);
	  reco->setSerialNumber(reader->getCurrentEventNumber());
	  //	  deltas->printTimes();
	  reco->copyGeometry(geom[runs[iPol].pol]);
	  reco->setDeltaTimes(deltas);
	  reco->replaceStrategy(runs[iPol].fMethod[iDelay].vertex[iVertex].method, 0);
	  //	  reco->clearStrategies();
	  
	  /* Extract data from other classes and fill eventData for your own analysis */
	  event_data[addedBranch]->setPolRatio(geom[runs[iPol].pol]->getPolRatio());
	  event_data[addedBranch]->setIsCalPulser(reader->isCalPulser());
	  event_data[addedBranch]->setOrigin(geom[runs[iPol].pol]->getOrigin());
	  event_data[addedBranch]->checkForBetterValues(reco->tempRunReco()); // this makes a copy of the event, therefore its fine to delete the event in reco
	  cout << "NAME "<< runs[iPol].fMethod[iDelay].vertex[iVertex].name << endl;
	  event_data[addedBranch]->printout();
	  delete reco;
	  addedBranch++;

	}//for iVertex
	/* remember to clear graphs for next delay finder
	   No deleting TGraphs, it's already taken care of in L0FileReader */
	gr.clear(); 
	delete deltas;
	
      }//for iDelay

    }//for iPol
    out_tree->Fill(); // fill the tree
  }// for events

  //delete objects
  delete reader;
  delete ice;

  fOut->cd();
  out_tree->Write();
  fOut->Close();
  delete ct;
  delete config;
  return 0;
}
  
