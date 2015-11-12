#define analysis_cxx
#include "analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TRint.h>

#include <fstream>
#include <iostream>

/**
 * Pretty-prints a variable and its content.
 */
#define PP(var) (std::cout << #var << ": " << (var) << std::endl)

/**
 * Shifts odd wire IDs by two.
 *
 * @tparam Integral Integer-like type
 * @param[in] id ID of wire
 * @return Transformed ID of wire
 */
template <typename Integral> Integral wire_id_shift(const Integral &id) {
    if (id % 2 == 0) {
        return id - 1;
    } else {
        return id + 1;
    }
}

void analysis::Loop() {
    //   In a ROOT session, you can do:
    //      Root > .L analysis.C
    //      Root > analysis t
    //      Root > t.GetEntry(12); // Fill t data members with entry number 12
    //      Root > t.Show();       // Show values of entry 12
    //      Root > t.Show(16);     // Read and show values of entry 16
    //      Root > t.Loop();       // Loop on all entries
    //

    //     This is the loop skeleton where:
    //    jentry is the global entry number in the chain
    //    ientry is the entry number in the current Tree
    //  Note that the argument to GetEntry must be:
    //    jentry for TChain::GetEntry
    //    ientry for TTree::GetEntry and TBranch::GetEntry
    //
    //       To read only selected branches, Insert statements like:
    // METHOD1:
    //    fChain->SetBranchStatus("*",0);  // disable all branches
    //    fChain->SetBranchStatus("branchname",1);  // activate branchname
    // METHOD2: replace line
    //    fChain->GetEntry(jentry);       //read all branches
    // by  b_branchname->GetEntry(ientry); //read only this branch
    TH1D *driftTimesHisto = new TH1D(
        "Driftzeiten", "Driftzeiten", 251, -2.5 / 2., 250 * 2.5 + 2.5 / 2.);
    TH2 *wireCorrHisto = new TH2D(
        "wireCorrelation", "wire correlations", 48, 0.5, 48.5, 48, 0.5, 48.5);

    if (fChain == 0)
        return;

    Long64_t nentries = fChain->GetEntriesFast();

    PP(nentries);

    std::ofstream json(filename + ".js");



    bool outer_first = true;
    json << "[";

    Long64_t nbytes = 0, nb = 0;
    // Iterate through all the events (contining multiple hits) …
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        // Try to find the ID in the tree. This might fail and give a negative
        // number.
        Long64_t ientry = LoadTree(jentry);

        // Crude error correction code.
        if (ientry < 0)
            break;

        // Load current entry in the data structure as a side effect (ugh). The
        // return is the loaded bytes in this task.
        nb = fChain->GetEntry(jentry);

        // Add bytes to total bytes read.
        nbytes += nb;

        // Skip empty events.
        if (nhits_le == 0) {
            continue;
        }

        if (!outer_first) {
            json << ",\n";
        }
            outer_first = false;

        json << "[";
        json << "[";
        bool first = true;
        for (UInt_t hit = 0; hit < nhits_le; hit++) {
            if (!first) {
                json << ",";
            }
                first = false;
            json << wire_id_shift(wire_le[hit]);
        }
        json << "]";
        json << ",";
        json << "[";
        first = true;
        for (UInt_t hit = 0; hit < nhits_le; hit++) {
            if (!first) {
                json << ",";
            }
                first = false;
            json << time_le[hit];
        }
        json << "]";
        json << ",";
        json << "[";
        first = true;
        for (UInt_t hit = 0; hit < nhits_le; hit++) {
            if (!first) {
                json << ",";
            }
                first = false;
            json << tot[hit];
        }
        json << "]";
        json << "]";

        // Iterate through the hits in *this* event. Usually this should be
        // around two hits as there are two wires passed with each particle.
        for (UInt_t hit = 0; hit < nhits_le; hit++) {
            // In case some threshold is not met, discard this hit.
            /*
            if (tot[hit] < 60) {
                continue;
            }
            */



            // Retrieve the physical time as the time in `time_le` is quantized
            // in 2.5 ns steps.
            Double_t time = time_le[hit] * 2.5;

            // Add the observed physical time to the histogram of drift times.
            driftTimesHisto->Fill(time);

            // Iterate through all the events again to build a two-point
            // correlation function.
            for (UInt_t j = 0; j < nhits_le; j++) {
                // Exclude self-correction as this would not be very
                // interesting.
                if (hit == j) {
                    continue;
                }

                const auto corrected_wire_1 = wire_id_shift(wire_le[hit]);
                const auto corrected_wire_2 = wire_id_shift(wire_le[j]);

                // Add the two points to the histogram.
                wireCorrHisto->Fill(corrected_wire_1, corrected_wire_2);
            }
        }

        // if (Cut(ientry) < 0) continue;
    }
    json << "]";

    driftTimesHisto->GetXaxis()->SetTitle("Zeit / ns");
    driftTimesHisto->GetYaxis()->SetTitle("Trefferanzahl");
    // gStyle->SetOptStat(0);
    driftTimesHisto->Draw();
}

int main(int argc, char **argv) {
    TROOT root("app", "app");
    TRint *app = new TRint("app", 0, NULL);
    TCanvas *c1 = new TCanvas("c", "c", 800, 600);
    TFile *f = new TFile(argv[1]);
    TTree *tree = (TTree *)f->FindObjectAny("t");
    // tree->Dump();
    analysis *ana = new analysis(tree);
    ana->filename = argv[1];
    ana->Loop();

    app->Run(kTRUE);
}
