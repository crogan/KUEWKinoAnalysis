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

string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;
string g_Yname;
double g_Ymin;
double g_Ymax;
double g_NY;

using namespace RestFrames;

void Tabulate(){
  RestFrames::SetStyle();

  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_10_01_20/";

  int BKG_SKIP = 1;
  
  // SampleSet ttX;
  // ttX.SetBkg(true);
  // ttX.SetTitle("ttbar");
  // ttX.SetColor(kAzure+1);
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&ttX);

  // SampleSet ST;
  // ST.SetBkg(true);
  // ST.SetTitle("SingleTop");
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
  // DYjets.SetTitle("DY");
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
  // Wjets.SetTitle("Wjets");
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
  // Znunu.SetTitle("Znunu");
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
  // SIG1.SetTitle("TChiWW_250_200");
  // SIG1.SetTreeName("SMS_250_200");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);
  
  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("TChiWZ_250_240");
  // SIG1.SetTreeName("SMS_250_240");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("TSlepSlep_275_270");
  // SIG1.SetTreeName("SMS_275_270");
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

  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("T2bW_750_720");
  // SIG1.SetTreeName("SMS_750_720");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("T2tt_700_660");
  SIG1.SetTreeName("SMS_700_660");
  SIG1.SetColor(kMagenta);
  SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);

  int Nsample = g_Samples.size();
 
  g_Xname = "M_{#perp}    [GeV]";
  g_Xmin = 0.;
  g_Xmax = 60; 
  g_NX = 60.;
  g_Yname = "R_{ISR}";
  //g_Yname = "#gamma_{T}";
  //g_Yname = "p_{T}^{ISR}";
  g_Ymin = 0.6;
  g_Ymax = 1.05;
  g_NY = 45;

  // g_Xname = "R_{ISR}";
  // g_Xmin = -1.;
  // g_Xmax = 1.; 
  // g_NX = 32;
  // g_Yname = "p_{T}^{ISR} [GeV]";
  // g_Ymin = -1.;
  // g_Ymax = 1.;
  // g_NY = 32;

  //string label = "2OSlep";
  //string label = "2SFOSlep";
  //string label = "3Slep_PT200";
  string label = "2lep_PT300";
  
  vector<TH2D*> hists;
  for(int s = 0; s < Nsample; s++){
    hists.push_back(new TH2D(Form("%s_%s_hist",g_Samples[s]->GetTitle().c_str(),label.c_str()),
			     Form("%s_%s_hist",g_Samples[s]->GetTitle().c_str(),label.c_str()),
			     g_NX,g_Xmin,g_Xmax,
			     g_NY,g_Ymin,g_Ymax));
  }

  
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
	
	///////////// 2 lepton
	if(base->Nlep != 2)
	  continue;

	if(base->ID_lep->at(0) < 3 ||
	   base->ID_lep->at(1) < 3)
	  continue;

	if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 6. ||
	   base->MiniIso_lep->at(1)*base->PT_lep->at(1) > 6.)
	  continue;

	if(base->SIP3D_lep->at(0) > 4 ||
	   base->SIP3D_lep->at(1) > 4)
	  continue;

	if(base->Charge_lep->at(0)+base->Charge_lep->at(1) != 0)
	  continue;

	// if(abs(base->PDGID_lep->at(0)) != abs(base->PDGID_lep->at(1)))
	//   continue;

	// TVector3 ETMiss;
	// ETMiss.SetPtEtaPhi(base->MET, 0., base->MET_phi);

	// TVector3 lep;
	// lep.SetPtEtaPhi(base->PT_lep->at(0),base->Eta_lep->at(0),base->Phi_lep->at(0));
	// double dphiML = fabs(lep.DeltaPhi(ETMiss));
	
	// TLorentzVector lep1, lep2;
	// TVector3 MET;

	// lep1.SetPtEtaPhiM(base->PT_lep->at(0),base->Eta_lep->at(0),base->Phi_lep->at(0),base->M_lep->at(0));
	// lep2.SetPtEtaPhiM(base->PT_lep->at(1),base->Eta_lep->at(1),base->Phi_lep->at(1),base->M_lep->at(1));
	// MET.SetPtEtaPhi(base->MET,0.,base->MET_phi);

	// double dphiML = std::max(fabs(lep1.Vect().DeltaPhi(MET)),fabs(lep2.Vect().DeltaPhi(MET)));

	// double MT2 = Calc_MT2(lep1,lep2,MET);

	
	if(base->Njet_S->at(1) != 0)
	  continue;
	if(base->NSV_S->at(1) != 0)
	  continue;
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
	
	if(base->PTISR->at(1) < 400.)
	  continue;

	if(base->RISR->at(1) < 0.6)
	  continue;

	if(base->Nbjet_ISR->at(1) > 0)
	  continue;

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



	double myMX3 = sqrt(base->MX3a->at(1)*base->MX3a->at(1)+base->MX3b->at(1)*base->MX3b->at(1))/sqrt(2.);
	double myMX3_BoostT = sqrt(base->MX3a_BoostT->at(1)*base->MX3a_BoostT->at(1)+base->MX3b_BoostT->at(1)*base->MX3b_BoostT->at(1))/sqrt(2.);
	double myMS_BoostT = sqrt(base->PX3_BoostT->at(1)*base->PX3_BoostT->at(1)+
				  base->MX3a_BoostT->at(1)*base->MX3a_BoostT->at(1))+
	  sqrt(base->PX3_BoostT->at(1)*base->PX3_BoostT->at(1)+
	       base->MX3b->at(1)*base->MX3b->at(1));

	double MX3a = base->MX3a_BoostT->at(1) - base->MVa->at(1)*base->MVa->at(1)/base->MX3a_BoostT->at(1);
	double MX3b = base->MX3b_BoostT->at(1) - base->MVb->at(1)*base->MVb->at(1)/base->MX3b_BoostT->at(1);

	double gammaT = 2.*myMX3_BoostT/myMS_BoostT;
	double gamma = sqrt(2.*(MX3a*MX3a+MX3a*MX3a))/(sqrt(MX3a*MX3a+base->PX3_BoostT->at(1)*base->PX3_BoostT->at(1))+
						       sqrt(MX3b*MX3b+base->PX3_BoostT->at(1)*base->PX3_BoostT->at(1)));

	double dphiSI = acos(-1.) - base->dphiSI->at(1);
	
	double MX3 = sqrt(base->MX3a->at(1)*base->MX3b->at(1));
	double MX3T = sqrt(base->MX3a_BoostT->at(1)*base->MX3b_BoostT->at(1));
		// cout << base->weight << endl;

	double cos2 = base->cosX2a->at(1)*base->cosX2a->at(1)+base->cosX3b->at(1)*base->cosX3b->at(1);

	double lepPT = base->PT_lep->at(0)-5.;
	double cosLep = lepPT/(sqrt(lepPT*lepPT+base->EL_BoostT->at(1)*base->EL_BoostT->at(1)));
	
	// hist->Fill(base->RISR->at(1), fabs(base->PzS->at(1))/sqrt(base->PzS->at(1)*base->PzS->at(1)+base->PTS->at(1)*base->PTS->at(1)), base->weight*double(SKIP));
	double weight = (base->weight != 0.) ? base->weight : 1.;

	// if(base->RISR->at(1) < 0.975)
	//   continue;

	if(gammaT < .2)
	  continue;
	
	hists[s]->Fill(myMX3_BoostT,base->RISR->at(1), weight*double(SKIP)*g_Samples[s]->GetScale());
      }
      
      delete base;
      delete chain;
    }
  }

 
  TFile* file = new TFile("Tabulate.root", "UPDATE");
  file->cd();
  for(int s = 0; s < Nsample; s++){
    hists[s]->Scale(137.);
    hists[s]->Write("",TObject::kOverwrite);
  }
  file->Close();

}
