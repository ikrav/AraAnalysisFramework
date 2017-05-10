#include "CppIncludes.h"
#include "RootIncludes.h"

#include "CurvedRay.h"
#include "OpticalIce.h"

#include "Channel.h"
#include "ChannelCollection.h"
#include "TimingMatrix.h"
#include "Pos.h"
#include "VertexPos.h"
#include "CorrGraph.h"
#include "THX.h"

#include "Input.h"
#include "EventGenerator.h"
#include "L0FileReader.h"
#include "AraSimFileReader.h"
#include "Output.h"
#include "Parser.h"

#include "StationGeometry.h"
#include "OpticalIce.h"
#include "CurvedRay.h"
#include "Reconstruction.h"
#include "EventVertex.h"
#include "VertexFinder.h"
#include "VertexPos.h"

#include "EventData.h"
#include "SimData.h"
#include "EarliestVertexFinder.h"
#include "IntMapVertexFinder.h"
#include "MCMCVertexFinder.h"

int main(int argc, char **argv){
 
  Reconstruction reco; // just to get the static pointers defined in all classes... 
  EventGenerator gen; 
  gen.setNumSignalChannels(6);
  CorrGraph::whichGraph=1; // use hilbert envelope!
  EarliestVertexFinder start_finder;
  MCMCVertexFinder mcmc_finder(0,0,0,0,1e5,100);
  MinuitVertexFinder minuit_finder;
  IntMapVertexFinder intmat_finder(0,0,0,0,1,36,72);

  VertexFinder::debug_bit=1;
//   IntMapVertexFinder::debug_bit=1;
//   THX::debug_bit=1;
  
  int numEvents=1;
  
  for(int e=0;e<numEvents;e++){
   
    gen.generateEvent();
    ChannelCollection cc=gen.getChannelCollection();
    cc.applyFinder(TimeFinder::GAUS);
    cc.applyDeltaFinder(DeltaFinder::CROS);

    start_finder.setRealPosition(gen.getSourcePosition());
    start_finder.findVertex(cc);
    
    cout<<"running findVertex() on mcmc"<<endl;
    mcmc_finder.findVertex(cc, start_finder);
    
//     cout<<"running findVertex() on minuit"<<endl;
//     minuit_finder.findVertex(cc, mcmc_finder);
    
    cout<<"running findVertex() on intmap"<<endl;
    intmat_finder.findVertex(cc, mcmc_finder);
    
    gen.printout();
//     mcmc_finder.printout(1);// option 1 is for spherical printout
//     minuit_finder.printout();
    intmat_finder.printout(1);
    
//     new TCanvas();
//     mcmc_finder.getHistXD()->draw();
       
//     new TCanvas();
//     intmat_finder.getHistXD()->draw();
    
  }
  
}