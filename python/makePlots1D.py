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

# Plot one histogram
def Plot(hist, info, plot_name):
    sample  = info["sample"]
    label   = info["label"]
    x_label = info["x_label"]
    y_label = info["y_label"]
    x_min   = info["x_min"]
    x_max   = info["x_max"]
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
    
    # setup hist 
    color       = "coral"
    line_width  = 3
    tools.setupHist(hist, title, x_label, y_label, x_min, x_max, color, line_width)
    
    # draw hist
    hist.Draw("hist error")
    
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
    can.Clear()
    can.Close()

# Plot multiple histograms
def PlotMultiple(hist_info, plot_info, plot_name):
    title   = plot_info["title"]
    x_label = plot_info["x_label"]
    y_label = plot_info["y_label"]
    x_min   = plot_info["x_min"]
    x_max   = plot_info["x_max"]

    # setup canvas
    can = ROOT.TCanvas("can", "can", 600, 600)
    can.SetLeftMargin(0.25)
    can.SetRightMargin(0.05)
    can.SetBottomMargin(0.15)
    can.SetGridx()
    can.SetGridy()
    can.Draw()
    can.cd()

    for key in hist_info:
        hist    = hist_info[key]["hist"]
        label   = hist_info[key]["label"]
        color   = hist_info[key]["color"]

        # setup hist 
        line_width  = 3
        tools.setupHist(hist, title, x_label, y_label, x_min, x_max, color, line_width)
        
        # legend

        # draw hist
        hist.Draw("same hist error")
    
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
        
    # legend
    
    # save plot
    can.SaveAs(plot_name)
    can.Clear()
    can.Close()

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
    info["x_min"]   = 0.85
    info["x_max"]   = 1.00
    
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
                    
                    # set info
                    info["sample"]  = sample_name
                    info["label"]   = label
                    
                    # load hist from ROOT file
                    input_file  = ROOT.TFile.Open(input_name, "READ")
                    hist2D      = input_file.Get("hist")
                    hist1D      = tools.get1DHist(hist2D)
    
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
    
    # parameters
    # original 2D histograms:
    # x: R_ISR;     [0.8, 1.0],     32 bins
    # y: M_perp;    [0.0, 64.0],    32 bins
    # for variable binning, the new bin edges should match the original histogram
    constant_rebin  = False
    variable_rebin  = True
    rebin_num       = 8
    #rebin_xbins     = np.array([0.80, 0.85, 0.90, 0.95, 1.00])
    rebin_xbins     = np.array([0.8000, 0.8500, 0.9000, 0.9250, 0.9500, 0.9625, 0.9750, 0.9875, 1.0000])
    nbins           = len(rebin_xbins) - 1
    variable        = "RISR"
    
    info            = {}
    info["x_label"] = "R_{ISR}"
    info["y_label"] = "N_{with low p_{T} e^{\pm}} / N_{without low p_{T} e^{\pm}}"
    info["x_min"]   = 0.85
    info["x_max"]   = 1.00
    
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
                
                # set info
                info["sample"]  = sample_name
                info["label"]   = label

                # load hists from ROOT file
                input_file_1    = ROOT.TFile.Open(input_name_1, "READ")
                input_file_2    = ROOT.TFile.Open(input_name_2, "READ")
                hist2D_1        = input_file_1.Get("hist")
                hist2D_2        = input_file_2.Get("hist")
                hist1D_1        = tools.get1DHist(hist2D_1)
                hist1D_2        = tools.get1DHist(hist2D_2)
            
                # variable bin size rebin
                if variable_rebin:
                    hist1D_rebin_1  = hist1D_1.Rebin(nbins, "hist1D_rebin_1", rebin_xbins)
                    hist1D_rebin_2  = hist1D_2.Rebin(nbins, "hist1D_rebin_2", rebin_xbins)
                    
                    plotRatio(hist1D_rebin_1, hist1D_rebin_2, info, plot_name)
                else:
                    # constant bin size rebin 
                    if constant_rebin:
                        hist1D_1.Rebin(rebin_num)
                        hist1D_2.Rebin(rebin_num)
                    
                    plotRatio(hist1D_1, hist1D_2, info, plot_name)


# Plot ratio using input histograms
# ratio = hist_2 / hist_1
def plotRatio(hist_1, hist_2, info, plot_name):
    # take ratio of hists
    ratio = hist_2.Clone("ratio")
    ratio.Divide(hist_1)
    
    Plot(ratio, info, plot_name)

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
    
    # parameters
    # original 2D histograms:
    # x: R_ISR;     [0.8, 1.0],     32 bins
    # y: M_perp;    [0.0, 64.0],    32 bins
    # for variable binning, the new bin edges should match the original histogram
    constant_rebin  = False
    variable_rebin  = True
    rebin_num       = 8
    #rebin_xbins     = np.array([0.80, 0.85, 0.90, 0.95, 1.00])
    rebin_xbins     = np.array([0.8000, 0.8500, 0.9000, 0.9250, 0.9500, 0.9625, 0.9750, 0.9875, 1.0000])
    nbins           = len(rebin_xbins) - 1
    variable        = "RISR"
    sqrtBack        = False
    
    if sqrtBack:
        # Use S / sqrt(B)
        sample_name = "SigOverSqrtBack"
        y_label     = "(S/#sqrt{B})_{with low p_{T} e^{\pm}} / (S/#sqrt{B})_{without low p_{T} e^{\pm}}"
    else:
        # Use S / B
        sample_name = "SigOverBack"
        y_label     = "(S/B)_{with low p_{T} e^{\pm}} / (S/B)_{without low p_{T} e^{\pm}}"
    
    info            = {}
    info["title"]   = sample_name
    info["x_label"] = "R_{ISR}"
    info["y_label"] = y_label
    info["sample"]  = sample_name
    info["x_min"]   = 0.85
    info["x_max"]   = 1.00

    hist_info = {}
    
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
        
            hist_info[label] = {}
            hist_info[label]["label"] = label
            hist_info[label]["color"] = "red"
            
            # set info
            info["label"] = label
            
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
            
            # variable bin size rebin
            if variable_rebin:
                signal_hist1D_rebin_1       = signal_hist1D_1.Rebin(nbins,      "signal_hist1D_rebin_1",        rebin_xbins)
                signal_hist1D_rebin_2       = signal_hist1D_2.Rebin(nbins,      "signal_hist1D_rebin_2",        rebin_xbins)
                background_hist1D_rebin_1   = background_hist1D_1.Rebin(nbins,  "background_hist1D_rebin_1",    rebin_xbins)
                background_hist1D_rebin_2   = background_hist1D_2.Rebin(nbins,  "background_hist1D_rebin_2",    rebin_xbins)
                
                # take square root of background
                if sqrtBack:
                    tools.sqrtHist1D(background_hist1D_rebin_1)
                    tools.sqrtHist1D(background_hist1D_rebin_2)
                
                plotDoubleRatio(signal_hist1D_rebin_1, signal_hist1D_rebin_2, background_hist1D_rebin_1, background_hist1D_rebin_2, hist_info, info, plot_name)
           
            else:
                # constant bin size rebin 
                if constant_rebin:
                    signal_hist1D_1.Rebin(rebin_num)
                    signal_hist1D_2.Rebin(rebin_num)
                    background_hist1D_1.Rebin(rebin_num)
                    background_hist1D_2.Rebin(rebin_num)
                
                # take square root of background
                if sqrtBack:
                    tools.sqrtHist1D(background_hist1D_1)
                    tools.sqrtHist1D(background_hist1D_2)

                plotDoubleRatio(signal_hist1D_1, signal_hist1D_2, background_hist1D_1, background_hist1D_2, hist_info, info, plot_name)

    plot_multi_name = plot_dir + "/" + sample_name + "_combined.pdf"
    PlotMultiple(hist_info, info, plot_multi_name)

# plot double ratio using input histograms
# double ratio = (s2 / s1) / (b2 / b1)
def plotDoubleRatio(hist_s1, hist_s2, hist_b1, hist_b2, hist_info, info, plot_name):
    signal_ratio = hist_s2.Clone("signal_ratio")
    signal_ratio.Divide(hist_s1)
    
    background_ratio = hist_b2.Clone("background_ratio")
    background_ratio.Divide(hist_b1)
    
    double_ratio = signal_ratio.Clone("double_ratio")
    double_ratio.Divide(background_ratio)

    label = info["label"]
    hist_info[label]["hist"] = double_ratio

    Plot(double_ratio, info, plot_name)

# main
def main():
    makePlots()
    makeRatioPlots()
    makeDoubleRatioPlots()

if __name__ == "__main__":
    main()

