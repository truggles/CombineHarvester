#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"

using namespace std;

int main() {
  //! [part1]
  // First define the location of the "auxiliaries" directory where we can
  // source the input files containing the datacard shapes
  string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/auxiliaries/shapes/";

  // Create an empty CombineHarvester instance that will hold all of the
  // datacard configuration and histograms etc.
  ch::CombineHarvester cb;
  // Uncomment this next line to see a *lot* of debug information
  // cb.SetVerbosity(3);

  // Here we will just define two categories for an 8TeV analysis. Each entry in
  // the vector below specifies a bin name and corresponding bin_id.
  ch::Categories cats = {
      //{1, "tt_inclusive"},
      {1, "tt_inclusive"}
    };
  // ch::Categories is just a typedef of vector<pair<int, string>>
  //! [part1]


  //! [part2]
  //vector<string> masses = ch::MassesFromRange("120-135:5");
  // Or equivalently, specify the mass points explicitly:
  vector<string> masses = {"125",};
  //! [part2]

  //! [part3]
  cb.AddObservations({"*"}, {"htt"}, {"13TeV"}, {"tt"}, cats);
  //! [part3]

  //! [part4]
  vector<string> bkg_procs = {"ZTT", "W", "QCD", "TT", "VV"};
  cb.AddProcesses({"*"}, {"htt"}, {"13TeV"}, {"tt"}, bkg_procs, cats, false);

  vector<string> sig_procs = {"ggH", "vbfH"};
  //vector<string> sig_procs = {"ggH", "qqH"};
  cb.AddProcesses(masses, {"htt"}, {"13TeV"}, {"tt"}, sig_procs, cats, true);
  //! [part4]


  //Some of the code for this is in a nested namespace, so
  // we'll make some using declarations first to simplify things a bit.
  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::bin_id;
  using ch::syst::process;


//  //! [part5]
  //cb.cp().signals()
  cb.cp().backgrounds()
      .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
      ({"13TeV"}, 1.04));
  cb.cp().signals()
      .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
      ({"13TeV"}, 1.04));
  //! [part5]

  //! [part6]
  cb.cp().backgrounds()
      .AddSyst(cb, "CMS_double_tau_id_eff_$ERA", "lnN", SystMap<era>::init
      ({"13TeV"}, 1.11));
  cb.cp().signals()
      .AddSyst(cb, "CMS_double_tau_id_eff_$ERA", "lnN", SystMap<era>::init
      ({"13TeV"}, 1.11));
  //cb.cp().process({"ggH"})
  //cb.cp().process({"ZTT"})
  //    .AddSyst(cb, "pdf_gg", "lnN", SystMap<>::init(1.097));
  cb.cp().process({"TT"})
      .AddSyst(cb, "tt_norm", "lnN", SystMap<>::init(1.1));
  cb.cp().process({"W"})
      .AddSyst(cb, "w_norm", "lnN", SystMap<>::init(1.2));
  cb.cp().process({"QCD"})
      .AddSyst(cb, "qcd_norm", "lnN", SystMap<>::init(1.3));
  cb.cp().process({"VV"})
      .AddSyst(cb, "vv_norm", "lnN", SystMap<>::init(1.1));
  cb.cp().process({"ZTT"})
      .AddSyst(cb, "ztt_norm", "lnN", SystMap<>::init(1.1));

  //cb.cp().process(ch::JoinStr({sig_procs, {"ZTT", "TT"}}))
  //    .AddSyst(cb, "CMS_eff_m", "lnN", SystMap<>::init(1.02));

  //cb.cp()
  //    .AddSyst(cb,
  //      "CMS_bkgNormalization_$ERA", "lnN", SystMap<era, bin_id, process>::init
  //      ({"13TeV"}, {1},     {"W"},        1.20)
  //      ({"13TeV"}, {1},     {"QCD"},        1.30)
  //      ({"13TeV"}, {1},     {"VV"},        1.10)
  //      ({"13TeV"}, {1},     {"ZTT"},        1.10)
  //      ({"13TeV"}, {1},     {"TT"},         1.20));

  //cb.cp().process(ch::JoinStr({sig_procs, {"ZTT"}}))
  //    .AddSyst(cb, "CMS_scale_t_mutau_$ERA", "shape", SystMap<>::init(1.00));
  //! [part6]

  //! [part7]
  cb.cp().backgrounds().ExtractShapes(
      aux_shapes + "Wisconsin/htt_tt.inputs-sm-13TeV.root",
      "$BIN/$PROCESS",
      "$BIN/$PROCESS_$SYSTEMATIC");
  cb.cp().signals().ExtractShapes(
      aux_shapes + "Wisconsin/htt_tt.inputs-sm-13TeV.root",
      "$BIN/$PROCESS$MASS",
      "$BIN/$PROCESS$MASS_$SYSTEMATIC");
  //! [part7]

  //! [part8]
//  auto bbb = ch::BinByBinFactory()
//    .SetAddThreshold(0.1)
//    .SetFixNorm(true);
//
//  bbb.AddBinByBin(cb.cp().backgrounds(), cb);
//
//  // This function modifies every entry to have a standardised bin name of
//  // the form: {analysis}_{channel}_{bin_id}_{era}
//  // which is commonly used in the htt analyses
//  ch::SetStandardBinNames(cb);
  //! [part8]

  //! [part9]
  // First we generate a set of bin names:
  set<string> bins = cb.bin_set();
  // This method will produce a set of unique bin names by considering all
  // Observation, Process and Systematic entries in the CombineHarvester
  // instance.

  // We create the output root file that will contain all the shapes.
  TFile output("htt_tt.input.root", "RECREATE");

  // Finally we iterate through each bin,mass combination and write a
  // datacard.
  for (auto b : bins) {
    for (auto m : masses) {
      //cout << ">> Writing datacard for bin: " << b << " and mass: " << m
      cout << ">> Writing datacard for bin: " << b << " and mass: " << m
           << "\n";
      // We need to filter on both the mass and the mass hypothesis,
      // where we must remember to include the "*" mass entry to get
      // all the data and backgrounds.
      cb.cp().bin({b}).mass({m, "*"}).WriteDatacard(
          b + "_" + m + ".txt", output);
    }
  }
  //! [part9]

}
