#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <filesystem>
#include <Riostream.h>

void DrawHistograms(const char* filename="../data/sim1000000_smearing.root", const char* treename="Events") {
    TStopwatch stopwatch;
    stopwatch.Start();

    string plotDir = "../plots/";
    std::filesystem::create_directories(plotDir);
    TFile* file = TFile::Open(filename);
    TTree* tree = (TTree*)file->Get(treename);

    auto branches = tree->GetListOfBranches();
    unsigned const int nBranches = branches->GetEntries();
    TH1D* mult = new TH1D("mult", "Molteplicità", 100, 0, 100);

    for (unsigned int i = 0; i < nBranches; ++i) {
        TBranch* branch = (TBranch*)branches->At(i);
        string branchName = branch->GetName();

        if (branchName == "vertex") {
            cout<<"IF Ciao"<<endl;
            vector<double>* vertex = new vector<double>();
            tree->SetBranchAddress(branchName.c_str(), &vertex);

            TH1D* histX = new TH1D("vertex_x", "Vertex - X Coordinate", 100, -0.5, 0.5);
            TH1D* histY = new TH1D("vertex_y", "Vertex - Y Coordinate", 100, -0.5, 0.5);
            TH1D* histZ = new TH1D("vertex_z", "Vertex - Z Coordinate", 100, -15, 15);

            unsigned int nEntries = tree->GetEntries();
            for (unsigned int j = 0; j < nEntries; ++j) {
                tree->GetEntry(j);
                histX->Fill((*vertex)[0]);
                histY->Fill((*vertex)[1]);
                histZ->Fill((*vertex)[2]);
            }

            TCanvas canvas;
            histX->Draw();
            canvas.SaveAs((plotDir + "vertex_x.png").c_str());
            histY->Draw();
            canvas.SaveAs((plotDir + "vertex_y.png").c_str());
            histZ->Draw();
            canvas.SaveAs((plotDir + "vertex_z.png").c_str());

            delete histX;
            delete histY;
            delete histZ;
            delete vertex;

        } else {
            vector<double>* data = new vector<double>();
            tree->SetBranchAddress(branchName.c_str(), &data);

            bool z_branch = branchName.find("_z") != std::string::npos;
            bool last_branch = i == nBranches - 1;

            TH1D* hist;
            if (z_branch) {
                hist = new TH1D(branchName.c_str(), branchName.c_str(), 100, -15, 15);
            } else {
                hist = new TH1D(branchName.c_str(), branchName.c_str(), 100, -10, 10);
            }

            unsigned const int nEntries = tree->GetEntries();
            for (unsigned int j = 0; j < nEntries; ++j) {
                //cout<<"ELSE Ciao"<<endl;
                tree->GetEntry(j);

                for (double value : *data) {
                    hist->Fill(value);
                }
                if (last_branch) {
                    mult->Fill(data->size());
                }
            }

            TCanvas canvas;
            hist->Draw();
            canvas.SaveAs((plotDir + branchName + ".png").c_str());

            if (last_branch) {
                mult->Draw();
                canvas.SaveAs((plotDir + "mult.png").c_str());
            }
            delete data;
            delete hist;
            if (last_branch) {
                delete mult;
            }
        }
    }

    file->Close();
    stopwatch.Stop();
    stopwatch.Print();
}
