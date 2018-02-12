# htt_zh.inputs-mssm-13TeV_AMass_new.root
# htt_zh.inputs-sm-13TeV_svFitMass_new.root

#### Standard Work Flow
### creating datacards
#newFolder=Blinded20180208_v1_AllNewestFullNewFR2
newFolder=Blinded20180212_v1_allNewestAllShapesFinal
MorphingZH2016 --output_folder=${newFolder} --postfix="_svFitMass_new" --input_folder="UW_TR"
## Building the workspaces:
cd output/${newFolder}

combineTool.py -M T2W -i {cmb,eeet,eemt,eeem,eett,emmt,mmmt,emmm,mmtt}/* -o workspace.root --parallel 16
#combineTool.py -M T2W -i cmb/125/htt_*.txt -o workspace.root --parallel 8


echo "Combined"

### Combine Channels Properly
for MASS in 110 120 125 130 140; do
    combineCards.py eeet/${MASS}/htt_*_*_13TeV.txt emmt/${MASS}/htt_*_*_13TeV.txt > llet_${MASS}.txt
    combineCards.py eemt/${MASS}/htt_*_*_13TeV.txt mmmt/${MASS}/htt_*_*_13TeV.txt > llmt_${MASS}.txt
    combineCards.py eeem/${MASS}/htt_*_*_13TeV.txt emmm/${MASS}/htt_*_*_13TeV.txt > llem_${MASS}.txt
    combineCards.py eett/${MASS}/htt_*_*_13TeV.txt mmtt/${MASS}/htt_*_*_13TeV.txt > lltt_${MASS}.txt

    ### Text2Workspace
    combineTool.py -M T2W -i llet_${MASS}.txt -m ${MASS} -o workspace_llet_${MASS}.root --parallel 4
    combineTool.py -M T2W -i llmt_${MASS}.txt -m ${MASS} -o workspace_llmt_${MASS}.root --parallel 4
    combineTool.py -M T2W -i llem_${MASS}.txt -m ${MASS} -o workspace_llem_${MASS}.root --parallel 4
    combineTool.py -M T2W -i lltt_${MASS}.txt -m ${MASS} -o workspace_lltt_${MASS}.root --parallel 4
    
    ### Calculating limits:
    combineTool.py -M Asymptotic -t -1 -d workspace_llet_${MASS}.root -m ${MASS} --rMin 0.25 --rMax 2.0 -n .limit.llet
    combineTool.py -M Asymptotic -t -1 -d workspace_llmt_${MASS}.root -m ${MASS} --rMin 0.25 --rMax 2.0 -n .limit.llmt
    combineTool.py -M Asymptotic -t -1 -d workspace_llem_${MASS}.root -m ${MASS} --rMin 0.25 --rMax 2.0 -n .limit.llem
    combineTool.py -M Asymptotic -t -1 -d workspace_lltt_${MASS}.root -m ${MASS} --rMin 0.25 --rMax 2.0 -n .limit.lltt
    combineTool.py -M Asymptotic -t -1 -d cmb/${MASS}/workspace.root -m ${MASS} --rMin 0.25 --rMax 2.0 --there -n .limit.cmb
done

combineTool.py -M CollectLimits higgsCombine.limit.llet.Asymptotic.mH*.root -o limits_llet.json
combineTool.py -M CollectLimits higgsCombine.limit.llmt.Asymptotic.mH*.root -o limits_llmt.json
combineTool.py -M CollectLimits higgsCombine.limit.lltt.Asymptotic.mH*.root -o limits_lltt.json
combineTool.py -M CollectLimits higgsCombine.limit.llem.Asymptotic.mH*.root -o limits_llem.json
combineTool.py -M CollectLimits cmb/*/higgsCombine.limit.cmb.Asymptotic.mH*.root -o limits_all.json








