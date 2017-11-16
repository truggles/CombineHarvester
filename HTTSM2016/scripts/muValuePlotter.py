# This macro makes the muValue v.s. different final state or v.s. different categories.
# The input to this macro is mu_XXX.json which XXX could be either channels or categories
#  python muValuePlotter.py  -o Mu_Category -t category
#  python muValuePlotter.py  -o Mu_Channel -t channel

#Before that one need to run the maxlikelihoodfit for each category and then collect the output such as the following:
#
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_0jet.root  -o Mu_0jet.json
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_boosted.root   -o Mu_boosted.json
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_vbf.root    -o Mu_vbf.json
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_em.root   -o Mu_em.json
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_tt.root    -o Mu_tt.json
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_et.root    -o Mu_et.json
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_mt.root     -o Mu_mt.json
#combineTool.py -M CollectLimits  higgsCombineTest.MaxLikelihoodFit.mH120_cmb.root   -o Mu_cmb.json



#combineTool.py -M CollectLimits  higgsCombine_Mur_0jet.MaxLikelihoodFit.mH120.root  -o Mu_0jet.json
#combineTool.py -M CollectLimits  higgsCombine_Mur_boosted.MaxLikelihoodFit.mH120.root   -o Mu_boosted.json
#combineTool.py -M CollectLimits  higgsCombine_Mur_vbf.MaxLikelihoodFit.mH120.root   -o Mu_vbf.json
#
#combineTool.py -M CollectLimits  higgsCombine_Mur_em.MaxLikelihoodFit.mH120.root  -o Mu_em.json
#combineTool.py -M CollectLimits  higgsCombine_Mur_et.MaxLikelihoodFit.mH120.root   -o Mu_et.json
#combineTool.py -M CollectLimits  higgsCombine_Mur_mt.MaxLikelihoodFit.mH120.root    -o Mu_mt.json
#combineTool.py -M CollectLimits  higgsCombine_Mur_tt.MaxLikelihoodFit.mH120.root   -o Mu_tt.json




import plotting as plot
import ROOT
import argparse
import json

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
plot.ModTDRStyle()



parser = argparse.ArgumentParser()
# parser.add_argument('--input', '-i', help='Output of PostFitShapes or PostFitShapesFromWorkspace, specified as FILE:BIN')
parser.add_argument('--output', '-o', default=None, help='Output name')
parser.add_argument('--typet', '-t', default='cat', help='category or channel')
parser.add_argument('--blind', '-b', dest='blind', action='store_true', help='Find Asimove jsons')
# parser.add_argument('--channel', '-c', default='mt', choices=['mt', 'et', 'em', 'tt', 'mm'], help='Channel')
# parser.add_argument('--x-title', default='m_{ll}', help='x-axis variable, without GeV')
# parser.add_argument('--logy', action='store_true')
# parser.add_argument('--y-min', type=float, default=1)

args = parser.parse_args()

Type=args.typet
Blind=args.blind
output=args.output
print "Type:",Type
print "Blind:",Blind
print "Output Name:",output

# Canvas and pads
canv = ROOT.TCanvas(args.output, args.output)
pads = plot.OnePad()
pads[0].SetTicks(1, -1)

# For hadronic channle solo
#if Type == 'signal_nom' :
#    if Blind :
#        pad = {'xmin' : -15, 'xmax' : 20, 'ymin' : 0, 'ymax' : 10}
#    else :
#        pad = {'xmin' : -30, 'xmax' : 25, 'ymin' : 0, 'ymax' : 10}
#elif Type == 'signal_s0' :
#    pad = {'xmin' : -50, 'xmax' : 50, 'ymin' : 0, 'ymax' : 10}

# For all 4 channels combine
if Type == 'signal_nom' :
    if Blind :
        pad = {'xmin' : -22, 'xmax' : 20, 'ymin' : 0, 'ymax' : 10}
    else :
        pad = {'xmin' : -22, 'xmax' : 20, 'ymin' : 0, 'ymax' : 10}
elif Type == 'signal_s0' :
    pad = {'xmin' : -30, 'xmax' : 30, 'ymin' : 0, 'ymax' : 10}
elif Type == 'signal_modS1' :
    pad = {'xmin' : -20, 'xmax' : 50, 'ymin' : 0, 'ymax' : 20}
else :
    pad = {'xmin' : -.5, 'xmax' : 3, 'ymin' : 0, 'ymax' : 10}
print pad

axis = ROOT.TH2F('axis', '', 1, pad['xmin'], pad['xmax'], pad['ymax'], pad['ymin'], pad['ymax'])

plot.Set(axis.GetYaxis(), LabelSize=0)
plot.Set(axis.GetXaxis(), Title='Best fit #mu = #sigma/#sigma_{SM}')
axis.Draw()



#y_pos = 8.5
y_pos = pad['ymax'] - 1.5

latex = ROOT.TLatex()
latexNum = ROOT.TLatex()
if Type == 'signal_modS1' :
    plot.Set(latex, TextAlign=12, TextSize=0.025)
else :
    plot.Set(latex, TextAlign=12, TextSize=0.035)
plot.Set(latexNum, TextAlign=12, TextSize=0.025)


Channel_Category_Name={
    'signal' : {
        'ggH': 'ggH',
        'qqH': 'qqH',
        'WH': 'WH',
        'ZH': 'ZH',
        'cmb': 'combined'
    },
    'category' : {
        '0jet': '0jet',
        'boosted': 'boosted',
        'vbf': 'vbf',
        'cmb': 'combined'
    },
    'channel' : {
        'et': 'e#tau_{h}',
        'mt': '#mu#tau_{h}',
        'em': 'e#mu',
        'tt': '#tau_{h}#tau_{h}',
        'cmb': 'combined'
    },
    'signal_nom' : {
        'r_ggH': 'ggH',
        'r_qqH': 'qqH',
        'r_WH': 'WH',
        'r_ZH': 'ZH',
        'r_cmb': 'cmb'
    },
    'signal_s0' : {
        'r_ggH': 'ggH',
        'r_qqH': 'qqH',
        'r_WH_lep': 'WH lep',
        'r_ZH_lep': 'ZH lep',
        'r_VH_had': 'VH had',
        'r_cmb': 'cmb'
    },
    'signal_s0_frozen' : {
        'wFrozen_r_ggH': 'ggH',
        'wFrozen_r_qqH': 'qqH',
        'wFrozen_r_ZH_lep': 'ZH lep',
        'wFrozen_r_VH_had': 'VH had',
        'r_cmb': 'cmb'
    },
    'signal_modS1' : {
        'r_ggH_1J_PTH_120_200': 'ggH_1J_PTH_120_200',
        'r_ggH_GE2J_PTH_120_200': 'ggH_GE2J_PTH_120_200',
        'r_ggH_VBFTOPO_JET3': 'ggH_VBFTOPO_JET3',
        'r_ggH_1J_PTH_GT200': 'ggH_1J_PTH_GT200',
        'r_ggH_1J_PTH_0_60': 'ggH_1J_PTH_0_60',
        'r_ggH_GE2J_PTH_GT200': 'ggH_GE2J_PTH_GT200',
        'r_ggH_GE2J_PTH_60_120': 'ggH_GE2J_PTH_60_120',
        'r_ggH_VBFTOPO_JET3VETO': 'ggH_VBFTOPO_JET3VETO',
        'r_ggH_GE2J_PTH_0_60': 'ggH_GE2J_PTH_0_60',
        'r_ggH_1J_PTH_60_120': 'ggH_1J_PTH_60_120',
        'r_ggH_0J': 'ggH_0J',
        'r_qqH': 'qqH',
        'r_WH_lep': 'WH lep',
        'r_ZH_lep': 'ZH lep',
        'r_VH_had': 'VH had',
        'r_cmb': 'cmb'
    },
}

i = 0
y_pos -= 1.0

chn = '120.0'

x_text = 1.85
num_text = x_text
y_adj = 0.0

if Type=='channel': proc = ['em','et', 'mt', 'tt','cmb']
elif Type=='category': proc = ['0jet','boosted', 'vbf','cmb']
elif Type=='signal': proc = ['ggH','qqH','WH','ZH','cmb']
elif Type=='signal_nom':
    proc = ['r_ggH','r_qqH','r_WH','r_ZH','r_cmb']
    x_text = 5
    num_text = 13
    spacingPerEntry = (pad['ymax'] - pad['ymin']) * 0.75 / len(proc)
    y_adj = spacingPerEntry*0.3
elif Type=='signal_s0':
    proc = ['r_ggH','r_qqH','r_VH_had','r_ZH_lep','r_WH_lep','r_cmb']
    x_text = 5
    num_text = 20
    spacingPerEntry = (pad['ymax'] - pad['ymin']) * 0.75 / len(proc)
    y_adj = spacingPerEntry*0.3
elif Type=='signal_s0_frozen':
    proc = ['wFrozen_r_ggH','wFrozen_r_qqH','wFrozen_r_ZH_lep','wFrozen_r_VH_had','r_cmb']
    x_text = 5
    num_text = 18
elif Type=='signal_s1': proc = ['ggH','qqH','WH','ZH','cmb']
elif Type=='signal_modS1': 
    proc = [
        'r_ggH_1J_PTH_120_200',
        'r_ggH_GE2J_PTH_120_200',
        'r_ggH_VBFTOPO_JET3',
        'r_ggH_1J_PTH_GT200',
        'r_ggH_1J_PTH_0_60',
        'r_ggH_GE2J_PTH_GT200',
        'r_ggH_GE2J_PTH_60_120',
        'r_ggH_VBFTOPO_JET3VETO',
        'r_ggH_GE2J_PTH_0_60',
        'r_ggH_1J_PTH_60_120',
        'r_ggH_0J',
        'r_qqH',
        'r_WH_lep',
        'r_ZH_lep',
        'r_VH_had',
        'r_cmb']
    spacingPerEntry = (pad['ymax'] - pad['ymin']) * 1 / len(proc)
    y_adj = spacingPerEntry*0.3
    x_text = 13
    num_text = 40
else:
    print 'either select category or channel'


gr = ROOT.TGraphAsymmErrors(len(proc))
plot.Set(gr, LineWidth=2, LineColor=ROOT.kBlue)

# 25% buffer
spacingPerEntry = (pad['ymax'] - pad['ymin']) * 0.75 / len(proc)

for pro in proc:
    append = '' if not Blind else '_blind'
    with open('Mu_%s%s.json' % ( pro,append) ) as jsonfile:
        js = json.load(jsonfile)

        mean=round(js[chn]['obs'], 2)
        up= round(js[chn]['exp+1']-mean,2)
        down=round(mean-js[chn]['exp-1'],2)
        
        gr.SetPoint(i, mean, y_pos)
        gr.SetPointError(i,down ,up, 0, 0)
        
        latex.DrawLatex(x_text, y_pos+y_adj, Channel_Category_Name[Type][pro])
        latexNum.DrawLatex(num_text, y_pos+y_adj, str(mean)+'#splitline{ +%s}{ -%s}'%(str(up),str(down)))
        if 'cmb' in pro :  combineMu= mean; lowBnad=down; highBand=up;
        i += 1
        y_pos -= spacingPerEntry

gr.Draw('SAMEP')


theory_band = ROOT.TBox(combineMu-lowBnad,pad['ymin'],combineMu+highBand,pad['ymax'])
theory_band.SetFillColor(ROOT.kYellow)
theory_band.Draw('same')

l=ROOT.TLine(combineMu,pad['ymin'],combineMu,pad['ymax'])
l.Draw('sameL')

gr.Draw('SAMEP')


legend = ROOT.TLegend(0.18, 0.13, 0.45, 0.22, '', 'NBNDC')
legend.Draw()


# Go back and tidy up the axes and frame
pads[0].cd()
pads[0].GetFrame().Draw()
pads[0].RedrawAxis()

# CMS logo
plot.DrawCMSLogo(pads[0], 'CMS', 'Preliminary', 11, 0.045, 0.05, 1.0, '', 1.0)
plot.DrawTitle(pads[0], '35.9 fb^{-1} (13 TeV)', 3)


# ... and we're done
base = '/afs/cern.ch/user/t/truggles/www/HTXS/Nov06/'+output
canv.Print(base+'.png')
canv.Print(base+'.pdf')
canv.Print(base+'.C')


