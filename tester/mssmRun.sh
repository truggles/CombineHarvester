# Script to make a convenient directory strucutre
# for the MSSM Phi->TauTau analysis
# masses are currently limited to completed
# ntuple root files.  More to come.

# Additionally, runs asymptotic limits for
# each channel by each mass


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR


for channel in em tt;
do
    for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
    do
        mkdir -p ${channel}/${mass}
    done
done

for channel in em tt;
do
    UW_mssm_dataCards ${channel}
    pushd ${channel}
    for mass in 80 90 100 110 120 130 140 160 180 600 900 1000 1200 1500 2900 3200;
    do
        pushd ${mass}
        ln -s $DIR/mssm_${channel}.input.root $DIR/${channel}/${mass}/mssm_${channel}.input.root
        combine -M Asymptotic -m ${mass} ${channel}_inclusive_mssm_${mass}.txt
        popd
    done
    popd
done


