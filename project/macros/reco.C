#include <TFile.h>
#include <TStopwatch.h>
#include <ROOT/RDataFrame.hxx>
#include <TRandom3.h>
#include <cmath>
#include <TClonesArray.h>
#include "Point.h"
#include "HitPoint.h"


TClonesArray* apply_smearing(TRandom3& rnd, const TClonesArray* hits, double sigmaXY, double sigmaZ, unsigned int rCil) {
    const unsigned int nRand = 5; // Number of random points to add
    int newArrSize = hits->GetSize() + nRand;

    TClonesArray* recoData = new TClonesArray("Point", newArrSize);

    for (int i = 0; i < newArrSize; ++i) {
        if (i < hits->GetSize()) {
            Point* point = (Point*)hits->At(i);
            double x = point->GetX();
            double y = point->GetY();
            double z = point->GetZ();

            double x_sm, y_sm, z_sm;
            double new_radius2;

            do {
                x_sm = x + rnd.Gaus(0, sigmaXY);
                y_sm = y + rnd.Gaus(0, sigmaXY);
                new_radius2 = x_sm * x_sm + y_sm * y_sm;
            } while (new_radius2 > (rCil + 0.2) * (rCil + 0.2) || new_radius2 < rCil * rCil);

            do {
                z_sm = z + rnd.Gaus(0, sigmaZ);
            } while (fabs(z_sm) > 13.5);

            new((*recoData)[i]) Point(x_sm, y_sm, z_sm);
        } else {
            double theta = rnd.Uniform(0, 2 * M_PI);
            double x = rCil * cos(theta);
            double y = rCil * sin(theta);
            double z = rnd.Uniform(-13.5, 13.5);
            new((*recoData)[i]) Point(x, y, z);
        }
    }

    return recoData;
}

void reco() {
    TStopwatch stopwatch;
    stopwatch.Start();

    ROOT::EnableImplicitMT();
    
    const char* inputFileName = "../data/sim1000000.root";
    const char* outputFileName = "../data/sim1000000_smearing.root";
    const char* treeName = "Events";

    ROOT::RDataFrame df(treeName, inputFileName);
    TRandom3 rnd;
    
    auto dfWithNoise = df
        .Redefine("inHits", [&rnd](const TClonesArray& hits) {
            return apply_smearing(rnd, &hits, 0.003, 0.012, 4);
        }, {"inHits"})
        .Redefine("outHits", [&rnd](const TClonesArray& hits) {
            return apply_smearing(rnd, &hits, 0.003, 0.012, 7);
        }, {"outHits"});

    dfWithNoise.Snapshot(treeName, outputFileName);

    stopwatch.Stop();
    stopwatch.Print();
}




/*

// Function to apply smearing while maintaining cylindrical surface constraint
vector<double> apply_smearing_xy(TRandom3& rnd, const vector<double>& xVec, const vector<double>& yVec, 
                               double sigma, bool isX, unsigned int rCil) {
    vector<double> withNoise;
    withNoise.reserve(xVec.size());
    
    for (size_t i = 0; i < xVec.size(); i++) {
        double x = xVec[i];
        double y = yVec[i];
        
        double x_sm;
        double y_sm;
        double new_radius2;
        // Apply gaussian smearing
        if (isX){
            do
            {
                x_sm = x + rnd.Gaus(0, sigma);
                new_radius2 = x_sm*x_sm + y*y;
            } while (new_radius2 > (rCil+0.2)*(rCil+0.2) || new_radius2 < (rCil)*(rCil));
            
        }

        if (!isX){
            do
            {
                y_sm = y + rnd.Gaus(0, sigma);
                new_radius2 = x*x + y_sm*y_sm;
            } while (new_radius2 > (rCil+0.2)*(rCil+0.2) || new_radius2 < (rCil)*(rCil));
            
        }
        // Return either x or y component depending on isX
        withNoise.push_back(isX ? x_sm : y_sm);
    }
    return withNoise;
}

// Regular smearing for z coordinate
vector<double> apply_smearing_z(TRandom3& rnd, const vector<double>& zVec, double sigma, double length=13.5) {
    vector<double> withNoise;
    withNoise.reserve(zVec.size());
    double z_sm;
    for (double z : zVec) {

        do
        {
            z_sm = z + rnd.Gaus(0, sigma);
        } while (abs(z_sm) > length);
        
        if (z_sm > length){ cout<<z_sm<<endl;}

        withNoise.push_back(z_sm);
    }
    return withNoise;
}

void reco() {
    TStopwatch stopwatch;
    stopwatch.Start();

    // Input and output file names
    const char* inputFileName = "../data/sim1000000.root";
    const char* outputFileName = "../data/sim1000000_smearing.root";
    const char* treeName = "Events";

    // Create a RDataFrame for the input tree
    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df(treeName, inputFileName);

    // Random number generator (thread-safe with TRandom3)
    TRandom3 rnd;
    double sigmaX = 0.003;
    double sigmaY = 0.003;
    double sigmaZ = 0.012;

    // Apply smearing and overwrite original branches
    auto dfWithNoise = df
        // Inner hits
        .Redefine("inHits_x", [&rnd, &sigmaX](const vector<double>& x, const vector<double>& y){
            return apply_smearing_xy(rnd, x, y, sigmaX, true, 4);
        }, {"inHits_x", "inHits_y"})
        .Redefine("inHits_y", [&rnd, &sigmaY](const vector<double>& x, const vector<double>& y){
            return apply_smearing_xy(rnd, x, y, sigmaY, false, 4);
        }, {"inHits_x", "inHits_y"})
        .Redefine("inHits_z", [&rnd, &sigmaZ](const vector<double>& z){
            return apply_smearing_z(rnd, z, sigmaZ, 13.5);
        }, {"inHits_z"})
        // Outer hits
        .Redefine("outHits_x", [&rnd, &sigmaX](const vector<double>& x, const vector<double>& y){
            return apply_smearing_xy(rnd, x, y, sigmaX, true, 7);
        }, {"outHits_x", "outHits_y"})
        .Redefine("outHits_y", [&rnd, &sigmaY](const vector<double>& x, const vector<double>& y){
            return apply_smearing_xy(rnd, x, y, sigmaY, false, 7);
        }, {"outHits_x", "outHits_y"})
        .Redefine("outHits_z", [&rnd, &sigmaZ](const vector<double>& z){
            return apply_smearing_z(rnd, z, sigmaZ, 13.5);
        }, {"outHits_z"});

    // Save the updated tree to a new ROOT file
    dfWithNoise.Snapshot(treeName, outputFileName);

    stopwatch.Stop();
    stopwatch.Print();
}

*/