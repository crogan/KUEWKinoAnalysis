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
#ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(11111111)

# Make plot
def Plot(hist, info, plot_name):
    sample  = info["sample"]
    label   = info["label"]
    x_label = info["x_label"]
    y_label = info["y_label"]
    label   = info["label"]
    title   = "{0}_{1}".format(sample, label) 
    title   = title.replace('_', ' ')
    print("Plotting {0}".format(title))
    
    # setup canvas
    can = ROOT.TCanvas("can", "can", 600, 600)
    can.SetLeftMargin(0.25)
    can.SetRightMargin(0.05)
    can.SetBottomMargin(0.15)
    can.SetGridx()
    can.SetGridy()
    can.Draw()
    can.cd()
    
    # draw hist
    hist.Draw("hist error")
    
    # setup hist 
    color       = "coral"
    line_width  = 3
    tools.setupHist(hist, title, x_label, y_label, color, line_width)
    hist.SetMarkerSize(0)

    hist.GetXaxis().CenterTitle()
    hist.GetXaxis().SetTitleFont(42)
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetTitleOffset(1.06)
    hist.GetXaxis().SetLabelFont(42)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetNdivisions(5, 5, 0, True)
    
    hist.GetYaxis().CenterTitle()
    hist.GetYaxis().SetTitleFont(42)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetTitleOffset(2.00)
    hist.GetYaxis().SetLabelFont(42)
    hist.GetYaxis().SetLabelSize(0.05)
    hist.GetYaxis().SetNdivisions(5, 5, 0, True)
    
    # save plot
    can.SaveAs(plot_name)
    del can

# Make plots
def makePlots():
    sample_names    = ["T4bd", "AllBkg", "ttbar", "ZDY", "Wjets"]
    selections      = ["1L_0J", "2L_0J"]    # lepton and Sjet selections
    #lepton_ids      = ["all", "maskBronze"] # lepton ID selections
    lepton_ids      = ["all"]               # lepton ID selections
    #datasets        = {
    #    "Standard" : {
    #        "plot_dir" : "UL2017_NanoAODv9_Plots1D_weight_PreUL_Mperp6",
    #        "hist_dir" : "UL2017_NanoAODv9_Hists_weight_PreUL"
    #    },
    #    "LowPtElectron" : {
    #        "plot_dir" : "LowPtElectron_UL2017_NanoAODv9_Plots1D_weight_PreUL_Mperp6",
    #        "hist_dir" : "LowPtElectron_UL2017_NanoAODv9_Hists_weight_PreUL"
    #    }
    #}
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
    
    info            = {}
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
    
                    info["sample"]  = sample_name
                    info["label"]   = label
    
                    Plot(hist1D, info, plot_name)

# Make ratio plots
def makeRatioPlots():
    sample_names    = ["T4bd", "AllBkg", "ttbar", "ZDY", "Wjets"]
    selections      = ["1L_0J", "2L_0J"]    # lepton and Sjet selections
    #lepton_ids      = ["all", "maskBronze"] # lepton ID selections
    lepton_ids      = ["all"]               # lepton ID selections
    #plot_dir        = "UL2017_NanoAODv9_RatioPlots1D_weight_PreUL_Mperp6"
    plot_dir        = "UL2017_NanoAODv9_RatioPlots1D_weight_PreUL"
    hist_dir_1      = "UL2017_NanoAODv9_Hists_weight_PreUL"
    hist_dir_2      = "LowPtElectron_UL2017_NanoAODv9_Hists_weight_PreUL"
    
    tools.makeDir(plot_dir)
    
    variable = "RISR"
    
    info            = {}
    info["x_label"] = "R_{ISR}"
    info["y_label"] = "N_{low pt elec} / N_{standard}"
    
    for sample_name in sample_names:
        for selection in selections:
            for lepton_id in lepton_ids:
                if lepton_id == "all":
                    plot_name       = plot_dir   + "/" + sample_name + "_" + selection + ".pdf"
                    input_name_1    = hist_dir_1 + "/" + sample_name + "_" + selection + ".root"
                    input_name_2    = hist_dir_2 + "/" + sample_name + "_" + selection + ".root"
                    label           = selection
                else:
                    plot_name       = plot_dir   + "/" + sample_name + "_" + selection + "_" + lepton_id + ".pdf"
                    input_name_1    = hist_dir_1 + "/" + sample_name + "_" + selection + "_" + lepton_id + ".root"
                    input_name_2    = hist_dir_2 + "/" + sample_name + "_" + selection + "_" + lepton_id + ".root"
                    label           = selection + "_" + lepton_id
                
                # load hists from ROOT file
                input_file_1    = ROOT.TFile.Open(input_name_1, "READ")
                input_file_2    = ROOT.TFile.Open(input_name_2, "READ")
                hist2D_1        = input_file_1.Get("hist")
                hist2D_2        = input_file_2.Get("hist")
                hist1D_1        = tools.get1DHist(hist2D_1)
                hist1D_2        = tools.get1DHist(hist2D_2)
                
                # take ratio of hists
                hist1D_ratio    = hist1D_2.Clone("hist1D_ratio")
                hist1D_ratio.Divide(hist1D_1)
                    
                info["sample"]  = sample_name
                info["label"]   = label
    
                Plot(hist1D_ratio, info, plot_name)

# Make double ratio plots
def makeDoubleRatioPlots():
    signal      = "T4bd"
    background  = "AllBkg"
    selections  = ["1L_0J", "2L_0J"]    # lepton and Sjet selections
    #lepton_ids  = ["all", "maskBronze"] # lepton ID selections
    lepton_ids  = ["all"]               # lepton ID selections
    plot_dir    = "UL2017_NanoAODv9_DoubleRatioPlots1D_weight_PreUL"
    hist_dir_1  = "UL2017_NanoAODv9_Hists_weight_PreUL"
    hist_dir_2  = "LowPtElectron_UL2017_NanoAODv9_Hists_weight_PreUL"
    
    tools.makeDir(plot_dir)
    
    rebin       = False
    variable    = "RISR"
    
    # Use S / B
    sqrtBack    = False
    sample_name = "SigOverBack"
    y_label     = "(S/B)_{2} / (S/B)_{1}"
    
    # Use S / sqrt(B)
    #sqrtBack    = True
    #sample_name = "SigOverSqrtBack"
    #y_label     = "(S/#sqrt{B})_{2} / (S/#sqrt{B})_{1}"
    
    info            = {}
    info["x_label"] = "R_{ISR}"
    info["y_label"] = y_label
    info["sample"]  = sample_name
    
    for selection in selections:
        for lepton_id in lepton_ids:
            if lepton_id == "all":
                plot_name           = plot_dir   + "/" + sample_name + "_" + selection + ".pdf"
                signal_name_1       = hist_dir_1 + "/" + signal      + "_" + selection + ".root"
                signal_name_2       = hist_dir_2 + "/" + signal      + "_" + selection + ".root"
                background_name_1   = hist_dir_1 + "/" + background  + "_" + selection + ".root"
                background_name_2   = hist_dir_2 + "/" + background  + "_" + selection + ".root"
                label               = selection
            else:
                plot_name           = plot_dir   + "/" + sample_name + "_" + selection + "_" + lepton_id + ".pdf"
                signal_name_1       = hist_dir_1 + "/" + signal      + "_" + selection + "_" + lepton_id + ".root"
                signal_name_2       = hist_dir_2 + "/" + signal      + "_" + selection + "_" + lepton_id + ".root"
                background_name_1   = hist_dir_1 + "/" + background  + "_" + selection + "_" + lepton_id + ".root"
                background_name_2   = hist_dir_2 + "/" + background  + "_" + selection + "_" + lepton_id + ".root"
                label               = selection + "_" + lepton_id
            
            # load hists from ROOT file
            signal_file_1       = ROOT.TFile.Open(signal_name_1, "READ")
            signal_file_2       = ROOT.TFile.Open(signal_name_2, "READ")
            background_file_1   = ROOT.TFile.Open(background_name_1, "READ")
            background_file_2   = ROOT.TFile.Open(background_name_2, "READ")
            signal_hist2D_1     = signal_file_1.Get("hist")
            signal_hist2D_2     = signal_file_2.Get("hist")
            background_hist2D_1 = background_file_1.Get("hist")
            background_hist2D_2 = background_file_2.Get("hist")
            signal_hist1D_1     = tools.get1DHist(signal_hist2D_1)
            signal_hist1D_2     = tools.get1DHist(signal_hist2D_2)
            background_hist1D_1 = tools.get1DHist(background_hist2D_1)
            background_hist1D_2 = tools.get1DHist(background_hist2D_2)
            
            # rebin 1D hists
            if rebin:
                signal_hist1D_1.Rebin(2)
                signal_hist1D_2.Rebin(2)
                background_hist1D_1.Rebin(2)
                background_hist1D_2.Rebin(2)
            
            # take square root of background
            if sqrtBack:
                tools.sqrtHist1D(background_hist1D_1)
                tools.sqrtHist1D(background_hist1D_2)
            
            # take ratio of hists
            signal_hist1D_ratio       = signal_hist1D_2.Clone("signal_hist1D_ratio")
            signal_hist1D_ratio.Divide(signal_hist1D_1)
            
            background_hist1D_ratio   = background_hist1D_2.Clone("background_hist1D_ratio")
            background_hist1D_ratio.Divide(background_hist1D_1)
            
            hist1D_double_ratio       = signal_hist1D_ratio.Clone("hist1D_double_ratio")
            hist1D_double_ratio.Divide(background_hist1D_ratio)
            
            info["label"] = label
            
            Plot(hist1D_double_ratio, info, plot_name)

def main():
    makePlots()
    makeRatioPlots()
    makeDoubleRatioPlots()

if __name__ == "__main__":
    main()


