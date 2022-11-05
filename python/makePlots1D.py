# makePlots1D.py

import os
import ROOT
import tools
import numpy as np

# Make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
ROOT.PyConfig.IgnoreCommandLineOptions = True
# Make plots faster without displaying them
ROOT.gROOT.SetBatch(ROOT.kTRUE)
# Tell ROOT not to be in charge of memory, fix issue of histograms being deleted when ROOT file is closed:
ROOT.TH1.AddDirectory(False)
# ROOT Style
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(11111111)

# Make plot
def Plot(hist, info, plot_name):
    title   = info["title"]
    x_label = info["x_label"]
    y_label = info["y_label"]
    label   = info["label"]
    print("Plotting {0}: {1}".format(title, label))
    
    # setup canvas
    can = ROOT.TCanvas("can", "can", 600, 600)
    can.SetLeftMargin(0.15)
    can.SetRightMargin(0.18)
    can.SetBottomMargin(0.15)
    can.SetGridx()
    can.SetGridy()
    can.Draw()
    can.cd()
    
    # draw hist
    hist.Draw("")
    
    # save plot
    can.SaveAs(plot_name)
    del can

# Make plots
def makePlots():
    print("Howdy!")
    sample_names    = ["T4bd", "AllBkg", "ttbar", "ZDY", "Wjets"]
    selections      = ["1L_0J", "2L_0J"]    # lepton and Sjet selections
    lepton_ids      = ["all", "maskBronze"] # lepton ID selections
    datasets        = {
        "Standard" : {
            "plot_dir" : "UL2017_NanoAODv9_Plots1D_weight_PreUL",
            "hist_dir" : "UL2017_NanoAODv9_Hists_weight_PreUL"
        },
        "LowPtElectron" : {
            "plot_dir" : "LowPtElectron_UL2017_NanoAODv9_Plots1D_weight_PreUL",
            "hist_dir" : "LowPtElectron_UL2017_NanoAODv9_Hists_weight_PreUL"
        }
    }
    
    variable = "RISR"
    
    info = {}
    info["x_label"] = "R_{ISR}"
    info["y_label"] = "Events"
    
    for dataset in datasets:
        plot_dir = datasets[dataset]["plot_dir"]
        hist_dir = datasets[dataset]["hist_dir"]
        tools.makeDir(plot_dir)
        for sample_name in sample_names:
            for selection in selections:
                for lepton_id in lepton_ids:
                    if lepton_id == "all":
                        plot_name   = plot_dir + "/" + sample_name + "_" + variable + "_" + selection + ".pdf"
                        input_name  = hist_dir + "/" + sample_name + "_" + selection + ".root"
                        label       = selection
                    else:
                        plot_name   = plot_dir + "/" + sample_name + "_" + variable + "_" + selection + "_" + lepton_id + ".pdf"
                        input_name  = hist_dir + "/" + sample_name + "_" + selection + "_" + lepton_id + ".root"
                        label       = selection + "_" + lepton_id
                    
                    # load hist from ROOT file
                    input_file  = ROOT.TFile.Open(input_name, "READ")
                    hist2D      = input_file.Get("hist")
                    hist1D      = tools.get1DHist(hist2D)
    
                    info["title"] = sample_name
                    info["label"] = label
    
                    Plot(hist1D, info, plot_name)

def main():
    makePlots()

if __name__ == "__main__":
    main()


