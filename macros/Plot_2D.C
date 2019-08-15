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
#include "../include/ReducedBase_slim.hh"
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

using namespace RestFrames;

void Plot_2D(){
  RestFrames::SetStyle();

  string StopNtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/StopNtuple_new/";

  int BKG_SKIP = 10;

  double CORRECT2 = 0.;
  double CORRECT = 0.;
  double TOTAL   = 0.;
  
  SampleSet ttX;
  ttX.SetBkg(true);
  ttX.SetTitle("t#bar{t} + X");
  ttX.SetColor(kAzure+1);
  ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
  // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
  // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17.root");
  ttX.SetSkip(10);
  g_Samples.push_back(&ttX);

  // SampleSet DYjets;
  // DYjets.SetBkg(true);
  // DYjets.SetTitle("Z/#gamma^{*} + jets");
  // DYjets.SetColor(kGreen-9);
  // DYjets.AddFile(StopNtuplePath+"All_Bkg_2017/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // DYjets.AddFile(StopNtuplePath+"All_Bkg_2017/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17.root");
  // //DYjets.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&DYjets);

  // SampleSet DB;
  // DB.SetBkg(true);
  // DB.SetTitle("DiBoson");
  // DB.SetColor(kOrange);
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/ZZTo2L2Nu_13TeV_powheg_pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_Fall17.root");
  // DB.AddFile(StopNtuplePath+"All_Bkg_2017/ZZTo4L_13TeV_powheg_pythia8_Fall17.root");
  // DB.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&DB);

  // SampleSet Wjets;
  // Wjets.SetBkg(true);
  // Wjets.SetTitle("W + jets");
  // Wjets.SetColor(kRed);
  // Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // Wjets.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&Wjets);

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{#chi}^{0}_{2} #tilde{#chi}^{#pm}_{1} #rightarrow Z #tilde{#chi}^{0}_{1} W #tilde{#chi}^{0}_{1}; m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 150 GeV");
  // SIG1.SetTreeName("SMS_300_250");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  int Nsample = g_Samples.size();
  g_PlotTitle = g_Samples[0]->GetTitle();
  
  //string g_Label = "No selection";
  string g_Label = "Region D";


  g_Xname = "N_{lep}^{S}";
  g_Xmin = 0.0;
  g_Xmax = 180.; 
  g_NX = 32;
  g_Yname = "N_{jet}^{S}";
  g_Ymin = 0.;
  g_Ymax = 180.;
  g_NY = 32;

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
     
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

	if(base->Nlep != 2)
	  continue;

	if(base->Njet_S->at(1) != 2)
	  continue;

	if(base->ID_lep->at(0) < 3 || base->ID_lep->at(1) < 3)
	  continue;

	if(base->MiniIso_lep->at(0) > 0.1 || base->MiniIso_lep->at(1) > 0.1)
	  continue;

	if(base->Nlep_a->at(1) != 2)
	  continue;

	if(base->Njet_b->at(1) != 2)
	  continue;

	if(base->PTISR->at(1) < 200)
	  continue;

	hist->Fill(base->EVa->at(1), base->EVb->at(1), base->weight*double(SKIP));
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
  hist->GetZaxis()->SetRangeUser(0.9*hist->GetMinimum(0.0),1.1*hist->GetMaximum());

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  // l.DrawLatex(0.17,0.855,g_PlotTitle.c_str());
  l.DrawLatex(0.41,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.01,0.943,"#bf{CMS} Simulation Preliminary");

  // l.SetTextSize(0.04);
  // l.SetTextFont(132);
  // l.DrawLatex(0.74,0.04,g_Label.c_str());


}
