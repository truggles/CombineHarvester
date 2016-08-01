# Script to make a convenient directory strucutre
# for the MSSM Phi->TauTau analysis
# masses are currently limited to completed
# ntuple root files.  More to come.

# Additionally, runs asymptotic limits for
# each channel by each mass


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

rm -r bbH/ ggH/

for signal in ggH bbH;
do
    #for channel in mt em tt ll;
    for channel in tt;
    #for channel in em tt ll;
    do
        #for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
        #for mass in 80 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900;
        for mass in 90 100 110 120 130 140 160 180 200 250 300 350 400 450 500 600 700 800 900 1000 1200 1400 1500 1600 1800 2000 2300 2600 2900 3200;
        do
            mkdir -p ${signal}/${channel}/${mass}
        done
    done
done

for signal in ggH bbH;
do
    #for channel in em tt;
    for channel in tt;
    do
        UW_mssm_dataCards ${channel} ${signal}
        pushd ${signal}/${channel}
        #for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
        #for mass in 80 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900;
        for mass in 90 100 110 120 130 140 160 180 200 250 300 350 400 450 500 600 700 800 900 1000 1200 1400 1500 1600 1800 2000 2300 2600 2900 3200;
        do
            pushd ${mass}
            ln -s $DIR/${signal}/mssm_${channel}.input.root $DIR/${signal}/${channel}/${mass}/mssm_${channel}.input.root
            #combine -M Asymptotic -t -1 -m ${mass} ${channel}_inclusive_mssm_${mass}.txt
            combine -M ProfileLikelihood --significance ${channel}_inclusive_mssm_${mass}.txt -t -1 --expectSignal=1 -m ${mass}
            #combine -M ProfileLikelihood --significance combine -M ProfileLikelihood --significance datacard.txt -t -1 --expectSignal=1 -t -1 --expectSignal=1
            popd
        done
        popd
    done
    #for channel in mt; #et;
    #do
    #    UW_mssm_dataCards ${channel} ${signal}
    #    pushd ${signal}/${channel}
    #    #for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
    #    for mass in 80 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900;
    #    do
    #        pushd ${mass}
    #        ln -s $DIR/${signal}/mssm_${channel}.input.root $DIR/${signal}/${channel}/${mass}/mssm_${channel}.input.root
    #        combine -M Asymptotic -m ${mass} ${channel}_inclusivemtnotwoprong_mssm_${mass}.txt
    #        popd
    #    done
    #    popd
    #done
    #pushd ${signal}
    ##for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
    #for mass in 80 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900;
    #do
    #    echo " ---- Doing LL combined ${mass} ---- "
    #    combineCards.py em/${mass}/em_inclusive_mssm_${mass}.txt tt/${mass}/tt_inclusive_mssm_${mass}.txt mt/${mass}/mt_inclusivemtnotwoprong_mssm_${mass}.txt > ll/${mass}/ll_inclusive_mssm_${mass}.txt
    #    combine -M Asymptotic -m ${mass} -n LL ll/${mass}/ll_inclusive_mssm_${mass}.txt
    #    echo " ---- Finishing LL combined ${mass} ---- "
    #done
    #popd
done


