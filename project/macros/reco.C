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
//    //cout<<fmod(phi, 2*M_PI)<<endl;    point1->GetY();    point1->GetY();    point1-->->->->->->->->->->->->->->->->>GetY();
//
//    return fmod(phi, 2*M_PI); // Returns the rest of the division
//}



void findAngle(double M1, double M2)
{
    // Store the tan value  of the angle
    double angle = abs((M2 - M1)
                       / (1 + M1 * M2));
 
    // Calculate tan inverse of the angle
    double ret = atan(angle);
 
    // Convert the angle from
    // radian to degree
    double val = (ret * 180) / M_PI;
 
    // Print the result
    // cout << val;
}




point get_vertex(Point* point1, Point* point2){

    unsigned int id = point2->GetId();
    
    double y1 point1->GetY();
    double z1 point1->GetZ();
    point1->GetY();
    return z1 - y1/(point2->GetY() - y1) * (poin2.GetZ() - z1);
    


}


double phiDiff(double phi1, double phi2) {
    double diff1 = TMath::Abs(phi1 - phi2);
    double diff2 = 2*TMath::Pi() - diff1;
    return TMath::Min(diff1, diff2);
}



void reco(unsigned int events=1000000){
    TStopwatch stopwatch;
    stopwatch.Start();

    string input = "../data/sim" +to_string(events)+ ".root";
    const char* inputFileName = input.c_str();

    string output = "../data/reco" + to_string(events) + ".root";
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

    TFile* outputFile = new TFile(outputFileName, "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        printf("Error: Cannot create output file '%s'\n", outputFileName);
        inputFile->Close();
        delete inputFile;
        return;
    }


    Point genVertex;
    Point recoVertex;
    TClonesArray inHits("Point", 80);  
    TClonesArray outHits("Point", 80); 
    unsigned int genMult;
    inputTree->SetBranchAddress("vertex", &genVertex);
    inputTree->SetBranchAddress("inHits", &inHits);
    inputTree->SetBranchAddress("outHits", &outHits);
    inputTree->SetBranchAddress("multiplicity", &genMult);

    TClonesArray* zReco = new TClonesArray("Point", 50);  // Around 25% of events lost due to eta, some more lost to smearing and noise


    TTree* outputTree = new TTree("Events", "Simulated and Reconstructed vertices");
    outputTree->Branch("genVertex", &genVertex);
    outputTree->Branch("recoVertex", &recoVertex);
    outputTree->Branch("genMultiplicity", &genMult);


    Point pIn, pOut, pVert

    unsigned int event_id = 0;

    for(event_id, event_id<events, ++event_id){

        inputTree->GetEntry();

        

    }
    stopwatch.Stop();
    stopwatch.Print();

}





