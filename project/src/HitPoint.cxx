#include "HitPoint.h"




ClassImp (HitPoint)

  HitPoint::HitPoint(): Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hdelta(0),
  Ht(0)
  {
  }


  HitPoint::HitPoint(const Event &ev,const Trajectory &traj,const double Rcil):Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hdelta(0),
  Ht(0)
  {
    SetDelta_and_T(ev, traj, Rcil);

  }
  
  HitPoint::HitPoint (const HitPoint &source):
    Trajectory(source),
    Hx(source.Hx),
    Hy(source.Hy),
    Hz(source.Hz),
    Hdelta(source.Hdelta),
    Ht(source.Ht)
  {

  }

  HitPoint::~HitPoint(){
  }

void HitPoint::SetDelta_and_T(const Event &ev, const Trajectory &traj, const double Rcil) {
    const double pc0 = traj.GetParC(0);
    const double pc1 = traj.GetParC(1);
    const double v0 = ev.GetVertix(0);
    const double v1 = ev.GetVertix(1);
    
    const double sum_sq = pc0*pc0 + pc1*pc1;
    const double prod = v0*pc0 + v1*pc1;
    
    Hdelta = prod*prod - 
             sum_sq*(v0*v0 + v1*v1 - Rcil*Rcil);
             
    Ht = (-prod + sqrt(Hdelta))/sum_sq;
    if(Ht < 0) {
        Ht = (-prod - sqrt(Hdelta))/sum_sq;
    }
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

//---------------------MULTIPLE SCATTERING-------------------------------------------------


void HitPoint::MSSetDelta_and_T(const HitPoint &h, const Trajectory &traj, const double Rcil) {
    const double pc0 = traj.GetParC(0);
    const double pc1 = traj.GetParC(1);
    const double h0 = h.GetX();
    const double h1 = h.GetY();
    
    const double sum_sq = pc0*pc0 + pc1*pc1;
    const double prod = h0*pc0 + h1*pc1;
    
    Hdelta = prod*prod - 
             sum_sq*(h0*h0 + h1*h1 - Rcil*Rcil);
             
    Ht = (-prod + sqrt(Hdelta))/sum_sq;
    if(Ht < 0) {
        Ht = (-prod - sqrt(Hdelta))/sum_sq;
    }
}

 void HitPoint::MSSetPoint(const HitPoint &h,const Trajectory &traj){
    Hx=h.GetX()+traj.GetParC(0)*Ht;
    Hy=h.GetY()+traj.GetParC(1)*Ht;
    Hz=h.GetZ()+traj.GetParC(2)*Ht;


 }