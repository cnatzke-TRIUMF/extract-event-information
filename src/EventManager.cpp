//////////////////////////////////////////////////////////////////////////////////
// Creates and fills histograms
//
// Author:          Connor Natzke (cnatzke@triumf.ca)
// Creation Date: Wednesday July 29, 2020	T15:22:33-07:00
// Last Update:   Wednesday July 29, 2020	T15:22:45-07:00
// Usage:
//
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "EventManager.h"
#include "progress_bar.h"
#include "LoadingMessenger.h"


/************************************************************//**
 * Creates and Fills histograms
 *
 * @param verbose Verbosity level
 ***************************************************************/
void EventManager::MakeHistograms(TChain *input_chain)
{
    int verbose = 0;

    // create histograms
    InitializeHistograms(0);
    FillHistograms(input_chain);

} // GenerateHistogramFile()

/************************************************************//**
 * Initializes histograms to be filled
 *
 * @param verbose Verbosity level
 ***************************************************************/
void EventManager::InitializeHistograms(int verbose)
{
    int num_bins = 4000;
    int bin_min = 0;
    int bin_max = 4000;

    if (verbose > 0 ) std::cout << "Initializing 1D histograms ... " << std::endl;

    // 1D Histograms
    hist_vec_1D.push_back(new TH1D("delta_time", "#Delta t", 2000, 0, 2000));
    hist_vec_1D.push_back(new TH1D("k_value", "K Value", 1000, 0, 1000));
    hist_vec_1D.push_back(new TH1D("energy", ";#gamma [keV];Counts / keV", num_bins, bin_min, bin_max));

    if (verbose > 0 ) std::cout << "Creating 1D histograms ... [DONE]" << std::endl;

    // 2D Histograms
    if (verbose > 0 ) std::cout << "Creating 2D histograms ... " << std::endl;
    TH2D *chan_hist = new TH2D("energy_channel", "", 70, 0, 70, num_bins, bin_min, bin_max);

    // adding 'total' histograms to back of vector
    hist_vec_2D.push_back(chan_hist);

    if (verbose > 0 ) std::cout << "Initializing 2D histograms ... [DONE]" << std::endl;
} // InitializeHistograms()

/************************************************************//**
 * Fills histograms
 *
 * @param gChain Data chain
 ***************************************************************/
void EventManager::FillHistograms(TChain *gChain)
{

    int prompt_time_max = 30;     // ns
    int time_random_min = 1000;     // ns
    int time_random_max = 2000;     // ns
    //float bg_subtraction_factor = 0;
    float bg_subtraction_factor = -1.0 * (prompt_time_max) / (time_random_max - time_random_min);

    if (gChain->FindBranch("TGriffin")) {
        gChain->SetBranchAddress("TGriffin", &fGrif);
        if (fGrif != NULL) {
            std::cout << "Succesfully found TGriffin branch" << std::endl;
        } else {
            std::cout << "Could not find TGriffin branch ... exiting" << std::endl;
        }
    }

    //DisplayLoadingMessage();
    LoadingMessenger load_man;
    load_man.DisplayLoadingMessage();

    long analysis_entries = gChain->GetEntries();
    ProgressBar progress_bar(analysis_entries, 70, '=', ' ');

    // initialize csv output
    std::ofstream csv_file("data.csv");
    csv_file << "event,charge,energy,time,crystal" << std::endl;

    for (auto i = 0; i < analysis_entries; i++) {
        gChain->GetEntry(i);

        // Applies secondary energy calculation
        PreProcessData();
        int multiplicity = energy_vec.size();
        for (int n = 0; n < multiplicity; n++){
            csv_file << i << "," << charge_vec.at(n) << "," << energy_vec.at(n) << "," << time_vec.at(n) << "," << det_vec.at(n) << std::endl;//<< "," << pos_vec.at(n) << std::endl;
        }

        /*
        // Filling histograms
        if (energy_vec.size() > 0) {
            for (unsigned int g1 = 0; g1 < energy_vec.size(); ++g1) {
                hist_vec_1D.at(1)->Fill(kvalue_vec.at(g1));
                hist_vec_1D.back()->Fill(energy_vec.at(g1));
                hist_vec_2D.at(0)->Fill(det_vec.at(g1), energy_vec.at(g1));
                for(unsigned int g2 = g1 + 1; g2 < energy_vec.size(); ++g2) {
                    if (g1 == g2) continue;
                    // Timing information
                    double delta_t = TMath::Abs(time_vec.at(g1) - time_vec.at(g2));
                    hist_vec_1D.at(0)->Fill(delta_t);
                } // grif2
            } // grif1
        } // end energy_vec
        */

        if (i % 10000 == 0) {
            progress_bar.display();
        }
        ++progress_bar;         // iterates progress_bar

        // cleaning up for next event
        energy_vec.clear();
        charge_vec.clear();
        pos_vec.clear();
        time_vec.clear();
        kvalue_vec.clear();
        det_vec.clear();

    }     // end TChain loop
    progress_bar.done();
    csv_file.close();
} // FillHistograms()

/************************************************************//**
 * Applies linear calibration to data points
 *
 ***************************************************************/
void EventManager::PreProcessData()
{
    float energy_temp = 0;
    int det_id = -1;

    for (auto j = 0; j < fGrif->GetSuppressedMultiplicity(fGriffinBgo); ++j) {
        det_id = fGrif->GetSuppressedHit(j)->GetArrayNumber();
        if (det_id == -1) {
            std::cout << "BAD DETECTOR" << std::endl;
            continue;
        }
        //if(fGrif->GetSuppressedHit(j)->GetKValue()!=700) {continue;} // removes GRIFFIN hits pileup events

        // No secondary calibration
        energy_temp = fGrif->GetSuppressedHit(j)->GetEnergy();


        energy_vec.push_back(energy_temp);
        charge_vec.push_back(fGrif->GetSuppressedHit(j)->GetCharge());
        pos_vec.push_back(fGrif->GetSuppressedHit(j)->GetPosition(145.0));
        time_vec.push_back(fGrif->GetSuppressedHit(j)->GetTime());
        kvalue_vec.push_back(fGrif->GetSuppressedHit(j)->GetKValue());
        det_vec.push_back(det_id);
    }
} // PreProcessData

/************************************************************//**
 * Opens Root files
 *
 * @param file_name Analysis file path
 ***************************************************************/
void EventManager::WriteHistogramsToFile()
{
    TFile *out_file = new TFile("histograms.root", "RECREATE");
    std::cout << "Writing histograms to file: " << out_file->GetName() << std::endl;

    out_file->cd();
    for (auto &h : hist_vec_1D) {
        h->Write();
    }
    for (auto &h : hist_vec_2D) {
        h->Write();
    }
    out_file->Close();

} // WriteHistogramsToFile()
