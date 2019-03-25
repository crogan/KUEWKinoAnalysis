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
#include "TBranch.h"
#include "TString.h"
#include "filterTools.h"
#include "TFileCollection.h"
using namespace std;

void filters_vs_MET(TString dataset){
  vector<TString> filter_names;
  TString draw_string="";
  TString sample ="";
  TChain* chain = new TChain("stopTreeMaker/AUX");









  cout << "finished initializing" << endl;


  
  //filter_names.push_back("noBadMuonsFilter");
  //filter_names.push_back("badMuonsFilter");
  //filter_names.push_back("duplicateMuonsFilter");

  

  //2017 dataset
  if(dataset == "dyJetsToLL"){
    TBranch *b_CSCTightHaloFilter;
    TBranch        *b_METFilters;   //!
    TBranch        *b_globalSuperTightHalo2016Filter;   //!
    TBranch        *b_goodVerticesFilter;   //!
    TBranch        *b_ecalBadCalibFilter;   //!
    TBranch        *b_HBHENoiseIsoFilter;   //!
    TBranch        *b_EcalDeadCellTriggerPrimitiveFilter;   //!
    TBranch        *b_BadChargedCandidateFilter;   //!
    TBranch        *b_BadPFMuonFilter;   //!
    TBranch        *b_HBHENoiseFilter;   //!
    TBranch        *b_HBHEIsoNoiseFilter; 
    TBranch        *b_met;   //!

    Float_t         met_f;
    Int_t           METFilters;
    Int_t           CSCTightHaloFilter;
    Int_t           globalSuperTightHalo2016Filter;
    Int_t           goodVerticesFilter;
    Int_t           ecalBadCalibFilter;
    Int_t           HBHENoiseIsoFilter;
    Int_t           EcalDeadCellTriggerPrimitiveFilter;
    UInt_t          BadChargedCandidateFilter;
    UInt_t          BadPFMuonFilter;
    UInt_t          HBHENoiseFilter;
    UInt_t          HBHEIsoNoiseFilter;



    chain->SetBranchAddress("met",&met_f,&b_met);
    chain->SetBranchAddress("METFilters", &METFilters, &b_METFilters);
    chain->SetBranchAddress("CSCTightHaloFilter", &CSCTightHaloFilter, &b_CSCTightHaloFilter);
    chain->SetBranchAddress("globalSuperTightHalo2016Filter", &globalSuperTightHalo2016Filter, &b_globalSuperTightHalo2016Filter);
    chain->SetBranchAddress("goodVerticesFilter", &goodVerticesFilter, &b_goodVerticesFilter);
    chain->SetBranchAddress("ecalBadCalibFilter", &ecalBadCalibFilter, &b_ecalBadCalibFilter);
    chain->SetBranchAddress("HBHENoiseIsoFilter", &HBHENoiseIsoFilter, &b_HBHENoiseIsoFilter);
    chain->SetBranchAddress("EcalDeadCellTriggerPrimitiveFilter", &EcalDeadCellTriggerPrimitiveFilter, &b_EcalDeadCellTriggerPrimitiveFilter);
    chain->SetBranchAddress("BadPFMuonFilter", &BadPFMuonFilter, &b_BadPFMuonFilter);
    chain->SetBranchAddress("HBHENoiseFilter", &HBHENoiseFilter, &b_HBHENoiseFilter);
    chain->SetBranchAddress("HBHEIsoNoiseFilter", &HBHEIsoNoiseFilter, &b_HBHEIsoNoiseFilter);
    chain->SetBranchAddress("BadChargedCandidateFilter", &BadChargedCandidateFilter, &b_BadChargedCandidateFilter);

    chain->SetBranchStatus("*",0);
    chain->SetBranchStatus("*Filter*",1);
    chain->SetBranchStatus("met",1);



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

    chain->AddFileInfoList((TCollection*)dyJetsToLL->GetList());
    sample = dyJetsToLL->GetName();
    cout << "sample: " << sample << endl;

  
    ////////////////////////////////////////////  
    // int Nentries = chain->GetEntries();
    // for(int i = 0; i < Nentries; i++){
    //   if(i%(Nentries/100)==0)
    //     cout << "Doing " << i << " out of " << Nentries << endl;
    //   chain->GetEntry(i);
    // }
    ////////////////////////////////////////////
  }

// use cut to get only entry numbers that pass cut, then use array of event numbers in GetEntry

// all call w chain
  // GetTreeNumber - with GetEntry
  // GetFile - returns pointer to TFile that is currently open
  // then use GetName, returns char* (also use w GetEntry)

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

    chain->AddFileInfoList((TCollection*)TChiToWZ->GetList());
    sample = TChiToWZ->GetName();
    cout << "sample: " << sample << endl;



   


  }
  else{
    cout << "error: not a valid dataset" << endl;
    return;
  }
  

  
  cout << "finished adding files to tchain" << endl;

  chain->Draw(">>list0","METFilters > 0");






  TCanvas *cv = new TCanvas("cv","cv");
  cv->Divide(3,3);

  for(int i = 0; i < filter_names.size(); i++){
    cv->cd(i+1);
    draw_string = "met:" + filter_names[i];  

    
    chain->Draw(draw_string,"met<1000","colz"); //set met cut at 1 TeV 
    gPad->SetLogz();   
  }







  //CMSmark();
  gStyle->SetOptTitle(kFALSE);
  gStyle->SetOptStat(0);
  cv->Update();
  cv->SaveAs(sample+".pdf");




}


