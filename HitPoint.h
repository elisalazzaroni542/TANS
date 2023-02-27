#ifndef HITPOINT_H
#define HITPOINT_H


#include "Trajectory.h"

class HitPoint : public Trajectory(){
  //classe che avrà il compito di generare l'HitPointo
  public:
    HitPoint(); //costruttore di default
    HitPoint(double x,double y,double z,const Event &ev,const Trajectory &traj,double Rcil); //costruttore standard
    HitPoint(const HitPoint& source); //copy constructor 
    virtual ~HitPoint();//distruttore di default

    double GetX()const{return Hx;}
    double GetY()const{return Hy;}
    double GetZ()const{return Hz;}
    double GetDelta()const{return Hdelta;}
    void SetT();
    double GetT()const{return Ht;}

  private:
    double Hx;
    double Hy;
    double Hz;
    double Hdelta; 
    double Ht;

ClassDef (HitPoint,1)
};


#endif     


