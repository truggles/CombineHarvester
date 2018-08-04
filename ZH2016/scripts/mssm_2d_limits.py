import ROOT
import json
from array import array
from collections import OrderedDict
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

# Didn't work
#def set_color_gradient( th2 ) :
#    Red = array('d', [1.0, 0.0, 0.0])
#    Green = array('d', [0.0, 1.0, 0.0])
#    Blue = array('d', [1.0, 0.0, 1.0])
#    Length = array('d', [ th2.GetMinimum(), th2.GetMaximum() / 100., th2.GetMaximum() ] )
#    nb = 50
#    print "here",th2.GetMinimum(), th2.GetMaximum()
#    print ROOT.TColor.CreateGradientColorTable( 3, Length, Red, Green, Blue, nb )
#    ROOT.gStyle.SetNumberContours(nb)
    
def check_for_intersections( h2, mass, limit ) :
    mass_bin = h2.GetXaxis().FindBin( mass )
    tan_beta_10_bin = h2.GetYaxis().FindBin( 60.0 )
    to_save = []
    previous_tuple = []
    current_tuple = []
    count = 0
    for y_bin in range( 1, tan_beta_10_bin+1 ) :
        y_bin_val = h2.GetYaxis().GetBinCenter( y_bin )
        bin_content = h2.GetBinContent( mass_bin, y_bin )
        # Initialize first one
        if previous_tuple == [] :
            previous_tuple = [ y_bin, y_bin_val, bin_content ] 
            current_tuple = [ y_bin, y_bin_val, bin_content ] 
        previous_tuple = list( current_tuple )
        current_tuple = [ y_bin, y_bin_val, bin_content ] 
        if bin_content >= limit and limit > previous_tuple[2] :
            #print ' - intersection', limit
            #print ' --- current',current_tuple
            #print ' --- previous',previous_tuple
            to_save.append( y_bin_val )
            count += 1
            
        if bin_content <= limit and limit < previous_tuple[2]  : 
            #print ' - intersection', limit
            #print ' --- current',current_tuple
            #print ' --- previous',previous_tuple
            to_save.append( y_bin_val )
            count += 1

        #print ' - ', y_bin, bin_content
    print "Mass: %i,  Limit: %.4f,  Intersection counts: %i" % (mass, limit, count)
    if len( to_save ) == 0 : return [0,]
    return to_save



def new_pad( name = 'p' ) :
    p = ROOT.TPad( name, name, 0, 0, 1, 1)
    p.Draw()
    p.cd()
    p.SetRightMargin( 2 * p.GetRightMargin() )
    return p

def slim_hist( h ) :
    h.GetXaxis().SetRangeUser( 220, 400 )
    h.GetYaxis().SetRangeUser( 0.5, 60 )
    h.GetXaxis().SetTitle( 'm_{A} (GeV)' )
    h.GetYaxis().SetTitle( 'tan(#beta)' )
    h.GetZaxis().SetTitleOffset( 2 )
    
def get_limit_from_json( json_name, use_obs = False ) :
    with open( json_name ) as jsonFile :
        jsonDict = json.load( jsonFile )

    target = "exp0" if not use_obs else "obs"

    d1 = {}
    for mass in jsonDict :
        d1[ float( mass ) ] = float(jsonDict[ mass ][ target ]/1000.) # convert to pb to compare with HIG-14-034

    # Sort this dict
    d2 = OrderedDict(sorted(d1.items(), key=lambda t: t[0]))

    masses = array('d', [])
    limits = array('d', [])
    for mass in d2.keys() :
        print mass, d2[ mass ]
        masses.append( mass )
        limits.append( d2[ mass ] )

    #g = ROOT.TGraph( len(masses), masses, limits )
    #g.SetLineWidth( 4 )
    return d2

# Good 2D palette color
from ROOT import gStyle
#ROOT.gStyle.SetPalette( 57, array('i', [i for i in range( 1, 251)]) ) # 57 == kBird

save_base = '/afs/cern.ch/user/t/truggles/www/azh_mssm_2d/Aug04/'

model_info = {
    #'mH Mod Plus' : 'mhmodp_mu200_13TeV.root',
    #'mH Mod Minus' : 'mhmodm_13TeV.root',
    #'hMSSM' : 'hMSSM_13TeV.root',
    #'Light Stop Mod' : 'lightstopmod_13TeV.root',
    #'New mH Max' : 'newmhmax_mu200_13TeV.root',
    #'Light Stau' : 'lightstau1_13TeV.root',
    'Low TanBeta' : 'low-tb-high_13TeV.root',
    #'tauphobic' : 'tauphobic_13TeV.root',
}

c = ROOT.TCanvas( 'c', 'c', 600, 600 )
p = new_pad( 'p1' )
p.SetLogz()

for model_name in model_info.keys() :
    model_file = ROOT.TFile( 'model_files/'+model_info[ model_name ], 'r' )

    # Start with the ggA cross section
    xs_gg_A = model_file.Get( 'xs_gg_A' )
    slim_hist( xs_gg_A )
    xs_gg_A.GetZaxis().SetTitle( '#sigma ggA (fb)' )
    xs_gg_A.Draw('COLZ')
    #c.SaveAs( save_base+'tmp1.png' )

    # Next get A -> Zh BR
    br_A_Zh = model_file.Get( 'br_A_Zh' )
    slim_hist( br_A_Zh )
    br_A_Zh.GetZaxis().SetTitle( 'BR A#rightarrowZh' )
    br_A_Zh.Draw('COLZ')
    #c.SaveAs( save_base+'tmp2.png' )

    # Next get A -> Zh BR
    br_h_tautau = model_file.Get( 'br_h_tautau' )
    slim_hist( br_h_tautau )
    br_h_tautau.GetZaxis().SetTitle( 'BR h#rightarrow#tau#tau' )
    br_h_tautau.Draw('COLZ')
    #c.SaveAs( save_base+'tmp3.png' )



    # Now multiply all of them together for
    # xs ggA -> Zh, h -> tautau, Z -> ll

    # Not sure why renaming the Z axis isn't working once cloned
    # so doing it here
    xs_gg_A.GetZaxis().SetTitle( '#sigma(ggA)*BR(A#rightarrowZh #rightarrow LL#tau#tau) (pb)' )

    xs_times_br = xs_gg_A.Clone()
    xs_times_br.SetTitle( 'm_{A}-tan(#beta) Limits for '+model_name)
    xs_times_br.Multiply( br_A_Zh )
    xs_times_br.Multiply( br_h_tautau )
    br_Z_LL = 0.03363 + 0.03366 + 0.03370 # ee + mumu + tautau, PDG
    xs_times_br.Scale( br_Z_LL )
    #xs_times_br.Scale( 1000. ) # pb --> fb
    xs_times_br.Draw('COLZ')
    p.Update()
    #c.SaveAs( save_base+'tmp4.png' )

    limit_dict = get_limit_from_json( 'cmb_limits.json' )

    # Check if there are intersections
    masses = array('d', [])
    limits_y_val = array('d', [])
    for mass, limit in limit_dict.iteritems() :
        # Just take first for now, we will look into multiple crossing later
        y_vals = check_for_intersections( xs_times_br, mass, limit )
        masses.append( mass )
        limits_y_val.append( y_vals[-1] )

    g = ROOT.TGraph( len(masses), masses, limits_y_val )
    g.SetLineWidth( 4 )
    g.Draw( 'same' )


    c.SaveAs( save_base+model_name.replace(' ','_')+'.png' )



