#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TClonesArray.h>
#include <TMath.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"
#include "../headers/Point.h"



/*
double findAngle(double M1, double M2)
{
    // Store the tan value  of the angle
    double angle = abs((M2 - M1)
                       / (1 + M1 * M2));
 
    // Calculate tan inverse of the angle
    double ret = atan(angle);
 
    // Convert the angle from
    // radian to degree
    //double val = (ret * 180) / M_PI;
 
    // Print the result
    // cout << val;
    return ret;
}
*/

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



void sim(int seed = 123, unsigned const int events = 1000000, bool MS = true, bool noise = true) {
    TStopwatch stopwatch;
    stopwatch.Start();

    TRandom3 rnd(seed);

    unsigned int m;
    //vector<double> vertex(3);
    Point vertex;
    TClonesArray inHits("Point", 80);  
    TClonesArray outHits("Point", 80); 

//    const int autoSaveSize = 100000;
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
        e.SetMultiplicity("custom");
        m = e.GetMultiplicity();
        //vertex[0] = e.GetVertex(0);
        //vertex[1] = e.GetVertex(1);
        //vertex[2] = e.GetVertex(2);

        vertex.Set(e.GetVertex(0),e.GetVertex(1), e.GetVertex(2), i);

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

            // Common hit recording logic
            //zIn_sm = hIn.GetZ() + rnd.Gaus(0, sigmaZ);
            //zOut_sm = hOut.GetZ() + rnd.Gaus(0, sigmaZ);
            
            
            if (abs(hIn.GetZ()) < 13.5) {
                hasHits=true;
                //phiIn = findAngle(hIn.GetY(), hIn.GetX()) + rnd.Gaus(0, sigmaPhi);
                //phiIn = atan2(hIn.GetY(), hIn.GetX()) + rnd.Gaus(0, sigmaPhi);
                new (inHits[inCounter]) Point(hIn.GetX(), hIn.GetY(), hIn.GetZ(), hIn.GetPhi(), i);
                ++inCounter;
                
                if (abs(hOut.GetZ()) < 13.5) {
                    //phiOut = findAngle(hOut.GetY(), hOut.GetX()) + rnd.Gaus(0, sigmaPhi);
                    //phiOut = atan2(hOut.GetY(), hOut.GetX()) + rnd.Gaus(0, sigmaPhi);
                    new (outHits[outCounter]) Point(hOut.GetX(), hOut.GetY(), hOut.GetZ(), hOut.GetPhi(), i);
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

    stopwatch.Stop();
    stopwatch.Print();
}

