#include <TFile.h>
#include <TStopwatch.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TClonesArray.h>
#include <vector>
#include <cmath>
#include <TMath.h>

#include "../headers/Point.h"


double wrap_phi(double phi){

    //cout<<phi<<endl;
    //cout<<fmod(phi, 2*M_PI)<<endl;

    return fmod(phi, 2*M_PI); // Returns the rest of the division
}

point get_vertex(Point* pointIn, Point* PointOut){

    unsigned int id = pointIn->GetId();

    


}