#include <TFile.h>
#include <TTree.h>



void DrawHistograms(const char* filename="../data/sim1000000.root", const char* treename="Events") {
    // Open the file and get the TTree
    TFile* file = TFile::Open(filename);
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    TTree* tree = (TTree*)file->Get(treename);
    if (!tree) {
        std::cerr << "Error: Tree " << treename << " not found in file!" << std::endl;
        file->Close();
        return;
    }

    // Get a list of branches
    TObjArray* branches = tree->GetListOfBranches();
    int nBranches = branches->GetEntries();

    // Loop over all branches
    for (int i = 0; i < nBranches; i++) {
        TBranch* branch = (TBranch*)branches->At(i);
        if (!branch) continue;

        std::string branchName = branch->GetName();

        // Special case for "vertex" branch
        if (branchName == "vertex") {
            std::vector<double>* vertex = nullptr;
            tree->SetBranchAddress(branchName.c_str(), &vertex);

            // Create histograms for x, y, z
            TH1D* histX = new TH1D("vertex_x", "Vertex - X Coordinate", 100, -100, 100);
            TH1D* histY = new TH1D("vertex_y", "Vertex - Y Coordinate", 100, -100, 100);
            TH1D* histZ = new TH1D("vertex_z", "Vertex - Z Coordinate", 100, -100, 100);

            // Fill histograms
            Long64_t nEntries = tree->GetEntries();
            for (Long64_t j = 0; j < nEntries; j++) {
                tree->GetEntry(j);
                if (!vertex || vertex->size() < 3) continue;

                histX->Fill((*vertex)[0]);
                histY->Fill((*vertex)[1]);
                histZ->Fill((*vertex)[2]);
            }

            // Save as PNG
            TCanvas canvas;
            histX->Draw();
            canvas.SaveAs("vertex_x.png");
            histY->Draw();
            canvas.SaveAs("vertex_y.png");
            histZ->Draw();
            canvas.SaveAs("vertex_z.png");

            // Cleanup
            delete histX;
            delete histY;
            delete histZ;
        } else {
            // General case for all other branches
            std::vector<double>* data = nullptr;
            tree->SetBranchAddress(branchName.c_str(), &data);

            std::string histName = "hist_" + branchName;
            std::string histTitle = "Branch: " + branchName;
            TH1D* hist = new TH1D(histName.c_str(), histTitle.c_str(), 100, -100, 100);

            // Fill histogram
            Long64_t nEntries = tree->GetEntries();
            for (Long64_t j = 0; j < nEntries; j++) {
                tree->GetEntry(j);
                if (!data) continue;

                for (double value : *data) {
                    hist->Fill(value);
                }
            }

            // Save as PNG
            TCanvas canvas;
            hist->Draw();
            canvas.SaveAs((branchName + ".png").c_str());

            // Cleanup
            delete hist;
        }
    }

    // Close the file
    file->Close();
    std::cout << "Histograms saved as PNG images." << std::endl;
}
