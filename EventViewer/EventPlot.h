#ifndef EVENTPLOT_H_
#define EVENTPLOT_H_


#include "TCanvas.h"
#include "TString.h"
#include "TH1D.h"
#include "L2Reader.h"

class EventPlot : public L2Reader {

public:

  EventPlot();
  EventPlot(string input, string branchName, string treeName);
  virtual ~EventPlot();

  void makeCanvas();
  void make3Canvas();
  void fillHistos();
  
  void processVertices(int coordinate=0, double trueX=0.0, double trueY=0.0, double trueZ=0.0);
  
  void draw();

  void clearVectors();
  void printout();
  void drawCommand(int commandID=0);
  void drawCommandAndTitle(int commandID=0);
  void drawAllSave();

  void setAxisTitleCartesian();  
  void setAxisTitleCartesianWithTrue();
  
  
  void setAxisTitleSpherical();
  void setAxisTitleSphericalWithTrue();

  void setLimits(int xLow, int xHigh, int yLow, int yHigh, int zLow, int zHigh);
  //  void setLimits(TH1F *hist, int low, int high);


  TH1D *getHistX();
  TH1D *getHistY();
  TH1D *getHistZ();

  TCanvas *getCanvas3();
  TCanvas *getCanvas();

  TCanvas *_canvas;
  TCanvas *_canvas3;

  TH1D *_hist_x;
  TH1D *_hist_y;
  TH1D *_hist_z;
  
  std::vector< std::vector<TString> > _draw_commands;
  std::vector< std::vector<TString> > _method_names;
  
  std::vector< std::vector<TString> > _commands_timing;
  std::vector< std::vector<TString> > _timing_names;

  TString _hist_x_name;
  TString _hist_y_name;
  TString _hist_z_name;

  TString _title_x;
  TString _title_y;
  TString _title_z;
  
  int _n_bins;

  int _x_min;
  int _x_max;

  int _y_min;
  int _y_max;
  
  int _z_min;
  int _z_max;
  
  double _true_x;
  double _true_y;
  double _true_z;

  void initialize();

};

#endif
