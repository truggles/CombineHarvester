
mkdir -p output/GoF/
cd output/GoF
#cd output/LIMITS/tt/


combine -M GoodnessOfFit ../LIMITS/tt/datacard.txt --algo=saturated
combine -M GoodnessOfFit ../LIMITS/tt/datacard.txt --algo=saturated -t 500 -s 1234
combineTool.py -M CollectGoodnessOfFit --input higgsCombineTest.GoodnessOfFit.mH120.root higgsCombineTest.GoodnessOfFit.mH120.1234.root -o GoF.json
python ../../../CombineTools/scripts/plotGof.py --statistic saturated --mass 120.0 -o ztt_saturated GoF.json  --title-right="2.3 fb^{-1} (13 TeV)" --title-left="#tau_{h}#tau_{h}, Standard QCD Mthd, m_{vis}"

cd ../..
