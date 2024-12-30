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


  HitPoint::HitPoint(const Event &ev,const Trajectory &traj,const double Rcil):Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hdelta(0),
  Ht(0)
  {
    SetDelta(ev, traj, Rcil);
    SetT(ev, traj);

  }

  //OLD IMPLEMENTATION
  //HitPoint::HitPoint(const Event &ev,const Trajectory &traj,double Rcil):Trajectory(),
  //Hx(0),
  //Hy(0),
  //Hz(0),
  //Hdelta(pow(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1),2)-(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2))*(pow(ev.GetVertix(0),2)+pow(ev.GetVertix(1),2)-pow(Rcil,2))),
  //Ht((-(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1))+sqrt(Hdelta))/(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2)))
  //{
  //  //standard constructor
  //   //cout<<"std constr-this= "<<this<<endl;
  //   if(Ht<0){
  //     Ht=(-(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1))-sqrt(Hdelta))/(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2));
  //   }
  //}
  
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

  void HitPoint::SetDelta(const Event &ev, const Trajectory &traj, const double Rcil){

    double first = pow(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1),2);
    double second = (pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2))*(pow(ev.GetVertix(0),2)+pow(ev.GetVertix(1),2)-pow(Rcil,2));

    Hdelta = first - second;
    //cout<<"Delta="<<Hdelta<<endl;
  }

  void HitPoint::MSSetDelta(const HitPoint &h, const Trajectory &traj, const double Rcil){

    double first = pow(h.GetX()*traj.GetParC(0)+h.GetY()*traj.GetParC(1),2);
    double second = (pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2))*(pow(h.GetX(),2)+pow(h.GetY(),2)-pow(Rcil,2));

    Hdelta = first - second;
    //cout<<"Delta="<<Hdelta<<endl;
  }

  void HitPoint::SetT(const Event &ev, const Trajectory &traj){

  Ht = (-(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1))+sqrt(Hdelta))/(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2));

     if(Ht<0){
        Ht=(-(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1))-sqrt(Hdelta))/(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2));
     }
  //cout<<"T="<<Ht<<endl;
  
}

void HitPoint::MSSetT(const HitPoint &h, const Trajectory &traj){

  Ht = (-(h.GetX()*traj.GetParC(0)+h.GetY()*traj.GetParC(1))+sqrt(Hdelta))/(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2));

     if(Ht<0){
        Ht=(-(h.GetX()*traj.GetParC(0)+h:GetY()*traj.GetParC(1))-sqrt(Hdelta))/(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2));
     }
  //cout<<"T="<<Ht<<endl;
  
}

 void HitPoint::PrintHit()const {
  
    cout<<"Discriminante dell'equazione Delta: "<<Hdelta<<endl;
    cout<<"Parametro t: "<<Ht<<endl;
    cout<<"Coordinate punto di impatto: "<<Hx<<", "<<Hy<<", "<<Hz<<endl;
 }
 
 void HitPoint::SetPoint(const Event &ev,const Trajectory &traj){
    Hx=ev.GetVertix(0)+traj.GetParC(0)*Ht;
    Hy=ev.GetVertix(1)+traj.GetParC(1)*Ht;
    Hz=ev.GetVertix(2)+traj.GetParC(2)*Ht;


 }

 void HitPoint::MSSetPoint(const HitPoint &h,const Trajectory &traj){
    Hx=h.GetX()+traj.GetParC(0)*Ht;
    Hy=h.GetY()+traj.GetParC(1)*Ht;
    Hz=h.GetZ()+traj.GetParC(2)*Ht;


 }