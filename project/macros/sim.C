#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TClonesArray.h>
#include <TMath.h>
#include "../headers/Event.h"
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"
#include "../headers/myPoint.h"





void sim(int seed = 123, unsigned const int events = 1000000, bool MS = true, bool noise = true, string distr="custom") {
    TStopwatch stopwatch;
    stopwatch.Start();

    cout<<"Done, running simulation..."<<endl;

    unsigned int m;
    myPoint vertex;
    TClonesArray inHits("myPoint", 80);  
    TClonesArray outHits("myPoint", 80); 

    if (gSystem->AccessPathName("../data/")) {
        gSystem->MakeDirectory("../data/");
    }

    string filename;
    if (noise){
        filename = "../data/sim" + to_string(events) + "_noise.root";
    } else {
        filename = "../data/sim" + to_string(events) + ".root";
    }

    TFile* file = TFile::Open(filename.c_str(), "RECREATE");

    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(0);

    tree->Branch("vertex", &vertex);
    tree->Branch("inHits", &inHits);
    tree->Branch("outHits", &outHits);
    tree->Branch("multiplicity", &m);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn(noise), hOut(noise);
    HitPoint hB(noise); // used only for MS

    TH1F* thetaHist = t.LoadDistribution("heta2");
    
    unsigned int inCounter = 0;
    unsigned int outCounter = 0;

    // Variables for for later
    const double rCilIn = 4.1;
    const double rCilOut = 7.1;
    const double rB = 3.1;
    double phiIn = 0.0;
    double phiOut = 0.0;    
    double zIn_sm = 0.0;
    double zOut_sm = 0.0;
    const double sigmaZ = 0.012;
    const double sigmaPhi = 0.003;
    const double noiseProb = 0.1; 
    const double maxNoisePoints = 3; // per layer 

    int processedEvents = 0;
    int eventsWithHits = 0;
    bool hasHits = false;
    for (unsigned int i = 0; i < events; ++i) {
        processedEvents++;
        e.SetVertex();
        e.SetMultiplicity(distr);
        m = e.GetMultiplicity();

        vertex.Set(e.GetVertex(0),e.GetVertex(1), e.GetVertex(2), i);

        inHits.Clear();
        outHits.Clear();
        inCounter = 0;
        outCounter = 0;


        while (e.RndmUni(0,1) < noiseProb && inCounter < maxNoisePoints && noise) {
            double z = e.RndmUni(-13.5, 13.5);
            double phi = e.RndmUni(0, 2 * M_PI);
            double x = rCilIn * cos(phi);
            double y = rCilIn * sin(phi);
            new (inHits[inCounter]) myPoint(x, y, z, i);
            hasHits=true;
            ++inCounter;
        }

        while (e.RndmUni(0,1)  < noiseProb && outCounter < maxNoisePoints && noise) {
            double z = e.RndmUni(-13.5, 13.5);
            double phi = e.RndmUni(0, 2 * M_PI);
            double x = rCilOut * cos(phi); 
            double y = rCilOut * sin(phi);
            new (outHits[outCounter]) myPoint(x, y, z, i);
            ++outCounter;
            hasHits=true;
        }



        for (unsigned int j = 0; j < m; ++j) {
            t.SetThetaNPhi(thetaHist);
            t.SetParC();

            if (MS) {

                hB.SetDelta_and_T(e, t, rB);
                hB.SetPoint(e, t);
                t.MSRotateParC(t);
                
                hIn.SetDelta_and_T(hB, t, rCilIn);
                hIn.SetPoint(hB, t, rCilIn);
                t.MSRotateParC(t);

                hOut.SetDelta_and_T(hIn, t, rCilOut);
                hOut.SetPoint(hIn, t, rCilOut);
            } else {

                hIn.SetDelta_and_T(e, t, rCilIn);
                hOut.SetDelta_and_T(e, t, rCilOut);
                hIn.SetPoint(e, t, rCilIn);
                hOut.SetPoint(e, t, rCilOut);
            }            
            
            if (abs(hIn.GetZ()) < 13.5) {
                hasHits=true;
                new (inHits[inCounter]) myPoint(hIn.GetX(), hIn.GetY(), hIn.GetZ(), hIn.GetPhi(), i);
                ++inCounter;
                
                if (abs(hOut.GetZ()) < 13.5) {
                    new (outHits[outCounter]) myPoint(hOut.GetX(), hOut.GetY(), hOut.GetZ(), hOut.GetPhi(), i);
                    ++outCounter;
                }
            }
        }
        if(hasHits){
            ++eventsWithHits;
            hasHits = false;
        }


        tree->Fill();
    }

    file->Write();
    file->Close();
    delete file;

    cout << "Total events: " << processedEvents << endl;
    cout << "Total events with hits: " << eventsWithHits << endl;
    cout << "Done."<< endl;

    stopwatch.Stop();
    stopwatch.Print();
}

