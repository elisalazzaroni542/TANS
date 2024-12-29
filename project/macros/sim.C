#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"
//#include <ROOT/RDataFrame.hxx>



void sim(int seed = 123, unsigned const int events = 1000) {
    TStopwatch stopwatch;
    stopwatch.Start();

//    vector<double> vertex_x(1), vertex_y(1), vertex_z(1);
    vector<double> vertex;
    vector<double> inHits_x, inHits_y, inHits_z;
    vector<double> outHits_x, outHits_y, outHits_z;
    
    vertex.resize(3);

    const size_t max_expected_multiplicity = 100;

    inHits_x.reserve(max_expected_multiplicity);
    inHits_y.reserve(max_expected_multiplicity);
    inHits_z.reserve(max_expected_multiplicity);
    outHits_x.reserve(max_expected_multiplicity);
    outHits_y.reserve(max_expected_multiplicity);
    outHits_z.reserve(max_expected_multiplicity);

    const int autoSaveSize = 100000;


    string filename = "../data/sim" + to_string(events) + ".root";
    
    
    TFile* file = TFile::Open(filename.c_str(), "RECREATE");

    
    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(autoSaveSize);
    //file->SetCompressionLevel(0);

    //tree->Branch("vertex_x", &vertex_x);
    //tree->Branch("vertex_y", &vertex_y);
    //tree->Branch("vertex_z", &vertex_z);

    tree->Branch("vertex", &vertex);
    tree->Branch("inHits_x", &inHits_x);
    tree->Branch("inHits_y", &inHits_y);
    tree->Branch("inHits_z", &inHits_z);
    tree->Branch("outHits_x", &outHits_x);
    tree->Branch("outHits_y", &outHits_y);
    tree->Branch("outHits_z", &outHits_z);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn, hOut;


    TH1F* thetaHist = t.LoadDistribution("heta2");

    //ROOT::EnableImplicitMT();

    //t.SetThetaNPhi(thetaHist);
    //t.SetParC(3);
    for (unsigned int i = 0; i < events; ++i) {
        e.SetVertix(3);
        e.SetMultiplicity("custom");
        //if (i%1000 == 0) {cout<<"Multiplicity = "<<e.GetMultiplicity()<<endl;}
        vertex[0] = e.GetVertix(0);
        vertex[1] = e.GetVertix(1);
        vertex[2] = e.GetVertix(2);
        // Clear previous hits
        inHits_x.clear();
        inHits_y.clear();
        inHits_z.clear();
        outHits_x.clear();
        outHits_y.clear();
        outHits_z.clear();

        for (unsigned int j = 0; j < e.GetMultiplicity(); ++j) {
            t.SetThetaNPhi(thetaHist);
            t.SetParC(3);
            hIn.SetPoint(e, t);
            hOut.SetPoint(e, t);

            inHits_x.push_back(hIn.GetX());
            inHits_y.push_back(hIn.GetY());
            inHits_z.push_back(hIn.GetZ());
            outHits_x.push_back(hOut.GetX());
            outHits_y.push_back(hOut.GetY());
            outHits_z.push_back(hOut.GetZ());
        }
        
        tree->Fill();
    }

    file->Write();
    file->Close();
    delete file;

    stopwatch.Stop();
    stopwatch.Print();
}




