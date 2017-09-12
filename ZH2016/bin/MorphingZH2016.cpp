#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include "boost/algorithm/string/predicate.hpp"
#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/regex.hpp"
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/CardWriter.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"
#include "CombineHarvester/CombineTools/interface/Algorithm.h"
#include "CombineHarvester/CombineTools/interface/AutoRebin.h"
#include "CombineHarvester/CombinePdfs/interface/MorphFunctions.h"
#include "CombineHarvester/ZH2016/interface/ZHSystematicsRun2.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "TH2.h"
#include "TF1.h"

using namespace std;
using boost::starts_with;
namespace po = boost::program_options;

template <typename T>
void To1Bin(T* proc)
{
    std::unique_ptr<TH1> originalHist = proc->ClonedScaledShape();
    TH1F *hist = new TH1F("hist","hist",1,0,1);
    double err = 0;
    double rate =
    originalHist->IntegralAndError(0, originalHist->GetNbinsX() + 1, err);
    hist->SetDirectory(0);
    hist->SetBinContent(1, rate);
    hist->SetBinError(1, err);
    proc->set_shape(*hist, true);  // True means adjust the process rate to the
    // integral of the hist
}

bool BinIsControlRegion(ch::Object const* obj)
{
    return (boost::regex_search(obj->bin(),boost::regex{"_cr$"}));
}

// Useful to have the inverse sometimes too
bool BinIsNotControlRegion(ch::Object const* obj)
{
    return !BinIsControlRegion(obj);
}



int main(int argc, char** argv) {
    // First define the location of the "auxiliaries" directory where we can
    // source the input files containing the datacard shapes
    string mass = "mA";
    string output_folder = "sm_run2";
    string input_folder="UW_TR/";
    string postfix="";
    bool azh = false;
    po::variables_map vm;
    po::options_description config("configuration");
    config.add_options()
    ("mass,m", po::value<string>(&mass)->default_value(mass))
    
    ("input_folder", po::value<string>(&input_folder)->default_value("USCMS"))
    
    ("postfix", po::value<string>(&postfix)->default_value(""))
    ("azh", po::value<bool>(&azh)->default_value(false))
    ("output_folder", po::value<string>(&output_folder)->default_value("sm_run2"));

    po::store(po::command_line_parser(argc, argv).options(config).run(), vm);
    po::notify(vm);
    
    
    
    
    
    typedef vector<string> VString;
    typedef vector<pair<int, string>> Categories;
    //! [part1]
    // First define the location of the "auxiliaries" directory where we can
    // source the input files containing the datacard shapes
    //    string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/CombineTools/bin/AllROOT_20fb/";
    std::string input_dir;
    input_dir  = string(getenv("CMSSW_BASE")) + "/src/CombineHarvester/ZH2016/shapes/"+input_folder+"/";
    
    
    
    VString chns = {"eeet", "eemt", "eeem", "eett", "emmt", "mmmt", "emmm", "mmtt"};
    VString bkg_procs;
    vector<string> sig_procs;
    vector<string> masses;

    if (!azh) {
        bkg_procs = {"RedBkg", "ZZ", "TriBoson", "WH_htt125", "ggH_hzz125",
            "ZH_hww125", "ggH_htt125", "qqH_htt125"};
        sig_procs = {"ZH_htt"};
        //masses = {"110","120","125","130","140"};
        masses = {"125"};
    }
    if (azh) {
        bkg_procs = {"RedBkg", "ZZ", "TriBoson", "WH_htt125", "ggH_hzz125",
            "ZH_hww125", "ggH_htt125", "qqH_htt125", "ZH_htt125"};
        sig_procs = {"azh"};
        masses = {"220", "240", "260", "280", "300", "320", "340", "350", "400"};
        masses = {"300"};
    }
    
    
    
    ch::CombineHarvester cb;
    
    
    
    map<string,Categories> cats;
    for (auto chn : chns) {
        cats[chn] = { {1, chn+"_inclusive"} };
    }
    
    
    
    
    
    using ch::syst::bin_id;
    
    //! [part2]
    for (auto chn : chns) {
        cb.AddObservations({"*"}, {"htt"}, {"13TeV"}, {chn},            cats[chn]);
        cb.AddProcesses(   {"*"}, {"htt"}, {"13TeV"}, {chn}, bkg_procs, cats[chn], false);
        cb.AddProcesses(masses,   {"htt"}, {"13TeV"}, {chn}, sig_procs, cats[chn], true);
        //Needed to add ewkz and W as these are not not available/Negative in qcd cR
    }
    
    
    
    //! [part4]
    
    
    //Some of the code for this is in a nested namespace, so
    // we'll make some using declarations first to simplify things a bit.
    //    using ch::syst::SystMap;
    //    using ch::syst::era;
    //    using ch::syst::channel;
    //    using ch::syst::bin_id;
    //    using ch::syst::process;
    
    
    
    
    
    
    ch::AddZHRun2Systematics(cb, azh);
    
    
        
    
    
    //! [part7]
    string analysis;
    if (azh) analysis = "mssm";
    if (!azh) analysis = "sm";
    
    for (string chn:chns){
        cb.cp().channel({chn}).backgrounds().ExtractShapes(
                input_dir + "htt_zh.inputs-"+analysis+"-13TeV"+postfix+".root",
                "$BIN/$PROCESS",
                "$BIN/$PROCESS_$SYSTEMATIC");
        cb.cp().channel({chn}).process(sig_procs).ExtractShapes(
                input_dir + "htt_zh.inputs-"+analysis+"-13TeV"+postfix+".root",
                "$BIN/$PROCESS$MASS",
                "$BIN/$PROCESS$MASS_$SYSTEMATIC");
    }
    
    
    
    
    
    //Now delete processes with 0 yield
    cb.FilterProcs([&](ch::Process *p) {
        bool null_yield = !(p->rate() > 0. || BinIsControlRegion(p));
        if (null_yield){
            std::cout << "[Null yield] Removing process with null yield: \n ";
            std::cout << ch::Process::PrintHeader << *p << "\n";
            cb.FilterSysts([&](ch::Systematic *s){
                bool remove_syst = (MatchingProcess(*p,*s));
                return remove_syst;
            });
        }
        return null_yield;
    });
    
    
    
    
    
    // And convert any shapes in the CRs to lnN:
    // Convert all shapes to lnN at this stage
    cb.cp().FilterSysts(BinIsNotControlRegion).syst_type({"shape"}).ForEachSyst([](ch::Systematic *sys) {
        sys->set_type("lnN");
    });
    
    
    //     //Merge to one bin for control region bins
    //    cb.cp().FilterAll(BinIsNotControlRegion).ForEachProc(To1Bin<ch::Process>);
    //    cb.cp().FilterAll(BinIsNotControlRegion).ForEachObs(To1Bin<ch::Observation>);
    
    
    
    
    //! [part8]
    auto bbb = ch::BinByBinFactory()
    .SetAddThreshold(0.05)
    .SetMergeThreshold(0.8)
    .SetFixNorm(false);
    bbb.MergeBinErrors(cb.cp().backgrounds());
    bbb.AddBinByBin(cb.cp().backgrounds(), cb);
    
    
    
    // And now do bbb for the control region with a slightly different config:
    auto bbb_ctl = ch::BinByBinFactory()
    .SetPattern("CMS_$ANALYSIS_$BIN_$ERA_$PROCESS_bin_$#")
    .SetAddThreshold(0.)
    .SetMergeThreshold(0.8)
    .SetFixNorm(false)  // contrary to signal region, bbb *should* change yield here
    .SetVerbosity(1);
    // Will merge but only for non Reducible Bkg processes, to be on the safe side
    bbb_ctl.MergeBinErrors(cb.cp().process({"RedBkg"}, false).FilterProcs(BinIsNotControlRegion));
    bbb_ctl.AddBinByBin(cb.cp().process({"RedBkg"}, false).FilterProcs(BinIsNotControlRegion), cb);
    cout << "\nDone with BBB\n";

    
    
    
    
    // This function modifies every entry to have a standardised bin name of
    // the form: {analysis}_{channel}_{bin_id}_{era}
    // which is commonly used in the htt analyses

    ch::SetStandardBinNames(cb);



    //! [part8]
    
    //! [part9]
    // First we generate a set of bin names:
    //    set<string> bins = cb.bin_set();
    // This method will produce a set of unique bin names by considering all
    // Observation, Process and Systematic entries in the CombineHarvester
    // instance.
    
    
    
    //// Add a theory group to the bottom of the DCs for use with CH Uncertainty Breakdown
    //string theoryUncertsString = "CMS_scale_gg_.*|CMS_qqH_QCDUnc.*|CMS_ggH_PDF.*|CMS_qqH_PDF.*|CMS_ggH_UEPS.*|CMS_qqH_UEPS.*|BR_htt_THU.*|BR_htt_PU_mq.*|BR_htt_PU_alphas.*|BR_hww_THU.*|BR_hww_PU_mq.*|BR_hww_PU_alphas.*|QCDScale_ggH.*|QCDScale_qqH.*|QCDScale_VH.*|QCDScale_VH.*|pdf_Higgs_gg.*|pdf_Higgs_qq.*|pdf_Higgs_VH.*|pdf_Higgs_VH.*|";

    ////cb.SetGroup("NonThySyst", {".*"});
    ////cb.RemoveGroup("NonThySyst", {theoryUncertsString});
    ////cb.SetGroup("JES", {"CMS_scale_j.*"});
    //cb.SetGroup("all", {".*"});
    //cb.SetGroup("BinByBin", {"CMS_htt_.*_bin_.*"});

    //// 4 Component Breakdown
    //cb.SetGroup("Theory", {theoryUncertsString});
    //cb.SetGroup("TheoryAndBBB", {theoryUncertsString,"CMS_htt_.*_bin_.*"});
    
    
    
    
    //! [part9]
    // First we generate a set of bin names:
    
    
    //Write out datacards. Naming convention important for rest of workflow. We
    //make one directory per chn-cat, one per chn and cmb. In this code we only
    //store the individual datacards for each directory to be combined later, but
    //note that it's also possible to write out the full combined card with CH
    string output_prefix = "output/";
    if(output_folder.compare(0,1,"/") == 0) output_prefix="";
    ch::CardWriter writer(output_prefix + output_folder + "/$TAG/$MASS/$BIN.txt",
                          output_prefix + output_folder + "/$TAG/common/htt_input.root");
    
    
    // We're not using mass as an identifier - which we need to tell the CardWriter
    // otherwise it will see "*" as the mass value for every object and skip it
    //    writer.SetWildcardMasses({});
    //    writer.SetVerbosity(1);
    
    writer.WriteCards("cmb", cb);
    for (auto chn : chns) {
        // And per-channel-category
        writer.WriteCards(chn, cb.cp().channel({chn}).bin_id({1})); // 1st argument is the folder name, see $TAG above
        
        
    }
    
    // This part is required in case we need to have limit for each separted categories
    // For all categories want to include control regions. This will
    // work even if the extra categories aren't there.


    
    
    cb.PrintAll();
    cout << "\nDone writing DCs\n";
    
    
}
