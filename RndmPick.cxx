#include <Riostream.h>
#include "RndmPick.h"
#include <TRandom3.h>

ClassImp (RndmPick)

  RndmPick::RndmPick(): TRandom3(),
  seed(123.),
  vertixSize(0.),
  vertix(NULL)
  {
    //default constructor
      cout<<"DEFAULT CONSTR-THIS= "<<this<<endl;
  }

  RndmPick::RndmPick(unsigned int seed):TRandom3(),
  Rseed(seed),
  RvertixSize(0),
  Rvertix(NULL)
  {
    //standard constructor
     cout<<"std constr-this= "<<this<<endl;
  }
