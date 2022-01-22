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

void SumEventCounter(){

  RestFrames::SetStyle();

   std::map<string,double> WJets_file_list_2016 = {
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.},
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.2},
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.2},
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.22},
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 4.5},
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.67},
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.37},
    {"EventCount_Plot_1D_NANO_Summer16_102X_WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 1.7},
   };

   std::map<string,double> ZJets_file_list_2016 = {
    {"EventCount_Plot_1D_NANO_Summer16_102X_ZJetsToNuNu_HT-100To200_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Summer16_102X_ZJetsToNuNu_HT-200To400_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Summer16_102X_ZJetsToNuNu_HT-400To600_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Summer16_102X_ZJetsToNuNu_HT-600To800_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Summer16_102X_ZJetsToNuNu_HT-800To1200_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Summer16_102X_ZJetsToNuNu_HT-1200To2500_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Summer16_102X_ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph", 1.},
   };

   std::map<string,double> WJets_file_list_2017 = {
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8", 1.2},
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8", 2.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8", 1.3},
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8", 16.35},
    {"EventCount_Plot_1D_NANO_Fall17_102X_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8", 2.75},
   };

   std::map<string,double> ZJets_file_list_2017 = {
    {"EventCount_Plot_1D_NANO_Fall17_102X_ZJetsToNuNu_HT-100To200_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_ZJetsToNuNu_HT-200To400_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_ZJetsToNuNu_HT-400To600_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_ZJetsToNuNu_HT-600To800_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_ZJetsToNuNu_HT-800To1200_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_ZJetsToNuNu_HT-1200To2500_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Fall17_102X_ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph", 1.},
   };

   std::map<string,double> WJets_file_list_2018 = {
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8", 5.},
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8", 1.},
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8", 0.85},
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8", 1.5},
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8", 1.27},
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8", 1.83},
   {"EventCount_Plot_1D_NANO_Autumn18_102X_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8", 50.23},
   };

   std::map<string,double> ZJets_file_list_2018 = {
    {"EventCount_Plot_1D_NANO_Autumn18_102X_ZJetsToNuNu_HT-100To200_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Autumn18_102X_ZJetsToNuNu_HT-200To400_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Autumn18_102X_ZJetsToNuNu_HT-400To600_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Autumn18_102X_ZJetsToNuNu_HT-600To800_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Autumn18_102X_ZJetsToNuNu_HT-800To1200_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Autumn18_102X_ZJetsToNuNu_HT-1200To2500_13TeV-madgraph", 1.},
    {"EventCount_Plot_1D_NANO_Autumn18_102X_ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph", 1.},
   };

  vector<std::map<string,double>> file_lists = {
   WJets_file_list_2016,
   WJets_file_list_2017,
   WJets_file_list_2018,
   //ZJets_file_list_2016,
   //ZJets_file_list_2017,
   //ZJets_file_list_2018,
  };
  string name_finder = "_W";
  string path_to_lists = "/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";
  cout << fixed;

  for(int l = 0; l < int(file_lists.size()); l++)
  {
    for(std::map<string,double>::iterator iter = file_lists[l].begin(); iter != file_lists[l].end(); ++iter)
    {
     string inputRootFileName = iter->first;
     double SF = iter->second;
     std::ifstream infile(path_to_lists+get_str_between_two_str(inputRootFileName,"NANO_",name_finder)+"_102X/"+inputRootFileName.substr(inputRootFileName.find(name_finder)+1)+".txt");
     string line = "";
     int counter = 0;
     int Nevents = 0;
     double Nweight = 0.;
     while(getline(infile,line))
     {
      int event;
      double weight;
      std::ifstream file;
      file.open(inputRootFileName+"_"+std::to_string(counter)+".txt");
      file >> event;
      file >> weight;
      Nevents += event;
      Nweight += weight;
      file.close();
      counter++;
     }
     infile.close();
     cout << inputRootFileName << " " << Nevents << " " << Nweight << endl;
   }
  }

}
