#include <Riostream.h>
#include "Event.h"
#include <TRandom3.h>
#include <TFile.h>
#include <math.h>
#include <string>
//#include <TH1F.h>


ClassImp (Event)

  Event::Event(): TRandom3(),
  Eseed(123),
  //EvertixSize(0),
  Emult(0),
  Evertix({-999, -999, -999})
  {
    //default constructor
      //cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  Event::Event(unsigned int seed):TRandom3(seed),
  Eseed(seed),
  //EvertixSize(0),
  Emult(0),
  Evertix({-999, -999, -999})
  {
    //standard constructor
     //cout<<"std constr-this= "<<this<<endl;
  }
  
  Event::Event (const Event &source):
   TRandom3(source),
   Eseed(source.Eseed),
   Evertix(source.Evertix)
   {

    // copy constructor

   }
   //EvertixSize(source.EvertixSize)
  //{//copy constructor
    //cout<<"copy constructor-this= "<<this<<endl;
  //  if (Evertix.size()>0){
  //    Evertix = source.Evertix;
//      Evertix.resize(3)
  //    for(int i=0;i<Evertix.size();++i){
  //      Evertix[i]=source.Evertix[i];
  //    }
  //  }
    //else {
    //  Evertix=NULL;
    //  Evertix=Evertix();
    //}

  //}

  Event::~Event(){
    //default destructor
    //cout<<"destructor-this= "<<this<<endl;
    //if (Evertix.size()>0) delete Evertix; // This way it eliminates all occupied positions of Evertix
  }

//------------------------IMPLEMENTAZIONE MEMBER FUNCTIONS-------------------------------------
  



  TH1F* Event::LoadDistribution(const char* histName) {
      TFile f("kinem.root");
      TH1F* h = (TH1F*)f.Get(histName);
      if (h) {
          h = (TH1F*)h->Clone("hCustomDist");  // Create a clone that survives after file closes
          h->SetDirectory(0);  // Detach from file
      }
      f.Close();
      return h;
  }

  
  //funzioni di estrazione casuale

  double Event::RndmGaus(double mean, double sigma){
    double gausN=gRandom->Gaus(mean,sigma);//da intendersi in cm
    //cout<<"Numero estratto: "<<gausN<<endl;
    return gausN;
  }

  double Event::RndmUni(double min=1, double max=5){//genera numero distribuito uniformemente tra min e max
    double uniN=gRandom->Uniform(min,max);
    //cout<<"Numero estratto: "<<uniN<<endl;
    return uniN;
  }
  
double Event::RndmCustom(TH1F* customHist) {
    return customHist->GetRandom();
}


  //funzioni di manipolazione--------------------------------------------------------------
//  void Event::SetVertix(int size=3){
void Event::SetVertix(unsigned int size) {
    if (size != Evertix.size()) {
        //Evertix.clear();
        //Evertix = new vector<double>;
        Evertix.resize(size);
    }
    for (unsigned int i = 0; i < size; ++i) {
        Evertix[i] = PickNCheckVertRndm(i);
    }
}

  void Event::SetMultiplicity(const string& distr){
    if(distr=="gaus"){//distribuzione gaussiana di molteplicità
     double mean,sigma;
     /*
     cout<<"Please insert mean and standard deviation for the gaussian disribution: "<<endl;
     cout<<"mean: ";
     cin>>mean;
     cout<<endl<<"std dev: ";
     cin>>sigma;
     */
      do{
       Emult=(int)RndmGaus(mean=20,sigma=5);
        }
      while(Emult<0);
    }
    else if(distr=="uni"){//distribuzione uniforme tra min e max
     double min=1;
     double max=5;
     /*
     cout<<"Please insert range for the uniform disribution: "<<endl;
     cout<<"minimum: ";
     cin>>min;
     cout<<endl<<"maximum: ";
     cin>>max;
     */
      do{
       Emult=(int)RndmUni(min,max);
        }
      while(Emult<0);
    }
    else if(distr=="custom"){//distribuzione letta da file
      static TH1F* customHist = nullptr;   // A static keeps it's value in different calls of the same function
                                           // But it's imited to it's scope, so it needs to be initialized outside of the if condition  
      if (!customHist) {  
        customHist = LoadDistribution("hm"); 
        cout<<"customHistDefined"<<endl; 
      }
      do{
       Emult=(int)RndmCustom(customHist); // Typecast necessario: Emult é un int ma RndmCustom ritorna un double
       }
      while(Emult<0);
    }
  }  

  double Event::GetVertix(unsigned int i) const{//restituisce la coordinata selezionata dalla i = 1,2,3
    if(i< Evertix.size()){
      //cout<<"coordinata "<<i<<" del vertice: "<<Evertix[i-1]<<endl;
      return Evertix[i];
    }
    else{cout<<"invalid input"<<endl;
        return 0;
        }
  }

  void Event::PrintEvent()const{
    cout<<"Coordinate del vertice generato: (x,y,z)= "<<endl;
    if (Evertix.size()>0){
      for(unsigned int i=0;i<Evertix.size();++i){
        cout<<Evertix[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"Nessun vertice trovato"<<endl;
    }
    if (Emult>=0) cout<<"Molteplicità: "<<Emult<<endl;
  }

    
  double Event::PickNCheckVertRndm(unsigned int numcoord){//numcoord è un numero (1, 2 o 3) che indica per quale coordinata si sta eseguendo il controllo del numero casuale
  //selezione del vertice e controlli per avere numeri compatibili con geometria del rivelatore-----------------
   double rndmN;
    if(numcoord==0||numcoord==1){
      do {
        rndmN=RndmGaus(0,0.01);
        //cout<<"Numero selezionato: "<<rndmN<<endl;
      }
      while (rndmN>=3||rndmN<=-3); 
    }
  
    else{
      do {
        rndmN=RndmGaus(0,5.3);
        //cout<<"Numero selezionato: "<<rndmN<<endl;
      }
      while (rndmN>=13.5||rndmN<=-13.5);
    } 
    return rndmN;  
  }

  