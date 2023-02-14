#include "RndmPick.h"
#include <Riostream.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <math.h>
#include <TFile.h>


void TestVertDistr (unsigned int seed){
    gRandom->SetSeed(seed);
    TH1F* cX=new TH1F("cX","Distribuzione coordinata X",100,-1,1);
    TH1F* cY=new TH1F("cY","Distribuzione coordinata Y",100,-1,1);
    TH1F* cZ=new TH1F("cZ","Distribuzione coordinata Z",100,-10,10);
    TH1F* hist=new TH1F("hist","Distr coord Z",100,-10,10);
    TFile f("histos.root","new");
    for(int i=0;i<100;i++){
        RndmPick v(seed);
        v.SetVertix(3);
        cX->Fill(v.GetVertix(1));
        cY->Fill(v.GetVertix(2));
        cZ->Fill(v.GetVertix(3));
        hist->Fill(v.GetVertix(3));
    }
    TCanvas* cv1=new TCanvas();
    TCanvas* cv2=new TCanvas();
    TCanvas* cv3=new TCanvas();
    TCanvas* cv4=new TCanvas();

    cv1->cd();
    cX->Draw();
    cv2->cd();
    cY->Draw();
    cv3->cd();
    cZ->Draw();
    cv4->cd();
    hist->Draw();
    hist->Write();

}

void TestSetMult(unsigned int seed){
    gRandom->SetSeed(seed);
    TH1F* h1=new TH1F("h1","DIstribuzione molteplicità gaussiana",100,-100,100);
    TH1F* h2=new TH1F("h2","Distribuzione molteplicità uniforme",100,-50,50);
    for(int i=0;i<10;i++){
        Event e1(seed);
        Event e2(seed);
        e1.SetMultiplicity(0);
        e1.SetVertix(3);
        e2.SetMultiplicity(1);
        e2.SetVertix(3);
        h1->Fill(e1.GetMultiplicity());
        h2->Fill(e2.GetMultiplicity());
    } 
    TCanvas* cv1=new TCanvas();
    TCanvas* cv2=new TCanvas(); 

    cv1->cd();
    h1->Draw();
    cv2->cd();
    h2->Draw(); 


}
