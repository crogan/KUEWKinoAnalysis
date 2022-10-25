#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>
#include <TColorWheel.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TLorentzVector.h>

#include "../include/ReducedBase_slim.hh"
#include "../include/SampleTool.hh"
#include "../include/CategoryTool.hh"
#include "../include/ScaleFactorTool.hh"
#include "../include/Leptonic.hh"
#include "../include/Hadronic.hh"
#include "../include/FitReader.hh"

#include "RestFrames/RestFrames.hh"

using namespace std;

string g_PlotTitle;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;
string g_Yname;
double g_Ymin;
double g_Ymax;
double g_NY;

using namespace RestFrames;

void MakePlot_2D()
{
  gROOT->SetBatch(kTRUE);
  RestFrames::SetStyle();
  
  double lumi = 137.0; 
  // convert lumi to string with specific precision
  stringstream stream;
  stream << fixed << setprecision(1) << lumi;
  string lumi_string = stream.str();
  lumi_string += " fb^{-1}";
  
  // set parameters
  string plot_dir           = "UL2017_NanoAODv9_Plots_weight_1";
  string hist_dir           = "UL2017_NanoAODv9_Hists_weight_1";
  //string plot_dir           = "UL2017_NanoAODv9_Plots_weight_PreUL";
  //string plot_dir           = "LowPtElectron_UL2017_NanoAODv9_Plots_weight_PreUL";
  //string sample_name        = "T4bd";
  string sample_name        = "TChiWZ";
  //string sample_name        = "ttbar";
  //string sample_name        = "ZDY";
  //string sample_name        = "Wjets";
  string selection          = "1L_0J";  // lepton and Sjet selection
  int Nlep_selection        = 1;        // lepton selection
  int NjetS_selection       = 0;        // Sjet selection
  string g_Label            = selection;
  replace(g_Label.begin(), g_Label.end(), '_', ' ');
  string plot_name          = plot_dir + "/" + sample_name + "_" + selection + ".pdf";
  string input_name         = hist_dir + "/" + sample_name + "_" + selection + ".root";
  
  printf("------------------------------\n");
  printf("Parameters:\n");
  printf("lumi = %f\n", lumi);
  printf("lumi_string = %s\n", lumi_string.c_str());
  printf("sample_name = %s\n", sample_name.c_str());
  printf("selection = %s\n", selection.c_str());
  printf("g_Label = %s\n", g_Label.c_str());
  printf("Nlep_selection = %d\n", Nlep_selection);
  printf("NjetS_selection = %d\n", NjetS_selection);
  printf("plot_name = %s\n", plot_name.c_str());
  printf("input_name = %s\n", input_name.c_str());
  printf("------------------------------\n");
  
  // x axis
  g_Xname   = "R_{ISR}";
  g_Xmin    = 0.8;
  g_Xmax    = 1.; 
  g_NX      = 32;
  
  // y axis
  g_Yname   = "M_{#perp} [GeV]";
  g_Ymin    = 0.;
  g_Ymax    = 64.;
  g_NY      = 32;
  
  // get hist from file
  unique_ptr<TFile> file(TFile::Open(input_name.c_str(), "READ"));
  unique_ptr<TH2D>  hist((TH2D*)file->Get("hist"));
 
  //TH2D* hist = new TH2D("hist", "hist",
  //                      g_NX, g_Xmin, g_Xmax,
  //                      g_NY, g_Ymin, g_Ymax
  //);
  
  g_PlotTitle = sample_name;
  
  // ----------------- //
  // --- Make Plot --- //
  // ----------------- //

  double n_events = hist->Integral() * lumi;
  
  printf("Total events (scaled to %s): %f\n", lumi_string.c_str(), n_events);

  string z_axis_title = "N_{events} / " + lumi_string;
  
  hist->Scale(lumi);
  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = (TCanvas*) new TCanvas("can", "can", 700., 600.);

  can->SetLeftMargin(0.15);
  can->SetRightMargin(0.18);
  can->SetBottomMargin(0.15);
  can->SetGridx();
  can->SetGridy();
  can->SetLogz();
  can->Draw();
  can->cd();
  hist->Draw("COLZ");
  hist->GetXaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleSize(0.06);
  hist->GetXaxis()->SetTitleOffset(1.06);
  hist->GetXaxis()->SetLabelFont(42);
  hist->GetXaxis()->SetLabelSize(0.05);
  hist->GetXaxis()->SetNdivisions(5, 5, 0, true);
  hist->GetXaxis()->SetTitle(g_Xname.c_str());
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetYaxis()->SetTitleSize(0.06);
  hist->GetYaxis()->SetTitleOffset(1.12);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelSize(0.05);
  hist->GetYaxis()->SetTitle(g_Yname.c_str());
  hist->GetZaxis()->CenterTitle();
  hist->GetZaxis()->SetTitleFont(42);
  hist->GetZaxis()->SetTitleSize(0.055);
  hist->GetZaxis()->SetTitleOffset(1.05);
  hist->GetZaxis()->SetLabelFont(42);
  hist->GetZaxis()->SetLabelSize(0.05);
  hist->GetZaxis()->SetTitle(z_axis_title.c_str());
  hist->GetZaxis()->SetRangeUser(0.9 * hist->GetMinimum(0.0), 1.1 * hist->GetMaximum());

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  //l.DrawLatex(0.17, 0.855, g_PlotTitle.c_str());
  l.DrawLatex(0.71, 0.943, g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.01, 0.943, "#bf{CMS} Simulation Preliminary");

  l.SetTextSize(0.045);
  l.SetTextFont(42);
  l.DrawLatex(0.7, 0.04, g_Label.c_str());

  can->SaveAs(plot_name.c_str());
  
  delete can;
}
