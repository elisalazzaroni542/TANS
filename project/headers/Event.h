#ifndef EVENT_H
#define EVENT_H

#include <Riostream.h>
#include <TRandom3.h>
#include <TH1F.h>
#include <TFile.h>
#include <string>



class Event : public TRandom3{

  public:
    Event(); 
    Event(unsigned int seed); 
    Event(const Event& source); 
    virtual ~Event();

    void ChangeSeed(unsigned int newseed){Eseed=newseed;};

    // Function to extract random numbers based on different distributions //

    static TH1F* LoadDistribution(const char* histName);

    double RndmGaus(double mean, double sigma); 
    double RndmUni(double min, double max);
    double RndmCustom(TH1F* customHist);

    // Function to manipulate the extracted numbers //

    double PickNCheckVertRndm (unsigned int numcoord); 
    void SetVertex ();
    double GetVertex(unsigned int i) const;
    unsigned int GetMultiplicity() const{return Emult;} 
    void PrintEvent () const; 
    void SetMultiplicity (const string& distr);

    
  private:
    unsigned int Eseed;
    vector<double> Evertex;
    int Emult;
   

ClassDef (Event,1)
};


#endif
