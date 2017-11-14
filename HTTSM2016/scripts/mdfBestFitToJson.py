import json
import ROOT

def printJson( jDict, r_val, isBlind=False ) :

    app = '_blind' if isBlind else ''
    if r_val == 'r' : app = '_cmb'+app
    with open('Mu_%s%s.json' % (r_val, app), 'w') as outFile :
        json.dump( jDict, outFile, indent=2 )
        outFile.close()
files = {
    'higgsCombineCmb' : 'higgsCombineCmb.MultiDimFit.mH120.root',
    'higgsCombineCmbBlind' : 'higgsCombineCmbBlind.MultiDimFit.mH120.root',
    'higgsCombineMulti' : 'higgsCombineMulti.MultiDimFit.mH120.root',
    'higgsCombineMultiBlind' : 'higgsCombineMultiBlind.MultiDimFit.mH120.root',
}

for key, iFileName in files.iteritems() :
    iFile = ROOT.TFile( iFileName, 'r' )
    iTree = iFile.Get('limit')
    
    list_of_possible_r_values = [
        "r",
        "r_ggH","r_qqH","r_WH","r_ZH", # Nominal
        "r_VH_had","r_WH_lep","r_ZH_lep", # Stage0 additions
        "r_ggH_VBFTOPO_JET3VETO","r_ggH_VBFTOPO_JET3","r_ggH_0J","r_ggH_1J_PTH_0_60","r_ggH_1J_PTH_60_120","r_ggH_1J_PTH_120_200","r_ggH_1J_PTH_GT200","r_ggH_GE2J_PTH_0_60","r_ggH_GE2J_PTH_60_120","r_ggH_GE2J_PTH_120_200","r_ggH_GE2J_PTH_GT200", # Stage1 Mod additions
    ]
    
    r_in_mdf = {}
    
    for i, row in enumerate(iTree) :
        for r in list_of_possible_r_values :
            #print r
            if hasattr( row, r ) :
                r_in_mdf[ r ] = {}
                #print "mdf fit has r: ",r
        break
    
    print r_in_mdf
    
    
    for r in r_in_mdf.keys() :
        mini = 999.
        maxi = -999.
    
        recents = [0., 0., 0.]
        
        vals = {}
        if iTree.GetEntries() != 3 :
            for i, row in enumerate(iTree) :
                val = getattr( row, r )
                if val < mini : mini = val
                if val > maxi : maxi = val
                if str(val) in vals.keys() :
                    vals[ str(val) ] = vals[ str(val) ] + 1
                else : vals[ str(val) ] = 1
    
                # save 3 most recent
                recents[2] = recents[1]
                recents[1] = recents[0]
                recents[0] = str(val)
    
            final_vals = {}
            for k in vals.keys() :
                if k in recents :
                    final_vals[ k ] = vals[ k ]
    
            final_out = {
            "exp-1" : mini,
            "exp+1" : maxi,
            "exp0" : 0.,
            }
            
            for k, v in final_vals.iteritems() :
                if v > 1 :
                    final_out[ "exp0" ] = float(k)
        if iTree.GetEntries() == 3 :
            for i, row in enumerate(iTree) :
                val = getattr( row, r )
                # save 3 most recent
                recents[2] = recents[1]
                recents[1] = recents[0]
                recents[0] = val

            mini = min( recents )
            recents.remove( mini )
            maxi = max( recents )
            recents.remove( maxi )
            nom = recents[0]

            final_out = {
            "exp-1" : mini,
            "exp+1" : maxi,
            "exp0" : nom,
            }
        
        print r, final_out["exp0"], "+/-", float(final_out["exp-1"]) - float(final_out["exp0"]), float(final_out["exp+1"]) - float(final_out["exp0"])
        final_out["obs"] = final_out["exp0"]
        #print final_out
        isBlind = True if 'Blind' in key else False
        printJson( {"120.0" : final_out}, r, isBlind )


    


