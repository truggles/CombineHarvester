
newFolder=Blinded20170828_htxs_s0

# Stage 0
MorphingSM2016 --output_folder=${newFolder} --postfix="-2D-HTXS" --do_nominal_signals=false  --do_stage0_signals=true --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true


### Building the workspaces:
cd output/${newFolder}
combineTool.py -M T2W -i {cmb,em,et,mt,tt}/* -o workspace.root --parallel 12



# Per HTXS Stage0 breakdown, very similar to per background with WH and ZH split
# following are shapes with zero yield
#    --PO 'map=htt_tt_.*_13TeV/ggH_fwd_htt:r_ggH_fwd[1,-100,100]' \
#    --PO 'map=htt_tt_.*_13TeV/qqH_fwd_htt:r_qqH_fwd[1,-100,100]' \
#    --PO 'map=htt_tt_.*_13TeV/ZH_lep_fwd_htt:r_ZH_lep_fwd[1,-100,100]' \
text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
    --PO 'map=htt_tt_.*_13TeV/ggH_htt:r_ggH[1,-100,100]' \
    --PO 'map=htt_tt_.*_13TeV/qqH_htt:r_qqH[1,-100,100]' \
    --PO 'map=htt_tt_.*_13TeV/VH_had_fwd_htt:r_VH_had_fwd[1,-50000,50000]' \
    --PO 'map=htt_tt_.*_13TeV/VH_had_htt:r_VH_had[1,-100,100]' \
    --PO 'map=htt_tt_.*_13TeV/WH_lep_fwd_htt:r_WH_lep_fwd[1,-50000,50000]' \
    --PO 'map=htt_tt_.*_13TeV/WH_lep_htt:r_WH_lep[1,-50000,50000]' \
    --PO 'map=htt_tt_.*_13TeV/ZH_lep_htt:r_ZH_lep[1,-5000,5000]' \
    tt/125/combined.txt.cmb -o workspace_htxs_stage0_breakdown.root -m 125


###############
### BLINDED ###
###############
# Run for each signal in tautau
for SIGNAL in r_VH_had_fwd r_WH_lep_fwd r_WH_lep; do
    echo ""
    echo "group: TauTau Signal " ${SIGNAL}
    combine -M MaxLikelihoodFit   workspace_htxs_stage0_breakdown.root  -t -1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -50000 --rMax 50000  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_VH_had_fwd=1,r_VH_had=1,r_WH_lep_fwd=1,r_WH_lep=1,r_ZH_lep=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}_blind
done

# Run for each signal in tautau
for SIGNAL in r_ggH r_qqH r_VH_had; do
    echo ""
    echo "group: TauTau Signal " ${SIGNAL}
    combine -M MaxLikelihoodFit   workspace_htxs_stage0_breakdown.root  -t -1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -100 --rMax 100  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_VH_had_fwd=1,r_VH_had=1,r_WH_lep_fwd=1,r_WH_lep=1,r_ZH_lep=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}_blind
done

# Run for each signal in tautau
for SIGNAL in r_ZH_lep; do
    echo ""
    echo "group: TauTau Signal " ${SIGNAL}
    combine -M MaxLikelihoodFit   workspace_htxs_stage0_breakdown.root  -t -1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -5000 --rMax 5000  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_VH_had_fwd=1,r_VH_had=1,r_WH_lep_fwd=1,r_WH_lep=1,r_ZH_lep=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}_blind
done

# Fit CMB
combine -M MaxLikelihoodFit   tt/125/workspace.root  -t -1 --expectSignal=1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad -n _Mu_r_cmb_blind
### END BLINDED ###



##################
### Un-BLINDED ###
##################
# Run for each signal in tautau
for SIGNAL in r_VH_had_fwd r_WH_lep_fwd r_WH_lep; do
    echo ""
    echo "group: TauTau Signal " ${SIGNAL}
    combine -M MaxLikelihoodFit   workspace_htxs_stage0_breakdown.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -50000 --rMax 50000  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_VH_had_fwd=1,r_VH_had=1,r_WH_lep_fwd=1,r_WH_lep=1,r_ZH_lep=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}
done

# Run for each signal in tautau
for SIGNAL in r_ggH r_qqH r_VH_had r_ZH_lep; do
    echo ""
    echo "group: TauTau Signal " ${SIGNAL}
    combine -M MaxLikelihoodFit   workspace_htxs_stage0_breakdown.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -100 --rMax 100  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_VH_had_fwd=1,r_VH_had=1,r_WH_lep_fwd=1,r_WH_lep=1,r_ZH_lep=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}
done

# Fit CMB
combine -M MaxLikelihoodFit   tt/125/workspace.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad -n _Mu_r_cmb



#### FREEZE SOME POORLY BEHAVING SIGNALS ###
#for SIGNAL in r_ggH r_qqH r_VH_had r_ZH_lep; do
#    echo ""
#    echo "group: TauTau Signal " ${SIGNAL}
#    combine -M MaxLikelihoodFit   workspace_htxs_stage0_breakdown.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -100 --rMax 100  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_VH_had_fwd=1,r_VH_had=1,r_WH_lep_fwd=1,r_WH_lep=1,r_ZH_lep=1 --redefineSignalPOIs ${SIGNAL} --freezeNuisances r_VH_had_fwd,r_WH_lep_fwd,r_WH_lep -n _Mu_wFrozen_${SIGNAL}
#done


