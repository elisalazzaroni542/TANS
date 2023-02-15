#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "Event.h"
#include <TRandom3.h>

class Trajectory : public Event{
  //classe che avrà il compito di generare le traiettorie
   public:
     Trajectory(); //costruttore di default
     Trajectory(unsigned int seed); //costruttore standard
     Trajectory(const Trajectory& source); //copy constructor (necessario perchè un data member (vertix) alloca memoria)
     virtual ~Trajectory();//distruttore di default

     double SetThetaNPhi();//estrae theta e phi di una traiettoria
     double GetTheta(){return Ttheta;};
     double GetPhi(){return Tphi;};
     double SetParC(double Theta, double Phi);//inizializza i parametri c1,2,3 dell'equazione con le Theta e Phi estratte
     double CalculateDelta(double *coord, double *parc, double Rcil);// calcola il discriminante dell'equazione. Rcil è il raggio del cilindro con cui si vuole calcolare l'intersezione
     double CalculateParT(double *coord, double *parc, double Delta);//calcola il parametro t per trovare l'equazione della retta in 3D
     double SetImpactCoord(double Rcil);//calcola le coordinate di impatto sul cilindro di raggio Rcil
   
   private:
     double TparT;//parametro t dell'equazione per la retta
     double Ttheta;//coord. theta estratta
     double Tphi;//coord. phi estratta


ClassDef (Trajectory,1)
};


#endif     
      

