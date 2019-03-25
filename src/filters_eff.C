#include <iostream>
//#include "AUX.h" //makeclass for stopntuple
//#include "TLorentzVector.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TPad.h"
#include "TString.h"
#include "filterTools.h"
using namespace std;

void filters_eff(TString dataset, TString metcut="", bool pass = true){
  vector<TString> filter_names;
  TString sample ="";
  TString cutstring="";
  TString filtercuts="";
  int tree_Nentries = -999;
  int sel_Nentries = -999;
  float tree_entries = -999.;
  float sel_entries = -999.;
  float eff = -999.;
  float met_entries = -999.;
  int met_Nentries = -999;
  TChain* chain = new TChain("stopTreeMaker/AUX");


  cout << "finished initializing" << endl;


  
  //filter_names.push_back("noBadMuonsFilter");
  //filter_names.push_back("badMuonsFilter");
  //filter_names.push_back("duplicateMuonsFilter");

  
  //2017 dataset
  if(dataset == "dyJetsToLL"){
    filter_names.push_back("globalSuperTightHalo2016Filter");
    filter_names.push_back("goodVerticesFilter");
    filter_names.push_back("EcalDeadCellTriggerPrimitiveFilter");
    filter_names.push_back("BadChargedCandidateFilter");
    filter_names.push_back("BadPFMuonFilter");
    filter_names.push_back("HBHENoiseFilter");
    filter_names.push_back("HBHEIsoNoiseFilter");
    filter_names.push_back("CSCTightHaloFilter");
    filter_names.push_back("METFilters");

    TFileCollection *dyJetsToLL= new TFileCollection("dyJetsToLL","dyJetsToLL");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220245/0000/stopFlatNtuples_*");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220311/0000/stopFlatNtuples_*");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220337/0000/stopFlatNtuples_*");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220403/0000/stopFlatNtuples_*");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220428/0000/stopFlatNtuples_*");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220454/0000/stopFlatNtuples_*");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220520/0000/stopFlatNtuples_*");
    dyJetsToLL->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/QCD/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8/crab_DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8RunIIFall17MiniAODv2/190201_220546/0000/stopFlatNtuples_*");

    chain->AddFileInfoList(dyJetsToLL->GetList());
    sample = dyJetsToLL->GetName();
    cout << "sample: " << sample << endl;
  }

  else if(dataset == "TChiToWZ"){
    filter_names.push_back("globalSuperTightHalo2016Filter");
    filter_names.push_back("goodVerticesFilter");
    filter_names.push_back("EcalDeadCellTriggerPrimitiveFilter");
    filter_names.push_back("BadChargedCandidateFilter");
    filter_names.push_back("BadPFMuonFilter");
    filter_names.push_back("HBHENoiseIsoFilter");
    filter_names.push_back("CSCTightHaloFilter");
    filter_names.push_back("METFilters");

    //2016 dataset
    TFileCollection *TChiToWZ = new TFileCollection("TChiToWZ","TChiToWZ");
    TChiToWZ->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/Signal/SMS-TChiWZ_ZToLL_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_SMS-TChiWZ_ZToLL_TuneCUETP8M1_13TeV-madgraphMLM-pythia8RunIISpring16MiniAODv2/181220_184342/0000/stopFlatNtuples_*");
    TChiToWZ->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/Signal/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8RunIISummer16MiniAODv2/181220_184216/0000/stopFlatNtuples_*");
    TChiToWZ->Add("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/jaking/Ewkinos/Signal/SMS-TChiWZ_ZToLL_mZMin-0p1_mLSP300to350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_SMS-TChiWZ_ZToLL_mZMin-0p1_mLSP300to350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8RunIISummer16MiniAODv2/181220_184151/0000/stopFlatNtuples_*");
    cout << "finished adding files to file list" << endl;

    chain->AddFileInfoList(TChiToWZ->GetList());
    sample = TChiToWZ->GetName();
    cout << "sample: " << sample << endl;
  }
  else{
    cout << "error: not a valid dataset" << endl;
    return;
  }
  

  
  cout << "finished adding files to tchain" << endl;

  if(metcut == ""){
    tree_Nentries = chain->GetEntries();
    cout << "no met cut, all entries" << endl;
    cout << "total entries: " << tree_Nentries << endl;
    tree_entries = (float)tree_Nentries;
    cout << "total float entries: " << tree_entries << endl;
  }

  else{
    cout << "met cut: " << metcut << endl;
    chain->Draw(">>list0",metcut);
    TEventList *list0 = (TEventList*)gDirectory->Get("list0"); //list of selected events
    met_Nentries = list0->GetN();
    met_entries = (float)met_Nentries;
    cout << "total entries with met cut: " << met_entries << endl;
  for(int i = 0; i < filter_names.size(); i++){
    if(pass == true){
      if(0 <= i && i < filter_names.size() - 2){
        cout << "loop #: " << i << endl;
        filtercuts = filtercuts + filter_names[i] + " > 0 && ";
        cout << filtercuts << endl;
      }
      if(i == filter_names.size() - 2){
        cout << "loop #: " << i << endl;
        filtercuts = filtercuts + filter_names[i] + "> 0 ";
        cutstring = filtercuts +  "&&" + metcut;
        cout << cutstring << endl;
        chain->Draw(">>list1",cutstring);
        TEventList *list = (TEventList*)gDirectory->Get("list1"); //list of selected events
        sel_Nentries = list->GetN();
        sel_entries = (float)sel_Nentries;
        cout << "passed entries: " << sel_entries << endl;
        eff = sel_entries / met_entries;
        //cout << "filter: " << filter_names[i] << endl;
        cout << "efficiency: " << eff << endl; 
        cout << "\n" << endl;
      }
    }
    else if(pass == false){
      if(0 <= i && i < filter_names.size() - 2){
        cout << "loop #: " << i << endl;
        filtercuts = filtercuts + filter_names[i] + " < 1 && ";
        cout << filtercuts << endl;
      }
      if(i == filter_names.size() - 2){
        cout << "loop #: " << i << endl;
        filtercuts = filtercuts + filter_names[i] + "< 1 ";
        cutstring = filtercuts +  "&&" + metcut;
        cout << cutstring << endl;
        chain->Draw(">>list1",cutstring);
        TEventList *list = (TEventList*)gDirectory->Get("list1"); //list of selected events
        sel_Nentries = list->GetN();
        sel_entries = (float)sel_Nentries;
        cout << "failed entries: " << sel_entries << endl;
        eff = sel_entries / met_entries;
        //cout << "filter: " << filter_names[i] << endl;
        cout << "efficiency: " << eff << endl; 
        cout << "\n" << endl;
      }    
    }
  }  
}  





}