

###### Standard Work Flow
##### creating datacards
code=tauM_eL_mT
newFolder=Blinded20170614_sm
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
    #combineCards.py eeet/125/htt_*_*_13TeV.txt emmt/125/htt_*_*_13TeV.txt > llet.txt
    #combineCards.py eemt/125/htt_*_*_13TeV.txt mmmt/125/htt_*_*_13TeV.txt > llmt.txt
    combineCards.py eeem/125/htt_*_*_13TeV.txt emmm/125/htt_*_*_13TeV.txt > llem.txt
    #combineCards.py eett/125/htt_*_*_13TeV.txt mmtt/125/htt_*_*_13TeV.txt > lltt.txt
    #### Text2Workspace
    #combineTool.py -M T2W -i llet.txt -m 125 -o workspace_llet.root --parallel 4
    #combineTool.py -M T2W -i llmt.txt -m 125 -o workspace_llmt.root --parallel 4
    combineTool.py -M T2W -i llem.txt -m 125 -o workspace_llem.root --parallel 4
    #combineTool.py -M T2W -i lltt.txt -m 125 -o workspace_lltt.root --parallel 4
    
    #### Calculating limits:
    #combineTool.py -M Asymptotic -t -1 -d workspace_llet.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.llet
    #combineTool.py -M Asymptotic -t -1 -d workspace_llmt.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.llmt
    combineTool.py -M Asymptotic -t -1 -d workspace_llem.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.llem
    #combineTool.py -M Asymptotic -t -1 -d workspace_lltt.root -m ${LT} --rMin 0.25 --rMax 2.0 -n .limit.lltt

    popd
done


#### Collect the output:
#combineTool.py -M CollectLimits output/${newFolder}/*.limit.llet* --use-dirs -o limits_${code}_llet.json
#combineTool.py -M CollectLimits output/${newFolder}/*.limit.llmt* --use-dirs -o limits_${code}_llmt.json
combineTool.py -M CollectLimits output/${newFolder}/*.limit.llem* --use-dirs -o limits_${code}_llem.json
#combineTool.py -M CollectLimits output/${newFolder}/*.limit.lltt* --use-dirs -o limits_${code}_lltt.json

mv limits* ./jsons



#### Plotting the limit:
#plotLimits.py limits_{cmb,em,et,mt,tt}.json:exp0 --auto-style
#plotLimits.py limits_cmb.json --auto-style   
#python ../../plotLimits_SM2.py limits_{cmb,em,et,mt,tt}.json:exp0 --auto-style --cms-sub Preliminary  -o lim_compare
#python ../../plotLimits_SM2.py limits_cmb.json  --auto-style --cms-sub Preliminary --show exp    -o lim_expected_cmb

#python ../HTTSM2016/plotLimits_SM2.py limits_${code}_llet_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_llet
#python ../HTTSM2016/plotLimits_SM2.py limits_${code}_llmt_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_llmt
#python ../HTTSM2016/plotLimits_SM2.py limits_${code}_llem_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_llem
#python ../HTTSM2016/plotLimits_SM2.py limits_${code}_lltt_${newFolder}.json  --auto-style --cms-sub Preliminary --show exp -o lim_expected_lltt

#plotLimits.py jsons/limits_tau{L,M,T}_eL_mL_lltt_Blinded20170614_sm.json:exp0 --auto-style --cms-sub Preliminary  -o lim_compare --x-title LT_Higgs


plotLimits.py {Loose,Medium,Tight}.json:exp0 --auto-style --cms-sub Preliminary  -o lim_compare_lltt --x-title LT_Higgs
plotLimits.py {Loose,Medium,Tight}Tau_{Loose,Tight}E.json:exp0 --auto-style --cms-sub Preliminary  -o lim_compare_llet --x-title LT_Higgs
mv limits_tauL_eL_mL_llmt_Blinded20170614_sm.json LooseTau_LooseM.json
mv limits_tauL_eT_mT_llmt_Blinded20170614_sm.json LooseTau_TightM.json
mv limits_tauM_eT_mT_llmt_Blinded20170614_sm.json MediumTau_TightM.json
mv limits_tauM_eL_mL_llmt_Blinded20170614_sm.json MediumTau_LooseM.json
mv limits_tauT_eL_mL_llmt_Blinded20170614_sm.json TightTau_LooseM.json
mv limits_tauT_eT_mT_llmt_Blinded20170614_sm.json TightTau_TightM.json
