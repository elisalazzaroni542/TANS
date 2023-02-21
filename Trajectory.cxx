#include <Riostream.h>
#include "Trajectory.h"
#include "Event.h"
#include <TRandom3.h>
#include <TFile.h>
#include <TMath.h>




ClassImp (Trajectory)

  Trajectory::Trajectory(): Event(),
  Ttheta(0),
  Tphi(0),
  TparCSize(0),
  TparC(NULL)
  {
    //default constructor
      //cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  Trajectory::Trajectory(unsigned int seed):Event(seed),
  Ttheta(0),
  Tphi(0),
  TparCSize(0),
  TparC(NULL)
  {
    //standard constructor
     //cout<<"std constr-this= "<<this<<endl;
  }
  
  Trajectory::Trajectory (const Trajectory &source):
   Event(source),
   Ttheta(source.Ttheta),
   Tphi(source.Tphi),
   TparCSize(source.TparCSize)
  {//copy constructor
    //cout<<"copy constructor-this= "<<this<<endl;
    if (TparCSize>0){
      TparC=new double[TparCSize];
      for(int i=0;i<TparCSize;i++){
        TparC[i]=source.TparC[i];
      }
    }
    else {
      TparC=NULL;
    }

  }

  Trajectory::~Trajectory(){
    //default destructor
    //cout<<"destructor-this= "<<this<<endl;
    if (TparCSize>0) delete []TparC;
  }

 //------------------------IMPLEMENTAZIONE MEMBER FUNCTIONS-------------------------------------
  //funzioni di estrazione casuale 

  double Event::RndmCustom(){//reimplementata da Event cambiando il nome del file da leggere
    TFile f("kinem.root");
    TH1F *h=(TH1F*)f.Get("heta2");//quello che viene letta in realtà è una distribuizione di pseudorapidità. Bisogna poi trasformarlo in theta
    double custN=h->GetRandom();
    //cout<<"Numero estratto: "<<custN<<endl;
    return custN;
  }
  
  //funzioni di manipolazione-----------------------------------------------------------

  void Trajectory::SetThetaNPhi(){
    double Heta=RndmCustom();//si estrae prima la rapidità Heta e poi si trasforma in theta
    Etheta=2*ATan(Exp(-Heta));
    Ephi=RndmUni(0,2*Pi());
  }

  void Trajectory::SetParC(int size=3){
    if (TparCSize>0) delete []TparC;
    TparC=new double[size];
    TparCSize=size;
    SetThetaNPhi();
    TparC[1]=Sin(GetTheta())*Cos(GetPhi());
    TparC[2]=Sin(GetTheta())*Sin(GetPhi())
    TparC[3]=Cos(GetTheta());
    cout<<"c1= "<<TparC[1]<<"c2= "<<TparC[2]<<"c3= "<<TparC[3]<<endl;
  }

  double Trajectory::GetParC(int i) const{//restituisce il parametro selezionato dalla i
    if(i>0&&i<=TparCSize){
      //cout<<"parametro "<<i<<" della retta: "<<TparC[i-1]<<endl;
      return TparC[i-1];
    }
    else{cout<<"invalid input"<<endl;
        return 0;
        }
  }


//in alternativa queste due funzioni possono essere implementate direttamente nel main
  double Trajectory::CalculateDelta(double Rcil){
    double Delta=Power(Evertix[1]*TparC[1]+Evertix[2]*TparC[2],2)-(Power(TparC[1],2)+Power(TparC[2],2))*(Power(Evertix[1],2)+Power(Evertix[2],2)-Power(Rcil,2));
    return Delta;
  }

  double Trajectory::CalculateParT(double Rcil){
    double parTp=(-(Evertix[1]*TparC[1]+Evertix[2]*TparC[2])+Sqrt(CalculateDelta(Rcil)))/(Power(TparC[1],2)+Power(TparC[2],2));
    double parTm=(-(Evertix[1]*TparC[1]+Evertix[2]*TparC[2])-Sqrt(CalculateDelta(Rcil)))/(Power(TparC[1],2)+Power(TparC[2],2));
    if(parTp>=0) return parTp;
    else if (parTm>=0) return parTm;
    else {cout<<"t always negative"<<endl;
         return 0;
    } 
  }

  void Trajectory::PrintTrajectory()const{
    cout<<"Coordinata Theta della traiettoria: "<<Ttheta<<endl;
    cout<<"Coordinata Phi della traiettoria: "<<Tphi<<endl;
    cout<<"Parametri della traiettoria (c1,c2,c3)= "<<endl;
    if (TparCSize>0){
      for(int i=0;i<TparCSize;i++){
        cout<<TparC[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"Nessun parametro trovato"<<endl;
    }
  }
//-----------ATTENZIONE----------
//questa funzione probabilmente è da implementare nel main
/*
  double CheckParT(){
    double t=CalculateParT(double Rcil);
    double test=Evertix[3]+TparC[3]*t;
    if(test>=-13.5&&test<=13.5) cout<<"t valid"<<endl;
    else cout<<"invalid t: restart event"<<endl;
  }
*/






