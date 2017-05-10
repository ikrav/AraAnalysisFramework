{

  gSystem->Load("libReco.so");

  Driver::setFilterType(Filter::SIMPLE, StationGeometry::VPOL);
  CurvedRay::useCurvedRays(CurvedRay::NTIME);


  Driver *driver = new Driver(-1, "/data/user/cbora/practice/results.root");
   
  //  L0FileReader *reader = new L0FileReader("/data/exp/ARA/2011/minbias/L0/0108/L0.run1000.root", "/home/cbora/dev/Reconstruction/Reconstruction/trunk/RecoData/testBed_thresholds.txt");  
  L0FileReader *reader = new L0FileReader("/data/user/cbora/practice/RecoData/L0.TestBed_2011.root", "/home/cbora/dev/Reconstruction/Reconstruction/trunk/RecoData/testBed_thresholds.txt");  
  driver->setupL0FileReader(reader);

  driver->addCommand("Label=example, POL=vpol, TIME=thresh[time], VERTEX=asm[cartesian]");
  driver->printout();
  driver->loop();

  if(driver) delete driver;
  
        
}  



    
    

