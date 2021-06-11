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
#include "TGRSIOptions.h"


/************************************************************//**
 * Creates and Fills histograms
 *
 * @param verbose Verbosity level
 ***************************************************************/
void EventManager::ConvertRootData(TChain *input_chain)
{
    int verbose = 0;

    // Turn off crosstalk corrections
    TGRSIOptions::AnalysisOptions()->SetCorrectCrossTalk(false);
    // Parses Root tree and returns csv file containing event information
    GetEventData(input_chain);

} // GenerateHistogramFile()

/************************************************************//**
 * Extracts event information from AnalysisTree
 *
 * @param gChain Data chain
 ***************************************************************/
void EventManager::GetEventData(TChain *gChain)
{

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
    std::string csv_filename = "data.csv";
    std::ofstream csv_file(csv_filename);
    csv_file << "event,multiplicity,charge,energy,time,crystal,x,y,z,k_value" << std::endl;

    for (auto i = 0; i < analysis_entries; i++) {
        gChain->GetEntry(i);

        // Build event vectors and apply any transformations
        PreProcessData();
        int multiplicity = energy_vec.size();
        for (int n = 0; n < multiplicity; n++) {
            csv_file << i << "," <<
                multiplicity << "," <<
                charge_vec.at(n) << "," <<
                energy_vec.at(n) << "," <<
                time_vec.at(n) << "," <<
                det_vec.at(n) << "," <<
                pos_vec.at(n).X() << "," <<
                pos_vec.at(n).Y() << "," <<
                pos_vec.at(n).Z() << "," <<
                kvalue_vec.at(n) <<
                std::endl;
        }

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
    std::cout << "Event data written to file: " << csv_filename << std::endl;
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
