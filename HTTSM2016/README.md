
# Checkout a fresh setup
```
export SCRAM_ARCH=slc6_amd64_gcc530
scram project CMSSW CMSSW_8_1_0
cd CMSSW_8_1_0/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit

# Update to a reccomended tag - currently the reccomended tag is v7.0.12
pushd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v7.0.12
popd

# Currently base off of truggles HTXS 81X branch
git clone -b SM2016-HTXS git@github.com:truggles/CombineHarvester.git

scram b -j 8

cd CombineHarvester/HTTSM2016
```    


# Get the shape files from GitLab
```
cd CombineHarvester/HTTSM2016
git clone https://:@gitlab.cern.ch:8443/cms-htt/SM-PAS-2016.git shapes  (from lxplus)
git clone https://gitlab.cern.ch/cms-htt/SM-PAS-2016.git shapes      (from elsewhere)
git pull --rebase 
```
