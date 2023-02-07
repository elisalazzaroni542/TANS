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
    //gRandom->SetSeed(Rseed);
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

  double RndmPick::GetVertix(int i){//restituisce la coordinata selezionata dalla i
    if(i>=0&&i<RvertixSize){
      return Rvertix[i];
    }
    else{cout<<"invalid input"<<endl;}
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

 
    
  double RndmPick::CheckRndm(double &rndmN, int numcoord){//numcoord è un numero (1, 2 o 3) che indica per quale coordinata si sta eseguendo il controllo del numero casuale
  //controlli per avere numeri compatibili con geometria del rivelatore-----------------
    if (numcoord<1){
      cout<<"Invalid set-up for function CheckRndm"<<endl;
    }
    else if(numcoord=1){ //check per coordinate x e y: vertice non deve superare il raggio del rivelatore
      if(rndmN>=3.08&&rndmN<=-3.08){
        rndmN=RndmGaus(0,0.1);//media e sigma per le coordinate x e y
        cout<<"New coordinate set: "<<rndmN<<endl;
        cout<<"checking again..."<<endl;
        CheckRndm(rndmN,numcoord);
      }
      else{
        cout<<"Check ok";
        double cX=rndmN;
        return cX;
      }
    }
    else if(numcoord=2){ //check per coordinate x e y: vertice non deve superare il raggio del rivelatore
      if(rndmN>=3.08&&rndmN<=-3.08){
        rndmN=RndmGaus(0,0.1);//media e sigma per le coordinate x e y
        cout<<"New coordinate set: "<<rndmN<<endl;
        cout<<"checking again..."<<endl;
        CheckRndm(rndmN,numcoord);
      }
      else{
        cout<<"Check ok";
        double cY=rndmN;
        return cY;
      }
    }  
    else if(numcoord=3){ //check per la coordinata z: non deve superare la lunghezza del rivelatore
      if(rndmN>=13.5&&rndmN<=-13.5){// check solo su metà lunghezza perchè parte da zero
        rndmN=RndmGaus(0,5.3);//media e sigma per la coordinata z
        cout<<"New coordinate set: "<<rndmN<<endl;
        cout<<"checking again..."<<endl;
        CheckRndm(rndmN,numcoord);
      }
      else{
        cout<<"Check ok";
        double cZ=rndmN;
        return cZ;
        }
    }
    else{cout<<"Invalid set-up for function CheckRndm"<<endl;}  
  }

  

