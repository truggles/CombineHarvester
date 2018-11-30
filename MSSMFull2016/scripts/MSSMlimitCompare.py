import CombineHarvester.CombineTools.plotting as plot 
import CombineHarvester.CombineTools.maketable as maketable
import ROOT
import math
import argparse
import json
import sys

ROOT.gROOT.SetBatch(ROOT.kTRUE)
parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help='named input file')
parser.add_argument('--labels', help='Labels for legend')
parser.add_argument('--parton_scalings', help='Option to apply parton lumi scaling to some of the curves. Options [ym,yd,n], m and d indicates multiply or divide')
parser.add_argument('--process', help='The process on which a limit has been calculated. [gg#phi, bb#phi]', default="gg#phi")
parser.add_argument('--custom_y_range', help='Fix y axis range', action='store_true', default=False)
parser.add_argument('--y_axis_min',  help='Fix y axis minimum', default=0.001)
parser.add_argument('--y_axis_max',  help='Fix y axis maximum', default=100.0)
parser.add_argument('--custom_x_range', help='Fix x axis range', action='store_true', default=False)
parser.add_argument('--x_axis_min',  help='Fix x axis minimum', default=90.0)
parser.add_argument('--x_axis_max',  help='Fix x axis maximum', default=1000.0)
parser.add_argument('--verbosity', '-v', help='verbosity', default=0)
parser.add_argument('--log', help='Set log range for x and y axis', action='store_true', default=False)
parser.add_argument('--expected_only', help='Plot expected limit difference only',action='store_true', default=False)
parser.add_argument('--outname','-o', help='Name of output plot', default='limit_comparison')
parser.add_argument('--relative',help='Relative difference in limit',action='store_true', default=False)
parser.add_argument('--absolute',help='Absolute difference in limit',action='store_true', default=False)
parser.add_argument('--channel_label',default='#mu#tau_{h} no b-tag',help='Channel - category label')
parser.add_argument('--title',help='Plot label', default='35.9 fb^{-1} (13 TeV)')
parser.add_argument(
    '--cms-sub', default='Internal', help="""Text below the CMS logo""")
#parser.add_argument('--table_vals', help='Amount of values to be written in a table for different masses', default=10)
args = parser.parse_args()

channel_label = args.channel_label

colourlist=[ROOT.kGreen+3,ROOT.kRed,ROOT.kBlue,ROOT.kBlack,ROOT.kYellow+2,ROOT.kOrange,ROOT.kCyan+3,ROOT.kMagenta+2,ROOT.kViolet-5,ROOT.kGray]
files = (args.file).split(',')
if args.labels:
  labels = (args.labels).split(',')
else:
  labels =[]
if args.parton_scalings:
  parton_scalings = (args.parton_scalings).split(',')
else:
  parton_scalings = []
outname = args.outname

if (args.relative or args.absolute) and len(files)!=2:
  print 'Provide exactly 2 input files to plot the relative difference in limit'

if args.relative and args.absolute:
  print 'Only specify either relative or absolute difference plots'

if len(colourlist) < len(files):
  print 'Too many input files! Maximum supported is %d'%len(colourlist)
  sys.exit(1)

if len(labels) < len(files) and not (args.relative or args.absolute) :
  print 'Provide at least as many legend labels as files'
  sys.exit(1)


exp_graph_list = [ROOT.TGraph() for i in range(len(files))]
#if args.relative or args.absolute:
obs_graph_list = [ROOT.TGraph() for i in range(len(files))]
for i in range(len(files)):
  if ".root" in files[i]:
    exp_graph_list[i] = plot.SortGraph((ROOT.TFile(files[i],'r')).Get('expected'))
 #   if args.relative or args.absolute:
    obs_graph_list[i] = plot.SortGraph((ROOT.TFile(files[i],'r')).Get('observed'))
  else:
    data = {}
    with open(files[i]) as jsonfile:
      data = json.load(jsonfile)
    exp_graph_list[i] = plot.LimitTGraphFromJSON(data, 'exp0')
   # if args.relative or args.absolute:
    obs_graph_list[i] = plot.LimitTGraphFromJSON(data, 'obs')

max_vals = []
for i in range(len(exp_graph_list)):
  max_vals.append(ROOT.TMath.MaxElement(exp_graph_list[i].GetN(),exp_graph_list[i].GetY()))
  if args.relative or args.absolute:
    relative_exp_graph = plot.GraphDifference(exp_graph_list[0],exp_graph_list[1],True if args.relative else False) 
    relative_obs_graph = plot.GraphDifference(obs_graph_list[0],obs_graph_list[1],True if args.relative else False)
    del max_vals[:]
    max_vals.append(ROOT.TMath.MaxElement(relative_exp_graph.GetN(),relative_exp_graph.GetY()))
    if not args.expected_only:
      max_vals.append(ROOT.TMath.MaxElement(relative_obs_graph.GetN(),relative_obs_graph.GetY()))

#Optional code to scale limits by parton lumi factors
parton_lumis = {90:2.149018,100:2.197008,110:2.241856,120:2.28606,130:2.33067,140:2.37202,160:2.45378,180:2.53216,200:2.60917,250:2.79534,300:2.97773,350:3.15818,400:3.33910,450:3.52189,500:3.70655,600:4.09093,700:4.49192,800:4.92356,900:5.38754,1000:5.887}
for j in range(len(parton_scalings)):
  yval, xval, yval_obs, xval_obs = ROOT.Double(0), ROOT.Double(0), ROOT.Double(0), ROOT.Double(0)
  for i in range((exp_graph_list[j].GetN())):
    exp_graph_list[j].GetPoint(i,xval,yval)
    obs_graph_list[j].GetPoint(i,xval_obs,yval_obs)
    if parton_scalings[j] == 'ym' : 
        exp_graph_list[j].SetPoint(i,xval,(yval)*parton_lumis[xval])
        obs_graph_list[j].SetPoint(i,xval_obs,(yval_obs)*parton_lumis[xval_obs])
    if parton_scalings[j] == 'yd' : 
        exp_graph_list[j].SetPoint(i,xval,(yval)/parton_lumis[xval])
        obs_graph_list[j].SetPoint(i,xval_obs,(yval_obs)/parton_lumis[xval_obs])

process_label=args.process

mass_list=[]
for i in range(exp_graph_list[0].GetN()) :
    mass_list.append(float(exp_graph_list[0].GetX()[i]))
mass_list = sorted(set(mass_list))
mass_bins=len(mass_list)
if int(args.verbosity) > 0 :
    print "mass_list: ", mass_list, "Total number: ", mass_bins 

# Setup the canvas: we'll use a two pad split, with a small top pad to contain
# the CMS logo and the legend
plot.ModTDRStyle(r=0.06, l=0.12)
axis = plot.makeHist1D('hist1d', mass_bins, exp_graph_list[0])
ROOT.gStyle.SetFrameLineWidth(2)
c1=ROOT.TCanvas()
pads = plot.TwoPadSplit(0.8, 0, 0)
pads[1].cd()
axis.GetYaxis().SetRangeUser(0,1.2*float(max(max_vals)))  
if args.log:
  axis.GetYaxis().SetRangeUser(0.001,1.2*float(max(max_vals)))
if process_label == "gg#phi" :
    if args.relative :
        axis.GetYaxis().SetTitle("Relative diff in limit on #sigma#font[42]{(gg#phi)}#upoint#font[52]{B}#font[42]{(#phi#rightarrow#tau#tau)} [pb]")
    elif args.absolute :
        axis.GetYaxis().SetTitle("Absolute diff in limit on #sigma#font[42]{(gg#phi)}#upoint#font[52]{B}#font[42]{(#phi#rightarrow#tau#tau)} [pb]")
    else :    
        axis.GetYaxis().SetTitle("95% CL limit on #sigma#font[42]{(gg#phi)}#upoint#font[52]{B}#font[42]{(#phi#rightarrow#tau#tau)} [pb]")
elif process_label == "bb#phi" :
    if args.relative :
        axis.GetYaxis().SetTitle("Relative diff in limit on #sigma#font[42]{(bb#phi)}#upoint#font[52]{B}#font[42]{(#phi#rightarrow#tau#tau)} [pb]")
    elif args.absolute :
        axis.GetYaxis().SetTitle("Absolute diff in limit on #sigma#font[42]{(bb#phi)}#upoint#font[52]{B}#font[42]{(#phi#rightarrow#tau#tau)} [pb]")
    else :    
        axis.GetYaxis().SetTitle("95% CL limit on #sigma#font[42]{(bb#phi)}#upoint#font[52]{B}#font[42]{(#phi#rightarrow#tau#tau)} [pb]")
else:
    exit("Currently process is not supported")
if args.custom_y_range : axis.GetYaxis().SetRangeUser(float(args.y_axis_min), float(args.y_axis_max))
axis.GetXaxis().SetTitle("m_{#phi} [GeV]")
if args.custom_x_range : axis.GetXaxis().SetRangeUser(float(args.x_axis_min), float(args.x_axis_max))
axis.GetYaxis().SetTitleSize(0.040)    
axis.GetYaxis().SetTitleOffset(1.2)    
pads[1].SetTickx()
pads[1].SetTicky()
if args.log :
    pads[1].SetLogx(1);
    pads[1].SetLogy(1);
    axis.SetNdivisions(50005, "X");
    axis.GetXaxis().SetMoreLogLabels();
    axis.GetXaxis().SetNoExponent();
    axis.GetXaxis().SetLabelSize(0.040);
axis.Draw()

if not (args.relative or args.absolute):
  for i in range(len(files)):
    exp_graph_list[i].SetLineColor(colourlist[i])
    exp_graph_list[i].SetLineWidth(3)
    exp_graph_list[i].SetMarkerStyle(20)
    exp_graph_list[i].SetMarkerColor(colourlist[i])
    exp_graph_list[i].SetLineStyle(2)
    exp_graph_list[i].Draw("PL")
    if not args.expected_only:
      obs_graph_list[i].SetLineColor(colourlist[i])
      obs_graph_list[i].SetLineWidth(3)
      obs_graph_list[i].SetMarkerStyle(20)
      obs_graph_list[i].SetMarkerColor(colourlist[i])
      obs_graph_list[i].SetLineStyle(1)
      obs_graph_list[i].Draw("PL")
else:
  relative_exp_graph.SetLineColor(ROOT.kRed)
  relative_exp_graph.SetLineWidth(3)
  relative_exp_graph.SetMarkerStyle(20)
  relative_exp_graph.SetMarkerColor(ROOT.kRed)
  relative_exp_graph.SetLineStyle(1)
  relative_exp_graph.Draw("PL")
  if not args.expected_only:
    relative_obs_graph.SetLineColor(ROOT.kBlue)
    relative_obs_graph.SetMarkerColor(ROOT.kBlue)
    relative_obs_graph.SetLineWidth(3)
    relative_obs_graph.SetLineStyle(2)
    relative_obs_graph.SetMarkerStyle(20)
    relative_obs_graph.Draw("PL")

pads[0].cd()
h_top = axis.Clone()
#necessary in case chosen range surrounds 0 which will cause axis to contain a horizontal line at 0
h_top.GetYaxis().SetRangeUser(0.001,100)
plot.Set(h_top.GetXaxis(), LabelSize=0, TitleSize=0, TickLength=0)
plot.Set(h_top.GetYaxis(), LabelSize=0, TitleSize=0, TickLength=0)
h_top.Draw()

legend = plot.PositionedLegend(0.5 if args.relative or args.absolute else 0.4, 0.11, 3, 0.015)
plot.Set(legend, NColumns=2, Header='#bf{%.0f%% CL Excluded:}' % 95)
if not (args.relative or args.absolute):
  for i in range(len(files)):
    legend.AddEntry(exp_graph_list[i],labels[i],"PL")
elif args.relative:
  legend.SetTextSize(0.025)
  legend.AddEntry(relative_exp_graph,"Exp 2*|"+labels[0]+"-"+labels[1]+"|/("+labels[0]+"+"+labels[1]+")","PL")
  if not args.expected_only:
    legend.AddEntry(relative_obs_graph,"Obs 2*|"+labels[0]+"-"+labels[1]+"|/("+labels[0]+"+"+labels[1]+")","PL")
elif args.absolute:
  legend.SetTextSize(0.025)
  legend.AddEntry(relative_exp_graph,"Exp 2*("+labels[0]+"-"+labels[1]+")/("+labels[0]+"+"+labels[1]+")","PL")
  if not args.expected_only:
    legend.AddEntry(relative_obs_graph,"Obs 2*("+labels[0]+"-"+labels[1]+")/("+labels[0]+"+"+labels[1]+")","PL")
legend.Draw()

latex2 = ROOT.TLatex()
latex2.SetNDC()
latex2.SetTextAngle(0)
latex2.SetTextColor(ROOT.kBlack)
latex2.SetTextSize(0.032)
latex2.DrawLatex(0.125,0.96,channel_label)

plot.DrawCMSLogo(pads[0], 'CMS', args.cms_sub, 11, 0.045, 0.15, 1.0, '', 1.0)
plot.DrawTitle(pads[0], '%s'%args.title, 3);
plot.FixOverlay()

# Redraw the frame because it usually gets covered by the filled areas
pads[1].cd()
pads[1].GetFrame().Draw()
pads[1].RedrawAxis()

c1.SaveAs("%s.pdf"%outname)
c1.SaveAs("%s.png"%outname)
    
