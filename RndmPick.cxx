#include <Riostream.h>
#include "RndmPick.h"
#include <TRandom3.h>


ClassImp (RndmPick)

  RndmPick::RndmPick(): TRandom3(),
  Rseed(123),
  RvertixSize(0),
  Rvertix(NULL)
  {
    //default constructor
      cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  RndmPick::RndmPick(unsigned int seed):TRandom3(seed),
  Rseed(seed),
  RvertixSize(0),
  Rvertix(NULL)
  {
    //standard constructor
     cout<<"std constr-this= "<<this<<endl;
  }
  
  RndmPick::RndmPick (const RndmPick &source):
   TRandom3(source),
   Rseed(source.Rseed),
   RvertixSize(source.RvertixSize)
  {//copy constructor
    cout<<"copy constructor-this= "<<this<<endl;
    if (RvertixSize>0){
      Rvertix=new double[RvertixSize];
      for(int i=0;i<RvertixSize;i++){
        Rvertix[i]=source.Rvertix[i];
      }
    }
    else {
      Rvertix=NULL;
    }

  }

  RndmPick::~RndmPick(){
    //default destructor
    cout<<"destructor-this= "<<this<<endl;
    if (RvertixSize>0) delete []Rvertix;
  }

  //implementazione member functions-------------------------------------

  double RndmPick::RndmGaus(double mean, double sigma){
    //gRandom->SetSeed(Rseed);
    //double *gausPtr;
    double gausN=gRandom->Gaus(mean,sigma);//da intendersi in cm
   // double gausN=*gausPtr;
    cout<<"Numero estratto: "<<gausN<<endl;
    return gausN;
  }

  void RndmPick::SetVertix(int size){
    if (RvertixSize>0) delete []Rvertix;
    Rvertix=new double[size];
    RvertixSize=size;
    for(int i=0;i<RvertixSize;i++) Rvertix[i]=CheckRndm(i+1);
  }

  double RndmPick::GetVertix(int i) const{//restituisce la coordinata selezionata dalla i
    if(i>0&&i<=RvertixSize){
      cout<<"coordinata "<<i<<" del vertice: "<<Rvertix[i-1]<<endl;
      return Rvertix[i-1];
    }
    else{cout<<"invalid input"<<endl;
        return 0;
        }
  }

  void RndmPick::PrintCoordinates()const{
    cout<<"Coordinate del vertice generato: (x,y,z)= "<<endl;
    if (RvertixSize>0){
      for(int i=0;i<RvertixSize;i++){
        cout<<Rvertix[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"Nessun vertice trovato"<<endl;
    }
  }

    
  double RndmPick::CheckRndm(int numcoord){//numcoord è un numero (1, 2 o 3) che indica per quale coordinata si sta eseguendo il controllo del numero casuale
  //controlli per avere numeri compatibili con geometria del rivelatore-----------------
   // vector <double> coord;
    //coord.reserve(3);
    //double coord[3];
    if(numcoord==1||numcoord==2){
      do {
        double rndmN=RndmGaus(0,0.1);
        cout<<"Numero selezionato: "<<rndmN<<endl;
      }
      while (rndmN>=3.08||rndmN<=-3.08) 
    }
    else{
      do {
        double rndmN=RndmGaus(0,5.3);
        cout<<"Numero selezionato: "<<rndmN<<endl;
      }
      while (rndmN>=13.5||rndmN<=-13.5)
    } 
    return rndmN;  
  }

  

