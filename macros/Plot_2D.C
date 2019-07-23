#include <iostream>
#include <string>

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
#include <TH2D.h>
#include <TStyle.h>

#include "RestFrames/RestFrames.hh"
#include "../include/ReducedBase_new.hh"
#include "../include/SampleSet.hh"

using namespace std;


vector<SampleSet*> g_Samples;

string g_PlotTitle;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;
string g_Yname;
double g_Ymin;
double g_Ymax;
double g_NY;
double units_per_bin_x;
double units_per_bin_y;


using namespace RestFrames;

void Plot_2D(){
  RestFrames::SetStyle();

  string StopNtuplePath = "/home/t3-ku/crogan/NTUPLES/StopNtuple_new/";
  
  int BKG_SKIP = 10; //takes 1 in every BKG_SKIP events


//   SampleSet ttX;
//   ttX.SetBkg(true);
//   ttX.SetTitle("t#bar{t} + X");
//   ttX.SetColor(kAzure+1);
//   ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_TuneCP5.root");
//   // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
//   // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
//   // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17.root");
//   ttX.SetSkip(10);
//   g_Samples.push_back(&ttX);

SampleSet DYjets;
  DYjets.SetBkg(true);
  DYjets.SetTitle("DY + jets");
  DYjets.SetColor(kGreen-7);
  DYjets.AddFile(StopNtuplePath+"All_Bkg_2017/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  DYjets.AddFile(StopNtuplePath+"All_Bkg_2017/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17.root");
  DYjets.SetSkip(BKG_SKIP);
  g_Samples.push_back(&DYjets);

//   SampleSet Wjets;
//   Wjets.SetBkg(true);
//   Wjets.SetTitle("W + jets");
//   Wjets.SetColor(kRed-7);
//   Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
//   Wjets.SetSkip(BKG_SKIP);
//   g_Samples.push_back(&Wjets);

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 197");
  // SIG1.SetTreeName("SMS_200_197");
  // SIG1.SetColor(kBlue-7);
  // SIG1.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);
 
  // SampleSet SIG2;
  // SIG2.SetBkg(false);
  // SIG2.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 170");
  // SIG2.SetTreeName("SMS_200_170");
  // SIG2.SetColor(kMagenta-7);
  // SIG2.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  // SIG2.SetSkip(1);
  // g_Samples.push_back(&SIG2);


  // SampleSet SIG3;
  // SIG3.SetBkg(false);
  // SIG3.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 1");
  // SIG3.SetTreeName("SMS_200_1");
  // SIG3.SetColor(kViolet-7);
  // SIG3.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  // SIG3.SetSkip(1);
  // g_Samples.push_back(&SIG3);

  int Nsample = g_Samples.size();
  //string g_Label = "No selection";
  string g_Label = "RISR vs. pT_lep, Cat. 2 (bkg:dy+jets)";


  g_Yname = "pT_lep (GeV)";
  g_Ymin = 0.0;
  g_Ymax = 200.;
  units_per_bin_y = 1.0;
  g_NY = (int)(g_Ymax - g_Ymin)/units_per_bin_y;


  g_Xname = "pT_ISR (GeV)";
  g_Xmin = 0.0;
  g_Xmax = 1500.; 
  units_per_bin_x = 10.;
  g_NX = (int)(g_Xmax - g_Xmin)/units_per_bin_x;


  // g_Xname = "RISR";
  // g_Xmin = 0.0;
  // g_Xmax = 1.3; 
  // units_per_bin_x = 0.01;
  // g_NX = (int)(g_Xmax - g_Xmin)/units_per_bin_x;

  // g_Xname = "Njets_S";
  // g_Xmin = 0.0;
  // g_Xmax = 5.0; 
  // units_per_bin_x = 1.0;
  // g_NX = (int)(g_Xmax - g_Xmin)/units_per_bin_x;

  // g_Yname = "Njets_ISR";
  // g_Ymin = 0.0;
  // g_Ymax = 5.0;
  // units_per_bin_y = 1.0;
  // g_NY = (int)(g_Ymax - g_Ymin)/units_per_bin_y;

  // g_Xname = "N_{lep}^{S}";
  // g_Xmin = 0.4;
  // g_Xmax = 1.1; 
  // g_NX = 32;

  // g_Yname = "N_{jet}^{S}";
  // g_Ymin = 0.;
  // g_Ymax = 180.;
  // g_NY = 32;

  int TREE = 2;

  TH2D* hist = new TH2D("hist","hist",
			g_NX,g_Xmin,g_Xmax,
			g_NY,g_Ymin,g_Ymax);


  
  for(int s = 0; s < Nsample; s++){
    
    int Nfile = g_Samples[s]->GetNFile();
    cout << "Processing " << Nfile << " files for sample " << g_Samples[s]->GetTitle() << endl;
    for(int f = 0; f < Nfile; f++){
      cout << "   Processing file " << g_Samples[s]->GetFile(f) << " w/ tree " << g_Samples[s]->GetTreeName() << endl;
    
      TChain* chain = new TChain(g_Samples[s]->GetTreeName().c_str());
      chain->Add(g_Samples[s]->GetFile(f).c_str());

      ReducedBase* base = new ReducedBase(chain);

      int Nentry = base->fChain->GetEntries();

      int SKIP = g_Samples[s]->GetSkip();
      //for(int e = 0; e < 10; e += SKIP){
	for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

	if(base->Nlep != 1) //single lepton final states
	  continue;

  if(base->MiniIso_lep->at(0) > 0.1) //miniIso cut
    continue;

	
	  hist->Fill(base->PTISR->at(1), base->PT_lep->at(0), base->weight*double(SKIP));
      }

      delete base;
      delete chain;
    }
  }

 
  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
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
  hist->SetTitle("");
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
  hist->GetYaxis()->SetTitle(g_Yname.c_str());
  hist->GetZaxis()->CenterTitle();
  hist->GetZaxis()->SetTitleFont(42);
  hist->GetZaxis()->SetTitleSize(0.06);
  hist->GetZaxis()->SetTitleOffset(1.1);
  hist->GetZaxis()->SetLabelFont(42);
  hist->GetZaxis()->SetLabelSize(0.05);
  hist->GetZaxis()->SetTitle("a. u.");
  hist->GetZaxis()->SetRangeUser(1E-3,1.1*hist->GetMaximum()); //0.9*hist->GetMinimum(0.0)

  // TLatex l;
  // l.SetTextFont(42);
  // l.SetNDC();
  // l.SetTextSize(0.035);
  // l.SetTextFont(42);
  // l.DrawLatex(0.41,0.943,g_Label.c_str());
  // l.SetTextSize(0.04);
  // l.SetTextFont(42);
  // l.DrawLatex(0.01,0.943,"#bf{#it{CMS}} Internal 13 TeV Simulation");

  TLatex l;
  l.SetTextFont(132);
  l.SetNDC();
  l.SetTextSize(0.05);
  l.SetTextFont(132);
  l.DrawLatex(0.42,0.94,g_Label.c_str());
  l.SetTextSize(0.03);
  l.SetTextFont(42);
  l.DrawLatex(0.145,0.92,"#bf{#it{CMS}} Internal 13 TeV Simulation");


  // l.SetTextSize(0.04);
  // l.SetTextFont(132);
  // l.DrawLatex(0.74,0.04,g_Label.c_str());


  // SampleSet sample;
  // sample.write_plot("output/2Dplots.root",g_Label, can);
  TString file_name = "output/2D_plots/"+g_Label+"miniIso.root";
  TFile* file = new TFile(file_name,"RECREATE");
  file->cd();
  can->Write();


}
