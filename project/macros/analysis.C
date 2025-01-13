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
    
    
    const int nResBins = 100;
    const double ResMin = -0.5;  // Adjust range based on your data
    const double ResMax =0.5;
    
    const int nMultBins = 50;
    const double MultMin = 0;  // Adjust range based on your data
    const double MultMax = 75;
    
    // Create histogram for zGen efficiency
    const int nZBins = 50;
    const double zMin = -20;  // Adjust range based on your data
    const double zMax = 20;

    // Create histograms for residuals and multiplicities
    TH1D* histRes = new TH1D("Residuals", "Z Residuals;|Z Difference (reco - gen)| [mm];Entries", nResBins, ResMin, ResMax);
    TH1D* histMultTotal = new TH1D("MultTotal", "Multiplicity Distribution;Multiplicity;Entries", nMultBins, MultMin, MultMax);
    TH1D* histMultValid = new TH1D("MultValid", "Valid Events Multiplicity;Multiplicity;Entries", nMultBins, MultMin, MultMax);  
    TH1D* histZTotal = new TH1D("ZTotal", "Total Events vs Z;Z_{gen} [mm];Entries", nZBins, zMin, zMax);
    TH1D* histZValid = new TH1D("ZValid", "Valid Events vs Z;Z_{gen} [mm];Entries", nZBins, zMin, zMax);
    
    histZTotal->Sumw2();
    histZValid->Sumw2();
    histRes->Sumw2();
    histMultTotal->Sumw2();
    histMultValid->Sumw2();
    
    // Create vectors for multiplicity vs z-difference
    vector<double> mult_bins(nMultBins);
    vector<double> zDiff_means(nMultBins, 0.0);
    vector<double> zDiff_errors(nMultBins, 0.0);
    vector<double> mult_counts(nMultBins, 0.0);
    vector<double> mult_errors(nMultBins);
    
     // Create vectors for efficiency vs zGen graph
    vector<double> z_positions;
    vector<double> z_efficiencies;
    vector<double> z_errors;
    vector<double> z_xerrors;  // Will be zero
    
    // Calculate means and errors for z-differences
    vector<double> valid_mult;
    vector<double> valid_zdiff;
    vector<double> valid_zdiff_errors;
    vector<double> valid_mult_errors;
    
    // Create vectors for efficiency graph
    vector<double> multiplicities;
    vector<double> efficiencies;
    vector<double> errorLows;
    vector<double> errorHighs;
    vector<double> errorLowsX;
    vector<double> errorHighsX;
    
    
    // Create vectors for zDiff vs zGen graph
    vector<double> zgen_positions;
    vector<double> zdiff_values;
    vector<double> zdiff_errors;
    vector<double> zero_xerrors;
    
    
    
    // Initialize multiplicity bins
    for(int i = 0; i < nMultBins; ++i) {
        mult_bins[i] = i + 0.5;  // Center of each bin
    }
    
    
    // Set histogram styles
    histMultTotal->SetLineColor(kBlue);
    histMultTotal->SetLineWidth(2);
    histMultValid->SetLineColor(kRed);
    histMultValid->SetLineWidth(2);

    // Process entries in the input tree
    for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
        inputTree->GetEntry(i);
        
        double zGen = genVertex->GetZ();
        
        // Fill total multiplicity and z histograms
        histMultTotal->Fill(genMult);
        histZTotal->Fill(zGen);

        if (recoVertex->GetId() != -1) {
            double zReco = recoVertex->GetZ();
            double zDiff = zReco - zGen;

            histRes->Fill(zDiff);
            
            // Accumulate absolute z-differences for each multiplicity bin
            if(genMult < nMultBins) {
                zDiff_means[genMult] += fabs(zDiff);
                mult_counts[genMult] += 1;
            }
            
            // Add values for zDiff vs zGen graph
            zgen_positions.push_back(zGen);
            zdiff_values.push_back(zDiff);
            zdiff_errors.push_back(histRes->GetRMS());  // Using the same error as other graphs
            zero_xerrors.push_back(0);  // No error on x-axis
            
            // Fill valid histograms if within resolution
            if (fabs(zDiff) < 3 * (histRes->GetStdDev())) {
                histMultValid->Fill(genMult);
                histZValid->Fill(zGen);
            }
        }
    }

   

    // Calculate efficiencies for each z bin
    for (int bin = 1; bin <= nZBins; ++bin) {
        int totalEvents = histZTotal->GetBinContent(bin);
        if (totalEvents > 0) {
            int validEvents = histZValid->GetBinContent(bin);
            double zPos = histZTotal->GetBinCenter(bin);
            double efficiency, errorLow, errorHigh;
            calculateErrors(validEvents, totalEvents, efficiency, errorLow, errorHigh);
            
            z_positions.push_back(zPos);
            z_efficiencies.push_back(efficiency);
            z_errors.push_back(errorHigh);  // Using the high error (could use average of high/low if preferred)
            z_xerrors.push_back(0);  // No error on x-axis as requested
        }
    }

    // Create TGraphErrors for efficiency vs zGen
    TGraphErrors* graphEffZ = new TGraphErrors(
        z_positions.size(),
        z_positions.data(),
        z_efficiencies.data(),
        z_xerrors.data(),
        z_errors.data()
    );
    
    graphEffZ->SetTitle("Efficiency vs. Z Position;Z_{gen} [mm];Efficiency");
    graphEffZ->SetMarkerStyle(20);
    graphEffZ->SetMarkerSize(0.8);
    graphEffZ->SetMarkerColor(kBlue);
    graphEffZ->SetLineColor(kBlue);

    

    for(int i = 0; i < nMultBins; ++i) {
        if(mult_counts[i] > 0) {
            valid_mult.push_back(mult_bins[i]);
            valid_zdiff.push_back(zDiff_means[i] / mult_counts[i]);
            valid_zdiff_errors.push_back(histRes->GetRMS() / sqrt(mult_counts[i]));
            valid_mult_errors.push_back(sqrt(mult_bins[i]));
        }
    }

    // Create TGraphErrors for multiplicity vs z-difference
    TGraphErrors* graphZDiff = new TGraphErrors(
        valid_mult.size(),
        valid_mult.data(),
        valid_zdiff.data(),
        valid_mult_errors.data(),
        valid_zdiff_errors.data()
    );
    
    graphZDiff->SetTitle("Absolute Z Difference vs. Multiplicity;Multiplicity;Mean |Z Difference| [mm]");
    graphZDiff->SetMarkerStyle(20);
    graphZDiff->SetMarkerSize(0.8);
    graphZDiff->SetMarkerColor(kBlue);
    graphZDiff->SetLineColor(kBlue);

    

    // Loop over histogram bins
    for (int bin = 1; bin <= histMultTotal->GetNbinsX(); ++bin) {
        int totalEvents = histMultTotal->GetBinContent(bin);
        if (totalEvents > 0) {  // Only process bins with entries
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
    graphEff->SetMarkerColor(kBlue);
    
    
    // Create TGraphErrors for zDiff vs zGen
    TGraphErrors* graphZDiffGen = new TGraphErrors(
       zgen_positions.size(),
       zgen_positions.data(),
       zdiff_values.data(),
       zero_xerrors.data(),
       zdiff_errors.data()
     );

    graphZDiffGen->SetTitle("Z Difference vs. Generated Z Position;Z_{gen} [mm];Z_{reco} - Z_{gen} [mm]");
    graphZDiffGen->SetMarkerStyle(20);
    graphZDiffGen->SetMarkerSize(0.8);
    graphZDiffGen->SetMarkerColor(kBlue);
    graphZDiffGen->SetLineColor(kBlue);

     // Draw zDiff vs zGen plot
    TCanvas* canvasZDiffGen = new TCanvas("canvasZDiffGen", "Z Difference vs Generated Z", 800, 600);
    graphZDiffGen->Draw("AP");
    canvasZDiffGen->SaveAs("../plots/ZDiff_vs_ZGen.png");

    // Don't forget to write to output file
    graphZDiffGen->Write();

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
    graphEff->GetYaxis()->SetRangeUser(0.7, 1.1);
    canvasGraph->SaveAs("../plots/Efficiency_vs_Multiplicity.png");

    // Draw residuals
    TCanvas* canvasHist = new TCanvas("canvasHist", "Z Residuals", 800, 600);
    histRes->Draw("E1 P");
    canvasHist->SaveAs("../plots/ZResiduals.png");

    // Draw multiplicity vs z-difference plot
    TCanvas* canvasMultZDiff = new TCanvas("canvasMultZDiff", "Absolute Z Difference vs Multiplicity", 800, 600);
    graphZDiff->Draw("AP");
    canvasMultZDiff->SaveAs("../plots/ZDiff_vs_Multiplicity.png");

    // Draw efficiency vs zGen plot
    TCanvas* canvasEffZ = new TCanvas("canvasEffZ", "Efficiency vs Z Position", 800, 600);
    graphEffZ->Draw("AP");
    graphEffZ->GetYaxis()->SetRangeUser(0.7, 1.1);  // Same range as efficiency vs multiplicity
    canvasEffZ->SaveAs("../plots/Efficiency_vs_Z.png");

    // Save everything to output file
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
