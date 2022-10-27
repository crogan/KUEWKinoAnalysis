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

# Make 2D plot
def Plot2D(sample_name, selection, g_Xname, g_Yname):
    lumi = 137.0 
    lumi_string = "{0:.1f} fb^{{-1}}".format(lumi)
    
    #print("lumi: {0:.1f}".format(lumi))
    #print("lumi_string: {0}".format(lumi_string))
  
    #plot_dir    = "UL2017_NanoAODv9_CustomPlots_weight_1"
    #hist_dir    = "UL2017_NanoAODv9_Hists_weight_1"
    #plot_dir    = "UL2017_NanoAODv9_CustomPlots_weight_PreUL"
    #hist_dir    = "UL2017_NanoAODv9_Hists_weight_PreUL"
    plot_dir    = "LowPtElectron_UL2017_NanoAODv9_CustomPlots_weight_PreUL"
    hist_dir    = "LowPtElectron_UL2017_NanoAODv9_Hists_weight_PreUL"
    plot_name   = plot_dir + "/" + sample_name + "_" + selection + ".pdf"
    input_name  = hist_dir + "/" + sample_name + "_" + selection + ".root"

    g_PlotTitle     = sample_name
    g_Label         = selection
    z_axis_title    = "N_{events} / " + lumi_string
    
    tools.makeDir(plot_dir)
    # load hist from ROOT file
    input_file  = ROOT.TFile.Open(input_name, "READ")
    hist        = input_file.Get("hist")
  
    # scale hist to lumi
    hist.Scale(lumi)

    # setup canvas
    can = ROOT.TCanvas("can", "can", 700, 600)
    can.SetLeftMargin(0.15)
    can.SetRightMargin(0.18)
    can.SetBottomMargin(0.15)
    can.SetGridx()
    can.SetGridy()
    can.SetLogz()
    can.Draw()
    can.cd()
    # setup hist 
    hist.Draw("COLZ")
    hist.GetXaxis().CenterTitle()
    hist.GetXaxis().SetTitleFont(42)
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetTitleOffset(1.06)
    hist.GetXaxis().SetLabelFont(42)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetNdivisions(5, 5, 0, True)
    hist.GetXaxis().SetTitle(g_Xname)
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
    hist.GetZaxis().SetTitle(z_axis_title)
    hist.GetZaxis().SetRangeUser(0.9 * hist.GetMinimum(0.0), 1.1 * hist.GetMaximum())
    
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

def makePlots2D():
    sample_name = "T4bd"
    #sample_name = "TChiWZ"
    selections  = ["1L_0J", "2L_0J"]    # lepton and Sjet selections
    
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
    
    #hist = ROOT.TH2D("hist", "hist",
    #                 g_NX, g_Xmin, g_Xmax,
    #                 g_NY, g_Ymin, g_Ymax
    #)
    
    for selection in selections:
        Plot2D(sample_name, selection, g_Xname, g_Yname)

def main():
    makePlots2D()

if __name__ == "__main__":
    main()

