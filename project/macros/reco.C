


#include <TFile.h>
#include <TStopwatch.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TClonesArray.h>
#include <vector>
#include <cmath>
#include <TMath.h>
#include <Riostream.h>
#include "../headers/Point.h"


double runningWindow(const vector<double>& zCollection, double window_size = 0.7) {
    
    double z_min = -13.5;
    double z_max = 13.5;
    
    size_t maxCount = 0;
    double bestZ = 0.0;
    
    // Slide window through the range with step size = window_size
    for (double windowStart = z_min; windowStart <= z_max - window_size; windowStart += window_size) {
        double windowEnd = windowStart + window_size;
        
        // Count points in current window
        size_t count = 0;
        for (double z : zCollection) {
            if (z >= windowStart && z < windowEnd) {
                count++;
            }
        }
        
        // Update if this window has more points
        if (count > maxCount) {
            maxCount = count;
            bestZ = windowStart + window_size/2; // Center of the window
        }
    }
    
    return bestZ;
}
double findAngle(double x, double y) {
    double dotProduct = x;
    double magnitudeProduct = TMath::Sqrt(x*x + y*y);
    
    double cosTheta = dotProduct / magnitudeProduct;
    cosTheta = TMath::Max(-1.0, TMath::Min(1.0, cosTheta));
    
    double theta = TMath::ACos(cosTheta);
    if (y < 0) {
        theta = 2 * M_PI - theta;
    }
    return theta;
}





double recoZ(Point* point1, Point* point2){

    
    double y1 = point1->GetY();
    double z1 = point1->GetZ();
    point1->GetY();
    return z1 - y1/(point2->GetY() - y1) * (point2->GetZ() - z1);
    


}


double phiDiff(double phi1, double phi2) {
    double diff1 = TMath::Abs(phi1 - phi2);
    double diff2 = 2*TMath::Pi() - diff1;
    return TMath::Min(diff1, diff2);
}



void reco(unsigned int events=1000000, double phiCut=0.01, double windowSize=0.2){
    TStopwatch stopwatch;
    stopwatch.Start();

    string input = "../data/sim" +to_string(events)+ "_noise.root";
    const char* inputFileName = input.c_str();

    string output = "../data/reco" + to_string(events) + ".root";
    const char* outputFileName = output.c_str();

    TFile* inputFile = new TFile(inputFileName, "READ");
    if (!inputFile || inputFile->IsZombie()) {
        printf("Error: Cannot open input file '%s'\n", inputFileName);
        return;
    }

    const char* treeName = "Events";
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

    Point* genVertex = new Point();
    Point* recoVertex = new Point(0., 0., 0., 0); // Initialize with zeros
    TClonesArray* inHits = new TClonesArray("Point", 80);
    TClonesArray* outHits = new TClonesArray("Point", 80);
    unsigned int genMult;
    
    inputTree->SetBranchAddress("vertex", &genVertex);
    inputTree->SetBranchAddress("inHits", &inHits);
    inputTree->SetBranchAddress("outHits", &outHits);
    inputTree->SetBranchAddress("multiplicity", &genMult);

    TTree* outputTree = new TTree("Events", "Simulated and Reconstructed vertices");
    outputTree->Branch("genVertex", &genVertex);
    outputTree->Branch("recoVertex", &recoVertex);
    outputTree->Branch("genMultiplicity", &genMult);
    outputTree->SetAutoSave(0);


    Point *pIn, *pOut;
    vector<double> zCollection;
    zCollection.reserve(50);

    for(int i=0; i < inputTree->GetEntriesFast(); ++i){
        inputTree->GetEntry(i);
        zCollection.clear();

        // Collect z positions from matching hits
        for(int j=0; j < outHits->GetEntriesFast(); ++j){
            pOut = (Point*)outHits->At(j);
            double angleOut = pOut->GetPhi();            
            
            for(int k=0; k < inHits->GetEntriesFast(); ++k){
                pIn = (Point*)inHits->At(k);
                if (abs(angleOut - pIn->GetPhi()) < phiCut){
                    zCollection.push_back(recoZ(pIn, pOut));
                }
            }
        }

        // Find vertex using running window
        if (!zCollection.empty()) {
            double vertexZ = runningWindow(zCollection, windowSize);
            recoVertex->Set(0., 0., vertexZ, i);
        } else {
            recoVertex->Set(0., 0., -20, -1); // No vertex found
        }
        outputTree->Fill();

    }

    outputFile->cd();
    outputTree->Write();
    outputFile->Close();
    inputFile->Close();

    delete genVertex;
    delete recoVertex;
    delete inHits;
    delete outHits;
    
    stopwatch.Stop();
    stopwatch.Print();
}

