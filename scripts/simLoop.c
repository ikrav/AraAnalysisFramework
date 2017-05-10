{

  //library for the reconstruction framework
  gSystem->Load("libReco.so");

  string input="$ARASIM/outputs/AraOut.root";

  FileReader::debug_bit=1;
  SimFileReader::debug_bit=1;
  
  SimFileReader reader(input);
  
  //to get the geometry
  StationGeometry *geom=reader.getStationGeometry();
  CurvedRay::setOpticalIce(new OpticalIce);
  Pos::setOriginStatic(geom->getOriginPosition());
  
  //ice model
  OpticalIce *ice = new OpticalIce();
  CurvedRay::setOpticalIce(ice);
  
  IntMapVertexFinder::debug_bit=1;
  
  IntMapVertexFinder intmap_finder("r_num=3, theta_num=36, phi_num=72");
//   intmap_finder.setRealPosition(reader.getRealPosition());
  
  reader.setMaxNumEvents(6);
  reader.printout();
    
  while(reader.loadNextEvent()){ // loop over events
      
    ChannelCollection chan=reader->getChannelCollection();
//     chan->printout();
    chan.applyFinder(TimeFinder::GAUS);
    chan.applyDeltaFinder(DeltaFinder::CROS);
    
    intmap_finder.setRealPosition(reader.getRealPosition());
    intmap_finder.findVertex(chan);
    intmap_finder.printout();    
    intmap_finder.getBestMap()->Draw("colz");
    
  }

}