# Documentation:
## Link to the CombineHarvester twiki:
[CombineHarvester twiki](http://cms-analysis.github.io/CombineHarvester/index.html)

## For checkout instructions see README file
Including checking out the code with a fresh CMSSW in 81X and fetching the GitLab repository with root files


# Running the code:
## creating datacards
There are multiple helpful flags to alter the processing conditions:
do_shape_systematics=false will disable the requirement for shape systematics (default = true)
do_all_masses=true will have combine search for all input masses: 110, 120, 125, 130, 140 (default = false)
do_fake_factor_method=true/false toggle between using the Fake Factor method and its associated uncertainties (default = false)
do_embedded=true/false select whether ZTT will be taken from MC or embedded process (default = false)
```    
MorphingSM2016 --output_folder="myDirectory" --postfix="-2D-HTXS" --control_region=1 --mm_fit=false --ttbar_fit=true
```


## Automatic Bin Statistics
Before creating the workspace you mush add auto bin statistics.  This replaces the pervious bin-by-bin method. Based on our directory structure:
```
cd output/myDirectory
for i in ./*/*/*.txt
do
    echo "* autoMCStats 0" >> $i
done
```


## Building the workspaces:
```
combineTool.py -M T2W -i {cmb,em,et,mt,tt}/* -o workspace.root --parallel 18
```


## Basic examples of above processing
Please see some of the scripts for running the different HTXS stages listed as:
```
HTXS_Nominal.sh
HTXS_Stage0.sh
HTXS_Stage1.sh
```

## HTXS basics
The Higgs Template Cross Section method creates multiple signal POIs depending on the topology of the ggH/VBF/WH/ZH event at generator level. You can find the list of the different resulting POIs in the above three files. For more full documentation see: https://twiki.cern.ch/twiki/bin/view/CMS/HiggsTemplateCrossSection


## run FitDiagnostics
```
combine -M FitDiagnostics cmb/125/workspace.root --robustFit=1 --rMin 0.5 --rMax 1.5 
``` 


## making the pulls
```
python ../../../../../HiggsAnalysis/CombinedLimit/test/diffNuisances.py  mlfit.root -A -a --stol 0.99 --stol 0.99 --vtol 99. --vtol2 99. -f text mlfit.root > mlfit.txt
```


## Computing the Impact

    [Blinded]combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 --doInitialFit --robustFit 1 -t -1 --rMin 0.5 --rMax 1.5 --expectSignal=1 --parallel 8
    [UnBlinded]combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 --doInitialFit --robustFit 1 --rMin 0.5 --rMax 1.5  --parallel 8
    ---------------------------
    [Blinded]combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 --robustFit 1 --doFits -t -1 --rMin 0.5 --rMax 1.5 --expectSignal=1 --parallel 8 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP
    [UnBlinded]combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 --robustFit 1 --doFits -t -1 --rMin 0.5 --rMax 1.5 --expectSignal=1 --parallel 8 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP

    *Note*: Instead of the above line one can run the jobs via lxbatch:
    combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 --robustFit 1 --doFits -t -1 --rMin 0.5 --rMax 1.5 --expectSignal=1 --job-mode lxbatch --task-name lxbatch-test --sub-opts='-q 8nh' --merge 10 --dry-run   (--dry-run option let you check how the jobs will look like before submiting to lxbatch)  
    combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 --robustFit 1 --doFits --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --rMin 0.5 --rMax 1.5  --job-mode lxbatch --task-name lxbatch-test --sub-opts='-q 8nh' --merge 5 
    ---------------------------
    # Impact for limited number of nuisance parameters 

   combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 --robustFit 1 --doFits  --rMin 0.5 --rMax 1.5  --parallel 18 --named CMS_scale_t_et_13TeV,CMS_scale_t_mt_13TeV --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP

    ---------------------------

    combineTool.py -M Impacts -d cmb/125/workspace.root -m 125 -o impacts.json
    plotImpacts.py -i impacts.json -o impacts


## Computing the expected significance:

    [PreFit Asimov]combine -M ProfileLikelihood --significance cmb/125/workspace.root -t -1 --expectSignal=1
    [PostFit Asimov]combine -M ProfileLikelihood --significance cmb/125/workspace.root -t -1 --toysFrequentist --expectSignal 1
    [PostFit]combine -M ProfileLikelihood --significance cmb/125/workspace.root 

## Computing the expected significance per channel per category (including constraints from all channels)

    cd scripts
    sh channelCompatibilityCheck.sh


## postfit plots

    PostFitShapes -o postfit_shapes.root -m 125 -f mlfit.root:fit_s --postfit --sampling --print -d cmb/125/combined.txt.cmb
    



## Making the pvalue plot:

    cp ../../../scripts/runPvalue.sh .
    sh  runPvalue.sh .

## Making the S/S+B weighted plot

    cp ../../../scripts/runSBWeighted.sh .
    sh runSBWeighted.sh






