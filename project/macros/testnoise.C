#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <cmath>

void AddRandomPoints(const char* filename="../data/sim1000000.root", const char* treename="Events") {
    // Open the ROOT file
    TFile file(filename, "UPDATE");
    if (file.IsZombie()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Get the TTree
    TTree* tree = (TTree*)file.Get(treename);
    if (!tree) {
        std::cerr << "Error: TTree not found!" << std::endl;
        return;
    }

    // Variables to hold existing and new values
    float x, y, z;

    // Set branch addresses for existing branches
    tree->SetBranchAddress("x", &x);
    tree->SetBranchAddress("y", &y);
    tree->SetBranchAddress("z", &z);

    // Create a new tree to hold the updated data
    TTree* newTree = tree->CloneTree(0); // Clone the structure, but not the entries

    // Copy existing entries to the new tree
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        newTree->Fill();
    }

    // Random number generator
    TRandom3 rand;

    // Number of points to generate
    const int nPoints = 1000; // Adjust as needed

    for (int i = 0; i < nPoints; ++i) {
        // Generate random cylindrical coordinates
        float r = rand.Uniform(0.0, 10.0);       // Radius range [0, 10]
        float theta = rand.Uniform(0.0, 2 * M_PI); // Angle range [0, 2π]
        z = rand.Uniform(-10.0, 10.0);          // Z range [-10, 10]

        // Convert to Cartesian coordinates
        x = r * cos(theta);
        y = r * sin(theta);

        // Fill the new tree with the new values
        newTree->Fill();
    }

    // Write the updated tree back to the file
    newTree->Write(treename, TObject::kOverwrite);
    file.Close();

    std::cout << "Random points added successfully!" << std::endl;
}
