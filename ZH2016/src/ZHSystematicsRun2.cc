#include "CombineHarvester/ZH2016/interface/ZHSystematicsRun2.h"
#include <vector>
#include <string>
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"

using namespace std;

namespace ch {
    
    using ch::syst::SystMap;
    using ch::syst::SystMapAsymm;
    using ch::syst::era;
    using ch::syst::channel;
    using ch::syst::bin_id;
    using ch::syst::process;
    using ch::syst::bin;
    using ch::JoinStr;
    
    void AddZHRun2Systematics(CombineHarvester & cb, bool & azh) {
        
        std::vector<std::string> sig_procs;
        if (azh) {
            sig_procs = {"azh"};
        }
        if (!azh) {
            sig_procs = {"ZH"};
        }
        
        // N.B. when adding this list of backgrounds to a nuisance, only
        // the backgrounds that are included in the background process
        // defined in MorphingAZH2016.cpp are included in the actual DCs
        // This is a list of all MC based backgrounds
        // QCD is explicitly excluded
        std::vector<std::string> all_bkgs = {
            "RedBkg",
            "ZZ",
            "TriBoson",
            "ttZ",
            "WH_htt125",
            "ggH_hzz125",
            "ZH_hww125",
            "ggH_htt125",
            "qqH_htt125"
            };
        std::vector<std::string> all_mc_bkgs = {
            "ZZ",
            "TriBoson",
            "ttZ",
            "WH_htt125",
            "ggH_hzz125",
            "ZH_hww125",
            "ggH_htt125",
            "qqH_htt125"
            };
        std::vector<std::string> eeChans = {
            "eeet",
            "eemt",
            "eeem",
            "eett"};
        std::vector<std::string> mmChans = {
            "emmt",
            "mmmt",
            "emmm",
            "mmtt"};


        //##############################################################################
        //  lumi
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).AddSyst(cb,
                "lumi_2016_13tev", "lnN", SystMap<>::init(1.025));



        //##############################################################################
        //  Background normalizations
        //##############################################################################
        
        cb.cp().process({"RedBkg"}).AddSyst(cb,
                "CMS_htt_zh_reducible_bkg_scale_$ERA", "lnN", SystMap<>::init(1.1));

        cb.cp().process({"ZZ"}).AddSyst(cb,
                "CMS_htt_zh_ZZ_scale_$ERA", "lnN", SystMap<>::init(1.1));

        cb.cp().process({"TriBoson"}).AddSyst(cb,
                "CMS_htt_zh_triBoson_scale_$ERA", "lnN", SystMap<>::init(1.2));

        cb.cp().process({"ttZ"}).AddSyst(cb,
                "CMS_htt_zh_ttZ_scale_$ERA", "lnN", SystMap<>::init(1.3));

        cb.cp().process({"WH_htt125"}).AddSyst(cb,
                "CMS_htt_zh_wh_htt_scale_$ERA", "lnN", SystMap<>::init(1.2));

        cb.cp().process({"ZH_hww125"}).AddSyst(cb,
                "CMS_htt_zh_zh_hww_scale_$ERA", "lnN", SystMap<>::init(1.2));

        cb.cp().process({"ggH_hzz125"}).AddSyst(cb,
                "CMS_htt_zh_hzz_scale_$ERA", "lnN", SystMap<>::init(1.2));

        if (azh) {
            cb.cp().process({"ZH_htt125"}).AddSyst(cb,
                    "CMS_htt_zh_zh_scale_$ERA", "lnN", SystMap<>::init(1.2));
        }

        //##############################################################################
        //  trigger   ##FIXME   the values have been chosen rather arbitrarily
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel(eeChans).AddSyst(cb,
                "CMS_eff_ee_trigger_$ERA", "lnN", SystMap<>::init(1.02));

        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel(mmChans).AddSyst(cb,
                "CMS_eff_mm_trigger_$ERA", "lnN", SystMap<>::init(1.02));

        

        
        //##############################################################################
        //  Electron, muon and tau Id  efficiencies
        //##############################################################################

        // Fully correlated
        cb.cp().AddSyst(cb, "CMS_eff_m_$ERA", "lnN", SystMap<channel, process>::init
                ({"eemt","eeem"}, JoinStr({sig_procs, all_mc_bkgs}),  1.02)
                ({"emmt","mmtt"}, JoinStr({sig_procs, all_mc_bkgs}),  1.04)
                ({"mmmt","emmm"}, JoinStr({sig_procs, all_mc_bkgs}),  1.06) );
        
        cb.cp().AddSyst(cb, "CMS_eff_e_$ERA", "lnN", SystMap<channel, process>::init
                ({"emmt","emmm"}, JoinStr({sig_procs, all_mc_bkgs}),  1.02)
                ({"eemt","eett"}, JoinStr({sig_procs, all_mc_bkgs}),  1.04)
                ({"eeet","eeem"}, JoinStr({sig_procs, all_mc_bkgs}),  1.06) );
        

        // Tau Efficiency applied to all MC
        // We have channel specific components and fully correlated components

        // ETau & MuTau
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","emmt","mmmt"}).AddSyst(cb,
                "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.045));
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","emmt","mmmt"}).AddSyst(cb,
                "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));

        // TauTau
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eett","mmtt"}).AddSyst(cb,
                "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.09));
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eett","mmtt"}).AddSyst(cb,
                "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.04));
        
        
        
        
        //##############################################################################
        //  b tag and mistag rate  efficiencies
        //##############################################################################
        
        //cb.cp().AddSyst(cb, "CMS_htt_eff_b_$ERA", "lnN", SystMap<channel, bin_id, process>::init
        //        ({"em"}, {1}, {"TTJ","TTT","TT"}, 1.035));

        //cb.cp().AddSyst(cb, "CMS_htt_eff_b_$ERA", "lnN", SystMap<channel, bin_id, process>::init
        //        ({"em"}, {2,3}, {"TTJ","TTT","TT"}, 1.05));

        //cb.cp().AddSyst(cb, "CMS_htt_eff_b_$ERA", "lnN", SystMap<channel, bin_id, process>::init
        //        ({"em"}, {2, 3}, {"VV","VVT","VVJ"}, 1.015)); // Mainly SingleTop
        
        


        //##############################################################################
        //  Electron and tau energy Scale
        //##############################################################################
        
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"em"}).AddSyst(cb,
        //        "CMS_scale_e_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));


        // Decay Mode based TES Settings
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt"}).AddSyst(cb,
        //        "CMS_scale_t_1prong_$ERA", "shape", SystMap<>::init(1.00));
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt"}).AddSyst(cb,
        //        "CMS_scale_t_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt"}).AddSyst(cb,
        //        "CMS_scale_t_3prong_$ERA", "shape", SystMap<>::init(1.00));
        

        //##############################################################################
        //  jet and met energy Scale
        //##############################################################################
 
        // MET Systematic shapes
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt","em"}).bin_id({1,2,3}).AddSyst(cb,
        //        "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt","em"}).bin_id({1,2,3}).AddSyst(cb,
        //        "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
        
        
        // Standard JES, factorized 27 JES implementation below
        // only use 1 at a time.
        // full 27 JES...
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"et","mt","tt"}).bin_id({1,2,3}).AddSyst(cb,
        //        "CMS_scale_j_$ERA", "shape", SystMap<>::init(1.00));

        // JES factorization test tautau        
        std::vector< std::string > uncertNames = {
            "AbsoluteFlavMap",
            "AbsoluteMPFBias",
            "AbsoluteScale",
            "AbsoluteStat",
            "FlavorQCD",
            "Fragmentation",
            "PileUpDataMC",
            "PileUpPtBB",
            "PileUpPtEC1",
            "PileUpPtEC2",
            "PileUpPtHF",
            "PileUpPtRef",
            "RelativeBal",
            "RelativeFSR",
            "RelativeJEREC1",
            "RelativeJEREC2",
            "RelativeJERHF",
            "RelativePtBB",
            "RelativePtEC1",
            "RelativePtEC2",
            "RelativePtHF",
            "RelativeStatEC",
            "RelativeStatFSR",
            "RelativeStatHF",
            "SinglePionECAL",
            "SinglePionHCAL",
            "TimePtEta",
        }; // end uncertNames
        // Uncomment below for 27 JES
        //for (string uncert:uncertNames){
        //    cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"tt"}).bin_id({10,11,12}).AddSyst(cb,
        //            "CMS_scale_j_"+uncert+"_$ERA", "shape", SystMap<>::init(1.00));
        //}
        


        cb.cp().AddSyst(cb, "CMS_htt_scale_met_$ERA", "lnN", SystMap<channel, bin_id, process>::init
                ({"ttbar"}, {1, 2, 3}, {all_mc_bkgs}, 1.01));
        


        
        

        
        //##############################################################################
        // jet  to tau fake only in tt, mt and et channels
        //##############################################################################
        
        //cb.cp().process( {"TTJ","ZJ","VVJ","W_rest","ZJ_rest","TTJ_rest","VVJ_rest"}).channel({"tt","mt","et"}).AddSyst(cb,
        //        "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));
        
        //cb.cp().process( {"W"}).channel({"tt","mt","et"}).bin_id({1,2,3,13,14,15}).AddSyst(cb,
        //        "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));
        



        //##############################################################################
        // Theoretical Uncertainties on signal
        //##############################################################################
        
        //scale_gg on signal
        //cb.cp().process( {"ggH"}).channel({"et","mt","tt","em"}).AddSyst(cb,
        //        "CMS_scale_gg_$ERA", "shape", SystMap<>::init(1.00));
        
        
        
        //    Uncertainty on BR for HTT @ 125 GeV
        cb.cp().process(sig_procs).AddSyst(cb,"BR_htt_THU", "lnN", SystMap<>::init(1.017));
        cb.cp().process(sig_procs).AddSyst(cb,"BR_htt_PU_mq", "lnN", SystMap<>::init(1.0099));
        cb.cp().process(sig_procs).AddSyst(cb,"BR_htt_PU_alphas", "lnN", SystMap<>::init(1.0062));
        
        
        if (azh) {
            cb.cp().process({"ggH_htt125"}).AddSyst(cb,"QCDScale_ggH", "lnN", SystMap<>::init(1.039));
            cb.cp().process({"qqH_htt125"}).AddSyst(cb,"QCDScale_qqH", "lnN", SystMap<>::init(1.004));
            cb.cp().process({"WH_htt125"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.007));
            cb.cp().process({"ZH_htt125"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.038));
        }
        else {
            cb.cp().process({"ggH_htt"}).AddSyst(cb,"QCDScale_ggH", "lnN", SystMap<>::init(1.039));
            cb.cp().process({"qqH_htt"}).AddSyst(cb,"QCDScale_qqH", "lnN", SystMap<>::init(1.004));
            cb.cp().process({"WH_htt"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.007));
            cb.cp().process({"ZH_htt"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.038));
        }
        
        if (azh) {
            cb.cp().process({"ggH_htt125"}).AddSyst(cb,"pdf_Higgs_gg", "lnN", SystMap<>::init(1.032));
            cb.cp().process({"qqH_htt125"}).AddSyst(cb,"pdf_Higgs_qq", "lnN", SystMap<>::init(1.021));
            cb.cp().process({"WH_htt125"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.019));
            cb.cp().process({"ZH_htt125"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.016));
        }
        else {
            cb.cp().process({"ggH_htt"}).AddSyst(cb,"pdf_Higgs_gg", "lnN", SystMap<>::init(1.032));
            cb.cp().process({"qqH_htt"}).AddSyst(cb,"pdf_Higgs_qq", "lnN", SystMap<>::init(1.021));
            cb.cp().process({"WH_htt"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.019));
            cb.cp().process({"ZH_htt"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.016));
        }
        
        
        
        
        //   Additonal uncertainties applied to the paper i.e. top mass 
        //cb.cp().process( {"ggH"}).channel({"et","mt","em","tt"}).AddSyst(cb,
        //        "TopMassTreatment_$ERA", "shape", SystMap<>::init(1.00));
        
        
        //cb.cp().AddSyst(cb, "CMS_ggH_STXSmig01", "lnN", SystMap<channel, bin_id, process>::init
        //                ({"em"},{1},{"ggH"}, 0.959)
        //                ({"et"},{1},{"ggH"}, 0.959)
        //                ({"mt"},{1},{"ggH"}, 0.959)
        //                ({"tt"},{1},{"ggH"}, 0.959)
        //                
        //                ({"em"},{2},{"ggH"}, 1.079)
        //                ({"et"},{2},{"ggH"}, 1.079)
        //                ({"mt"},{2},{"ggH"}, 1.079)
        //                ({"tt"},{2},{"ggH"}, 1.079)
        //                
        //                ({"em"},{3},{"ggH"}, 1.039)
        //                ({"et"},{3},{"ggH"}, 1.039)
        //                ({"mt"},{3},{"ggH"}, 1.039)
        //                ({"tt"},{3},{"ggH"}, 1.039)
        //                );
        //
        //
        //cb.cp().AddSyst(cb, "CMS_ggH_STXSmig12", "lnN", SystMap<channel, bin_id, process>::init
        //                ({"em"},{1},{"ggH"}, 1.000)
        //                ({"et"},{1},{"ggH"}, 1.000)
        //                ({"mt"},{1},{"ggH"}, 1.000)
        //                ({"tt"},{1},{"ggH"}, 1.000)
        //                
        //                ({"em"},{2},{"ggH"}, 0.932)
        //                ({"et"},{2},{"ggH"}, 0.932)
        //                ({"mt"},{2},{"ggH"}, 0.932)
        //                ({"tt"},{2},{"ggH"}, 0.932)
        //                
        //                ({"em"},{3},{"ggH"}, 1.161)
        //                ({"et"},{3},{"ggH"}, 1.161)
        //                ({"mt"},{3},{"ggH"}, 1.161)
        //                ({"tt"},{3},{"ggH"}, 1.161)
        //                );
        
        //cb.cp().AddSyst(cb, "CMS_ggH_STXSVBF2j", "lnN", SystMap<channel, bin_id, process>::init
        //                ({"em"},{1},{"ggH"}, 1.000)
        //                ({"et"},{1},{"ggH"}, 1.000)
        //                ({"mt"},{1},{"ggH"}, 1.000)
        //                ({"tt"},{1},{"ggH"}, 1.000)
        //                
        //                ({"em"},{2},{"ggH"}, 1.000)
        //                ({"et"},{2},{"ggH"}, 1.000)
        //                ({"mt"},{2},{"ggH"}, 1.000)
        //                ({"tt"},{2},{"ggH"}, 1.000)
        //                
        //                ({"em"},{3},{"ggH"}, 1.200)
        //                ({"et"},{3},{"ggH"}, 1.200)
        //                ({"mt"},{3},{"ggH"}, 1.200)
        //                ({"tt"},{3},{"ggH"}, 1.200)
        //                );
        
                        
        
    }
}
