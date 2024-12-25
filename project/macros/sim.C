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

void sim(int seed=123, unsigned const int events=1000){

    TStopwatch stopwatch;
    stopwatch.Start();

/*
    gSystem->AddIncludePath("-I../headers");
    gSystem->AddDynamicPath("../src");
    
    gSystem->Load("Event_cxx.so");
    gSystem->Load("HitPoint_cxx.so");
    gSystem->Load("Trajectory_cxx.so");
*/
    string filename = "../data/sim" + to_string(events) + ".root";  
    //cout<<filename<<endl;
    TFile *file = new TFile(filename.c_str(), "RECREATE");
    //TFile *file = new TFile("../data/ciao.root", "RECREATE");

    TTree *tree = new TTree("Events", "Simulated events");

    const unsigned int rIn  = 4;
    const unsigned int rOut = 7;

    vector <Trajectory> Trajs;

    vector<double> vertex_x, vertex_y, vertex_z;
    vector<double> inHits_x, inHits_y, inHits_z;
    vector<double> outHits_x, outHits_y, outHits_z;


    vertex_x.reserve(1);
    vertex_y.reserve(1);
    vertex_z.reserve(1);

    // Create branches
    tree->Branch("vertex_x", &vertex_x);
    tree->Branch("vertex_y", &vertex_y);
    tree->Branch("vertex_z", &vertex_z);

    tree->Branch("inHits_x", &inHits_x);
    tree->Branch("inHits_y", &inHits_y);
    tree->Branch("inHits_z", &inHits_z);

    tree->Branch("outHits_x", &outHits_x);
    tree->Branch("outHits_y", &outHits_y);
    tree->Branch("outHits_z", &outHits_z);


    for(unsigned int i = 0; i < events; ++i) {
        Event e(seed);
        e.SetVertix(3);
        e.SetMultiplicity(1);

        const unsigned int mult = e.GetMultiplicity();
        Trajs.clear();
        Trajs.reserve(mult);

        for(unsigned int j = 0; j < mult; ++j) {
            Trajectory t(seed);
            t.SetThetaNPhi();
            t.SetParC();
            Trajs.emplace_back(t);
        }

        vertex_x.emplace_back(e.GetVertix(1));
        vertex_y.emplace_back(e.GetVertix(2));
        vertex_z.emplace_back(e.GetVertix(3));

        inHits_x.clear();
        inHits_y.clear();
        inHits_z.clear();
        outHits_x.clear();
        outHits_y.clear();
        outHits_z.clear();

        for (const auto& traj : Trajs) {
            HitPoint hIn(e, traj, rIn);
            HitPoint hOut(e, traj, rOut);

            hIn.SetPoint(e, traj);
            hOut.SetPoint(e, traj);

            inHits_x.emplace_back(hIn.GetX());
            inHits_y.emplace_back(hIn.GetY());
            inHits_z.emplace_back(hIn.GetZ());

            outHits_x.emplace_back(hOut.GetX());
            outHits_y.emplace_back(hOut.GetY());
            outHits_z.emplace_back(hOut.GetZ());
        }

        tree->Fill();
    }

// No need to clear vectors after loop; they will be destroyed with scope.

    tree->Write();
    cout<<"HERE"<<endl;
    file->Close();

    delete file;

    stopwatch.Stop();
    stopwatch.Print();

}


