

# Standard Work Flow

# creating datacards

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



