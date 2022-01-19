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


using namespace RestFrames;

double getWeightTot(string input_dataset, string input_filetag, string EventCountFile)
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
  return tot_Nweight;
}

void EventCountParser(){

   RestFrames::SetStyle();

   string path_to_lists = "/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";
   vector<string> datasets_list_2016 = {
    "WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
   };
   string filetag = "Summer16_102X";
   string eventcount = "root/EventCount/EventCount_NANO_Summer16_102X.root";
   for(int i = 0; i < int(datasets_list_2016.size()); i++) { cout << "Total Weight: " << filetag << " " << datasets_list_2016[i] << " " << getWeightTot(datasets_list_2016[i],filetag,eventcount) << endl; }

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
   for(int i = 0; i < int(datasets_list_2017.size()); i++) { cout << "Total Weight: " << filetag << " " << datasets_list_2017[i] << " " << getWeightTot(datasets_list_2017[i],filetag,eventcount) << endl; }

   vector<string> datasets_list_2018 = {
    "WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8",
   };
   filetag = "Autumn18_102X";
   eventcount = "root/EventCount/EventCount_NANO_Autumn18_102X.root";
   for(int i = 0; i < int(datasets_list_2018.size()); i++) { cout << "Total Weight: " << filetag << " " << datasets_list_2018[i] << " " << getWeightTot(datasets_list_2018[i],filetag,eventcount) << endl; }

}
