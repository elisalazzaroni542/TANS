#include <Riostream.h>
#include "Trajectory.h"
#include "Event.h"
//#include <TRandom3.h>
#include <TFile.h>
#include <math.h>
//#include <TH1F.h>




ClassImp (Trajectory)

  Trajectory::Trajectory(): Event(),
  Ttheta(0),
  Tphi(0),
  TparC({-999, -999, -999})
  //TparCSize(0),
  //TparC(NULL)
  {
    //default constructor
      //cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  Trajectory::Trajectory(unsigned int seed):Event(seed),
  Ttheta(0),
  Tphi(0),
  TparC({-999, -999, -999})
  //TparCSize(0),
  //TparC(NULL)
  {
    //standard constructor
     //cout<<"std constr-this= "<<this<<endl;
  }
  
  Trajectory::Trajectory (const Trajectory &source):
    Event(source),
    Ttheta(source.Ttheta),
    Tphi(source.Tphi),
    TparC(source.TparC)
    {

      //copy constructor

    }


  //TparCSize(source.TparCSize)
  //{//copy constructor
  //  //cout<<"copy constructor-this= "<<this<<endl;
  //  if (TparCSize>0){
  //    TparC=new double[TparCSize];
  //    for(int i=0;i<TparCSize;++i){
  //      TparC[i]=source.TparC[i];
  //    }
  //  }
  //  else {
  //    TparC=NULL;
  //  }
//
  //}

  Trajectory::~Trajectory(){
    //default destructor
    //cout<<"destructor-this= "<<this<<endl;
    //if (TparCSize>0) delete []TparC;
  }

 //------------------------IMPLEMENTAZIONE MEMBER FUNCTIONS-------------------------------------
  //funzioni di estrazione casuale 

//  double Trajectory::RndmCustom(){//reimplementata da Event cambiando il nome del file da leggere
//    TFile f("kinem.root");
//    TH1F *h=(TH1F*)f.Get("heta2");//quello che viene letta in realtà è una distribuizione di pseudorapidità. Bisogna poi trasformarlo in theta
//    double custN=h->GetRandom();
//    //cout<<"Numero estratto: "<<custN<<endl;
//    return custN;
//  }
  
  //funzioni di manipolazione-----------------------------------------------------------

  void Trajectory::SetThetaNPhi(TH1F* customHist){
    double heta=RndmCustom(customHist);//si estrae prima la pseudorapidità heta e poi si trasforma in theta
    Ttheta=2*atan(exp(-heta));
    Tphi=RndmUni(0,2*M_PI);
  }

  void Trajectory::SetParC(unsigned const int size){ // TODO: REMOVE SIZE?
    //if (size!=TparC.size()){ 
      //delete []TparC;
      //TparC=new double[size];
      //TparCSize.resize(size);
      //}

    //SetThetaNPhi();
    TparC[0]=(double)sin(GetTheta())*cos(GetPhi());
    TparC[1]=(double)sin(GetTheta())*sin(GetPhi());
    TparC[2]=(double)cos(GetTheta());
    //cout<<"c1= "<<TparC[0]<<" c2= "<<TparC[1]<<" c3= "<<TparC[2]<<endl;
  }

  double Trajectory::GetParC(unsigned const int i) const{//restituisce il parametro selezionato dalla i
    if(i<TparC.size()){
      //cout<<"parametro "<<i<<" della retta: "<<TparC[i-1]<<endl;
      return TparC[i];
    }
    else{cout<<"Invalid trajectory input"<<endl;
        return 0;
        }
  }


  void Trajectory::PrintTrajectory()const{
    cout<<"Coordinata Theta della traiettoria: "<<Ttheta<<endl;
    cout<<"Coordinata Phi della traiettoria: "<<Tphi<<endl;
    cout<<"Parametri della traiettoria (c1,c2,c3)= "<<endl;
    if (TparC.size()>0){
      for(unsigned int i=0;i<TparC.size();++i){
        cout<<TparC[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"Nessun parametro trovato"<<endl;
    }
  }
//-----------ATTENZIONE----------
/*
  double CheckParT(){
    double t=CalculateParT(double Rcil);
    double test=GetVertix[3]+TparC[3]*t;
    if(test>=-13.5&&test<=13.5) cout<<"t valid"<<endl;
    else cout<<"invalid t: restart event"<<endl;
  }
*/






