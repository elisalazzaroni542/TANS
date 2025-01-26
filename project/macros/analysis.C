#include <TGraph.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TStopwatch.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TClonesArray.h>
#include <vector>
#include <cmath>
#include <TMath.h>
#include <Riostream.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include "../headers/myPoint.h"
#include <TProfile.h>


void calculateErrors(int rec, int gen, double& efficiency, double& errorLow, double& errorHigh) {
    if (gen == 0) {
        efficiency = 0;
        errorLow = 0;
        errorHigh = 0;
        return;
    }

    efficiency = static_cast<double>(rec) / gen;
    double error = sqrt(efficiency * (1 - efficiency) / gen);
    errorLow = min(efficiency, error);
    errorHigh = min(1.0 - efficiency, error);
}

void analysis(unsigned int events = 1000000, string distr="custom") {
    TStopwatch stopwatch;
    stopwatch.Start();

    string input = "../data/reco" + to_string(events) + ".root";
    const char* inputFileName = input.c_str();

    string output = "../data/analysis" + to_string(events) + ".root";
    const char* outputFileName = output.c_str();

    TFile* inputFile = new TFile(inputFileName, "READ");
    if (!inputFile || inputFile->IsZombie()) {
        printf("Error: Cannot open input file '%s'\n", inputFileName);
        return;
    }

    const char* treeName = "Events";
    TTree* inputTree = (TTree*)inputFile->Get(treeName);
    if (!inputTree) {
        printf("Error: TTree '%s' not found in file '%s'\n", treeName, inputFileName);
        inputFile->Close();
        delete inputFile;
        return;
    }

    TFile* outputFile = new TFile(outputFileName, "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        printf("Error: Cannot create output file '%s'\n", outputFileName);
        inputFile->Close();
        delete inputFile;
        return;
    }

    myPoint* genVertex = new myPoint();
    myPoint* recoVertex = new myPoint(0., 0., 0., 0);
    unsigned int genMult;

    inputTree->SetBranchAddress("genVertex", &genVertex);
    inputTree->SetBranchAddress("recoVertex", &recoVertex);
    inputTree->SetBranchAddress("genMultiplicity", &genMult);
    
    
    const int nResBins = 300;
    const double ResMin = -0.05;
    const double ResMax = 0.05;
    
    const int nZBins = 50;
    const double zMin = -20; 
    const double zMax = 20;


    const int nZProfileBins = 11;
    const double zProfileMin = -13.5;  
    const double zProfileMax = 13.5;
    
    unsigned int nMultBins=0;
    double MultMin=0; 
    double MultMax=0;
    
    if( distr=="uni"){
      nMultBins = 30;
      MultMin = 0; 
      MultMax = 30;
     
    } else if(distr=="gaus"){
      nMultBins = 40;
      MultMin = 1; 
      MultMax = 41;         
    
    } else { // Custom
      nMultBins = 70;
      MultMin = 1; 
      MultMax = 70;
    
    }
    

    TProfile* profileZDiffGen = new TProfile("profileZDiffGen", 
        "Z Difference vs. Generated Z Position;Z_{gen} [cm];|Z_{reco} - Z_{gen}| [cm]", 
        nZProfileBins, zProfileMin, zProfileMax, "e");

    profileZDiffGen->SetMarkerStyle(20);
    profileZDiffGen->SetMarkerSize(0.8);
    profileZDiffGen->SetMarkerColor(kBlue+2);
    profileZDiffGen->SetLineColor(kBlue+2);    

    TH1D* histRes = new TH1D("Residuals", "Z Residuals;Z Difference (reco - gen) [cm];Entries", nResBins, ResMin, ResMax);
    TH1D* histMultTotal = new TH1D("MultTotal", "Multiplicity Distribution;Multiplicity;Entries", nMultBins, MultMin, MultMax);
    TH1D* histMultValid = new TH1D("MultValid", "Valid Events Multiplicity;Multiplicity;Entries", nMultBins, MultMin, MultMax);  
    TH1D* histZTotal = new TH1D("ZTotal", "Total Vertices vs Z;Z_{gen} [cm];Entries", nZBins, zMin, zMax);
    TH1D* histZValid = new TH1D("ZValid", "Reco Vertices vs Z;Z_{gen} [cm];Entries", nZBins, zMin, zMax);
 
    
    vector<double> mult_bins(nMultBins);
    vector<double> zDiff_means(nMultBins, 0.0);
    vector<double> zDiff_errors(nMultBins, 0.0);
    vector<double> mult_counts(nMultBins, 0.0);
    vector<double> mult_errors(nMultBins);
    
    vector<double> z_positions;
    vector<double> z_efficiencies;
    vector<double> z_errors;
    vector<double> z_xerrors;  // Will be zero
    
    vector<double> valid_mult;
    vector<double> valid_zdiff;
    vector<double> valid_zdiff_errors;
    vector<double> valid_mult_errors;
    
    vector<double> multiplicities;
    vector<double> efficiencies;
    vector<double> errorLows;
    vector<double> errorHighs;
    vector<double> errorLowsX;
    vector<double> errorHighsX;
    
    
    vector<double> zdiff_errors;
    vector<double> zero_xerrors;
    
    
    
    for(unsigned int i = 0; i < nMultBins; ++i) {
        mult_bins[i] = histMultValid->GetBinCenter(i);  // Center of each bin
    }
    
    histRes->SetMarkerColor(kBlue+2);
    histRes->SetLineColor(kBlue+2);
    histMultTotal->SetLineColor(kBlue);
    histMultTotal->SetLineWidth(2);
    histMultValid->SetLineColor(kRed);
    histMultValid->SetLineWidth(2);

    for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
        inputTree->GetEntry(i);
        
        double zGen = genVertex->GetZ();
        
        histMultTotal->Fill(genMult);
        histZTotal->Fill(zGen);

        if (recoVertex->GetId() != -1) {
            double zReco = recoVertex->GetZ();
            double zDiff = zReco - zGen;

            histRes->Fill(zDiff);
            profileZDiffGen->Fill(zGen, fabs(zDiff));
            
            if(genMult < nMultBins) {
                zDiff_means[genMult] += fabs(zDiff);
                mult_counts[genMult] += 1;
            }
            
            zdiff_errors.push_back(histRes->GetRMS());  
            zero_xerrors.push_back(0);  // No error on x-axis
            
            if (fabs(zDiff) < 3 * (histRes->GetStdDev())) {
                histMultValid->Fill(genMult);
                histZValid->Fill(zGen);
            }
        }
    }

   
// EFFICIENCY VS Z POSITION---------------------------------------------------------------------------------------------------------------------
    for (int bin = 1; bin <= nZBins; ++bin) {
        int totalEvents = histZTotal->GetBinContent(bin);
        if (totalEvents > 0) {
            int validEvents = histZValid->GetBinContent(bin);
            double zPos = histZTotal->GetBinCenter(bin);
            double efficiency, errorLow, errorHigh;
            calculateErrors(validEvents, totalEvents, efficiency, errorLow, errorHigh);
            
            z_positions.push_back(zPos);
            z_efficiencies.push_back(efficiency);
            z_errors.push_back(errorHigh); 
            z_xerrors.push_back(0);
        }
    }

    TGraphErrors* graphEffZ = new TGraphErrors(
        z_positions.size(),
        z_positions.data(),
        z_efficiencies.data(),
        z_xerrors.data(),
        z_errors.data()
    );
    
    graphEffZ->SetTitle("Efficiency vs. Z Position;Z_{gen} [cm];Efficiency");
    graphEffZ->SetMarkerStyle(20);
    graphEffZ->SetMarkerSize(0.4);
    graphEffZ->SetMarkerColor(kBlue+2);
    graphEffZ->SetLineColor(kBlue+2);
    
// Z DIFFERENCE VS MULTIPLICITY-------------------------------------------------------------------------------------------------------
    int step=1;
    for(unsigned int i = 0; i < nMultBins; i+=step) {
        if(mult_counts[i] > 0) {
            valid_mult.push_back(mult_bins[i]);
            valid_zdiff.push_back(zDiff_means[i] / mult_counts[i]);
            valid_zdiff_errors.push_back(histRes->GetRMS() / sqrt(mult_counts[i]));
            valid_mult_errors.push_back(sqrt(mult_bins[i]));
            if (distr!="uni"&& i>7){step=7;}
        }
        
    }

    TGraphErrors* graphZDiff = new TGraphErrors(
        valid_mult.size(),
        valid_mult.data(),
        valid_zdiff.data(),
        valid_mult_errors.data(),
        valid_zdiff_errors.data()
    );
    
    graphZDiff->SetTitle("Absolute Z Difference vs. Multiplicity;Multiplicity;Mean |Z Difference| [cm]");
    graphZDiff->SetMarkerStyle(20);
    graphZDiff->SetMarkerSize(0.8);
    graphZDiff->SetMarkerColor(kBlue+2);
    graphZDiff->SetLineColor(kBlue+2);

//EFFICIENCY VS MULTIPLICITY-----------------------------------------------------------------------------------------------------------------------------  
    step=1;
    for (int bin = 1; bin <= histMultTotal->GetNbinsX(); bin+= step) {
        int totalEvents = histMultTotal->GetBinContent(bin);
        if (totalEvents > 0) {  
            int validEvents = histMultValid->GetBinContent(bin);
            double mult = histMultTotal->GetBinCenter(bin);
            double errmult = sqrt(mult);
            double efficiency, errorLow, errorHigh;
            calculateErrors(validEvents, totalEvents, efficiency, errorLow, errorHigh);

            multiplicities.push_back(mult);
            efficiencies.push_back(efficiency);
            errorLows.push_back(errorLow);
            errorHighs.push_back(errorHigh);
            errorLowsX.push_back(errmult);
            errorHighsX.push_back(errmult);
            if (distr!="uni"&&bin>7){step=7;}
        }
    }

    TGraphAsymmErrors* graphEff = new TGraphAsymmErrors(
        multiplicities.size(),
        multiplicities.data(),
        efficiencies.data(),
        errorLowsX.data(),
        errorHighsX.data(),
        errorLows.data(),
        errorHighs.data()
    );

    graphEff->SetTitle("Efficiency vs. Multiplicity;Multiplicity;Efficiency");
    graphEff->SetMarkerStyle(20);
    graphEff->SetMarkerSize(0.5);
    graphEff->SetMarkerColor(kBlue+2);
    graphEff->SetLineColor(kBlue+2);
    
// CANVASES-----------------------------------------------------------------------------------------------------------------------------   
 
    TCanvas* canvasMult = new TCanvas("canvasMult", "Multiplicity Distributions", 1600, 1300);
    canvasMult->SetTicks(1, 1);
    canvasMult->SetGrid();
    TLegend* legend = new TLegend(0.65, 0.75, 0.85, 0.85);
    legend->AddEntry(histMultTotal, "Total vertices", "l");
    legend->AddEntry(histMultValid, "Reco vertices", "l");
    histMultTotal->GetYaxis()->SetMaxDigits(3);
    histMultValid->GetYaxis()->SetMaxDigits(3);
    histMultTotal->Draw("HIST");
    histMultValid->Draw("HIST SAME");
    histMultTotal->SetStats(0);
    histMultValid->SetStats(0);
    legend->Draw();
    canvasMult->SaveAs("../plots/MultiplicityDistributions.png");
    
    TCanvas* canvasHist = new TCanvas("canvasHist", "Z Residuals", 1600, 1300);
    canvasHist->SetTicks(1, 1);
    canvasHist->SetGrid();
    histRes->Draw("E1 P");
    histRes->GetYaxis()->SetMaxDigits(3);
    canvasHist->SaveAs("../plots/ZResiduals.png");   
    
    
// SET RANGES---------------------------------------------------------------------------------------------------------------

  if( distr=="uni"){
    profileZDiffGen->GetYaxis()->SetRangeUser(0.010, 0.016);
    profileZDiffGen->GetYaxis()->SetMaxDigits(2);
    profileZDiffGen->GetYaxis()->SetTitleOffset(1.2);
    graphEff->GetYaxis()->SetRangeUser(0.95, 1.001);
    graphZDiff->GetYaxis()->SetRangeUser(0.009, 0.014);
    graphZDiff->GetYaxis()->SetMaxDigits(2);
    graphEffZ->GetYaxis()->SetRangeUser(0.9, 1.001);
  
  
  } else if (distr=="gaus"){
    profileZDiffGen->GetYaxis()->SetRangeUser(0.010, 0.018);
    profileZDiffGen->GetYaxis()->SetMaxDigits(2);
    profileZDiffGen->GetYaxis()->SetTitleOffset(1.2);
    graphEff->GetYaxis()->SetRangeUser(0.85, 1.001);
    graphZDiff->GetYaxis()->SetRangeUser(0, 0.08);
    graphEffZ->GetYaxis()->SetRangeUser(0.9, 1.001);
  
  } else { // custom
    profileZDiffGen->GetYaxis()->SetRangeUser(0.012, 0.023);
    profileZDiffGen->GetYaxis()->SetMaxDigits(2);
    profileZDiffGen->GetYaxis()->SetTitleOffset(1.2);
    graphEff->GetYaxis()->SetRangeUser(0.84, 1.001);
    graphZDiff->GetYaxis()->SetRangeUser(0, 0.08);
    graphEffZ->GetYaxis()->SetRangeUser(0.84, 1.001);
  
  }
    
    
    
//GENERAL CANVASES-------------------------------------------------------------------------------------------------------------    
    
    TCanvas* canvasZDiffGen = new TCanvas("canvasZDiffGen", "Z Difference vs Generated Z", 1600, 1300);
    canvasZDiffGen->SetTicks(1, 1);
    canvasZDiffGen->SetGrid();
    profileZDiffGen->SetStats(0);
    profileZDiffGen->Draw();
    canvasZDiffGen->SaveAs("../plots/ZDiff_vs_ZGen.png");
    profileZDiffGen->Write();

    TCanvas* canvasGraph = new TCanvas("canvasGraph", "Efficiency vs. Multiplicity", 1600, 1300);
    canvasGraph->SetTicks(1, 1);
    canvasGraph->SetGrid();
    graphEff->Draw("AP");
    canvasGraph->SaveAs("../plots/Efficiency_vs_Multiplicity.png");

    TCanvas* canvasMultZDiff = new TCanvas("canvasMultZDiff", "Absolute Z Difference vs Multiplicity", 1600, 1300);
    canvasMultZDiff->SetTicks(1, 1);
    canvasMultZDiff->SetGrid();
    graphZDiff->Draw("AP");
    canvasMultZDiff->SaveAs("../plots/ZDiff_vs_Multiplicity.png");

    TCanvas* canvasEffZ = new TCanvas("canvasEffZ", "Efficiency vs Z Position", 1600, 1300);
    canvasEffZ->SetTicks(1, 1);
    canvasEffZ->SetGrid();
    graphEffZ->Draw("AP");
    canvasEffZ->SaveAs("../plots/Efficiency_vs_Z.png");
    


    outputFile->cd();
    histRes->Write();
    histMultTotal->Write();
    histMultValid->Write();
    histZTotal->Write();
    histZValid->Write();
    graphEff->Write();
    graphZDiff->Write();
    graphEffZ->Write();
    
    outputFile->Close();
    inputFile->Close();

    delete genVertex;
    delete recoVertex;

    stopwatch.Stop();
    stopwatch.Print();
}

