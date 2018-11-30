
## Nominal
#pushd htxs_nom_fits
#for SIGNAL in r_ggH r_qqH r_WH r_ZH r_cmb; do
#    combineTool.py -M CollectLimits higgsCombine_Mu_${SIGNAL}.FitDiagnostics.mH120.root -o Mu_${SIGNAL}.json
#    combineTool.py -M CollectLimits higgsCombine_Mu_${SIGNAL}_blind.FitDiagnostics.mH120.root -o Mu_${SIGNAL}_blind.json
#done
#popd

### Stage0
##pushd htxs_s0_fits
##for SIGNAL in r_VH_had r_VH_had_fwd r_WH_lep r_WH_lep_fwd r_ZH_lep r_cmb r_ggH r_qqH wFrozen_r_VH_had wFrozen_r_ZH_lep wFrozen_r_ggH wFrozen_r_qqH; do
for SIGNAL in r_VH_had r_VH_had_fwd r_WH_lep r_WH_lep_fwd r_ZH_lep r_cmb r_ggH r_qqH; do
    combineTool.py -M CollectLimits higgsCombine_Mu_${SIGNAL}.FitDiagnostics.mH120.root -o Mu_${SIGNAL}.json
    combineTool.py -M CollectLimits higgsCombine_Mu_${SIGNAL}_blind.FitDiagnostics.mH120.root -o Mu_${SIGNAL}_blind.json
done
##popd

# python ../scripts/muValuePlotter.py  -o Mu_Signal_Stage0 -t signal_s0

# python ../scripts/muValuePlotter.py  -o Mu_Signal_Nom -t signal_nom
# python ../scripts/muValuePlotter.py  -o Mu_Signal_Nom_Blind -t signal_nom --blind
# python ../scripts/muValuePlotter.py  -o Mu_Signal_S0 -t signal_s0
# python ../scripts/muValuePlotter.py  -o Mu_Signal_S0_Blind -t signal_s0 --blind
