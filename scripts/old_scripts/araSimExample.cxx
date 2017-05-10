{

  //library for the reconstruction framework
  gSystem->Load("libReco.so");


  vector<string> input;
  input.clear();
  input.push_back("AraOut.root");

  AraSimFileReader *reader = new AraSimFileReader(input);
  
  //to get the geometry
  StationGeometry *geom = reader->getStationGeometry();

  //ice model
  OpticalIce *ice = new OpticalIce();
  CurvedRay::setOpticalIce(ice);
  
  while(reader->getNextEvent()){ // loop over events
    
    ChannelCollection chan = reader->getChannelCollection();
    
    for( int i=0; i< chan.size(); i++){
      
      TGraph *gr = (TGraph*) chan[i]->getWaveform()->Clone();

    }

    if(reader) delete reader; reader=0;
    if(ice) delete ice; ice=0;

}
