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
#include <TSystem.h>
#include <TApplication.h>

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

std::string get_str_between_two_str(const std::string &s, const std::string &start_delim, const std::string &stop_delim)
{
 unsigned first_delim_pos = s.find(start_delim);
 unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
 unsigned last_delim_pos = s.find_first_of(stop_delim, end_pos_of_first_delim);
 return s.substr(end_pos_of_first_delim,last_delim_pos - end_pos_of_first_delim);
}

void WriteScript(const string& src_name,
                 const string& filetag,
                 const string& dataset,
                 const int& fileline,
		 const long double& weight_val)
{
  ofstream file;
  file.open(src_name);
  string pwd = gSystem->pwd();
  string dataset_name = get_str_between_two_str(dataset,"/",".");
  file << "universe = vanilla" << endl;
  file << "executable = execute_script_Plot_1D_NANO.sh" << endl;
  file << "getenv = True" << endl;
  file << "use_x509userproxy = true" << endl;
  string args = "\\\""+filetag+"\\\",\\\""+dataset_name+"\\\","+std::to_string(fileline)+","+std::to_string(weight_val*1.e7);
  file << "Arguments = " << args << endl;
  file << "output = " << filetag+"_"+dataset_name+"_"+std::to_string(fileline) << ".out" << endl;
  file << "error = "  << filetag+"_"+dataset_name+"_"+std::to_string(fileline) << ".err" << endl;
  file << "log = "    << filetag+"_"+dataset_name+"_"+std::to_string(fileline) << ".log" << endl;
  //file << "transfer_input_files = /uscms/home/z374f439/nobackup/whatever_you_want/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,scripts/cmssw_setup_connect.sh,scripts/setup_RestFrames_connect.sh,macros/EventCounter.C,include/SUSYNANOBase_slim.hh,/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/"+filetag+dataset << endl;
  file << "transfer_input_files = /uscms/home/z374f439/nobackup/whatever_you_want/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,scripts/cmssw_setup_connect.sh,scripts/setup_RestFrames_connect.sh,macros/Condor_Plot_1D_NANO.C,include/SUSYNANOBase_slim.hh,/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/"+filetag+dataset << endl;
  file << "should_transfer_files = YES" << endl;
  file << "when_to_transfer_output = ON_EXIT" << endl;
  file << "transfer_output_files = output_Plot_1D_NANO_"+filetag+"_"+dataset_name+"_"+std::to_string(fileline)+".root" << endl;
  file << "transfer_output_remaps = \""+pwd+"/output_Plot_1D_NANO_"+filetag+"_"+dataset_name+"_"+std::to_string(fileline)+".root\"" << endl;
  //file << "transfer_output_files = EventCount_Plot_1D_NANO_"+filetag+"_"+dataset_name+"_"+std::to_string(fileline)+".txt" << endl;
  //file << "transfer_output_remaps = \""+pwd+"/EventCount_Plot_1D_NANO_"+filetag+"_"+dataset_name+"_"+std::to_string(fileline)+".txt\"" << endl;
  file << "queue " << endl;
  file.close();  
}

double getTot(string input_dataset, string input_filetag, string EventCountFile)
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

void SubmitCondor_Plot_1D_NANO(){

  
  RestFrames::SetStyle();

   std::map<string,long double> dataset_list_2016 = {
    {"/WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 1353.0 * 1.21},
    {"/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 1346.0 * 1.21},
    {"/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 360.1 * 1.21},
    {"/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 48.8 * 1.21},
    {"/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 12.07 * 1.21},
    {"/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 5.497 * 1.21},
    {"/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 1.329 * 1.21},
    {"/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 0.03209 * 1.21},
    {"/ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-100To200_13TeV-madgraph","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 280.35 },
    {"/ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-200To400_13TeV-madgraph","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 77.67 },
    {"/ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-400To600_13TeV-madgraph","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 10.73 },
    {"/ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-600To800_13TeV-madgraph","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 2.559 },
    {"/ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-800To1200_13TeV-madgraph","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 1.1796 },
    {"/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-1200To2500_13TeV-madgraph","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 0.28833 },
    {"/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 0.006945 },
    {"/DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 169.9 * 1.23},
    {"/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 147.40 * 1.23},
    {"/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 40.99 * 1.23},
    {"/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 5.678 * 1.23},
    {"/DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 1.367 * 1.23},
    {"/DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 0.6304 * 1.23},
    {"/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 0.1514 * 1.23},
    {"/DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8","Summer16_102X","root/EventCount/EventCount_NANO_Summer16_102X.root")) * 0.003565 * 1.23},

   };

   std::map<string,long double> dataset_list_2017 = {
    {"/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 1292.0 * 1.21},
    {"/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 1395.0 * 1.21},
    {"/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 407.9 * 1.21},
    {"/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 57.48 * 1.21},
    {"/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 12.87 * 1.21},
    {"/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 5.366 * 1.21},
    {"/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 1.074 * 1.21},
    {"/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 0.008001 * 1.21},
    {"/ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-100To200_13TeV-madgraph","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 280.35 },
    {"/ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-200To400_13TeV-madgraph","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 77.67 },
    {"/ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-400To600_13TeV-madgraph","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 10.73 },
    {"/ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-600To800_13TeV-madgraph","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 2.559 },
    {"/ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-800To1200_13TeV-madgraph","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 1.1796 },
    {"/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-1200To2500_13TeV-madgraph","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 0.28833 },
    {"/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 0.006945 },
    {"/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 169.9 * 1.23},
    {"/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 161.1 * 1.23},
    {"/DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 48.66 * 1.23},
    {"/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 6.968 * 1.23},
    {"/DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 1.743 * 1.23},
    {"/DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 0.6304 * 1.23},
    {"/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 0.1933 * 1.23},
    {"/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8","Fall17_102X","root/EventCount/EventCount_NANO_Fall17_102X.root")) * 0.003468 * 1.23},
   };

   std::map<string,long double> dataset_list_2018 = {
    {"/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 1292.0 * 1.21},
    {"/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 1395.0 * 1.21},
    {"/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 407.9 * 1.21},
    {"/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 57.48 * 1.21},
    {"/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 12.87 * 1.21},
    {"/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 5.366 * 1.21},
    {"/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 1.074 * 1.21},
    {"/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 0.008001 * 1.21},
    {"/ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-100To200_13TeV-madgraph","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 280.35 },
    {"/ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-200To400_13TeV-madgraph","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 77.67 },
    {"/ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-400To600_13TeV-madgraph","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 10.73 },
    {"/ZJetsToNuNu_HT-600To800_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-600To800_13TeV-madgraph","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 2.559 },
    {"/ZJetsToNuNu_HT-800To1200_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-800To1200_13TeV-madgraph","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 1.1796 },
    {"/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-1200To2500_13TeV-madgraph","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 0.28833 },
    {"/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph.txt", (1.0/getTot("ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 0.006945 },
    {"/DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 169.9 * 1.23},
    {"/DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 161.1 * 1.23},
    {"/DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 48.66 * 1.23},
    {"/DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 6.968 * 1.23},
    {"/DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 1.743 * 1.23},
    {"/DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 0.6304 * 1.23},
    {"/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 0.1933 * 1.23},
    {"/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8.txt", (1.0/getTot("DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8","Autumn18_102X","root/EventCount/EventCount_NANO_Autumn18_102X.root")) * 0.003468 * 1.23},
   };

  vector<std::map<string,long double>> datasets = {
   dataset_list_2016,
   dataset_list_2017,
   dataset_list_2018,
  };

  for(int d = 0; d < int(datasets.size()); d++)
  {
   string filetag = "";
   if(d == 0) 
   {
    cout << "   Submitting 2016..." << endl;
    filetag = "Summer16_102X";
   } 
   if(d == 1)
   {
    cout << "   Submitting 2017..." << endl;
    filetag = "Fall17_102X";
   } 
   if(d == 2)
   {
    cout << "   Submitting 2018..." << endl;
    filetag = "Autumn18_102X";
   } 
   string path_to_lists = "/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";
   //setup and submit condor job
   for(std::map<string,long double>::iterator iter = datasets[d].begin(); iter != datasets[d].end(); ++iter)
   {
    string dataset = iter->first;
    long double weight = iter->second;
    string dataset_name = get_str_between_two_str(dataset,"/",".");
    std::ifstream infile(path_to_lists+filetag+dataset);
    string line = "";
    int counter = 0;
    while(getline(infile,line))
    {
     string src_name = filetag+"_"+dataset_name+"_"+std::to_string(counter)+".sh";
     WriteScript(src_name, filetag, dataset, counter, weight); 
     gSystem->Exec(("condor_submit "+src_name).c_str());
     counter++;
    }
   }
  }
 gApplication->Terminate(0);
}
