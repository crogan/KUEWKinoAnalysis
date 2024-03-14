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

bool connect = true;

string path_to_lists = "";

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
  if(input_filetag.find("_SMS") != std::string::npos)
    input_filetag.erase(input_filetag.length()-4);
  ifstream checkfile;
  checkfile.open("checkfile.txt");
  while (!checkfile.eof())
  {
   string line = "";
   getline(checkfile,line);
   if(line.find(input_dataset+"_"+input_filetag+".root") != string::npos) 
   {
    checkfile.close();
    return true;
   }
  }
  checkfile.close();
  return false;
}

double getTot(string input_dataset, string input_filetag, string EventCountFile, bool weight, bool ntuple)
{
  string filename = "";
  if(ntuple && connect)
    filename = "/stash/user/zflowers/NTUPLES/HADD/"+input_filetag+"/"+input_dataset+"_"+input_filetag+".root";
  if(ntuple && !connect)
    filename = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v0/"+input_filetag+"/"+input_dataset+"_"+input_filetag+".root";
  else
    filename = EventCountFile;

  if(ntuple && filename.find("_SMS.root") != std::string::npos)
   filename.erase(filename.find("_SMS.root"),4);
  if(ntuple && !find_file(input_dataset,input_filetag))
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
  if(tree == 0 || tree == nullptr)
  {
   tree = nullptr;
   fout->Close();
   return -1.;
  }
  
  tree->SetMakeClass(1);
  tree->SetBranchAddress("Nevent", &Nevent,&b_Nevent);
  tree->SetBranchAddress("Nweight", &Nweight,&b_Nweight);
  tree->SetBranchAddress("filetag", &filetag,&b_filetag);
  tree->SetBranchAddress("dataset", &dataset,&b_dataset);
  tree->SetBranchAddress("MP", &MP,&b_MP);
  tree->SetBranchAddress("MC", &MC,&b_MC);

  double tot_Nevent = 0;
  double tot_Nweight = 0;


  if(input_filetag.find("_SMS") != std::string::npos)
    input_filetag.erase(input_filetag.length()-4);
  for(int i = 0; i < tree->GetEntries(); i++)
  {
   tree->GetEntry(i);
   if(*dataset == input_dataset && filetag->find(input_filetag) != std::string::npos)
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

bool check_dataset_file(string dataset_name)
{
 std::ifstream testfile(dataset_name);
 if(testfile.peek() == std::ifstream::traits_type::eof())
 {
  testfile.close();
  return false;
 }
 testfile.close();
 return true;
}

double EventsInDAS(string dataset = "", string filetag = "")
{
 if(filetag.find("_SMS") != std::string::npos)
   filetag.erase(filetag.length()-4);
 filetag.erase(filetag.length()-5);
 double Events = 0.;
 //gSystem->Exec(("dasgoclient -query=\"file dataset=/"+dataset+"/*"+filetag+"*NanoAODv7*"+"/NANO*\" -json > "+filetag+"_"+dataset+".json").c_str());
 //cout << "dasgoclient -query=\"dataset=/"+dataset+"/*"+filetag+"*NanoAODv7*"+"/NANO*\" -json > "+filetag+"_"+dataset+".json" << endl;
 gSystem->Exec(("dasgoclient -query=\"dataset=/"+dataset+"/*"+filetag+"*NanoAODv7*"+"*/NANO*\" >> datasets_"+filetag+"_"+dataset+".txt").c_str());
 if(!check_dataset_file("datasets_"+filetag+"_"+dataset+".txt"))
   gSystem->Exec(("dasgoclient -query=\"dataset=/"+dataset+"/*"+filetag+"*NanoAODv4*"+"*/NANO*\" >> datasets_"+filetag+"_"+dataset+".txt").c_str());
 if(!check_dataset_file("datasets_"+filetag+"_"+dataset+".txt"))
   gSystem->Exec(("dasgoclient -query=\"dataset=/"+dataset+"/*"+filetag+"*NanoAOD*"+"*/NANO*\" >> datasets_"+filetag+"_"+dataset+".txt").c_str());
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
 int entries = tree->GetEntries();
 m_weight = weight;
/*
 for(int i = 0; i < entries; i++)
 {
  tree->GetEntry(i);
  if((i)%(std::max(1, int(entries/10))) == 0)
    cout << "event " << i << " | " << entries << endl;
  m_weight += weight;
 }
 m_weight /= entries;
*/
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

   if(connect)
     path_to_lists = "/stash/user/zflowers/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";
   else
     path_to_lists = "/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/";

   //vector<string> datasets_list = {
    //"GluGluHToWWToLNuQQ_M125_NNPDF31_TuneCP5_PSweights_13TeV_powheg_JHUGen710_pythia8",
    //"TTJets_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8",
   //};
   vector<string> filetags = {
    "Summer16_102X",
    "Fall17_102X",
    "Autumn18_102X",
    "Summer16_102X_SMS",
    "Fall17_102X_SMS",
    "Autumn18_102X_SMS",
   };
   //vector<string> datasets_list;
   //std::map<std::string,double> XS_map = XS.InitMap_Xsec_BKG();
   //for ( const auto &myPair : XS_map )
   //{
   //  string dataset_name = myPair.first;
   //  if(dataset_name[0] == '&')
   //    dataset_name.erase(0,1);
   //  datasets_list.push_back(dataset_name);
   //}
   XsecTool XS;
   for(int t = 0; t < int(filetags[t].size()); t++)
   {
    string eventcount = "root/EventCount/EventCount_NANO_"+filetags[t]+".root";
    if(connect)
      gSystem->Exec(("ls /stash/user/zflowers/NTUPLES/HADD/"+filetags[t]+"/ > checkfile.txt").c_str());
    else
      gSystem->Exec(("xrdfs root://cmseos.fnal.gov/ ls /store/group/lpcsusylep/NTUPLES_v0/"+filetags[t]+"/ > checkfile.txt").c_str());
    ifstream tagfile;
    tagfile.open("samples/NANO/"+filetags[t]+"/Tags.list");
    while (!tagfile.eof())
    {
     string dataset = "";
     getline(tagfile,dataset);
     if(dataset == "") continue;
     if(dataset[0] == '#') continue;

     //int Events_File_Default = int(getTot(dataset,filetags[t],eventcount,false));
     //int Events_File_new = int(getTot(dataset,filetags[t],"/uscms/home/z374f439/nobackup/EventCount/root/EventCount_TEST.root",false));
     //cout << "Default: " << Events_File_Default << endl;
     //cout << "New: " << Events_File_new << endl;

     //int TotEvents_Ntuple = int(getTot(dataset,filetags[t],eventcount,false,true));
     //int TotEvents = int(getTot(dataset,filetags[t],eventcount,false,false));
     //int Events_DAS = int(EventsInDAS(dataset,filetags[t]));
     double TotEvents_Ntuple = getTot(dataset,filetags[t],eventcount,false,true);
     double TotEvents = getTot(dataset,filetags[t],eventcount,false,false);
     double Events_DAS = EventsInDAS(dataset,filetags[t]);
     //double TotWeight_Ntuple = getTot(dataset,filetags[t],eventcount,true,true); // EventCount tree in ntuples is bugged...
     //double TotWeight = getTot(dataset,filetags[t],eventcount,true,false);
     //double e_weight = GetSingleEventWeight(dataset,filetags[t]);
     //double theory_XS = XS.GetXsec_BKG(dataset)/1000.;
     //double calc_XS = TotWeight*e_weight/1000.0;
     //double calc_XS = TotWeight/TotEvents_Ntuple;
     //double calc_XS = TotWeight/TotEvents;

     //cout << "Checking all event weights... " << endl;
     //CheckAllEWeights(dataset,filetags,e_weight);

     //cout << "tot event from ntuple "+dataset+": " << TotEvents_Ntuple << endl;
     //cout << "tot event from local event count: " << TotEvents << endl;
     //cout << "tot event from DAS: " << Events_DAS << endl;
     //cout << "tot weight from ntuple "+dataset+": " << TotWeight_Ntuple << endl;
     //cout << "tot weight from local event count: " << TotWeight << endl;
     //cout << "event weight: " << e_weight << endl;
     //cout << "calc xsec: " << calc_XS << endl;
     //cout << "theory xsec: " << theory_XS << endl;

     if(TotEvents_Ntuple == -1) continue;
     if(TotEvents != Events_DAS || TotEvents_Ntuple != Events_DAS || TotEvents_Ntuple != TotEvents)
     //if(TotEvents != Events_DAS)
     {
       cout << "check dataset: " << dataset << " " << filetags[t] << endl;
       cout << "  Ntuple:     " << TotEvents_Ntuple << endl;
       cout << "  EventCount: " << TotEvents << endl;
       cout << "  DAS:        " << Events_DAS << endl;
     }
     //if(TotEvents_Ntuple == -1. || TotEvents == -1.) continue;
     //if(TotEvents_Ntuple != TotEvents)
     //{
     // cout << "check dataset: " << dataset << " " << filetags[t] << endl;
     // cout << "ntuple events:      " << TotEvents_Ntuple << endl;
     // cout << "event count events: " << TotEvents << endl;
     //}
/* 
     if((theory_XS - calc_XS)/theory_XS*100. > 1.)
     {
      cout << "check cross section: " << dataset << endl;
      cout << "theory XS: " << theory_XS << endl;
      cout << "calc xsec: " << calc_XS << endl;
     }
*/ 
    }
cout << "Gonna crash...??? " << endl;
    tagfile.close();
    gSystem->Exec("rm checkfile.txt");
   }
cout << "Gonna crash...??? " << endl;

}
