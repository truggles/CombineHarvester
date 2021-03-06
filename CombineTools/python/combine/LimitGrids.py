import ROOT
import json
import itertools
import glob
import sys
import re
import zipfile
import os
from math import floor
from array import array

import CombineHarvester.CombineTools.combine.utils as utils
from CombineHarvester.CombineTools.combine.CombineToolBase import CombineToolBase
import CombineHarvester.CombineTools.plotting as plot

class AsymptoticGrid(CombineToolBase):
  description = 'Calculate asymptotic limits on parameter grids' 
  requires_root = True

  def __init__(self):
    CombineToolBase.__init__(self)

  def attach_intercept_args(self, group):
    CombineToolBase.attach_intercept_args(self, group)
    group.add_argument('--setPhysicsModelParameters', default=None)
    group.add_argument('--freezeNuisances', default=None)

  def attach_args(self, group):
    CombineToolBase.attach_args(self, group)
    group.add_argument('config', help='json config file')

  def run_method(self):
    ROOT.PyConfig.IgnoreCommandLineOptions = True
    ROOT.gROOT.SetBatch(ROOT.kTRUE)

    # This is what the logic should be:
    #  - get the list of model points
    #  - figure out which files are:
    #    - completely missing
    #    - there but corrupt, missing tree
    #    - ok
    #  - If we have anything in the third category proceed to produce output files
    #  - Anything in the first two gets added to the queue only if --doFits is specified
    #    so that the 


    # Step 1 - open the json config file
    with open(self.args.config) as json_file:    
        cfg = json.load(json_file)
    # to do - have to handle the case where it doesn't exist
    points = []; blacklisted_points = []
    for igrid in cfg['grids']:
      assert(len(igrid) == 3)
      if igrid[2]=='' : points.extend(itertools.product(utils.split_vals(igrid[0]), utils.split_vals(igrid[1])))
      else : blacklisted_points.extend(itertools.product(utils.split_vals(igrid[0]), utils.split_vals(igrid[1]), utils.split_vals(igrid[2])))
    POIs = cfg['POIs']
    opts = cfg['opts']

    # Have to merge some arguments from both the command line and the "opts" in the json file
    to_freeze = []
    to_set = []
    set_opt, opts = self.extract_arg('--setPhysicsModelParameters', opts)
    if set_opt is not None: to_set.append(set_opt)
    freeze_opt, opts = self.extract_arg('--freezeNuisances', opts)
    if freeze_opt is not None: to_freeze.append(freeze_opt)
    if hasattr(self.args, 'setPhysicsModelParameters') and self.args.setPhysicsModelParameters is not None:
        to_set.append(self.args.setPhysicsModelParameters)
    if hasattr(self.args, 'freezeNuisances') and self.args.freezeNuisances is not None:
        to_freeze.append(self.args.freezeNuisances)

    file_dict = { }
    for p in points:
      file_dict[p] = []

    for f in glob.glob('higgsCombine.%s.*.%s.*.Asymptotic.mH*.root' % (POIs[0], POIs[1])):
      # print f
      rgx = re.compile('higgsCombine\.%s\.(?P<p1>.*)\.%s\.(?P<p2>.*)\.Asymptotic\.mH.*\.root' % (POIs[0], POIs[1]))
      matches = rgx.search(f)
      p = (matches.group('p1'), matches.group('p2'))
      if p in file_dict:
        file_dict[p].append(f)

    for key,val in file_dict.iteritems():
      name = '%s.%s.%s.%s' % (POIs[0], key[0], POIs[1], key[1])
      print '>> Point %s' % name
      if len(val) == 0:
        print 'Going to run limit for point %s' % (key,)
        set_arg = ','.join(['%s=%s,%s=%s' % (POIs[0], key[0], POIs[1], key[1])] + to_set)
        freeze_arg = ','.join(['%s,%s' % (POIs[0], POIs[1])] + to_freeze)
        point_args = '-n .%s --setPhysicsModelParameters %s --freezeNuisances %s' % (name, set_arg, freeze_arg)
        cmd = ' '.join(['combine -M Asymptotic', opts, point_args] + self.passthru)
        self.job_queue.append(cmd)

    bail_out = len(self.job_queue) > 0
    self.flush_queue()

    if bail_out: 
        print '>> New jobs were created / run in this cycle, run the script again to collect the output'
        sys.exit(0)

    xvals = []
    yvals = []
    zvals_m2s = []; zvals_m1s = []; zvals_exp = []; zvals_p1s = []; zvals_p2s = []; zvals_obs = []
    for key,val in file_dict.iteritems():
      for filename in val:
        fin = ROOT.TFile(filename)
        if fin.IsZombie(): continue
        tree = fin.Get('limit')
        for evt in tree:
          if abs(evt.quantileExpected+1)<0.01:
            xvals.append(float(key[0]))
            yvals.append(float(key[1]))
            #print 'At point %s have observed CLs = %f' % (key, evt.limit)
            zvals_obs.append(float(evt.limit))
          if abs(evt.quantileExpected-0.025)<0.01:
            #print 'At point %s have -2sigma CLs = %f' % (key, evt.limit)
            zvals_m2s.append(float(evt.limit))
          if abs(evt.quantileExpected-0.16)<0.01:
            #print 'At point %s have -1sigma CLs = %f' % (key, evt.limit)
            zvals_m1s.append(float(evt.limit))
          if abs(evt.quantileExpected-0.5)<0.01:
            #print 'At point %s have expected CLs = %f' % (key, evt.limit)
            zvals_exp.append(float(evt.limit))
          if abs(evt.quantileExpected-0.84)<0.01:
            #print 'At point %s have +1sigma CLs = %f' % (key, evt.limit)
            zvals_p1s.append(float(evt.limit))
          if abs(evt.quantileExpected-0.975)<0.01:
            #print 'At point %s have +2sigma CLs = %f' % (key, evt.limit)
            zvals_p2s.append(float(evt.limit))
    for POI1, POI2, CLs in blacklisted_points:
      xvals.append(float(POI1))
      yvals.append(float(POI2))
      zvals_m2s.append(float(CLs))
      zvals_m1s.append(float(CLs))
      zvals_exp.append(float(CLs))
      zvals_p1s.append(float(CLs))
      zvals_p2s.append(float(CLs))
      zvals_obs.append(float(CLs))
    graph_m2s = ROOT.TGraph2D(len(zvals_m2s), array('d', xvals), array('d', yvals), array('d', zvals_m2s))
    graph_m1s = ROOT.TGraph2D(len(zvals_m1s), array('d', xvals), array('d', yvals), array('d', zvals_m1s))
    graph_exp = ROOT.TGraph2D(len(zvals_exp), array('d', xvals), array('d', yvals), array('d', zvals_exp))
    graph_p1s = ROOT.TGraph2D(len(zvals_p1s), array('d', xvals), array('d', yvals), array('d', zvals_p1s))
    graph_p2s = ROOT.TGraph2D(len(zvals_p2s), array('d', xvals), array('d', yvals), array('d', zvals_p2s))
    graph_obs = ROOT.TGraph2D(len(zvals_obs), array('d', xvals), array('d', yvals), array('d', zvals_obs))
    #h_bins = cfg['hist_binning']
    #hist = ROOT.TH2F('h_observed', '', h_bins[0], h_bins[1], h_bins[2], h_bins[3], h_bins[4], h_bins[5])
    #for i in xrange(1, hist.GetNbinsX()+1):
    #  for j in xrange(1, hist.GetNbinsY()+1):
    #    hist.SetBinContent(i, j, graph.Interpolate(hist.GetXaxis().GetBinCenter(i), hist.GetYaxis().GetBinCenter(j)))
    fout = ROOT.TFile('asymptotic_grid.root', 'RECREATE')
    fout.WriteTObject(graph_m2s, 'exp-2')
    fout.WriteTObject(graph_m1s, 'exp-1')
    fout.WriteTObject(graph_exp, 'exp0')
    fout.WriteTObject(graph_p1s, 'exp+1')
    fout.WriteTObject(graph_p2s, 'exp+2')
    fout.WriteTObject(graph_obs, 'obs')
    #fout.WriteTObject(hist)
    fout.Close()
    # Next step: open output files
    # Fill TGraph2D with CLs, CLs+b

class HybridNewGrid(CombineToolBase):
    description = 'Calculate toy-based limits on parameter grids'
    requires_root = True

    def __init__(self):
        CombineToolBase.__init__(self)

    def attach_intercept_args(self, group):
        CombineToolBase.attach_intercept_args(self, group)
        group.add_argument('--setPhysicsModelParameters', default=None)
        group.add_argument('--freezeNuisances', default=None)
    def attach_args(self, group):
        CombineToolBase.attach_args(self, group)
        group.add_argument('config', help='json config file')
        group.add_argument('--cycles', default=0, type=int, help='Number of job cycles to create per point')
        group.add_argument('--output', action='store_true', help='Write CLs grids into an output file')

    def GetCombinedHypoTest(self, files):
        if len(files) == 0: return None
        results = []
        for file in files:
            found_res = False
            f = ROOT.TFile(file)
            ROOT.gDirectory.cd('toys')
            for key in ROOT.gDirectory.GetListOfKeys():
                if ROOT.gROOT.GetClass(key.GetClassName()).InheritsFrom(ROOT.RooStats.HypoTestResult.Class()):
                    results.append(ROOT.gDirectory.Get(key.GetName()))
                    found_res = True
            f.Close()
            if not found_res:
                print '>> Warning, did not find a HypoTestResult object in file %s' % file
        if (len(results)) > 1:
          for r in results[1:]:
            results[0].Append(r)
        ntoys = min(results[0].GetNullDistribution().GetSize(), results[0].GetAltDistribution().GetSize())
        if ntoys == 0:
            print '>> Warning, HypoTestResult from file(s) %s does not contain any toy results, did something go wrong in your fits?' % '+'.join(files)
        return results[0]

    def ValidateHypoTest(self, hyp_res, min_toys, max_toys, contours, signif, cl, output=False, verbose=False):
        results = {}

        # We will take the number of toys thrown as the minimum of the number of b-only or s+b toys
        ntoys = min(hyp_res.GetNullDistribution().GetSize(), hyp_res.GetAltDistribution().GetSize())
        results['ntoys'] = ntoys

        if verbose:
            print '>>> Toys completed: %i [min=%i, max=%i]' % (ntoys, min_toys, max_toys)

        # If we're not going to write the CLs grids out and we fail the ntoys criteria then we
        # don't need to waste time getting all the CLs values. Can just return the results dict as-is.
        # 1st test - have we thrown at least the minimum number of toys?
        if ntoys < min_toys and not output:
            return (False, results)
        # 2nd test - have we thrown the maximum (or more) toys?
        if ntoys >= max_toys and not output:
            return (True, results)

        # 3rd test - are we > X sigma away from the exclusion CLs? This must be true for all the
        # contours we're interested in
        btoys = sorted([x for x in hyp_res.GetNullDistribution().GetSamplingDistribution()])
        crossing = 1 - cl
        signif_results = {}

        # save the real observed test stat, we'll change it in this
        # loop to get the expected but we'll want to restore it at the end
        q_obs = hyp_res.GetTestStatisticData()

        if verbose:
            print '>>> CLs target is a significance of %.1f standard deviations from %.3f' % (signif, crossing)

        for contour in contours:
            # Start by assuming this contour passes, we'll set it to False if it fails
            signif_results[contour] = True

            # If this is an expected contour we will extract the quantile from the name
            if 'exp' in contour:
                quantile = ROOT.Math.normal_cdf(float(contour.replace('exp', '')))
                if verbose:
                    print '>>> Checking the %s contour at quantile=%f' % (contour, quantile)
                # Get the stat statistic value at this quantile by rounding to the nearest b-only toy
                testStat = btoys[int(min(floor(quantile * len(btoys) +0.5), len(btoys)))]
                hyp_res.SetTestStatisticData(testStat)
            elif contour == 'obs':
                if verbose: print '>>> Checking the %s contour' % contour
            else:
                raise RuntimeError('Contour %s not recognised' % contour)

            # Currently assume we always want to use CLs, should provide option
            # for CLs+b at some point
            CLs = hyp_res.CLs()
            CLsErr = hyp_res.CLsError()
            if ntoys == 0: CLsErr = 0 # If there were no toys then ClsError() will return inf
            dist = 0.
            if CLsErr == 0.:
                if verbose:
                    print '>>>> CLs = %g +/- %g (infinite significance), will treat as passing' % (CLs, CLsErr)
                dist = 999.
            else:
                dist = abs(CLs - crossing) / CLsErr
                if verbose:
                    print '>>>> CLs = %g +/- %g, reached %.1f sigma signifance' % (CLs, CLsErr, dist)
                if dist < signif:
                    signif_results[contour] = False
            results[contour] = (CLs, CLsErr, dist)
            # Set the observed test statistic back to the real data value
            hyp_res.SetTestStatisticData(q_obs)

        # Now do the full logic of the validation and return
        all_ok = (ntoys >= min_toys) # OK if min toys passes
        for (key, val) in signif_results.iteritems():
            all_ok = all_ok and val # still OK if all contour significances pass
        all_ok = all_ok or (ntoys >= max_toys) # Always OK if we've reached the maximum
        results['ok'] = all_ok
        return (all_ok, results)


    def run_method(self):
        ROOT.PyConfig.IgnoreCommandLineOptions = True
        ROOT.gROOT.SetBatch(ROOT.kTRUE)

        # Open the json config file
        with open(self.args.config) as json_file:
            cfg = json.load(json_file)

        # Set all the parameter values locally using defaults if necessary
        grids           = cfg['grids']
        POIs            = cfg['POIs']
        opts            = cfg['opts']
        toys_per_cycle  = cfg['toys_per_cycle']
        zipname         = cfg.get('zipfile',    None)
        contours        = cfg.get('contours',   ['obs', 'exp-2', 'exp-1', 'exp0', 'exp+1', 'exp+2'])
        min_toys        = cfg.get('min_toys',   500)
        max_toys        = cfg.get('max_toys',   5000)
        signif          = cfg.get('signif',     3.0)
        cl              = cfg.get('CL',         0.95)
        verbose         = cfg.get('verbose',    False)
        make_plots      = cfg.get('make_plots', False)
        # Write CLs values into the output even if current toys do not pass validation
        incomplete      = cfg.get('output_incomplete', False)
        outfile         = cfg.get('output','hybrid_grid.root')
        # NB: blacklisting not yet implemented for this method

        # Have to merge some arguments from both the command line and the "opts" in the json file
        to_freeze = []
        to_set = []
        set_opt, opts = self.extract_arg('--setPhysicsModelParameters', opts)
        if set_opt is not None: to_set.append(set_opt)
        freeze_opt, opts = self.extract_arg('--freezeNuisances', opts)
        if freeze_opt is not None: to_freeze.append(freeze_opt)
        if hasattr(self.args, 'setPhysicsModelParameters') and self.args.setPhysicsModelParameters is not None:
            to_set.append(self.args.setPhysicsModelParameters)
        if hasattr(self.args, 'freezeNuisances') and self.args.freezeNuisances is not None:
            to_freeze.append(self.args.freezeNuisances)

        points = []; blacklisted_points = []
        for igrid in grids:
            assert(len(igrid) == 3)
            if igrid[2] == '':
                points.extend(itertools.product(utils.split_vals(igrid[0]), utils.split_vals(igrid[1])))
            else:
                blacklisted_points.extend(itertools.product(utils.split_vals(igrid[0]), utils.split_vals(igrid[1]), utils.split_vals(igrid[2])))

        # This dictionary will keep track of the combine output files for each model point
        file_dict = { }
        for p in points:
            file_dict[p] = {}

        # The regex we will use to identify output files and extract POI values
        rgx = re.compile('higgsCombine\.%s\.(?P<p1>.*)\.%s\.(?P<p2>.*)\.HybridNew\.mH.*\.(?P<toy>.*)\.root' % (POIs[0], POIs[1]))

        # Can optionally copy output root files into a zip archive
        # If the user has specified a zipfile we will first
        # look for output files in this archive before scanning the
        # current directory
        if zipname:
            # Open the zip file in append mode, this should also
            # create it if it doesn't exist
            zipf = zipfile.ZipFile(zipname, 'a')
            for f in zipf.namelist():
                matches = rgx.search(f)
                p = (matches.group('p1'), matches.group('p2'))
                seed = int(matches.group('toy'))
                if p in file_dict:
                    if seed not in file_dict[p]:
                        # For each model point have a dictionary keyed on the seed number
                        # with a value pointing to the file in the archive in the format
                        # ROOT expects: "zipfile.zip#higgsCombine.blah.root"
                        file_dict[p][seed] = zipname+'#'+f

        # Now look for files in the local directory
        for f in glob.glob('higgsCombine.%s.*.%s.*.HybridNew.mH*.root' % (POIs[0], POIs[1])):
            matches = rgx.search(f)
            p = (matches.group('p1'), matches.group('p2'))
            seed = int(matches.group('toy'))
            if p in file_dict:
                # Don't add this file to the list if its seed number is already
                # a value in the dict.
                if seed not in file_dict[p]:
                    # If we're using the zipfile we'll add this now and
                    # then delete it from the local directory
                    # But: only in the file is good, we don't want to pollute the zip
                    # file with incomplete or failed jobs
                    if zipname and plot.TFileIsGood(f):
                        zipf.write(f) # assume this throws if it fails
                        print 'Adding %s to %s' % (f, zipname)
                        file_dict[p][seed] = zipname+'#'+f
                        os.remove(f)
                    else:  # otherwise just add the file to the dict in the normal way
                        file_dict[p][seed] = f

        if zipname:
            zipf.close()

        # These lists will keep track of the CLs values which we will use
        # to create the output TGraph2Ds
        output_x = []
        output_y = []
        output_data = {}
        output_ntoys = []
        output_clserr = {}
        output_signif = {}
        # One list of Z-values per contour 
        for contour in contours:
            output_data[contour] = []
            output_clserr[contour] = []
            output_signif[contour] = []


        # Also keep track of the number of model points which have met the
        # CLs criteria
        total_points = 0
        complete_points = 0

        for key,val in file_dict.iteritems():
            total_points += 1
            name = '%s.%s.%s.%s' % (POIs[0], key[0], POIs[1], key[1])
            files = [x for x in val.values() if plot.TFileIsGood(x)]
            # Merge the HypoTestResult objects from each file into one
            res = self.GetCombinedHypoTest(files)

            # Do the validation of this model point
            # 
            ok, point_res = self.ValidateHypoTest(res,
                min_toys = min_toys,
                max_toys = max_toys,
                contours = contours,
                signif   = signif,
                cl       = cl,
                output   = self.args.output,
                verbose  = verbose) if res is not None else (False, {"ntoys" : 0})

            print '>> Point %s [%i toys, %s]' % (name, point_res['ntoys'], 'DONE' if ok else 'INCOMPLETE')
            
            if ok:
                complete_points += 1
            
            # Make plots of the test statistic distributions if requested
            if res is not None and make_plots:
                self.PlotTestStat(res, 'plot_'+name, opts = cfg['plot_settings'], poi_vals = (float(key[0]), float(key[1])))
  
            # Add the resulting CLs values to the output arrays. Normally just
            # for the model points that passed the validation criteria, but if "output_incomplete"
            # has been set to true then we'll write all model points where at least one HypoTestResult
            # is present
            if res is not None and (ok or incomplete) and self.args.output:
                output_x.append(float(key[0]))
                output_y.append(float(key[1]))
                output_ntoys.append(point_res['ntoys'])
                for contour in contours:
                    output_data[contour].append(point_res[contour][0])
                    output_clserr[contour].append(point_res[contour][1])
                    output_signif[contour].append(point_res[contour][2])
            
            # Do the job cycle generation if requested
            if not ok and self.args.cycles > 0:
                print '>>> Going to generate %i job(s) for point %s' % (self.args.cycles, key)
                # Figure out the next seed numbers we need to run by finding the maximum seed number
                # so far
                done_cycles = val.keys()
                new_idx = max(done_cycles)+1 if len(done_cycles) > 0 else 1
                new_cycles = range(new_idx, new_idx+self.args.cycles)
                
                print '>>> Done cycles: ' + ','.join(str(x) for x in done_cycles)
                print '>>> New cycles: ' + ','.join(str(x) for x in new_cycles)
                
                # Build to combine command. Here we'll take responsibility for setting the name and the
                # model parameters, making sure the latter are frozen
                set_arg = ','.join(['%s=%s,%s=%s' % (POIs[0], key[0], POIs[1], key[1])] + to_set)
                freeze_arg = ','.join(['%s,%s' % (POIs[0], POIs[1])] + to_freeze)
                point_args = '-n .%s --setPhysicsModelParameters %s --freezeNuisances %s' % (name, set_arg, freeze_arg)
                # Build a command for each job cycle setting the number of toys and random seed and passing through any other
                # user options from the config file or the command line
                for idx in new_cycles:
                    cmd = ' '.join(['combine -M HybridNew', opts, point_args, '-T %i' % toys_per_cycle, '-s %i' % idx] + self.passthru)
                    self.job_queue.append(cmd)

        print ">> %i/%i points have completed and require no further toys" % (complete_points, total_points)
        self.flush_queue()

        # Create and write output CLs TGraph2Ds here
        # TODO: add graphs with the CLs errors, the numbers of toys and whether or not the point passes
        if self.args.output:
            fout = ROOT.TFile(outfile, 'RECREATE')
            for c in contours:
                graph = ROOT.TGraph2D(len(output_data[c]), array('d', output_x), array('d', output_y), array('d', output_data[c]))
                graph.SetName(c)
                fout.WriteTObject(graph, c)
                # Also write a Graph with the CLsErr
                graph = ROOT.TGraph2D(len(output_clserr[c]), array('d', output_x), array('d', output_y), array('d', output_clserr[c]))
                graph.SetName('clsErr_'+c)
                fout.WriteTObject(graph, 'clsErr_'+c)
                # And a Graph with the significance
                graph = ROOT.TGraph2D(len(output_signif[c]), array('d', output_x), array('d', output_y), array('d', output_signif[c]))
                graph.SetName('signif_'+c)
                fout.WriteTObject(graph, 'signif_'+c)
            graph = ROOT.TGraph2D(len(output_ntoys), array('d', output_x), array('d', output_y), array('d', output_ntoys))
            graph.SetName('ntoys'+c)
            fout.WriteTObject(graph, 'ntoys')
            fout.Close()

    def PlotTestStat(self, result, name, opts, poi_vals):
      null_vals = [x * -2. for x in result.GetNullDistribution().GetSamplingDistribution()]
      alt_vals = [x * -2. for x in result.GetAltDistribution().GetSamplingDistribution()]
      if len(null_vals) == 0 or len(alt_vals) == 0:
        print '>> Errror in PlotTestStat for %s, null and/or alt distributions are empty'
        return
      plot.ModTDRStyle()
      canv = ROOT.TCanvas(name, name)
      pad = plot.OnePad()[0]
      min_val = min(min(alt_vals), min(null_vals))
      max_val = max(max(alt_vals), max(null_vals))
      min_plot_range = min_val - 0.05 * (max_val - min_val)
      if opts['one_sided']:
        min_plot_range = 0.
        pad.SetLogy(True)
      max_plot_range = max_val + 0.05 * (max_val - min_val)
      hist_null = ROOT.TH1F('null', 'null', 40, min_plot_range, max_plot_range)
      hist_alt = ROOT.TH1F('alt', 'alt', 40, min_plot_range, max_plot_range)
      for val in null_vals: hist_null.Fill(val)
      for val in alt_vals: hist_alt.Fill(val)
      hist_alt.SetLineColor(ROOT.TColor.GetColor(4, 4, 255))
      hist_alt.SetFillColor(plot.CreateTransparentColor(ROOT.TColor.GetColor(4, 4, 255), 0.4))
      hist_alt.GetXaxis().SetTitle('-2 #times ln(^{}L_{%s}/^{}L_{%s})' % (opts['alt_label'], opts['null_label']))
      hist_alt.GetYaxis().SetTitle('Pseudo-experiments')
      hist_alt.Draw()
      hist_null.SetLineColor(ROOT.TColor.GetColor(252, 86, 11))
      hist_null.SetFillColor(plot.CreateTransparentColor(ROOT.TColor.GetColor(254, 195, 40), 0.4))
      hist_null.Draw('SAME')
      val_obs = result.GetTestStatisticData() * -2.
      obs = ROOT.TArrow(val_obs, 0, val_obs, hist_alt.GetMaximum() * 0.3, 0.05 , '<-|')
      obs.SetLineColor(ROOT.kRed)
      obs.SetLineWidth(3)
      obs.Draw()
      plot.FixTopRange(pad, plot.GetPadYMax(pad), 0.25)
      leg = plot.PositionedLegend(0.22, 0.2, 3, 0.02)
      leg.AddEntry(hist_alt, opts['alt_label'], 'F')
      leg.AddEntry(hist_null, opts['null_label'], 'F')
      leg.AddEntry(obs, 'Observed', 'L')
      leg.Draw()
      plot.DrawCMSLogo(pad, 'CMS', opts['cms_subtitle'], 0, 0.15, 0.035, 1.2)
      pt_l = ROOT.TPaveText(0.23, 0.75, 0.33, 0.9, 'NDCNB')
      pt_l.AddText('Model:')
      pt_l.AddText('Toys:')
      pt_l.AddText('CLs+b:')
      pt_l.AddText('CLb:')
      pt_l.AddText('CLs:')
      plot.Set(pt_l, TextAlign=11, TextFont=62, BorderSize=0)
      pt_l.Draw()
      pt_r = ROOT.TPaveText(0.33, 0.75, 0.63, 0.9, 'NDCNB')
      pt_r.AddText('%s [%s = %.1f, %s = %.1f]' % (opts['model_label'], opts['poi_labels'][0], poi_vals[0], opts['poi_labels'][1], poi_vals[1]))
      pt_r.AddText('%i (%s) + %i (%s)' % (result.GetNullDistribution().GetSize(), opts['null_label'], result.GetAltDistribution().GetSize(), opts['alt_label']))
      pt_r.AddText('%.3f #pm %.3f' % (result.CLsplusb(), result.CLsplusbError()))
      pt_r.AddText('%.3f #pm %.3f' % (result.CLb(), result.CLbError()))
      pt_r.AddText('%.3f #pm %.3f' % (result.CLs(), result.CLsError()))
      plot.Set(pt_r, TextAlign=11, TextFont=42, BorderSize=0)
      pt_r.Draw()
      pad.GetFrame().Draw()
      pad.RedrawAxis()
      for fmt in opts['formats']:
          canv.SaveAs(fmt)
