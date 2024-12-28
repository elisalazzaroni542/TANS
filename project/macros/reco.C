# include <TFile.h>
#include <TStopwatch.h>
#include <ROOT/RDataFrame.hxx>
#include <TRandom3.h>



void reco(){

    TStopwatch stopwatch;
    stopwatch.Start();

    //TFile* fIn = TFile::Open(simData, "READ");

    // Input and output file names
    const char* inputFileName = "../data/sim100000.root";
    const char* outputFileName = "../data/sim100000_smearing.root";
    const char* treeName = "Events"; // Replace with your tree name

    // Create a RDataFrame for the input tree
    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df(treeName, inputFileName);

    // Random number generator (thread-safe with TRandom3)
    TRandom3 rnd;

    // Add Gaussian noise to a branch (e.g., "x")
auto dfWithNoise = df.Define("sm_inHits_x", [&rnd](const std::vector<double>& xVec) {
                                std::vector<double> xWithNoise;
                                xWithNoise.reserve(xVec.size());
                                for (auto x : xVec) {
                                    xWithNoise.push_back(x + rnd.Gaus(0, 0.012)); // Noise for each "x"
                                }
                                return xWithNoise;
                            }, {"inHits_x"})
                   .Define("sm_inHits_y", [&rnd](const std::vector<double>& yVec) {
                                std::vector<double> yWithNoise;
                                yWithNoise.reserve(yVec.size());
                                for (auto y : yVec) {
                                    yWithNoise.push_back(y + rnd.Gaus(0, 0.012)); // Noise for each "y"
                                }
                                return yWithNoise;
                            }, {"inHits_y"})
                   .Define("sm_inHits_z", [&rnd](const std::vector<double>& zVec) {
                                std::vector<double> zWithNoise;
                                zWithNoise.reserve(zVec.size());
                                for (auto z : zVec) {
                                    zWithNoise.push_back(z + rnd.Gaus(0, 0.012)); // Noise for each "z"
                                }
                                return zWithNoise;
                            }, {"inHits_z"})
                   .Define("sm_outHits_x", [&rnd](const std::vector<double>& xVec) {
                                std::vector<double> xWithNoise;
                                xWithNoise.reserve(xVec.size());
                                for (auto x : xVec) {
                                    xWithNoise.push_back(x + rnd.Gaus(0, 0.012)); // Noise for each "x"
                                }
                                return xWithNoise;
                            }, {"outHits_x"})
                   .Define("sm_outHits_y", [&rnd](const std::vector<double>& yVec) {
                                std::vector<double> yWithNoise;
                                yWithNoise.reserve(yVec.size());
                                for (auto y : yVec) {
                                    yWithNoise.push_back(y + rnd.Gaus(0, 0.012)); // Noise for each "y"
                                }
                                return yWithNoise;
                            }, {"outHits_y"})
                   .Define("sm_outHits_z", [&rnd](const std::vector<double>& zVec) {
                                std::vector<double> zWithNoise;
                                zWithNoise.reserve(zVec.size());
                                for (auto z : zVec) {
                                    zWithNoise.push_back(z + rnd.Gaus(0, 0.012)); // Noise for each "z"
                                }
                                return zWithNoise;
                            }, {"outHits_z"});




    // Save the updated tree to a new ROOT file
    dfWithNoise.Snapshot(treeName, outputFileName);

    stopwatch.Stop();
    stopwatch.Print();


}













