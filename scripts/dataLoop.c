{

  gSystem->Load("libReco.so");
  
//   Reconstruction reco; // just to get the static pointers defined in all classes... 
  
//   FileReader reader("$DATA/ARA/2011/minbias/L0/0501/L0.run*.root", "$RECO/RecoData/testBed_thresholds.txt"); // arg2 a file containing thresholds for testBed
  FileReader reader("$DATA/ARA/2011/minbias/L0/0501/L0.run*.root");
  
  StationGeometry *geom=reader.getStationGeometry();
  Channel::setGeometry(geom);
  CurvedRay::setOpticalIce(new OpticalIce);
  Pos::setOriginStatic(geom->getOriginPosition());
  
//   geom->printout();
//   reco->setStationGeometry(geom);
  
  EarliestVertexFinder start_finder;
  MCMCVertexFinder mcmc_finder(0,0,0,0,1e5,100);
  MinuitVertexFinder minuit_finder;
  IntMapVertexFinder intmap_finder(1,0,0,0,5,72,144);
//   VertexFinder::debug_bit=1;
    
  TreeWriter writer;
//   writer.setSaveChannelsBit(0);
//   writer.setSaveVertexBit(0);
//   writer.setSaveFindersBit(0);
//   writer.setSaveHousekeepingBit(0);
  
  reader.setMaxNumEvents(1);
//   reader.setInterpolationFactor(1);
  
  for(int e=0;e<1e6;e++){ // loop over events
    
    if(!reader.loadNextEvent()) break;

    std::cout<<"e= "<<e<<std::endl;
    
    ChannelCollection chan = reader.getChannelCollection();

    chan.applyFinder(TimeFinder::GAUS);
    chan.applyDeltaFinder(DeltaFinder::CROS);
//     chan.printout();
    
//     start_finder.findVertex(chan);
    
//     mcmc_finder.findVertex(chan);
    
    cout<<"running findVertex() on intmap"<<endl;
    intmap_finder.findVertex(chan);
    intmap_finder.printout();
    intmap_finder.getBestMap()->Draw("colz");
  
    writer.setChannels(chan);
    writer.addFinder(&intmap_finder);
//     writer.printout();
    writer.fill();
        
  }


  writer.write();
  writer.close();
    
}


    
    
    
    
    
