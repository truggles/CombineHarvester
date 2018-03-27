
# Checkout instructions

    scram project CMSSW CMSSW_7_4_7
    cd CMSSW_7_4_7/src
    cmsenv
    git clone -b 74x-root6 https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
    git clone -b SM2016-VH https://github.com/truggles/CombineHarvester.git CombineHarvester
    scram b -j 8

# Getting data cards

    Current DCs here - /afs/cern.ch/work/t/truggles/public/VH_DCs/20180327/

Make a directory to store the above files depending on which analysis you care about:

    mkdir -p shapes/TEST/

and copy the data card of choice into there.

# Standard Work Flow


# creating datacards
You will need to adjust the --postfix selection below. If you data card root file is called
"htt_zh.inputs-sm-13TeV_svFitMass_new.root" you will need --postfix="_svFitMass_new"

    newFolder=Blinded20170611
    MorphingVH2016 --output_folder=${newFolder} --postfix="_new" --input_folder="TEST"

# Move to CMSSW 81X
Before proceeding below, you need to move the workspace to CMSSW 81X newer combine area, v7
so that you pick up the ability to use autoMCStats.  All text DCs need

    * autoMCStats 0

appened to the last line.

# make workspaces

    cd output/${newFolder}
    combineTool.py -M T2W -i {cmb,eeet,eemt,eeem,eett,emmt,mmmt,emmm,mmtt,emt,ett,mtt,mmt}/* -o workspace.root --parallel 16

# estimated significance

    combine -M ProfileLikelihood --significance cmb/125/workspace.root -t -1 --expectSignal=1

# estimated limits

    combineTool.py -M Asymptotic -t -1 -d */*/workspace.root --rMin 0.25 --rMax 2.0 --there -n .limit --parallel 16
    combineTool.py -M CollectLimits */*/*.limit.* --use-dirs -o limits.json

    python ../scripts/plotLimits_SM2.py zh_limits.json --auto-style --cms-sub Preliminary  -o lim_zh_compare
    python ../scripts/plotLimits_SM2.py {llet,llmt,lltt,llem,cmb}_zh_limits.json:exp0 --auto-style --cms-sub Preliminary  -o lim_zh_higgs_final_states
    python ../scripts/plotLimits_SM2.py azh_limits.json --auto-style --cms-sub Preliminary  -o lim_azh_compare --limit-on "95% CL limit on #sigma(ggA)xB(A#rightarrowZh#rightarrowLL#tau#tau) (fb)"


# estimate mu

    combine -M MaxLikelihoodFit -m 125 125_all_workspace.root -t -1 --expectSignal=1 --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP --X-rtd FITTER_BOUND --minimizerTolerance=0.01 --rMin=-20 --rMax=20

# estimate significance

    combine -M ProfileLikelihood --significance -m 125 125_all_workspace.root -t -1 --expectSignal=1



