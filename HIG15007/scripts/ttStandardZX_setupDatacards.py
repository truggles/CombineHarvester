#!/usr/bin/env python

import CombineHarvester.CombineTools.ch as ch
import os


# Need this later
def RenameSyst(cmb, syst, old, new):
    if old in syst.name():
        oldname = syst.name()
        syst.set_name(new)
        # Should change the ch::Parameter names too
        cmb.RenameParameter(oldname, new)


shapes_dir = os.environ['CMSSW_BASE'] + '/src/CombineHarvester/HIG15007/shapes'

cb = ch.CombineHarvester()

#####################################################################################
# Set the processes and categories
#####################################################################################
sig_procs = ['ZTT']

bkg_procs = {
  'tt': ['ZJ', 'ZL', 'W', 'QCD', 'TT', 'VV']
}

bins = {
  'tt': [#(0, 'tt_inclusive'),
        (0, 'tt_0jet'),
        #(1, 'tt_1jet'),
        #(2, 'tt_2jet'),
        (1, 'tt_1jet_low'),
        (2, 'tt_1jet_medium'),
        (3, 'tt_1jet_high'),
        (4, 'tt_vbf'),
        (5, 'tt_1bjet'),
        (6, 'tt_2bjet'),
        ]
}

channels = ['tt',]

#####################################################################################
# Set input shape files
#####################################################################################
files = {
    'tt': {
        'Wisconsin': 'htt_tt.inputs-sm-13TeV_svFitMass-FFv3old.root'
        #'Wisconsin': 'htt_tt.inputs-sm-13TeV_visMass-FFv3old.root'
    },
}

inputs = {
    'tt': 'Wisconsin',
}

#####################################################################################
# Create CH entries and load shapes
#####################################################################################
for chn in channels:
    ana = ['ztt']
    era = ['13TeV']
    cb.AddObservations(['*'], ana, era, [chn], bins[chn])
    cb.AddProcesses(['*'], ana, era, [chn], bkg_procs[chn], bins[chn], False)
    cb.AddProcesses(['*'], ana, era, [chn], sig_procs, bins[chn], True)

#####################################################################################
# Define systematic uncertainties
#####################################################################################
# define some useful shortcuts
real_m = ['ZTT', 'ZLL', 'ZL', 'ZJ', 'TT', 'VV']  # procs with a real muon
real_e = ['ZTT', 'ZLL', 'ZL', 'ZJ', 'TT', 'VV']  # procs with a real electron

constrain_eff_t = False

# Only create the eff_t lnN if we want this to be constrained,
# otherwise set a rateParam.
# Split tau ID efficiency uncertainty into part ("CMS_eff_t") that is correlated between channels
# and part ("CMS_eff_t_et", "CMS_eff_t_mt", "CMS_eff_t_tt") that is uncorrelated
#if constrain_eff_t:
#    cb.cp().AddSyst(
#        cb, 'CMS_eff_t', 'lnN', ch.SystMap('channel', 'process')
#            (['tt'],       ['ZTT', 'TT', 'VV'], 1.10))
#    cb.cp().AddSyst(
#        cb, 'CMS_eff_t_$CHANNEL', 'lnN', ch.SystMap('channel', 'process')
#            (['tt'],       ['ZTT', 'TT', 'VV'], 1.06))
#else:
#    cb.cp().AddSyst(
#        cb, 'CMS_eff_t', 'rateParam', ch.SystMap('channel', 'process')
#            (['tt'],       ['ZTT', 'TT', 'VV'], 1.0))
#    # We should set a sensible range for the resulting parameter
#    cb.GetParameter('CMS_eff_t').set_range(0.5, 1.5)


# Split tau energy scale uncertainty into part ("CMS_scale_t") that is correlated between channels
# and part ("CMS_scale_t_et", "CMS_scale_t_mt", "CMS_scale_t_tt") that is uncorrelated
cb.cp().AddSyst(
    cb, 'CMS_scale_t_$CHANNEL_$ERA', 'shape', ch.SystMap('channel', 'process')
        (['tt'], ['ZTT','ZL','ZJ'], 1.0))
#cb.cp().AddSyst(
#    cb, 'CMS_scale_t_$CHANNEL_$ERA', 'shape', ch.SystMap('channel', 'process')
#        (['et', 'mt'], ['ZTT'], 0.500))


## QCD Fake Factor shape uncertainties
#cb.cp().AddSyst(
#    cb, 'CMS_ztt_ff_qcd_stat_$CHANNEL_$ERA', 'shape', ch.SystMap('channel', 'process')
#        (['tt'], ['QCD'], 0.71))
#cb.cp().AddSyst( # $BIN includes tt_category, so removed $CHANNEL
#    cb, 'CMS_ztt_ff_qcd_stat_$BIN_$ERA', 'shape', ch.SystMap('channel', 'process')
#        (['tt'], ['QCD'], 0.71))
#cb.cp().AddSyst(
#    cb, 'CMS_ztt_ff_qcd_syst_$CHANNEL_$ERA', 'shape', ch.SystMap('channel', 'process')
#        (['tt'], ['QCD'], 0.71))
#cb.cp().AddSyst( # $BIN includes tt_category, so removed $CHANNEL
#    cb, 'CMS_ztt_ff_qcd_syst_$BIN_$ERA', 'shape', ch.SystMap('channel', 'process')
#        (['tt'], ['QCD'], 0.71))


# Normal QCD uncertainty
cb.cp().process(['QCD']).AddSyst(
    cb, 'CMS_$ANALYSIS_qcdSyst_$BIN_$ERA', 'lnN', ch.SystMap('channel', 'bin_id')
        #(['tt'], {0},  1.20) # tt_inclusive
        (['tt'], {0},  1.20) # tt_0jet
        #(['tt'], {2},  1.30) # tt_1jet
        #(['tt'], {1},  1.30) # tt_2jet
        (['tt'], {1},  1.30) # tt_1jet_low
        (['tt'], {2},  1.30) # tt_1jet_medium
        (['tt'], {3},  2.0) # tt_1jet_high
        (['tt'], {4},  2.0) # tt_vbf
        (['tt'], {5},  2.0) # tt_1bjet
        (['tt'], {6},  2.0) # tt_2bjet
        )

cb.cp().process(['TT']).AddSyst(
    cb, 'CMS_$ANALYSIS_ttjNorm_$ERA', 'lnN', ch.SystMap('channel')
        (['tt'],  1.10))

cb.cp().process(['VV']).AddSyst(
    cb, 'CMS_$ANALYSIS_vvNorm_$ERA', 'lnN', ch.SystMap('channel')
        (['tt'],  1.15))

#cb.cp().process(['ZTT']).AddSyst(
#    cb, 'CMS_$ANALYSIS_ZTTNorm_$ERA', 'lnN', ch.SystMap('channel')
#        (['tt'],  1.1))

cb.cp().process(['W']).AddSyst(
    cb, 'CMS_$ANALYSIS_wjNorm_$CHANNEL_$ERA', 'lnN', ch.SystMap('channel')
        (['tt'],  1.20))

cb.cp().process(['ZL']).AddSyst(
    cb, 'CMS_$ANALYSIS_ZLNorm_$CHANNEL_$ERA', 'lnN', ch.SystMap('channel')
        (['tt'],  1.30))

cb.cp().process(['ZJ']).AddSyst(
    cb, 'CMS_$ANALYSIS_ZJNorm_$CHANNEL_$ERA', 'lnN', ch.SystMap('channel')
        (['tt'],  1.30))

# KIT cards only have one zllFakeTau param, but need at least three:
#  - e->tau fake rate 
#  - mu->tau fake rate (CV: use 100% uncertainty for now, as no measurement of mu->tau fake-rate in Run 2 data available yet)
#  - jet->tau fake rate


cb.cp().AddSyst(
    cb, 'lumi_$ERA', 'lnN', ch.SystMap('channel', 'process')
        (['tt'], ['ZTT', 'ZLL', 'TT', 'VV'],      1.027))


#####################################################################################
# Load the shapes
#####################################################################################
for chn in channels:
    cb.cp().channel([chn]).ExtractShapes(
        '%s' % (files[chn][inputs[chn]]),
        #'%s/%s/%s' % (shapes_dir, inputs[chn], files[chn][inputs[chn]]),
        '$BIN/$PROCESS', '$BIN/$PROCESS_$SYSTEMATIC')

# Want to treat scale_t as one parameter for now
cb.ForEachSyst(lambda sys: RenameSyst(cb, sys, 'CMS_scale_t', 'CMS_scale_t'))

constrain_scale_t = False
if not constrain_scale_t:
    cb.cp().syst_name(['CMS_scale_t']).ForEachSyst(lambda sys: sys.set_type('shapeU'))


#####################################################################################
# Create bin-by-bin
#####################################################################################
bbb = ch.BinByBinFactory()
bbb.SetPattern('CMS_$ANALYSIS_$BIN_$ERA_$PROCESS_bin_$#')
bbb.SetAddThreshold(0.1)
bbb.SetMergeThreshold(0.5) # For now we merge, but to be checked
bbb.SetFixNorm(True)
bbb.MergeAndAdd(cb.cp().backgrounds(), cb)


#####################################################################################
# Set nuisance parameter groups
#####################################################################################
# Start by calling everything syst and allsyst
cb.SetGroup('allsyst', ['.*'])
cb.SetGroup('syst', ['.*'])

## Then set lumi, and remove from both of the above
#cb.SetGroup('lumi', ['lumi_.*'])
#cb.RemoveGroup('syst', ['lumi_.*'])
#cb.RemoveGroup('allsyst', ['lumi_.*'])
#
## Then tauid, and remove it only from syst
#cb.SetGroup('tauid', ['#CMS_eff_t'])
#cb.RemoveGroup('syst', #['CMS_eff_t'])

# Now we can split into:#
#    - stat + syst + tau#id + lumi   ..or..
#    - stat + allsyst + #lumi

cb.PrintAll()


######################################################################################
# Write the cards
######################################################################################
writer = ch.CardWriter('$TAG/datacardStandardZX.txt',
                       '$TAG/shapes.root')
writer.SetWildcardMasses([])  # We don't use the $MASS property here
writer.SetVerbosity(1)
x = writer.WriteCards('output/LIMITS/cmb', cb)  # All cards combined
print x
x['output/LIMITS/cmb/datacardStandardZX.txt'].PrintAll()
for chn in channels:  # plus a subdir per channel
    writer.WriteCards('output/LIMITS/%s'%chn, cb.cp().channel([chn]))
