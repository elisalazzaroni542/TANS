#ifndef RNDMPICK_H
#define RNDMPICK_H

#include "TRandom3.h"

class RndmPick : public TRandom3{
  //classe che avrà il compito di generare tutti i numeri casuali.
  public:
    RndmPick(); //costruttore di default
    RndmPick(unsigned int seed); //costruttore standard
    RndmPick(const RndmPick& source); //copy constructor (necessario perchè un data member (vertix) alloca memoria)
    virtual ~RndmPick();//distruttore di default

    void ChangeSeed(unsigned int newseed){Rseed=newseed};

    //funzioni che estraggono secondo diverse distribuzioni//

    double RndmGaus(double mean, double sigma); //funzione che estrae un numero a caso da una distribuzione gaussiana

    //funzioni di manipolazione risultati//

    double CheckRndm (int numcoord); //funzione che controlla che il numero estratto rispetti delle condizioni
    void PrintCoordinates () const; //funzione che stampa le coordinate del punto estratto
    void SetVertix (int size);
    double GetVertix(int i) const;

  private:
    unsigned int Rseed;
    int RvertixSize;//dimensione del vettore vertix
    double *Rvertix;//vettore con le coordinate del vertice

ClassDef (RndmPick,1)
};


#endif






