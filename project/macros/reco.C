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

































vector<array<double,3>> generate_cylinder_points(TRandom3& rnd, double radius, double length, int nPoints) {
    vector<array<double,3>> points;
    points.reserve(nPoints);
    
    for(int i = 0; i < nPoints; i++) {
        // Generate random z, theta
        double z = rnd.Uniform(-length/2, length/2);
        double theta = rnd.Uniform(0, 2 * M_PI);
        
        // Convert to cartesian coordinates
        double x = radius * cos(theta);
        double y = radius * sin(theta);
        
        points.push_back({x, y, z});
    }
    
    return points;
}

// Function to merge vectors and apply smearing
vector<double> merge_and_smear(TRandom3& rnd, const vector<double>& original, 
                             const vector<double>& random_points, double sigma) {
    vector<double> result = original;
    result.reserve(original.size() + random_points.size());
    
    // Add random points with smearing
    for(const auto& point : random_points) {
        result.push_back(point + rnd.Gaus(0, sigma));
    }
    
    return result;
}

void reco(){
    TStopwatch stopwatch;
    stopwatch.Start();
    
    const char* inputFileName = "../data/sim1000000.root";
    const char* outputFileName = "../data/sim1000000_smearing.root";
    const char* treeName = "Events";
    
    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df(treeName, inputFileName);
    
    TRandom3 rnd;
    double sigmaX = 0.003;
    double sigmaY = 0.003;
    double sigmaZ = 0.012;
    
    // Constants for cylinders
    const double innerRadius = 4.0;
    const double outerRadius = 7.0;
    const double length = 27.0;
    const int nRandomPoints = 5;
    
    auto dfWithNoise = df
        .Define("random_inner_points", [&rnd, innerRadius, length, nRandomPoints]() {
            return generate_cylinder_points(rnd, innerRadius, length, nRandomPoints);
        })
        .Define("random_outer_points", [&rnd, outerRadius, length, nRandomPoints]() {
            return generate_cylinder_points(rnd, outerRadius, length, nRandomPoints);
        })
        // Inner hits with noise and random points
        .Define("inHits_x_sm", [&rnd, sigmaX](const vector<double>& orig, const vector<array<double,3>>& random) {
            vector<double> random_x;
            random_x.reserve(random.size());
            for(const auto& p : random) random_x.push_back(p[0]);
            return merge_and_smear(rnd, orig, random_x, sigmaX);
        }, {"inHits_x", "random_inner_points"})
        .Define("inHits_y_sm", [&rnd, sigmaY](const vector<double>& orig, const vector<array<double,3>>& random) {
            vector<double> random_y;
            random_y.reserve(random.size());
            for(const auto& p : random) random_y.push_back(p[1]);
            return merge_and_smear(rnd, orig, random_y, sigmaY);
        }, {"inHits_y", "random_inner_points"})
        .Define("inHits_z_sm", [&rnd, sigmaZ](const vector<double>& orig, const vector<array<double,3>>& random) {
            vector<double> random_z;
            random_z.reserve(random.size());
            for(const auto& p : random) random_z.push_back(p[2]);
            return merge_and_smear(rnd, orig, random_z, sigmaZ);
        }, {"inHits_z", "random_inner_points"})
        // Outer hits with noise and random points
        .Define("outHits_x_sm", [&rnd, sigmaX](const vector<double>& orig, const vector<array<double,3>>& random) {
            vector<double> random_x;
            random_x.reserve(random.size());
            for(const auto& p : random) random_x.push_back(p[0]);
            return merge_and_smear(rnd, orig, random_x, sigmaX);
        }, {"outHits_x", "random_outer_points"})
        .Define("outHits_y_sm", [&rnd, sigmaY](const vector<double>& orig, const vector<array<double,3>>& random) {
            vector<double> random_y;
            random_y.reserve(random.size());
            for(const auto& p : random) random_y.push_back(p[1]);
            return merge_and_smear(rnd, orig, random_y, sigmaY);
        }, {"outHits_y", "random_outer_points"})
        .Define("outHits_z_sm", [&rnd, sigmaZ](const vector<double>& orig, const vector<array<double,3>>& random) {
            vector<double> random_z;
            random_z.reserve(random.size());
            for(const auto& p : random) random_z.push_back(p[2]);
            return merge_and_smear(rnd, orig, random_z, sigmaZ);
        }, {"outHits_z", "random_outer_points"});

    dfWithNoise.Snapshot(treeName, outputFileName);
    
    stopwatch.Stop();
    stopwatch.Print();
}




