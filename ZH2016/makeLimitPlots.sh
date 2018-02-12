

pushd ${1}
echo "pushd ${1}"

for CHAN in llet llmt lltt llem all; do
    echo "Plotting limits for ${CHAN}"
    python ../../scripts/plotLimits_SM2.py limits_${CHAN}.json --auto-style --cms-sub Preliminary  -o lim_zh_${CHAN} --show exp
done

echo "Plotting limits for all higgs final states combined"
python ../../scripts/plotLimits_SM2.py limits_{llet,llmt,lltt,llem,all}.json:exp0 --auto-style --cms-sub Preliminary  -o lim_zh_higgs_final_states

popd
