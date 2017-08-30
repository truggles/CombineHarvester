
# See: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit#MultiSignalModel_ready_made_mode


newFolder=Blinded20170828_htxs_nomNewDC

# Nominal
MorphingSM2016 --output_folder=${newFolder} --postfix="-2D" --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true


### Building the workspaces:
cd output/${newFolder}
combineTool.py -M T2W -i {cmb,em,et,mt,tt}/* -o workspace.root --parallel 12



## Per channel specific workspace
#text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
#    --PO 'map=htt_em_.*_13TeV/.*H_htt:r_em[1,0,5]' \
#    --PO 'map=htt_et_.*_13TeV/.*H_htt:r_et[1,0,5]' \
#    --PO 'map=htt_mt_.*_13TeV/.*H_htt:r_mt[1,0,5]' \
#    --PO 'map=htt_tt_.*_13TeV/.*H_htt:r_tt[1,0,5]' \
#    cmb/125/combined.txt.cmb -o workspace_channel_breakdown.root -m 125
#
## Per category specific workspace
#text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
#    --PO 'map=htt_.*_1_13TeV/.*H_htt:r_0jet[1,0,5]' \
#    --PO 'map=htt_.*_2_13TeV/.*H_htt:r_boosted[1,0,5]' \
#    --PO 'map=htt_.*_3_13TeV/.*H_htt:r_vbf[1,0,5]' \
#    cmb/125/combined.txt.cmb -o workspace_category_breakdown.root -m 125
#
## Per background R
#text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
#    --PO 'map=htt_.*_13TeV/ggH_htt:r_ggH[1,-10,10]' \
#    --PO 'map=htt_.*_13TeV/qqH_htt:r_qqH[1,-10,10]' \
#    --PO 'map=htt_.*_13TeV/WH_htt:r_WH[1,-10,10]' \
#    --PO 'map=htt_.*_13TeV/ZH_htt:r_ZH[1,-10,10]' \
#    cmb/125/combined.txt.cmb -o workspace_per_background_breakdown.root -m 125

# Per background R tautau
text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
    --PO 'map=htt_tt_.*_13TeV/ggH_htt:r_ggH[1,-50,50]' \
    --PO 'map=htt_tt_.*_13TeV/qqH_htt:r_qqH[1,-50,50]' \
    --PO 'map=htt_tt_.*_13TeV/WH_htt:r_WH[1,-50,50]' \
    --PO 'map=htt_tt_.*_13TeV/ZH_htt:r_ZH[1,-50,50]' \
    tt/125/combined.txt.cmb -o workspace_per_background_breakdown_tt.root -m 125

# Run for each signal in tautau
for SIGNAL in r_ggH r_qqH r_WH r_ZH; do
    echo ""
    echo "group: TauTau Signal " ${SIGNAL}
    combine -M MaxLikelihoodFit   workspace_per_background_breakdown_tt.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -50 --rMax 50  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_WH=1,r_ZH=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}
done


## Run for each signal 
#for SIGNAL in r_ggH r_qqH r_WH r_ZH; do
#    echo ""
#    echo "group: Signal " ${SIGNAL}
#    combine -M MaxLikelihoodFit   workspace_per_background_breakdown.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -10 --rMax 10  --setPhysicsModelParameters r_ggH=1,r_qqH=1,r_WH=1,r_ZH=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}
#done
#
#
## Run for each channel
#for SIGNAL in r_em r_et r_mt r_tt; do
#    echo ""
#    echo "group: Channel " ${SIGNAL}
#    combine -M MaxLikelihoodFit   workspace_channel_breakdown.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -5 --rMax 5  --setPhysicsModelParameters r_em=1,r_et=1,r_mt=1,r_tt=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}
#done
#
## Run for each category
#for SIGNAL in r_0jet r_boosted r_vbf; do
#    echo ""
#    echo "group: Category " ${SIGNAL}
#    combine -M MaxLikelihoodFit   workspace_category_breakdown.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -5 --rMax 5  --setPhysicsModelParameters r_0jet=1,r_boosted=1,r_vbf=1 --redefineSignalPOIs ${SIGNAL} -n _Mu_${SIGNAL}
#done
#
#echo ""
#echo "group: Combined"
#combine -M MaxLikelihoodFit  cmb/125/workspace.root  --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin -5 --rMax 5  -n _Mu_Combined





