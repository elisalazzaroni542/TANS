#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"


//void sim(int seed = 123, unsigned const int events = 1000) {
//    TStopwatch stopwatch;
//    stopwatch.Start();
//
//    string filename = "../data/sim" + to_string(events) + ".root";
//    TFile *file = new TFile(filename.c_str(), "RECREATE");
//    //file->SetCompressionLevel(9); // Moderate compression
//
//    TTree *tree = new TTree("Events", "Simulated events");
//
//    const unsigned int rIn = 4, rOut = 7;
//    vector<double> vertex_x(1), vertex_y(1), vertex_z(1);
//    vector<double> inHits_x, inHits_y, inHits_z, outHits_x, outHits_y, outHits_z;
//
//    size_t max_hits = 100; // Estimate maximum hits per event
//    inHits_x.resize(max_hits);
//    inHits_y.resize(max_hits);
//    inHits_z.resize(max_hits);
//    outHits_x.resize(max_hits);
//    outHits_y.resize(max_hits);
//    outHits_z.resize(max_hits);
//
//    tree->Branch("vertex_x", &vertex_x);
//    tree->Branch("vertex_y", &vertex_y);
//    tree->Branch("vertex_z", &vertex_z);
//    tree->Branch("inHits_x", &inHits_x);
//    tree->Branch("inHits_y", &inHits_y);
//    tree->Branch("inHits_z", &inHits_z);
//    tree->Branch("outHits_x", &outHits_x);
//    tree->Branch("outHits_y", &outHits_y);
//    tree->Branch("outHits_z", &outHits_z);
//
//    Event e(seed);
//    Trajectory t(seed);
//    HitPoint hIn, hOut;
//
//    for (unsigned int i = 0; i < events; ++i) {
//        e.SetVertix(3);
//        e.SetMultiplicity(1);
//
//        vertex_x[0] = e.GetVertix(1);
//        vertex_y[0] = e.GetVertix(2);
//        vertex_z[0] = e.GetVertix(3);
//
//        for (unsigned int j = 0; j < e.GetMultiplicity(); ++j) {
//            t.SetThetaNPhi();
//            t.SetParC();
//
//            hIn.SetPoint(e, t);
//            hOut.SetPoint(e, t);
//
//            inHits_x[j] = hIn.GetX();
//            inHits_y[j] = hIn.GetY();
//            inHits_z[j] = hIn.GetZ();
//            outHits_x[j] = hOut.GetX();
//            outHits_y[j] = hOut.GetY();
//            outHits_z[j] = hOut.GetZ();
//
//        }
//
//        tree->Fill();
//    }
//
//    file->cd();
//    tree->Print();
//    tree->Write();
//    file->Close();
//    delete file;
//
//    stopwatch.Stop();
//    stopwatch.Print();
//}


void sim(int seed = 123, unsigned const int events = 1000) {
    TStopwatch stopwatch;
    stopwatch.Start();

    vector<double> vertex_x(1), vertex_y(1), vertex_z(1);
    vector<double> inHits_x, inHits_y, inHits_z;
    vector<double> outHits_x, outHits_y, outHits_z;
    
    const size_t expected_multiplicity = 100;

    inHits_x.reserve(expected_multiplicity);
    inHits_y.reserve(expected_multiplicity);
    inHits_z.reserve(expected_multiplicity);
    outHits_x.reserve(expected_multiplicity);
    outHits_y.reserve(expected_multiplicity);
    outHits_z.reserve(expected_multiplicity);

    const int autoSaveSize = 100000;

    string filename = "../data/sim" + to_string(events) + ".root";
    TFile* file = TFile::Open(filename.c_str(), "RECREATE");

    
    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(autoSaveSize);
    //file->SetCompressionLevel(0);

    tree->Branch("vertex_x", &vertex_x);
    tree->Branch("vertex_y", &vertex_y);
    tree->Branch("vertex_z", &vertex_z);
    tree->Branch("inHits_x", &inHits_x);
    tree->Branch("inHits_y", &inHits_y);
    tree->Branch("inHits_z", &inHits_z);
    tree->Branch("outHits_x", &outHits_x);
    tree->Branch("outHits_y", &outHits_y);
    tree->Branch("outHits_z", &outHits_z);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn, hOut;


    for (unsigned int i = 0; i < events; ++i) {
        e.SetVertix(3);
        e.SetMultiplicity(1);
        
        vertex_x[0] = e.GetVertix(1);
        vertex_y[0] = e.GetVertix(2);
        vertex_z[0] = e.GetVertix(3);

        // Clear previous hits
        inHits_x.clear();
        inHits_y.clear();
        inHits_z.clear();
        outHits_x.clear();
        outHits_y.clear();
        outHits_z.clear();

        for (unsigned int j = 0; j < e.GetMultiplicity(); ++j) {
            t.SetThetaNPhi();
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




