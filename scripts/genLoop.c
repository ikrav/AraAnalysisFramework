{
  gSystem->Load("libReco.so");

  StationGeometry *geom=new StationGeometry();
  Channel::setGeometry(geom);
  CurvedRay::setOpticalIce(new OpticalIce);
  Pos::setOriginStatic(geom->getOriginPosition());
  
  EventGenerator gen; 
  gen.setNumSignalChannels(6);
  EarliestVertexFinder start_finder;
  MCMCVertexFinder mcmc_finder(0,0,0,0,1e5,100);
  MinuitVertexFinder minuit_finder;
  IntMapVertexFinder intmap_finder(1,0,0,0,5,36,72);

  VertexFinder::debug_bit=1;
  IntMapVertexFinder::debug_bit=1;
//   THX::debug_bit=1;
  
  TreeWriter writer("test.root");
  
  int numEvents=1;

  writer.printout();
  
  for(int e=0;e<numEvents;e++){
   
    cout<<"e= "<<e<<endl;
    
//     cout<<">>gen.generateEvent();"<<endl;
    gen.generateEvent();
    
//     cout<<">>ChannelCollection cc;"<<endl;
    ChannelCollection cc;
    
//     cout<<">>cc.assignDeepCopy(gen.getChannelCollection());"<<endl;
    cc.assignDeepCopy(gen.getChannelCollection());
    
//     cout<<">>cc.applyFinder(TimeFinder::GAUS);"<<endl;
    cc.applyFinder(TimeFinder::GAUS);
    
//     cout<<">>cc.applyDeltaFinder(DeltaFinder::CROS);"<<endl;
    cc.applyDeltaFinder(DeltaFinder::CROS);
//     cc.printout();
    
//     cout<<">>start_finder.setRealPosition(gen.getSourcePosition());"<<endl;
    start_finder.setRealPosition(gen.getSourcePosition());
    
//     cout<<">>start_finder.findVertex(cc);"<<endl;
//     start_finder.findVertex(cc);
    
//     cout<<"running findVertex() on mcmc"<<endl;
//     mcmc_finder.findVertex(cc, start_finder);
    
//     cout<<"running findVertex() on minuit"<<endl;
//     minuit_finder.findVertex(cc, mcmc_finder);
        
    cout<<">>intmap_finder.findVertex(cc);"<<endl;
    intmap_finder.setRealPosition(gen.getSourcePosition());
    intmap_finder.findVertex(cc);
    intmap_finder.printout();
    
//     intmap_finder.findVertex(cc, intmap_finder);
    
    intmap_finder.getBestMap()->Draw("colz");
    
    
    writer.setChannels(cc);
    writer.addFinder(&intmap_finder);
    
    writer.fill();
    
  }
  
  writer.write();
  writer.close();
  
}