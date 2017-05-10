{
  //  gDebug=3;
  gSystem->Load("libReco.so");
  EventVertex::debug_bit=1;
  Reconstruction::debug_bit=1;
  
  
  EventGenerator *gen=new EventGenerator();
  gen->setXrange(-1600,1600);
  gen->setYrange(-1600,1600);
  gen->setAmplitudeLog(3);
  gen->setSignalWidth(3);
  gen->setNumSignalChannels(4);
  
  Reconstruction reco;
  reco.setupEventGenerator(gen,200);
  
  reco.setOutputFile("results_gen_prev.root");

  reco.addCommand("vpol times simple");
//   reco.addCommand("vpol real_times 2 0.4");
  reco.addCommand("find scan_vpol cartesian earliest");
  reco.addCommand("find scan_vpol cartesian minuit");
  //  reco.addCommand("find scan_vpol cartesian asm");
//  reco.addCommand("find intmap 1 180 360");
//   reco.addCommand("find mcmc");
//   reco.addCommand("find scan lock12");
  
//   reco.addCommand("vpol real_times 0.3"); // use only Vpol, use real timing data with errors 0.3 ns
//   reco.addCommand("find real_vertex 2000 0.1 0.1");
//   reco.addCommand("scan lock0 find");
//   reco.addCommand("scan lock12 find");

  reco.loop();
  //  reco.loadEvent();
  //reco.run();
  
  ChannelCollection cc=reco.getEventData()->getEventVertex()->getChannels();
  TimingMatrix tm=cc.getTimingMatrix();
  
  
  //   reco.closeFile();
  
  
}
