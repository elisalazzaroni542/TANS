#include <TFile.h>
#include <TStopwatch.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TClonesArray.h>
#include <vector>
#include <cmath>
#include <TMath.h>
#include <Riostream.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include "../headers/Point.h"



void analysis(unsigned int events=1000000){
    TStopwatch stopwatch;
    stopwatch.Start();

    string input = "../data/reco" + to_string(events) + ".root";
    const char* inputFileName = input.c_str();

    string output = "../data/analysis" + to_string(events) + ".root";
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
    unsigned int genMult;
    
    inputTree->SetBranchAddress("genVertex", &genVertex);
    inputTree->SetBranchAddress("recoVertex", &recoVertex);
    inputTree->SetBranchAddress("genMultiplicity", &genMult);
    
    TH1D* histRes = new TH1D("Residuals", "Z Residuals;Z Difference (reco - gen) [mm];Entries", 100, -0.5, 0.5);
    histRes->Sumw2();    
   // TTree* outputTree = new TTree("Plots", "Performace plots");
    //outputTree->Branch("histRes", &histRes);
    //outputTree->Branch("effVSmult", &effVSmult);
    //outputTree->Branch("resVSmult", &resVSmult);

    Point *pTrue, *pRec;
    
     // Process entries in the input tree
    for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
        inputTree->GetEntry(i);
        // Check if pointers are valid
     if (!genVertex || !recoVertex) continue;
       
         // Calculate the difference between reco and gen Z coordinates
         double zGen = genVertex->GetZ();
         double zReco = recoVertex->GetZ();
         double zDiff = zReco - zGen;
         
         // Fill the histogram
         histRes->Fill(zDiff);
       
    }
    // Customize marker style, size, and color
    histRes->SetMarkerStyle(20);  // Full circle
    histRes->SetMarkerSize(0.1);   // Bigger markers
    histRes->SetMarkerColor(kRed); // Red markers
     
    
    TCanvas* canvas = new TCanvas("canvas", "Z Residuals", 800, 600);
    histRes->Draw("E1 P");
    canvas->SaveAs("../plots/ZResiduals.png");
    
     

    outputFile->cd();
    outputFile->Close();
    inputFile->Close();

    delete genVertex;
    delete recoVertex;
    
    
    stopwatch.Stop();
    stopwatch.Print();
}



/*
void reco(unsigned int events=1000000, double phiCut=0.01){
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
    Point* recoVertex = new Point();
    TClonesArray* inHits = new TClonesArray("Point", 80);
    TClonesArray* outHits = new TClonesArray("Point", 80);
    unsigned int genMult;
    inputTree->SetBranchAddress("vertex", &genVertex);
    inputTree->SetBranchAddress("inHits", &inHits);
    inputTree->SetBranchAddress("outHits", &outHits);
    inputTree->SetBranchAddress("multiplicity", &genMult);

    TClonesArray* zReco = new TClonesArray("Point", 50);  // Around 25% of events lost due to eta, some more lost to smearing


    TTree* outputTree = new TTree("Events", "Simulated and Reconstructed vertices");
    outputTree->Branch("genVertex", &genVertex);
    outputTree->Branch("recoVertex", &recoVertex);
    outputTree->Branch("genMultiplicity", &genMult);


    Point *pIn, *pOut, *pVert;

    unsigned int event_id = 0;
    unsigned int ith_entry = 0;
    double angleIn;
    double angleOut;
    vector<double> zCollection;
    zCollection.reserve(50);
    cout<<inputTree->GetEntriesFast()<<endl;
    for(int i=0; i<inputTree->GetEntriesFast(); ++i){

        inputTree->GetEntry(i);

        for(int j=0; j<outHits->GetEntriesFast(); ++j){
            zCollection.clear();
            pOut = (Point*)outHits->At(j);
            angleOut = findAngle(pOut->GetX(), pOut->GetY());            
            for(int k=0; k<inHits->GetEntriesFast(); ++k){
                pIn = (Point*)inHits->At(k);

                if (abs(angleOut - findAngle(pIn->GetX(), pIn->GetY())) < phiCut){
                    zCollection.push_back(recoZ(pIn, pOut));
                }


            }

        }


    }
    delete genVertex;
    delete recoVertex;
    delete inHits;
    delete outHits;
    stopwatch.Stop();
    stopwatch.Print();

}


*/


