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

bool find_file(string input_dataset, string input_filetag)
{
  bool found = false;
  gSystem->Exec(("xrdfs root://cmseos.fnal.gov/ ls /store/group/lpcsusylep/NTUPLES_v0/"+input_filetag+"/ > checkfile.txt").c_str());
  ifstream checkfile;
  checkfile.open("checkfile.txt");
  while (!checkfile.eof())
  {
   string line = "";
   getline(checkfile,line);
   if(line.find(input_dataset+"_"+input_filetag+".root") != string::npos) 
   {
    checkfile.close();
    gSystem->Exec("rm checkfile.txt");
    return true;
   }
  }
  checkfile.close();
  gSystem->Exec("rm checkfile.txt");
  return found;
}

double getTot(string input_dataset, string input_filetag, string EventCountFile, bool weight, bool ntuple)
{
  string filename = "";
  if(ntuple)
    filename = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v0/"+input_filetag+"/"+input_dataset+"_"+input_filetag+".root";
  else
    filename = EventCountFile;

  if(!find_file(input_dataset,input_filetag))
    return -1.;
  TFile* fout = TFile::Open(filename.c_str(),"READ");

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

double GetSingleEventWeight(string dataset = "", string filetag = "")
{
 double m_weight = 0.;
 string filename = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v0/"+filetag+"/"+dataset+"_"+filetag+".root";
 if(!find_file(dataset,filetag))
   return -1.;
 TFile* file = TFile::Open(filename.c_str(),"READ");
 Double_t weight;
 TTree *tree;
 file->GetObject("KUAnalysis",tree);
 tree->SetBranchAddress("weight",&weight);
 tree->GetEntry(0);
 m_weight = weight;
 file->Close();
 return m_weight;
}

void CheckAllEWeights(string dataset = "", string filetag = "", double e_weight = 0.)
{
 string filename = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v0/"+filetag+"/"+dataset+"_"+filetag+".root";
 if(gSystem->AccessPathName(filename.c_str())) { cout << "File: " << filename << " does not exist..." << std::endl; return; }
 TFile* file = TFile::Open(filename.c_str(),"READ");
 Double_t weight;
 TTree *tree;
 file->GetObject("KUAnalysis",tree);
 tree->SetBranchAddress("weight",&weight);
 int entries = tree->GetEntries();
 int bad_events = 0;
 double deviation = 0.;
 for(int i = 0; i < entries; i++)
 {
  tree->GetEntry(i);
  if((i)%(std::max(1, int(entries/10))) == 0)
    cout << "event " << i << " | " << entries << endl;
  if(fabs((weight - e_weight)/e_weight*100.) > 0.01)
  {
   bad_events++;
   deviation += weight - e_weight;
  }
 }
 cout << "average deviation for " << filename << ": " << deviation/bad_events << "(" << deviation/bad_events << "/" << e_weight << ")" << endl;
 file->Close();
}

void CheckWeights(){

   RestFrames::SetStyle();
   cout << std::fixed;

   XsecTool XS;
   string filetag;
   string eventcount;
   //vector<string> datasets_list_2017 = {
   // //"GluGluHToWWToLNuQQ_M125_NNPDF31_TuneCP5_PSweights_13TeV_powheg_JHUGen710_pythia8",
   // "TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
   // "WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8",
   //};
   vector<string> datasets_list_2017;
   std::map<std::string,double> XS_map = XS.InitMap_Xsec_BKG();
   for ( const auto &myPair : XS_map )
   {
     string dataset_name = myPair.first;
     if(dataset_name[0] == '&')
       dataset_name.erase(0,1);
     datasets_list_2017.push_back(dataset_name);
   }
   filetag = "Fall17_102X";
   eventcount = "root/EventCount/EventCount_NANO_Fall17_102X.root";
   for(int i = 0; i < int(datasets_list_2017.size()); i++) 
   { 
    //int Events_File_Default = int(getTot(datasets_list_2017[i],filetag,eventcount,false));
    //int Events_File_new = int(getTot(datasets_list_2017[i],filetag,"/uscms/home/z374f439/nobackup/EventCount/root/EventCount_TEST.root",false));
    //cout << "Default: " << Events_File_Default << endl;
    //cout << "New: " << Events_File_new << endl;

    double TotEvents_Ntuple = getTot(datasets_list_2017[i],filetag,eventcount,false,true);
    double TotEvents = getTot(datasets_list_2017[i],filetag,eventcount,false,false);
    //double TotWeight_Ntuple = getTot(datasets_list_2017[i],filetag,eventcount,true,true); // EventCount tree in ntuples is bugged...
    double TotWeight = getTot(datasets_list_2017[i],filetag,eventcount,true,false);
    double e_weight = GetSingleEventWeight(datasets_list_2017[i],filetag);
    double theory_XS = XS.GetXsec_BKG(datasets_list_2017[i])/1000.;
    double calc_XS = TotWeight*e_weight/1000.0;
    //cout << "Checking all event weights... " << endl;
    //CheckAllEWeights(datasets_list_2017[i],filetag,e_weight);

    //cout << "tot event from ntuple "+datasets_list_2017[i]+": " << TotEvents_Ntuple << endl;
    //cout << "tot event from local event count: " << TotEvents << endl;
    //cout << "tot weight from ntuple "+datasets_list_2017[i]+": " << TotWeight_Ntuple << endl;
    //cout << "tot weight from local event count: " << TotWeight << endl;
    //cout << "event weight: " << e_weight << endl;
    //cout << "calc xsec: " << calc_XS << endl;
    //cout << "theory xsec: " << theory_XS << endl;
    if(TotEvents_Ntuple == -1.) continue;
    if(TotEvents_Ntuple != TotEvents)
    {
     cout << "check dataset: " << datasets_list_2017[i] << endl;
     cout << "ntuple events:      " << TotEvents_Ntuple << endl;
     cout << "event count events: " << TotEvents << endl;
    }
    if((theory_XS - calc_XS)/theory_XS*100. > 1.)
    {
     cout << "check cross section: " << datasets_list_2017[i] << endl;
     cout << "theory XS: " << theory_XS << endl;
     cout << "calc xsec: " << calc_XS << endl;
    }



   }
}
