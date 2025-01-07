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
    std::vector<double> vertex(3);
    TClonesArray inHits("Point", 100);  
    TClonesArray outHits("Point", 100); 

    const int autoSaveSize = 100000;

    string filename = "../data/sim" + to_string(events) + ".root";
    TFile* file = TFile::Open(filename.c_str(), "RECREATE");
    if (!file || file->IsZombie()) {
        printf("Error: Cannot open input file '%s'\n", filename.c_str());
        delete file;
        return;
    }


    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(autoSaveSize);

    tree->Branch("vertex", &vertex);
    tree->Branch("inHits", &inHits);
    tree->Branch("outHits", &outHits);
    tree->Branch("multiplicity", &m);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn, hOut;
    //Point *pIn, pOut;

    TH1F* thetaHist = t.LoadDistribution("heta2");
    
    if (!MS){
      for (unsigned int i = 0; i < events; ++i) {
          e.SetVertex();
          e.SetMultiplicity("custom");
          m = e.GetMultiplicity();
          vertex[0] = e.GetVertex(0);
          vertex[1] = e.GetVertex(1);
          vertex[2] = e.GetVertex(2);
  
          inHits.Clear();
          outHits.Clear();
          
  
          for (unsigned int j = 0; j < m; ++j) {
              t.SetThetaNPhi(thetaHist);
              t.SetParC();
  
              hIn.SetDelta_and_T(e, t, 4.1);
              hOut.SetDelta_and_T(e, t, 7.1);
   
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
          e.SetVertex();
          e.SetMultiplicity("custom");
          m = e.GetMultiplicity();
          vertex[0] = e.GetVertex(0);
          vertex[1] = e.GetVertex(1);
          vertex[2] = e.GetVertex(2);
  
          inHits.Clear();
          outHits.Clear();
  
          for (unsigned int j = 0; j < m; ++j) {
              //cout<<"------------------------------------------------------------------------Event "<<i<<"---------------------------------------------------------------------------------------------"<<endl;
              t.SetThetaNPhi(thetaHist);
              t.SetParC();
  
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
 

/*
void sim(int seed = 123, unsigned const int events = 1000000, bool MS=false) {
    TStopwatch stopwatch;
    stopwatch.Start();

    unsigned int m;
    vector<double> Vertex(3);
    TClonesArray inHits("Point", 100);  
    TClonesArray outHits("Point", 100); 

    const int autoSaveSize = 100000;

    string fIn = "../data/sim" + to_string(events) + ".root";
    //const char* inputFile = fIn.c_str();
    TFile* file = TFile::Open(fIn.c_str(), "RECREATE");
    if (!file || file->IsZombie()) {
        printf("Error: Cannot open input file '%s'\n", fIn.c_str());
        delete file;
        return;
    }


    TTree* tree = new TTree("Events", "Simulated events");
    tree->SetAutoSave(autoSaveSize);

    tree->Branch("Vertex", &Vertex, 3200, 0);
    tree->Branch("inHits", &inHits, 3200, 0);
    tree->Branch("outHits", &outHits, 3200, 0);
    tree->Branch("multiplicity", &m, 3200, 0);

    Event e(seed);
    Trajectory t(seed);
    HitPoint hIn, hOut, hB;
    //Point *pIn, pOut;

    TH1F* thetaHist = t.LoadDistribution("heta2");
    
      for (unsigned int i = 0; i < events; ++i) {
          e.SetVertex();
          e.SetMultiplicity("custom");
          m = e.GetMultiplicity();
          Vertex[0] = e.GetVertex(0);
          Vertex[1] = e.GetVertex(1);
          Vertex[2] = e.GetVertex(2);
  
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
  
              hB.SetDelta_and_T(e, t, 3);//Hit against beam pipe
              //t.PrintTrajectory();
              hB.SetPoint(e, t);
              //hB.PrintHit();
              t.MSRotateParC(t);//first rotation
            
              hIn.MSSetDelta_and_T(hB, t, 4);//hit layer 1
              //t.PrintTrajectory();
              hIn.MSSetPoint(hB, t);
              //hIn.PrintHit();
              t.MSRotateParC(t);//second rotation

              hOut.MSSetDelta_and_T(hIn, t, 7);//hit layer 2
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
*/

