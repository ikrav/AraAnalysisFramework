{

  TFile *f=new TFile("results.root");
  
  TTree *T=(TTree*) f->Get("tree");
  
  int numEntries=T->GetEntries();
  
  EventData *event=0;
  
  T->SetBranchAddress("DataBranch", &event);
  
  cout<<"N= "<<numEntries<<" T= "<<T<<endl;
  
  TH1D *histR=new TH1D("histR", "residuals in R; #delta R [m]", 300, -3000, 3000);
  TH1D *histTheta=new TH1D("histTheta", "residuals in #theta; #Delta#theta [deg]", 300, -30, 30);
  TH1D *histTheta=new TH1D("histTheta", "residuals in #phi; #Delta#theta [deg]", 300, -30, 30);
  
  
  for(int e=0;e<numEntries;e++){
    
    T->GetEntry(e);
    
    double foundR=event->getVertexResult()->getR();
    double realR=event->getVertexResult()->getRealPos()->getR();
    
    histR->Fill(realR-foundR);
    
    double foundTheta=event->getVertexResult()->getTheta();
    double realTheta=event->getVertexResult()->getRealPos()->getTheta();
        
    histTheta->Fill(Pos::rad2deg(realTheta-foundTheta));
    
    double foundPhi=event->getVertexResult()->getPhi();
    double realPhi=event->getVertexResult()->getRealPos()->getPhi();
    
    histPhi->Fill(Pos::rad2deg(realPhi-foundPhi));
      
  }// for e

}