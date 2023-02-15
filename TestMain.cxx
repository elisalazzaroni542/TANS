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
    for(int i=0;i<100;i++){
        Event v(seed);
        v.SetVertix(3);
        cX->Fill(v.GetVertix(1));
        cY->Fill(v.GetVertix(2));
        cZ->Fill(v.GetVertix(3));
    }
    TCanvas* cv1=new TCanvas();
    TCanvas* cv2=new TCanvas();
    TCanvas* cv3=new TCanvas();

    cv1->cd();
    cX->Draw();
    cv2->cd();
    cY->Draw();
    cv3->cd();
    cZ->Draw();

}

void TestSetMult(unsigned int seed){
    gRandom->SetSeed(seed);
    TH1F* h1=new TH1F("h1","DIstribuzione molteplicita' da file",100,0,70);
    TH1F* h2=new TH1F("h2","Distribuzione molteplicita' da file",100,0,70);
    for(int i=0;i<100;i++){
        Event e1(seed);
        Event e2(seed);
        e1.SetMultiplicity(2);
        e1.SetVertix();
        e1.PrintEvent();
        e2.SetMultiplicity(2);
        e2.SetVertix();
        e2.PrintEvent();
        h1->Fill(e1.GetMultiplicity());
        h2->Fill(e2.GetMultiplicity());
    } 
    TCanvas* cv1=new TCanvas();
    TCanvas* cv2=new TCanvas();
    TCanvas* cv3=new TCanvas();
    TFile mf("kinem.root");
    TH1F *rif=(TH1F*)mf.Get("hm");
    

    cv1->cd();
    h1->Draw();
    cv2->cd();
    h2->Draw();
    cv3->cd();
    rif->DrawCopy();//usare DrawCopy quando si vuole disegnare qualcosa che proviene da fuori lo scope della funzione. 


}
