#ifndef HITPOINT_H
#define HITPOINT_H


#include "Trajectory.h"
#include "Event.h"

class HitPoint : public Trajectory{
  //classe che avrà il compito di generare l'HitPointo
  public:
    HitPoint(); //costruttore di default
    HitPoint(const Event &ev, const Trajectory &traj, const double Rcil); //costruttore standard
    HitPoint(const HitPoint& source); //copy constructor 
    virtual ~HitPoint();//distruttore di default

    void SetPoint(const Event &ev,const Trajectory &traj);//crea le coordinate dei punti di impatto 
    double GetX()const{return Hx;}
    double GetY()const{return Hy;}
    double GetZ()const{return Hz;}
    double GetDelta()const{return Hdelta;}
    void SetDelta_and_T(const Event &ev, const Trajectory &traj, const double Rcil);
    void SetDelta(const Event &ev, const Trajectory &traj, const double Rcil);
    void SetT(const Event &ev, const Trajectory &traj);//calcola il parametro t dell'equazione parametrica
    double GetT()const{return Ht;}
    void PrintHit()const;//stampa le info sul punto di impatto

    //------------------------------MS-------------------------------------------------------------------------------------

    void MSSetPoint(const HitPoint &h,const Trajectory &traj);//crea le coordinate dei punti di impatto 
    void MSSetDelta_and_T(const HitPoint &h, const Trajectory &traj, const double Rcil);

  private:
    double Hx;
    double Hy;
    double Hz;
    double Hdelta; //discriminante dell'equazione
    double Ht;//parametro t dell'equazione

ClassDef (HitPoint,1)
};


#endif     