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

def countEvents(root_file):
    result = -1
    return result

def processDir(directory):
    print("Counting events.")
    print("directory: {0}".format(directory))
    
    # get ROOT files
    root_files = glob.glob("{0}/*.root".format(directory))
    n_root_files = len(root_files)

    n_events_map = {}

    print("Found {0} ROOT files:".format(n_root_files))
    for root_file in root_files:
        base_name = os.path.basename(root_file)
        n_events = countEvents(root_file)
        n_events_map[base_name] = n_events
        print(" - {0}".format(base_name))

    print("Number of events:")
    for base_name in n_events_map:
        n_events = n_events_map[base_name]
        print("{0}: {1}".format(base_name, n_events))

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
    
    processDir(directory)

def main():
    run()

if __name__ == "__main__":
    main()

