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

# get tree from open file
# WARNING: Do not open TFile in getTree(); if you do, the returned TTree object will be destroyed when the TFile closes.
#          Pass open TFile to getTree().
def getTree(open_file, tree_name):
    tree     = open_file.Get(tree_name)
    n_events = tree.GetEntries()
    print("tree: {0}, number of events: {1}".format(tree_name, n_events))
    return tree

def plot(plot_dir, plot_name, tree, variable, cuts = "", setLogY=True):
    output_name = "{0}/{1}".format(plot_dir, plot_name)
    
    c = ROOT.TCanvas("c", "c", 800, 800)
    
    # set log y
    if setLogY:
        c.SetLogy(1)
    
    tree.Draw(variable, cuts)
    
    c.Update()
    c.SaveAs(output_name + ".pdf")
    c.SaveAs(output_name + ".png")

def makePlots():
    print("Go make plots!")
    
    tree_name  = "Events"
    
    # file for ntuples (from Erich's NANO AOD script)
    #input_file = "root://xrootd.unl.edu//store/mc/RunIIFall17MiniAODv2/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/70000/D0890E60-8086-E811-93BF-0025904B7C26.root"
    
    # file from samples/NANO/Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8.txt: 
    #input_file = "root://cmsxrootd.fnal.gov//store/mc/RunIIFall17NanoAODv7/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/270000/FC6D9F02-3BCC-454B-A535-9FE6B2783A49.root"
    #plot_dir   = "plots_SV_Standard_TTJets_DiLept"

    # custom file created on UNL
    input_file = "samples/custom/myNanoProdMc2017_NANO_v2.root"
    plot_dir   = "plots_SV_Custom_TTJets_DiLept"

    # SV variables:
    # ------------
    # SV_dlen    : Float_t decay length in cm                             *
    # SV_dlenSig : Float_t decay length significance                      *
    # SV_dxy     : Float_t 2D decay length in cm                          *
    # SV_dxySig  : Float_t 2D decay length significance                   *
    # SV_pAngle  : Float_t pointing angle, i.e. acos(p_SV * (SV - PV))    *
    # SV_chi2    : Float_t reduced chi2, i.e. chi/ndof                    *
    # SV_eta     : Float_t eta                                            *
    # SV_mass    : Float_t mass                                           *
    # SV_ndof    : Float_t number of degrees of freedom                   *
    # SV_phi     : Float_t phi                                            *
    # SV_pt      : Float_t pt                                             *
    # SV_x       : Float_t secondary vertex X position, in cm             *
    # SV_y       : Float_t secondary vertex Y position, in cm             *
    # SV_z       : Float_t secondary vertex Z position, in cm             *
    # ------------
    
    # standard variables
    variables = [
        "SV_dlen",
        "SV_dlenSig",
        "SV_dxy",
        "SV_dxySig",
        "SV_pAngle",
        "SV_chi2",
        "SV_eta",
        "SV_mass",
        "SV_ndof",
        "SV_phi",
        "SV_pt",
        "SV_x",
        "SV_y",
        "SV_z",
    ]
    # added variables (in custom ntuples)
    variables.append("SV_flavor")
    variables.append("SV_ntrk")
    
    # WARNING: Make sure to open file here, not within getTree() so that TFile stays open. 
    #          If TFile closes, then TTree object is destroyed.
    open_file   = ROOT.TFile.Open(input_file)
    tree        = getTree(open_file, tree_name)
    
    makeDir(plot_dir)
    
    for variable in variables:
        plot_name = variable
        plot(plot_dir, plot_name, tree, variable)

def main():
    makePlots()

if __name__ == "__main__":
    main()

