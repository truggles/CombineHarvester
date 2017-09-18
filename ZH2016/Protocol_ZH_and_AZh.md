
# Checkout instructions

    scram project CMSSW CMSSW_7_4_7
    cd CMSSW_7_4_7/src
    cmsenv
    git clone -b 74x-root6 https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
    git clone -b ZH2016 https://github.com/truggles/CombineHarvester.git CombineHarvester
    scram b -j 8

# Getting data cards
We will get a GitLab repo in the future, but try these to start:
AZh analysis - https://github.com/truggles/2016-sync/blob/azhSync/UW/azh_dc_sync.txt#L7
Zh analysis - https://github.com/truggles/2016-sync/blob/azhSync/UW/zh_dc_sync.txt#L8

Make a directory to store the above files depending on which analysis you care about:

    mkdir -p shapes/UW_TR/

and copy the data card of choice into there.

# Standard Work Flow


# creating datacards
You can toggel between ZH and AZh analysis using: --azh="true" in the MorphingZH2016 command below

You will need to adjust the --postfix selection below. If you data card root file is called
"htt_zh.inputs-sm-13TeV_svFitMass_new.root" you will need --postfix="_svFitMass_new"

    newFolder=Blinded20170611
    MorphingZH2016 --output_folder=${newFolder} --postfix="_4LMass_new" --input_folder="UW_TR"

# make workspaces

    cd output/${newFolder}
    combineTool.py -M T2W -i {cmb,eeet,eemt,eeem,eett,emmt,mmmt,emmm,mmtt}/* -o workspace.root --parallel 16

# estimated significance

    combine -M ProfileLikelihood --significance cmb/125/workspace.root -t -1 --expectSignal=1

# estimated limits

    combineTool.py -M Asymptotic -t -1 -d */*/workspace.root --rMin 0.25 --rMax 2.0 --there -n .limit --parallel 16
    combineTool.py -M CollectLimits */*/*.limit.* --use-dirs -o limits.json

    python ../scripts/plotLimits_SM2.py zh_limits.json --auto-style --cms-sub Preliminary  -o lim_zh_compare
    python ../scripts/plotLimits_SM2.py {llet,llmt,lltt,llem,cmb}_zh_limits.json:exp0 --auto-style --cms-sub Preliminary  -o lim_zh_higgs_final_states
    python ../scripts/plotLimits_SM2.py azh_limits.json --auto-style --cms-sub Preliminary  -o lim_azh_compare --limit-on "95% CL limit on #sigma(ggA)xB(A#rightarrowZh#rightarrowLL#tau#tau) (fb)"


