#include <Riostream.h>
#include "HitPoint.h"
#include "Trajectory.h"
#include "Event.h"
#include <TRandom3.h>
#include <TFile.h>
#include <math.h>




ClassImp (HitPoint)

  HitPoint::HitPoint(): Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hdelta(0),
  Ht(0)
  {
    //default constructor
      //cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  HitPoint::HitPoint(const Event &ev,const Trajectory &traj,double Rcil):Trajectory(seed),
  Hx(ev.Evertix[1]+traj.TparC[1]*Ht),
  Hy(ev.Evertix[2]+traj.TparC[2]*Ht),
  Hz(ev.Evertix[3]+traj.TparC[3]*Ht),
  Hdelta(pow(ev.Evertix[1]*traj.TparC[1]+ev.Evertix[2]*traj.TparC[2],2)-(pow(traj.TparC[1],2)+pow(traj.TparC[2],2))*(pow(ev.Evertix[1],2)+pow(ev.Evertix[2],2)-pow(Rcil,2))),
  Ht((-(ev.Evertix[1]*traj.TparC[1]+ev.Evertix[2]*traj.TparC[2])+sqrt(Hdelta))/(pow(traj.TparC[1],2)+pow(traj.TparC[2],2)))
  {
    //standard constructor
     //cout<<"std constr-this= "<<this<<endl;
  }
  
  HitPoint::HitPoint (const HitPoint &source):
   Trajectory(source),
   Hx(source.Hx),
   Hy(source.Hy),
   Hz(source.Hz),
   Hdelta(source.Hdelta),
   Ht(source.Ht)
  {//copy constructor
    //cout<<"copy constructor-this= "<<this<<endl;

  }

  HitPoint::~HitPoint(){
    //default destructor
    //cout<<"destructor-this= "<<this<<endl;
    
  }

 //------------------------IMPLEMENTAZIONE MEMBER FUNCTIONS-------------------------------------

 void HitPoint::SetT(const Event &ev,const Trajectory &traj,double Rcil){
    Ht=(-(ev.Evertix[1]*traj.TparC[1]+ev.Evertix[2]*traj.TparC[2])+sqrt(Hdelta))/(pow(traj.TparC[1],2)+pow(traj.TparC[2],2));
    if(Ht>=0){
        cout<<"parametro t= "<<Ht<<endl;
        } 
    else {
        Ht=(-(ev.Evertix[1]*traj.TparC[1]+ev.Evertix[2]*traj.TparC[2])-sqrt(Hdelta))/(pow(traj.TparC[1],2)+pow(traj.TparC[2],2));
        cout<<"parametro t= "<<Ht<<endl;
    } 
 }
 void HitPoint::PrintHit()const {
    cout<<"Discriminante dell'equazione Delta: "<<Hdelta<<endl;
    cout<<"Parametro t: "<<Ht<<endl;
    cout<<"Coordinate punto di impatto: "<<Hx<<", "<<Hy<<", "<<Hz<<endl;
 }
 /*
 void HitPoint::SetPoint(const Event &ev,const Trajectory &traj){
    Hx=ev.Evertix[1]+traj.TparC[1]*Ht;
    Hy=ev.Evertix[2]+traj.TparC[2]*Ht;
    Hz=ev.Evertix[3]+traj.TparC[3]*Ht;
 }*/