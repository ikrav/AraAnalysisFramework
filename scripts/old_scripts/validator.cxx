{

  gSystem->Load("libReco.so");
  //.../L0/0108/L0.run1004.root
  L0FileReader *reader = new L0FileReader("/data/exp/ARA/2011/minbias/L0/0108/L0.run1004.root", "/home/cbora/dev/Reconstruction/Reconstruction/trunk/RecoData/testBed_thresholds.txt"); // a file containing thresholds for testBed
  
  StationGeometry *geom = reader->getStationGeometry();
  OpticalIce *ice = new OpticalIce();
  
  CurvedRay::setOpticalIce(ice);
  CurvedRay::useCurvedRays(CurvedRay::NTIME);
  Output *output = new Output("/data/user/cbora/mcmc_guy_validation.root", "DataBranch");

  EventData *data = 0; // this will be a pointer to an object owned by Output
  int nProcess = 0;

  while(reader->getNextEvent()){ // loop over events
    
    //    if(nProcess == 1) break;

    data = output->getEventData();

    ChannelCollection chan = reader->getChannelCollection().getVpolChannelCollection();

    //    chan.applyFinder(TimeFinder::THRESH);
    chan.applyFinder(TimeFinder::CSW);
    //    chan.applyTimeDeltaFinder(DeltaFinder::CROS);


    data->setData(chan, geom, ice);

    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::EARLIEST, VertexFinder::SPHERICAL, VertexFinder::TIMES, VertexFinder::UNLOCK, 0, 0,0);
    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::EARLIEST, VertexFinder::SPHERICAL, VertexFinder::PAIRS, VertexFinder::UNLOCK, 0, 0,0);
    
    data->findVertex(StationGeometry::VPOL, EventVertex::DEFAULT);  // use earliest as starting positions for minuit

    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::MCMC, VertexFinder::CARTESIAN, VertexFinder::TIMES, VertexFinder::UNLOCK);
    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::UNLKUMINUIT, VertexFinder::CARTESIAN, VertexFinder::TIMES, VertexFinder::UNLOCK);
    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::ASM, VertexFinder::CARTESIAN, VertexFinder::TIMES, VertexFinder::UNLOCK);
    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::UNLKUMINUIT, VertexFinder::SPHERICAL, VertexFinder::TIMES, VertexFinder::UNLOCK);
    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::SCAN, VertexFinder::SPHERICAL, VertexFinder::TIMES, VertexFinder::UNLOCK, 100, 100, 100);
    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::MCMC, VertexFinder::CARTESIAN, VertexFinder::TIMES, VertexFinder::UNLOCK, 0,0,0);

    data->findVertex(StationGeometry::VPOL, EventVertex::PREVIOUS); // find vertex of the vpol


    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::SCAN, VertexFinder::SPHERICAL, VertexFinder::TIMES, VertexFinder::UNLOCK, 100, 100, 100);
    //    data->findVertex(StationGeometry::VPOL, EventVertex::AVERAGE);
    
    //    chan.applyFinder(TimeFinder::THRESH);

    Filter *filter = new SimpleFilter(data, StationGeometry::VPOL);

    if(filter->pass()){
      output->fill(); // fills event and allocates memory for a new event data
      nProcess++;
    }

    if(filter) delete filter;
    filter=0;

    output->clear();

    chan.clear();
  }

  ice->printout();
  output->write(); // writes to file
  
  if(output) delete output;
  if(ice) delete ice;
  if(reader) delete reader;
  

}


    
    
    
    
    
