#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"


//gSystem->AddIncludePath("-I../headers");
//gSystem->AddDynamicPath("../src");
//gSystem->Load("Event_cxx.so");
//gSystem->Load("HitPoint_cxx.so");
//gSystem->Load("Trajectory_cxx.so");

//void sim(int seed=123, unsigned const int events=1000){
//
//    TStopwatch stopwatch;
//    stopwatch.Start();
//
///*
//    gSystem->AddIncludePath("-I../headers");
//    gSystem->AddDynamicPath("../src");
//    
//    gSystem->Load("Event_cxx.so");
//    gSystem->Load("HitPoint_cxx.so");
//    gSystem->Load("Trajectory_cxx.so");
//*/
//    string filename = "../data/sim" + to_string(events) + ".root";  
//    //cout<<filename<<endl;
//    TFile *file = new TFile(filename.c_str(), "RECREATE");
//    //TFile *file = new TFile("../data/ciao.root", "RECREATE");
//    file->SetCompressionLevel(4);
//    TTree *tree = new TTree("Events", "Simulated events");
//
//    const unsigned int rIn  = 4;
//    const unsigned int rOut = 7;
//
//    vector <Trajectory> Trajs;
//
//    vector<double> vertex_x, vertex_y, vertex_z;
//    vector<double> inHits_x, inHits_y, inHits_z;
//    vector<double> outHits_x, outHits_y, outHits_z;
//
//
//    vertex_x.reserve(1);
//    vertex_y.reserve(1);
//    vertex_z.reserve(1);
//
//    // Create branches
//    tree->Branch("vertex_x", &vertex_x);
//    tree->Branch("vertex_y", &vertex_y);
//    tree->Branch("vertex_z", &vertex_z);
//
//    tree->Branch("inHits_x", &inHits_x);
//    tree->Branch("inHits_y", &inHits_y);
//    tree->Branch("inHits_z", &inHits_z);
//
//    tree->Branch("outHits_x", &outHits_x);
//    tree->Branch("outHits_y", &outHits_y);
//    tree->Branch("outHits_z", &outHits_z);
//
//
//    for(unsigned int i = 0; i < events; ++i) {
//        Event e(seed);
//        e.SetVertix(3);
//        e.SetMultiplicity(1);
//
//        const unsigned int mult = e.GetMultiplicity();
//        Trajs.clear();
//        Trajs.reserve(mult);
//
//        for(unsigned int j = 0; j < mult; ++j) {
//            Trajectory t(seed);
//            t.SetThetaNPhi();
//            t.SetParC();
//            Trajs.emplace_back(t);
//        }
//
//
//        vertex_x[0] = e.GetVertix(1);
//        vertex_y[0] = e.GetVertix(2);
//        vertex_z[0] = e.GetVertix(3);
//
//        //vertex_x.emplace_back(e.GetVertix(1));
//        //vertex_y.emplace_back(e.GetVertix(2));
//        //vertex_z.emplace_back(e.GetVertix(3));
//
//        inHits_x.clear();
//        inHits_y.clear();
//        inHits_z.clear();
//        outHits_x.clear();
//        outHits_y.clear();
//        outHits_z.clear();
//
//        for (const auto& traj : Trajs) {
//            HitPoint hIn(e, traj, rIn);
//            HitPoint hOut(e, traj, rOut);
//
//            hIn.SetPoint(e, traj);
//            hOut.SetPoint(e, traj);
//
//            inHits_x.emplace_back(hIn.GetX());
//            inHits_y.emplace_back(hIn.GetY());
//            inHits_z.emplace_back(hIn.GetZ());
//
//            outHits_x.emplace_back(hOut.GetX());
//            outHits_y.emplace_back(hOut.GetY());
//            outHits_z.emplace_back(hOut.GetZ());
//        }
//
//        tree->Fill();
//    }
//
////    tree->cd();
//    file->cd();
//    tree->Print();
//    tree->Write();
//    cout<<"HERE"<<endl;
//    file->Close();
//
//    delete file;
//
//    stopwatch.Stop();
//    stopwatch.Print();
//
//}


void sim(int seed = 123, unsigned const int events = 1000) {
    TStopwatch stopwatch;
    stopwatch.Start();

    string filename = "../data/sim" + to_string(events) + ".root";
    TFile *file = new TFile(filename.c_str(), "RECREATE");
    file->SetCompressionLevel(4); // Moderate compression

    TTree *tree = new TTree("Events", "Simulated events");

    const unsigned int rIn = 4, rOut = 7;
    vector<double> vertex_x(1), vertex_y(1), vertex_z(1);
    vector<double> inHits_x, inHits_y, inHits_z, outHits_x, outHits_y, outHits_z;

    size_t max_hits = 100; // Estimate maximum hits per event
    inHits_x.resize(max_hits);
    inHits_y.resize(max_hits);
    inHits_z.resize(max_hits);
    outHits_x.resize(max_hits);
    outHits_y.resize(max_hits);
    outHits_z.resize(max_hits);

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

        for (unsigned int j = 0; j < e.GetMultiplicity(); ++j) {
            t.SetThetaNPhi();
            t.SetParC();

            hIn.SetPoint(e, t);
            hOut.SetPoint(e, t);

            inHits_x[j] = hIn.GetX();
            inHits_y[j] = hIn.GetY();
            inHits_z[j] = hIn.GetZ();
            outHits_x[j] = hOut.GetX();
            outHits_y[j] = hOut.GetY();
            outHits_z[j] = hOut.GetZ();

        }

        tree->Fill();
    }

    file->cd();
    tree->Print();
    tree->Write();
    file->Close();
    delete file;

    stopwatch.Stop();
    stopwatch.Print();
}
