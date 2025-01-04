#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TClonesArray.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"
#include "../headers/Point.h"


void sim(int seed = 123, unsigned const int events = 1000000, bool MS=false) {
    TStopwatch stopwatch;
    stopwatch.Start();

    unsigned int m;
    vector<double> vertex(3);
    TClonesArray inHits("Point", 100);  
    TClonesArray outHits("Point", 100); 

    const int autoSaveSize = 100000;

    string filename = "../data/sim" + to_string(events) + ".root";
    TFile* file = TFile::Open(filename.c_str(), "RECREATE");

    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(autoSaveSize);

    tree->Branch("vertex", &vertex, 3200, 0);
    tree->Branch("inHits", &inHits, 3200, 0);
    tree->Branch("outHits", &outHits, 3200, 0);
    tree->Branch("multiplicity", &m, 3200, 0);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn, hOut;
    //Point *pIn, pOut;

    TH1F* thetaHist = t.LoadDistribution("heta2");
    
      for (unsigned int i = 0; i < events; ++i) {
          e.SetVertix(3);
          e.SetMultiplicity("custom");
          m = e.GetMultiplicity();
          vertex[0] = e.GetVertix(0);
          vertex[1] = e.GetVertix(1);
          vertex[2] = e.GetVertix(2);
  
          inHits.Clear();
          outHits.Clear();
          
  
          for (unsigned int j = 0; j < m; ++j) {
              t.SetThetaNPhi(thetaHist);
              t.SetParC();
  
            if (!MS){
              hIn.SetDelta_and_T(e, t, 4);
              hOut.SetDelta_and_T(e, t, 7);
   
              hIn.SetPoint(e, t);
              hOut.SetPoint(e, t);
            } else {
  
              HitPoint hB;
              hB.SetDelta_and_T(e, t, 3.1);//Hit against beam pipe
              //t.PrintTrajectory();
              hB.SetPoint(e, t);
              //hB.PrintHit();
              t.MSRotateParC(t);//first rotation
            
              hIn.MSSetDelta_and_T(hB, t, 4.1);//hit layer 1
              //t.PrintTrajectory();
              hIn.MSSetPoint(hB, t);
              //hIn.PrintHit();
              t.MSRotateParC(t);//second rotation

              hOut.MSSetDelta_and_T(hIn, t, 7.1);//hit layer 2
              //t.PrintTrajectory();  
              hOut.MSSetPoint(hIn, t);
              //hOut.PrintHit();

            }
              
            if (abs(hIn.GetZ()) < 13.5) {
                Point* pIn = new (inHits[inHits.GetEntriesFast()]) Point(hIn.GetX(), hIn.GetY(), hIn.GetZ(), i);

                if (abs(hOut.GetZ()) < 13.5) {
                    Point* pOut = new (outHits[outHits.GetEntriesFast()]) Point(hOut.GetX(), hOut.GetY(), hOut.GetZ(), i);
                }


            }


          }
  
          tree->Fill();
      }
  
      file->Write();
      file->Close();
      delete file;
  
      stopwatch.Stop();
      stopwatch.Print();
}



/*
void sim(int seed = 123, unsigned const int events = 1000000, bool MS=false) {
    TStopwatch stopwatch;
    stopwatch.Start();

    unsigned int m;
    std::vector<double> vertex(3);
    TClonesArray inHits("Point", 100);  
    TClonesArray outHits("Point", 100); 

    const int autoSaveSize = 100000;

    string filename = "../data/sim" + to_string(events) + ".root";
    TFile* file = TFile::Open(filename.c_str(), "RECREATE");

    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(autoSaveSize);

    tree->Branch("vertex", &vertex, 3200, 0);
    tree->Branch("inHits", &inHits, 3200, 0);
    tree->Branch("outHits", &outHits, 3200, 0);
    tree->Branch("multiplicity", &m, 3200, 0);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn, hOut;
    //Point *pIn, pOut;

    TH1F* thetaHist = t.LoadDistribution("heta2");
    
    if (!MS){
      for (unsigned int i = 0; i < events; ++i) {
          e.SetVertix(3);
          e.SetMultiplicity("custom");
          m = e.GetMultiplicity();
          vertex[0] = e.GetVertix(0);
          vertex[1] = e.GetVertix(1);
          vertex[2] = e.GetVertix(2);
  
          inHits.Clear();
          outHits.Clear();
          
  
          for (unsigned int j = 0; j < m; ++j) {
              t.SetThetaNPhi(thetaHist);
              t.SetParC(3);
  
              hIn.SetDelta_and_T(e, t, 4);
              hOut.SetDelta_and_T(e, t, 7);
   
              hIn.SetPoint(e, t);
              hOut.SetPoint(e, t);
  
              if (abs(hIn.GetZ()) < 13.5) {
                  Point* pIn = new (inHits[inHits.GetEntriesFast()]) Point(hIn.GetX(), hIn.GetY(), hIn.GetZ(), i);
              }
              if (abs(hOut.GetZ()) < 13.5) {
                  Point* pOut = new (outHits[outHits.GetEntriesFast()]) Point(hOut.GetX(), hOut.GetY(), hOut.GetZ(), i);
              }
          }
  
          tree->Fill();
      }
  
      file->Write();
      file->Close();
      delete file;
  
      stopwatch.Stop();
      stopwatch.Print();
    }
    else{  
        HitPoint hB;
              
        for (unsigned int i = 0; i < events; ++i) {
          e.SetVertix(3);
          e.SetMultiplicity("custom");
          m = e.GetMultiplicity();
          vertex[0] = e.GetVertix(0);
          vertex[1] = e.GetVertix(1);
          vertex[2] = e.GetVertix(2);
  
          inHits.Clear();
          outHits.Clear();
  
          for (unsigned int j = 0; j < m; ++j) {
              //cout<<"------------------------------------------------------------------------Event "<<i<<"---------------------------------------------------------------------------------------------"<<endl;
              t.SetThetaNPhi(thetaHist);
              t.SetParC(3);
  
              hB.SetDelta_and_T(e, t, 3.1);//Hit against beam pipe
              //t.PrintTrajectory();
              hB.SetPoint(e, t);
              //hB.PrintHit();
              t.MSRotateParC(t);//first rotation
            
              hIn.MSSetDelta_and_T(hB, t, 4.1);//hit layer 1
              //t.PrintTrajectory();
              hIn.MSSetPoint(hB, t);
              //hIn.PrintHit();
              t.MSRotateParC(t);//second rotation

              hOut.MSSetDelta_and_T(hIn, t, 7.1);//hit layer 2
              //t.PrintTrajectory();  
              hOut.MSSetPoint(hIn, t);
              //hOut.PrintHit();
  
              if (abs(hIn.GetZ()) < 13.5) {
                  Point* pIn = new (inHits[inHits.GetEntriesFast()]) Point(hIn.GetX(), hIn.GetY(), hIn.GetZ(), i);
              }
              if (abs(hOut.GetZ()) < 13.5) {
                  Point* pOut = new (outHits[outHits.GetEntriesFast()]) Point(hOut.GetX(), hOut.GetY(), hOut.GetZ(), i);
              }
          }
  
          tree->Fill();
      }
  
      file->Write();
      file->Close();
      delete file;
  
      stopwatch.Stop();
      stopwatch.Print();
    }
}

*/