#include "CombineHarvester/ZH2016/interface/VHSystematicsRun2.h"
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
    
    void AddVHRun2Systematics(CombineHarvester & cb) {
        
        std::vector<std::string> sig_procs;
        sig_procs = {"ggH_htt","qqH_htt","WH_htt","ZH_htt"};
        
        // N.B. when adding this list of backgrounds to a nuisance, only
        // the backgrounds that are included in the background process
        // defined in MorphingAZH2016.cpp are included in the actual DCs
        // This is a list of all MC based backgrounds
        // QCD is explicitly excluded
        std::vector<std::string> all_bkgs = {
            "allFakes",
            "jetFakes",
            "DY",
            "ttW",
            "WH_hww125",
            // new above / old below
            "ZZ",
            "ggZZ",
            "TriBoson",
            "ttZ",
            "WZ",
            "TT",
            "ggH_hzz125",
            "ZH_hww125",
            };
        std::vector<std::string> all_zh_bkgs = {
            "allFakes",
            "ZZ",
            "ggZZ",
            "TriBoson",
            "ttZ",
            "DY",
            "WZ",
            "TT",
            "ggH_hzz125",
            "ZH_hww125",
            };
        std::vector<std::string> all_mc_bkgs = {
            "DY",
            "ttW",
            "WH_hww125",
            // new above / old below
            "ZZ",
            "ggZZ",
            "TriBoson",
            "ttZ",
            "WZ",
            "TT",
            "ggH_hzz125",
            "ZH_hww125",
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
        std::vector<std::string> whChans = {
            "mtt","ett","mmt","emt"};


        //##############################################################################
        //  lumi
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).AddSyst(cb,
                "lumi_13TeV_2016", "lnN", SystMap<>::init(1.025));



        //##############################################################################
        //  Background normalizations
        //##############################################################################


        cb.cp().process({"ZZ"}).AddSyst(cb,
                "CMS_htt_zzXsec_$ERA", "lnN", SystMap<>::init(1.048));

        cb.cp().process({"ggZZ"}).AddSyst(cb,
                "CMS_htt_ggzzXsec_$ERA", "lnN", SystMap<>::init(1.1));

        cb.cp().process({"TriBoson"}).AddSyst(cb,
                "CMS_htt_vvvXsec_$ERA", "lnN", SystMap<>::init(1.25));

        cb.cp().process({"ttZ"}).AddSyst(cb,
                "CMS_htt_ttzXsec_$ERA", "lnN", SystMap<>::init(1.25));

        cb.cp().process({"ttW"}).AddSyst(cb,
                "CMS_htt_ttwXsec_13TeV", "lnN", SystMap<>::init(1.25));

        cb.cp().process({"DY"}).AddSyst(cb,
                "CMS_htt_zXsec_13TeV", "lnN", SystMap<>::init(1.03));

        cb.cp().process({"WZ"}).AddSyst(cb,
                "CMS_htt_wzXsec_$ERA", "lnN", SystMap<>::init(1.055));

        cb.cp().process({"TT"}).AddSyst(cb,
                "CMS_htt_tjXsec_13TeV", "lnN", SystMap<>::init(1.06));


        //##############################################################################
        //  trigger   ##FIXME   the values have been chosen rather arbitrarily
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel(eeChans).AddSyst(cb,
                "CMS_eff_trigger_ee_$ERA", "lnN", SystMap<>::init(1.02));

        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel(mmChans).AddSyst(cb,
                "CMS_eff_trigger_mm_$ERA", "lnN", SystMap<>::init(1.02));

        cb.cp().process(all_mc_bkgs).channel({"emt"}).AddSyst(cb, "CMS_eff_trigger_et_13TeV", "lnN", SystMap<>::init(1.01));
        cb.cp().process(all_mc_bkgs).channel({"emt"}).AddSyst(cb, "CMS_eff_trigger_mt_13TeV", "lnN", SystMap<>::init(1.01));


        cb.cp().process(all_mc_bkgs).channel({"ett"}).AddSyst(cb, "CMS_eff_trigger_et_13TeV", "lnN", SystMap<>::init(1.02));
        cb.cp().process(all_mc_bkgs).channel({"mmt","mtt"}).AddSyst(cb, "CMS_eff_trigger_mt_13TeV", "lnN", SystMap<>::init(1.02));
        

        
        //##############################################################################
        //  Electron, muon and tau Id  efficiencies
        //##############################################################################

        // Fully correlated
        cb.cp().AddSyst(cb, "CMS_eff_m", "lnN", SystMap<channel, process>::init
                ({"eemt","eeem","mtt","emt"}, JoinStr({sig_procs, all_mc_bkgs}),  1.02)
                ({"emmt","mmtt","mmt"}, JoinStr({sig_procs, all_mc_bkgs}),  1.04)
                ({"mmmt","emmm"}, JoinStr({sig_procs, all_mc_bkgs}),  1.06) );
        
        cb.cp().AddSyst(cb, "CMS_eff_e", "lnN", SystMap<channel, process>::init
                ({"emmt","emmm","emt","ett"}, JoinStr({sig_procs, all_mc_bkgs}),  1.02)
                ({"eemt","eett"}, JoinStr({sig_procs, all_mc_bkgs}),  1.04)
                ({"eeet","eeem"}, JoinStr({sig_procs, all_mc_bkgs}),  1.06) );
        

        // Tau Efficiency applied to all MC
        // We have fully correlated components
        // And components based on anti-e / anti-mu discriminants used

        // Single Tau Final States
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","emmt","mmmt","mmt","emt"}).AddSyst(cb,
                "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.045));
        // TauTau
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eett","mmtt","ett","mtt"}).AddSyst(cb,
                "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.09));
        
        
        // Categorize by exact anti-e / anti-mu discriminants
        // e VL, m T
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eemt","mmmt","mmt"}).AddSyst(cb,
                "CMS_eff_t_eVLmuT_$ERA", "lnN", SystMap<>::init(1.02));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"mtt"}).AddSyst(cb,
                "CMS_eff_t_eVLmuT_$ERA", "lnN", SystMap<>::init(1.04));
        // e T, m L
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","emmt"}).AddSyst(cb,
                "CMS_eff_t_eTmuL_$ERA", "lnN", SystMap<>::init(1.02));
        // e VT, m L
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"ett"}).AddSyst(cb,
                "CMS_eff_t_eVTmuL_$ERA", "lnN", SystMap<>::init(1.04));
        // e VL, m L
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eett","mmtt"}).AddSyst(cb,
                "CMS_eff_t_eVLmuL_$ERA", "lnN", SystMap<>::init(1.04));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"emt"}).AddSyst(cb,
                "CMS_eff_t_eVLmuL_$ERA", "lnN", SystMap<>::init(1.02));
        
        
        //##############################################################################
        //  Fake Rate Uncertainties on the Reducible Background
        //##############################################################################
        
        // Modified Run-I Fake Rate uncertainty Method
        //cb.cp().process({"allFakes"}).channel({"eeet","emmt"}).AddSyst(cb,
        //        "CMS_htt_zh_reducible_bkg_scale_e_$ERA", "lnN", SystMap<>::init(1.1));
        //cb.cp().process({"allFakes"}).channel({"eeet","emmt"}).AddSyst(cb,
        //        "CMS_htt_zh_reducible_bkg_scale_t_$ERA", "lnN", SystMap<>::init(1.2));

        //cb.cp().process({"allFakes"}).channel({"eemt","mmmt"}).AddSyst(cb,
        //        "CMS_htt_zh_reducible_bkg_scale_m_$ERA", "lnN", SystMap<>::init(1.1));
        //cb.cp().process({"allFakes"}).channel({"eemt","mmmt"}).AddSyst(cb,
        //        "CMS_htt_zh_reducible_bkg_scale_t_$ERA", "lnN", SystMap<>::init(1.2));

        //cb.cp().process({"allFakes"}).channel({"eeem","emmm"}).AddSyst(cb,
        //        "CMS_htt_zh_reducible_bkg_scale_e_$ERA", "lnN", SystMap<>::init(1.1));
        //cb.cp().process({"allFakes"}).channel({"eeem","emmm"}).AddSyst(cb,
        //        "CMS_htt_zh_reducible_bkg_scale_m_$ERA", "lnN", SystMap<>::init(1.1));

        //cb.cp().process({"allFakes"}).channel({"eett","mmtt"}).AddSyst(cb,
        //        "CMS_htt_zh_reducible_bkg_scale_t_$ERA", "lnN", SystMap<>::init(1.15));

        // New FR Uncerts, Data-Prompt MC Method
        cb.cp().process({"allFakes"}).channel({"eeet","emmt"}).AddSyst(cb,
                "CMS_htt_zh_reducible_bkg_scale_LLET_$ERA", "lnN", SystMap<>::init(1.5));
        cb.cp().process({"allFakes"}).channel({"eemt","mmmt"}).AddSyst(cb,
                "CMS_htt_zh_reducible_bkg_scale_LLMT_$ERA", "lnN", SystMap<>::init(1.25));
        cb.cp().process({"allFakes"}).channel({"eeem","emmm"}).AddSyst(cb,
                "CMS_htt_zh_reducible_bkg_scale_LLEM_$ERA", "lnN", SystMap<>::init(2.0));
        cb.cp().process({"allFakes"}).channel({"eett","mmtt"}).AddSyst(cb,
                "CMS_htt_zh_reducible_bkg_scale_LLTT_$ERA", "lnN", SystMap<>::init(1.4));


        cb.cp().process({"allFakes"}).channel({"eeet","eemt","eeem","emmt","mmmt","emmm"}).AddSyst(cb,
                "CMS_scale_fake_rate_prompt_MC_EandM_$ERA", "shape", SystMap<>::init(1.00));

        cb.cp().process({"allFakes"}).channel({"eeet","eemt","eett","emmt","mmmt","mmtt"}).AddSyst(cb,
                "CMS_scale_fake_rate_prompt_MC_Tau_$ERA", "shape", SystMap<>::init(1.00));

        // WH Reducible
        cb.cp().process({"allFakes","jetFakes"}).channel(whChans).AddSyst(cb, "reducible_norm_$CHANNEL", "lnN", SystMap<>::init(1.20));

        cb.cp().process({"allFakes"}).channel({"emt"}).AddSyst(cb,"CMS_fakeEle_PromptSubtraction_13TeV", "shape", SystMap<>::init(1.00));
        cb.cp().process({"allFakes"}).channel({"emt","mmt"}).AddSyst(cb,"CMS_fakeMu_PromptSubtraction_13TeV", "shape", SystMap<>::init(1.00));

        // WH Tau per bin Fake Rate shifts
        std::vector<std::string> tau_dms = {"dm0","dm1","dm10"};
        std::vector<std::string> tau_pt_bins = {"pt20to25","pt25to30","pt30to35","pt35to40","pt40to50","pt50to60","ptgt60"};
        for (auto dm : tau_dms) {
            for (auto pt_bin : tau_pt_bins) {
                cb.cp().process({"allFakes"}).channel({"emt","mmt"}).AddSyst(cb,
                        "CMS_fakeTauT_"+dm+"_"+pt_bin+"_13TeV", "shape", SystMap<>::init(1.00));
                cb.cp().process({"jetFakes"}).channel({"ett","mtt"}).AddSyst(cb,
                        "CMS_fakeTauVT_"+dm+"_"+pt_bin+"_13TeV", "shape", SystMap<>::init(1.00));
            }
        }
        
        // WH Elec/Mu per bin Fake Rate shifts
        std::vector<std::string> lep_pt_bins = {"pt10to15","pt15to20","pt20to30","pt30to40","ptgt40"};
        for (auto pt_bin : lep_pt_bins) {
            cb.cp().process({"allFakes"}).channel({"emt"}).AddSyst(cb,
                    "CMS_fakeEle_"+pt_bin+"_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"allFakes"}).channel({"emt","mmt"}).AddSyst(cb,
                    "CMS_fakeMu_"+pt_bin+"_13TeV", "shape", SystMap<>::init(1.00));
        }
        
        
        //##############################################################################
        //  b tag and mistag rate  efficiencies
        //##############################################################################
        
        // Heavy flavor
        cb.cp().process({"TT","ttW","ttZ"}).AddSyst(cb,
                "CMS_eff_b_$ERA", "lnN", SystMap<>::init(1.045));
        
        // Light flavor
        cb.cp().process(JoinStr({sig_procs, {"ZZ","ggZZ","TriBoson","DY","WZ","ggH_hzz125","ZH_hww125","WH_hww125"}})).AddSyst(cb,
                "CMS_eff_b_$ERA", "lnN", SystMap<>::init(1.0015));



        //##############################################################################
        //  Electron, Muon, and Tau energy Scale
        //##############################################################################
        
        // Electron, 1% barrel, 2.5% end cap
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","eeem","emmt","emmm","emt","ett"}).AddSyst(cb,
                "CMS_scale_e_$ERA", "shape", SystMap<>::init(1.00));


        // Muon
        //cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"emt","mmt"}).AddSyst(cb,
        //        "CMS_scale_m_$ERA", "shape", SystMap<>::init(1.00));


        // Decay Mode based TES Settings
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","emmt","mmmt","mmtt","emt","mmt","ett","mtt"}).AddSyst(cb,
                "CMS_scale_t_1prong_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","emmt","mmmt","mmtt","emt","mmt","ett","mtt"}).AddSyst(cb,
                "CMS_scale_t_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","emmt","mmmt","mmtt","emt","mmt","ett","mtt"}).AddSyst(cb,
                "CMS_scale_t_3prong_$ERA", "shape", SystMap<>::init(1.00));
        

        ////##############################################################################
        ////  jet and met energy Scale
        ////##############################################################################
 
        // MET Systematic shapes
        // Not applied in WH semi-leptonic
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","eeem","emmt","mmmt","mmtt","emmm","ett","mtt"}).AddSyst(cb,
                "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","eeem","emmt","mmmt","mmtt","emmm","ett","mtt"}).AddSyst(cb,
                "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
        
        

        
        //##############################################################################
        // jet  to tau fake only in tt, mt and et channels
        //##############################################################################
        
        //cb.cp().process( {"TTJ","ZJ","VVJ","W_rest","ZJ_rest","TTJ_rest","VVJ_rest"}).channel({"tt","mt","et"}).AddSyst(cb,
        //        "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));
        
        //cb.cp().process( {"W"}).channel({"tt","mt","et"}).bin_id({1,2,3,13,14,15}).AddSyst(cb,
        //        "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));
        


        //##############################################################################
        // jet to elec/mu/tau fake for triggering lepton in WH
        //##############################################################################

        cb.cp().process({"TT","DY"}).channel({"emt","mmt"}).AddSyst(cb, "CMS_htt_jetFakeLep_13TeV", "lnN", SystMap<>::init(1.20));



        //##############################################################################
        // Theoretical Uncertainties on signal
        //##############################################################################
        
        
        //    Uncertainty on BR for HTT @ 125 GeV
        cb.cp().process(JoinStr({sig_procs,{"WH_hww125","ZH_hww125","ggH_hzz125"}})).AddSyst(cb,
                "BR_htt_THU", "lnN", SystMap<>::init(1.017));
        cb.cp().process(JoinStr({sig_procs,{"WH_hww125","ZH_hww125","ggH_hzz125"}})).AddSyst(cb,
                "BR_htt_PU_mq", "lnN", SystMap<>::init(1.0099));
        cb.cp().process(JoinStr({sig_procs,{"WH_hww125","ZH_hww125","ggH_hzz125"}})).AddSyst(cb,
                "BR_htt_PU_alphas", "lnN", SystMap<>::init(1.0062));
        
        cb.cp().process({"ggH_htt","ggH_hzz125"}).AddSyst(cb,"QCDScale_ggH", "lnN", SystMap<>::init(1.039));
        cb.cp().process({"qqH_htt"}).AddSyst(cb,"QCDScale_qqH", "lnN", SystMap<>::init(1.004));
        cb.cp().process({"WH_htt","WH_hww125"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.007));
        cb.cp().process({"ZH_htt","ZH_hww125"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.038));
        
        cb.cp().process({"ggH_htt","ggH_hzz125"}).AddSyst(cb,"pdf_Higgs_gg", "lnN", SystMap<>::init(1.032));
        cb.cp().process({"qqH_htt"}).AddSyst(cb,"pdf_Higgs_qq", "lnN", SystMap<>::init(1.021));
        cb.cp().process({"WH_htt","WH_hww125"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.019));
        cb.cp().process({"ZH_htt","ZH_hww125"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.016));

    }
}
