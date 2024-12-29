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


    auto dfWithNoise = df.Define("sm_inHits_x", [&rnd, &sigmaX](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaX);}, {"inHits_x"})
                        .Define("sm_inHits_y", [&rnd, &sigmaY](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaY);}, {"inHits_y"})
                        .Define("sm_inHits_z", [&rnd, &sigmaZ](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaZ);}, {"inHits_z"})
                        .Define("sm_outHits_x", [&rnd, &sigmaX](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaX);}, {"inHits_x"})
                        .Define("sm_outHits_y", [&rnd, &sigmaY](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaY);}, {"inHits_y"})
                        .Define("sm_outHits_z", [&rnd, &sigmaZ](const vector<double>& coorVec){return apply_smearing(rnd, coorVec, sigmaZ);}, {"inHits_z"});


    //                    .Define("sm_inHits_y", apply_smearing, {"inHits_y"})
    //                    .Define("sm_inHits_z", apply_smearing, {"inHits_z"})
    //                    .Define("sm_outHits_x", apply_smearing, {"outHits_x"})
    //                    .Define("sm_outHits_z", apply_smearing, {"outHits_y"})
    //                    .Define("sm_outHits_z", apply_smearing, {"outHits_z"});
//
//
    //// Add Gaussian noise to a branch (e.g., "x")
    //auto dfWithNoise = df.Define("sm_inHits_x", [&rnd](const std::vector<double>& xVec) {
    //                            vector<double> xWithNoise;
    //                            xWithNoise.reserve(xVec.size());
    //                            for (auto x : xVec) {
    //                                xWithNoise.push_back(x + rnd.Gaus(0, 0.012)); // Noise for each "x"
    //                            }
    //                            return xWithNoise;
    //                        }, {"inHits_x"})
    //                    .Define("sm_inHits_y", [&rnd](const std::vector<double>& yVec) {
    //                            vector<double> yWithNoise;
    //                            yWithNoise.reserve(yVec.size());
    //                            for (auto y : yVec) {
    //                                yWithNoise.push_back(y + rnd.Gaus(0, 0.012)); // Noise for each "y"
    //                            }
    //                            return yWithNoise;
    //                        }, {"inHits_y"})
    //                    .Define("sm_inHits_z", [&rnd](const std::vector<double>& zVec) {
    //                            vector<double> zWithNoise;
    //                            zWithNoise.reserve(zVec.size());
    //                            for (auto z : zVec) {
    //                                zWithNoise.push_back(z + rnd.Gaus(0, 0.012)); // Noise for each "z"
    //                            }
    //                            return zWithNoise;
    //                        }, {"inHits_z"})
    //                    .Define("sm_outHits_x", [&rnd](const std::vector<double>& xVec) {
    //                            vector<double> xWithNoise;
    //                            xWithNoise.reserve(xVec.size());
    //                            for (auto x : xVec) {
    //                                xWithNoise.push_back(x + rnd.Gaus(0, 0.012)); // Noise for each "x"
    //                            }
    //                            return xWithNoise;
    //                        }, {"outHits_x"})
    //                    .Define("sm_outHits_y", [&rnd](const std::vector<double>& yVec) {
    //                            vector<double> yWithNoise;
    //                            yWithNoise.reserve(yVec.size());
    //                            for (auto y : yVec) {
    //                                yWithNoise.push_back(y + rnd.Gaus(0, 0.012)); // Noise for each "y"
    //                            }
    //                            return yWithNoise;
    //                        }, {"outHits_y"})
    //                    .Define("sm_outHits_z", [&rnd](const std::vector<double>& zVec) {
    //                            vector<double> zWithNoise;
    //                            zWithNoise.reserve(zVec.size());
    //                            for (auto z : zVec) {
    //                                zWithNoise.push_back(z + rnd.Gaus(0, 0.012)); // Noise for each "z"
    //                            }
    //                            return zWithNoise;
    //                        }, {"outHits_z"});




    // Save the updated tree to a new ROOT file
    dfWithNoise.Snapshot(treeName, outputFileName);
    
    //delete dfWithNoise;
    //delete rnd;


    stopwatch.Stop();
    stopwatch.Print();


}













