{


  EventVertex::debug_bit=1;
  Reconstruction::debug_bit=1;
  
  
  EventGenerator *gen=new EventGenerator();
  gen->setXrange(-1600,1600);
  gen->setYrange(-1600,1600);
  gen->setAmplitudeLog(3);
  gen->setSignalWidth(10);
  gen->setNumSignalChannels(4);
  
  Reconstruction reco;
  reco.setupEventGenerator(gen,10);
  
  reco.setOutputFile("results.root");

  reco.addCommand("vpol pairs cros");
//   reco.addCommand("vpol real_times 2 0.4");
  reco.addCommand("find mcmc 5e4 3000 10");
//   reco.addCommand("find mcmc");
//   reco.addCommand("find scan lock12");
  
//   reco.addCommand("vpol real_times 0.3"); // use only Vpol, use real timing data with errors 0.3 ns
//   reco.addCommand("find real_vertex 2000 0.1 0.1");
//   reco.addCommand("scan lock0 find");
//   reco.addCommand("scan lock12 find");

  reco.loop();
  
  reco.closeFile();
  
  
}