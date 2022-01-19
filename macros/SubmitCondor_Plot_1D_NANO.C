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
  file << "transfer_input_files = /uscms/home/z374f439/nobackup/whatever_you_want/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,scripts/cmssw_setup_connect.sh,scripts/setup_RestFrames_connect.sh,macros/Condor_Plot_1D_NANO.C,include/SUSYNANOBase_slim.hh,/uscms/home/z374f439/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/samples/NANO/"+filetag+dataset << endl;
  file << "should_transfer_files = YES" << endl;
  file << "when_to_transfer_output = ON_EXIT" << endl;
  file << "transfer_output_files = output_Plot_1D_NANO_"+filetag+"_"+dataset_name+"_"+std::to_string(fileline)+".root" << endl;
  file << "transfer_output_remaps = \""+pwd+"/output_Plot_1D_NANO_"+filetag+"_"+dataset_name+"_"+std::to_string(fileline)+".root\"" << endl;
  file << "queue " << endl;
  file.close();  
}

void SubmitCondor_Plot_1D_NANO(){

  
  RestFrames::SetStyle();

   std::map<string,long double> dataset_list_2016 = {
    {"/WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/10020533.0) * 1353.0 * 1.21 * 1.},
    {"/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/78043017.0) * 1345 * 1.21 * 1.2},
    {"/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/38984322.0) * 359.7 * 1.21 * 0.2},
    {"/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/7687800.0) * 48.91 * 1.21 * 0.22},
    {"/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/18687480.0) * 12.05 * 1.21 * 4.5},
    {"/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/7830536.0) * 5.501 * 1.21 * 0.67},
    {"/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/6872441.0) * 1.329 * 1.21 * 1.37},
    {"/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt", (1.0/2637821.0) * 0.03216 * 1.21 * 1.7},
   };

   std::map<string,long double> dataset_list_2017 = {
    {"/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/22201315.0) * 1353.0 * 1.21 * 1.},
    {"/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/35804623.0) * 1395.0 * 1.21 * 1.},
    {"/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/21192211.0) * 407.9 * 1.21 * 1.},
    {"/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/14250114.0) * 57.48 * 1.21 * 1.2},
    {"/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/21582309.0) * 12.87 * 1.21 * 2.},
    {"/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/19557162.0) * 5.366 * 1.21 * 1.3},
    {"/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/19991892.0) * 1.074 * 1.21 * 16.35},
    {"/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/20629585.0) * 0.008001 * 1.21 * 2.75},
   };

   std::map<string,long double> dataset_list_2018 = {
    {"/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/28054964.875751) * 1353.0 * 1.21 * 1.},
    {"/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/29385684.215082) * 1395.0 * 1.21 * 5.},
    {"/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/25415129.219721) * 407.9 * 1.21 * 1.},
    {"/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/5913597.776379) * 57.48 * 1.21 * 0.85},
    {"/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/19690762.318584) * 12.87 * 1.21 * 1.5},
    {"/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/8357921.274426) * 5.366 * 1.21 * 1.27},
    {"/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/7567071.151891) * 1.074 * 1.21 * 1.83},
    {"/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8.txt", (1.0/3189396.081144) * 0.008001 * 1.21 * 50.23},
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
