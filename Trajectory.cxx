#include <Riostream.h>
#include "Trajectory.h"
#include "Event.h"
#include <TRandom3.h>
#include <TFile.h>




ClassImp (Trajectory)

  Trajectory::Trajectory(): Event(),
  TparT(0),
  Ttheta(0),
  Tphi(0)
  {
    //default constructor
      //cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  Trajectory::Trajectory(unsigned int seed):Event(seed),
  TparT(0),
  Ttheta(0),
  Tphi(0)
  {
    //standard constructor
     //cout<<"std constr-this= "<<this<<endl;
  }
  
  Trajectory::Trajectory (const Trajectory &source):
   Event(source),
   TparT(source.TparT),
   Ttheta(source.Ttheta),
   Tphi(source.Tphi)
  {//copy constructor
    //cout<<"copy constructor-this= "<<this<<endl;
   
  }

  

  Trajectory::~Trajectory(){
    //default destructor
    //cout<<"destructor-this= "<<this<<endl;
    
  }