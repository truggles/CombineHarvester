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
    
    void AddZHRun2Systematics(CombineHarvester & cb) {
        
        std::vector<std::string> sig_procs;
        sig_procs = {"azh"};
        
        // N.B. when adding this list of backgrounds to a nuisance, only
        // the backgrounds that are included in the background process
        // defined in MorphingAZH2016.cpp are included in the actual DCs
        // This is a list of all MC based backgrounds
        // QCD is explicitly excluded
        std::vector<std::string> all_bkgs = {
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
            "WH_hww125",
            "ggH_htt125",
            "qqH_htt125",
            "WH_htt125",
            "ZH_htt125",
            };
        std::vector<std::string> all_mc_bkgs = {
            "ZZ",
            "ggZZ",
            "TriBoson",
            "ttZ",
            "DY",
            "WZ",
            "TT",
            "ggH_hzz125",
            "ZH_hww125",
            "WH_hww125",
            "ggH_htt125",
            "qqH_htt125",
            "WH_htt125",
            "ZH_htt125",
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



        cb.cp().process({"ZZ"}).AddSyst(cb,
                "CMS_htt_zzXsec_$ERA", "lnN", SystMap<>::init(1.048));

        cb.cp().process({"ggZZ"}).AddSyst(cb,
                "CMS_htt_ggzzXsec_$ERA", "lnN", SystMap<>::init(1.1));

        cb.cp().process({"TriBoson"}).AddSyst(cb,
                "CMS_htt_zh_triBoson_scale_$ERA", "lnN", SystMap<>::init(1.25));

        cb.cp().process({"ttZ"}).AddSyst(cb,
                "CMS_htt_ttzXsec_$ERA", "lnN", SystMap<>::init(1.25));

        cb.cp().process({"DY"}).AddSyst(cb,
                "CMS_htt_zh_ZTT_scale_$ERA", "lnN", SystMap<>::init(1.03));

        cb.cp().process({"WZ"}).AddSyst(cb,
                "CMS_htt_wzXsec_$ERA", "lnN", SystMap<>::init(1.055));

        cb.cp().process({"TT"}).AddSyst(cb,
                "CMS_htt_zh_TT_scale_$ERA", "lnN", SystMap<>::init(1.06));


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
        cb.cp().AddSyst(cb, "CMS_eff_m", "lnN", SystMap<channel, process>::init
                ({"eemt","eeem"}, JoinStr({sig_procs, all_mc_bkgs}),  1.02)
                ({"emmt","mmtt"}, JoinStr({sig_procs, all_mc_bkgs}),  1.04)
                ({"mmmt","emmm"}, JoinStr({sig_procs, all_mc_bkgs}),  1.06) );
        
        cb.cp().AddSyst(cb, "CMS_eff_e", "lnN", SystMap<channel, process>::init
                ({"emmt","emmm"}, JoinStr({sig_procs, all_mc_bkgs}),  1.02)
                ({"eemt","eett"}, JoinStr({sig_procs, all_mc_bkgs}),  1.04)
                ({"eeet","eeem"}, JoinStr({sig_procs, all_mc_bkgs}),  1.06) );
        

        // Tau Efficiency applied to all MC
        // We have fully correlated components
        // And components based on anti-e / anti-mu discriminants used

        // Single Tau Final States
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","emmt","mmmt"}).AddSyst(cb,
                "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.045));
        // TauTau
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eett","mmtt"}).AddSyst(cb,
                "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.09));
        
        
        // Categorize by exact anti-e / anti-mu discriminants
        // e VL, m T
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eemt","mmmt"}).AddSyst(cb,
                "CMS_eff_t_eVLmuT_$ERA", "lnN", SystMap<>::init(1.02));
        // e T, m L
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","emmt"}).AddSyst(cb,
                "CMS_eff_t_eTmuL_$ERA", "lnN", SystMap<>::init(1.02));
        // e VL, m L
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eett","mmtt"}).AddSyst(cb,
                "CMS_eff_t_eVLmuL_$ERA", "lnN", SystMap<>::init(1.04));
        
        
        //##############################################################################
        //  Fake Rate Uncertainties on the Reducible Background
        //##############################################################################
        
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

        
        
        //##############################################################################
        //  b tag and mistag rate  efficiencies
        //##############################################################################
        
        // Heavy flavor
        cb.cp().process({"TT","ttZ"}).AddSyst(cb,
                "CMS_eff_b_$ERA", "lnN", SystMap<>::init(1.045));
        
        // Light flavor
        cb.cp().process(JoinStr({sig_procs, {"ZZ","ggZZ","TriBoson","DY","WZ","ggH_hzz125",
                "ZH_hww125","WH_hww125","ttHnonBB","ttH_other125"}})).AddSyst(cb,
                "CMS_eff_b_$ERA", "lnN", SystMap<>::init(1.0015));


        //##############################################################################
        //  Electron and tau energy Scale
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","eeem","emmt","emmm"}).AddSyst(cb,
                "CMS_scale_e_$ERA", "shape", SystMap<>::init(1.00));


        // Decay Mode based TES Settings
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","emmt","mmmt","mmtt"}).AddSyst(cb,
                "CMS_scale_t_1prong_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","emmt","mmmt","mmtt"}).AddSyst(cb,
                "CMS_scale_t_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).channel({"eeet","eemt","eett","emmt","mmmt","mmtt"}).AddSyst(cb,
                "CMS_scale_t_3prong_$ERA", "shape", SystMap<>::init(1.00));
        

        ////##############################################################################
        ////  jet and met energy Scale
        ////##############################################################################
 
        // MET Systematic shapes
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).AddSyst(cb,
                "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
        cb.cp().process(JoinStr({sig_procs, all_mc_bkgs})).AddSyst(cb,
                "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
        
        

        
        //##############################################################################
        // Theoretical Uncertainties on signal
        //##############################################################################
        
        //    Uncertainty on BR for HTT @ 125 GeV
        //    Adding AZh process here because we study it through H-->tautau decays
        cb.cp().process(JoinStr({sig_procs,{"ggH_htt125","qqH_htt125","WH_htt125","ZH_htt125"}})).AddSyst(cb,"BR_htt_THU", "lnN", SystMap<>::init(1.017));
        cb.cp().process(JoinStr({sig_procs,{"ggH_htt125","qqH_htt125","WH_htt125","ZH_htt125"}})).AddSyst(cb,"BR_htt_PU_mq", "lnN", SystMap<>::init(1.0099));
        cb.cp().process(JoinStr({sig_procs,{"ggH_htt125","qqH_htt125","WH_htt125","ZH_htt125"}})).AddSyst(cb,"BR_htt_PU_alphas", "lnN", SystMap<>::init(1.0062));
        
        
        cb.cp().process({"ggH_hzz125","ggH_htt125"}).AddSyst(cb,"QCDScale_ggH", "lnN", SystMap<>::init(1.039));
        cb.cp().process({"qqH_htt125"}).AddSyst(cb,"QCDScale_qqH", "lnN", SystMap<>::init(1.004));
        cb.cp().process({"WH_htt125","WH_hww125"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.007));
        cb.cp().process({"ZH_htt125","ZH_hww125"}).AddSyst(cb,"QCDScale_VH", "lnN", SystMap<>::init(1.038));
        cb.cp().process({"ttHnonBB","ttH_other125"}).AddSyst(cb,"QCDScale_ttH", "lnN", SystMap<>::init(1.075)); // Avg of + and - scale
        
        cb.cp().process({"ggH_htt125","ggH_htt125"}).AddSyst(cb,"pdf_Higgs_gg", "lnN", SystMap<>::init(1.032));
        cb.cp().process({"qqH_htt125"}).AddSyst(cb,"pdf_Higgs_qq", "lnN", SystMap<>::init(1.021));
        cb.cp().process({"WH_htt125","WH_hww125"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.019));
        cb.cp().process({"ZH_htt125","ZH_hww125"}).AddSyst(cb,"pdf_Higgs_VH", "lnN", SystMap<>::init(1.016));
        cb.cp().process({"ttHnonBB","ttH_other125"}).AddSyst(cb,"pdf_Higgs_ttH", "lnN", SystMap<>::init(1.036));
        
    }
}
