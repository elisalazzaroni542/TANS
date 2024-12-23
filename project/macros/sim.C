#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"


gSystem->AddIncludePath("-I../headers");
gSystem->AddDynamicPath("../src");
    
gSystem->Load("Event_cxx.so");
gSystem->Load("HitPoint_cxx.so");
gSystem->Load("Trajectory_cxx.so");

void sim(int seed=123, unsigned const int events=1){

    TStopwatch stopwatch;
    stopwatch.Start();


    std::string filename = "../data/sim" + std::to_string(events) + ".root";  
//    TFile *file = new TFile(filename, "RECREATE");
    TFile *file = new TFile("../data/ciao.root", "RECREATE");

    TTree *tree = new TTree("Events", "Simulated events");

    const unsigned int rIn  = 4;
    const unsigned int rOut = 7;

    vector <Trajectory> Trajs;
    vector <HitPoint> inHits;
    vector <HitPoint> outHits;

    for(unsigned int i=0; i<events; ++i){
        Event e(seed);
        e.SetMultiplicity(1);

        const unsigned int mult = e.GetMultiplicity();

        Trajs.resize(mult);
        Trajectory t(seed);
        for(unsigned int j=1;j<=mult;++j){
            t.SetThetaNPhi();
        //std::cout<<"\nHere\n";
            t.SetParC();
            t.PrintTrajectory();
            Trajs.push_back(t);
        }



        inHits.resize(mult);
        outHits.resize(mult);


        tree->Branch("vertex", &e);
        tree->Branch("inHits", &inHits);
        tree->Branch("outHits", &outHits);


        for(int k=0;k<(int)Trajs.size();++k){
            HitPoint hIn(e,Trajs[k],rIn);
            HitPoint hOut(e,Trajs[k],rOut);

            hIn.SetPoint(e, Trajs[k]);
            hOut.SetPoint(e, Trajs[k]);

            inHits[k] = hIn;
            outHits[k] = hOut;
        }


        tree->Fill();

        Trajs.clear();
        inHits.clear();
        outHits.clear();



    }
    tree->Write();
    file->Close();

    stopwatch.Stop();
    stopwatch.Print();

}






