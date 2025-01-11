#ifndef HITPOINT_H
#define HITPOINT_H


#include "Trajectory.h"

class HitPoint : public Trajectory{

  public:
    HitPoint();
    HitPoint(bool smearing);
    HitPoint(const Event &ev, const Trajectory &traj, const double rCil, bool smearing);
    HitPoint(const HitPoint& source);
    virtual ~HitPoint();

    void SetPoint(const Event &ev,const Trajectory &traj); 
    void SetPoint(const Event &ev,const Trajectory &traj, double rCil);

    double GetX()const{return Hx;}
    double GetY()const{return Hy;}
    double GetZ()const{return Hz;}
    double GetPhi()const{return Hphi;}
    double GetDelta()const{return Hdelta;}
    void SetDelta_and_T(const Event &ev, const Trajectory &traj, const double rCil);
    double GetT()const{return Ht;}
    void PrintHit()const;

    //------------------------------MS-------------------------------------------------------------------------------------

//    void SetPoint(const HitPoint &h,const Trajectory &traj); 
    void SetPoint(const HitPoint &h,const Trajectory &traj, double rCil);

    void SetDelta_and_T(const HitPoint &h, const Trajectory &traj, const double rCil);

  private:
    double Hx;
    double Hy;
    double Hz;
    double Hphi;
    const int Hsmearing; // 1 if smearing, 0 if not
    double Hdelta;   // Delta of the intersection equation
    double Ht;       // T parameter 

ClassDef (HitPoint,1)
};


#endif     