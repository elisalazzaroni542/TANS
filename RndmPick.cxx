#include <Riostream.h>
#include "RndmPick.h"
#include <TRandom3.h>
#include <vector>

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
    return gausN;
    cout<<"Numero estratto: "<<gausN<<endl;
  }

  void RndmPick::SetVertix(int size){
    if (RvertixSize>0) delete []Rvertix;
    Rvertix=new double[size];
    RvertixSize=size;
    for(int i=0;i<RvertixSize;i++) Rvertix[i]=CheckRndm(i+1);
  }

  double RndmPick::GetVertix(int i) const{//restituisce la coordinata selezionata dalla i
    if(i>=0&&i<RvertixSize){
      return Rvertix[i];
      cout<<"coordinata "<<i<<" del vertice: "<<Rvertix[i]<<endl;
    }
    else{cout<<"invalid input"<<endl;
        return 0;
        }
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

    
  double RndmPick::CheckRndm(int numcoord){//numcoord è un numero (1, 2 o 3) che indica per quale coordinata si sta eseguendo il controllo del numero casuale
  //controlli per avere numeri compatibili con geometria del rivelatore-----------------
   // vector <double> coord;
    //coord.reserve(3);
    //double coord[3];
    if (numcoord<1){
      cout<<"Invalid set-up for function CheckRndm"<<endl;
    }
    else if(numcoord==1){ //check per coordinate x e y: vertice non deve superare il raggio del rivelatore
      double rndmN=RndmGaus(0,0.1);//estrazione del numero
      if(rndmN>=3.08&&rndmN<=-3.08){
        cout<<"coordinate "<<rndmN<<" out of bound"<<endl;
        cout<<"checking again..."<<endl;
        CheckRndm(numcoord);
      }
      else{
        cout<<"Check ok"<<endl;
        return rndmN;
        cout<<"Numero selezionato: "<<rndmN<<endl;
        //coord.push_back(cX);
      }
    }
    else if(numcoord==2){ //check per coordinate x e y: vertice non deve superare il raggio del rivelatore
      double rndmN=RndmGaus(0,0.1);
      if(rndmN>=3.08&&rndmN<=-3.08){
        cout<<"coordinate "<<rndmN<<" out of bound"<<endl;
        cout<<"checking again..."<<endl;
        CheckRndm(numcoord);
      }
      else{
        cout<<"Check ok"<<endl;
        return rndmN;
        cout<<"Numero selezionato: "<<rndmN<<endl;
        //coord.push_back(cY);
      }
    }  
    else if(numcoord==3){ //check per la coordinata z: non deve superare la lunghezza del rivelatore
      double rndmN=RndmGaus(0,5.3);
      if(rndmN>=13.5&&rndmN<=-13.5){// check solo su metà lunghezza perchè parte da zero
        cout<<"coordinate "<<rndmN<<" out of bound"<<endl;
        cout<<"checking again..."<<endl;
        CheckRndm(numcoord);
      }
      else{
        cout<<"Check ok"<<endl;
        return rndmN;
        cout<<"Numero selezionato: "<<rndmN<<endl;
        //coord.push_back(cZ);
        }
    }
    else{cout<<"Invalid set-up for function CheckRndm"<<endl;
        return 0;
        }
  return 0;  
  }

  

