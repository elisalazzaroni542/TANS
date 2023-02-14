#include <Riostream.h>
#include "Event.h"
#include <TRandom3.h>
#include <TFile.h>


ClassImp (Event)

  Event::Event(): TRandom3(),
  Rseed(123),
  EvertixSize(0),
  Emult(0),
  Evertix(NULL)
  {
    //default constructor
      cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  Event::Event(unsigned int seed):TRandom3(seed),
  Rseed(seed),
  EvertixSize(0),
  Emult(0),
  Evertix(NULL)
  {
    //standard constructor
     cout<<"std constr-this= "<<this<<endl;
  }
  
  Event::Event (const Event &source):
   TRandom3(source),
   Rseed(source.Rseed),
   EvertixSize(source.EvertixSize)
  {//copy constructor
    cout<<"copy constructor-this= "<<this<<endl;
    if (EvertixSize>0){
      Evertix=new double[EvertixSize];
      for(int i=0;i<EvertixSize;i++){
        Evertix[i]=source.Evertix[i];
      }
    }
    else {
      Evertix=NULL;
    }

  }

  Event::~Event(){
    //default destructor
    cout<<"destructor-this= "<<this<<endl;
    if (EvertixSize>0) delete []Evertix;
  }

//------------------------IMPLEMENTAZIONE MEMBER FUNCTIONS-------------------------------------
  //funzioni di estrazione casuale

  double Event::RndmGaus(double mean, double sigma){
    double gausN=gRandom->Gaus(mean,sigma);//da intendersi in cm
    cout<<"Numero estratto: "<<gausN<<endl;
    return gausN;
  }

  double Event::RndmUni(double min=1, double max=5){//genera numero distribuito uniformemente tra min e max
    double uniN=gRandom->Uniform(min,max);
    cout<<"Numero estratto: "<<uniN<<endl;
    return uniN;
  }
  /*
  double Event::RndmCustom(string NameFile, string "nomeIsto"){
    TFile f("NameFile");
    TH1F *h=(TH1F*)f.Get("nomeIsto");
    double custN=h->GetRandom();//da intendersi in cm
    cout<<"Numero estratto: "<<custN<<endl;
    return custN;
  }
*/

  //funzioni di manipolazione
  void Event::SetVertix(int size){
    if (EvertixSize>0) delete []Evertix;
    Evertix=new double[size];
    EvertixSize=size;
    for(int i=0;i<EvertixSize;i++) Evertix[i]=PickNCheckRndm(i+1);
  }

  double Event::GetVertix(int i) const{//restituisce la coordinata selezionata dalla i
    if(i>0&&i<=EvertixSize){
      cout<<"coordinata "<<i<<" del vertice: "<<Evertix[i-1]<<endl;
      return Evertix[i-1];
    }
    else{cout<<"invalid input"<<endl;
        return 0;
        }
  }

  void Event::PrintEvent()const{
    cout<<"Coordinate del vertice generato: (x,y,z)= "<<endl;
    if (EvertixSize>0){
      for(int i=0;i<EvertixSize;i++){
        cout<<Evertix[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"Nessun vertice trovato"<<endl;
    }
    if (EMult>=0) cout<<"Molteplicità: "<<EMult<<endl;
  }

    
  double Event::PickNCheckRndm(int numcoord){//numcoord è un numero (1, 2 o 3) che indica per quale coordinata si sta eseguendo il controllo del numero casuale
  //selezione del vertice e controlli per avere numeri compatibili con geometria del rivelatore-----------------
  
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

  

