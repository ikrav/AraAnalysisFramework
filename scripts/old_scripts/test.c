
{
  
gSystem->Load("$RECO/libReco.so");

OpticalIce *ice=new OpticalIce();
StationGeometry *geom=new StationGeometry();

Channel::setGeometry(geom);
CurvedRay::setOpticalIce(ice);

EventGenerator gen;
gen.setXrange(-1600,1600);
gen.setYrange(-1600,1600);
gen.setAmplitudeLog(4);
gen.setSignalWidth(10);
cout<<"generating event..."<<endl;
gen.generateEvent();

VertexPos *initial=new VertexPos();
initial->setRealPosition(Pos(gen.getSimData()->getCartesian()));

cout<<"made ChannelCollection"<<endl;
ChannelCollection cc=gen.getChannelCollection();
cc=cc.getVpolChannelCollection(); // take only Vpol for shorter analysis

cc.applyFinder(TimeFinder::REAL, 0);// just use the real times (can add gaussian errors using parameter)
cc.applyDeltaFinder(DeltaFinder::SUBTRACT);
TH1D *hRes=GTools::getLogBinHist("hRes", "residuals histogram", 100, 1e-6, 10); 

TimingMatrix TM(cc.getTimingMatrix());

TM.fillResiduals(hRes);// to check the error distribution

RealVertexFinder *real=new RealVertexFinder(cc, initial, VertexFinder::SPHERICAL, 0, 0, 0, 1000, 10*VertexPos::DEGTORAD, 10*VertexPos::DEGTORAD); // use true vertex position with errors on R, theta, phi

real->findVertex();// pass real vertex pos to found vertex pos+errors

cout<<"real vertex position + errors..."<<endl;
real->printout();

// now use scan to try to find the best vertex position, on an interval around the real pos...
// VertexFinder::SPHERICAL - use spherical coordinates
// VertexFinder::TIMES - use hit times (instead of pairs)
// VertexFinder::LOCK2 - lock phi (use LOCK0 to lock R, LOCK1 to lock theta, LOCK01 to lock both etc...)
ScanVertexFinder *v1=new ScanVertexFinder(cc, (VertexPos*) real, VertexFinder::SPHERICAL, VertexFinder::TIMES, VertexFinder::LOCK0);

cout<<"v1, theta and phi"<<endl;
v1->findVertex();
v1->printout();

ScanVertexFinder *v2=new ScanVertexFinder(cc, (VertexPos*) v1, VertexFinder::SPHERICAL, VertexFinder::TIMES, VertexFinder::UNLOCK);

cout<<"v2, all axes..."<<endl;
// v2->printout();
v2->findVertex();
v2->printout();
// v->getHist2D()->Draw("colz");
// v2->getHist1D()->Draw();

ScanVertexFinder *v3=new ScanVertexFinder(cc, (VertexPos*) v2, VertexFinder::SPHERICAL, VertexFinder::TIMES, VertexFinder::LOCK12);

cout<<"v3, just R"<<endl;

v3->findVertex();
v3->printout();
}