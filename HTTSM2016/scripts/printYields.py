import ROOT
import math

def pYields ( fName ) :
    f = ROOT.TFile( fName,'r')
    
    dirs = [
        'tt_inclusive',
        'tt_0jet',
        'tt_boosted',
        'tt_vbf',
    ]
    
    bkgs = [
        'ggH_htt125',

        'ggH_htt_GG2H125',
        'ggH_htt_GG2H_FWDH125',

        'ggH_htt_GG2H_VBFTOPO_JET3VETO125',
        'ggH_htt_GG2H_VBFTOPO_JET3125',
        'ggH_htt_GG2H_0J125',
        'ggH_htt_GG2H_1J_PTH_0_60125',
        'ggH_htt_GG2H_1J_PTH_60_120125',
        'ggH_htt_GG2H_1J_PTH_120_200125',
        'ggH_htt_GG2H_1J_PTH_GT200125',
        'ggH_htt_GG2H_GE2J_PTH_0_60125',
        'ggH_htt_GG2H_GE2J_PTH_60_120125',
        'ggH_htt_GG2H_GE2J_PTH_120_200125',
        'ggH_htt_GG2H_GE2J_PTH_GT200125',

        'qqH_htt125',

        'qqH_htt_VBF125',
        'qqH_htt_VBF_FWDH125',

        'qqH_htt_QQ2HQQ_FWDH125',
        'qqH_htt_QQ2HQQ_VBFTOPO_JET3VETO125',
        'qqH_htt_QQ2HQQ_VBFTOPO_JET3125',
        'qqH_htt_QQ2HQQ_VH2JET125',
        'qqH_htt_QQ2HQQ_REST125',
        'qqH_htt_QQ2HQQ_PTJET1_GT200125',


        'ZH_htt125',

        'ZH_htt_VH2HQQ125',
        'ZH_htt_VH2HQQ_FWDH125',
        'ZH_htt_QQ2HLL_FWDH125',
        'ZH_htt_QQ2HLL125',

        'ZH_htt_QQ2HQQ_FWDH125',
        'ZH_htt_QQ2HQQ_VBFTOPO_JET3VETO125',
        'ZH_htt_QQ2HQQ_VBFTOPO_JET3125',
        'ZH_htt_QQ2HQQ_VH2JET125',
        'ZH_htt_QQ2HQQ_REST125',
        'ZH_htt_QQ2HQQ_PTJET1_GT200125',
        'ZH_htt_QQ2HLL_FWDH125',
        'ZH_htt_QQ2HLL_PTV_0_150125',
        'ZH_htt_QQ2HLL_PTV_150_250_0J125',
        'ZH_htt_QQ2HLL_PTV_150_250_GE1J125',
        'ZH_htt_QQ2HLL_PTV_GT250125',


        'WH_htt125',

        'WH_htt_VH2HQQ125',
        'WH_htt_QQ2HLNU125',
        'WH_htt_QQ2HLNU_FWDH125',

        'WH_htt_QQ2HQQ_VBFTOPO_JET3VETO125',
        'WH_htt_QQ2HQQ_VBFTOPO_JET3125',
        'WH_htt_QQ2HQQ_VH2JET125',
        'WH_htt_QQ2HQQ_REST125',
        'WH_htt_QQ2HQQ_PTJET1_GT200125',
        'WH_htt_QQ2HLNU_FWDH125',
        'WH_htt_QQ2HLNU_PTV_0_150125',
        'WH_htt_QQ2HLNU_PTV_150_250_0J125',
        'WH_htt_QQ2HLNU_PTV_150_250_GE1J125',
        'WH_htt_QQ2HLNU_PTV_GT250125',
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
            
n = 'shapes/USCMS/htt_tt.inputs-sm-13TeV-2D.root'
print n
pYields( n )

