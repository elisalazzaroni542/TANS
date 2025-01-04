#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "Event.h"

class Trajectory : public Event{

   public:
    Trajectory(); 
    Trajectory(unsigned int seed);
    Trajectory(const Trajectory& source); 
    virtual ~Trajectory();
     
    void SetThetaNPhi(TH1F* customHist);
    float GetTheta(){return Ttheta;};
    float GetPhi(){return Tphi;};
    void SetParC();
    double GetParC(const unsigned int i) const;
    void PrintTrajectory() const;
     
    void MSRotateParC(Trajectory &traj);
   
   private:
     float Ttheta;
     float Tphi;
     vector<double> TparC;

ClassDef (Trajectory,1)
};


#endif     
      

