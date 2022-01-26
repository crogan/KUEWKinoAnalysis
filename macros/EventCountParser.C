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
#include <TSystem.h>
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

string path_to_lists = "/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";

std::string get_str_between_two_str(const std::string &s, const std::string &start_delim, const std::string &stop_delim)
{
 unsigned first_delim_pos = s.find(start_delim);
 unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
 unsigned last_delim_pos = s.find_first_of(stop_delim, end_pos_of_first_delim);
 return s.substr(end_pos_of_first_delim,last_delim_pos - end_pos_of_first_delim);
}

void getEvents_Manual(const string& input_dataset, const string& input_filetag, int& Nevents, double& Nweight)
{
 string inputFileName = "EventCount_Plot_1D_NANO_"+input_filetag+"_"+input_dataset;
 std::ifstream infile(path_to_lists+input_filetag+"/"+input_dataset+".txt");
 string line = "";
 int counter = 0;
 while(getline(infile,line))
 {
  int event;
  double weight;
  std::ifstream file;
  file.open(inputFileName+"_"+std::to_string(counter)+".txt");
  file >> event;
  file >> weight;
  Nevents += event;
  Nweight += weight;
  file.close();
  counter++;
 }
 infile.close();
}

double getTot(string input_dataset, string input_filetag, string EventCountFile, bool weight)
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
  if(weight)
    return tot_Nweight;
  else
    return tot_Nevent;
}

double EventsInDAS(string dataset = "", string filetag = "")
{
 filetag.erase(filetag.length()-5);
 double Events = 0.;
 //gSystem->Exec(("dasgoclient -query=\"file dataset=/"+dataset+"/*"+filetag+"*NanoAODv7*"+"/NANO*\" -json > "+filetag+"_"+dataset+".json").c_str());
 //cout << "dasgoclient -query=\"dataset=/"+dataset+"/*"+filetag+"*NanoAODv7*"+"/NANO*\" -json > "+filetag+"_"+dataset+".json" << endl;
 gSystem->Exec(("dasgoclient -query=\"dataset=/"+dataset+"/*"+filetag+"*NanoAODv7*"+"/NANO*\" > datasets_"+filetag+"_"+dataset+".txt").c_str());
 std::ifstream infile("datasets_"+filetag+"_"+dataset+".txt");
 string dataset_fullname = "";
 while(getline(infile,dataset_fullname))
 {
  gSystem->Exec(("dasgoclient -query=\"file dataset="+dataset_fullname+"\" -json >> "+filetag+"_"+dataset+".json").c_str());
 }
 infile.close();
 infile.open(filetag+"_"+dataset+".json");
 string line = "";
 while(getline(infile,line))
 {
  if(line.find("nevents") != std::string::npos)
  {
   string events_string = get_str_between_two_str(line,"nevents\":",",");
   Events += std::stod(events_string);
  }
 }
 gSystem->Exec("rm datasets_*.txt");
 gSystem->Exec("rm *.json");
 return Events;
}

void EventCountParser(){

   RestFrames::SetStyle();

   cout << std::fixed;
   vector<string> datasets_list_2016 = {
    "WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "ZJetsToNuNu_HT-100To200_13TeV-madgraph",
    "ZJetsToNuNu_HT-200To400_13TeV-madgraph",
    "ZJetsToNuNu_HT-400To600_13TeV-madgraph",
    "ZJetsToNuNu_HT-600To800_13TeV-madgraph",
    "ZJetsToNuNu_HT-800To1200_13TeV-madgraph",
    "ZJetsToNuNu_HT-1200To2500_13TeV-madgraph",
    "ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph",
   };
   string filetag = "Summer16_102X";
   string eventcount = "root/EventCount/EventCount_NANO_Summer16_102X.root";
   for(int i = 0; i < int(datasets_list_2016.size()); i++) 
   { 
    int Events_DAS = int(EventsInDAS(datasets_list_2016[i],filetag));
    int Events_File = int(getTot(datasets_list_2016[i],filetag,eventcount,false));
    int Events_Manual = 0;
    double Weight_Manual = 0;
    getEvents_Manual(datasets_list_2016[i],filetag,Events_Manual,Weight_Manual);
    double Weight_File = getTot(datasets_list_2016[i],filetag,eventcount,true);
    if(Weight_File != Weight_Manual)
    {
     //cout << "Total Weight From File: " << filetag << " " << datasets_list_2016[i] << " " << Weight_File << endl; 
     //cout << "Total Weight From Manual: " << filetag << " " << datasets_list_2016[i] << " " << Weight_Manual << endl; 
     cout << "Weight From File - Weight From Manual: " << filetag << " " << datasets_list_2016[i] << " " << Weight_File - Weight_Manual << endl;
    }
    if(Events_File != Events_DAS)
    {
     cout << filetag << " " << datasets_list_2016[i] << endl;
     cout << "Total Events From File: " << filetag << " " << datasets_list_2016[i] << " " << Events_File << endl; 
     cout << "Total Events From DAS: " << filetag << " " << datasets_list_2016[i] << " " << Events_DAS << endl;
    }
    if(Events_Manual != Events_DAS)
    {
     cout << filetag << " " << datasets_list_2016[i] << endl;
     cout << "Total Events From Manual: " << filetag << " " << datasets_list_2016[i] << " " << Events_Manual << endl; 
     cout << "Total Events From DAS: " << filetag << " " << datasets_list_2016[i] << " " << Events_DAS << endl;
    }
   }

   vector<string> datasets_list_2017 = {
    "WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8",
    "ZJetsToNuNu_HT-100To200_13TeV-madgraph",
    "ZJetsToNuNu_HT-200To400_13TeV-madgraph",
    "ZJetsToNuNu_HT-400To600_13TeV-madgraph",
    "ZJetsToNuNu_HT-600To800_13TeV-madgraph",
    "ZJetsToNuNu_HT-800To1200_13TeV-madgraph",
    "ZJetsToNuNu_HT-1200To2500_13TeV-madgraph",
    "ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph",
   };
   filetag = "Fall17_102X";
   eventcount = "root/EventCount/EventCount_NANO_Fall17_102X.root";
   for(int i = 0; i < int(datasets_list_2017.size()); i++) 
   { 
    int Events_DAS = int(EventsInDAS(datasets_list_2017[i],filetag));
    int Events_File = int(getTot(datasets_list_2017[i],filetag,eventcount,false));
    int Events_Manual = 0;
    double Weight_Manual = 0.;
    getEvents_Manual(datasets_list_2017[i],filetag,Events_Manual,Weight_Manual);
    double Weight_File = getTot(datasets_list_2017[i],filetag,eventcount,true);
    if(Weight_File != Weight_Manual)
    {
     //cout << "Total Weight From File: " << filetag << " " << datasets_list_2017[i] << " " << Weight_File << endl; 
     //cout << "Total Weight From Manual: " << filetag << " " << datasets_list_2017[i] << " " << Weight_Manual << endl; 
     cout << "Weight From File - Weight From Manual: " << filetag << " " << datasets_list_2017[i] << " " << Weight_File - Weight_Manual << endl;
    }
    if(Events_File != Events_DAS)
    {
     cout << filetag << " " << datasets_list_2017[i] << endl;
     cout << "Total Events From File: " << filetag << " " << datasets_list_2017[i] << " " << Events_File << endl; 
     cout << "Total Events From DAS: " << filetag << " " << datasets_list_2017[i] << " " << Events_DAS << endl;
    }
    if(Events_Manual != Events_DAS)
    {
     cout << filetag << " " << datasets_list_2017[i] << endl;
     cout << "Total Events From Manual: " << filetag << " " << datasets_list_2017[i] << " " << Events_Manual << endl; 
     cout << "Total Events From DAS: " << filetag << " " << datasets_list_2017[i] << " " << Events_DAS << endl;
    }
   }

   vector<string> datasets_list_2018 = {
    "WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8",
    "WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8",
    "ZJetsToNuNu_HT-100To200_13TeV-madgraph",
    "ZJetsToNuNu_HT-200To400_13TeV-madgraph",
    "ZJetsToNuNu_HT-400To600_13TeV-madgraph",
    "ZJetsToNuNu_HT-600To800_13TeV-madgraph",
    "ZJetsToNuNu_HT-800To1200_13TeV-madgraph",
    "ZJetsToNuNu_HT-1200To2500_13TeV-madgraph",
    "ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph",
   };
   filetag = "Autumn18_102X";
   eventcount = "root/EventCount/EventCount_NANO_Autumn18_102X.root";
   for(int i = 0; i < int(datasets_list_2018.size()); i++) 
   { 
    int Events_DAS = int(EventsInDAS(datasets_list_2018[i],filetag));
    int Events_File = int(getTot(datasets_list_2018[i],filetag,eventcount,false));
    int Events_Manual = 0;
    double Weight_Manual = 0;
    getEvents_Manual(datasets_list_2018[i],filetag,Events_Manual,Weight_Manual);
    double Weight_File = getTot(datasets_list_2018[i],filetag,eventcount,true);
    if(Weight_File != Weight_Manual)
    {
     //cout << "Total Weight From File: " << filetag << " " << datasets_list_2018[i] << " " << Weight_File << endl; 
     //cout << "Total Weight From Manual: " << filetag << " " << datasets_list_2018[i] << " " << Weight_Manual << endl; 
     cout << "Weight From File - Weight From Manual: " << filetag << " " << datasets_list_2018[i] << " " << Weight_File - Weight_Manual << endl;
    }
    if(Events_File != Events_DAS)
    {
     cout << filetag << " " << datasets_list_2018[i] << endl;
     cout << "Total Events From File: " << filetag << " " << datasets_list_2018[i] << " " << Events_File << endl; 
     cout << "Total Events From DAS: " << filetag << " " << datasets_list_2018[i] << " " << Events_DAS << endl;
    }
    if(Events_Manual != Events_DAS)
    {
     cout << filetag << " " << datasets_list_2018[i] << endl;
     cout << "Total Events From Manual: " << filetag << " " << datasets_list_2018[i] << " " << Events_Manual << endl; 
     cout << "Total Events From DAS: " << filetag << " " << datasets_list_2018[i] << " " << Events_DAS << endl;
    }
   }

}
