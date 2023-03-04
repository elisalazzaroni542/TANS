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

  HitPoint::HitPoint(const Event &ev,const Trajectory &traj,double Rcil):Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hdelta(pow(ev.GetVertix(1)*traj.GetParC(1)+ev.GetVertix(2)*traj.GetParC(2),2)-(pow(traj.GetParC(1),2)+pow(traj.GetParC(2),2))*(pow(ev.GetVertix(1),2)+pow(ev.GetVertix(2),2)-pow(Rcil,2))),
  Ht((-(ev.GetVertix(1)*traj.GetParC(1)+ev.GetVertix(2)*traj.GetParC(2))+sqrt(Hdelta))/(pow(traj.GetParC(1),2)+pow(traj.GetParC(2),2)))
  {
    //standard constructor
     //cout<<"std constr-this= "<<this<<endl;
     if(Ht<0){
       Ht=(-(ev.GetVertix(1)*traj.GetParC(1)+ev.GetVertix(2)*traj.GetParC(2))-sqrt(Hdelta))/(pow(traj.GetParC(1),2)+pow(traj.GetParC(2),2));
     }
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
/*
 void HitPoint::SetT(const Event &ev,const Trajectory &traj,double Rcil){
    Ht=(-(ev.GetVertix(1)*traj.GetParC(1)+ev.GetVertix(2)*traj.GetParC(2))+sqrt(Hdelta))/(pow(traj.GetParC(1),2)+pow(traj.GetParC(2),2));
    if(Ht>=0){
        cout<<"parametro t= "<<Ht<<endl;
        } 
    else {
        Ht=(-(ev.GetVertix(1)*traj.GetParC(1)+ev.GetVertix(2)*traj.GetParC(2))-sqrt(Hdelta))/(pow(traj.GetParC(1),2)+pow(traj.GetParC(2),2));
        cout<<"parametro t= "<<Ht<<endl;
    } 
 }*/
 void HitPoint::PrintHit()const {
    cout<<"Discriminante dell'equazione Delta: "<<Hdelta<<endl;
    cout<<"Parametro t: "<<Ht<<endl;
    cout<<"Coordinate punto di impatto: "<<Hx<<", "<<Hy<<", "<<Hz<<endl;
 }
 
 void HitPoint::SetPoint(const Event &ev,const Trajectory &traj){
    Hx=ev.GetVertix(1)+traj.GetParC(1)*Ht;
    Hy=ev.GetVertix(2)+traj.GetParC(2)*Ht;
    Hz=ev.GetVertix(3)+traj.GetParC(3)*Ht;
 }