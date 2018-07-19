# htt_zh.inputs-mssm-13TeV_AMass_new.root
# htt_zh.inputs-sm-13TeV_svFitMass_new.root

#### Standard Work Flow
### creating datacards
newFolder=Blinded20180719_azh
MorphingZH2016 --output_folder=${newFolder} --postfix="_AMassConst_new" --input_folder="July19"
##### Building the workspaces:
cd output/${newFolder}

#for MASS in 220; do # 240 260 280 300 320 340 350 400; do
#    
#    for CHANS in eeet eemt eett eeem emmt mmmt mmtt emmm; do
#
#        echo ""
#        echo ${MASS}
#        echo ${CHANS}
#        echo ""
#
#        combineTool.py -M T2W -i ${CHANS}/${MASS}/htt_${CHANS}_1_13TeV.txt -m ${MASS} -o ${CHANS}_${MASS}_workspace.root
#        combineTool.py -M Asymptotic -m ${MASS} -t -1 ${CHANS}/${MASS}/${CHANS}_${MASS}_workspace.root -n .limit_${CHANS}
#    done
#done
for MASS in 220 240 260 280 300 320 340 350 400; do
    combineCards.py {eeet,emmt}/${MASS}/htt_*_1_13TeV.txt > llet_${MASS}_cmb.txt
    combineCards.py {eemt,mmmt}/${MASS}/htt_*_1_13TeV.txt > llmt_${MASS}_cmb.txt
    combineCards.py {eett,mmtt}/${MASS}/htt_*_1_13TeV.txt > lltt_${MASS}_cmb.txt
    combineCards.py {eeem,emmm}/${MASS}/htt_*_1_13TeV.txt > llem_${MASS}_cmb.txt
    combineCards.py cmb/${MASS}/htt_*_1_13TeV.txt > cmb_${MASS}_cmb.txt
    
    for CHANS in cmb llet llmt lltt llem; do
        combineTool.py -M T2W -i ${CHANS}_${MASS}_cmb.txt -m ${MASS} -o ${CHANS}_${MASS}_workspace.root
        combineTool.py -M Asymptotic -m ${MASS} -t -1 ${CHANS}_${MASS}_workspace.root -n .limit_${CHANS}
        #combineTool.py -M CollectLimits *.limit_${CHANS}.* -o ${CHANS}_limits.json
    done
done








##combineTool.py -M T2W -i {cmb,eeet,eemt,eeem,eett,emmt,mmmt,emmm,mmtt}/* -o workspace.root --parallel 16
#
#echo "Combined"
#
##combine -M ProfileLikelihood --significance cmb/125/workspace.root -t -1 --expectSignal=1
#
###### Calculating limits:
###combineTool.py -M Asymptotic -t -1 -d */*/workspace.root --rMin 0.25 --rMax 2.0 --there -n .limit --parallel 16
##
###### Collect the output:
#combineTool.py -M CollectLimits ll*/*.limit.* --use-dirs -o limits.json

####### Plotting the limit:
####plotLimits.py limits_{cmb,em,et,mt,tt}.json:exp0 --auto-style
####plotLimits.py limits_cmb.json --auto-style   
####python ../../plotLimits_SM2.py limits_{cmb,em,et,mt,tt}.json:exp0 --auto-style --cms-sub Preliminary  -o lim_compare
####python ../../plotLimits_SM2.py limits_cmb.json  --auto-style --cms-sub Preliminary --show exp    -o lim_expected_cmb
####python ../../plotLimits_SM2.py limits_tt.json  --auto-style --cms-sub Preliminary --show exp    -o lim_expected_tt
####python ../../plotLimits_SM2.py limits_mt.json  --auto-style --cms-sub Preliminary --show exp    -o lim_expected_mt
####python ../../plotLimits_SM2.py limits_et.json  --auto-style --cms-sub Preliminary --show exp    -o lim_expected_et
####python ../../plotLimits_SM2.py limits_em.json  --auto-style --cms-sub Preliminary --show exp    -o lim_expected_em
####
####
######### run MaxLikelihoodFit
#######combine -M MaxLikelihoodFit cmb/125/workspace.root -t -1  --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin 0.25 --rMax 2.0 --expectSignal=1
#####combine -M MaxLikelihoodFit tt/125/workspace.root -t -1  --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin 0.1 --rMax 5.0 --expectSignal=1
#####combine -M MaxLikelihoodFit cmb/125/workspace.root -t -1  --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad  --rMin 0.1 --rMax 5.0 --expectSignal=1
####combine -M MaxLikelihoodFit cmb/125/workspace.root --robustFit=1 --minimizerAlgoForMinos=Minuit2,Migrad
######### making the pulls
####python ../../../../HiggsAnalysis/CombinedLimit/test/diffNuisances.py  mlfit.root -A -a --stol 0.99 --stol 0.99 --vtol 99. --vtol2 99. -f text mlfit.root > mlfit.txt
#####XXX PostFitShapes -o postfit_shapes.root -m 125 -f mlfit.root:fit_b --postfit --sampling --print -d cmb/125/combined.txt.cmb
###
###
###
###
###
###
