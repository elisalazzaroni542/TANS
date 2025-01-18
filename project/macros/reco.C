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

double runningWindow(const vector<double>& zCollection, double window_size = 0.5) {
    double z_min = -13.5;
    double z_max = 13.5;
    unsigned maxCount = 0;
    double bestWindowStart = 0.0;
    
    for (double windowStart = z_min; windowStart <= z_max - window_size; windowStart += window_size) {
        double windowEnd = windowStart + window_size;
        unsigned int count = 0;
        for (double z : zCollection) {
            if ((z >= windowStart && z < windowEnd)) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            bestWindowStart = windowStart;
        }
    }
    
    double sum = 0.0;
    unsigned int count = 0;
    for (double z : zCollection) {
        if (z >= bestWindowStart && z < bestWindowStart + window_size) {
            sum += z;
            count++;
        }
    }
    
    return count > 0 ? sum / count : bestWindowStart + window_size/2;
}




double recoZ(Point* point1, Point* point2){   



    double x1 = point1->GetX();
    double y1 = point1->GetY();
    double z1 = point1->GetZ();

    double tx = - x1/(point2->GetX() - x1);
    double ty = - y1/(point2->GetY() - y1);

    double t = (tx + ty)/2;

    return z1 + t * (point2->GetZ() - z1);


/*
    double y1 = point1->GetX();
    double z1 = point1->GetZ();

    
    return z1 - y1/(point2->GetX() - y1) * (point2->GetZ() - z1);
*/
}


double phiDiff(double phi1, double phi2) {
    double diff1 = TMath::Abs(phi1 - phi2);
    double diff2 = 2*TMath::Pi() - diff1;
    return TMath::Min(diff1, diff2);
}



void reco(unsigned int events=1000000, double phiCut=0.01, double windowSize=0.2, bool noise=true){
    TStopwatch stopwatch;
    stopwatch.Start();
    cout<<""<<endl;
    cout<<"Done, running reconstruction..."<<endl;

    string input;
    const char* inputFileName;

    if (noise){
        input = "../data/sim" +to_string(events)+ "_noise.root";
        inputFileName = input.c_str();
    } else {
        input = "../data/sim" +to_string(events)+ ".root";
        inputFileName = input.c_str();
    }

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
    Point* recoVertex = new Point();
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
    unsigned int maxHits = 0;
    vector<double> zCollection;
    zCollection.reserve(80);

    for(int i=0; i < inputTree->GetEntriesFast(); ++i){
        inputTree->GetEntry(i);
        zCollection.clear();

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

        if (!zCollection.empty()) {
            maxHits = zCollection.size() > maxHits ? zCollection.size() : maxHits;
            double vertexZ = runningWindow(zCollection, windowSize);
            recoVertex->Set(0., 0., vertexZ, i);
        } else {
            recoVertex->Set(0., 0., -20, -1); // No vertex reconstructed
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
    cout<<"Max reco vertices: "<<maxHits<<endl;
    cout<<"Done."<<endl;
    stopwatch.Stop();
    stopwatch.Print();
}

