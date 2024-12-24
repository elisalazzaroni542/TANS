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
    std::string filename = "../data/sim" + std::to_string(events) + ".root";  
//    TFile *file = new TFile(filename, "RECREATE");
    TFile *file = new TFile("../data/ciao.root", "RECREATE");

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


    for(unsigned int i=0; i<events; ++i){
        Event e(seed);
        e.SetVertix(3);
        e.SetMultiplicity(1);

        const unsigned int mult = e.GetMultiplicity();

        cout<<"Molteplicita: "<<mult<<endl;

        

        cout<<"Passed event creation"<<endl;

        Trajs.resize(mult);
        Trajectory t(seed);
        for(unsigned int j=0;j<mult;++j){
            t.SetThetaNPhi();
        //std::cout<<"\nHere\n";
            t.SetParC();
            t.PrintTrajectory();
            Trajs[j] = t;
        }

        inHits_x.resize(mult);
        inHits_y.resize(mult);
        inHits_z.resize(mult);

        outHits_x.resize(mult);
        outHits_y.resize(mult);
        outHits_z.resize(mult);

        cout<<"Passed traj computation"<<endl;
        vertex_x.push_back(e.GetVertix(1));
        vertex_y.push_back(e.GetVertix(2));
        vertex_z.push_back(e.GetVertix(3));

        cout<<"Vertex branch"<<endl;

        cout<<"Trajs.size() = "<<(int)Trajs.size()<<endl;

        cout<<"Passed tree branch declaration"<<endl;
        for(int k=0;k<(int)Trajs.size();++k){
            HitPoint hIn(e,Trajs[k],rIn);
            HitPoint hOut(e,Trajs[k],rOut);

            hIn.SetPoint(e, Trajs[k]);
            hOut.SetPoint(e, Trajs[k]);

            inHits_x[k] = hIn.GetX();
            inHits_y[k] = hIn.GetY(); 
            inHits_z[k] = hIn.GetZ();

            outHits_x[k] = hOut.GetX();
            outHits_y[k] = hOut.GetY();
            outHits_z[k] = hOut.GetZ();        
        }

        cout<<"Passed hit point computation"<<endl;

        tree->Fill();

        Trajs.clear();
        vertex_x.clear();
        vertex_y.clear();
        vertex_z.clear();
        inHits_x.clear();
        inHits_y.clear();
        inHits_z.clear();
        outHits_x.clear();
        outHits_y.clear();
        outHits_z.clear();

    }
    tree->Write();
    file->Close();

    stopwatch.Stop();
    stopwatch.Print();

}


