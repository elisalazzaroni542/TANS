#include "HitPoint.h"




ClassImp (HitPoint)

  HitPoint::HitPoint(): Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hphi(0),
  Hsmearing(1),
  Hdelta(0),
  Ht(0)
  {
  }

  HitPoint::HitPoint(bool smearing): Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hphi(0),
  Hsmearing(smearing ? 1 : 0),
  Hdelta(0),
  Ht(0)
  {
  }



  HitPoint::HitPoint(const Event &ev,const Trajectory &traj,const double rCil, bool smearing):Trajectory(),
  Hx(0),
  Hy(0),
  Hz(0),
  Hphi(0),
  Hdelta(0),
  Hsmearing(smearing ? 1 : 0),
  Ht(0)
  {
    SetDelta_and_T(ev, traj, rCil);

  }
  
  HitPoint::HitPoint (const HitPoint &source):
    Trajectory(source),
    Hx(source.Hx),
    Hy(source.Hy),
    Hz(source.Hz),
    Hphi(source.Hphi),    
    Hsmearing(source.Hsmearing),
    Hdelta(source.Hdelta),
    Ht(source.Ht)
  {

  }

  HitPoint::~HitPoint(){
  }

void HitPoint::SetDelta_and_T(const Event &ev, const Trajectory &traj, const double rCil) {
    const double pc0 = traj.GetParC(0);
    const double pc1 = traj.GetParC(1);
    const double v0 = ev.GetVertex(0);
    const double v1 = ev.GetVertex(1);
    
    const double sum_sq = pc0*pc0 + pc1*pc1;
    const double prod = v0*pc0 + v1*pc1;
    
    Hdelta = prod*prod - 
             sum_sq*(v0*v0 + v1*v1 - rCil*rCil);
             
    Ht = (-prod + sqrt(Hdelta))/sum_sq;
    if(Ht < 0) {
        Ht = (-prod - sqrt(Hdelta))/sum_sq;
    }
}

 void HitPoint::PrintHit()const {
  
    cout<<"Delta: "<<Hdelta<<endl;
    cout<<"t: "<<Ht<<endl;
    cout<<"Hit point coordinates: "<<Hx<<", "<<Hy<<", "<<Hz<<endl;
 }
 

void HitPoint::SetPoint(const Event &ev, const Trajectory &traj, double rCil) {
    
    double smear = Hsmearing * RndmGaus(0, 0.003);
    double newPhi = smear/rCil + traj.GetPhi();
    int notSmearing = 1 - Hsmearing;

    Hx = notSmearing * (ev.GetVertex(0) + traj.GetParC(0) * Ht) + Hsmearing * rCil * cos(newPhi);
    Hy = notSmearing * (ev.GetVertex(1) + traj.GetParC(1) * Ht) + Hsmearing * rCil * sin(newPhi);
    Hz = ev.GetVertex(2) + traj.GetParC(2) * Ht + Hsmearing * RndmGaus(0, 0.012);
    Hphi = newPhi;
}



void HitPoint::SetPoint(const Event &ev, const Trajectory &traj) { // Used for the beam pipe hit

    Hx = ev.GetVertex(0) + traj.GetParC(0) * Ht;
    Hy = ev.GetVertex(1) + traj.GetParC(1) * Ht;
    Hz = ev.GetVertex(2) + traj.GetParC(2) * Ht;

}

//---------------------MULTIPLE SCATTERING-------------------------------------------------


void HitPoint::SetDelta_and_T(const HitPoint &h, const Trajectory &traj, const double rCil) {
    const double pc0 = traj.GetParC(0);
    const double pc1 = traj.GetParC(1);
    const double h0 = h.GetX();
    const double h1 = h.GetY();
    
    const double sum_sq = pc0*pc0 + pc1*pc1;
    const double prod = h0*pc0 + h1*pc1;
    
    Hdelta = prod*prod - 
             sum_sq*(h0*h0 + h1*h1 - rCil*rCil);
             
    Ht = (-prod + sqrt(Hdelta))/sum_sq;
    if(Ht < 0) {
        Ht = (-prod - sqrt(Hdelta))/sum_sq;
    }
}

void HitPoint::SetPoint(const HitPoint &h, const Trajectory &traj, const double rCil) {

    double smear = Hsmearing * RndmGaus(0, 0.003);
    double newPhi = smear/rCil + traj.GetPhi();
    int notSmearing = 1 - Hsmearing;

    Hx = notSmearing * (h.GetX() + traj.GetParC(0) * Ht) + Hsmearing * rCil * cos(newPhi);
    Hy = notSmearing * (h.GetY() + traj.GetParC(1) * Ht) + Hsmearing * rCil * sin(newPhi);
    Hz = h.GetZ() + traj.GetParC(2) * Ht + Hsmearing * RndmGaus(0, 0.012);
    Hphi = newPhi;

}

