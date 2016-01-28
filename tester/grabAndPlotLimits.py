import ROOT
import array
from ROOT import gPad


masses = [80, 90, 100, 110, 120, 130, 140, 160, 180, 600, 900, 1000, 1200, 1500, 2900, 3200]
ROOT.gROOT.SetBatch(True)

#binArray = array.array( 'd', [0,] )
#for m in masses :
#    binArray.append( m )
#numBins = len( binArray ) - 1
#
#h = ROOT.TH1F('h1','Expected Limits: MSSM H/A#arrow#tau#tau', numBins, binArray)

def plotLimits( channel ) :
    #limits = [[0 for x in range(5)] for x in range( len(masses) )]
    limits = [[0 for x in range( len(masses) )] for x in range(5)]
    #limits = [array.array( 'd', []) for x in range(5)]
    #print limits

    mCnt = 0
    for mass in masses :
        f = ROOT.TFile('%s/%i/higgsCombineTest.Asymptotic.mH%i.root' % (channel, mass, mass), 'r')
        t = f.Get('limit')
        print "Channel: ",channel,"   Mass: ",mass
        i = 0
        for row in t :
            if row.quantileExpected == -1 : continue
            #print "Sig: ",row.quantileExpected," limit: ",row.limit
            limits[i][mCnt] = row.limit
            #limits[i].append( row.limit )
            i += 1

        #print limits[mCnt]
            
        mCnt += 1
    #print limits
    #print limits[0]
    #print limits[1]
    #print limits[2]
    n = len(masses)
    neg2 = ROOT.TGraph( len(masses))
    neg1 = ROOT.TGraph( len(masses))
    med = ROOT.TGraph( len(masses))
    pos1 = ROOT.TGraph( len(masses))
    pos2 = ROOT.TGraph( len(masses))
    midShade = ROOT.TGraph( len(masses)*2)
    outShade = ROOT.TGraph( len(masses)*2)
    for i in range( len(masses) ) :
        neg2.SetPoint( i, masses[i], limits[0][i] )
        neg1.SetPoint( i, masses[i], limits[1][i] )
        med.SetPoint( i, masses[i], limits[2][i] )
        pos1.SetPoint( i, masses[i], limits[3][i] )
        pos2.SetPoint( i, masses[i], limits[4][i] )
        midShade.SetPoint( i, masses[i],limits[3][i] )
        midShade.SetPoint( n+i, masses[n-i-1],limits[1][n-i-1] )
        outShade.SetPoint( i, masses[i],limits[4][i] )
        outShade.SetPoint( n+i, masses[n-i-1],limits[0][n-i-1] )

    #c1 = ROOT.TCanvas( 'c1', 'c1', 600, 600 )
    #c1.cd()
    #c1.SetGrid()
    #c1.DrawFrame( masses[0], 0, masses[-1], max( limits[4] ) )
    #print c1.DrawFrame( masses[0], 0, masses[-1], 20.0 )
    outShade.SetFillStyle(1001)
    outShade.SetFillColor(5)
    #outShade.Draw('f')
    midShade.SetFillStyle(1001)
    midShade.SetFillColor(3)
    #midShade.Draw('f')
    ##c1.DrawFrame()
    #neg1.Draw('l')
    #neg2.Draw('l same')
    #med.Draw('l same')
    #pos1.Draw('l same')
    #pos2.Draw('l same')
    ##p1.Update()
    ##pad = gPad
    ##pad.SetLogy()
    #print gPad
    #px = gPad.GetPrimitive( "hframe" )
    #print px
    #c1.SaveAs('/afs/cern.ch/user/t/truggles/www/limits/limitsTest_%s.png' % channel)
    c2 = ROOT.TCanvas( 'c2', 'c2', 600, 600 )
    p1 = ROOT.TPad( 'p1', 'p1', 0, 0, 1, 1)
    p1.Draw()
    p1.cd()
    med.SetLineStyle(2)
    outShade.GetXaxis().SetTitle('GeV')
    outShade.GetYaxis().SetTitle('Cross Section X BR (pb^{1})')
    outShade.SetTitle('Expected Limits A/H #rightarrow #tau#tau: Channel %s' % channel)
    
    outShade.Draw('Af')
    midShade.Draw('f')
    #neg1.Draw('l')
    #neg2.Draw('l same')
    med.Draw('l')
    #pos1.Draw('l same')
    #pos2.Draw('l same')
    #px.Draw()
    p1.SetLogy()
    
    c2.SaveAs('/afs/cern.ch/user/t/truggles/www/limits/limitsTest_%s2.png' % channel)

    print "PNG files saved here: /afs/cern.ch/user/t/truggles/www/limits/"
    print "They are visible online at: http://truggles.web.cern.ch/truggles/limits/"
    
    


channels = ['em', 'tt']
for channel in channels :
    plotLimits( channel )
