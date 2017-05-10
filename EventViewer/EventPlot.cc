#include "EventPlot.h"

EventPlot::EventPlot() : L2Reader() {

  initialize();
}

EventPlot::EventPlot(string input, string branchName, string treeName) : L2Reader(input, branchName, treeName) {

  initialize();
}

EventPlot::~EventPlot(){

  if(_hist_x) delete _hist_x; _hist_x=0;
  if(_hist_y) delete _hist_y; _hist_y=0;
  if(_hist_z) delete _hist_z; _hist_z=0;
  

  if(_canvas) delete _canvas; _canvas=0;
  if(_canvas3) delete _canvas3; _canvas3=0;
 
  clearVectors();
}

void EventPlot::initialize(){
  
  _canvas=0;
  _canvas3=0;
  _hist_x =0;
  _hist_y =0;
  _hist_z =0;

  clearVectors();

  _hist_x_name = "_hist_x";
  _hist_y_name = "_hist_y";
  _hist_z_name = "_hist_z";

  _title_x = "";
  _title_y = "";
  _title_z = "";

  _n_bins = 200;
  
  //these  will change
  //  _x_min = _y_min = -100;
  ///  _x_max = _x_max = 100;
  //  _z_min = -250;
  //  _z_max = 0;
  _true_x = _true_y = _true_z;

  _x_min = _y_min = _z_min = -10;
  _x_max = _y_max = _z_max = 10;

  _hist_x = new TH1D(_hist_x_name, _title_x, _n_bins, _x_min, _x_max);
  _hist_y = new TH1D(_hist_y_name, _title_y, _n_bins, _y_min, _y_max);
  _hist_z = new TH1D(_hist_z_name, _title_z, _n_bins, _z_min, _z_max);
  
  _hist_x->SetLineWidth(3);
  _hist_y->SetLineWidth(3);
  _hist_z->SetLineWidth(3);
  
  _hist_x->GetXaxis()->SetLabelSize(0.06);
  _hist_y->GetXaxis()->SetLabelSize(0.06);
  _hist_z->GetXaxis()->SetLabelSize(0.06);

  _hist_x->GetXaxis()->SetLabelSize(0.06);
  _hist_y->GetXaxis()->SetLabelSize(0.06);
  _hist_z->GetXaxis()->SetLabelSize(0.06);


  _hist_x->GetXaxis()->SetTitle("X_found - X_true");
  _hist_y->GetXaxis()->SetTitle("Y_found - Y_true");
  _hist_z->GetXaxis()->SetTitle("Z_found - Z_true");
  
  _hist_x->GetXaxis()->SetTitleOffset(1.4);
  _hist_y->GetXaxis()->SetTitleOffset(1.4);
  _hist_z->GetXaxis()->SetTitleOffset(1.4);
  
  _hist_x->GetXaxis()->SetTitleSize(0.05);
  _hist_y->GetXaxis()->SetTitleSize(0.05);
  _hist_z->GetXaxis()->SetTitleSize(0.05);

  _hist_x->GetXaxis()->SetTitleOffset(1);
  _hist_y->GetXaxis()->SetTitleOffset(1);
  _hist_z->GetXaxis()->SetTitleOffset(1);

  TH1::AddDirectory(kFALSE);
}

void EventPlot::clearVectors(){

  _draw_commands.clear();
  _method_names.clear();
  _commands_timing.clear();
  _timing_names.clear();
} 


void EventPlot::makeCanvas(){

  if(_canvas) delete _canvas; _canvas=0;
  _canvas = new TCanvas();
  _canvas->cd();
}

void EventPlot::processVertices(int coordinate, double trueX, double trueY, double trueZ){
  _true_x = - trueX;
  _true_y = - trueY;
  _true_z = - trueZ;
  //  TString prefix = TString("getEventData()");
 
  TString coordinates_string[2][3] = { {")->getX() + ", ")->getY() + ", ")->getZ() + "},
				       {")->getR() + ", ")->getTheta() + ", ")->getPhi() + "} };
  
  TString posX;
  posX.Form("%f", _true_x);
  
  TString posY;
  posY.Form("%f", _true_y);
  
  TString posZ;
  posZ.Form("%f", _true_z);

  if(next()){

    if(getEventData()){
      
      if(getEventData()->getVertexVpol()){  // VPOL stuff
	int size = getEventData()->getVertexVpol()->getFinderVector().size();
	
	if(size > 0)
	  for(int i=0; i<size; i++){

	    //loop through the vertex finders
	    //for each one of them draw tree
	    //	    TString command = prefix + TString("->getVertexVpol()->getFinder(");
	    TString iString;
	    iString.Form("%d", i);
	    TString command = TString("_vertexVpol->getFinder(");

	    TString commandX = command + iString+ coordinates_string[coordinate][0] + posX + TString(">> ") + _hist_x_name;
	    TString commandY = command + iString + coordinates_string[coordinate][1] + posY + TString(" >> ") + _hist_y_name;
	    TString commandZ = command + iString + coordinates_string[coordinate][2] + posZ + TString(" >> ") + _hist_z_name;

	    std::vector<TString> temp_command;
	    temp_command.push_back(commandX);
	    temp_command.push_back(commandY);
	    temp_command.push_back(commandZ);
	    _draw_commands.push_back(temp_command);
	    
	    string name;
	    string methodName = _event_data->getVertexVpol()->getFinder(i)->getFinderName();
	    methodName += "_VPOL";
	    if(_event_data->getVertexVpol()->getFinder(i)->isCartesian())
	      methodName +="_[CARTESIAN]_";
	    else 
	      methodName +="_[SPHERICAL]_";

	    methodName += _event_data->getVertexVpol()->getFinder(i)->getChannels().getBestChannel()->getFinderName();

	    std::vector<TString> temp_name;
	    temp_name.push_back(methodName);
	    std::cout<<"Name: "<< methodName << std::endl;
	    _method_names.push_back(temp_name);
	  }
      }
      if(getEventData()->getVertexHpol()){ //HPOL stuff
	
	int size = getEventData()->getVertexHpol()->getFinderVector().size();
	if(size >0){
	  for (int i=0; i<size; i++){
	    
	    TString iString;
	    iString.Form("%d", i);
	    TString command = TString("_vertexHpol()->getFinder(");
	    TString commandX = command + iString + coordinates_string[coordinate][0] + posX + TString(">> ") + _hist_x_name;
	    TString commandY = command + iString + coordinates_string[coordinate][1] + posY + TString(">> ") + _hist_y_name;
	    TString commandZ = command + iString + coordinates_string[coordinate][2] + posZ + TString(" >> ") + _hist_z_name;
	     
	  
	    std::vector<TString> temp_command;
	    temp_command.push_back(commandX);
	    temp_command.push_back(commandY);
	    temp_command.push_back(commandZ);

	    _draw_commands.push_back(temp_command);

	    //TString methodName = "_vertexHpol->getFinder(i)->getName()";
	    string methodName = _event_data->getVertexHpol()->getFinder(i)->getFinderName();
	    methodName += "_HPOL";
	    if(_event_data->getVertexHpol()->getFinder(i)->isCartesian())
	      methodName +="_[CARTESIAN]_";
	    else 
	      methodName +="_[SPHERICAL]_";
	    methodName += _event_data->getVertexHpol()->getFinder(i)->getChannels().getBestChannel()->getFinderName();
	    std::vector<TString> temp_name;
	    temp_name.push_back(methodName);
	    _method_names.push_back(temp_name);
	  }
	}
      }
      
      if(getEventData()->getVertexCombined()){ //combined stuff
	
	int size = getEventData()->getVertexCombined()->getFinderVector().size();
	if(size >0)
	  for(int i=0; i<size; i++){
	    
	    TString iString;
	    iString.Form("%d", i);
	    TString command = TString("_vertexCombined()->getFinder(");
	    TString commandX = command + iString + coordinates_string[coordinate][0] + posX + TString(">> ") + _hist_x_name;
	    TString commandY = command + iString + coordinates_string[coordinate][1] + posY + TString(">> ") + _hist_y_name;
	    TString commandZ = command + iString + coordinates_string[coordinate][2] + posZ + TString(" >> ") + _hist_z_name;
	     
	    std::vector<TString> temp_command;
	    temp_command.push_back(commandX);
	    temp_command.push_back(commandY);
	    temp_command.push_back(commandZ);


	    _draw_commands.push_back(temp_command);
	    string methodName = _event_data->getVertexCombined()->getFinder(i)->getFinderName();
	    methodName += "_COMBINED";
	    if(_event_data->getVertexCombined()->getFinder(i)->isCartesian())
	      methodName +="_[CARTESIAN]_";
	    else 
	      methodName +="_[SPHERICAL]_";
	    
	    methodName += _event_data->getVertexCombined()->getFinder(i)->getChannels().getBestChannel()->getFinderName();
	    	    
	    std::vector<TString> temp_name;
	    temp_name.push_back(methodName);
	    _method_names.push_back(temp_name);
	  }//for loop
      }
    }
  }  //next()
}

//void EventPlot::fillHistos(double h1, double h2, double h3){
void EventPlot::fillHistos(){

}


void EventPlot::setLimits(int xLow, int xHigh, int yLow, int yHigh, int zLow, int zHigh){

  _hist_x->GetXaxis()->SetLimits(xLow, xHigh);
  _hist_y->GetXaxis()->SetLimits(zLow, yHigh);
  _hist_z->GetXaxis()->SetLimits(zLow, zHigh);

}

void EventPlot::printout(){

  for(int i=0; i<_draw_commands.size(); i++){

    std::cout <<"COMMAND #: " << i << std::endl;
    for(int j=0; j<_draw_commands[i].size(); j++){ 
      std::cout <<"\t "<< _draw_commands[i][j] << std::endl;
    }
    std::cout << "\n \n"<< std::endl;
  }  
} 

void EventPlot::draw(){

  makeCanvas();  
  if(_draw_commands.size() == 0) {
    std::cerr<<" No commands in the buffer " << std::endl;
    return;
  }
  if(_draw_commands[0].size() == 0){
    std::cerr <<"No additional commands in buffer "<< std::endl;
    return;
  }

  _tree->Draw(_draw_commands[0][0]);
}

void EventPlot::drawCommand(int commandID){

  if(_draw_commands.size() < commandID){
    std::cerr<<" This command does not exist "<< std::endl;
    return;
  }
  
  make3Canvas();
  _canvas3->cd(1);
  _tree->Draw(_draw_commands[commandID][0]);
  _canvas3->cd(2);
  _tree->Draw(_draw_commands[commandID][1]);
  _canvas3->cd(3);
  _tree->Draw(_draw_commands[commandID][2]);
}

void EventPlot::drawCommandAndTitle(int commandID){

  if(_draw_commands.size() < commandID){
    std::cerr<<" This command does not exist "<< std::endl;
    return;
  }
  
  make3Canvas();
  _canvas3->cd(1);
  _hist_x->SetTitle(_method_names[commandID][0]);
  _tree->Draw(_draw_commands[commandID][0]);
  _canvas3->cd(2);
  _hist_y->SetTitle(_method_names[commandID][0]);
  _tree->Draw(_draw_commands[commandID][1]);
  _canvas3->cd(3);
  _hist_z->SetTitle(_method_names[commandID][0]);
  _tree->Draw(_draw_commands[commandID][2]);

}

void EventPlot::drawAllSave(){

  for(int i=0; i<_draw_commands.size(); i++){

    drawCommandAndTitle(i);
    TString output= TString("EventPlot_")+_method_names[i][0]+ TString(".png");
    _canvas3->SaveAs(output);
  }

}

void EventPlot::make3Canvas(){

  if(_canvas3) delete _canvas3;
  _canvas3 = 0;
  
  _canvas3 = new TCanvas();
  _canvas3->Divide(3,1);
}

TH1D *EventPlot::getHistX(){

  return _hist_x;
}

TH1D *EventPlot::getHistY(){

  return _hist_y;
}

TH1D *EventPlot::getHistZ(){

  return _hist_z;
}


TCanvas *EventPlot::getCanvas3(){

  return _canvas3;
}

TCanvas *EventPlot::getCanvas(){

  return _canvas;
}


void EventPlot::setAxisTitleCartesian(){

  if(_hist_x && _hist_y && _hist_z){
    
    _hist_x->SetXTitle("Reconstructed X, m");
    _hist_y->SetXTitle("Reconstructed Y, m");
    _hist_z->SetXTitle("Reconstructed Z, m");
  }
  else {
    std::cerr<<" No histograms already set "<< std::endl;
    return;
  }
}


void EventPlot::setAxisTitleCartesianWithTrue(){


  if(_hist_x && _hist_y && _hist_z){
    
    _hist_x->SetXTitle("X_found - X_true, m");
    _hist_y->SetXTitle("Y_found - Y_true, m");
    _hist_z->SetXTitle("Z_found - Z_true, m");
  }
  else{
    std::cerr<<"Histograms are NULL"<< std:: endl;
    return;
  }
  
}

void EventPlot::setAxisTitleSpherical(){

  if(_hist_x && _hist_y && _hist_z){
    
    _hist_x->SetXTitle("R, meters");
    _hist_y->SetXTitle("Theta, radians");
    _hist_z->SetXTitle("Phi, radians");
  }
  else {
    std::cerr <<"Histograms are NULL"<< std::endl;
  }
}

void EventPlot::setAxisTitleSphericalWithTrue(){

  if(_hist_x && _hist_y && _hist_z){
    
    _hist_x->SetXTitle("R_found - R_true, m");
    _hist_y->SetXTitle("Theta_found - Theta_true, radians");
    _hist_z->SetXTitle("Phi_found - Phi_true, radians");
  }
  else {
    std::cerr <<"Histograms are NULL"<< std::endl;
  }
}
