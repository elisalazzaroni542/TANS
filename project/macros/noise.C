#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <cmath>


void noise((const char* filename="../data/sim1000000.root", const char* treename="Events")){

    TFile file(filename, "UPDATE");

    TTree* old_tree = (TTree*)file.Get(treename);
    TTree* new_tree = new TTree("Events", "Simulated events");



    new_tree->Branch("vertex", &vertex);
    new_tree->Branch("inHits_x", &inHits_x);
    new_tree->Branch("inHits_y", &inHits_y);
    new_tree->Branch("inHits_z", &inHits_z);
    new_tree->Branch("outHits_x", &outHits_x);
    new_tree->Branch("outHits_y", &outHits_y);
    new_tree->Branch("outHits_z", &outHits_z);

    

}