dir=final2
#dir=final4SM

### Limits comparing 2.3/fb to 40/fb
python scripts/plotMSSMLimits.py --logy --logx ${dir}_ggH_cmb.json final3_ggH_cmb.json --cms-sub="Simulation" --title-right="2.3 fb^{-1} v. 40 fb^{-1} (13 TeV)" -o ${dir}_40fb_tt_ggH
python scripts/plotMSSMLimits.py --logy --logx ${dir}_bbH_cmb.json final3_bbH_cmb.json --process="bb#phi" --cms-sub="Simulation" --title-right="2.3 fb^{-1} v. 40 fb^{-1} (13 TeV)" -o ${dir}_40fb_tt_bbH


##### from input/mssm_protocol.txt - July 31, 2016
##### 1a)
#MorphingMSSMRun2 --output_folder="${dir}" -m MH --input_folder_tt="Wisconsin" --auto_rebin=1 --real_data=1 --postfix='_2p3fb'
###MorphingMSSMRun2 --output_folder="${dir}" -m MH --input_folder_tt="Wisconsin" --auto_rebin=1 --postfix="_40fb"
#
#### 2a) (base off of a previous version of their directions)
#combineTool.py -M T2W -o "ggPhi.root" -P CombineHarvester.CombinePdfs.ModelIndependent:floatingMSSMXSHiggs --PO 'modes=ggH' --PO 'ggHRange=0:20' -i output/${dir}/htt_cmb_8_13TeV/
#combineTool.py -M T2W -o "ggPhi.root" -P CombineHarvester.CombinePdfs.ModelIndependent:floatingMSSMXSHiggs --PO 'modes=ggH' --PO 'ggHRange=0:20' -i output/${dir}/htt_cmb_9_13TeV/
#combineTool.py -M T2W -o "ggPhi.root" -P CombineHarvester.CombinePdfs.ModelIndependent:floatingMSSMXSHiggs --PO 'modes=ggH' --PO 'ggHRange=0:20' -i output/${dir}/cmb/
#combineTool.py -M T2W -o "bbPhi.root" -P CombineHarvester.CombinePdfs.ModelIndependent:floatingMSSMXSHiggs --PO 'modes=bbH' --PO 'bbHRange=0:20' -i output/${dir}/htt_cmb_8_13TeV/
#combineTool.py -M T2W -o "bbPhi.root" -P CombineHarvester.CombinePdfs.ModelIndependent:floatingMSSMXSHiggs --PO 'modes=bbH' --PO 'bbHRange=0:20' -i output/${dir}/htt_cmb_9_13TeV/
#combineTool.py -M T2W -o "bbPhi.root" -P CombineHarvester.CombinePdfs.ModelIndependent:floatingMSSMXSHiggs --PO 'modes=bbH' --PO 'bbHRange=0:20' -i output/${dir}/cmb/
#
#
#
#### 3a)
#combineTool.py -m "90,100,110,120,130,140,160,180,200,250,350,400,450,500,600,700,800,900,1000,1200,1400,1600,1800,2000,2300,2600,2900,3200" -M Asymptotic --boundlist input/mssm_boundaries.json  --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_ggH -d output/${dir}/*/ggPhi.root --there -n ".ggH"
#combineTool.py -m "90,100,110,120,130,140,160,180,200,250,350,400,450,500,600,700,800,900,1000,1200,1400,1600,1800,2000,2300,2600,2900,3200" -M Asymptotic --boundlist input/mssm_boundaries.json  --setPhysicsModelParameters r_ggH=0,r_bbH=0 --redefineSignalPOIs r_bbH -d output/${dir}/*/bbPhi.root --there -n ".bbH"
# 
#combineTool.py -M CollectLimits output/${dir}/*/higgsCombine.ggH*.root --use-dirs -o "${dir}_ggH.json"
#combineTool.py -M CollectLimits output/${dir}/*/higgsCombine.bbH*.root --use-dirs -o "${dir}_bbH.json"
#
#
# Make Brazilian limits plots
## 4a)
#python scripts/plotMSSMLimits.py --logy --logx ${dir}_ggH_cmb.json --cms-sub="Simulation" --title-right="40 fb^{-1} (13 TeV)" -o ${dir}_tt_ggH
#python scripts/plotMSSMLimits.py --logy --logx ${dir}_bbH_cmb.json --process="bb#phi" --cms-sub="Simulation" --title-right="40 fb^{-1} (13 TeV)" -o ${dir}_tt_bbH
##python scripts/plotMSSMLimits.py --show='exp' --logy --logx ${dir}_ggH_cmb.json --cms-sub="Simulation" --title-right="40 fb^{-1} (13 TeV)" -o ${dir}_tt_ggH
##python scripts/plotMSSMLimits.py --show='exp' --logy --logx ${dir}_bbH_cmb.json --process="bb#phi" --cms-sub="Simulation" --title-right="40 fb^{-1} (13 TeV)" -o ${dir}_tt_bbH
# 
#
#
#
#### 5) (not sure if 5 is necessary)
####PostFitShapesFromWorkspace -d output/${dir}/htt_cmb_8_13TeV/htt_tt_8_13TeV.txt -w output/${dir}/htt_cmb_8_13TeV/ggPhi.root -o htt_tt_8_13TeV_shapes_20160731.root --print --freeze r_ggH=0.1,r_bbH=0.1,MH=700
#
#
#### 6)
#combineTool.py -M MaxLikelihoodFit --setPhysicsModelParameters MH=700,r_ggH=0.1,r_bbH=0.1 -d output/${dir}/cmb/ggPhi.root --redefineSignalPOIs r_ggH --there -m 700 --boundlist input/mssm_boundaries.json --minimizerTolerance 0.01 --minimizerStrategy 1
#
#PostFitShapesFromWorkspace -d output/${dir}/cmb/combined.txt.cmb -w output/${dir}/cmb/ggPhi.root -o shapes.root --print --freeze r_ggH=0.1,r_bbH=0.1,MH=700 --postfit --sampling -f output/${dir}/cmb/mlfit.Test.root:fit_s
#
#python scripts/makeMassPlots.py









