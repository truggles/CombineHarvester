
newFolder=Stage0_20171116_htxs

# Stage 0
MorphingSM2016 --output_folder=${newFolder} --postfix="-2D-HTXS" --do_nominal_signals=false  --do_stage0_signals=true --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true


cd output/${newFolder}

echo "Adding autoMCStats"
for i in ./*/*/*.txt
do
    echo "* autoMCStats 0" >> $i
done

### Building the workspaces:
combineTool.py -M T2W -i {cmb,em,et,mt,tt}/* -o workspace.root --parallel 12



text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
    --PO 'map=htt_.*_.*_13TeV/ggH_htt:r_ggH[1,-20,20]' \
    --PO 'map=htt_.*_.*_13TeV/qqH_htt:r_qqH[1,-20,20]' \
    --PO 'map=htt_.*_.*_13TeV/WH_had_htt:r_WH_had[1,-100,100]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_had_htt:r_ZH_had[1,-100,100]' \
    --PO 'map=htt_.*_.*_13TeV/WH_lep_htt:r_WH_lep[1,-500,500]' \
    --PO 'map=htt_.*_.*_13TeV/ZH_lep_htt:r_ZH_lep[1,-500,500]' \
    cmb/125/combined.txt.cmb -o workspace_htxs_stage0_breakdown.root -m 125

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --algo=singles --cl=0.68 -n Cmb &

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --algo=singles --cl=0.68 -n CmbBlind -t -1 --expectSignal=1 &


#combine -M MultiDimFit workspace_htxs_stage0_breakdown.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --algo=singles --cl=0.68 -n Multi &

#combine -M MultiDimFit workspace_htxs_stage0_breakdown.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --algo=singles --cl=0.68 -n MultiBlind -t -1 --expectSignal=1 &


