# countEvents.py

import os
import glob
import json
import ROOT
import argparse
from colorama import Fore, Back, Style
import tools

#
# Count the number of "total" and "saved" events for all ROOT files in a directory.
# Print the results and save results to a csv file.
#
# Example syntax:
# python python/countEvents.py --directory <path_to_directory> --csv <path_to_csv>
#

# TODO
#
# DONE
# - Update get_eos_file_list() to use a pattern
# - Sort sample names alphabetically for printing and csv
# - Make event count class
# - Add "--sms" option; for signal, count events for a specific mass point
# - Instead of passing an analysis tree for an SMS mass point as an option,
#   we need to store these for each sample in a json file, as the mass points are different for every signal sample.

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)

class EventCount:
    def __init__(self, event_count_tree="EventCount", analysis_tree="KUAnalysis", analysis_tree_file=""):
        self.event_count_tree   = event_count_tree
        self.analysis_tree      = analysis_tree
        self.analysis_tree_file = analysis_tree_file
        self.analysis_tree_map  = {}

        # Load the analysis tree file if it is set
        if self.analysis_tree_file:
            with open(analysis_tree_file, 'r') as f:
                self.analysis_tree_map = json.load(f)

    def GetEventCountTree(self):
        return self.event_count_tree
    
    def SetEventCountTree(self, event_count_tree):
        self.event_count_tree = event_count_tree
    
    def GetAnalysisTree(self):
        return self.analysis_tree
    
    def SetAnalysisTree(self, analysis_tree):
        self.analysis_tree = analysis_tree
    
    def GetMatchingKey(self, base_name):
        result = ""
        # Note: in the list of keys, duplicates are removed
        keys = list(self.analysis_tree_map.keys())
        matches = [key for key in keys if key in base_name]
        n_matches = len(matches)
        #print("keys: {0}".format(keys))
        #print("n_matches: {0}, matches: {1}".format(n_matches, matches))
        if n_matches == 1:
            result = matches[0]
        elif n_matches == 0:
            print(Fore.RED + "ERROR: No matching key found for the base name '{0}'.".format(base_name) + Fore.RESET)
        elif n_matches > 1:
            print(Fore.RED + "ERROR: There are {0} matching keys found for the base name '{1}'.".format(n_matches, base_name) + Fore.RESET)
        return result

    # count total events in a ROOT file
    # iterate over entries in the event count tree
    def countTotalEvents(self, root_file):
        result = 0
        tree = self.GetEventCountTree()
        chain = ROOT.TChain(tree)
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
        tree = self.GetAnalysisTree()
        chain = ROOT.TChain(tree)
        chain.Add(root_file)
        n_events = chain.GetEntries()
        return int(n_events)

    # process directory containing ROOT files
    def processDir(self, directory, pattern, csv, sms, eos, verbose):
        if verbose:
            print(Fore.GREEN + "Counting events." + Fore.RESET)
            print("----------------------------")
            print("directory: {0}".format(directory))
            print("pattern: {0}".format(pattern))
            print("csv: {0}".format(csv))
            print("sms: {0}".format(sms))
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
            print(Fore.GREEN + "Found {0} ROOT files with these base names:".format(n_root_files) + Fore.RESET)

        # headers for csv
        headers = []
        if sms:
            headers = ["sample", "total_events", "analysis_tree", "saved_events"]
        else:
            headers = ["sample", "total_events", "saved_events"]
        output_data.append(headers)

        # count events
        for root_file in root_files:
            base_name = os.path.basename(root_file)
            base_name = base_name.replace(".root", "")
            base_file_names.append(base_name)
            if sms:
                key = self.GetMatchingKey(base_name)
                if key in self.analysis_tree_map:
                    tree = self.analysis_tree_map[key]
                    self.SetAnalysisTree(tree)
                else:
                    # We cannot assign a valid analysis tree
                    tree = "FIXME"
                    self.SetAnalysisTree(tree)
                    print(Fore.RED + "ERROR: The base name '{0}' is not in the analysis tree map (from the json file)!".format(base_name) + Fore.RESET)
                    print(Fore.RED + " - We cannot assign an analysis tree for this signal sample!" + Fore.RESET)
                    print(Fore.RED + " - To fix this error, please add this sample and an analysis tree (choose a valid mass point) to this file: {0}".format(self.analysis_tree_file) + Fore.RESET)
            n_total_events = self.countTotalEvents(root_file)
            n_saved_events = self.countSavedEvents(root_file)
            n_events_map[base_name] = {}
            n_events_map[base_name]["n_total_events"] = n_total_events
            n_events_map[base_name]["n_saved_events"] = n_saved_events
            if sms:
                n_events_map[base_name]["analysis_tree"] = self.analysis_tree
            row = []
            if sms:
                row = [base_name, n_total_events, self.analysis_tree, n_saved_events]
            else:
                row = [base_name, n_total_events, n_saved_events]
            output_data.append(row)
            sum_total_events += n_total_events
            sum_saved_events += n_saved_events
            if verbose:
                print(" - {0}".format(base_name))

        # print results
        if sms:
            print("Sample: total events, analysis tree: saved events")
        else:
            print("Sample: total events, saved events")
        for base_name in base_file_names:
            analysis_tree   = ""
            n_total_events = n_events_map[base_name]["n_total_events"]
            n_saved_events = n_events_map[base_name]["n_saved_events"]
            if sms:
                analysis_tree = n_events_map[base_name]["analysis_tree"]
            if sms:
                print("{0}: {1}, {2}: {3}".format(base_name, n_total_events, analysis_tree, n_saved_events))
            else:
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
    parser.add_argument("--year",       "-y", default="",                               help="year of data taking (optional)")
    parser.add_argument("--sms",        "-s", default = False,  action = "store_true",  help="run over signal sample (optional)")
    parser.add_argument("--eos",        "-e", default = False,  action = "store_true",  help="run over ROOT files on EOS")
    parser.add_argument("--verbose",    "-v", default = False,  action = "store_true",  help="verbose flag to print more things")

    options     = parser.parse_args()
    directory   = options.directory
    pattern     = options.pattern
    csv         = options.csv
    year        = options.year
    sms         = options.sms
    eos         = options.eos
    verbose     = options.verbose

    # valid years of data taking
    valid_years = ["2016", "2017", "2018"]

    # Check that the directory is set.
    if not directory:
        print(Fore.RED + "ERROR: 'directory' is not set. Please provide a directory using the -d option." + Fore.RESET)
        return
    
    # json file for SMS to map sample ROOT files to analysis trees
    analysis_tree_file = ""
    
    if sms:
        # Check that the user entered a valid year.
        if year not in valid_years:
            print(Fore.RED + "ERROR: The year '{0}' is not valid. Please provide a valid year {1} using the -y option.".format(year, valid_years) + Fore.RESET)
            return
        # Assign analysis_tree_file based on the year.
        analysis_tree_file = "json/EventCount/AnalysisTrees_{0}_SMS.json".format(year)
        print(Fore.GREEN + "Using the analysis tree file '{0}'.".format(analysis_tree_file) + Fore.RESET)

    if sms:
        event_count = EventCount(event_count_tree="EventCount", analysis_tree="", analysis_tree_file=analysis_tree_file)
    else:
        event_count = EventCount()

    event_count.processDir(directory, pattern, csv, sms, eos, verbose)

def main():
    run()

if __name__ == "__main__":
    main()
