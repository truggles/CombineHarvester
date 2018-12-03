#include "CombineHarvester/HTTSM2016/interface/HttSystematics_SMRun2.h"
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
    
    void AddSMRun2Systematics(CombineHarvester & cb, int control_region, bool mm_fit, bool ttbar_fit, bool do_shape_systematics) {
        // Create a CombineHarvester clone that only contains the signal
        // categories
        //
        // cb_sig is unused at the moment, (was it ever used in this analysis?) delete?
        //CombineHarvester cb_sig = cb.cp();
        //
        //
        //if (control_region == 1){
        //    // we only want to cosider systematic uncertainties in the signal region.
        //    // limit to only the 0jet/1jet and vbf categories
        //    cb_sig.bin_id({1,2,3});
        //}
        
        
        std::vector<std::string> sig_procs = {"ggH_htt","qqH_htt","WH_htt","ZH_htt"};
        std::vector<std::string> sig_procs_stage0 = {
            "ggH_htt",
            "qqH_htt",
            "WH_had_htt",
            "WH_lep_htt",
            "ZH_had_htt",
            "ZH_lep_htt"
        };
        std::vector<std::string> sig_procs_stage1 = {
            "ggH_VBFTOPO_JET3VETO_htt",
            "ggH_VBFTOPO_JET3_htt",
            "ggH_0J_htt",
            "ggH_1J_PTH_0_60_htt",
            "ggH_1J_PTH_60_120_htt",
            "ggH_1J_PTH_120_200_htt",
            "ggH_1J_PTH_GT200_htt",
            "ggH_GE2J_PTH_0_60_htt",
            "ggH_GE2J_PTH_60_120_htt",
            "ggH_GE2J_PTH_120_200_htt",
            "ggH_GE2J_PTH_GT200_htt",

            "qqH_FWDH_htt",
            "qqH_VBFTOPO_JET3VETO_htt",
            "qqH_VBFTOPO_JET3_htt",
            "qqH_VH2JET_htt",
            "qqH_REST_htt",
            "qqH_PTJET1_GT200_htt",

            "WH_had_FWDH_htt",
            "WH_had_VBFTOPO_JET3VETO_htt",
            "WH_had_VBFTOPO_JET3_htt",
            "WH_had_VH2JET_htt",
            "WH_had_REST_htt",
            "WH_had_PTJET1_GT200_htt",
            "WH_lep_FWDH_htt",
            "WH_lep_PTV_0_150_htt",
            "WH_lep_PTV_150_250_0J_htt",
            "WH_lep_PTV_150_250_GE1J_htt",
            "WH_lep_PTV_GT250_htt",

            "ZH_had_FWDH_htt",
            "ZH_had_VBFTOPO_JET3VETO_htt",
            "ZH_had_VBFTOPO_JET3_htt",
            "ZH_had_VH2JET_htt",
            "ZH_had_REST_htt",
            "ZH_had_PTJET1_GT200_htt",
            "ZH_lep_FWDH_htt",
            "ZH_lep_PTV_0_150_htt",
            "ZH_lep_PTV_150_250_0J_htt",
            "ZH_lep_PTV_150_250_GE1J_htt",
            "ZH_lep_PTV_GT250_htt"
        };
        std::vector<std::string> sig_procs_stage_sans_VH_had_VBFTOPO_JET3VETO = {
            "ggH_VBFTOPO_JET3VETO_htt",
            "ggH_VBFTOPO_JET3_htt",
            "ggH_0J_htt",
            "ggH_1J_PTH_0_60_htt",
            "ggH_1J_PTH_60_120_htt",
            "ggH_1J_PTH_120_200_htt",
            "ggH_1J_PTH_GT200_htt",
            "ggH_GE2J_PTH_0_60_htt",
            "ggH_GE2J_PTH_60_120_htt",
            "ggH_GE2J_PTH_120_200_htt",
            "ggH_GE2J_PTH_GT200_htt",

            "qqH_FWDH_htt",
            "qqH_VBFTOPO_JET3VETO_htt",
            "qqH_VBFTOPO_JET3_htt",
            "qqH_VH2JET_htt",
            "qqH_REST_htt",
            "qqH_PTJET1_GT200_htt",

            "VH_had_FWDH_htt",
            "VH_had_VBFTOPO_JET3_htt",
            "VH_had_VH2JET_htt",
            "VH_had_REST_htt",
            "VH_had_PTJET1_GT200_htt",

            "WH_lep_FWDH_htt",
            "WH_lep_PTV_0_150_htt",
            "WH_lep_PTV_150_250_0J_htt",
            "WH_lep_PTV_150_250_GE1J_htt",
            "WH_lep_PTV_GT250_htt",

            "ZH_lep_FWDH_htt",
            "ZH_lep_PTV_0_150_htt",
            "ZH_lep_PTV_150_250_0J_htt",
            "ZH_lep_PTV_150_250_GE1J_htt",
            "ZH_lep_PTV_GT250_htt"
        };
        // No harm in defining additional ggH here even if they aren't used
        std::vector<std::string> ggH_sig_procs = {
            "ggH_htt",
            "ggH_VBFTOPO_JET3VETO_htt",
            "ggH_VBFTOPO_JET3_htt",
            "ggH_0J_htt",
            "ggH_1J_PTH_0_60_htt",
            "ggH_1J_PTH_60_120_htt",
            "ggH_1J_PTH_120_200_htt",
            "ggH_1J_PTH_GT200_htt",
            "ggH_GE2J_PTH_0_60_htt",
            "ggH_GE2J_PTH_60_120_htt",
            "ggH_GE2J_PTH_120_200_htt",
            "ggH_GE2J_PTH_GT200_htt"
        };
        std::vector<std::string> ggH_sig_procs_125 = {
            "ggH_htt125",
            "ggH_VBFTOPO_JET3VETO_htt125",
            "ggH_VBFTOPO_JET3_htt125",
            "ggH_0J_htt125",
            "ggH_1J_PTH_0_60_htt125",
            "ggH_1J_PTH_60_120_htt125",
            "ggH_1J_PTH_120_200_htt125",
            "ggH_1J_PTH_GT200_htt125",
            "ggH_GE2J_PTH_0_60_htt125",
            "ggH_GE2J_PTH_60_120_htt125",
            "ggH_GE2J_PTH_120_200_htt125",
            "ggH_GE2J_PTH_GT200_htt125"
        };
        std::vector<std::string> qqH_sig_procs = {
            "qqH_htt",
            "qqH_FWDH_htt",
            "qqH_VBFTOPO_JET3VETO_htt",
            "qqH_VBFTOPO_JET3_htt",
            "qqH_VH2JET_htt",
            "qqH_REST_htt",
            "qqH_PTJET1_GT200_htt"
        };
        
        // N.B. when adding this list of backgrounds to a nuisance, only
        // the backgrounds that are included in the background process
        // defined in MorphingSM2016.cpp are included in the actual DCs
        // This is a list of all MC based backgrounds
        // QCD is explicitly excluded
        std::vector<std::string> all_mc_bkgs = {
            "ZL","ZJ","ZTT","TTJ","TTT","TT",
            "W","W_rest","ZJ_rest","TTJ_rest","VVJ_rest","VV","VVT","VVJ",
            "ggH_hww125","qqH_hww125","EWKZ"};
        std::vector<std::string> all_mc_bkgs_no_W = {
            "ZL","ZJ","ZTT","TTJ","TTT","TT",
            "ZJ_rest","TTJ_rest","VVJ_rest","VV","VVT","VVJ",
            "ggH_hww125","qqH_hww125","EWKZ"};
        std::vector<std::string> all_mc_bkgs_no_TTJ = {
            "ZL","ZJ","ZTT","TTT","TT",
            "ZJ_rest","TTJ_rest","VVJ_rest","VV","VVT","VVJ",
            "ggH_hww125","qqH_hww125","EWKZ"};

        //##############################################################################
        //  lumi
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, {"VV","VVT","VVJ","ggH_hww125","qqH_hww125"}})).AddSyst(cb,
                                            "lumi_13TeV_2016", "lnN", SystMap<>::init(1.025));
        cb.cp().process({"W_rest", "ZJ_rest", "TTJ_rest", "VVJ_rest"}).channel({"tt"}).AddSyst(cb,"lumi_13TeV_2016", "lnN", SystMap<>::init(1.025));
        
        //Add luminosity uncertainty for W in em, tt, ttbar and the mm region as norm is from MC
        cb.cp().process({"W"}).channel({"tt","em","mm","ttbar"}).AddSyst(cb,
                                            "lumi_13TeV_2016", "lnN", SystMap<>::init(1.025));

        if (!ttbar_fit){
              cb.cp().process({"TTT","TTJ"}).AddSyst(cb,"lumi_13TeV_2016", "lnN", SystMap<>::init(1.025));
        }
        
        //##############################################################################
        //  trigger
        //##############################################################################
        
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs_no_W, {"embedded"}})).channel({"mt"}).AddSyst(cb,
                                             "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
        
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs_no_W, {"embedded"}})).channel({"et"}).AddSyst(cb,
                                             "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
        
        // hard coding channel here keeps "em" and "ttbar" correlated
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded"}})).channel({"em","ttbar"}).AddSyst(cb,
                                             "CMS_eff_trigger_em_$ERA", "lnN", SystMap<>::init(1.02));

        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded"}})).channel({"tt"}).AddSyst(cb,
                                            "CMS_eff_trigger_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.10));


        
        
        //##############################################################################
        //  Electron, muon and tau Id  efficiencies
        //##############################################################################
        cb.cp().AddSyst(cb, "CMS_eff_m", "lnN", SystMap<channel, process>::init
                        ({"mm"}, {"ZTT", "TT", "VV", "ZL", "ZJ"},  1.02)
                        ({"mt"}, JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs_no_W}),  1.02)
                        ({"em","ttbar"}, JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs}),  1.02));
        
        cb.cp().AddSyst(cb, "CMS_eff_e", "lnN", SystMap<channel, process>::init
                        ({"et"}, JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs_no_W}),  1.02)
                        ({"em","ttbar"}, JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs}),       1.02));
        

        // Tau Efficiency applied to all MC
        // in tautau channel the applied value depends on the number of taus which is determined by
        // gen match. WJets for example is assumed to have 1 real tau and 1 fake as is TTJ
        // compared to ZTT which has 2 real taus.
        // We also have channel specific components and fully correlated components
        //
        // ETau & MuTau
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded"}})).channel({"et","mt"}).AddSyst(cb,
                                             "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.045));
        
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded"}})).channel({"et","mt"}).AddSyst(cb,
                                             "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));

        // TauTau - 2 real taus
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, {"embedded","ZTT","VV","VVT","TTT","EWKZ"}})).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.09));
        
        cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, {"embedded", "ZTT","VV","VVT","TTT","EWKZ"}})).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.04));

        // TauTau - 1+ jet to tau fakes
        cb.cp().process({"TTJ","ZJ","VVJ","W","W_rest","ZJ_rest","TTJ_rest","VVJ_rest"}).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$ERA", "lnN", SystMap<>::init(1.06));
        
        cb.cp().process({"TTJ","ZJ","VVJ","W","W_rest","ZJ_rest","TTJ_rest","VVJ_rest"}).channel({"tt"}).AddSyst(cb,
                                             "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.02));
        
        
        
        
        //##############################################################################
        //  Tau Id shape uncertainty (added March 08)
        //##############################################################################
        
        if (do_shape_systematics) {
            cb.cp().process({"embedded", "ZTT"}).channel({"et","mt"}).bin_id({1}).AddSyst(cb,
                                             "CMS_tauDMReco_1prong_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process({"embedded", "ZTT"}).channel({"et","mt"}).bin_id({1}).AddSyst(cb,
                                             "CMS_tauDMReco_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process({"embedded", "ZTT"}).channel({"et","mt"}).bin_id({1}).AddSyst(cb,
                                             "CMS_tauDMReco_3prong_$ERA", "shape", SystMap<>::init(1.00));
        }
        
        
        
        //##############################################################################
        //  b tag and mistag rate  efficiencies
        //##############################################################################
        
        cb.cp().AddSyst(cb, "CMS_eff_b_$ERA", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"}, {1}, {"TTJ","TTT","TT"}, 1.035));
        cb.cp().AddSyst(cb, "CMS_eff_b_$ERA", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"}, {2,3}, {"TTJ","TTT","TT"}, 1.05));

        cb.cp().AddSyst(cb, "CMS_eff_b_$ERA", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"}, {2, 3}, {"VV","VVT","VVJ"}, 1.015)); // Mainly SingleTop
        
        
        //##############################################################################
        //  Electron and tau energy Scale
        //##############################################################################
        
        if (do_shape_systematics) {
            cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded", "QCD"}})).channel({"em"}).AddSyst(cb,
                                                 "CMS_scale_e_$CHANNEL_$ERA", "shape", SystMap<>::init(1.00));
        }
        


        // Decay Mode based TES Settings
        if (do_shape_systematics) {
            cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded"}})).channel({"et","mt","tt"}).AddSyst(cb,
                                                      "CMS_scale_t_1prong_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded"}})).channel({"et","mt","tt"}).AddSyst(cb,
                                                      "CMS_scale_t_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs, {"embedded"}})).channel({"et","mt","tt"}).AddSyst(cb,
                                                      "CMS_scale_t_3prong_$ERA", "shape", SystMap<>::init(1.00));
        }
        

        //##############################################################################
        //  jet and met energy Scale
        //##############################################################################
 
        // MET Systematic shapes
        if (do_shape_systematics) {
            cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs})).channel({"et","mt","tt","em"}).bin_id({1,2,3}).AddSyst(cb,
                                                      "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1, all_mc_bkgs})).channel({"et","mt","tt","em"}).bin_id({1,2,3}).AddSyst(cb,
                                                      "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
        }
        // FIXME, in 2016 ttbar used lnN not shape. Do we want that in full Run-II?
        cb.cp().AddSyst(cb,
                        "CMS_htt_scale_met_$ERA", "lnN", SystMap<channel, bin_id, process>::init
                        ({"ttbar"}, {1, 2, 3}, {all_mc_bkgs}, 1.01));
        
        

        // JES factorization into 5 (6) categories for 2016 (2017)
        std::vector< std::string > uncertNames = {
            "RelativeBal",
            "eta0to5",
            "eta0to3",
            "eta3to5"
        }; // end uncertNames
        // FIXME add "RelativeSample" for 2017

        // Apply JES to all categories
        if (do_shape_systematics) {
            for (string uncert:uncertNames){
                cb.cp().process(JoinStr({sig_procs, sig_procs_stage0, sig_procs_stage1})).channel({"et","mt","em","tt"}).AddSyst(cb,
                        "CMS_scale_j_"+uncert+"_$ERA", "shape", SystMap<>::init(1.00));
            }
        }
        
        


        
        if (control_region > 0 && do_shape_systematics) {
            // Add to all CRs, don't include QCD or WJets in et/mt which have CRs, or QCD in tt
            
            cb.cp().process(all_mc_bkgs).channel({"et","mt"}).bin_id({10, 11, 12, 13, 14, 15}).AddSyst(cb,
                                                            "CMS_scale_met_clustered_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process(all_mc_bkgs).channel({"et","mt"}).bin_id({10, 11, 12, 13, 14, 15}).AddSyst(cb,
                                                            "CMS_scale_met_unclustered_$ERA", "shape", SystMap<>::init(1.00));
        }


        

        
        //##############################################################################
        //  normalization uncertaintie for the embedded samples
        //##############################################################################

        cb.cp().process({"embedded"}).AddSyst(cb,"CMS_htt_embedded_13TeV", "lnN", SystMap<>::init(1.04));


        
        
        //##############################################################################
        //  Background normalization uncertainties
        //##############################################################################
        
        //   Diboson  Normalisation - fully correlated
        cb.cp().process({"VV","VVT","VVJ","VVJ_rest"}).AddSyst(cb,
                                        "CMS_htt_vvXsec_13TeV", "lnN", SystMap<>::init(1.05));
        if (! ttbar_fit){
        //   ttbar Normalisation - fully correlated
        cb.cp().process({"TT","TTT","TTJ","TTJ_rest"}).AddSyst(cb,
                      "CMS_htt_tjXsec_13TeV", "lnN", SystMap<>::init(1.06));}

        // W norm, just for em, tt and the mm region where MC norm is from MC
//        cb.cp().process({"W","W_rest"}).channel({"tt","em","mm"}).AddSyst(cb,
//                                             "CMS_htt_wjXsec_13TeV", "lnN", SystMap<>::init(1.20)); // splitted to two uncertainties as following
        
        cb.cp().process({"W"}).channel({"em"}).AddSyst(cb,
                                                       "CMS_htt_jetFakeLep_13TeV", "lnN", SystMap<>::init(1.20));
        
        cb.cp().process({"W"}).channel({"tt"}).AddSyst(cb,
                                                       "CMS_htt_wjXsec_13TeV", "lnN", SystMap<>::init(1.04));
        
        

        // QCD norm, just for tt
        cb.cp().process({"QCD"}).channel({"em"}).bin_id({1}).AddSyst(cb,
                                             "CMS_htt_QCD_0jet_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.10));
        cb.cp().process({"QCD"}).channel({"em"}).bin_id({2}).AddSyst(cb,
                                             "CMS_htt_QCD_boosted_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.10));
        cb.cp().process({"QCD"}).channel({"em"}).bin_id({3}).AddSyst(cb,
                                             "CMS_htt_QCD_VBF_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.20));
        
        
        // QCD norm, just for tt
        cb.cp().process({"QCD"}).channel({"tt"}).bin_id({1}).AddSyst(cb,
                                             "CMS_htt_QCD_0jet_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.027));
        cb.cp().process({"QCD"}).channel({"tt"}).bin_id({2}).AddSyst(cb,
                                             "CMS_htt_QCD_boosted_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.027));
        cb.cp().process({"QCD"}).channel({"tt"}).bin_id({3}).AddSyst(cb,
                                             "CMS_htt_QCD_VBF_$CHANNEL_13TeV", "lnN", SystMap<>::init(1.15));
        
        
        //Iso to antiiso extrapolation
        cb.cp().process({"QCD"}).channel({"mt"}).bin_id({1,2,3}).AddSyst(cb,
                                             "QCD_Extrap_Iso_nonIso_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.20));
        cb.cp().process({"QCD"}).channel({"et"}).bin_id({1,2,3}).AddSyst(cb,
                                             "QCD_Extrap_Iso_nonIso_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.20));
        
        
        //This should affect only shape (normalized to nominal values)
        if (do_shape_systematics) {
            cb.cp().process({"QCD"}).channel({"et","mt"}).bin_id({1}).AddSyst(cb,
                                                 "WSFUncert_$CHANNEL_0jet_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process({"QCD"}).channel({"et","mt"}).bin_id({2}).AddSyst(cb,
                                                 "WSFUncert_$CHANNEL_boosted_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process({"QCD"}).channel({"et","mt"}).bin_id({3}).AddSyst(cb,
                                                 "WSFUncert_$CHANNEL_vbf_$ERA", "shape", SystMap<>::init(1.00));
        }
        
        
        // based on the Ersatz study in Run1
        cb.cp().process({"W"}).channel({"et","mt"}).bin_id({1}).AddSyst(cb,
                                             "WHighMTtoLowMT_0jet_$ERA", "lnN", SystMap<>::init(1.10));
        cb.cp().process({"W"}).channel({"et","mt"}).bin_id({2}).AddSyst(cb,
                                             "WHighMTtoLowMT_boosted_$ERA", "lnN", SystMap<>::init(1.05));
        cb.cp().process({"W"}).channel({"et","mt"}).bin_id({3}).AddSyst(cb,
                                             "WHighMTtoLowMT_vbf_$ERA", "lnN", SystMap<>::init(1.10));
        
        

        
        if (do_shape_systematics) {
            //##############################################################################
            //  DY LO->NLO reweighting, Between no and twice the correc(on.
            //##############################################################################
            
            cb.cp().process( {"ZTT","ZJ","ZL","ZJ_rest"}).channel({"et","mt","tt"}).AddSyst(cb,
                                                 "CMS_htt_dyShape_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( {"ZTT","ZL"}).channel({"em"}).AddSyst(cb,
                                                 "CMS_htt_dyShape_$ERA", "shape", SystMap<>::init(1.00));
            
            
            //##############################################################################
            // Ttbar shape reweighting, Between no and twice the correction
            //##############################################################################
            
            cb.cp().process( {"TTJ","TTT","TTJ_rest"}).channel({"tt"}).AddSyst(cb,
                                            "CMS_htt_ttbarShape_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( {"TTJ","TTT"}).channel({"et","mt"}).AddSyst(cb,
                                            "CMS_htt_ttbarShape_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( {"TT"}).channel({"em"}).AddSyst(cb,
                                            "CMS_htt_ttbarShape_$ERA", "shape", SystMap<>::init(1.00));
            
            //##############################################################################
            // ZL shape  and electron/muon  to tau fake only in  mt and et channels (updated March 22)
            //##############################################################################
            
            cb.cp().process( {"ZL"}).channel({"mt","et"}).AddSyst(cb,
                                                             "CMS_ZLShape_$CHANNEL_1prong_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( {"ZL"}).channel({"mt","et"}).AddSyst(cb,
                                                             "CMS_ZLShape_$CHANNEL_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
            
            
            
            //Changed March 10, the DM splitting was necessary based on observed differences in sim and data
            cb.cp().process( {"ZL"}).channel({"mt"}).bin_id({1,2,3}).AddSyst(cb,
                                                                         "CMS_mFakeTau_1prong_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( {"ZL"}).channel({"mt"}).bin_id({1,2,3}).AddSyst(cb,
                                                                         "CMS_mFakeTau_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( {"ZL"}).channel({"et"}).bin_id({1,2,3}).AddSyst(cb,
                                                                         "CMS_eFakeTau_1prong_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( {"ZL"}).channel({"et"}).bin_id({1,2,3}).AddSyst(cb,
                                                                         "CMS_eFakeTau_1prong1pizero_$ERA", "shape", SystMap<>::init(1.00));
            
       

 
            //##############################################################################
            // jet  to tau fake only in tt, mt and et channels
            //##############################################################################
            
            cb.cp().process( {"TTJ","ZJ","VVJ","W_rest","ZJ_rest","TTJ_rest","VVJ_rest"}).channel({"tt","mt","et"}).AddSyst(cb,
                                                                                "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));
            
            cb.cp().process( {"W"}).channel({"tt","mt","et"}).bin_id({1,2,3,13,14,15}).AddSyst(cb,
                                                                    "CMS_htt_jetToTauFake_$ERA", "shape", SystMap<>::init(1.00));
        



            //##############################################################################
            // Theoretical Uncertainties on signal
            //##############################################################################

            // Gluon Fusion WG1 uncertainty scheme
            // See: https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWG/SignalModelingTools
            // Needed for Grand Higgs Combination
            // With the addition of these uncertainties, the scale_gg uncertainty
            // above can be removed for that channel.
            // Only in tautau channel at the moment
            //cb.cp().process( ggH_sig_procs ).channel({"tt","mt"}).AddSyst(cb,
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_Mu_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_Res_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_Mig01_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_Mig12_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_VBF2j_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_VBF3j_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_PT60_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_PT120_$ERA", "shape", SystMap<>::init(1.00));
            cb.cp().process( ggH_sig_procs ).channel({"et","mt","tt","em"}).AddSyst(cb,
                    "THU_ggH_qmtop_$ERA", "shape", SystMap<>::init(1.00));
        }
        



                        
                        
        cb.cp().AddSyst(cb, "CMS_qqH_QCDUnc", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"},{1},qqH_sig_procs, 0.997)
                        ({"et"},{1},qqH_sig_procs, 1.003)
                        ({"mt"},{1},qqH_sig_procs, 0.998)
                        ({"tt"},{1},qqH_sig_procs, 0.997)
                        
                        ({"em"},{2},qqH_sig_procs, 1.004)
                        ({"et"},{2},qqH_sig_procs, 1.004)
                        ({"mt"},{2},qqH_sig_procs, 1.002)
                        ({"tt"},{2},qqH_sig_procs, 1.003)
                        
                        ({"em"},{3},qqH_sig_procs, 1.005)
                        ({"et"},{3},qqH_sig_procs, 1.005)
                        ({"mt"},{3},qqH_sig_procs, 1.002)
                        ({"tt"},{3},qqH_sig_procs, 1.003)
                        );
       
        
        
        
        cb.cp().AddSyst(cb, "CMS_ggH_PDF", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"},{1},ggH_sig_procs, 1.007)
                        ({"et"},{1},ggH_sig_procs, 1.007)
                        ({"mt"},{1},ggH_sig_procs, 1.007)
                        ({"tt"},{1},ggH_sig_procs, 1.009)
                        
                        ({"em"},{2},ggH_sig_procs, 1.007)
                        ({"et"},{2},ggH_sig_procs, 1.007)
                        ({"mt"},{2},ggH_sig_procs, 1.007)
                        ({"tt"},{2},ggH_sig_procs, 1.009)
                        
                        ({"em"},{3},ggH_sig_procs, 1.007)
                        ({"et"},{3},ggH_sig_procs, 1.007)
                        ({"mt"},{3},ggH_sig_procs, 1.007)
                        ({"tt"},{3},ggH_sig_procs, 1.009)
                        );
        
        
        
        cb.cp().AddSyst(cb, "CMS_qqH_PDF", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"},{1},qqH_sig_procs, 1.011)
                        ({"et"},{1},qqH_sig_procs, 1.005)
                        ({"mt"},{1},qqH_sig_procs, 1.005)
                        ({"tt"},{1},qqH_sig_procs, 1.008)
                        
                        ({"em"},{2},qqH_sig_procs, 1.005)
                        ({"et"},{2},qqH_sig_procs, 1.002)
                        ({"mt"},{2},qqH_sig_procs, 1.002)
                        ({"tt"},{2},qqH_sig_procs, 1.003)
                        
                        ({"em"},{3},qqH_sig_procs, 1.005)
                        ({"et"},{3},qqH_sig_procs, 1.005)
                        ({"mt"},{3},qqH_sig_procs, 1.005)
                        ({"tt"},{3},qqH_sig_procs, 1.005)
                        );
        
        
        
        // Other analyses split by CMS_ggH_UE & CMS_ggH_PS
        // We do not have those numbers split out
        // According to Andrew, the vast majority of this
        // measured nuisance is the Parton Shower component
        // so rename this to align with PS
        //cb.cp().AddSyst(cb, "CMS_ggH_UEPS", "lnN", SystMap<channel, bin_id, process>::init
        cb.cp().AddSyst(cb, "CMS_ggH_PS", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"},{1},ggH_sig_procs, 1.015)
                        ({"et"},{1},ggH_sig_procs, 1.015)
                        ({"mt"},{1},ggH_sig_procs, 1.015)
                        ({"tt"},{1},ggH_sig_procs, 1.015)
                        
                        ({"em"},{2},ggH_sig_procs, 0.945)
                        ({"et"},{2},ggH_sig_procs, 0.945)
                        ({"mt"},{2},ggH_sig_procs, 0.945)
                        ({"tt"},{2},ggH_sig_procs, 0.945)
                        
                        ({"em"},{3},ggH_sig_procs, 1.03)
                        ({"et"},{3},ggH_sig_procs, 1.03)
                        ({"mt"},{3},ggH_sig_procs, 1.03)
                        ({"tt"},{3},ggH_sig_procs, 1.03)
                        );
        
        
        
        cb.cp().AddSyst(cb, "CMS_qqH_PS", "lnN", SystMap<channel, bin_id, process>::init
                        ({"em"},{1},qqH_sig_procs, 1.015)
                        ({"et"},{1},qqH_sig_procs, 1.015)
                        ({"mt"},{1},qqH_sig_procs, 1.015)
                        ({"tt"},{1},qqH_sig_procs, 1.015)
                        
                        ({"em"},{2},qqH_sig_procs, 0.945)
                        ({"et"},{2},qqH_sig_procs, 0.945)
                        ({"mt"},{2},qqH_sig_procs, 0.945)
                        ({"tt"},{2},qqH_sig_procs, 0.945)
                        
                        ({"em"},{3},qqH_sig_procs, 1.03)
                        ({"et"},{3},qqH_sig_procs, 1.03)
                        ({"mt"},{3},qqH_sig_procs, 1.03)
                        ({"tt"},{3},qqH_sig_procs, 1.03)
                        );
        
        
        
        //    Uncertainty on BR for HTT @ 125 GeV
        cb.cp().process(sig_procs).AddSyst(cb,"BR_htt_THU", "lnN", SystMap<>::init(1.017));
        cb.cp().process(sig_procs).AddSyst(cb,"BR_htt_PU_mq", "lnN", SystMap<>::init(1.0099));
        cb.cp().process(sig_procs).AddSyst(cb,"BR_htt_PU_alphas", "lnN", SystMap<>::init(1.0062));
        
        //    Uncertainty on BR of HWW @ 125 GeV
        cb.cp().process({"ggH_hww125","qqH_hww125"}).AddSyst(cb,"BR_hww_THU", "lnN", SystMap<>::init(1.0099));
        cb.cp().process({"ggH_hww125","qqH_hww125"}).AddSyst(cb,"BR_hww_PU_mq", "lnN", SystMap<>::init(1.0099));
        cb.cp().process({"ggH_hww125","qqH_hww125"}).AddSyst(cb,"BR_hww_PU_alphas", "lnN", SystMap<>::init(1.0066));
        
        
        cb.cp().process( JoinStr({ggH_sig_procs,{"ggH_hww125"}}) ).AddSyst(cb,"QCDscale_ggH", "lnN", SystMap<>::init(1.039));
        cb.cp().process( JoinStr({qqH_sig_procs,{"qqH_hww125"}}) ).AddSyst(cb,"QCDscale_qqH", "lnN", SystMap<>::init(1.004));
        cb.cp().process({"WH_htt"}).AddSyst(cb,"QCDscale_VH", "lnN", SystMap<>::init(1.007));
        cb.cp().process({"ZH_htt"}).AddSyst(cb,"QCDscale_VH", "lnN", SystMap<>::init(1.038));
        
        cb.cp().process( JoinStr({ggH_sig_procs,{"ggH_hww125"}}) ).AddSyst(cb,"pdf_Higgs_gg", "lnN", SystMap<>::init(1.032));
        cb.cp().process( JoinStr({qqH_sig_procs,{"qqH_hww125"}}) ).AddSyst(cb,"pdf_Higgs_qqbar", "lnN", SystMap<>::init(1.021));
        cb.cp().process({"WH_htt"}).AddSyst(cb,"pdf_Higgs_qqbar", "lnN", SystMap<>::init(1.019));
        cb.cp().process({"ZH_htt"}).AddSyst(cb,"pdf_Higgs_qqbar", "lnN", SystMap<>::init(1.016));
        
        
        
        
        
        
        
        //   Additonal uncertainties applied to the paper i.e. top mass 
        //cb.cp().process( ggH_sig_procs ).channel({"et","mt","em","tt"}).AddSyst(cb,
        //cb.cp().process( ggH_sig_procs ).channel({"et","mt","em"}).AddSyst(cb,
        //        "TopMassTreatment_$ERA", "shape", SystMap<>::init(1.00));
        
        
        //cb.cp().AddSyst(cb, "CMS_ggH_STXSmig01", "lnN", SystMap<channel, bin_id, process>::init
        //                ({"em"},{1},{"ggH_htt"}, 0.959)
        //                ({"et"},{1},{"ggH_htt"}, 0.959)
        //                ({"mt"},{1},{"ggH_htt"}, 0.959)
        //                //({"tt"},{1},ggH_sig_procs, 0.959)
        //                
        //                ({"em"},{2},{"ggH_htt"}, 1.079)
        //                ({"et"},{2},{"ggH_htt"}, 1.079)
        //                ({"mt"},{2},{"ggH_htt"}, 1.079)
        //                //({"tt"},{2},ggH_sig_procs, 1.079)
        //                
        //                ({"em"},{3},{"ggH_htt"}, 1.039)
        //                ({"et"},{3},{"ggH_htt"}, 1.039)
        //                ({"mt"},{3},{"ggH_htt"}, 1.039)
        //                //({"tt"},{3},ggH_sig_procs, 1.039)
        //                );
        //
        //
        //cb.cp().AddSyst(cb, "CMS_ggH_STXSmig12", "lnN", SystMap<channel, bin_id, process>::init
        //                ({"em"},{1},{"ggH_htt"}, 1.000)
        //                ({"et"},{1},{"ggH_htt"}, 1.000)
        //                ({"mt"},{1},{"ggH_htt"}, 1.000)
        //                //({"tt"},{1},ggH_sig_procs, 1.000)
        //                
        //                ({"em"},{2},{"ggH_htt"}, 0.932)
        //                ({"et"},{2},{"ggH_htt"}, 0.932)
        //                ({"mt"},{2},{"ggH_htt"}, 0.932)
        //                //({"tt"},{2},ggH_sig_procs, 0.932)
        //                
        //                ({"em"},{3},{"ggH_htt"}, 1.161)
        //                ({"et"},{3},{"ggH_htt"}, 1.161)
        //                ({"mt"},{3},{"ggH_htt"}, 1.161)
        //                //({"tt"},{3},ggH_sig_procs, 1.161)
        //                );
        //
        //cb.cp().AddSyst(cb, "CMS_ggH_STXSVBF2j", "lnN", SystMap<channel, bin_id, process>::init
        //                ({"em"},{1},{"ggH_htt"}, 1.000)
        //                ({"et"},{1},{"ggH_htt"}, 1.000)
        //                ({"mt"},{1},{"ggH_htt"}, 1.000)
        //                //({"tt"},{1},ggH_sig_procs, 1.000)
        //                
        //                ({"em"},{2},{"ggH_htt"}, 1.000)
        //                ({"et"},{2},{"ggH_htt"}, 1.000)
        //                ({"mt"},{2},{"ggH_htt"}, 1.000)
        //                //({"tt"},{2},ggH_sig_procs, 1.000)
        //                
        //                ({"em"},{3},{"ggH_htt"}, 1.200)
        //                ({"et"},{3},{"ggH_htt"}, 1.200)
        //                ({"mt"},{3},{"ggH_htt"}, 1.200)
        //                //({"tt"},{3},ggH_sig_procs, 1.200)
        //                );
        
                        
                        
        
        //  // Recoil corrections
        //  // ------------------
        //  // These should not be applied to the W in all control regions becasuse we should
        //  // treat it as an uncertainty on the low/high mT factor.
        //  // For now we also avoid applying this to any of the high-mT control regions
        //  // as the exact (anti-)correlation with low mT needs to be established
        //  // CHECK THIS
        //  cb.cp().AddSyst(cb,
        //    "CMS_htt_boson_scale_met_$ERA", "lnN", SystMap<channel, bin_id, process>::init
        //    ({"et", "mt", "em", "tt"}, {1, 2, 3, 4,5,6}, JoinStr({signal, {"ZTT", "W"}}), 1.02));
        //
        
        
        
        if(mm_fit)
        {
            // Add Z crosssection uncertainty on ZL, ZJ  (not ZTT due to taking into account the mm control region).
            // Also don't add it for the mm control region
            cb.SetFlag("filters-use-regex", true);
            cb.cp().channel({"mm"},false).process({"ZL", "ZJ"}).AddSyst(cb,
                                             "CMS_htt_zjXsec_13TeV", "lnN", SystMap<>::init(1.04));
            cb.cp().channel({"mm"},false).bin({"_cr$"}).process({"ZTT"}).AddSyst(cb,
                                             "CMS_htt_zjXsec_13TeV", "lnN", SystMap<>::init(1.04));
            cb.SetFlag("filters-use-regex", false);
            
            cb.FilterSysts([](ch::Systematic *syst) {
                return syst->name() == "lumi_13TeV_2016" &&
                (
                 (syst->channel() == "mm" && syst->process() == "ZL") ||
                 (syst->channel() != "mm" && syst->process() == "ZTT" &&
                  (syst->bin_id() == 1 || syst->bin_id() == 2 || syst->bin_id() == 3 || syst->bin_id() == 4 || syst->bin_id() == 5 || syst->bin_id() == 6 ))
                 );
            });
        }
        else
        {
            // Z->mumu CR normalization propagation
            // 0jet normalization only
            cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                             "CMS_htt_zmm_norm_extrap_0jet_$CHANNEL_$ERA", "lnN",
                                             SystMap<channel, bin_id>::init({"em","tt"},{1}, 1.07));
            cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                             "CMS_htt_zmm_norm_extrap_0jet_lt_$ERA", "lnN",
                                             SystMap<channel, bin_id>::init({"et","mt"},{1}, 1.07));
            
            // boosted normalization only
            cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                             "CMS_htt_zmm_norm_extrap_boosted_$CHANNEL_$ERA", "lnN",
                                             SystMap<channel, bin_id>::init({"em","tt"},{2}, 1.07));
            cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                             "CMS_htt_zmm_norm_extrap_boosted_lt_$ERA", "lnN",
                                             SystMap<channel, bin_id>::init({"et","mt"},{2}, 1.07));
            
            // VBF norm and shape for et/mt/tt
            cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"em"}).bin_id({3}).AddSyst(cb,
                                             "CMS_htt_zmm_norm_extrap_VBF_em_$ERA", "lnN", SystMap<>::init(1.15));
            cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"et","mt"}).bin_id({3}).AddSyst(cb,
                                             "CMS_htt_zmm_norm_extrap_VBF_lt_$ERA", "lnN", SystMap<>::init(1.15));
            cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"tt"}).bin_id({3}).AddSyst(cb,
                                             "CMS_htt_zmm_norm_extrap_VBF_tt_$ERA", "lnN", SystMap<>::init(1.10));

//            // FIXME should have EWKZ in all
//            cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"tt"}).bin_id({3}).AddSyst(cb,
//                                             "CMS_htt_zmumuShape_VBF_$ERA", "shape", SystMap<>::init(1.00));
//            cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"mt","et"}).bin_id({3}).AddSyst(cb,
//                                             "CMS_htt_zmumuShape_VBF_$ERA", "shape", SystMap<>::init(1.00));
//            cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest"}).channel({"em"}).bin_id({3}).AddSyst(cb,
//                                            "CMS_htt_zmumuShape_VBF_$ERA", "shape", SystMap<>::init(1.00));

            if (do_shape_systematics) {
                cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"tt","et","mt"}).bin_id({3}).AddSyst(cb,
                                                "CMS_htt_zmumuShape_VBF_$ERA", "shape", SystMap<>::init(1.00));
                cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest"}).channel({"em"}).bin_id({3}).AddSyst(cb,
                                                "CMS_htt_zmumuShape_VBF_$ERA", "shape", SystMap<>::init(1.00));
            }


            // Add the zmumu extrapolation uncertainties to Drell-Yan in CRs
            // if applicable
            if(control_region > 0)
            {
                // Z->mumu CR normalization propagation
                // 0jet normalization only
                cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                                 "CMS_htt_zmm_norm_extrap_0jet_$CHANNEL_$ERA", "lnN",
                                                 SystMap<channel, bin_id>::init({"tt"},{10}, 1.07));
                cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                                 "CMS_htt_zmm_norm_extrap_0jet_lt_$ERA", "lnN",
                                                 SystMap<channel, bin_id>::init({"et","mt"},{10,13}, 1.07));
                
                // boosted normalization only
                cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                                 "CMS_htt_zmm_norm_extrap_boosted_$CHANNEL_$ERA", "lnN",
                                                 SystMap<channel, bin_id>::init({"tt"},{11}, 1.07));
                cb.cp().process({"ZTT", "ZL", "ZJ", "ZJ_rest", "EWKZ"}).AddSyst(cb,
                                                 "CMS_htt_zmm_norm_extrap_boosted_lt_$ERA", "lnN",
                                                 SystMap<channel, bin_id>::init({"et","mt"},{11,14}, 1.07));
                
                // VBF norm and shape for et/mt/tt
                cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"et","mt"}).bin_id({12,15}).AddSyst(cb,
                                                 "CMS_htt_zmm_norm_extrap_VBF_lt_$ERA", "lnN", SystMap<>::init(1.15));
                cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"tt"}).bin_id({12}).AddSyst(cb,
                                                 "CMS_htt_zmm_norm_extrap_VBF_tt_$ERA", "lnN", SystMap<>::init(1.10));

                if (do_shape_systematics) {
                    cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest"}).channel({"mt","et"}).bin_id({12,15}).AddSyst(cb,
                                                     "CMS_htt_zmumuShape_VBF_$ERA", "shape", SystMap<>::init(1.00));
                    cb.cp().process( {"ZL","ZTT","ZJ", "ZJ_rest", "EWKZ"}).channel({"tt"}).bin_id({12,15}).AddSyst(cb,
                                                     "CMS_htt_zmumuShape_VBF_$ERA", "shape", SystMap<>::init(1.00));
                }

            }
            
        } // end not mm_fit
        
        
        
        
        //  if (control_region == 0) {
        //    // the uncertainty model in the signal region is the classical one
        
        //    }
        

        if (control_region == 1) {
            // Create rateParams for control regions:
            //  - [x] 1 rateParam for all W in every region
            //  - [x] 1 rateParam for QCD in low mT
            //  - [x] 1 rateParam for QCD in high mT
            //  - [x] lnNs for the QCD OS/SS ratio
            //         * should account for stat + syst
            //         * systs should account for: extrap. from anti-iso to iso region,
            //           possible difference between ratio in low mT and high mT (overkill?)
            //  - [x] lnNs for the W+jets OS/SS ratio
            //         * should account for stat only if not being accounted for with bbb,
            //           i.e. because the OS/SS ratio was measured with a relaxed selection
            //         * systs should account for: changes in low/high mT and OS/SS due to JES
            //           and btag (if relevant); OS/SS being wrong in the MC (from enriched data?);
            //           low/high mT being wrong in the MC (fake rate dependence?)
            
            // Going to use the regex filtering to select the right subset of
            // categories for each rateParam
            cb.SetFlag("filters-use-regex", true);
            //      for (auto bin : cb_sig.cp().channel({"et", "mt"}).bin_set()) {
            // Regex that matches, e.g. mt_nobtag or mt_nobtag_X
            
            
            
            cb.cp().bin({"mt_0jet","mt_wjets_0jet_cr"}).process({"W"}).AddSyst(cb, "rate_W_cr_0jet_mt", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"mt_boosted","mt_wjets_boosted_cr","mt_vbf"}).process({"W"}).AddSyst(cb, "rate_W_cr_boosted_mt", "rateParam", SystMap<>::init(1.0));
//            cb.cp().bin({"mt_vbf","mt_wjets_vbf_cr"}).process({"W"}).AddSyst(cb, "rate_W_cr_vbf_mt", "rateParam", SystMap<>::init(1.0));
            
            cb.cp().bin({"et_0jet","et_wjets_0jet_cr"}).process({"W"}).AddSyst(cb, "rate_W_cr_0jet_et", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_boosted","et_wjets_boosted_cr","et_vbf"}).process({"W"}).AddSyst(cb, "rate_W_cr_boosted_et", "rateParam", SystMap<>::init(1.0));
//            cb.cp().bin({"et_vbf","et_wjets_vbf_cr"}).process({"W"}).AddSyst(cb, "rate_W_cr_vbf_et", "rateParam", SystMap<>::init(1.0));
            
            
            cb.cp().bin({"mt_0jet","mt_antiiso_0jet_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_0jet_mt", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"mt_boosted","mt_antiiso_boosted_cr","mt_vbf"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_boosted_mt", "rateParam", SystMap<>::init(1.0));
//            cb.cp().bin({"mt_vbf","mt_antiiso_vbf_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_vbf_mt", "rateParam", SystMap<>::init(1.0));
            
            cb.cp().bin({"et_0jet","et_antiiso_0jet_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_0jet_et", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_boosted","et_antiiso_boosted_cr","et_vbf"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_boosted_et", "rateParam", SystMap<>::init(1.0));
//            cb.cp().bin({"et_vbf","et_antiiso_vbf_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_vbf_et", "rateParam", SystMap<>::init(1.0));
            
            
            //          cb.cp().bin({bin+"(|_0jet)$"}).process({"W"}).AddSyst(cb, "rate_QCD_cr_0jet_"+bin, "rateParam", SystMap<>::init(1.0));
            //          cb.cp().bin({bin+"(|_boosted)$"}).process({"W"}).AddSyst(cb, "rate_W_cr_1jet_"+bin, "rateParam", SystMap<>::init(1.0));
            //          cb.cp().bin({bin+"(|_vbf)$"}).process({"W"}).AddSyst(cb, "rate_W_cr_vbf_"+bin, "rateParam", SystMap<>::init(1.0));
 
            cb.cp().bin({"tt_0jet","tt_0jet_qcd_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_0jet_tt", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_boosted","tt_boosted_qcd_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_boosted_tt", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_vbf","tt_vbf_qcd_cr"}).process({"QCD"}).AddSyst(cb, "rate_QCD_cr_vbf_tt", "rateParam", SystMap<>::init(1.0));
            
            
            //        cb.cp().bin({bin+"(|_.*)$"}).process({"W"}).AddSyst(cb,
            //          "rate_W_cr_"+bin, "rateParam", SystMap<>::init(1.0));
            //
            //        // Regex that matches, e.g. mt_nobtag or mt_nobtag_qcd_cr
            //        cb.cp().bin({bin+"(|_antiiso_)$"}).process({"QCD"}).AddSyst(cb,
            //          "rate_QCD_antiiso_"+bin, "rateParam", SystMap<>::init(1.0));
            
            // Regex that matches, e.g. mt_nobtag_wjets_cr or mt_nobtag_wjets_ss_cr
            //        cb.cp().bin({bin+"_wjets_$"}).process({"QCD"}).AddSyst(cb,
            //          "rate_QCD_highmT_"+bin, "rateParam", SystMap<>::init(1.0));
            //      }
            
            /////////////////
            // Systematics //
            /////////////////

            
            // Should set a sensible range for our rateParams
            for (auto sys : cb.cp().syst_type({"rateParam"}).syst_name_set()) {
                cb.GetParameter(sys)->set_range(0.0, 5.0);
            }
            cb.SetFlag("filters-use-regex", false);
        }
            
        
        if (mm_fit) {
            cb.SetFlag("filters-use-regex", true);
            
            cb.cp().bin({"mt_0jet"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_0jet", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_0jet"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_0jet", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_0jet"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_0jet", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"em_0jet"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_0jet", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"mm_0jet"}).process({"ZL"}).AddSyst(cb, "rate_mm_ZTT_0jet", "rateParam", SystMap<>::init(1.0));
            
            cb.cp().bin({"mt_boosted"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_boosted", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_boosted"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_boosted", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_boosted"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_boosted", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"em_boosted"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_boosted", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"mm_1jet"}).process({"ZL"}).AddSyst(cb, "rate_mm_ZTT_1jet", "rateParam", SystMap<>::init(1.0));
            
            cb.cp().bin({"mt_vbf"}).process({"ZTT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_vbf"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_vbf", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_vbf"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_vbf", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"em_vbf"}).process({"ZTT"}).AddSyst(cb, "rate_mm_ZTT_vbf", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"mm_vbf"}).process({"ZL"}).AddSyst(cb, "rate_mm_ZTT_vbf", "rateParam", SystMap<>::init(1.0));
            
            cb.GetParameter("rate_mm_ZTT_0jet")->set_range(0.9, 1.1);
            cb.GetParameter("rate_mm_ZTT_1jet")->set_range(0.9, 1.1);
            cb.GetParameter("rate_mm_ZTT_vbf")->set_range(0.9, 1.1);
            
            cb.SetFlag("filters-use-regex", false);
        }
        
        if (ttbar_fit) {
            cb.SetFlag("filters-use-regex", true);
            
            cb.cp().bin({"mt_0jet"}).process({"TTJ","TTT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_0jet"}).process({"TTJ","TTT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_0jet"}).process({"TTJ","TTT","TTJ_rest"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"em_0jet"}).process({"TT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            
            
            cb.cp().bin({"mt_boosted"}).process({"TTJ","TTT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_boosted"}).process({"TTJ","TTT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_boosted"}).process({"TTJ","TTT","TTJ_rest"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"em_boosted"}).process({"TT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            
            
            cb.cp().bin({"mt_vbf"}).process({"TTJ","TTT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"et_vbf"}).process({"TTJ","TTT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"tt_vbf"}).process({"TTJ","TTT","TTJ_rest"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            cb.cp().bin({"em_vbf"}).process({"TT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            
            cb.cp().bin({"ttbar_all"}).process({"TT"}).AddSyst(cb, "rate_ttbar", "rateParam", SystMap<>::init(1.0));
            
            cb.GetParameter("rate_ttbar")->set_range(0.80, 1.20);
            
            cb.SetFlag("filters-use-regex", false);
        }

        //jet fakes: shape uncertainties
        if (do_shape_systematics) {
            cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_norm_ff_qcd_1prong_njet0_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_norm_ff_qcd_1prong_njet1_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_norm_ff_qcd_3prong_njet0_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_norm_ff_qcd_3prong_njet1_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));

            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_w_1prong_njet0_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_w_1prong_njet1_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_w_3prong_njet0_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_w_3prong_njet1_$CHANNEL_stat_13TeV", "shape", SystMap<>::init(1.00));

            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_tt_1prong_njet0_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_tt_1prong_njet1_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_tt_3prong_njet0_stat_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_tt_3prong_njet1_stat_13TeV", "shape", SystMap<>::init(1.00));

            cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_norm_ff_qcd_$CHANNEL_syst_13TeV", "shape", SystMap<>::init(1.00));
            
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_w_syst_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"mt","et"}).AddSyst(cb, "CMS_htt_norm_ff_tt_syst_13TeV", "shape", SystMap<>::init(1.00));
            
            cb.cp().process({"jetFakes"}).channel({"tt"}).AddSyst(cb, "CMS_htt_norm_ff_w_$CHANNEL_syst_13TeV", "shape", SystMap<>::init(1.00));
            cb.cp().process({"jetFakes"}).channel({"tt"}).AddSyst(cb, "CMS_htt_norm_ff_ttbar_$CHANNEL_syst_13TeV", "shape", SystMap<>::init(1.00));
        }

        //jet fakes: stat norm unc
        cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_ff_norm_stat_$CHANNEL_$BIN_13TeV", "lnN", SystMap<channel, bin_id>::init
                                                                        ({"mt"}, {1}, 1.04)
                                                                        ({"mt"}, {2}, 1.03)
                                                                        ({"mt"}, {3}, 1.045)
                                                                        ({"et"}, {1}, 1.04)
                                                                        ({"et"}, {2}, 1.05)
                                                                        ({"et"}, {3}, 1.065)
                                                                        ({"tt"}, {1}, 1.03)
                                                                        ({"tt"}, {2}, 1.04)
                                                                        ({"tt"}, {3}, 1.05)
                                                                        );
        //jet fakes: syst norm: bin-correlated
        cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_ff_norm_syst_$CHANNEL_13TeV", "lnN", SystMap<channel, bin_id>::init
                                                                        ({"mt"}, {1}, 1.065)
                                                                        ({"mt"}, {2}, 1.062)
                                                                        ({"mt"}, {3}, 1.078)
                                                                        ({"et"}, {1}, 1.073)
                                                                        ({"et"}, {2}, 1.067)
                                                                        ({"et"}, {3}, 1.083)
                                                                        ({"tt"}, {1}, 1.026)
                                                                        ({"tt"}, {2}, 1.032)
                                                                        ({"tt"}, {3}, 1.040)
                                                                        );
        //jet fakes: syst norm: bin-dependent
        cb.cp().process({"jetFakes"}).channel({"mt","et","tt"}).AddSyst(cb, "CMS_htt_ff_sub_syst_$CHANNEL_$BIN_13TeV", "lnN", SystMap<channel, bin_id>::init
                                                                        ({"mt"}, {1}, 1.06)
                                                                        ({"mt"}, {2}, 1.04)
                                                                        ({"mt"}, {3}, 1.04)
                                                                        ({"et"}, {1}, 1.06)
                                                                        ({"et"}, {2}, 1.04)
                                                                        ({"et"}, {3}, 1.04)
                                                                        ({"tt"}, {1}, 1.06)
                                                                        ({"tt"}, {2}, 1.04)
                                                                        );

        
    }
}
