{

  gSystem->Load("libReco.so");
  
  L0FileReader *reader = new L0FileReader("/data/exp/ARA/2011/minbias/L0/0108/L0.run1004.root", "/home/cbora/dev/Reconstruction/Reconstruction/trunk/RecoData/testBed_thresholds.txt"); // a file containing thresholds for testBed
  
  StationGeometry *geom = reader->getStationGeometry();
  OpticalIce *ice = new OpticalIce();
  CurvedRay::setOpticalIce(ice);
  Output *output = new Output();

  EventData *data = 0; // this will be a pointer to an object owned by Output

  while(reader->getNextEvent()){ // loop over events
    
    data = output->getEventData();

    ChannelCollection chan = reader->getChannelCollection();
    chan.applyFinder(TimeFinder::SIMPLE);
    data->setData(chan, geom, ice);

    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::EARLIEST, VertexFinder::CARTESIAN, VertexFinder::TIMES, VertexFinder::UNLOCK, 0, 0,0);
    //    data->findVertex(StationGeometry::VPOL);  // use earliest as starting positions for minuit
    
    //    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::MINUIT, VertexFinder::CARTESIAN, VertexFinder::TIMES, VertexFinder::UNLOCK, 0, 0, 0);
    data->addVertexFinder(StationGeometry::VPOL, VertexFinder::ASM, VertexFinder::CARTESIAN, VertexFinder::TIMES, VertexFinder::UNLOCK, 0, 0, 0);

    data->findVertex(StationGeometry::VPOL); // find vertex of the vpol
    output->fill(); // fills event and allocates memory for a new event data
    output->clear();

    chan.clear();
  }

  output->write(); // writes to file
  
  if(output) delete output;
  if(ice) delete ice;
  if(reader) delete reader;
  

}


    
    
    
    
    
