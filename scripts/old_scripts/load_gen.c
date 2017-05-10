{
  
  OpticalIce *ice=new OpticalIce();
  StationGeometry *geom=new StationGeometry();
  
  Channel::setGeometry(geom);
  CurvedRay::setOpticalIce(ice);

  EventGenerator gen;
  gen.setXrange(-1600,1600);
  gen.setYrange(-1600,1600);
  gen.setAmplitudeLog(3);
  gen.setSignalWidth(10);
  gen.setNumSignalChannels(4);
   
  ChannelCollection cc=gen.getChannelCollection();
  cc.applyFinder(TimeFinder::REAL_TIMES, 0);

  VertexPos pos(200,100, -300);
  pos.setSpherical(3000, 3.14/2, 3.14);
  pos.setSymmetricErrorsSpherical(1000, 3.14/2, 3.14);

  IntMapVertexFinder mvf(cc, &pos, 0,0,0,0,1,360,720);

  
  
  
}