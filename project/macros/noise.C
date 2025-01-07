#include <TFile.h>
#include <TStopwatch.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TClonesArray.h>
#include <vector>
#include <cmath>
#include "../headers/Point.h"

void apply_smearing(TRandom3& rnd, TClonesArray* hits, double sigmaPhi, double sigmaZ, unsigned int rCil) {
    const unsigned int nRand = 5; // Number of random points to add
    int origSize = hits->GetEntriesFast();
    unsigned int id=-1;
    // First smear existing hits in place
    for (int i = 0; i < origSize; ++i) {
        Point* point = (Point*)hits->At(i);
        //if (!point) continue;
        
        double x = point->GetX();
        double y = point->GetY();
        double z = point->GetZ();
        double phi = point->GetPhi(rCil);
        id = point-> GetId();
        double x_sm, y_sm, z_sm, phi_sm;
        double new_radius2;
        

        phi_sm = phi + rnd.Gaus(0, sigmaPhi);
        x_sm = rCil*cos(phi_sm);
        y_sm = rCil*sin(phi_sm);

        /*
        do {

            x_sm = x + rnd.Gaus(0, sigmaXY);
            y_sm = y + rnd.Gaus(0, sigmaXY);
            new_radius2 = x_sm * x_sm + y_sm * y_sm;
        } while (new_radius2 > (rCil + 0.2) * (rCil + 0.2) || new_radius2 < rCil * rCil);
        */

        do {
            z_sm = z + rnd.Gaus(0, sigmaZ);
        } while (abs(z_sm) > 13.5);
        
        point->Set(x_sm, y_sm, z_sm, id);
    }
    
    // Then add random points
    for (unsigned int i = 0; i < nRand; ++i) {  // Fixed sign comparison warning
        double phi = rnd.Uniform(0, 2 * M_PI);
        double x = rCil * cos(phi);
        double y = rCil * sin(phi);
        double z = rnd.Uniform(-13.5, 13.5);
        new((*hits)[hits->GetEntriesFast()]) Point(x, y, z, id);
    }
}

void noise(unsigned int seed=123, const char* inputFileName = "../data/sim1000000.root", const char* outputFileName = "../data/sim1000000_smearing.root") {
    TStopwatch stopwatch;
    stopwatch.Start();

//    const char* inputFileName = "../data/sim1000000.root";
//    const char* outputFileName = "../data/sim1000000_smearing.root";
    const char* treeName = "Events";

    // Open input file and get tree
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

    // Input variables
    vector<double>* vertex = nullptr;
    TClonesArray* inHits = new TClonesArray("Point", 25000000);  // Initialize with class name
    TClonesArray* outHits = new TClonesArray("Point", 25000000); // Initialize with class name
    UInt_t multiplicity;

    // Set branch addresses
    inputTree->SetBranchAddress("vertex", &vertex);
    inputTree->SetBranchAddress("inHits", &inHits);
    inputTree->SetBranchAddress("outHits", &outHits);
    inputTree->SetBranchAddress("multiplicity", &multiplicity);

    // Create output file and tree
    TFile* outputFile = new TFile(outputFileName, "RECREATE");
    TTree* outputTree = new TTree(treeName, "Tree with smeared hits");

    // Output variables
    std::vector<double> vertex_out;
    UInt_t multiplicity_out;

    // Create branches - specify the class name in the branch creation
    outputTree->Branch("vertex", &vertex_out);
    outputTree->Branch("inHits", &inHits, 32000, 1);  // Added splitting level
    outputTree->Branch("outHits", &outHits, 32000, 1); // Added splitting level
    outputTree->Branch("multiplicity", &multiplicity_out);

    // Random number generator
    TRandom3 rnd(seed);

    // Variables to track total hits
    Long64_t totalInHitsBefore = 0;
    Long64_t totalInHitsAfter = 0;

    // Process events
    Long64_t nEntries = inputTree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        if (i % 100000 == 0) {
            printf("Processing event %lld/%lld\n", i, nEntries);
        }
        
        inputTree->GetEntry(i);

        totalInHitsBefore += inHits->GetEntriesFast();

        // Copy vertex and multiplicity
        vertex_out = *vertex;
        multiplicity_out = multiplicity;

        // Apply smearing
        apply_smearing(rnd, inHits, 0.003, 0.012, 4);
        apply_smearing(rnd, outHits, 0.003, 0.012, 7);

        totalInHitsAfter += inHits->GetEntriesFast();

        outputTree->Fill();
    }

    // Print statistics
    printf("\nFinal Statistics:\n");
    printf("Total inner hits before smearing: %lld\n", totalInHitsBefore);
    printf("Total inner hits after smearing and noise: %lld\n", totalInHitsAfter);
    printf("Ratio: %.2f\n", double(totalInHitsAfter)/totalInHitsBefore);

    // Write and close
    outputTree->Write();
    outputFile->Close();
    inputFile->Close();

    // Cleanup
    delete inHits;
    delete outHits;
    delete outputFile;
    delete inputFile;

    stopwatch.Stop();
    stopwatch.Print();
}