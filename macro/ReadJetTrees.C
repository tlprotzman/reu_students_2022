/*
Currently this read R=0.4 Anti-Kt jets, both truth and reconstructed. The
jets are not matched.  Data exists in the input file for R=0.2 through R=0.8
jets in 0.1 increments, they can be specified by changing the prefix of the 
branch read in.  
*/

#include <TROOT.h>

#include <TFile.h>
#include <TTree.h>

#include <iostream>

int ReadJetTrees(int n_events=0, const std::string infile="jet_data_sample_11.root") {
    // This reads in the jet file and finds the TTree stored within it
    TFile *in = new TFile(infile.c_str());
    TTree *jet_tree = in->Get<TTree>("jet_tree");

    // Storing all the jets in each event
    // Add more or change to look at different jet R
    std::vector<double> *r04_g_eta = nullptr;
    std::vector<double> *r04_g_phi = nullptr;
    std::vector<double> *r04_g_pt = nullptr;
    std::vector<double> *r04_eta = nullptr;
    std::vector<double> *r04_phi = nullptr;
    std::vector<double> *r04_pt = nullptr;

    // Reading the needed variables from the trees
    // Change r04 prefix to r02 for instance to read R=0.2 jets
    jet_tree->SetBranchAddress("r04_g_eta", &r04_g_eta);
    jet_tree->SetBranchAddress("r04_g_pt", &r04_g_pt);
    jet_tree->SetBranchAddress("r04_g_phi", &r04_g_phi);

    jet_tree->SetBranchAddress("r04_eta", &r04_eta);
    jet_tree->SetBranchAddress("r04_pt", &r04_pt);
    jet_tree->SetBranchAddress("r04_phi", &r04_phi);

    // Set up output files and histograms here

    // Set the number of events to process.  If 0, all will be processed
    if (!n_events || n_events > jet_tree->GetEntries()) {
        n_events = jet_tree->GetEntries();
    }

    // Here's where your main loop will go
    for (uint n = 0; n < n_events; n++) {
        jet_tree->GetEvent(n);
        // Plot pt, eta, phi, etc... 

    }

    // Draw or save your histograms/output



    // Cleanup
    delete r04_g_eta;
    delete r04_g_phi;
    delete r04_g_pt;
    delete r04_eta;
    delete r04_phi;
    delete r04_pt;
    delete in;

    // Because people keep asking, returning 0 is standard for the entry point 
    // of a program if it successfully completes.  It can then be used by other
    // programs or scripts to determin if an error occured.  For instance, if I 
    // wasn't being sloppy, when open the file and read the jet tree I should 
    // validate that those actually happen, and could return say -1 and -2 if 
    // they don't.  But, I'm lazy ¯\_(ツ)_/¯
    return 0;
}