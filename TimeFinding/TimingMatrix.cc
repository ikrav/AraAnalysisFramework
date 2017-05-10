#include "TimingMatrix.h"
#include "DeltaFinder.h"

ClassImp(TimingMatrix);

TimingMatrix::TimingMatrix(){
 
  _matrix.clear();
  
}

TimingMatrix::~TimingMatrix(){

  for(int i=0; i<_matrix.size(); i++)
    _matrix[i].clear();

  _matrix.clear();
  
}

void TimingMatrix::printout() const {

  int width=10;

  std::cout<<__PRETTY_FUNCTION__<<" type: "<<DeltaFinder::name_delta_type(_matrix[0][1].getFinderType())<<std::endl;
  
  std::cout << "\n";
  for(int i=0; i<_matrix[0].size(); i++) std::cout<<std::setw(width-1)<<"ch: "<<std::setw(2)<<_matrix[0][i].getSecondChannelId();
  std::cout <<"\n\n";

  //actual printout
  for(int i=0;i<_matrix.size(); i++){

    for(int j=0;j<_matrix[i].size(); j++) {
      
//       if(j==0) std::cout <<"ch: "<<_matrix[i][0].getFirstChannelId();
      std::cout<<std::setw(width)<<_matrix[i][j].getTime();
      if(_matrix[i][j].getWeight()) std::cout<<"*";
      else std::cout<<" ";
      
    }
    
    std::cout<<"\n\n";
        
  }
  
  std::cout<<"\n\n";
  
}

void TimingMatrix::printResiduals() const {

  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  int width=10;
  
  for(int i=0;i<getNumChans(); i++){
   
    for(int j=0;j<_matrix[i].size(); j++){ 
      
      std::cout<<std::setw(width)<<_matrix[i][j].getResidual();
      if(_matrix[i][j].getWeight()) std::cout<<"*";
      else std::cout<<" ";
    
    }
      
    std::cout<<"\n\n";
        
  }
  
  std::cout<<"\n\n";
  
}

void TimingMatrix::printWeights() const {

  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  int width=10;
  
  for(int i=0;i<getNumChans(); i++){
   
    for(int j=0;j<_matrix[i].size(); j++){ 
      
      std::cout<<std::setw(width)<<_matrix[i][j].getWeight();
//       if(_matrix[i][j].getWeight()) std::cout<<"*";
      std::cout<<" ";
    
    }
      
    std::cout<<"\n\n";
        
  }
  
  std::cout<<"\n\n";
  
}

void TimingMatrix::printErrors() const {

  std::cout<<__PRETTY_FUNCTION__<<std::endl;
  
  int width=10;
  
  for(int i=0;i<getNumChans(); i++){
   
    for(int j=0;j<_matrix[i].size(); j++){ 
      
      std::cout<<std::setw(width)<<_matrix[i][j].getError();
      if(_matrix[i][j].getWeight()) std::cout<<"*";
      else std::cout<<" ";
    
    }
      
    std::cout<<"\n\n";
        
  }
  
  std::cout<<"\n\n";
  
}

int TimingMatrix::size() const {
  
  return _matrix.size();
  
}

int TimingMatrix::getNumChans() const {
 
  return _matrix.size();
  
}

int TimingMatrix::getNumCombinations() const {
 
  return getNumChans()*(getNumChans()-1)/2;
  
}

int TimingMatrix::getNumUsefulPairs() const {
 
  int num=0;
  
  for(int i=0;i<getNumChans()-1;i++){
    
    for(int j=i+1;j<getNumChans(); j++){
      
      if(_matrix[i][j].getWeight()>0) num++;
      
    }
    
  }
  
  return num;
  
  
}

PairTiming TimingMatrix::getPair(int channel_id1, int channel_id2){
 
  if(_matrix.empty()){ std::cerr<<"This matrix is empty...\n"; return PairTiming(); }
  
  int index1=-1;
  int index2=-1;
    
  for(int i=0;i<_matrix.size();i++) if(_matrix[i].size() && _matrix[i][0].getFirstChannelId()==channel_id1) index1=i;
  
  for(int i=0;i<_matrix[0].size(); i++) if(_matrix[0][i].getSecondChannelId()==channel_id2) index2=i;
  
  if(index1<0 || index2<0){ std::cerr<<"can't find channels with indices "<<channel_id1<<" and "<<channel_id2<<"\n"; return PairTiming(); }
  
  return _matrix[index1][index2];
  
}

std::vector<PairTiming> &TimingMatrix::operator[](int index){
 
  if(index<0 || index>=_matrix.size()){ std::cerr<<__PRETTY_FUNCTION__<<"  ERROR: index "<<index<<" is out of bounds...\n"; }
  
  return _matrix[index];
  
}

void TimingMatrix::addTimingVector(std::vector<PairTiming> timing_vector){
  
  _matrix.push_back(timing_vector);
  
}

void TimingMatrix::fillResiduals(TH1D *hist){
 
  for(int i=0;i<getNumChans()-1;i++) for(int j=i+1; j<getNumChans(); j++){
   
    hist->Fill(fabs(_matrix[i][j].getResidual()));
    
  }
  
}
