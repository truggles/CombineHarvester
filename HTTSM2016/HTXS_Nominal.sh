
# See: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit#MultiSignalModel_ready_made_mode


newFolder=Nominal20171116_htxs2

# Nominal
MorphingSM2016 --output_folder=${newFolder} --postfix="-2D-HTXS" --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true --do_all_masses=false


cd output/${newFolder}

echo "Adding autoMCStats"
for i in ./*/*/*.txt
do
    echo "* autoMCStats 0" >> $i
done

### Building the workspaces:
combineTool.py -M T2W -i {cmb,em,et,mt,tt}/* -o workspace.root --parallel 12


# Per signal 
#text2workspace.py -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
#    --PO 'map=htt_.*_.*_13TeV/ggH_htt:r_ggH[1,-25,25]' \
#    --PO 'map=htt_.*_.*_13TeV/qqH_htt:r_qqH[1,-25,25]' \
#    --PO 'map=htt_.*_.*_13TeV/WH_htt:r_WH[1,-25,25]' \
#    --PO 'map=htt_.*_.*_13TeV/ZH_htt:r_ZH[1,-25,25]' \
#    cmb/125/combined.txt.cmb -o workspace_per_signal_breakdown_cmb.root -m 125

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --algo=singles --cl=0.68 -n Cmb &

combine -M MultiDimFit cmb/125/workspace.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --algo=singles --cl=0.68 -n CmbBlind -t -1 --expectSignal=1 &


#combine -M MultiDimFit workspace_per_signal_breakdown_cmb.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n Multi &

#combine -M MultiDimFit workspace_per_signal_breakdown_cmb.root --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad --algo=singles --cl=0.68 -n MultiBlind -t -1 --expectSignal=1 &


