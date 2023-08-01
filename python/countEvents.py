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
    result = 0
    tree_name = "EventCount"
    chain = ROOT.TChain(tree_name)
    chain.Add(root_file)
    n_entries = chain.GetEntries()
    for i in range(n_entries):
        chain.GetEntry(i)
        n_events = chain.Nevent
        result += n_events
        #print("n_events = {0}".format(n_events))
    return result

def processDir(directory, verbose):
    if verbose:
        print("Counting events.")
        print("directory: {0}".format(directory))
    
    total_event_count = 0
    base_file_names = []
    n_events_map = {}
    
    # get ROOT files
    root_files = glob.glob("{0}/*.root".format(directory))
    n_root_files = len(root_files)

    # count events
    if verbose:
        print("Found {0} ROOT files:".format(n_root_files))
    for root_file in root_files:
        base_name = os.path.basename(root_file)
        base_file_names.append(base_name)
        n_events = countEvents(root_file)
        n_events_map[base_name] = n_events
        total_event_count += n_events
        if verbose:
            print(" - {0}".format(base_name))

    # print results
    print("Number of events:")
    for base_name in base_file_names:
        n_events = n_events_map[base_name]
        print("{0}: {1}".format(base_name, n_events))
    print("Total event count: {0}".format(total_event_count))

def run():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--directory",  "-d", default="",       help="directory containing root files")
    parser.add_argument("--verbose",    "-v", default = False,  action = "store_true",  help="verbose flag to print more things")

    options     = parser.parse_args()
    directory   = options.directory
    verbose     = options.verbose

    # check that directory is set
    if not directory:
        print("ERROR: 'directory' is not set. Please provide a directory using the -d option.")
        return
    
    processDir(directory, verbose)

def main():
    run()

if __name__ == "__main__":
    main()

