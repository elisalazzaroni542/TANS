#include <TFile.h>
#include <TStopwatch.h>
#include <ROOT/RDataFrame.hxx>
#include <TRandom3.h>

vector<double> apply_smearing(TRandom3& rnd, const vector<double>& xVec, double sigma) {
                            vector<double> xWithNoise;
                            xWithNoise.reserve(xVec.size());
                            for (double x : xVec) {
                                xWithNoise.push_back(x + rnd.Gaus(0, sigma)); // Noise for each "x"
                            }
                            return xWithNoise;
                        }

void reco(){

    TStopwatch stopwatch;
    stopwatch.Start();

    //TFile* fIn = TFile::Open(simData, "READ");

    // Input and output file names
    const char* inputFileName = "../data/sim1000000.root";
    const char* outputFileName = "../data/sim1000000_smearing.root";
    const char* treeName = "Events"; // Replace with your tree name

    // Create a RDataFrame for the input tree
    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df(treeName, inputFileName);

    // Random number generator (thread-safe with TRandom3)
    TRandom3 rnd;
    double sigmaX = 0.003;
    double sigmaY = 0.003;
    double sigmaZ = 0.012;


    auto dfWithNoise = df.Define("inHits_x_sm", [&rnd, &sigmaX](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaX);}, {"inHits_x"})
                        .Define("inHits_y_sm", [&rnd, &sigmaY](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaY);}, {"inHits_y"})
                        .Define("inHits_z_sm", [&rnd, &sigmaZ](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaZ);}, {"inHits_z"})
                        .Define("outHits_x_sm", [&rnd, &sigmaX](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaX);}, {"outHits_x"})
                        .Define("outHits_y_sm", [&rnd, &sigmaY](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaY);}, {"outHits_y"})
                        .Define("outHits_z_sm", [&rnd, &sigmaZ](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaZ);}, {"outHits_z"});


 


    // Save the updated tree to a new ROOT file
    dfWithNoise.Snapshot(treeName, outputFileName);
    
    //delete dfWithNoise;
    //delete rnd;


    stopwatch.Stop();
    stopwatch.Print();


}













