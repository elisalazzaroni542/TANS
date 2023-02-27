#include <Riostream.h>
#include "HitPoint.h"
#include "Trajectory.h"
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
  Hx(0),
  Hy(0),
  Hz(0),
  Hdelta(pow(ev.Evertix[1]*traj.TparC[1]+ev.Evertix[2]*traj.TparC[2],2)-(pow(traj.TparC[1],2)+pow(traj.TparC[2],2))*(pow(ev.Evertix[1],2)+pow(ev.Evertix[2],2)-pow(Rcil,2))),
  Ht(0)
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