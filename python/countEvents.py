# countEvents.py

import os
import glob
import ROOT
import argparse
import tools

#
# Count the number of "total" and "saved" events for all ROOT files in a directory.
# Print the results and save results to a csv file.
#
# Example syntax:
# python python/countEvents.py --directory <path_to_directory> --csv <path_to_csv>
#

# TODO
# DONE
# - Update get_eos_file_list() to use a pattern
# - Sort sample names alphabetically for printing and csv

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)

# count total events in a ROOT file
# iterate over entries in EventCount tree
def countTotalEvents(root_file):
    result = 0
    tree_name = "EventCount"
    chain = ROOT.TChain(tree_name)
    chain.Add(root_file)
    n_entries = chain.GetEntries()
    for i in range(n_entries):
        chain.GetEntry(i)
        n_events = chain.Nevent
        result += n_events
    return int(result)

# count saved events in a ROOT file
# use the number of entries in the KUAnalysis tree
def countSavedEvents(root_file):
    tree_name = "KUAnalysis"
    chain = ROOT.TChain(tree_name)
    chain.Add(root_file)
    n_events = chain.GetEntries()
    return int(n_events)

# process directory containing ROOT files
def processDir(directory, pattern, csv, eos, verbose):
    if verbose:
        print("Counting events.")
        print("----------------------------")
        print("directory: {0}".format(directory))
        print("pattern: {0}".format(pattern))
        print("csv: {0}".format(csv))
        print("eos: {0}".format(eos))
        print("verbose: {0}".format(verbose))
        print("----------------------------")
    
    root_files = []
    base_file_names = []
    output_data = []
    n_events_map = {}
    sum_total_events = 0
    sum_saved_events = 0
    
    # get ROOT files
    if eos:
        # Note: for eos, files are assumed to be ROOT files; don't use pattern = "*.root"
        # Note: for eos, the pattern is requried to be a substring of the file name
        root_files = tools.get_eos_file_list(directory, pattern)
    else:
        # Note: for glob, the pattern is passed to glob
        root_files = tools.get_file_list_glob(directory, pattern)

    # sort ROOT files alphabetically    
    root_files.sort()
    n_root_files = len(root_files)

    if verbose:
        #print("ROOT files: {0}".format(root_files))
        print("Found {0} ROOT files:".format(n_root_files))

    # headers for csv
    output_data.append(["sample", "total_events", "saved_events"])

    # count events
    for root_file in root_files:
        base_name = os.path.basename(root_file)
        base_file_names.append(base_name)
        n_total_events = countTotalEvents(root_file)
        n_saved_events = countSavedEvents(root_file)
        n_events_map[base_name] = {}
        n_events_map[base_name]["n_total_events"] = n_total_events
        n_events_map[base_name]["n_saved_events"] = n_saved_events
        output_data.append([base_name, n_total_events, n_saved_events])
        sum_total_events += n_total_events
        sum_saved_events += n_saved_events
        if verbose:
            print(" - {0}".format(base_name))

    # print results
    print("Sample: total events, saved events")
    for base_name in base_file_names:
        n_total_events = n_events_map[base_name]["n_total_events"]
        n_saved_events = n_events_map[base_name]["n_saved_events"]
        print("{0}: {1}, {2}".format(base_name, n_total_events, n_saved_events))
    print("Sum of total events from all samples: {0}".format(sum_total_events))
    print("Sum of saved events from all samples: {0}".format(sum_saved_events))
    
    # if csv file name is set, then save data to csv file
    if csv:
        tools.writeCSV(csv, output_data)

def run():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--directory",  "-d", default="",                               help="directory containing ROOT files (required)")
    parser.add_argument("--pattern",    "-p", default="*.root",                         help="pattern for root file names (optional)")
    parser.add_argument("--csv",        "-c", default="",                               help="output csv file name (optional)")
    parser.add_argument("--eos",        "-e", default = False,  action = "store_true",  help="run over ROOT files on EOS")
    parser.add_argument("--verbose",    "-v", default = False,  action = "store_true",  help="verbose flag to print more things")

    options     = parser.parse_args()
    directory   = options.directory
    pattern     = options.pattern
    csv         = options.csv
    eos         = options.eos
    verbose     = options.verbose

    # check that directory is set
    if not directory:
        print("ERROR: 'directory' is not set. Please provide a directory using the -d option.")
        return
    
    processDir(directory, pattern, csv, eos, verbose)

def main():
    run()

if __name__ == "__main__":
    main()

