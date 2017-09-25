
doAZH=false
doZH=true

#for TEST in sept18BaselineTightM sept18MedBJetVeto; do
for TEST in sept21_preBJetCut sept21_preLTCut sept21_bJetCut; do
    ### AZH ###
    if $doAZH; then
        newFolder=Blinded20170915_azh_${TEST}
        #MorphingZH2016 --output_folder=${newFolder} --azh="true" --postfix="_AMass_new" --input_folder="UW_TR"
        MorphingZH2016 --output_folder=${newFolder} --azh="true" --postfix="_AMass_new" --input_folder=${TEST}
        pushd output/${newFolder}
        
        mkdir -p ../../Results_${newFolder}

        
        combineCards.py {eeet,emmt}/300/htt_*_1_13TeV.txt > llet_cmb.txt
        combineCards.py {eemt,mmmt}/300/htt_*_1_13TeV.txt > llmt_cmb.txt
        combineCards.py {eett,mmtt}/300/htt_*_1_13TeV.txt > lltt_cmb.txt
        combineCards.py {eeem,emmm}/300/htt_*_1_13TeV.txt > llem_cmb.txt
        combineCards.py {eeet,eemt,eett,eeem}/300/htt_*_1_13TeV.txt > zee_cmb.txt
        combineCards.py {emmt,emmt,mmtt,emmm}/300/htt_*_1_13TeV.txt > zmm_cmb.txt
        for MASS in 220 240 260 280 300 320 340 350 400; do
            combineCards.py {eeet,eemt,eett,eeem,emmt,mmmt,mmtt,emmm}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_cmb.txt
            combineTool.py -M T2W -i ${MASS}_cmb.txt -m ${MASS} -o ${MASS}_workspace.root
            combineTool.py -M Asymptotic --rMin=-5 --rMax=5 -m ${MASS} -t -1 ${MASS}_workspace.root -n .limit_${MASS}
        done
        combineTool.py -M CollectLimits *.limit_[2,3,4]* -o all_azh_limits.json
        cp all_azh_limits.json ../../Results_${newFolder}

        
        for CHANS in llet llmt lltt llem zee zmm; do
            combineTool.py -M T2W -i ${CHANS}_cmb.txt -m 300 -o ${CHANS}_workspace.root
            combineTool.py -M Asymptotic -m 300 -t -1 ${CHANS}_workspace.root --rMin=-5 --rMax=5 -n .limit_${CHANS}
            combineTool.py -M CollectLimits *.limit_${CHANS}.* -o ${CHANS}_limits.json
            cp ${CHANS}_limits.json ../../Results_${newFolder}
        done
        popd
    fi
    
    
    ### ZH ###
    if $doZH; then
        newFolder=Blinded20170915_zh_${TEST}
        #MorphingZH2016 --output_folder=${newFolder} --postfix="_svFitMass_new" --input_folder="UW_TR"
        MorphingZH2016 --output_folder=${newFolder} --postfix="_svFitMass_new" --input_folder=${TEST}
        pushd output/${newFolder}
        
        mkdir -p ../../Results_${newFolder}
        
        for MASS in 110 120 125 130 140; do
            combineCards.py {eeet,eemt,eett,eeem,emmt,mmmt,mmtt,emmm}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_all_cmb.txt
            combineCards.py {eeet,emmt}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_llet_cmb.txt
            combineCards.py {eemt,mmmt}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_llmt_cmb.txt
            combineCards.py {eett,mmtt}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_lltt_cmb.txt
            combineCards.py {eeem,emmm}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_llem_cmb.txt
            combineCards.py {eeet,eemt,eett,eeem}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_zee_cmb.txt
            combineCards.py {emmt,mmmt,mmtt,emmm}/${MASS}/htt_*_1_13TeV.txt > ${MASS}_zmm_cmb.txt
        done

        
        for CHAN in llet llmt lltt llem zee zmm all; do
            for MASS in 110 120 125 130 140; do
                combineTool.py -M T2W -i ${MASS}_${CHAN}_cmb.txt -m ${MASS} -o ${MASS}_${CHAN}_workspace.root
                combineTool.py -M Asymptotic --rMin=-5 --rMax=5 -m ${MASS} -t -1 ${MASS}_${CHAN}_workspace.root -n .limit_${MASS}_${CHAN}
            done
            combineTool.py -M CollectLimits *.limit_*_${CHAN}.* -o ${CHAN}_zh_limits.json
            cp ${CHAN}_zh_limits.json ../../Results_${newFolder}
        done
        popd
    fi
    
    
    echo ""
    echo "All done, see results in:"
    echo ""
    echo "Results_${newFolder} and the associated AZH one too"
    echo ""
done






