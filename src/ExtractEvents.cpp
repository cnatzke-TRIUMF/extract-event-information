//////////////////////////////////////////////////////////////////////////////////
// Creates histograms used in efficiency plot creation
//
// Author:        Connor Natzke (cnatzke@triumf.ca)
// Creation Date: Wednesday July 29, 2020 T15:22:33-07:00
// Last Update:   Wednesday July 29, 2020 T15:22:45-07:00
// Usage:
//
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TFile.h" // needed for GetRunNumber
#include "TGRSIUtilities.h"
#include "TParserLibrary.h"
#include "TEnv.h"

#include "EventManager.h"
#include "ExtractEvents.h"

int main(int argc, char **argv)
{

    if (argc == 1) { // no inputs given
        PrintUsage(argv);
        return 0;
    }

    // makes time retrival happy and loads GRSIEnv
    std::string grsi_path = getenv("GRSISYS");
    if(grsi_path.length() > 0) {
        grsi_path += "/";
    }
    grsi_path += ".grsirc";
    gEnv->ReadFile(grsi_path.c_str(), kEnvChange);

    TParserLibrary::Get()->Load();

    for (auto i = 1; i < argc; i++) AutoFileDetect(argv[i]);

    if (!gChain) std::cout << "No gChain found" << std::endl;
    if (!gChain->GetEntries()) std::cout << "Found gChain, but no entries retrieved" << std::endl;

    if (!gChain || !gChain->GetEntries()) {
        std::cerr << "Failed to find anything. Exiting" << std::endl;
        return 1;
    }

    EventManager event_man;
    std::string fName = gChain->GetCurrentFile()->GetName();
    int run_number = GetRunNumber(fName.c_str());

    std::cout << "Processing run " << run_number << " with " << gChain->GetNtrees() << " file(s)" << std::endl;

    // Fill histograms
    event_man.ConvertRootData(gChain);

    // Write histograms to file
    //hist_man.WriteHistogramsToFile();

    return 0;
} // main()

/************************************************************//**
 * Opens Root files
 *
 * @param file_name Analysis file path
 ***************************************************************/
void OpenRootFile(std::string file_name)
{
    TFile f(file_name.c_str());
    if (f.Get("AnalysisTree")) {
        if (!gChain) {
            gChain = new TChain("AnalysisTree");
            notifier->AddChain(gChain);
            gChain->SetNotify(notifier);
        }
        gChain->Add(file_name.c_str());
        //std::cout << "Added: " << file_name << std::endl;
    }
} // end OpenRootFile

/******************************************************************************
 * Determines input file type
 *
 * @param file_name  Input file
 *****************************************************************************/
void AutoFileDetect(std::string file_name)
{
    size_t dot_pos = file_name.find_last_of('.');
    std::string ext = file_name.substr(dot_pos + 1);

    if (ext == "root") {
        OpenRootFile(file_name);
    }
    else if (ext == "cal") {
        notifier->AddCalFile(file_name);
    } else {
        std::cerr << "Discarding unknown file: " << file_name.c_str() << std::endl;
    }
} // AutoFileDetect()

/******************************************************************************
 * Prints usage message and version
 *****************************************************************************/
void PrintUsage(char* argv[]){
    std::cerr << argv[0] << " Version: " << ExtractEvents_VERSION_MAJOR
              << "." << ExtractEvents_VERSION_MINOR << "\n"
              << "usage: " << argv[0] << " calibration_file analysis_tree [analysis_tree_2 ... ] \n"
              << " calibration_file:       calibration file (must end with .cal)\n"
              << " analysis_tree:          analysis tree to process (must end with .root)\n"
              << std::endl;
} // end PrintUsage
