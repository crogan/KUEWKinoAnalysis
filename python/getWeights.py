# getWeights.py

import ROOT
import csv
import tools

# TODO
# - Add Wjets and Zjets samples

# DONE
# - Get event weights from PreUL samples

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)

def getWeight(root_file, tree_name, is_signal):
    weight  = 0.0
    chain   = ROOT.TChain(tree_name)
    chain.Add(root_file)
    chain.GetEntry(0)
    weight  = chain.weight
    return weight

# create csv with number of events
def createCSV(output_csv, samples, lumi):
    print("Creating {0}".format(output_csv))
    
    column_titles = ["sample", "lumi", "weight_ntuple", "weight_lumi"]
    
    with open(output_csv, 'w') as f:
        csv_writer = csv.writer(f)
        csv_writer.writerow(column_titles)
        for sample in samples:
            path            = samples[sample]["path"] 
            tree_name       = samples[sample]["tree_name"] 
            is_signal       = samples[sample]["is_signal"] 

            weight_ntuple   = getWeight(path, tree_name, is_signal)
            weight_lumi     = lumi * weight_ntuple
            
            #nevents_processed   = getProcessedEvents(path, "EventCount", is_signal)
            #nevents_ntuple      = getNtupleEvents(path, tree_name, is_signal)
            #acceptance          = float(nevents_ntuple) / float(nevents_processed)
            
            row = [sample, lumi, weight_ntuple, weight_lumi]
            csv_writer.writerow(row)
            print(row)

def getWeights():
    # PreUL 2017 samples
    era         = "2017"
    lumi_json   = "json/samples/Lumis.json"
    output_csv  = "csv/samples/PreUL{0}_Weights.csv".format(era)
    ntuple_path = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v0"
    lumi        = tools.getLumi(era, lumi_json)

    if ntuple_path[-1] != "/":
        ntuple_path += "/"

    signal_samples = [
        "T2_4bd_500_490",
    ]
    background_samples = [
        "TTJets_DiLept",
        "ZJetsToNuNu_HT-100To200",
        "ZJetsToNuNu_HT-200To400",
        "ZJetsToNuNu_HT-400To600",
        "ZJetsToNuNu_HT-600To800",
        "ZJetsToNuNu_HT-800To1200",
        "ZJetsToNuNu_HT-1200To2500",
        "ZJetsToNuNu_HT-2500ToInf",
        "WJetsToLNu_HT-70To100",
        "WJetsToLNu_HT-100To200",
        "WJetsToLNu_HT-200To400",
        "WJetsToLNu_HT-400To600",
        "WJetsToLNu_HT-600To800",
        "WJetsToLNu_HT-800To1200",
        "WJetsToLNu_HT-1200To2500",
        "WJetsToLNu_HT-2500ToInf",
    ]

    samples = {}
    for sample in signal_samples:
        samples[sample] = {}
        samples[sample]["is_signal"] = True
        samples[sample]["tree_name"] = "SMS_500_490"
    for sample in background_samples:
        samples[sample] = {}
        samples[sample]["is_signal"] = False
        samples[sample]["tree_name"] = "KUAnalysis"
    
    #samples = {}
    #samples["T2_4bd_500_490"]   = {}
    #samples["TTJets_DiLept"]    = {}
    #samples["T2_4bd_500_490"]["is_signal"]  = True
    #samples["TTJets_DiLept"]["is_signal"]   = False
    #samples["T2_4bd_500_490"]["tree_name"]  = "SMS_500_490"
    #samples["TTJets_DiLept"]["tree_name"]   = "KUAnalysis"

    samples["T2_4bd_500_490"]["path"]               = ntuple_path + "Fall17_102X_SMS/SMS-T2-4bd_genMET-80_mStop-500_mLSP-490_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["TTJets_DiLept"]["path"]                = ntuple_path + "Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["ZJetsToNuNu_HT-100To200"]["path"]      = ntuple_path + "Fall17_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Fall17_102X.root"
    samples["ZJetsToNuNu_HT-200To400"]["path"]      = ntuple_path + "Fall17_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Fall17_102X.root"
    samples["ZJetsToNuNu_HT-400To600"]["path"]      = ntuple_path + "Fall17_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Fall17_102X.root"
    samples["ZJetsToNuNu_HT-600To800"]["path"]      = ntuple_path + "Fall17_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Fall17_102X.root"
    samples["ZJetsToNuNu_HT-800To1200"]["path"]     = ntuple_path + "Fall17_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Fall17_102X.root"
    samples["ZJetsToNuNu_HT-1200To2500"]["path"]    = ntuple_path + "Fall17_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Fall17_102X.root"
    samples["ZJetsToNuNu_HT-2500ToInf"]["path"]     = ntuple_path + "Fall17_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Fall17_102X.root"
    samples["WJetsToLNu_HT-70To100"]["path"]        = ntuple_path + "Fall17_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["WJetsToLNu_HT-100To200"]["path"]       = ntuple_path + "Fall17_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["WJetsToLNu_HT-200To400"]["path"]       = ntuple_path + "Fall17_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["WJetsToLNu_HT-400To600"]["path"]       = ntuple_path + "Fall17_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["WJetsToLNu_HT-600To800"]["path"]       = ntuple_path + "Fall17_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["WJetsToLNu_HT-800To1200"]["path"]      = ntuple_path + "Fall17_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["WJetsToLNu_HT-1200To2500"]["path"]     = ntuple_path + "Fall17_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"
    samples["WJetsToLNu_HT-2500ToInf"]["path"]      = ntuple_path + "Fall17_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root"

    createCSV(output_csv, samples, lumi)

def main():
    getWeights()

if __name__ == "__main__":
    main()

