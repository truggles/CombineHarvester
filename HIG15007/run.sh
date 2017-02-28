rm -r output/LIMITS/
#python scripts/ttFF_setupDatacards.py
python scripts/ttOld_setupDatacards.py

cd output/LIMITS/tt/

text2workspace.py datacard.txt

combine -M MaxLikelihoodFit datacard.root --robustFit=1 --preFitValue=1. --X-rtd FITTER_NEW_CROSSING_ALGO --minimizerAlgoForMinos=Minuit2 --minimizerToleranceForMinos=0.1 --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerAlgo=Minuit2 --minimizerStrategy=0 --minimizerTolerance=0.1 --cminFallbackAlgo \"Minuit2,0:1.\"  --rMin 0.35 --rMax 1.3 > fit_output.txt

PostFitShapesFromWorkspace -o ztt_tt_shapes.root -f mlfit.root:fit_s --postfit --sampling --print -d datacard.txt -w datacard.root

cd ../../plots

python ../../Draw_postfit.py

cd ../..
