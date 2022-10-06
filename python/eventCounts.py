# eventCounts.py

import ROOT
import csv

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)

# number of processed events (run over when creating ntuples, before cuts)
def getProcessedEvents(root_file, tree_name, is_signal):
    nevents  = 0
    chain    = ROOT.TChain(tree_name)
    chain.Add(root_file)
    nentries = chain.GetEntries()
    nentries = int(nentries)
    for i in range(nentries):
        # select entry
        chain.GetEntry(i)
        # get branch
        Nevent = chain.Nevent
        Nevent = int(Nevent)
        # add to total
        nevents += Nevent
        #print(i, Nevent)
    nevents = int(nevents)
    return nevents

# number of ntuple events (saved in ntuples after cuts)
def getNtupleEvents(root_file, tree_name, is_signal):
    nevents = 0
    chain   = ROOT.TChain(tree_name)
    chain.Add(root_file)
    nevents = chain.GetEntries()
    nevents = int(nevents)
    return nevents

def createCSV(output_csv, samples):
    column_titles = ["sample", "nevents_processed", "nevents_ntuple"]
    with open(output_csv, 'w') as f:
        csv_writer = csv.writer(f)
        csv_writer.writerow(column_titles)
        for sample in samples:
            path            = samples[sample]["path"] 
            tree_name       = samples[sample]["tree_name"] 
            is_signal       = samples[sample]["is_signal"] 
            
            nevents_processed   = getProcessedEvents(path, "EventCount", is_signal)
            nevents_ntuple      = getNtupleEvents(path, tree_name, is_signal)
            
            row = [sample, nevents_processed, nevents_ntuple]
            csv_writer.writerow(row)
            print(row)

def eventCounts():
    # NANO AOD v9 samples
    # 2017
    era         = "2017"
    output_csv  = "csv/samples/nEvents_UL{0}_NanoAODv9.csv".format(era)
    samples = {}
    samples["TTJets_DiLept"]    = {}
    samples["T2_4bd_500_490"]   = {}
    samples["TTJets_DiLept"]["is_signal"]   = False
    samples["T2_4bd_500_490"]["is_signal"]  = True
    samples["TTJets_DiLept"]["tree_name"]   = "KUAnalysis"
    samples["T2_4bd_500_490"]["tree_name"]  = "SMS_500_490"
    samples["TTJets_DiLept"]["path"]        = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_NanoAODv9/TTjets_DILEP.root"
    samples["T2_4bd_500_490"]["path"]       = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_NanoAODv9/SMS-T2-4bd_genMET-80_mStop-500_mLSP-490_TuneCP5_13TeV-madgraphMLM-pythia8_UL2017_NanoAODv9_.root"
    createCSV(output_csv, samples)

def main():
    eventCounts()

if __name__ == "__main__":
    main()


