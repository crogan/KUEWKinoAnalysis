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

#include "../include/mt2_bisect.hh"

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

  //string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_12_05_20/";
  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_23_05_20/";

  int BKG_SKIP = 1;
  
  // SampleSet ttX;
  // ttX.SetBkg(true);
  // ttX.SetTitle("t #bar{t} + jets");
  // ttX.SetColor(kAzure+1);
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&ttX);

  // SampleSet ST;
  // ST.SetBkg(true);
  // ST.SetTitle("Single Top");
  // ST.SetColor(kAzure+1);
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8_Fall17_102X.root");
  // ST.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&ST);

  // SampleSet DYjets;
  // DYjets.SetBkg(true);
  // DYjets.SetTitle("Z/#gamma * + jets");
  // DYjets.SetColor(kGreen-9);
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-600toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&DYjets);

  // SampleSet DB;
  // DB.SetBkg(true);
  // DB.SetTitle("DiBoson");
  // DB.SetColor(kOrange);
  // DB.AddFile(NtuplePath+"Fall17_102X/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_v2_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2L2Nu_13TeV_powheg_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo4L_13TeV_powheg_pythia8_Fall17_102X.root");
  // DB.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&DB);

  // SampleSet Wjets;
  // Wjets.SetBkg(true);
  // Wjets.SetTitle("W + jets");
  // Wjets.SetColor(kRed);
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&Wjets);

  // SampleSet Znunu;
  // Znunu.SetBkg(true);
  // Znunu.SetTitle("Z(#nu #nu) + jets");
  // Znunu.SetColor(kRed);
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Fall17_102X.root");
  // Znunu.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&Znunu);


  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{#chi}^{+}_{1} #tilde{#chi}^{-}_{1} #rightarrow W(l#nu) #tilde{#chi}^{0}_{1} W(l#nu) #tilde{#chi}^{0}_{1}; m_{#chi^{#pm}_{1}} = 250, m_{#chi^{0}_{1}} = 240 GeV");
  // SIG1.SetTreeName("SMS_250_240");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);
  
  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{#chi}^{0}_{2} #tilde{#chi}^{#pm}_{1} #rightarrow Z #tilde{#chi}^{0}_{1} W #tilde{#chi}^{0}_{1}; m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 300, m_{#chi^{0}_{1}} = 290 GeV");
  // SIG1.SetTreeName("SMS_300_290");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{l} #tilde{l} #rightarrow l #tilde{#chi}^{0}_{1} l #tilde{#chi}^{0}_{1}; m_{ #tilde{l}} = 300, m_{#tilde{#chi}^{0}_{1}} = 290 GeV");
  // SIG1.SetTreeName("SMS_300_290");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // SIG1.SetScale(2.8);
  // g_Samples.push_back(&SIG1);
  
  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#scale[0.8]{#tilde{t} #tilde{t} #rightarrow b f #bar{f} #tilde{#chi}^{0}_{1} b f #bar{f} #tilde{#chi}^{0}_{1}}; m_{t} = 500, m_{#chi^{0}_{1}} = 490 GeV");
  // SIG1.SetTreeName("SMS_500_490");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2-4bd_genMET-80_mStop-500_mLSP-420_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("#scale[0.8]{#tilde{t} #tilde{t} #rightarrow b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1}) b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1})}; m_{t} = 700, m_{#chi^{0}_{1}} = 680 GeV");
  SIG1.SetTreeName("SMS_700_680");
  SIG1.SetColor(kMagenta);
  SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);

  int Nsample = g_Samples.size();
  g_PlotTitle = g_Samples[0]->GetTitle();
  
  //string g_Label = "No selection";
  string g_Label = "1 lepton";


  //g_Xname = "E_{lep #scale[0.8]{#perp}}^{ S}  [GeV]";
  //g_Xname = "#bar{M}_{#tilde{#chi_{2}} #scale[0.8]{#perp}}  [GeV]";
  // g_Xname = "p_{T}^{lep} [GeV]";
  g_Xname = "M_{#perp}    [GeV]";
  g_Xmin = 0.;
  g_Xmax = 60; 
  g_NX = 32;
  g_Yname = "R_{ISR}";
  //g_Yname = "#gamma_{T}";
  //g_Yname = "p_{T}^{ISR}";
  g_Ymin = 0.6;
  g_Ymax = 1.02;
  g_NY = 32;

  // g_Xname = "R_{ISR}";
  // g_Xmin = -1.;
  // g_Xmax = 1.; 
  // g_NX = 32;
  // g_Yname = "p_{T}^{ISR} [GeV]";
  // g_Ymin = -1.;
  // g_Ymax = 1.;
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
     
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

	///////////// 1 lepton
	// if(base->Nlep != 1)
	//   continue;

	// if(base->ID_lep->at(0) < 3)
	//   continue;

	// if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 4.)
	//   continue;

	// if(base->SIP3D_lep->at(0) > 3)
	//   continue;

	// g_Label = "1L Region";
	// if(base->Njet_S->at(1) != 0)
	//   continue;
	// if(base->NSV_S->at(1) != 0)
	//   continue;

	// g_Label = "1L-1SV Region";
	// if(base->Njet_S->at(1) != 0)
	//   continue;
	// if(base->NSV_S->at(1) != 1)
	//   continue;
	/////////////////
	
	// 2 lepton
	if(base->Nlep != 2)
	  continue;

	if(base->ID_lep->at(0) < 3 ||
	   base->ID_lep->at(1) < 3)
	  continue;

	if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 5. ||
	   base->MiniIso_lep->at(1)*base->PT_lep->at(1) > 5.)
	  continue;

	if(base->SIP3D_lep->at(0) > 4 ||
	   base->SIP3D_lep->at(1) > 4)
	  continue;

	// if(base->Charge_lep->at(0)+base->Charge_lep->at(1) != 0)
	//   continue;

	// TVector3 ETMiss;
	// ETMiss.SetPtEtaPhi(base->MET, 0., base->MET_phi);

	// TVector3 lep;
	// lep.SetPtEtaPhi(base->PT_lep->at(0),base->Eta_lep->at(0),base->Phi_lep->at(0));
	// double dphiML = fabs(lep.DeltaPhi(ETMiss));

		/////////////// 3 lepton
	// if(base->Nlep != 3)
	//   continue;

	// if(base->ID_lep->at(0) < 3 ||
	//    base->ID_lep->at(1) < 3 ||
	//    base->ID_lep->at(2) < 3)
	//   continue;

	// if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 6. ||
	//    base->MiniIso_lep->at(1)*base->PT_lep->at(1) > 6. ||
	//    base->MiniIso_lep->at(2)*base->PT_lep->at(2) > 6.)
	//   continue;

	// if(base->SIP3D_lep->at(0) > 4 ||
	//    base->SIP3D_lep->at(1) > 4 ||
	//    base->SIP3D_lep->at(2) > 4)
	//   continue;
	//////////////////////////
       
	
	TLorentzVector lep1, lep2;
	TVector3 MET;

	lep1.SetPtEtaPhiM(base->PT_lep->at(0),base->Eta_lep->at(0),base->Phi_lep->at(0),base->M_lep->at(0));
	lep2.SetPtEtaPhiM(base->PT_lep->at(1),base->Eta_lep->at(1),base->Phi_lep->at(1),base->M_lep->at(1));
	MET.SetPtEtaPhi(base->MET,0.,base->MET_phi);

	double dphiML = std::max(fabs(lep1.Vect().DeltaPhi(MET)),fabs(lep2.Vect().DeltaPhi(MET)));

	// double MT2 = Calc_MT2(lep1,lep2,MET);

	g_Label = "di-lepton selection";
	if(base->Njet_S != 0)
	  continue;
	if(base->NSV_S != 0)
	  continue;
	/////////////////
       

	 
	// if(base->MET < 200)
	//   continue;
	
	if(base->PTISR < 200.)
	  continue;

	if(base->RISR < 0.6)
	  continue;

	// if(base->Nbjet_ISR->at(1) > 0)
	//   continue;

	// if(base->index_SV_b->size() < 2)
	//   continue;
	// if(base->index_SV_b->at(1).size() < 1)
	//   continue;
	// if(base->NSV <= base->index_SV_b->at(1)[0])
	//   continue;
	// double etaSV = base->Eta_SV->at(base->index_SV_b->at(1)[0]);
	// double phiSV = base->Phi_SV->at(base->index_SV_b->at(1)[0]);
	// TVector3 SV;
	// SV.SetPtEtaPhi(1,etaSV,phiSV);
	// double dphiSVM = fabs(SV.DeltaPhi(ETMiss));

	// if(dphiML > 1.)
	//   continue;

	// if(base->NSV_S->at(1) > 0)
	//   continue;

	// if(base->NSV_S->at(1) < 1)
	//   continue;

	// if(base->MS->at(1) > 80.)
	//   continue;
	
	// if(base->Njet_a->at(1)+base->NSV_a->at(1) != 0)
	//   continue;

	// if(base->Njet_b->at(1)+base->NSV_b->at(1) != 1)
	//   continue;




	double weight = (base->weight != 0.) ? base->weight : 1.;

	// if(base->RISR->at(1) < 0.975)
	//   continue;

	
	hist->Fill(base->Mperp, base->RISR, weight*double(SKIP)*g_Samples[s]->GetScale());
      }
      
      delete base;
      delete chain;
    }
  }

  cout << "Total " << hist->Integral()*137. << endl;
  
  hist->Scale(137.);
  
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
  hist->GetZaxis()->SetTitleSize(0.055);
  hist->GetZaxis()->SetTitleOffset(1.05);
  hist->GetZaxis()->SetLabelFont(42);
  hist->GetZaxis()->SetLabelSize(0.05);
  hist->GetZaxis()->SetTitle("N_{events} / 137 fb^{-1}");
  hist->GetZaxis()->SetRangeUser(0.9*hist->GetMinimum(0.0),1.1*hist->GetMaximum());

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  // l.DrawLatex(0.17,0.855,g_PlotTitle.c_str());
  l.DrawLatex(0.71,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.01,0.943,"#bf{CMS} Simulation Preliminary");

  l.SetTextSize(0.045);
  l.SetTextFont(42);
  l.DrawLatex(0.7,0.04,g_Label.c_str());


}
