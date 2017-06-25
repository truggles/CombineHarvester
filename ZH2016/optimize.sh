

#htt_zh.inputs-sm-13TeV_svFitMass_LT20.root
LT=50
###### Standard Work Flow
##### creating datacards
newFolder=Blinded20170613
for LT in 20 30 40 50 60 70 80 90 100; do
    MorphingZH2016 --output_folder=${newFolder} --postfix="_svFitMass_LT${LT}" --input_folder="UW_TR"
    ## Building the workspaces:
    pushd output/${newFolder}
    
    # For single channel optimization
    #combineTool.py -M T2W -i {eeet,eemt,eeem,eett,emmt,mmmt,emmm,mmtt}/125/htt_*_*_13TeV.txt -o workspace.root --parallel 16
    #for CHANNEL in eeet eemt eeem eett emmt mmmt emmm mmtt; do
    #    combineTool.py -M Asymptotic -t -1 -d ${CHANNEL}/125/workspace.root -m ${LT} --rMin 0.25 --rMax 2.0 --there -n .limit.${CHANNEL}
    #done

    #### Combine Channels Properly
    combineCards.py eeet/125/htt_*_*_13TeV.txt emmt/125/htt_*_*_13TeV.txt > llet.txt
    combineCards.py eemt/125/htt_*_*_13TeV.txt mmmt/125/htt_*_*_13TeV.txt > llmt.txt
    combineCards.py eeem/125/htt_*_*_13TeV.txt emmm/125/htt_*_*_13TeV.txt > llem.txt
    combineCards.py eett/125/htt_*_*_13TeV.txt mmtt/125/htt_*_*_13TeV.txt > lltt.txt
    #### Text2Workspace
    combineTool.py -M T2W -i llet.txt -m 125 -o workspace_llet.root --parallel 4
    combineTool.py -M T2W -i llmt.txt -m 125 -o workspace_llmt.root --parallel 4
    combineTool.py -M T2W -i llem.txt -m 125 -o workspace_llem.root --parallel 4
    combineTool.py -M T2W -i lltt.txt -m 125 -o workspace_lltt.root --parallel 4
    
    #### Calculating limits:
    combineTool.py -M Asymptotic -t -1 -d workspace_llet.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.llet
    combineTool.py -M Asymptotic -t -1 -d workspace_llmt.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.llmt
    combineTool.py -M Asymptotic -t -1 -d workspace_llem.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.llem
    combineTool.py -M Asymptotic -t -1 -d workspace_lltt.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.lltt

    popd
done


#### Collect the output:
combineTool.py -M CollectLimits output/${newFolder}/*.limit.llet* --use-dirs -o limits_llet.json
combineTool.py -M CollectLimits output/${newFolder}/*.limit.llmt* --use-dirs -o limits_llmt.json
combineTool.py -M CollectLimits output/${newFolder}/*.limit.llem* --use-dirs -o limits_llem.json
combineTool.py -M CollectLimits output/${newFolder}/*.limit.lltt* --use-dirs -o limits_lltt.json



#### Plotting the limit:
#plotLimits.py limits_{cmb,em,et,mt,tt}.json:exp0 --auto-style
#plotLimits.py limits_cmb.json --auto-style   
#python ../../plotLimits_SM2.py limits_{cmb,em,et,mt,tt}.json:exp0 --auto-style --cms-sub Preliminary  -o lim_compare
#python ../../plotLimits_SM2.py limits_cmb.json  --auto-style --cms-sub Preliminary --show exp    -o lim_expected_cmb
python ../HTTSM2016/plotLimits_SM2.py limits_llet_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_llet
python ../HTTSM2016/plotLimits_SM2.py limits_llmt_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_llmt
python ../HTTSM2016/plotLimits_SM2.py limits_llem_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_llem
python ../HTTSM2016/plotLimits_SM2.py limits_lltt_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_lltt



