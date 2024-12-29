#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "Event.h"
//#include <TRandom3.h>

class Trajectory : public Event{
  //classe che avrà il compito di generare le traiettorie
   public:
     Trajectory(); //costruttore di default
     Trajectory(unsigned int seed); //costruttore standard
     Trajectory(const Trajectory& source); //copy constructor (necessario perchè un data member (vertix) alloca memoria)
     virtual ~Trajectory();//distruttore di default

     //double RndmCustom();//reimplementato da Event
     
     void SetThetaNPhi(TH1F* customHist);//estrae theta e phi di una traiettoria
     float GetTheta(){return Ttheta;};
     float GetPhi(){return Tphi;};
     void SetParC(const unsigned int size=3);//inizializza i parametri c1,2,3 dell'equazione con le Theta e Phi estratte
     double GetParC(const unsigned int i) const;//restituisce il parametro identificato dalla i
     void PrintTrajectory() const;//stampa le informazioni relative a una traiettoria
     //double SetImpactCoord(double Rcil);//calcola le coordinate di impatto sul cilindro di raggio Rcil
   
   private:
     float Ttheta;//coord. theta estratta
     float Tphi;//coord. phi estratta
     //int TparCSize;//dimensione del vettore parC
     vector<double> TparC;//vettore che contiene i 3 parametri della retta c1,2,3

ClassDef (Trajectory,1)
};


#endif     
      

