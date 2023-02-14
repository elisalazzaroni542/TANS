#ifndef EVENT_H
#define EVENT_H


#include <TRandom3.h>

class Event : public TRandom3{
  //classe che avrà il compito di generare l'evento
  public:
    Event(); //costruttore di default
    Event(usigned int seed); //costruttore standard
    Event(const Event& source); //copy constructor (necessario perchè un data member (vertix) alloca memoria)
    virtual ~Event();//distruttore di default

    void ChangeSeed(unsigned int newseed){Rseed=newseed;};

    //funzioni che estraggono secondo diverse distribuzioni//

    double RndmGaus(double mean, double sigma); //funzione che estrae un numero a caso da una distribuzione gaussiana
    double RndmUni(double min, double max);//funzione che estrae un numero secondo una distribuzione uniforme tra min e max
    //double RndmCustom(string NameFile );//funzione che estrae un numero da una distribuzione qualsiasi selezionata dall'utente tramite importazione di un istogramma

    //funzioni di manipolazione risultati//

    double PickNCheckRndm (int numcoord); //funzione che estrae e controlla che il numero estratto rispetti delle condizioni
    void SetVertix (int size);
    double GetVertix(int i) const;
    void CheckMultiplicity (); //funzione che controlla che la molteplicità sia accettabile
    void PrintEvent () const; //funzione che stampa le coordinate del vertice e la sua molteplicità
    void SetMultiplicity ();//funzione che inizializza la molteplicità dell'evento
    //double GetEvent(RndmPick vertix, double mult) const;

  private:
    unsigned int Eseed;
    int EvertixSize;//dimensione del vettore vertix
    double *Evertix;//vettore con le coordinate del vertice
    double Emult;//molteplicità dell'impatto
   

ClassDef (Event,1)
};


#endif
