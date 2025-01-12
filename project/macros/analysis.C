#include <TGraph.h>
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
#include "../headers/Point.h"

void calculateErrors(int rec, int gen, double& efficiency, double& errorLow, double& errorHigh) {
    if (gen == 0) {
        efficiency = 0;
        errorLow = 0;
        errorHigh = 0;
        return;
    }

    efficiency = static_cast<double>(rec) / gen;
    double error = sqrt(efficiency * (1 - efficiency) / gen);
    errorLow = std::min(efficiency, error);
    errorHigh = std::min(1.0 - efficiency, error);
}

void analysis(unsigned int events = 1000000) {
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

    Point* genVertex = new Point();
    Point* recoVertex = new Point(0., 0., 0., 0);
    unsigned int genMult;

    inputTree->SetBranchAddress("genVertex", &genVertex);
    inputTree->SetBranchAddress("recoVertex", &recoVertex);
    inputTree->SetBranchAddress("genMultiplicity", &genMult);

    // Create histograms for residuals and multiplicities
    TH1D* histRes = new TH1D("Residuals", "Z Residuals;Z Difference (reco - gen) [mm];Entries", 100, -0.5, 0.5);
    TH1D* histMultTotal = new TH1D("MultTotal", "Multiplicity Distribution;Multiplicity;Entries", 75, 0, 75);
    TH1D* histMultValid = new TH1D("MultValid", "Valid Events Multiplicity;Multiplicity;Entries", 75, 0, 75);
    
    histRes->Sumw2();
    histMultTotal->Sumw2();
    histMultValid->Sumw2();

    // Set histogram styles
    histMultTotal->SetLineColor(kBlue);
    histMultTotal->SetLineWidth(2);
    histMultValid->SetLineColor(kRed);
    histMultValid->SetLineWidth(2);

    // Process entries in the input tree
    for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
        inputTree->GetEntry(i);
        
        // Fill total multiplicity histogram
        histMultTotal->Fill(genMult);

        if (recoVertex->GetId() != -1) {
            double zGen = genVertex->GetZ();
            double zReco = recoVertex->GetZ();
            double zDiff = zReco - zGen;

            histRes->Fill(zDiff);
            
            // Fill valid multiplicity histogram if within resolution
            if (fabs(zDiff) < 3 * (histRes->GetStdDev())) {
                histMultValid->Fill(genMult);
            }
        }
    }

    // Calculate standard deviation of total multiplicity
    

    // Create vectors for efficiency graph
    vector<double> multiplicities;
    vector<double> efficiencies;
    vector<double> errorLows;
    vector<double> errorHighs;
    vector<double> errorLowsX;
    vector<double> errorHighsX;

    // Loop over histogram bins
    for (int bin = 1; bin <= histMultTotal->GetNbinsX(); ++bin) {
        int totalEvents = histMultTotal->GetBinContent(bin);
        if (totalEvents > 0) {  // Only process bins with entries
            int validEvents = histMultValid->GetBinContent(bin);
            double mult = histMultTotal->GetBinCenter(bin);
            double errmult= sqrt(mult);
            double efficiency, errorLow, errorHigh;
            calculateErrors(validEvents, totalEvents, efficiency, errorLow, errorHigh);

            multiplicities.push_back(mult);
            efficiencies.push_back(efficiency);
            errorLows.push_back(errorLow);
            errorHighs.push_back(errorHigh);
            errorLowsX.push_back(errmult);
            errorHighsX.push_back(errmult);
        }
    }

    TGraphAsymmErrors* graphEff = new TGraphAsymmErrors(
        multiplicities.size(),
        multiplicities.data(),
        efficiencies.data(),
        errorLowsX.data(),   // X low errors
        errorHighsX.data(),   // X high errors
        errorLows.data(),
        errorHighs.data()
    );

    graphEff->SetTitle("Efficiency vs. Multiplicity;Multiplicity;Efficiency");
    graphEff->SetMarkerStyle(20);
    graphEff->SetMarkerSize(0.5);
    graphEff->SetMarkerColor(kBlue);

    // Draw multiplicity distributions together
    TCanvas* canvasMult = new TCanvas("canvasMult", "Multiplicity Distributions", 800, 600);
    
    // Create legend
    TLegend* legend = new TLegend(0.65, 0.75, 0.85, 0.85);
    legend->AddEntry(histMultTotal, "Total Events", "l");
    legend->AddEntry(histMultValid, "Valid Events", "l");
    
    // Draw histograms
    histMultTotal->Draw("HIST");
    histMultValid->Draw("HIST SAME");
    legend->Draw();
    
    canvasMult->SaveAs("../plots/MultiplicityDistributions.png");

    // Draw efficiency plot
    TCanvas* canvasGraph = new TCanvas("canvasGraph", "Efficiency vs. Multiplicity", 800, 600);
    graphEff->Draw("AP");
    graphEff->GetYaxis()->SetRangeUser(0.7, 1.5);
    canvasGraph->SaveAs("../plots/Efficiency_vs_Multiplicity.png");

    // Draw residuals
    TCanvas* canvasHist = new TCanvas("canvasHist", "Z Residuals", 800, 600);
    histRes->Draw("E1 P");
    canvasHist->SaveAs("../plots/ZResiduals.png");

    // Save everything to output file
    outputFile->cd();
    histRes->Write();
    histMultTotal->Write();
    histMultValid->Write();
    graphEff->Write();
    
    outputFile->Close();
    inputFile->Close();

    delete genVertex;
    delete recoVertex;

    stopwatch.Stop();
    stopwatch.Print();
}
