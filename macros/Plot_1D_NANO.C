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

void load_files_from_list(vector<string>& files, string fileListName)
{
 std::ifstream infile(fileListName);
 string line = "";
 while(getline(infile,line))
 {
  files.push_back(line);
 }
}

double getWeightPerEvent(string input_dataset, string input_filetag, string EventCountFile)
{
  TFile* fout = new TFile(EventCountFile.c_str(),"READ");
  TBranch* b_dataset = nullptr;
  TBranch* b_filetag = nullptr;
  TBranch* b_Nweight = nullptr;
  TBranch* b_Nevent = nullptr;
  TBranch* b_MP = nullptr;
  TBranch* b_MC = nullptr;
  string* dataset = nullptr;
  string* filetag = nullptr;
  double Nweight = 0.0;
  double Nevent = 0.0;
  int MP = 0;
  int MC = 0;
  TTree* tree = nullptr;
  tree = (TTree*) fout->Get("EventCount");
  
  tree->SetMakeClass(1);
  tree->SetBranchAddress("Nevent", &Nevent,&b_Nevent);
  tree->SetBranchAddress("Nweight", &Nweight,&b_Nweight);
  tree->SetBranchAddress("filetag", &filetag,&b_filetag);
  tree->SetBranchAddress("dataset", &dataset,&b_dataset);
  tree->SetBranchAddress("MP", &MP,&b_MP);
  tree->SetBranchAddress("MC", &MC,&b_MC);

  double tot_Nevent = 0;
  double tot_Nweight = 0;

  for(int i = 0; i < tree->GetEntries(); i++)
  {
   tree->GetEntry(i);
   if(dataset->find(input_dataset) != std::string::npos && filetag->find(input_filetag) != std::string::npos)
   {
    tot_Nevent += Nevent;
    tot_Nweight += Nweight;
   }
  }
  fout->Close();
  delete fout;
  return tot_Nevent/tot_Nweight;
}

void Plot_1D_NANO(){

  
  RestFrames::SetStyle();

  string g_Label = "";
  g_Xname = "HT  [GeV]";

  g_Xmin = 0.;
  g_Xmax = 3000.; 
  g_NX = 128;

   g_PlotTitle = "";

   string path_to_lists = "/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";

/*
   vector<string> datasets_list_2016 = {
    "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
   };
   string filetag = "Summer16_102X";
   string eventcount = "root/EventCount/EventCount_NANO_Summer16_102X.root";
   for(int i = 0; i < int(datasets_list_2016.size()); i++) { cout << "Weight Per Event: " << filetag << " " << datasets_list_2016[i] << " " << getWeightPerEvent(datasets_list_2016[i],filetag,eventcount) << endl; }

   vector<string> datasets_list_2017 = {
    "WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8",
   };
   filetag = "Fall17_102X";
   eventcount = "root/EventCount/EventCount_NANO_Fall17_102X.root";
   for(int i = 0; i < int(datasets_list_2017.size()); i++) { cout << "Weight Per Event: " << filetag << " " << datasets_list_2017[i] << " " << getWeightPerEvent(datasets_list_2017[i],filetag,eventcount) << endl; }

   vector<string> datasets_list_2018 = {
    "WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8",
   };
   filetag = "Autumn18_102X";
   eventcount = "root/EventCount/EventCount_NANO_Autumn18_102X.root";
   for(int i = 0; i < int(datasets_list_2018.size()); i++) { cout << "Weight Per Event: " << filetag << " " << datasets_list_2018[i] << " " << getWeightPerEvent(datasets_list_2018[i],filetag,eventcount) << endl; }
*/




   std::map<string,double> dataset_list_2016 = {
    { "Summer16_102X/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 1345 * 1.21 * 0.993},
    { "Summer16_102X/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 1.329 * 1.21 * 1.332},
    { "Summer16_102X/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 359.7 * 1.21 * 1.002},
    { "Summer16_102X/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 0.03216 * 1.21 * 4.200},
    { "Summer16_102X/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 48.91 * 1.21 * 1.009},
    { "Summer16_102X/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 12.05 * 1.21 * 1.120},
    { "Summer16_102X/WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 1353.0 * 1.21},
    { "Summer16_102X/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",1.0 * 5.501 * 1.21 * 1.202},
   };

   std::map<string,double> dataset_list_2017 = {
    {"Fall17_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00114 * 1353.0 * 1.21},
    {"Fall17_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00163 * 1395.0 * 1.21 * 0.993},
    {"Fall17_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00275 * 407.9 * 1.21 * 1.002},
    {"Fall17_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00443 * 57.48 * 1.21 * 1.009},
    {"Fall17_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00587 * 12.87 * 1.21 * 1.120},
    {"Fall17_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00787 * 5.366 * 1.21 * 1.202},
    {"Fall17_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.01334 * 1.074 * 1.21 * 1.332},
    {"Fall17_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.04197 * 0.008001 * 1.21 * 4.200},
   };

   std::map<string,double> dataset_list_2018 = {
    {"Autumn18_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00135 * 1395.0 * 1.21 * 0.993},
    {"Autumn18_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00884 * 1.074 * 1.21 * 1.332},
    {"Autumn18_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00212 * 407.9 * 1.21 * 1.002},
    {"Autumn18_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.02652 * 0.008001 * 1.21 * 4.200},
    {"Autumn18_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00323 * 57.48 * 1.21 * 1.009},
    {"Autumn18_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00409 * 12.87 * 1.21 * 1.120},
    {"Autumn18_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00104 * 1353.0 * 1.21},
    {"Autumn18_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt",1.00536 * 5.366 * 1.21 * 1.202},
   };

  vector<std::map<string,double>> datasets = {
   dataset_list_2016,
   dataset_list_2017,
   dataset_list_2018,
  };

  for(int d = 0; d < int(datasets.size()); d++)
  {
   vector<string> files;
   for(std::map<string,double>::iterator iter = datasets[d].begin(); iter != datasets[d].end(); ++iter)
   {
    files.clear();
    string dataset = iter->first;
    double weight = iter->second;
    load_files_from_list(files,path_to_lists+dataset);
    dataset = get_str_between_two_str(dataset,"/",".");
    TH1D* hist = new TH1D(("hist_"+dataset).c_str(), ("hist_"+dataset).c_str(), g_NX, g_Xmin, g_Xmax);
    TChain* chain = new TChain("Events");

    for(int f = 0; f < int(files.size()); f++) { chain->Add(files[f].c_str()); }

      SUSYNANOBase* base = new SUSYNANOBase(chain);
      int Nentry = base->fChain->GetEntries();
      
      int SKIP = 1;
      
      // event loop


      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;
	
        

if(e>10) break;
	
	hist->Fill(base->LHE_HT, base->genWeight*weight*double(SKIP));
      }
      
      delete base;
      delete chain;

       //cout << "Total " << hist->Integral()*137. << endl;
       
     
     //  hist->Scale(1./hist->Integral());
       
       gStyle->SetOptTitle(0);
       gStyle->SetOptStat(0);
       gStyle->SetOptFit(11111111);
     
       TFile* output_file = new TFile(("output_Plot_1D_NANO_"+dataset+".root").c_str(),"UPDATE");
    /* 
       TCanvas* can = (TCanvas*) new TCanvas("can","can",700.,600);
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
       hist->GetXaxis()->SetTitle(g_Xname.c_str());
       hist->GetYaxis()->CenterTitle();
       hist->GetYaxis()->SetTitleFont(42);
       hist->GetYaxis()->SetTitleSize(0.06);
       hist->GetYaxis()->SetTitleOffset(1.12);
       hist->GetYaxis()->SetLabelFont(42);
       hist->GetYaxis()->SetLabelSize(0.05);
       hist->GetYaxis()->SetTitle("N_{events} / 137 fb^{-1}");
       hist->GetYaxis()->SetRangeUser(0.9*hist->GetMinimum(0.0),1.1*hist->GetMaximum());
     
       TLatex l;
       l.SetTextFont(42);
       l.SetNDC();
       l.SetTextSize(0.035);
       l.SetTextFont(42);
       // l.DrawLatex(0.17,0.855,g_PlotTitle.c_str());
       l.DrawLatex(0.71,0.943,g_PlotTitle.c_str());
       l.SetTextSize(0.04);
       l.SetTextFont(42);
       l.DrawLatex(0.01,0.943,"#bf{CMS} Simulation Preliminary");
     
       l.SetTextSize(0.045);
       l.SetTextFont(42);
       l.DrawLatex(0.7,0.04,g_Label.c_str());
     
       //can->SaveAs("output_Plot_1D_NANO.pdf");
       //can->Write();
       */
       hist->Write();
       output_file->Close(); 
       //delete hist;
       //delete output_file;
   }
  }

}
