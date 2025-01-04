#ifndef HITPOINT_H
#define HITPOINT_H


#include "Trajectory.h"

class HitPoint : public Trajectory{

  public:
    HitPoint();
    HitPoint(const Event &ev, const Trajectory &traj, const double Rcil);
    HitPoint(const HitPoint& source);
    virtual ~HitPoint();

    void SetPoint(const Event &ev,const Trajectory &traj); 
    double GetX()const{return Hx;}
    double GetY()const{return Hy;}
    double GetZ()const{return Hz;}
    double GetDelta()const{return Hdelta;}
    void SetDelta_and_T(const Event &ev, const Trajectory &traj, const double Rcil);
    double GetT()const{return Ht;}
    void PrintHit()const;

    //------------------------------MS-------------------------------------------------------------------------------------

    void MSSetPoint(const HitPoint &h,const Trajectory &traj); 
    void MSSetDelta_and_T(const HitPoint &h, const Trajectory &traj, const double Rcil);

  private:
    double Hx;
    double Hy;
    double Hz;
    double Hdelta;   // Delta of the intersection equation
    double Ht;       // T parameter 

ClassDef (HitPoint,1)
};


#endif     