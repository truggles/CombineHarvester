

# pushd to folder {1}
pushd ${1}
echo "pushd ${1}"

for CHAN in llet llmt lltt llem zee zmm all; do
    echo "Plotting limits for ${CHAN}"
    python ../scripts/plotLimits_SM2.py ${CHAN}_zh_limits.json --auto-style --cms-sub Preliminary  -o lim_zh_${CHAN} --show exp
done

echo "Plotting limits for all higgs final states combined"
python ../scripts/plotLimits_SM2.py {llet,llmt,lltt,llem,all}_zh_limits.json:exp0 --auto-style --cms-sub Preliminary  -o lim_zh_higgs_final_states
python ../scripts/plotLimits_SM2.py {zee,zmm,all}_zh_limits.json:exp0 --auto-style --cms-sub Preliminary  -o lim_zh_z_boson_final_states

popd
