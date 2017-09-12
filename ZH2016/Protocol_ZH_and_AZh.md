

# Standard Work Flow

# First grab some data cards, we will get a GitLab repo in the future, but try these to start:
AZh analysis - https://github.com/truggles/2016-sync/blob/azhSync/UW/azh_dc_sync.txt#L7
Zh analysis - https://github.com/truggles/2016-sync/blob/azhSync/UW/zh_dc_sync.txt#L8

Make a directory to store these:

mkdir -p shapes/UW_TR/

and copy the data card of choice into there.

# creating datacards
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



