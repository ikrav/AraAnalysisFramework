{

  gROOT->SetStyle("Plain");

  EventGenerator gen;
  
  int N=512;
  double dt=0.5;
  
  double hit_time=120;
  double amplitude=500;
  double width=10;
  double noise_rms=40;
  
  // make the noise only waveform
  TGraph *grNoise=new TGraph();
  gen.fillGraphWithNoise(grNoise, noise_rms);
  grNoise->SetLineColor(2);
  
  // make the signal only graph
  TGraph *grSignal=new TGraph();
  for(int i=0;i<N;i++) grSignal->SetPoint(i, i*dt, 0);
  gen.addSignalToGraph(grSignal, hit_time, amplitude, width);
  grSignal->SetLineColor(4);
  
  // make the combined graph
  TGraph *grNoiseSignal=grNoise->Clone();
  gen.addSignalToGraph(grNoiseSignal, hit_time, amplitude, width);
  grNoiseSignal->SetLineColor(1);
  
  // plotting
  TCanvas *c1=new TCanvas("c1","example waveform", 2000,1000);
  c1->Divide(1,3);
  
  c1->cd(1);
  grNoise->SetTitle(Form("Noise only, RMS= %3.0f mV ; time (ns); amplitude (mV) ", noise_rms));
  grNoise->Draw("al");
  
  c1->cd(2);
  grSignal->SetTitle(Form("Signal only, t= %5.1f ns, A= %5.1f mV, #sigma= %3.0f ns ; time (ns); amplitude (mV) ", hit_time, amplitude, width));
  grSignal->Draw("al");
  
  c1->cd(3);
  grNoiseSignal->SetTitle("Combined signal and noise; time (ns); amplitude (mV) ");
  grNoiseSignal->Draw("al");
  
  
  
//   TMultiGraph *mg=new TMultiGraph();
//   TLegend *leg=new TLegend(0.65, 0.65, 0.88, 0.88);
  
//   mg->Add(grNoise);
//   mg->Add(grSignal);
//   mg->Add(grNoiseSignal);
//   mg->SetTitle("Simple generated waveform; time (ns); amplitude (mV)");
  
//   mg->Draw("al");
  
//   leg->AddEntry(grNoise, Form("noise only, RMS= %3.0f mV  ", noise_rms), "l");
//   leg->AddEntry(grSignal, Form("signal, t= %5.1f ns, A= %5.1f mV, #sigma= %3.0f ns  ", hit_time, amplitude, width), "l");
//   leg->AddEntry(grNoiseSignal, "combined signal and noise  ", "l");
  
//   leg->Draw();
  
  GTools::saveCanvas(c1, "plots/example_waveform");
}