# eventCounts.py

import ROOT
import csv

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)


def createCSV(output_csv, samples):
    column_titles = ["sample", "nevents_processed", "nevents_ntuple"]
    with open(output_csv, 'w') as f:
        csv_writer = csv.writer(f)
        csv_writer.writerow(column_titles)
        for sample in samples:
            row = [sample, 1, 1]
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
    samples["TTJets_DiLept"]["file"]        = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_NanoAODv9/TTjets_DILEP.root"
    samples["T2_4bd_500_490"]["file"]       = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_NanoAODv9/SMS-T2-4bd_genMET-80_mStop-500_mLSP-490_TuneCP5_13TeV-madgraphMLM-pythia8_UL2017_NanoAODv9_.root"
    createCSV(output_csv, samples)

def main():
    eventCounts()

if __name__ == "__main__":
    main()


