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
    for channel in mt em tt ll;
    do
        for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
        do
            mkdir -p ${signal}/${channel}/${mass}
        done
    done
done

for signal in ggH bbH;
do
    for channel in em tt;
    do
        UW_mssm_dataCards ${channel} ${signal}
        pushd ${signal}/${channel}
        for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
        do
            pushd ${mass}
            ln -s $DIR/${signal}/mssm_${channel}.input.root $DIR/${signal}/${channel}/${mass}/mssm_${channel}.input.root
            combine -M Asymptotic -m ${mass} ${channel}_inclusive_mssm_${mass}.txt
            popd
        done
        popd
    done
    for channel in mt; #et;
    do
        UW_mssm_dataCards ${channel} ${signal}
        pushd ${signal}/${channel}
        for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
        do
            pushd ${mass}
            ln -s $DIR/${signal}/mssm_${channel}.input.root $DIR/${signal}/${channel}/${mass}/mssm_${channel}.input.root
            combine -M Asymptotic -m ${mass} ${channel}_inclusivemtnotwoprong_mssm_${mass}.txt
            popd
        done
        popd
    done
    pushd ${signal}
    for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
    do
        echo " ---- Doing LL combined ${mass} ---- "
        combineCards.py em/${mass}/em_inclusive_mssm_${mass}.txt tt/${mass}/tt_inclusive_mssm_${mass}.txt mt/${mass}/mt_inclusivemtnotwoprong_mssm_${mass}.txt > ll/${mass}/ll_inclusive_mssm_${mass}.txt
        combine -M Asymptotic -m ${mass} -n LL ll/${mass}/ll_inclusive_mssm_${mass}.txt
        echo " ---- Finishing LL combined ${mass} ---- "
    done
    popd
done


