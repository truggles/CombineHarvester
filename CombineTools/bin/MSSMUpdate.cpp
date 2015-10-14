#include <string>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include "boost/filesystem.hpp"
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/HttSystematics.h"
#include "CombineHarvester/CombineTools/interface/CardWriter.h"
#include "CombineHarvester/CombineTools/interface/CopyTools.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"

using namespace std;

int main() {
  ch::CombineHarvester cb;

  typedef vector<pair<int, string>> Categories;
  typedef vector<string> VString;

  string auxiliaries  = string(getenv("CMSSW_BASE")) + "/src/auxiliaries/";
  string aux_shapes   = auxiliaries +"shapes/";
  string aux_pruning  = auxiliaries +"pruning/";
  string input_dir =
      string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/CombineTools/input";

  VString chns =
      {"mt", "et", "tt"};

  map<string, string> input_folders = {
      {"mt", "Imperial"},
      {"et", "Imperial"},
      {"tt", "CERN"}
  };
  
  map<string, VString> bkg_procs;
  bkg_procs["mt"] = {"ZTT", "QCD", "W", "ZJ", "ZL", "TT", "VV"};
  bkg_procs["et"] = {"ZTT", "QCD", "W", "ZJ", "ZL", "TT", "VV"};
  bkg_procs["tt"] = {"ZTT", "QCD", "W", "ZJ", "ZL", "TT", "VV"};
  
  VString sig_procs = {"ggH", "bbH"};
  VString SM_procs = {"ggH_SM125", "qqH_SM125", "VH_SM125"};
  
  map<string, Categories> cats;
  cats["mt_8TeV"] = {
      {10, "muTau_nobtag_low"}, {11, "muTau_nobtag_medium"}, {12, "muTau_nobtag_high"}, {13, "muTau_btag_low"}, {14, "muTau_btag_high"}};
  cats["et_8TeV"] = {
      {10, "eleTau_nobtag_low"}, {11, "eleTau_nobtag_medium"}, {12, "eleTau_nobtag_high"}, {13, "eleTau_btag_low"}, {14, "eleTau_btag_high"}};
  cats["tt_8TeV"] = {
      {10, "tauTau_nobtag_low"}, {11, "tauTau_nobtag_medium"}, {12, "tauTau_nobtag_high"}, {13, "tauTau_btag_low"}, {14, "tauTau_btag_high"}};
  
  auto masses = ch::MassesFromRange(
      "90,100,120-140:10,140-200:20,200-500:50,600-1000:100"); //90,100?
    
  for (auto chn : chns) {
    cb.AddObservations(
      {"*"}, {"htt"}, {"8TeV"}, {chn}, cats[chn+"_8TeV"]);
    cb.AddProcesses(
      {"*"}, {"htt"}, {"8TeV"}, {chn}, bkg_procs[chn], cats[chn+"_8TeV"], false);
    cb.AddProcesses(
      masses, {"htt"}, {"8TeV"}, {chn}, sig_procs, cats[chn+"_8TeV"], true);
    cb.AddProcesses(
      masses, {"htt"}, {"8TeV"}, {chn}, SM_procs, cats[chn+"_8TeV"], true);
  }
  
  ch::AddMSSMUpdateSystematics_et_mt(cb);
  //ch::AddMSSMUpdateSystematics_em(cb);
  //ch::AddMSSMUpdateSystematics_mm(cb);
  ch::AddMSSMUpdateSystematics_tt(cb);

  cout << ">> Extracting histograms from input root files...\n";
  for (string chn : chns) {
    string file = aux_shapes + input_folders[chn] + "/htt_" + chn +
                  ".inputs-mssm-" + "8TeV" + "-0.root";
    cb.cp().channel({chn}).era({"8TeV"}).backgrounds().ExtractShapes(
        file, "$BIN/$PROCESS", "$BIN/$PROCESS_$SYSTEMATIC");
    cb.cp().channel({chn}).era({"8TeV"}).signals().ExtractShapes(
        file, "$BIN/$PROCESS$MASS", "$BIN/$PROCESS$MASS_$SYSTEMATIC");
  }
  
  map<string, VString> signal_types = {
    //{"ggH", {"ggh", "ggH", "ggA"}},
    //{"bbH", {"bbh", "bbH", "bbA"}}
    {"ggH", {"ggH"}},
    {"bbH", {"bbH"}} //SM_procs hier dazu?
  };
  cout << "Scaling signal process rates for acceptance...\n";
  for (string e : {"8TeV"}) {
    for (string p : sig_procs) { //SM_procs hier dazu?
      cout << "Scaling for process " << p << " and era " << e << "\n";
      auto gr = ch::TGraphFromTable(
          input_dir + "/xsecs_brs/mssm_" + p + "_" + e + "_accept.txt", "mPhi",
          "accept");
      cb.cp().process(signal_types[p]).era({e}).ForEachProc([&](ch::Process *proc) {
        double m = boost::lexical_cast<double>(proc->mass());
        proc->set_rate(proc->rate() * gr.Eval(m));
      });
    }
  }
  
  cout << ">> Setting standardised bin names...\n";
  ch::SetStandardBinNames(cb);
  
  string folder = "output/mssm_cards/LIMITS";
  boost::filesystem::create_directories(folder);
  boost::filesystem::create_directories(folder + "/common");
  for (auto m : masses) {
    boost::filesystem::create_directories(folder + "/" + m);
  }

  for (string chn : chns) {
    TFile output((folder + "/common/htt_" + chn + ".input.root").c_str(),
                 "RECREATE");
    auto bins = cb.cp().channel({chn}).bin_set();
    for (auto b : bins) {
      for (auto m : masses) {
        cout << ">> Writing datacard for bin: " << b << " and mass: " << m
                  << "\r" << flush;
        cb.cp().channel({chn}).bin({b}).mass({m, "*"}).WriteDatacard(
            folder + "/" + m + "/" + b + ".txt", output);
      }
    }
    output.Close();
  }

  
  cout << "\n>> Done!\n";
}