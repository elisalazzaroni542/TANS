#include "RndmPick.h"
#include <Riostream.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <math.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <vector>


void TestVertDistr (unsigned int seed){
    TStopwatch timer;
    timer.Start();
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

    timer.Stop();
    timer.Print();

}

void TestSetMult(unsigned int seed){
    TStopwatch timer;
    timer.Start();
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

    timer.Stop();
    timer.Print(); 


}

void TestRndmUni(unsigned int seed){
    TStopwatch timer;
    timer.Start();
    gRandom->SetSeed(seed);
    TH1F* h1=new TH1F("h1","DIstribuzione numeri estratti con RndmUni event",100,-1,7);
    TH1F* h2=new TH1F("h2","DIstribuzione numeri estratti con RndmUni trajectory",100,-1,7);
    for(int i=0;i<10000;i++){
        Event e1(seed);
        Trajectory t1(seed);
        h1->Fill(e1.RndmUni(0,2*M_PI));
        h2->Fill(t1.RndmUni(0,2*M_PI));

    }
    
    TCanvas* cv1=new TCanvas();
    TCanvas* cv2=new TCanvas();
    cv1->cd();
    h1->Draw();
    cv2->cd();
    h2->Draw();
    
    timer.Stop();
    timer.Print();
} 

//-------------------------------------------------------------------------MAIN------------------------------------------------------



  double CalculateDelta(Event &e, Trajectory &t, double Rcil){
    double Delta=pow(e.GetVertix[1]*t.GetParC[1]+e.GetVertix[2]*t.GetParC[2],2)-(pow(t.GetParC[1],2)+pow(t.GetParC[2],2))*(pow(e.GetVertix[1],2)+pow(e.GetVertix[2],2)-pow(Rcil,2));
    cout<<"Delta= "<<Delta<<endl;
    return Delta;
  }

  double CalculateParT(Event &e, Trajectory &t, double Rcil){
    double parTp=(-(e.GetVertix[1]*t.GetParC[1]+e.GetVertix[2]*t.GetParC[2])+sqrt(CalculateDelta(e,t,Rcil)))/(pow(t.GetParC[1],2)+pow(t.GetParC[2],2));
    double parTm=(-(e.GetVertix[1]*t.GetParC[1]+e.GetVertix[2]*t.GetParC[2])-sqrt(CalculateDelta(e,t,Rcil)))/(pow(t.GetParC[1],2)+pow(t.GetParC[2],2));
    if(parTp>=0){
        cout<<"parTp= "<<parTp<<endl;
        return parTp;} 
    else if (parTm>=0) {
        cout<<"parTm= "<<parTm<<endl;
        return parTm;}
    else {cout<<"t always negative"<<endl;
         return 0;
    } 
  }

int main(){
    //creiamo un evento:
    unsigned int seed=123;
    gRandom->SetSeed(seed);
    Event e(seed);
    e.SetVertix();
    e.SetMultiplicity(1);
    e.PrintEvent();
    //associamo al vertice le sue traiettorie
    vector <Trajectory> Trajs;
    Trajs.reserve(5);
    for(int i=0;i<e.GetMultiplicity();i++){
        Trajectory t(seed);
        t.SetThetaNPhi();
        t.SetParC();
        t.PrintTrajectory();
        Trajs.push_back(t);
    }
    //stampiamo le verie traiettorie salvate nel vector
    for(int j=0;j<Trajs.size();j++){
        Trajs[j].PrintTrajectory();

    }
    //calcoliamo Delta e t per ogni traiettoria
    for(int k=0;k<Trajs.size();k++){
        //raggio del berillio
        double Rcil=3.08;
        CalculateDelta(e,Trajs[k],Rcil);
        CalculateParT(e,Trajs[k],Rcil);

    }
    delete Trajs;
    return 0;


}

