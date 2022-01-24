# makePlots.py

import os
import ROOT

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)

# creates directory if it does not exist
def makeDir(dir_name):
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

# get chain from list of ROOT files
def getChain(input_files, num_files):
    # use num_files as max if it is not negative
    if num_files >= 0:
        input_files = input_files[0:num_files]
    chain = ROOT.TChain('pixelTree')
    for f in input_files:
        chain.Add(f)
    return 

# ERROR: returned TTree object is broken
def getTree(open_file, tree_name):
    tree     = open_file.Get(tree_name)
    n_events = tree.GetEntries()
    print("tree (1): {0}".format(tree))
    print("number of events: {0}".format(tree.GetEntries()))
    #print("tree: {0}, number of events: {1}".format(tree_name, n_events))
    #tree.Draw("SV_pt")
    return tree

def plot(plot_dir, plot_name, tree, variable, cuts = ""):
    output_name = "{0}/{1}".format(plot_dir, plot_name)
    print("tree (3): {0}".format(tree))
    print("number of events: {0}".format(tree.GetEntries()))
    
    #print(tree.GetEntries())
    #tree.Scan(variable)
    #c = ROOT.TCanvas("c", "c", 800, 800)
    #tree.Draw(variable, cuts)
    #tree.Draw(variable)
    #c.Update()
    #c.SaveAs(output_name + ".pdf")
    #c.SaveAs(output_name + ".png")

def makePlots():
    print("Go make plots!")
    
    # file for ntuples (from Erich's NANO AOD script)
    #input_file = "root://xrootd.unl.edu//store/mc/RunIIFall17MiniAODv2/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/70000/D0890E60-8086-E811-93BF-0025904B7C26.root"
    
    # file from samples/NANO/Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8.txt: 
    input_file = "root://cmsxrootd.fnal.gov//store/mc/RunIIFall17NanoAODv7/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/270000/FC6D9F02-3BCC-454B-A535-9FE6B2783A49.root"
    
    tree_name  = "Events"
    plot_dir   = "plots_sv"
    plot_name  = "SV_pt"
    variable   = "SV_pt"
    
    tFile = ROOT.TFile.Open(input_file)
    tree  = getTree(tFile, tree_name)
    print("tree (2): {0}".format(tree))
    print("number of events: {0}".format(tree.GetEntries()))
    
    makeDir(plot_dir)
    
    plot(plot_dir, plot_name, tree, variable)

def main():
    makePlots()

if __name__ == "__main__":
    main()

