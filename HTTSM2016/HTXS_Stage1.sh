
newFolder=Stage1_20171110_htxs

# Stage 1
MorphingSM2016 --output_folder=${newFolder} --postfix="-2D-HTXS" --do_nominal_signals=false --do_stage1_signals=true --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true


### Building the workspaces:
cd output/${newFolder}
combineTool.py -M T2W -i {cmb,em,et,mt,tt}/* -o workspace.root --parallel 12



### Per HTXS Stage1 breakdown
### qqH_FWDH_htt == zero yield
### ZH_lep_FWDH_htt == zero yield
##
### two processes, VH_FWDH and WH_FWDH have 0.004 event expected yield and are a mess
###    --PO 'map=htt_tt_.*_13TeV/VH_had_FWDH_htt:r_VH_had_FWDH[1,-50000,50000]' \
###    --PO 'map=htt_tt_.*_13TeV/WH_lep_FWDH_htt:r_WH_lep_FWDH[1,-50000,50000]' \

text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
    --PO 'map=htt_.*_.*_13TeV/ggH_VBFTOPO_JET3VETO_htt:r_ggH_VBFTOPO_JET3VETO[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_VBFTOPO_JET3_htt:r_ggH_VBFTOPO_JET3[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_0J_htt:r_ggH_0J[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_0_60_htt:r_ggH_1J_PTH_0_60[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_60_120_htt:r_ggH_1J_PTH_60_120[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_120_200_htt:r_ggH_1J_PTH_120_200[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_GT200_htt:r_ggH_1J_PTH_GT200[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_0_60_htt:r_ggH_GE2J_PTH_0_60[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_60_120_htt:r_ggH_GE2J_PTH_60_120[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_120_200_htt:r_ggH_GE2J_PTH_120_200[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_GT200_htt:r_ggH_GE2J_PTH_GT200[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_VBFTOPO_JET3VETO_htt:r_qqH_had_VBFTOPO_JET3VETO[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_VBFTOPO_JET3_htt:r_qqH_had_VBFTOPO_JET3[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_VH2JET_htt:r_qqH_had_VH2JET[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_REST_htt:r_qqH_had_REST[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_PTJET1_GT200_htt:r_qqH_had_PTJET1_GT200[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_VBFTOPO_JET3VETO_htt:r_WH_had_VBFTOPO_JET3VETO[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_VBFTOPO_JET3_htt:r_WH_had_VBFTOPO_JET3[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_VH2JET_htt:r_WH_had_VH2JET[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_REST_htt:r_WH_had_REST[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_PTJET1_GT200_htt:r_WH_had_PTJET1_GT200[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_0_150_htt:r_WH_lep_PTV_0_150[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_150_250_0J_htt:r_WH_lep_PTV_150_250_0J[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_150_250_GE1J_htt:r_WH_lep_PTV_150_250_GE1J[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_GT250_htt:r_WH_lep_PTV_GT250[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_VBFTOPO_JET3VETO_htt:r_ZH_had_VBFTOPO_JET3VETO[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_VBFTOPO_JET3_htt:r_ZH_had_VBFTOPO_JET3[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_VH2JET_htt:r_ZH_had_VH2JET[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_REST_htt:r_ZH_had_REST[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_PTJET1_GT200_htt:r_ZH_had_PTJET1_GT200[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_0_150_htt:r_ZH_lep_PTV_0_150[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_150_250_0J_htt:r_ZH_lep_PTV_150_250_0J[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_150_250_GE1J_htt:r_ZH_lep_PTV_150_250_GE1J[1,-5000,5000]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_GT250_htt:r_ZH_lep_PTV_GT250[1,-5000,5000]' \
    cmb/125/combined.txt.cmb -o workspace_htxs_stage1_breakdown.root -m 125

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n Cmb &

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n CmbBlind -t -1 --expectSignal=1 &

#combine -M MultiDimFit workspace_htxs_stage1_breakdown.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n Multi &

#combine -M MultiDimFit workspace_htxs_stage1_breakdown.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n MultiBlind -t -1 --expectSignal=1 &









### All signals in row
### r_ggH_VBFTOPO_JET3VETO r_ggH_VBFTOPO_JET3 r_ggH_0J r_ggH_1J_PTH_0_60 r_ggH_1J_PTH_60_120 r_ggH_1J_PTH_120_200 r_ggH_1J_PTH_GT200 r_ggH_GE2J_PTH_0_60 r_ggH_GE2J_PTH_60_120 r_ggH_GE2J_PTH_120_200 r_ggH_GE2J_PTH_GT200 r_qqH_VBFTOPO_JET3VETO r_qqH_VBFTOPO_JET3 r_qqH_VH2JET r_qqH_REST r_qqH_PTJET1_GT200 r_VH_had_FWDH r_VH_had_VBFTOPO_JET3VETO r_VH_had_VBFTOPO_JET3 r_VH_had_VH2JET r_VH_had_REST r_VH_had_PTJET1_GT200 r_WH_lep_FWDH r_WH_lep_PTV_0_150 r_WH_lep_PTV_150_250_0J r_WH_lep_PTV_150_250_GE1J r_WH_lep_PTV_GT250 r_ZH_lep_PTV_0_150 r_ZH_lep_PTV_150_250_0J r_ZH_lep_PTV_150_250_GE1J r_ZH_lep_PTV_GT250
##
##
### All signal mus merged qqH with VH
### r_ggH_VBFTOPO_JET3VETO r_ggH_VBFTOPO_JET3 r_ggH_0J r_ggH_1J_PTH_0_60 r_ggH_1J_PTH_60_120 r_ggH_1J_PTH_120_200 r_ggH_1J_PTH_GT200 r_ggH_GE2J_PTH_0_60 r_ggH_GE2J_PTH_60_120 r_ggH_GE2J_PTH_120_200 r_ggH_GE2J_PTH_GT200 r_VH_had_FWDH r_VH_had_VBFTOPO_JET3VETO r_VH_had_VBFTOPO_JET3 r_VH_had_VH2JET r_VH_had_REST r_VH_had_PTJET1_GT200 r_WH_lep_FWDH r_WH_lep_PTV_0_150 r_WH_lep_PTV_150_250_0J r_WH_lep_PTV_150_250_GE1J r_WH_lep_PTV_GT250 r_ZH_lep_PTV_0_150 r_ZH_lep_PTV_150_250_0J r_ZH_lep_PTV_150_250_GE1J r_ZH_lep_PTV_GT250
# r_ggH_VBFTOPO_JET3VETO=1,r_ggH_VBFTOPO_JET3=1,r_ggH_0J=1,r_ggH_1J_PTH_0_60=1,r_ggH_1J_PTH_60_120=1,r_ggH_1J_PTH_120_200=1,r_ggH_1J_PTH_GT200=1,r_ggH_GE2J_PTH_0_60=1,r_ggH_GE2J_PTH_60_120=1,r_ggH_GE2J_PTH_120_200=1,r_ggH_GE2J_PTH_GT200=1,r_VH_had_FWDH=1,r_VH_had_VBFTOPO_JET3VETO=1,r_VH_had_VBFTOPO_JET3=1,r_VH_had_VH2JET=1,r_VH_had_REST=1,r_VH_had_PTJET1_GT200=1,r_WH_lep_FWDH=1,r_WH_lep_PTV_0_150=1,r_WH_lep_PTV_150_250_0J=1,r_WH_lep_PTV_150_250_GE1J=1,r_WH_lep_PTV_GT250=1,r_ZH_lep_PTV_0_150=1,r_ZH_lep_PTV_150_250_0J=1,r_ZH_lep_PTV_150_250_GE1J=1,r_ZH_lep_PTV_GT250=1



################
#### BLINDED ###
################
## Run for each signal in tautau
## Removed FWDH for the moment, the signals are too small
#for SIGNAL in r_VH_had_FWDH r_WH_lep_FWDH; do
#    echo ""
#    echo "group: All Channels Signal " ${SIGNAL}
#    combine -M MaxLikelihoodFit workspace_htxs_stage1_breakdown.root  -t -1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -50000 --rMax 50000  --setPhysicsModelParameters r_ggH_VBFTOPO_JET3VETO=1,r_ggH_VBFTOPO_JET3=1,r_ggH_0J=1,r_ggH_1J_PTH_0_60=1,r_ggH_1J_PTH_60_120=1,r_ggH_1J_PTH_120_200=1,r_ggH_1J_PTH_GT200=1,r_ggH_GE2J_PTH_0_60=1,r_ggH_GE2J_PTH_60_120=1,r_ggH_GE2J_PTH_120_200=1,r_ggH_GE2J_PTH_GT200=1,r_VH_had_FWDH=1,r_VH_had_VBFTOPO_JET3VETO=1,r_VH_had_VBFTOPO_JET3=1,r_VH_had_VH2JET=1,r_VH_had_REST=1,r_VH_had_PTJET1_GT200=1,r_WH_lep_FWDH=1,r_WH_lep_PTV_0_150=1,r_WH_lep_PTV_150_250_0J=1,r_WH_lep_PTV_150_250_GE1J=1,r_WH_lep_PTV_GT250=1,r_ZH_lep_PTV_0_150=1,r_ZH_lep_PTV_150_250_0J=1,r_ZH_lep_PTV_150_250_GE1J=1,r_ZH_lep_PTV_GT250=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}_blind
#done
#
##for SIGNAL in r_ggH_VBFTOPO_JET3VETO r_ggH_VBFTOPO_JET3 r_ggH_0J r_ggH_1J_PTH_0_60 r_ggH_1J_PTH_60_120 r_ggH_1J_PTH_120_200 r_ggH_1J_PTH_GT200 r_ggH_GE2J_PTH_0_60 r_ggH_GE2J_PTH_60_120 r_ggH_GE2J_PTH_120_200 r_ggH_GE2J_PTH_GT200 r_VH_had_VBFTOPO_JET3VETO r_VH_had_VBFTOPO_JET3 r_VH_had_VH2JET r_VH_had_REST r_VH_had_PTJET1_GT200 r_WH_lep_PTV_0_150 r_WH_lep_PTV_150_250_0J r_WH_lep_PTV_150_250_GE1J r_WH_lep_PTV_GT250 r_ZH_lep_PTV_0_150 r_ZH_lep_PTV_150_250_0J r_ZH_lep_PTV_150_250_GE1J r_ZH_lep_PTV_GT250; do
##    echo ""
##    echo "group: All Channels Signal " ${SIGNAL}
##    combine -M MaxLikelihoodFit workspace_htxs_stage1_breakdown.root  -t -1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -5000 --rMax 5000  --setPhysicsModelParameters r_ggH_VBFTOPO_JET3VETO=1,r_ggH_VBFTOPO_JET3=1,r_ggH_0J=1,r_ggH_1J_PTH_0_60=1,r_ggH_1J_PTH_60_120=1,r_ggH_1J_PTH_120_200=1,r_ggH_1J_PTH_GT200=1,r_ggH_GE2J_PTH_0_60=1,r_ggH_GE2J_PTH_60_120=1,r_ggH_GE2J_PTH_120_200=1,r_ggH_GE2J_PTH_GT200=1,r_VH_had_VBFTOPO_JET3VETO=1,r_VH_had_VBFTOPO_JET3=1,r_VH_had_VH2JET=1,r_VH_had_REST=1,r_VH_had_PTJET1_GT200=1,r_WH_lep_PTV_0_150=1,r_WH_lep_PTV_150_250_0J=1,r_WH_lep_PTV_150_250_GE1J=1,r_WH_lep_PTV_GT250=1,r_ZH_lep_PTV_0_150=1,r_ZH_lep_PTV_150_250_0J=1,r_ZH_lep_PTV_150_250_GE1J=1,r_ZH_lep_PTV_GT250=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}_blind
##done
#
## Fit CMB
#combine -M MaxLikelihoodFit   tt/125/workspace.root  -t -1 --expectSignal=1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --rMin -5 --rMax 5 -n _Mu_r_cmb_blind
#### END BLINDED ###
#
#
