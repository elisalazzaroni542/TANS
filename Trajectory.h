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

     double RndmCustom();//reimplementato da Event
     
     void SetThetaNPhi();//estrae theta e phi di una traiettoria
     double GetTheta(){return Ttheta;};
     void GetPhi(){return Tphi;};
     void SetParC(int size=3);//inizializza i parametri c1,2,3 dell'equazione con le Theta e Phi estratte
     double GetParC(int i) const;//restituisce il parametro identificato dalla i
     double CalculateDelta(double Rcil);// calcola il discriminante dell'equazione. Rcil è il raggio del cilindro con cui si vuole calcolare l'intersezione
     double CalculateParT(double Rcil);//calcola il parametro t per trovare l'equazione della retta in 3D
     void PrintTrajectory() const;//stampa le informazioni relative a una traiettoria
     //double SetImpactCoord(double Rcil);//calcola le coordinate di impatto sul cilindro di raggio Rcil
   
   private:
     double Ttheta;//coord. theta estratta
     double Tphi;//coord. phi estratta
     double TparCSize;//dimensione del vettore parC
     double *TparC;//vettore che contiene i 3 parametri della retta c1,2,3

ClassDef (Trajectory,1)
};


#endif     
      

