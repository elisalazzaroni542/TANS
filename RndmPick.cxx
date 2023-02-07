#include <Riostream.h>
#include "RndmPick.h"
#include <TRandom3.h>

ClassImp (RndmPick)

  RndmPick::RndmPick(): TRandom3(),
  Rseed(123.),
  RvertixSize(0.),
  Rvertix(NULL)
  {
    //default constructor
      cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  RndmPick::RndmPick(unsigned int seed):TRandom3(),
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
    gRandom->SetSeed(Rseed);
    double *gausPtr;
    gausPtr=gRandom->Gaus(mean,sigma)//da intendersi in cm
    double gausN=*gausPtr;
    return gausN;
  }

  void RndmPick::SetVertix(double *coord, double size ){
    if (RvertixSize>0) delete []Rvertix;
    Rvertix=new double[size];
    RvertixSize=size;
    for(int i=0;i<RvertixSize;i++) Rvertix[i]=coord[i];
  }

  void RndmPick::PrintCoordinates()const{
    cout<<"Coordinate del vertice generato: (x,y,z)= "<<endl;
    if (RvertixSize>0){
      for(int i=0;i<=RvertixSize;i++){
        cout<<Rvertix[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"Nessun vertice trovato"<<endl;
    }
  }

  

