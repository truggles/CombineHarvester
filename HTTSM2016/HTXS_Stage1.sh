
newFolder=Stage1_20171110_htxs

# Stage 1
MorphingSM2016 --output_folder=${newFolder} --postfix="-2D-HTXS" --do_nominal_signals=false --do_stage1_signals=true --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true --do_all_masses=false


cd output/${newFolder}

echo "Adding autoMCStats"
for i in ./*/*/*.txt
do
    echo "* autoMCStats 0" >> $i
done

### Building the workspaces:
combineTool.py -M T2W -i {cmb,em,et,mt,tt}/* -o workspace.root --parallel 12



### Per HTXS Stage1 breakdown
### qqH_FWDH_htt == zero yield
### ZH_lep_FWDH_htt == zero yield
##
### two processes, VH_FWDH and WH_FWDH have 0.004 event expected yield and are a mess
###    --PO 'map=htt_tt_.*_13TeV/VH_had_FWDH_htt:r_VH_had_FWDH[1,-50000,50000]' \
###    --PO 'map=htt_tt_.*_13TeV/WH_lep_FWDH_htt:r_WH_lep_FWDH[1,-50000,50000]' \

text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
    --PO 'map=htt_.*_.*_13TeV/ggH_VBFTOPO_JET3VETO_htt:r_ggH_VBFTOPO_JET3VETO[1,-100,100]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_VBFTOPO_JET3_htt:r_ggH_VBFTOPO_JET3[1,-50,50]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_0J_htt:r_ggH_0J[1,-10,10]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_0_60_htt:r_ggH_1J_PTH_0_60[1,-100,100]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_60_120_htt:r_ggH_1J_PTH_60_120[1,-50,50]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_120_200_htt:r_ggH_1J_PTH_120_200[1,-50,50]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_1J_PTH_GT200_htt:r_ggH_1J_PTH_GT200[1,-25,25]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_0_60_htt:r_ggH_GE2J_PTH_0_60[1,-100,100]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_60_120_htt:r_ggH_GE2J_PTH_60_120[1,-25,25]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_120_200_htt:r_ggH_GE2J_PTH_120_200[1,-25,25]' \
    --PO 'map=htt_.*_.*_13TeV/ggH_GE2J_PTH_GT200_htt:r_ggH_GE2J_PTH_GT200[1,-25,25]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_VBFTOPO_JET3VETO_htt:r_qqH_had_VBFTOPO_JET3VETO[1,-25,25]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_VBFTOPO_JET3_htt:r_qqH_had_VBFTOPO_JET3[1,-50,50]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_VH2JET_htt:r_qqH_had_VH2JET[1,-100,100]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_REST_htt:r_qqH_had_REST[1,-75,75]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_PTJET1_GT200_htt:r_qqH_had_PTJET1_GT200[1,-25,25]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_VBFTOPO_JET3VETO_htt:r_WH_had_VBFTOPO_JET3VETO[1,-1000,1000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_VBFTOPO_JET3_htt:r_WH_had_VBFTOPO_JET3[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_VH2JET_htt:r_WH_had_VH2JET[1,-200,200]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_REST_htt:r_WH_had_REST[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_PTJET1_GT200_htt:r_WH_had_PTJET1_GT200[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_0_150_htt:r_WH_lep_PTV_0_150[1,-250,250]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_150_250_0J_htt:r_WH_lep_PTV_150_250_0J[1,-1000,1000]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_150_250_GE1J_htt:r_WH_lep_PTV_150_250_GE1J[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_PTV_GT250_htt:r_WH_lep_PTV_GT250[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_VBFTOPO_JET3VETO_htt:r_ZH_had_VBFTOPO_JET3VETO[1,-1500,1500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_VBFTOPO_JET3_htt:r_ZH_had_VBFTOPO_JET3[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_VH2JET_htt:r_ZH_had_VH2JET[1,-300,300]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_REST_htt:r_ZH_had_REST[1,-200,200]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_PTJET1_GT200_htt:r_ZH_had_PTJET1_GT200[1,-2500,2500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_0_150_htt:r_ZH_lep_PTV_0_150[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_150_250_0J_htt:r_ZH_lep_PTV_150_250_0J[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_150_250_GE1J_htt:r_ZH_lep_PTV_150_250_GE1J[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_PTV_GT250_htt:r_ZH_lep_PTV_GT250[1,-200,200]' \
    cmb/125/combined.txt.cmb -o workspace_htxs_stage1_breakdown.root -m 125

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n Cmb &

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n CmbBlind -t -1 --expectSignal=1 &

combine -M MultiDimFit workspace_htxs_stage1_breakdown.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n Multi2 &

combine -M MultiDimFit workspace_htxs_stage1_breakdown.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n MultiBlind2 -t -1 --expectSignal=1 &







