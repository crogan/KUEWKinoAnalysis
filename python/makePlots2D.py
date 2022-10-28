# makePlots2D.py

import os
import ROOT
import tools

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

# get lumi label
def getLumiLabel(lumi):
    label = "{0:.1f} fb^{{-1}}".format(lumi)
    return label

# Make 2D plot
def Plot2D(hist, sample_name, selection, plot_name, g_Xname, g_Yname, g_Zname, setLog=False, z_limits=[]):
    g_PlotTitle     = sample_name
    g_Label         = selection
    
    # z axis range
    if z_limits:
        # if z_limits is given, use for z axis range
        z_min = z_limits[0]
        z_max = z_limits[1]
    else:
        # if z_limits is not given, autoset z axis range
        z_min = 0.9 * hist.GetMinimum(0.0)
        z_max = 1.1 * hist.GetMaximum()
    
    #print("plot_name = {0}".format(plot_name))
    #print("z_min = {0:.3f}".format(z_min))
    #print("z_max = {0:.3f}".format(z_max))
    
    # setup canvas
    can = ROOT.TCanvas("can", "can", 700, 600)
    can.SetLeftMargin(0.15)
    can.SetRightMargin(0.18)
    can.SetBottomMargin(0.15)
    can.SetGridx()
    can.SetGridy()
    if setLog:
        can.SetLogz()
    can.Draw()
    can.cd()
    
    # draw hist
    hist.Draw("COLZ")
    
    # setup hist 
    hist.GetXaxis().CenterTitle()
    hist.GetXaxis().SetTitleFont(42)
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetTitleOffset(1.06)
    hist.GetXaxis().SetLabelFont(42)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetTitle(g_Xname)
    hist.GetXaxis().SetNdivisions(5, 5, 0, True)
    
    hist.GetYaxis().CenterTitle()
    hist.GetYaxis().SetTitleFont(42)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetTitleOffset(1.12)
    hist.GetYaxis().SetLabelFont(42)
    hist.GetYaxis().SetLabelSize(0.05)
    hist.GetYaxis().SetTitle(g_Yname)
    
    hist.GetZaxis().CenterTitle()
    hist.GetZaxis().SetTitleFont(42)
    hist.GetZaxis().SetTitleSize(0.055)
    hist.GetZaxis().SetTitleOffset(1.05)
    hist.GetZaxis().SetLabelFont(42)
    hist.GetZaxis().SetLabelSize(0.05)
    hist.GetZaxis().SetTitle(g_Zname)
    #hist.GetZaxis().SetRangeUser(0.9 * hist.GetMinimum(0.0), 1.1 * hist.GetMaximum())
    hist.GetZaxis().SetRangeUser(z_min, z_max)
    
    # text
    l = ROOT.TLatex()
    l.SetTextFont(42)
    l.SetNDC()
    
    l.SetTextSize(0.035)
    l.SetTextFont(42)
    # l.DrawLatex(0.17, 0.855, g_PlotTitle)
    l.DrawLatex(0.71, 0.943, g_PlotTitle)
    
    l.SetTextSize(0.04)
    l.SetTextFont(42)
    l.DrawLatex(0.01, 0.943, "#bf{CMS} Simulation Preliminary")
    
    l.SetTextSize(0.045)
    l.SetTextFont(42)
    l.DrawLatex(0.7, 0.04, g_Label)
    
    # save plot
    can.SaveAs(plot_name)
    del can

# Make 2D plots for datasets
def makePlots2D():
    #sample_name = "T4bd"
    #sample_name = "TChiWZ"
    sample_names    = ["T4bd", "AllBkg", "ttbar", "ZDY", "Wjets"]
    selections      = ["1L_0J", "2L_0J"]    # lepton and Sjet selections
    datasets        = {
        "Standard" : {
            "plot_dir" : "UL2017_NanoAODv9_CustomPlots_weight_PreUL",
            "hist_dir" : "UL2017_NanoAODv9_Hists_weight_PreUL"
        },
        "LowPtElectron" : {
            "plot_dir" : "LowPtElectron_UL2017_NanoAODv9_CustomPlots_weight_PreUL",
            "hist_dir" : "LowPtElectron_UL2017_NanoAODv9_Hists_weight_PreUL"
        }
    }
    
    # WARNING: If hist was already scaled to lumi, do not scale again!
    lumi = 137.0 
    lumi_label = getLumiLabel(lumi) 
    z_axis_title = "N_{events} / " + lumi_label
    
    #print("lumi: {0:.1f}".format(lumi))
    #print("lumi_label: {0}".format(lumi_label))
    #print("z_axis_title: {0}".format(z_axis_title))
    
    # x axis
    g_Xname = "R_{ISR}"
    g_Xmin  = 0.8
    g_Xmax  = 1.0 
    g_NX    = 32
    
    # y axis
    g_Yname = "M_{#perp} [GeV]"
    g_Ymin  = 0.0
    g_Ymax  = 64.0
    g_NY    = 32

    # z axis
    setLog  = True
    g_Zname = z_axis_title
    
    # z axis ranges for each sample and selection
    z_limits_map = {
        "T4bd" : {
            "1L_0J" : [0.01, 200.0],
            "2L_0J" : [0.01, 20.0]
        },
        "AllBkg" : {
            "1L_0J" : [0.01, 1.0e4],
            "2L_0J" : [0.01, 1.0e3]
        },
        "ttbar" : {
            "1L_0J" : [0.01, 20.0],
            "2L_0J" : [0.01, 10.0]
        },
        "ZDY" : {
            "1L_0J" : [0.01, 1000.0],
            "2L_0J" : [0.01, 20.0]
        },
        "Wjets" : {
            "1L_0J" : [0.01, 5000.0],
            "2L_0J" : [0.01, 300.0]
        }
    }
    
    #hist = ROOT.TH2D("hist", "hist",
    #                 g_NX, g_Xmin, g_Xmax,
    #                 g_NY, g_Ymin, g_Ymax
    #)

    for dataset in datasets:
        plot_dir = datasets[dataset]["plot_dir"]
        hist_dir = datasets[dataset]["hist_dir"]
        tools.makeDir(plot_dir)
        for sample_name in sample_names:
            for selection in selections:
                plot_name   = plot_dir + "/" + sample_name + "_" + selection + ".pdf"
                input_name  = hist_dir + "/" + sample_name + "_" + selection + ".root"
                
                # load hist from ROOT file
                input_file  = ROOT.TFile.Open(input_name, "READ")
                hist        = input_file.Get("hist")
                
                z_limits    = z_limits_map[sample_name][selection]
                
                Plot2D(hist, sample_name, selection, plot_name, g_Xname, g_Yname, g_Zname, setLog, z_limits)

# Make 2D ratio plots for datasets
def makeRatioPlots2D():
    #sample_name = "T4bd"
    sample_names    = ["T4bd", "AllBkg", "ttbar", "ZDY", "Wjets"]
    selections      = ["1L_0J", "2L_0J"]    # lepton and Sjet selections
    plot_dir        = "UL2017_NanoAODv9_RatioPlots_weight_PreUL"
    hist_dir_1      = "UL2017_NanoAODv9_Hists_weight_PreUL"
    hist_dir_2      = "LowPtElectron_UL2017_NanoAODv9_Hists_weight_PreUL"
        
    tools.makeDir(plot_dir)
    
    g_Xname     = "R_{ISR}"
    g_Yname     = "M_{#perp} [GeV]"
    g_Zname     = "N_{low pt elec} / N_{standard}"
    
    setLog      = False
    
    # z axis ranges for each sample and selection
    z_limits_map = {
        "T4bd" : {
            "1L_0J" : [0.0, 5.0],
            "2L_0J" : [0.0, 10.0]
        },
        "AllBkg" : {
            "1L_0J" : [0.0, 5.0],
            "2L_0J" : [0.0, 5.0]
        },
        "ttbar" : {
            "1L_0J" : [0.0, 5.0],
            "2L_0J" : [0.0, 5.0]
        },
        "ZDY" : {
            "1L_0J" : [0.0, 5.0],
            "2L_0J" : [0.0, 10.0]
        },
        "Wjets" : {
            "1L_0J" : [0.0, 5.0],
            "2L_0J" : [0.0, 5.0]
        }
    }

    for sample_name in sample_names:
        for selection in selections:
            plot_name       = plot_dir   + "/" + sample_name + "_" + selection + ".pdf"
            input_name_1    = hist_dir_1 + "/" + sample_name + "_" + selection + ".root"
            input_name_2    = hist_dir_2 + "/" + sample_name + "_" + selection + ".root"
            
            # load hists from ROOT file
            input_file_1    = ROOT.TFile.Open(input_name_1, "READ")
            input_file_2    = ROOT.TFile.Open(input_name_2, "READ")
            hist_1          = input_file_1.Get("hist")
            hist_2          = input_file_2.Get("hist")
            
            # take ratio of hists
            hist_ratio      = hist_2.Clone("hist_ratio")
            hist_ratio.Divide(hist_1)
            
            z_limits = z_limits_map[sample_name][selection]
            
            Plot2D(hist_ratio, sample_name, selection, plot_name, g_Xname, g_Yname, g_Zname, setLog, z_limits)

def main():
    makePlots2D()
    makeRatioPlots2D()

if __name__ == "__main__":
    main()

