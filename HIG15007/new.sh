rm -r output/LIMITS/
mkdir -p output/GoF/
mkdir -p output/plots/
mkdir -p output/impacts/

#for MTHD in FFZX StandardZX; do
for MTHD in FF; do
    python scripts/tt${MTHD}_setupDatacards.py
    cd output/LIMITS/tt/
    text2workspace.py datacard${MTHD}.txt
    combine -M MaxLikelihoodFit datacard${MTHD}.root --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"  --rMin 0.35 --rMax 1.3 > fit_output${MTHD}.txt
    PostFitShapesFromWorkspace -o ztt_tt_shapes${MTHD}.root -f mlfit.root:fit_s --postfit --sampling --print -d datacard${MTHD}.txt -w datacard${MTHD}.root
#    cd ../../..

    echo "Plots"
    cd ../../plots
    python ../../Draw_postfit.py --mthd=${MTHD}
    cd ../..

    #echo "GoodnessOfFit"
    #cd output/GoF
    #
    #combine -M GoodnessOfFit ../LIMITS/tt/datacard${MTHD}.txt --algo=saturated
    #for i in 0 1 2 3 4; do
    #    combine -M GoodnessOfFit ../LIMITS/tt/datacard${MTHD}.txt --algo=saturated -t 100 -s ${i}
    #done
    #combineTool.py -M CollectGoodnessOfFit --input higgsCombineTest.GoodnessOfFit.mH120.root higgsCombineTest.GoodnessOfFit.mH120.*.root -o GoF${MTHD}.json
    #python ../../../CombineTools/scripts/plotGof.py --statistic saturated --mass 120.0 -o ztt_saturated${MTHD} GoF${MTHD}.json  --title-right="2.3 fb^{-1} (13 TeV)" --title-left="#tau_{h}#tau_{h}, ${MTHD} QCD Mthd, m_{#tau#tau}"
    #cd ../..
#
#    echo "Impacts"
#    cd output/impacts
#    combineTool.py -M Impacts -m 120 -d ../LIMITS/tt/datacard${MTHD}.root --doInitialFit --robustFit 1
#    combineTool.py -M Impacts -m 120 -d ../LIMITS/tt/datacard${MTHD}.root --robustFit 1 --doFits
#    combineTool.py -M Impacts -m 120 -d ../LIMITS/tt/datacard${MTHD}.root -o impacts${MTHD}.json
#    plotImpacts.py -i impacts${MTHD}.json -o impacts${MTHD}
#    cd ../..
done
