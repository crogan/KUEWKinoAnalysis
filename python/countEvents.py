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
# - Add "--sms" option; for signal, count events for a specific mass point
# DONE
# - Update get_eos_file_list() to use a pattern
# - Sort sample names alphabetically for printing and csv
# - Make event count class

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)

class EventCount:
    def __init__(self, event_count_tree="EventCount", analysis_tree="KUAnalysis"):
        self.event_count_tree   = event_count_tree
        self.analysis_tree      = analysis_tree

    # count total events in a ROOT file
    # iterate over entries in the event count tree
    def countTotalEvents(self, root_file):
        result = 0
        chain = ROOT.TChain(self.event_count_tree)
        chain.Add(root_file)
        n_entries = chain.GetEntries()
        for i in range(n_entries):
            chain.GetEntry(i)
            n_events = chain.Nevent
            result += n_events
        return int(result)

    # count saved events in a ROOT file
    # use the number of entries in the analysis tree
    def countSavedEvents(self, root_file):
        chain = ROOT.TChain(self.analysis_tree)
        chain.Add(root_file)
        n_events = chain.GetEntries()
        return int(n_events)

    # process directory containing ROOT files
    def processDir(self, directory, pattern, csv, eos, verbose):
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
        # - if pattern is set, then require file name to contain pattern
        if eos:
            root_files = tools.get_eos_file_list(directory, pattern)
        else:
            if pattern:
                root_files = glob.glob("{0}/*{1}*.root".format(directory, pattern))
            else:
                root_files = glob.glob("{0}/*.root".format(directory))

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
            n_total_events = self.countTotalEvents(root_file)
            n_saved_events = self.countSavedEvents(root_file)
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
    parser.add_argument("--pattern",    "-p", default="",                               help="pattern for root file names (optional)")
    parser.add_argument("--csv",        "-c", default="",                               help="output csv file name (optional)")
    parser.add_argument("--tree",       "-t", default="",                               help="analysis tree name (required for signal)")
    parser.add_argument("--sms",        "-s", default = False,  action = "store_true",  help="run over signal sample (optional)")
    parser.add_argument("--eos",        "-e", default = False,  action = "store_true",  help="run over ROOT files on EOS")
    parser.add_argument("--verbose",    "-v", default = False,  action = "store_true",  help="verbose flag to print more things")

    options     = parser.parse_args()
    directory   = options.directory
    pattern     = options.pattern
    csv         = options.csv
    tree        = options.tree
    sms         = options.sms
    eos         = options.eos
    verbose     = options.verbose

    # check that directory is set
    if not directory:
        print("ERROR: 'directory' is not set. Please provide a directory using the -d option.")
        return
    
    # if we are running over signal, check that an analysis tree is specified
    if sms:
        if not tree:
            print("ERROR: 'tree' is not set, but --sms was used. Please provide an analysis tree name using the -t option.")
            print(" - An analysis tree name is required for signal, and you need to choose a mass point (for example, SMS_1000_500).")
            return

    if sms:
        event_count = EventCount(event_count_tree="EventCount", analysis_tree=tree)
    else:
        event_count = EventCount()

    event_count.processDir(directory, pattern, csv, eos, verbose)

def main():
    run()

if __name__ == "__main__":
    main()
