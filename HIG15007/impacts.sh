mkdir -p output/impacts
cd output/impacts
combineTool.py -M Impacts -m 120 -d ../LIMITS/tt/datacard.root --doInitialFit --robustFit 1
combineTool.py -M Impacts -m 120 -d ../LIMITS/tt/datacard.root --robustFit 1 --doFits
combineTool.py -M Impacts -m 120 -d ../LIMITS/tt/datacard.root -o impacts.json
plotImpacts.py -i impacts.json -o impacts
cd ../..
