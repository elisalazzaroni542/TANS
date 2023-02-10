#include "RndmPick.h"
#include <Riostream.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <math.h>


void TestVertDistr (unsigned int seed){
    gRandom->SetSeed(seed);
    TH1F* cX=new TH1F("cX","Distribuzione coordinata X",100,-5,5);
    TH1F* cY=new TH1F("cY","Distribuzione coordinata Y",100,-5,5);
    TH1F* cZ=new TH1F("cZ","Distribuzione coordinata Z",100,-10,10);
    for(int i=0;i<100;i++){
        RndmPick v;
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