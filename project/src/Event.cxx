#include "Event.h"

ClassImp (Event)

  Event::Event(): TRandom3(),
  Eseed(123),
  Emult(0),
  Evertex({-999, -999, -999})
  {
  }

  Event::Event(unsigned int seed):TRandom3(seed),
  Eseed(seed),
  Emult(0),
  Evertex({-999, -999, -999})
  {
  }
  
  Event::Event (const Event &source):
   TRandom3(source),
   Eseed(source.Eseed),
   Evertex(source.Evertex)
   {
   }

  Event::~Event(){
  }

//------------------------ MEMBER FUNCTIONS -------------------------------------//
  



  TH1F* Event::LoadDistribution(const char* histName) {
      TFile f("kinem.root");
      TH1F* h = (TH1F*)f.Get(histName);
      if (h) {
          h = (TH1F*)h->Clone("hCustomDist");
          h->SetDirectory(0); // Detach h from the file object
      }
      f.Close();
      return h;
  }


  double Event::RndmGaus(double mean, double sigma){
    return Gaus(mean,sigma);
  }

  double Event::RndmUni(double min=1, double max=5){
   return Uniform(min,max);

  }
  
double Event::RndmCustom(TH1F* customHist) {
    
    return customHist->GetRandom(this);
}


void Event::SetVertex() {
    for (unsigned int i = 0; i < 3; ++i) {
        Evertex[i] = PickNCheckVertRndm(i);
    }
}

  void Event::SetMultiplicity(const string& distr){
    if(distr=="gaus"){

     double mean, sigma;
      do{
       Emult=(int)RndmGaus(mean=20,sigma=10);
        }
      while(Emult<1);
    }


    else if(distr=="uni"){
     double min=10;
     double max=20;

      
      Emult=(int)RndmUni(min,max);
       
    }


    else if(distr=="custom"){
      static TH1F* customHist = nullptr;   // A static keeps it's value in different calls of the same function
                                           // But it's limited to it's scope, so it needs to be initialized outside of the if condition  
      if (!customHist) {  
        customHist = LoadDistribution("hm"); 
      }
      do{
       Emult=(int)RndmCustom(customHist); // Typecast since RndCustom returns a double
       }
      while(Emult<0);
    }
  }  

  double Event::GetVertex(unsigned int i) const{
    if(i< Evertex.size()){
      return Evertex[i];
    }
    else{cout<<"invalid input"<<endl;
        return 0;
        }
  }

  void Event::PrintEvent()const{
    cout<<"Generated vertex coordinates (x,y,z): "<<endl;
    if (Evertex.size()>0){
      for(unsigned int i=0;i<Evertex.size();++i){
        cout<<Evertex[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"No vertex found"<<endl;
    }
    if (Emult>=0) cout<<"Multiplicity: "<<Emult<<endl;
  }

    
  double Event::PickNCheckVertRndm(unsigned int numcoord){

   double rndmN;
    if(numcoord==0||numcoord==1){
      do {
        rndmN=RndmGaus(0,0.01);
      }
      while (rndmN>=3||rndmN<=-3); 
    }
  
    else{
      do {
        rndmN=RndmGaus(0,5.3);
      }
      while (rndmN>13.5||rndmN<-13.5);
    } 
    return rndmN;  
  }

  
