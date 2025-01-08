#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TClonesArray.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"
#include "../headers/Point.h"



void sim(int seed = 123, unsigned const int events = 1000000, bool MS = true, bool smearing = true) {
    TStopwatch stopwatch;
    stopwatch.Start();

    TRandom3 rnd(seed);

    unsigned int m;
    std::vector<double> vertex(3);
    TClonesArray inHits("Point", 80);  
    TClonesArray outHits("Point", 80); 

    const int autoSaveSize = 100000;

    string filename = "../data/sim" + to_string(events) + ".root";
    TFile* file = TFile::Open(filename.c_str(), "RECREATE");

    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(autoSaveSize);

    tree->Branch("vertex", &vertex);
    tree->Branch("inHits", &inHits);
    tree->Branch("outHits", &outHits);
    tree->Branch("multiplicity", &m);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn, hOut;
    HitPoint hB; // used only for MS

    TH1F* thetaHist = t.LoadDistribution("heta2");
    
    unsigned int inCounter = 0;
    unsigned int outCounter = 0;

    // Variables for smearing
    double phiIn = 0.0;
    double phiOut = 0.0;    
    double zIn_sm = 0.0;
    double zOut_sm = 0.0;
    const double sigmaZ = 0.012;
    const double sigmaPhi = 0.003;
    const double noiseProb = 0.1; 
    const double maxNoisePoints = 3; // per layer 

    if(!smearing){
        const double sigmaZ = 0.00;
        const double sigmaPhi = 0.00;
    }

    for (unsigned int i = 0; i < events; ++i) {
        e.SetVertex();
        e.SetMultiplicity("custom");
        m = e.GetMultiplicity();
        vertex[0] = e.GetVertex(0);
        vertex[1] = e.GetVertex(1);
        vertex[2] = e.GetVertex(2);

        inHits.Clear();
        outHits.Clear();
        inCounter = 0;
        outCounter = 0;


        while (rnd.Uniform() < noiseProb && inCounter < maxNoisePoints) {
            double z = rnd.Uniform(-13.5, 13.5);
            double phi = rnd.Uniform(0, 2 * M_PI);
            double x = 4 * cos(phi);  // radius = 4 for inner layer
            double y = 4 * sin(phi);
            new (inHits[inCounter]) Point(x, y, z, i);
            ++inCounter;
        }

        // Outer layer noise
        while (rnd.Uniform() < noiseProb && outCounter < maxNoisePoints) {
            double z = rnd.Uniform(-13.5, 13.5);
            double phi = rnd.Uniform(0, 2 * M_PI);
            double x = 7 * cos(phi);  // radius = 7 for outer layer
            double y = 7 * sin(phi);
            new (outHits[outCounter]) Point(x, y, z, i);
            ++outCounter;
        }



        for (unsigned int j = 0; j < m; ++j) {
            t.SetThetaNPhi(thetaHist);
            t.SetParC();

            if (MS) {
                // Multiple scattering path
                hB.SetDelta_and_T(e, t, 3.1);
                hB.SetPoint(e, t);
                t.MSRotateParC(t);
                
                hIn.MSSetDelta_and_T(hB, t, 4.1);
                hIn.MSSetPoint(hB, t);
                t.MSRotateParC(t);

                hOut.MSSetDelta_and_T(hIn, t, 7.1);
                hOut.MSSetPoint(hIn, t);
            } else {
                // Direct path
                hIn.SetDelta_and_T(e, t, 4.1);
                hOut.SetDelta_and_T(e, t, 7.1);
                hIn.SetPoint(e, t);
                hOut.SetPoint(e, t);
            }

            // Common hit recording logic
            zIn_sm = hIn.GetZ() + rnd.Gaus(0, sigmaZ);
            zOut_sm = hOut.GetZ() + rnd.Gaus(0, sigmaZ);
            
            if (abs(zIn_sm) < 13.5) {
                phiIn = asin(hIn.GetY()/4) + rnd.Gaus(0, sigmaPhi);
                new (inHits[inCounter]) Point(4*cos(phiIn), 4*sin(phiIn), zIn_sm, i);
                ++inCounter;
                
                if (abs(zOut_sm) < 13.5) {
                    phiOut = asin(hOut.GetY()/7) + rnd.Gaus(0, sigmaPhi);
                    new (outHits[outCounter]) Point(7*cos(phiOut), 7*sin(phiOut), zOut_sm, i);
                    ++outCounter;
                }
            }
        }



        tree->Fill();
    }

    file->Write();
    file->Close();
    delete file;

    stopwatch.Stop();
    stopwatch.Print();
}
