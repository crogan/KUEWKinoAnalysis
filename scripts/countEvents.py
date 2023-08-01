# countEvents.py

import os
import glob
import ROOT
import argparse

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)

def countEvents(directory):
    print("Counting events.")
    print("directory: {0}".format(directory))
    
    # get ROOT files
    root_files = glob.glob("{0}/*.root".format(directory))
    n_root_files = len(root_files)

    print("Found {0} ROOT files:".format(n_root_files))
    for root_file in root_files:
        print(" - {0}".format(os.path.basename(root_file)))

def run():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--directory",  "-d", default="",   help="directory containing root files")

    options     = parser.parse_args()
    directory   = options.directory

    # check that directory is set
    if not directory:
        print("ERROR: 'directory' is not set. Please provide a directory using the -d option.")
        return
    
    countEvents(directory)

def main():
    run()

if __name__ == "__main__":
    main()


