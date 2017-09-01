import ROOT
import math

def pYields ( fName, bkgs ) :
    f = ROOT.TFile( fName,'r')
    
    dirs = [
        'tt_inclusive',
        'tt_0jet',
        'tt_boosted',
        'tt_vbf',
    ]
    
    
    for dir in dirs :
        print dir
        for bkg in bkgs :
            h = f.Get(dir+'/'+bkg)
            #print ' - ',bkg, h.Integral()
            toRoot = 0.0
            for b in range(1, h.GetXaxis().GetNbins()+1 ) :
                toRoot += h.GetBinError( b )**2
            print ' - %40s %10.4f +/- %4.4f' % (bkg, h.Integral(), math.sqrt(toRoot))


bkgs_nom = [
    'ggH_htt125',
    'qqH_htt125',
    'WH_htt125',
    'ZH_htt125',
]

bkgs_s0 = [
    "ggH_fwd_htt125",
    "ggH_htt125",
    "qqH_fwd_htt125",
    "qqH_htt125",
    "VH_had_fwd_htt125",
    "VH_had_htt125",
    "WH_lep_fwd_htt125",
    "WH_lep_htt125",
    "ZH_lep_fwd_htt125",
    "ZH_lep_htt125"
]

bkgs_s1 = [
    "ggH_VBFTOPO_JET3VETO_htt125",
    "ggH_VBFTOPO_JET3_htt125",
    "ggH_0J_htt125",
    "ggH_1J_PTH_0_60_htt125",
    "ggH_1J_PTH_60_120_htt125",
    "ggH_1J_PTH_120_200_htt125",
    "ggH_1J_PTH_GT200_htt125",
    "ggH_GE2J_PTH_0_60_htt125",
    "ggH_GE2J_PTH_60_120_htt125",
    "ggH_GE2J_PTH_120_200_htt125",
    "ggH_GE2J_PTH_GT200_htt125",

    "qqH_FWDH_htt125",
    "qqH_VBFTOPO_JET3VETO_htt125",
    "qqH_VBFTOPO_JET3_htt125",
    "qqH_VH2JET_htt125",
    "qqH_REST_htt125",
    "qqH_PTJET1_GT200_htt125",

    "VH_had_FWDH_htt125",
    "VH_had_VBFTOPO_JET3VETO_htt125",
    "VH_had_VBFTOPO_JET3_htt125",
    "VH_had_VH2JET_htt125",
    "VH_had_REST_htt125",
    "VH_had_PTJET1_GT200_htt125",

    "WH_lep_FWDH_htt125",
    "WH_lep_PTV_0_150_htt125",
    "WH_lep_PTV_150_250_0J_htt125",
    "WH_lep_PTV_150_250_GE1J_htt125",
    "WH_lep_PTV_GT250_htt125",

    "ZH_lep_FWDH_htt125",
    "ZH_lep_PTV_0_150_htt125",
    "ZH_lep_PTV_150_250_0J_htt125",
    "ZH_lep_PTV_150_250_GE1J_htt125",
    "ZH_lep_PTV_GT250_htt125"
]


            
n = 'shapes/USCMS/htt_tt.inputs-sm-13TeV-2D.root'
print n
pYields( n, bkgs_nom )
n = 'shapes/USCMS/htt_tt.inputs-sm-13TeV-2D-HTXS.root'
print n
pYields( n, bkgs_s0 )
n = 'shapes/USCMS/htt_tt.inputs-sm-13TeV-2D-HTXS.root'
print n
pYields( n, bkgs_s1 )

