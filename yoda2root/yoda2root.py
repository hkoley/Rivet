#! /usr/bin/env python

"""\
%(prog)s yodafile [rootfile]
Convert a YODA data file to the ROOT data format.
"""

import yoda, os, sys, argparse
from yoda.script_helpers import parse_x2y_args

parser = argparse.ArgumentParser(usage=__doc__)
parser.add_argument("ARGS", nargs="+", help="infile [outfile]")
parser.add_argument("-m", "--match", dest="MATCH", metavar="PATT", default=None,
                    help="only write out histograms whose path matches this regex")
parser.add_argument("-M", "--unmatch", dest="UNMATCH", metavar="PATT", default=None,
                    help="exclude histograms whose path matches this regex")
parser.add_argument("-g", "--as-graphs", dest="AS_GRAPHS", action="store_true", default=False,
                    help="convert to ROOT TGraphs rather than native types")
parser.add_argument("-f", "--use-focus", dest="USE_FOCUS", action="store_true", default=False,
                    help="use the bin focus rather than centre when creating TGraph points")
parser.add_argument("-d", "--div-by-binsize", dest="DIVBINSIZE", action="store_true", default=False,
                    help="divide bin values by bin size (width, area, etc.)")

args = parser.parse_args()
in_out = parse_x2y_args(args.ARGS, ".yoda", ".root")
if not in_out:
    sys.stderr.write("You must specify the YODA and ROOT file names\n")
    sys.exit(1)

try:
    import ROOT
    ROOT.gROOT.SetBatch(True)
except ImportError:
    sys.stderr.write("Could not load ROOT Python module, exiting...\n")
    sys.exit(2)

for i, o in in_out:
    of = ROOT.TFile(o, "recreate")
    analysisobjects = yoda.readYODA(i, False, args.MATCH, args.UNMATCH)
    rootobjects = [yoda.root.to_root(ao,
                                     asgraph=args.AS_GRAPHS,
                                     usefocus=args.USE_FOCUS,
                                     widthscale=args.DIVBINSIZE) for ao in analysisobjects]

    ## Protect against "/" in the histogram name, which ROOT does not like
    for obj in rootobjects:
        ## It's possible for the ROOT objects to be null, if conversion failed
        if obj is None:
            continue
        ## Split the name on "/" directory separators
        parts = obj.GetName().split("/")
        ## Walk down dir tree, making it as we go
        d = of
        for part in parts[:-1]:
            subdir = d.GetDirectory(part)
            if not subdir:
                subdir = d.mkdir(part)
            d = subdir
        ## Write the histo into the leaf dir
        d.WriteTObject(obj, parts[-1])
    of.Close()
