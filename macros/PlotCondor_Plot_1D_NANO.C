#include <iostream>
#include <string>
#include <fstream>

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
#include <TLine.h>
#include <TLorentzVector.h>

#include "../include/SUSYNANOBase_slim.hh"
#include "../include/SampleTool.hh"
#include "../include/CategoryTool.hh"
#include "../include/ScaleFactorTool.hh"
#include "../include/Leptonic.hh"
#include "../include/Hadronic.hh"
#include "../include/FitReader.hh"
#include "../include/XsecTool.hh"

#include "RestFrames/RestFrames.hh"

using namespace std;


string g_PlotTitle;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;

using namespace RestFrames;

std::string get_str_between_two_str(const std::string &s, const std::string &start_delim, const std::string &stop_delim)
{
 unsigned first_delim_pos = s.find(start_delim);
 unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
 unsigned last_delim_pos = s.find_first_of(stop_delim, end_pos_of_first_delim);
 return s.substr(end_pos_of_first_delim,last_delim_pos - end_pos_of_first_delim);
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void load_files_from_list(vector<string>& files, string fileListName)
{
 std::ifstream infile(fileListName);
 string line = "";
 while(getline(infile,line))
 {
  files.push_back(line);
 }
}

void PlotCondor_Plot_1D_NANO(){

  
  RestFrames::SetStyle();

  string g_Label = "";
  g_Xname = "HT  [GeV]";

  g_Xmin = 0.;
  g_Xmax = 3000.; 
  g_NX = 128;

   g_PlotTitle = "";

   bool Scale = false;

   std::map<string,double> file_list_2016 = {
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.},
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.2},
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.2},
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.22},
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 4.5},
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.67},
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.37},
    {"output_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.7},
   };

   std::map<string,double> file_list_2017 = {
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8", 1.2},
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8", 2.},
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8", 1.3},
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8", 16.35},
    {"output_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8", 2.75},
   };

   std::map<string,double> file_list_2018 = {
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8", 5.},
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8", 0.85},
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8", 1.5},
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8", 1.27},
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8", 1.83},
   {"output_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8", 50.23},
   };

  vector<std::map<string,double>> file_lists = {
   file_list_2016,
   file_list_2017,
   file_list_2018,
  };
  string path_to_lists = "/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";

  for(int l = 0; l < int(file_lists.size()); l++)
  {
   TH1D* hist_tot = new TH1D((get_str_between_two_str(file_lists[l].begin()->first,"NANO_","_W")).c_str(), (get_str_between_two_str(file_lists[l].begin()->first,"NANO_","_W")).c_str(), g_NX, g_Xmin, g_Xmax);
    for(std::map<string,double>::iterator iter = file_lists[l].begin(); iter != file_lists[l].end(); ++iter)
    {
     string inputRootFileName = iter->first;
     double SF = iter->second;
     std::ifstream infile(path_to_lists+get_str_between_two_str(inputRootFileName,"NANO_","_W")+"_102X/"+inputRootFileName.substr(inputRootFileName.find("_W"+1))+".txt");
     string line = "";
     int counter = 0;
     while(getline(infile,line))
     {
      TFile* file = new TFile((inputRootFileName+"_"+std::to_string(counter)+".root").c_str());
      string hist_name = inputRootFileName+"_"+std::to_string(counter);
      replace(hist_name,"output_Plot_1D_NANO","hist");
      TH1D* hist = (TH1D*)file->Get(hist_name.c_str());
      if(Scale)
        hist->Scale(SF);
      hist_tot->Add(hist);
      file->Close();
      delete file;    
     }
   }
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(11111111);
   
   TFile* output_file = new TFile(("output_"+get_str_between_two_str(file_lists[l].begin()->first,"NANO_","_W")+".root").c_str(),"RECREATE");

   TCanvas* can = (TCanvas*) new TCanvas(("can_"+get_str_between_two_str(file_lists[l].begin()->first,"NANO_","_W")).c_str(),("can_"+get_str_between_two_str(file_lists[l].begin()->first,"NANO_","_W")).c_str(),700.,600);
   can->SetLeftMargin(0.15);
   can->SetRightMargin(0.18);
   can->SetBottomMargin(0.15);
   can->SetGridx();
   can->SetGridy();
   can->SetLogy();
   can->Draw();
   can->cd();
   hist_tot->Draw("");
   hist_tot->GetXaxis()->CenterTitle();
   hist_tot->GetXaxis()->SetTitleFont(42);
   hist_tot->GetXaxis()->SetTitleSize(0.06);
   hist_tot->GetXaxis()->SetTitleOffset(1.06);
   hist_tot->GetXaxis()->SetLabelFont(42);
   hist_tot->GetXaxis()->SetLabelSize(0.05);
   hist_tot->GetXaxis()->SetTitle(g_Xname.c_str());
   hist_tot->GetYaxis()->CenterTitle();
   hist_tot->GetYaxis()->SetTitleFont(42);
   hist_tot->GetYaxis()->SetTitleSize(0.06);
   hist_tot->GetYaxis()->SetTitleOffset(1.12);
   hist_tot->GetYaxis()->SetLabelFont(42);
   hist_tot->GetYaxis()->SetLabelSize(0.05);
   //hist_tot->GetYaxis()->SetTitle("N_{events} / 137 fb^{-1}");
   //hist_tot->GetYaxis()->SetRangeUser(0.9*hist_tot->GetMinimum(0.0),1.1*hist_tot->GetMaximum());
   //hist_tot->GetYaxis()->SetRangeUser(1.e-10,1.e3);
   
   TLatex la;
   la.SetTextFont(42);
   la.SetNDC();
   la.SetTextSize(0.035);
   la.SetTextFont(42);
   // la.DrawLatex(0.17,0.855,g_PlotTitle.c_str());
   la.DrawLatex(0.71,0.943,g_PlotTitle.c_str());
   la.SetTextSize(0.04);
   la.SetTextFont(42);
   la.DrawLatex(0.01,0.943,"#bf{CMS} Simulation Preliminary");
   
   la.SetTextSize(0.045);
   la.SetTextFont(42);
   la.DrawLatex(0.7,0.04,g_Label.c_str());

   // Draw Lines At HT Slices
   double min = hist_tot->GetMinimum()*100.;
   double max = hist_tot->GetMaximum();
   TLine line_100(100.,min,100.,max);
   line_100.SetLineStyle(4);
   line_100.SetLineColor(kBlack);
   line_100.SetLineWidth(2);
   line_100.Draw("SAMES");
   TLine line_200(200.,min,200.,max);
   line_200.SetLineStyle(4);
   line_200.SetLineColor(kBlack);
   line_200.SetLineWidth(2);
   line_200.Draw("SAMES");
   TLine line_400(400.,min,400.,max);
   line_400.SetLineStyle(4);
   line_400.SetLineColor(kBlack);
   line_400.SetLineWidth(2);
   line_400.Draw("SAMES");
   TLine line_600(600.,min,600.,max);
   line_600.SetLineStyle(4);
   line_600.SetLineColor(kBlack);
   line_600.SetLineWidth(2);
   line_600.Draw("SAMES");
   TLine line_800(800.,min,800.,max);
   line_800.SetLineStyle(4);
   line_800.SetLineColor(kBlack);
   line_800.SetLineWidth(2);
   line_800.Draw("SAMES");
   TLine line_1200(1200.,min,1200.,max);
   line_1200.SetLineStyle(4);
   line_1200.SetLineColor(kBlack);
   line_1200.SetLineWidth(2);
   line_1200.Draw("SAMES");
   TLine line_2500(2500.,min,2500.,max);
   line_2500.SetLineStyle(4);
   line_2500.SetLineColor(kBlack);
   line_2500.SetLineWidth(2);
   line_2500.Draw("SAMES");
   
   can->SaveAs((get_str_between_two_str(file_lists[l].begin()->first,"NANO_","_W")+".pdf").c_str());
   can->Write();
   hist_tot->Write();
   output_file->Close(); 
   //delete hist;
   //delete output_file;
  }

}
