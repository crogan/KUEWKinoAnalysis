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

void Plot_dev_2D(){
  RestFrames::SetStyle();

  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_10_01_20/";
  

  int BKG_SKIP = 11;
  
  // SampleSet ttX;
  // ttX.SetBkg(true);
  // ttX.SetTitle("t#bar{t} + X");
  // ttX.SetColor(kAzure+1);
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&ttX);

  SampleSet DYjets;
  DYjets.SetBkg(true);
  DYjets.SetTitle("Z/#gamma^{*} + jets");
  DYjets.SetColor(kGreen-9);
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-600toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  DYjets.SetSkip(BKG_SKIP);
  g_Samples.push_back(&DYjets);

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

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{#chi}^{0}_{2} #tilde{#chi}^{#pm}_{1} #rightarrow Z #tilde{#chi}^{0}_{1} W #tilde{#chi}^{0}_{1}; m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 300, m_{#chi^{0}_{1}} = 280 GeV");
  // SIG1.SetTreeName("SMS_300_260");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{#chi}^{0}_{2} #tilde{#chi}^{#pm}_{1} #rightarrow Z #tilde{#chi}^{0}_{1} W #tilde{#chi}^{0}_{1}; m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 300, m_{#chi^{0}_{1}} = 260 GeV");
  // SIG1.SetTreeName("SMS_300_250");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);
  
  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{t} #tilde{t} #rightarrow b f #bar{f} #tilde{#chi}^{0}_{1} b f #bar{f} #tilde{#chi}^{0}_{1}; m_{t} = 500, m_{#chi^{0}_{1}} = 420 GeV");
  // SIG1.SetTreeName("SMS_500_420");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2-4bd_genMET-80_mStop-500_mLSP-420_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#tilde{t} #tilde{t} #rightarrow b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1}) b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1}); m_{t} = 500, m_{#chi^{0}_{1}} = 480 GeV");
  // SIG1.SetTreeName("SMS_500_480");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  int Nsample = g_Samples.size();
  g_PlotTitle = g_Samples[0]->GetTitle();
  
  //string g_Label = "No selection";
  string g_Label = "1 lepton";


  g_Xname = "R_{ISR}";
  g_Xmin = 0.;
  g_Xmax = 1.0; 
  g_NX = 32;
  g_Yname = "p_{T}^{ISR} [GeV]";
  g_Ymin = 0.;
  g_Ymax = 75.;
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

  // RestFrames setup
  LabRecoFrame LAB("LAB","LAB");
  DecayRecoFrame CM("CM","CM");
  DecayRecoFrame S("S","#tilde{S}");
  DecayRecoFrame X3a("X3a","#tilde{#chi}_{3a}");
  DecayRecoFrame X3b("X3b","#tilde{#chi}_{3b}");
  DecayRecoFrame X2a("X2a","#tilde{#chi}_{2a}");
  DecayRecoFrame X2b("X2b","#tilde{#chi}_{2b}");
  
  VisibleRecoFrame ISR("ISR","ISR");
  VisibleRecoFrame Ja("Ja","jets_{a}");
  VisibleRecoFrame Jb("Jb","jets_{b}");
  VisibleRecoFrame La("La","leps_{a}");
  VisibleRecoFrame Lb("Lb","leps_{b}");
  InvisibleRecoFrame X1a("X1a","#tilde{#chi}_{1a}");
  InvisibleRecoFrame X1b("X1b","#tilde{#chi}_{1b}");

  LAB.SetChildFrame(CM);
  CM.AddChildFrame(S);
  CM.AddChildFrame(ISR);
  S.AddChildFrame(X3a);
  S.AddChildFrame(X3b);
  X3a.AddChildFrame(X2a);
  X3b.AddChildFrame(X2b);
  X3a.AddChildFrame(Ja);
  X3b.AddChildFrame(Jb);
  X2a.AddChildFrame(X1a);
  X2b.AddChildFrame(X1b);
  X2a.AddChildFrame(La);
  X2b.AddChildFrame(Lb);
 
  if(!LAB.InitializeTree()){
    cout << "Problem initializing tree" << endl;
  }
    
  InvisibleGroup INV("INV","Invisible System");
  INV.AddFrame(X1a);
  INV.AddFrame(X1b);
    
  SetMassInvJigsaw InvM("InvM_%d", "Set inv. system mass");
  INV.AddJigsaw(InvM);

  SetRapidityInvJigsaw InvEta("InvEta_%d", "Set inv. system rapidity");
  INV.AddJigsaw(InvEta);
  InvEta.AddVisibleFrames(S.GetListVisibleFrames());
    
  MinMassesSqInvJigsaw InvSplit("InvSplit", "INV -> #tilde{#chi_{1a}}+ #tilde{#chi_{1b}}", 2);
  INV.AddJigsaw(InvSplit);
  InvSplit.AddVisibleFrame(Ja, 0);
  InvSplit.AddVisibleFrame(Jb, 1);
  InvSplit.AddVisibleFrame(La, 0);
  InvSplit.AddVisibleFrame(Lb, 1);
  InvSplit.AddInvisibleFrame(X1a, 0);
  InvSplit.AddInvisibleFrame(X1b, 1);

  if(!LAB.InitializeAnalysis())
    cout << "Problem initializing analysis tree" << endl;

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
	if(base->Nlep != 1)
	  continue;

	if(base->ID_lep->at(0) < 3)
	  continue;

	if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 6.)
	  continue;

	if(base->SIP3D_lep->at(0) > 4)
	  continue;
	/////////////////
	
	///////////// 2 lepton
	// if(base->Nlep != 2)
	//   continue;

	// if(base->ID_lep->at(0) < 3 ||
	//    base->ID_lep->at(1) < 3)
	//   continue;

	// if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 6. ||
	//    base->MiniIso_lep->at(1)*base->PT_lep->at(1) > 6.)
	//   continue;

	// if(base->SIP3D_lep->at(0) > 4 ||
	//    base->SIP3D_lep->at(1) > 4)
	// continue;
	/////////////////
	
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

	 
	if(base->MET < 200)
	  continue;
	
	if(base->PTISR->at(1) < 250)
	  continue;

	if(base->RISR->at(1) < 0.8)
	  continue;

	if(base->Nbjet_ISR->at(1) > 0)
	  continue;
	
	if(base->Njet_S->at(1)+base->NSV_S->at(1) != 1)
	  continue;

	// if(base->NSV_S->at(1) > 0)
	//   continue;

	// if(base->NSV_S->at(1) < 1)
	//   continue;

	// if(base->MS->at(1) > 80.)
	//   continue;
	
	if(base->Njet_a->at(1)+base->NSV_a->at(1) != 0)
	  continue;

	if(base->Nbjet_b->at(1) != 1)
	  continue;
	if(base->NSV_b->at(1) != 0)
	  continue;
	

	// if(base->Nlep_a->at(1) != 2)
	//   continue;

	// RestFrames stuff
	LAB.ClearEvent();

	TVector3 ETMiss;
	ETMiss.SetPtEtaPhi(base->MET, 0., base->MET_phi);
	INV.SetLabFrameThreeVector(ETMiss);

	// if(base->Njet_a->at(1) != int(base->index_jet_a->at(1).size()))
	//   cout << "jet a " << endl;
	// if(base->Njet_b->at(1) != int(base->index_jet_b->at(1).size()))
	//   cout << "jet b" << endl;
	// if(base->NSV_a->at(1) != int(base->index_SV_a->at(1).size()))
	//   cout << "SV a" << endl;
	// if(base->NSV_b->at(1) != int(base->index_SV_b->at(1).size()))
	//   cout << "SV b" << endl;
	// if(base->Nlep_a->at(1) != int(base->index_lep_a->at(1).size()))
	//   cout << "lep a" << endl;
	// if(base->Nlep_b->at(1) != int(base->index_lep_b->at(1).size()))
	//   cout << "lep b" << endl;

	if(base->Njet_ISR->size() < 2)
	  continue;
	if(base->Njet_a->size() < 2)
	  continue;
	if(base->Njet_b->size() < 2)
	  continue;
	if(base->Nlep_a->size() < 2)
	  continue;
	if(base->Nlep_b->size() < 2)
	  continue;
	if(base->NSV_a->size() < 2)
	  continue;
	if(base->NSV_b->size() < 2)
	  continue;

	double dphiJMETmin = acos(-1.);
	for(int i = 0; i < base->Njet && i < 3; i++){
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_jet->at(i),base->Eta_jet->at(i),base->Phi_jet->at(i),base->M_jet->at(i));
	  double dphi = ETMiss.DeltaPhi(part.Vect());
	  if(dphi < dphiJMETmin)
	    dphiJMETmin = dphi;
	}
	
	TLorentzVector vISR(0.,0.,0.,0.);
	for(int i = 0; i < base->Njet_ISR->at(1); i++){
	  int index = base->index_jet_ISR->at(1)[i];
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_jet->at(index),base->Eta_jet->at(index),base->Phi_jet->at(index),base->M_jet->at(index));
	  vISR += part;
	}
	ISR.SetLabFrameFourVector(vISR);
	
	TLorentzVector vLa(0.,0.,0.,0.);
	for(int i = 0; i < base->Nlep_a->at(1); i++){
	  int index = base->index_lep_a->at(1)[i];
	   if(index >= base->Nlep)
	    cout << "asdfdf" << endl;
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_lep->at(index),base->Eta_lep->at(index),base->Phi_lep->at(index),base->M_lep->at(index));
	  vLa += part;
	}
	La.SetLabFrameFourVector(vLa);
	TLorentzVector vLb(0.,0.,0.,0.);
	for(int i = 0; i < base->Nlep_b->at(1); i++){
	  int index = base->index_lep_b->at(1)[i];
	   if(index >= base->Nlep)
	    cout << "asdfdf" << endl;
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_lep->at(index),base->Eta_lep->at(index),base->Phi_lep->at(index),base->M_lep->at(index));
	  vLb += part;
	}
	Lb.SetLabFrameFourVector(vLb);
	
	TLorentzVector vJa(0.,0.,0.,0.);
	for(int i = 0; i < base->Njet_a->at(1); i++){
	  int index = base->index_jet_a->at(1)[i];
	   if(index >= base->Njet)
	    cout << "asdfdf" << endl;
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_jet->at(index),base->Eta_jet->at(index),base->Phi_jet->at(index),base->M_jet->at(index));
	  vJa += part;
	}
	for(int i = 0; i < base->NSV_a->at(1); i++){
	  int index = base->index_SV_a->at(1)[i];
	   if(index >= base->NSV)
	    cout << "asdfdf" << endl;
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_SV->at(index),base->Eta_SV->at(index),base->Phi_SV->at(index),base->M_SV->at(index));
	  vJa += part;
	}
	Ja.SetLabFrameFourVector(vJa);
	TLorentzVector vJb(0.,0.,0.,0.);
	for(int i = 0; i < base->Njet_b->at(1); i++){
	  int index = base->index_jet_b->at(1)[i];
	   if(index >= base->Njet)
	    cout << "asdfdf" << endl;
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_jet->at(index),base->Eta_jet->at(index),base->Phi_jet->at(index),base->M_jet->at(index));
	  vJb += part;
	}
	for(int i = 0; i < base->NSV_b->at(1); i++){
	  int index = base->index_SV_b->at(1)[i];
	   if(index >= base->NSV)
	    cout << "asdfdf" << endl;
	  TLorentzVector part;
	  part.SetPtEtaPhiM(base->PT_SV->at(index),base->Eta_SV->at(index),base->Phi_SV->at(index),base->M_SV->at(index));
	  vJb += part;
	}
	Jb.SetLabFrameFourVector(vJb);

	if(!LAB.AnalyzeEvent())
	  cout << "Something went wrong with tree event analysis" << endl;
	
	TLorentzVector vP_I_CM  = S.GetListInvisibleFrames().GetFourVector(CM);
	TLorentzVector vP_S_CM  = S.GetFourVector(CM);
	TLorentzVector vP_Ja_S  = Ja.GetFourVector(S);
	TLorentzVector vP_Jb_S  = Jb.GetFourVector(S);
	TLorentzVector vP_La_S  = La.GetFourVector(S);
	TLorentzVector vP_Lb_S  = Lb.GetFourVector(S);
	TLorentzVector vP_Ia_S  = X1a.GetFourVector(S);
	TLorentzVector vP_Ib_S  = X1b.GetFourVector(S);

	double myRISR = vP_S_CM.Vect().Unit().Dot(vP_I_CM.Vect())/vP_S_CM.Vect().Mag();
	
	// m_MJ[t] = (vP_Ja_S+vP_Jb_S).M();
	double ML = (vP_La_S+vP_Lb_S).M();
	// m_EJ[t] = (vP_Ja_S+vP_Jb_S).E();
	// m_EL[t] = (vP_La_S+vP_Lb_S).E();
	// m_PJ[t] = (vP_Ja_S+vP_Jb_S).P();
	// m_PL[t] = (vP_La_S+vP_Lb_S).P();
	
	// m_PX3[t] = (vP_Ja_S+vP_La_S+vP_Ia_S).P();

	// removing momentum components parallel to CM->S boost
	TVector3 boostVis = (vP_Ja_S+vP_La_S+vP_Jb_S+vP_Lb_S).BoostVector();
	TVector3 boostInv = (vP_Ia_S+vP_Ib_S).BoostVector();
	TVector3 daBoost = vP_S_CM.Vect().Unit();

	boostVis = (boostVis.Dot(daBoost))*daBoost;
	boostInv = (boostInv.Dot(daBoost))*daBoost;

	vP_Ja_S.Boost(-boostVis);
	vP_Jb_S.Boost(-boostVis);
	vP_La_S.Boost(-boostVis);
	vP_Lb_S.Boost(-boostVis);
	vP_Ia_S.Boost(-boostInv);
	vP_Ib_S.Boost(-boostInv);

    
	double PX3_BoostT = (vP_Ja_S+vP_La_S+vP_Ia_S).P();
	double MX3a_BoostT = (vP_Ja_S+vP_La_S+vP_Ia_S).M();
	double MX3b_BoostT = (vP_Jb_S+vP_Lb_S+vP_Ib_S).M();
	double MS_BoostT = (vP_Ja_S+vP_La_S+vP_Ia_S+vP_Jb_S+vP_Lb_S+vP_Ib_S).M();

	double myMX3_BoostT = sqrt(MX3a_BoostT*MX3a_BoostT + MX3b_BoostT*MX3b_BoostT)/sqrt(2.);
	
	double myMX3 = sqrt(base->MX3a->at(1)*base->MX3a->at(1)+base->MX3b->at(1)*base->MX3b->at(1))/sqrt(2.);

	// if(2.*myMX3_BoostT/MS_BoostT < 0.4)
	//   continue;

	// TVector3 lepa,lepb;
	// lepa.SetPtEtaPhi(base->PT_lep->at(0),base->Eta_lep->at(0),base->Phi_lep->at(0));
	// lepb.SetPtEtaPhi(base->PT_lep->at(1),base->Eta_lep->at(1),base->Phi_lep->at(1));

	// double la_par = lepa.Dot(ETMiss.Unit());
	// double lb_par = lepb.Dot(ETMiss.Unit());
	// TVector3 z; z.SetXYZ(0.,0.,1.);
	// TVector3 perp; perp = ETMiss.Cross(z).Unit();
	
	// double la_perp = lepa.Dot(perp.Unit());
	// double lb_perp = lepb.Dot(perp.Unit());
	
	// double epsa = ETMiss.Mag()*lb_perp/(lb_perp*la_par-la_perp*lb_par);
	// double epsb = ETMiss.Mag()*la_perp/(la_perp*lb_par-lb_perp*la_par);

	// double Mtt = 2.*(lepa.Mag()*lepb.Mag()-lepa.Dot(lepb))*(1.+epsa)*(1.+epsb);
	// //double Mtt = 2.*lepa.Dot(lepb)*(1.+epsa)*(1.+epsb);
	// Mtt = (Mtt > 0) ? sqrt(Mtt) : sqrt(-Mtt);

	double gammaT = 2.*myMX3_BoostT/MS_BoostT;
	double dphiSI = acos(-1.) - base->dphiSI->at(1);

	double cosX3a = X3a.GetCosDecayAngle(La);

	double R = (base->PVa->at(1) - base->PVb->at(1))/(base->PVa->at(1) + base->PVb->at(1));
	double RT = (base->EVa_BoostT->at(1) - base->EVb_BoostT->at(1))/(base->EVa_BoostT->at(1) + base->EVb_BoostT->at(1));
	
	double cos2 = base->cosX2a->at(1)*base->cosX2a->at(1)+base->cosX3b->at(1)*base->cosX3b->at(1);
	
	// cout << base->weight << endl;
	
	// hist->Fill(base->RISR->at(1), fabs(base->PzS->at(1))/sqrt(base->PzS->at(1)*base->PzS->at(1)+base->PTS->at(1)*base->PTS->at(1)), base->weight*double(SKIP));
	double weight = (base->weight != 0.) ? base->weight : 1.;
	hist->Fill(gammaT, base->MX3a->at(1), weight*double(SKIP));

      }
      
      delete base;
      delete chain;
    }
  }

  cout << "Total " << hist->Integral()*137. << endl;
  
  hist->Scale(1./hist->Integral());
  
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
  l.DrawLatex(0.71,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.15,0.943,"#bf{CMS} Simulation Preliminary");

  // l.SetTextSize(0.045);
  // l.SetTextFont(132);
  // l.DrawLatex(0.74,0.04,g_Label.c_str());


}
