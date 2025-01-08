#include <TFile.h>
#include <TStopwatch.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TClonesArray.h>
#include <vector>
#include <cmath>
#include <TMath.h>
#include "../headers/Point.h"


//double wrap_phi(double phi){
//
//    //cout<<phi<<endl;
//    //cout<<fmod(phi, 2*M_PI)<<endl;
//
//    return fmod(phi, 2*M_PI); // Returns the rest of the division
//}

point get_vertex(Point* pointIn, Point* PointOut){

    unsigned int id = pointIn->GetId();

    


}


double phiDiff(double phi1, double phi2) {
    double diff1 = TMath::Abs(phi1 - phi2);
    double diff2 = 2*TMath::Pi() - diff1;
    return TMath::Min(diff1, diff2);
}



void reco( string fInName = "sim1000000_noise.root", string fOutName = "sim1000000_reco.root"){
    TStopwatch stopwatch;
    stopwatch.Start();

    string input = "../data/" + fInName;
    const char* inputFileName = input.c_str();

    string output = "../data/" + fOutName;
    const char* outputFileName = output.c_str();


    TFile* inputFile = new TFile(inputFileName, "READ");
    if (!inputFile || inputFile->IsZombie()) {
        printf("Error: Cannot open input file '%s'\n", inputFileName);
        return;
    }

    TTree* inputTree = (TTree*)inputFile->Get(treeName);
    if (!inputTree) {
        printf("Error: TTree '%s' not found in file '%s'\n", treeName, inputFileName);
        inputFile->Close();
        delete inputFile;
        return;
    }

    TClonesArray* inHits = new TClonesArray("Point", 25000000);  // Initialize with class name
    TClonesArray* outHits = new TClonesArray("Point", 25000000); // Initialize with class name

    inputTree->SetBranchAddress("inHits", &inHits);
    inputTree->SetBranchAddress("outHits", &outHits);



    TClonesArray* genVertex = new TClonesArray("Point", 1000000);  // Initialize with class name
    TClonesArray* recoVertex = new TClonesArray("Point", 1000000);  // Initialize with class name


    TTree* outputTree = new TTree("Events", "Simulated and Reconstructed vertices");




    unsigned int event_id = 0;



}




