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

  string StopNtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/StopNtuple/";

  int BKG_SKIP = 10;

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
  ttX.SetSkip(50);
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
  // SIG1.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 100");
  // SIG1.SetTreeName("SMS_300_240");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  int Nsample = g_Samples.size();
  
  //string g_Label = "No selection";
  string g_Label = "Region D";


  g_Xname = "N_{lep}^{S}";
  g_Xmin = -1.;
  g_Xmax = 1.; 
  g_NX = 32;
  g_Yname = "N_{jet}^{S}";
  g_Ymin = -1.;
  g_Ymax = 1.;
  g_NY = 32;

  int TREE = 2;

  TH2D* hist = new TH2D("hist","hist",
			g_NX,g_Xmin,g_Xmax,
			g_NY,g_Ymin,g_Ymax);

  // Set up RestFrames tree
  LabRecoFrame       LAB("LAB","LAB");
  VisibleRecoFrame   ISR("ISR","ISR");
  DecayRecoFrame     CM("CM","CM");
  DecayRecoFrame     X2X2("X2X2","#tilde{#chi}^{ 0}_{2} #tilde{#chi}^{ 0}_{2}");
  DecayRecoFrame     X2a("X2a","#tilde{#chi}^{ 0}_{2 a}");
  DecayRecoFrame     X2b("X2b","#tilde{#chi}^{ 0}_{2 b}");
  VisibleRecoFrame   Va("Za","Z_{a}");
  VisibleRecoFrame   Vb("Hb","H_{b}");
  InvisibleRecoFrame X1a("X1a","#tilde{#chi}^{ 0}_{1 a}");
  InvisibleRecoFrame X1b("X1b","#tilde{#chi}^{ 0}_{1 b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  LAB.SetChildFrame(CM);
  CM.AddChildFrame(ISR);
  CM.AddChildFrame(X2X2);
  X2X2.AddChildFrame(X2a);
  X2X2.AddChildFrame(X2b);
  X2a.AddChildFrame(Va);
  X2a.AddChildFrame(X1a);
  X2b.AddChildFrame(Vb);
  X2b.AddChildFrame(X1b);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Invisible Groups
  InvisibleGroup INV("INV","#tilde{#chi}_{1}^{ 0} Jigsaws");
  INV.AddFrames(X1a+X1b);     
 
  // Set di-LSP mass to minimum Lorentz-invariant expression
  SetMassInvJigsaw X1_mass("X1_mass","Set M_{#tilde{#chi}_{1}^{ 0} #tilde{#chi}_{1}^{ 0}} to minimum"); 
  INV.AddJigsaw(X1_mass);  
  

  // Set di-LSP rapidity to that of visible particles
  SetRapidityInvJigsaw X1_eta("X1_eta","#eta_{#tilde{#chi}_{1}^{ 0} #tilde{#chi}_{1}^{ 0}} = #eta_{2#gamma+2#it{l}}");
  INV.AddJigsaw(X1_eta);
  X1_eta.AddVisibleFrames(X2X2.GetListVisibleFrames());

  //ContraBoostInvJigsaw X1X1_minM2("X1X1_contra","Contraboost invariant Jigsaw");
  //MinMassDiffInvJigsaw X1X1_contra("MinMh_R","min M_{h}, M_{h}^{ a}= M_{h}^{ b}",2);
  MinMassesSqInvJigsaw X1X1_contra("MinMWa_R","min M_{W}, M_{W}^{a,a}= M_{W}^{a,b}", 2);
  INV.AddJigsaw(X1X1_contra);
  X1X1_contra.AddVisibleFrames(X2a.GetListVisibleFrames(), 0);
  X1X1_contra.AddVisibleFrames(X2b.GetListVisibleFrames(), 1);
  X1X1_contra.AddInvisibleFrame(X1a, 0);
  X1X1_contra.AddInvisibleFrame(X1b, 1);

  CombinatoricGroup COMB("COMB", "Combinatoric System");
  COMB.AddFrame(Va);
  COMB.AddFrame(Vb);
  COMB.SetNElementsForFrame(Va, 1);
  COMB.SetNElementsForFrame(Vb, 1);
  MinMassesCombJigsaw CombSplit("CombSplit", "Minimize M_{Va} and M_{Vb} Jigsaw");
  CombSplit.AddCombFrame(Va, 0);
  CombSplit.AddCombFrame(Vb, 1);
  CombSplit.AddObjectFrame(Va, 0);
  CombSplit.AddObjectFrame(Vb, 1);
  COMB.AddJigsaw(CombSplit);
     
  
  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << std::endl << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

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

	if(base->MiniIso_lep->at(0) > 0 || base->MiniIso_lep->at(1) > 0)
	  continue;
	
	// if(base->ID_lep->at(0) < 3 || base->ID_lep->at(1) < 3 || base->ID_lep->at(2) < 3)
	//   continue;

	// if(base->Njet_S->at(1) > 0)
	//   continue;
	
	// TLorentzVector vISR;
	// for(int i = 0; i < base->Njet; i++){
	//   TLorentzVector jeti;
	//   jeti.SetPtEtaPhiM(base->PT_jet->at(i),base->Eta_jet->at(i),base->Phi_jet->at(i),base->M_jet->at(i));
	//   vISR += jeti;
	// }
	
	// TLorentzVector vVa, vVb, vV;
	// for(int i = 0; i < 3; i++){
	//   TLorentzVector lepi;
	//   lepi.SetPtEtaPhiM(base->PT_lep->at(i),base->Eta_lep->at(i),base->Phi_lep->at(i),base->M_lep->at(i));
	//   vV += lepi;
	// }

	// bool isOSSF = false;
	// for(int i = 0; i < 2; i++){
	//   TLorentzVector lepi;
	//   lepi.SetPtEtaPhiM(base->PT_lep->at(i),base->Eta_lep->at(i),base->Phi_lep->at(i),base->M_lep->at(i));
	//   for(int j = i+1; j < 3; j++){
	//     if(base->PDGID_lep->at(i)+base->PDGID_lep->at(j) == 0){
	//       TLorentzVector lepj;
	//       lepj.SetPtEtaPhiM(base->PT_lep->at(j),base->Eta_lep->at(j),base->Phi_lep->at(j),base->M_lep->at(j));
	//       if(!isOSSF || (lepi+lepj).M() < vVa.M()){
	// 	vVa = lepi+lepj;
	// 	vVb = vV - vVa;
	// 	isOSSF = true;
	//       }
	//     }
	//   }
	// }
	// if(!isOSSF)
	//   continue;

	// OSSF
	if(base->PDGID_lep->at(0)+base->PDGID_lep->at(1) != 0)
	  continue;

	TLorentzVector vV, vVa, vVb;
	
	TLorentzVector lep[2];
	for(int i = 0; i < 2; i++){
	  lep[i].SetPtEtaPhiM(base->PT_lep->at(i),base->Eta_lep->at(i),base->Phi_lep->at(i),base->M_lep->at(i));
	  vV += lep[i];
	  vVa += lep[i];
	}
	  
	TLorentzVector vISR;
	for(int i = 0; i < base->Njet; i++){
	  TLorentzVector jeti;
	  jeti.SetPtEtaPhiM(base->PT_jet->at(i),base->Eta_jet->at(i),base->Phi_jet->at(i),base->M_jet->at(i));
	  vISR += jeti;
	}

	TLorentzVector jet[2];
	for(int i = 0; i < 2; i++){
	  int index = base->index_jet_S->at(1)[i];
	  jet[i].SetPtEtaPhiM(base->PT_jet->at(index),base->Eta_jet->at(index),base->Phi_jet->at(index),base->M_jet->at(index));
	  vISR -= jet[i];
	  vV += jet[i];
	  vVb += jet[i];
	}
	
	TLorentzVector P_V = vV;
	TLorentzVector P_I;
	P_I.SetPtEtaPhiM(base->MET, 0., base->MET_phi, 0.);
	
	P_V.SetPxPyPzE(P_V.Px(),P_V.Py(),0.,sqrt(P_V.Pt()*P_V.Pt() + P_V.M2()));
	P_I.SetPxPyPzE(P_I.Px(),P_I.Py(),0.,sqrt(P_I.Pt()*P_I.Pt() + P_I.M2()));

	double Ei = P_V.E()*(P_I.P()*P_I.P() + P_V.Vect().Dot(P_I.Vect()))/(P_V.P()*P_V.P() + P_V.Vect().Dot(P_I.Vect()));

	
	
	// if(Ei < 0){
	//   //cout << "HERE " << endl;
	//   continue;
	// } 
      
	double MI2 = (pow(P_V.E()*(P_I.P()*P_I.P() + P_V.Vect().Dot(P_I.Vect()))/(P_V.P()*P_V.P() + P_V.Vect().Dot(P_I.Vect())),2.) -
		      P_I.P()*P_I.P() - P_V.M2() +
		      pow(vVa.M()+vVb.M(),2.)) / 4.;

	// if(MI2 < 0.)
	//   continue;
	
	double MI = sqrt( MI2 );

	double Minv = sqrt(Ei*Ei - P_I.P()*P_I.P());

	// analyze event
	LAB.ClearEvent();                                 // clear the reco tree

	std::vector<RFKey> jetID;
	std::vector<RFKey> lepID;
	for(int i = 0; i < 2; i++){
	  jetID.push_back(COMB.AddLabFrameFourVector(jet[i]));
	  lepID.push_back(COMB.AddLabFrameFourVector(lep[i]));
	}
	
	//Va.SetLabFrameFourVector(vVa); // Set lepton 4-vectors
	//Vb.SetLabFrameFourVector(vVb); 
	ISR.SetLabFrameFourVector(vISR);
	TLorentzVector MET;
	MET.SetVectM(P_I.Vect(), Minv);    // Get the MET from gen tree
	//INV.SetLabFrameFourVector(MET);
	INV.SetLabFrameThreeVector(P_I.Vect());                  // Set the MET in reco tree

	// X1a.SetMinimumMass(MI);
	// X1b.SetMinimumMass(MI);

	// X1a.SetMinimumMass(220);
	// X1b.SetMinimumMass(220);
      
	LAB.AnalyzeEvent();

	int Njet_Va = 0;
	int Njet_Vb   = 0;
	int Nlep_Va = 0;
	int Nlep_Vb   = 0;
	for(int i = 0; i < 2; i++){
	  if(COMB.GetFrame(jetID[i]) == Va)
	    Njet_Va++;
	  else
	    Njet_Vb++;
	  if(COMB.GetFrame(lepID[i]) == Va)
	    Nlep_Va++;
	  else
	    Nlep_Vb++;
	}

	TOTAL += base->weight;
	if((Njet_Va == 2 && Nlep_Va == 0) ||
	   (Njet_Va == 0 && Nlep_Va == 2))
	  CORRECT += base->weight;
	
	//cout << Njet_Va << " " << Nlep_Va << endl;
	
		// cout << Minv << " " << X2X2.GetListInvisibleFrames().GetMass() << " ";
	// cout << MI << " " << X1a.GetMass() << " " << X1a.GetMass() << endl;;
      
      
	// if(base->Nlep_S->at(2)+base->Nlep_ISR->at(2) != 2)
	//   continue;

	// if(base->RISR->at(2) < 0.7)
	//   continue;

	// if(base->PTISR->at(2) < 250)
	//   continue;

	double MP = X2a.GetMass();

	double RISR2 = MI/MP;

	if(base->PTISR->at(1) < 200.)
	  continue;

	// if(base->RISR->at(1) < 0.6)
	//   continue;

	TVector3 vP_ISR = X2X2.GetFourVector(CM).Vect();
	TVector3 vP_I   = X2X2.GetListInvisibleFrames().GetFourVector(CM).Vect();
	TVector3 vP_V   = X2X2.GetListVisibleFrames().GetFourVector(CM).Vect();

	double cosI = X2X2.GetListInvisibleFrames().GetFourVector(X2X2).Vect().Unit().Dot(vP_ISR.Unit());
	double dphiI = X2X2.GetDeltaPhiBoostVisible();
	
	vP_ISR.SetZ(0.);
	vP_I.SetZ(0.);
       
	double RISR = fabs(vP_I.Dot(vP_ISR.Unit())) / vP_ISR.Mag();
	double PTISR = vP_ISR.Mag();


	if(base->Nbjet > 0)
	  continue;
	
	//cout << base->Nbjet << endl;

	//cout << base->MiniIso_lep->at(0) << " " << base->MiniIso_lep->at(1) << endl;

	if(!((Njet_Va == 2 && Nlep_Va == 0) ||
	     (Njet_Va == 0 && Nlep_Va == 2)))
	  continue;
	//   hist->Fill(RISR, 0.5, base->weight*double(SKIP));
	// else
	//   hist->Fill(RISR, -0.5, base->weight*double(SKIP));
	if(Njet_Va == 0)
	  hist->Fill(X2X2.GetCosDecayAngle(), X2b.GetCosDecayAngle(), base->weight*double(SKIP));
	else
	  hist->Fill(-X2X2.GetCosDecayAngle(), X2a.GetCosDecayAngle(), base->weight*double(SKIP));
      }

      delete base;
      delete chain;
    }
  }

  cout << "TOTAL CORRECT ASSIGNMENT = " << CORRECT/TOTAL << endl;
  
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
