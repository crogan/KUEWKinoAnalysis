// C++ includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <istream>
#include <stdio.h>
#include <dirent.h>
#include <vector>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TList.h>
#include <TLeafElement.h>
#include <TLorentzVector.h>

#include "ReducedBase.hh"
#include "FitBuilder.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"
#include "SampleSet.hh"

using namespace std;
using std::vector;

vector<SampleSet*> g_Samples;

int main(int argc, char* argv[]) {
  // define samples
  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_10_01_20/";
  int BKG_SKIP = 100000;

  SampleSet ttX;
  ttX.SetBkg(true);
  ttX.SetTitle("ttbar");
  ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  ttX.SetSkip(BKG_SKIP);
  g_Samples.push_back(&ttX);

  SampleSet ST;
  ST.SetBkg(true);
  ST.SetTitle("ST");
  ST.AddFile(NtuplePath+"Fall17_102X/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
  ST.AddFile(NtuplePath+"Fall17_102X/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
  ST.AddFile(NtuplePath+"Fall17_102X/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
  ST.AddFile(NtuplePath+"Fall17_102X/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  ST.AddFile(NtuplePath+"Fall17_102X/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8_Fall17_102X.root");
  ST.SetSkip(BKG_SKIP);
  g_Samples.push_back(&ST);

  SampleSet DYjets;
  DYjets.SetBkg(true);
  DYjets.SetTitle("ZDY");
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
  DYjets.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Fall17_102X.root");
  DYjets.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Fall17_102X.root");
  DYjets.SetSkip(BKG_SKIP);
  g_Samples.push_back(&DYjets);

  SampleSet DB;
  DB.SetBkg(true);
  DB.SetTitle("DB");
  DB.AddFile(NtuplePath+"Fall17_102X/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_v2_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2L2Nu_13TeV_powheg_pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  DB.AddFile(NtuplePath+"Fall17_102X/ZZTo4L_13TeV_powheg_pythia8_Fall17_102X.root");
  DB.SetSkip(BKG_SKIP);
  g_Samples.push_back(&DB);

  SampleSet Wjets;
  Wjets.SetBkg(true);
  Wjets.SetTitle("Wjets");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  Wjets.SetSkip(BKG_SKIP);
  g_Samples.push_back(&Wjets);

  vector<SampleSet*> SMS_T2bW;
  int is = 0;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 5; j++){
      int M0 = 500 + 100*i;
      int M1 = M0-10*(j+1);
      SMS_T2bW.push_back(new SampleSet());
      is++;
      SMS_T2bW[is-1]->SetBkg(false);
      SMS_T2bW[is-1]->SetTitle(Form("T2bW_%d", 100000*M0+M1));
      SMS_T2bW[is-1]->SetTreeName(Form("SMS_%d_%d", M0, M1));
      SMS_T2bW[is-1]->AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
      g_Samples.push_back(SMS_T2bW[is-1]);
    }
  }
  
  // define leptons
  Lep elp(kElectron, kPos);
  Lep elm(kElectron, kNeg);
  Lep mup(kMuon, kPos);
  Lep mum(kMuon, kNeg);

  // define leptonic categories
  // 1 L
  Leptonic L_elp_0(LepList().Add(elp), LepList(), "elp");
  Leptonic L_elm_0(LepList().Add(elm), LepList(), "elm");
  Leptonic L_mup_0(LepList().Add(mup), LepList(), "mup");
  Leptonic L_mum_0(LepList().Add(mum), LepList(), "mum");

  // 2 L, opposite hemisphere
  Leptonic    L_OS_el_el(LepList().Add(elp), LepList().Add(elm), "el^el");
  Leptonic    L_SS_el_el(LepList().Add(elp), LepList().Add(elp), "el^el");
  L_SS_el_el += Leptonic(LepList().Add(elm), LepList().Add(elm));
  Leptonic    L_OS_mu_mu(LepList().Add(mup), LepList().Add(mum), "mu^mu");
  Leptonic    L_SS_mu_mu(LepList().Add(mup), LepList().Add(mup), "mu^mu");
  L_SS_mu_mu += Leptonic(LepList().Add(mum), LepList().Add(mum));
  Leptonic    L_OS_el_mu(LepList().Add(elp), LepList().Add(mum), "el^mu");
  L_OS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mup));
  Leptonic    L_SS_el_mu(LepList().Add(elp), LepList().Add(mup), "el^mu");
  L_SS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mum));

  // 2 L, same hemisphere
  Leptonic    L_OS_elel_0(LepList().Add(elp).Add(elm), LepList(), "elel^0");
  Leptonic    L_SS_elel_0(LepList().Add(elp).Add(elp), LepList(), "elel^0");
  L_SS_elel_0 += Leptonic(LepList().Add(elm).Add(elm), LepList());
  Leptonic    L_OS_mumu_0(LepList().Add(mup).Add(mum), LepList(), "mumu^0");
  Leptonic    L_SS_mumu_0(LepList().Add(mup).Add(mup), LepList(), "mumu^0");
  L_SS_mumu_0 += Leptonic(LepList().Add(mum).Add(mum), LepList());
  Leptonic    L_OS_elmu_0(LepList().Add(elp).Add(mum), LepList(), "elmu^0");
  L_OS_elmu_0 += Leptonic(LepList().Add(elm).Add(mup), LepList());
  Leptonic    L_SS_elmu_0(LepList().Add(elp).Add(mup), LepList(), "elmu^0");
  L_SS_elmu_0 += Leptonic(LepList().Add(elm).Add(mum), LepList());


  // cout << "Testing" << endl;
  // cout << "equal " << (L_OS_el_el == L_OS_el_el) << endl;
  // cout << "not "   << (L_SS_el_el == L_OS_el_el) << endl;
  
  // define hadronic categories
  Hadronic          H_0( 0,  0,  0, "0jsv");
  Hadronic        H_1SV( 0,  0,  1, "0j1sv");
  Hadronic      H_ge2SV( 0,  0, -2, "0jge2sv");
  Hadronic      H_0Bjet(-1,  0,  0, "0bjet");
  H_0Bjet +=   Hadronic(-1,  0, -1);
  Hadronic      H_1Bjet(-1,  1,  0, "1bjet");
  H_1Bjet +=   Hadronic(-1,  1, -1);
  Hadronic    H_ge1Bjet(-1, -1,  0, "ge1bjet");
  H_ge1Bjet += Hadronic(-1, -1, -1);
  Hadronic    H_ge2Bjet(-1, -2,  0, "ge2bjet");
  H_ge2Bjet += Hadronic(-1, -2, -1);

  vector<const Hadronic*> H_ISR;
  H_ISR.push_back(&H_0Bjet);
  H_ISR.push_back(&H_ge1Bjet);

  vector<const Hadronic*> H_1L;
  H_1L.push_back(&H_0);
  H_1L.push_back(&H_1SV);

  vector<const Hadronic*> H_2L_OS;
  H_2L_OS.push_back(&H_0);
  H_2L_OS.push_back(&H_1SV);

  vector<const Hadronic*> H_2L_SS;
  H_2L_SS.push_back(&H_0);
  H_2L_SS.push_back(&H_1SV);

  vector<double> PTISR_1L;
  PTISR_1L.push_back(200.);
  PTISR_1L.push_back(300.);
  PTISR_1L.push_back(500.);

  vector<double> PTISR_2L_OS;
  PTISR_2L_OS.push_back(200.);
  PTISR_2L_OS.push_back(300.);

  vector<double> PTISR_2L_SS;
  PTISR_2L_SS.push_back(200.);
  
  CategoryList Cats_1L;
  Cats_1L += Category(L_elp_0, H_0, H_0, "1L");
  Cats_1L += Category(L_elm_0, H_0, H_0, "1L");
  Cats_1L += Category(L_mup_0, H_0, H_0, "1L");
  Cats_1L += Category(L_mum_0, H_0, H_0, "1L");

  CategoryList Cats_2LOS;
  Cats_2LOS += Category(L_OS_el_el,  H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_mu_mu,  H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_el_mu,  H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_elel_0, H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_mumu_0, H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_elmu_0, H_0, H_0, "2LOS");

  CategoryList Cats_2LSS;
  Cats_2LSS += Category(L_SS_el_el,  H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_mu_mu,  H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_el_mu,  H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_elel_0, H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_mumu_0, H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_elmu_0, H_0, H_0, "2LSS");

  vector<double> Mperp_bins;
  Mperp_bins.push_back(0.);
  Mperp_bins.push_back(5.);
  Mperp_bins.push_back(10.);
  Mperp_bins.push_back(20.);
  Mperp_bins.push_back(40.);
  Mperp_bins.push_back(70.);
  Mperp_bins.push_back(100.);
  vector<double> RISR_bins;
  RISR_bins.push_back(0.5);
  RISR_bins.push_back(0.6);
  RISR_bins.push_back(0.7);
  RISR_bins.push_back(0.8);
  RISR_bins.push_back(0.85);
  RISR_bins.push_back(0.9);
  RISR_bins.push_back(0.925);
  RISR_bins.push_back(0.95);
  RISR_bins.push_back(0.975);
  RISR_bins.push_back(1.);
  RISR_bins.push_back(1.05);
  FitBin FitBin_default(Mperp_bins, RISR_bins);

  Cats_1L = Cats_1L.CreateFitBinRegions(FitBin_default);
  Cats_1L = Cats_1L.CreateLeptonIDRegions(3, 2);
  Cats_1L = Cats_1L.CreateHadronicSRegions(H_1L);
  Cats_1L = Cats_1L.CreateHadronicISRRegions(H_ISR);
  Cats_1L = Cats_1L.CreateGenericRegions("PTISR", PTISR_1L);

  Cats_2LOS = Cats_2LOS.CreateFitBinRegions(FitBin_default);
  Cats_2LOS = Cats_2LOS.CreateLeptonIDRegions(3, 2);
  Cats_2LOS = Cats_2LOS.CreateHadronicSRegions(H_2L_OS);
  Cats_2LOS = Cats_2LOS.CreateHadronicISRRegions(H_ISR);
  Cats_2LOS = Cats_2LOS.CreateGenericRegions("PTISR", PTISR_2L_OS);

  Cats_2LSS = Cats_2LSS.CreateFitBinRegions(FitBin_default);
  Cats_2LSS = Cats_2LSS.CreateLeptonIDRegions(3, 2);
  Cats_2LSS = Cats_2LSS.CreateHadronicSRegions(H_2L_SS);
  Cats_2LSS = Cats_2LSS.CreateHadronicISRRegions(H_ISR);
  Cats_2LSS = Cats_2LSS.CreateGenericRegions("PTISR", PTISR_2L_SS);
  
  //////////////////////////////////////////////////////
  CategoryList Cats_All;
  Cats_All += Cats_1L;
  Cats_All += Cats_2LOS;
  Cats_All += Cats_2LSS;

  
  // Cats_1L.Print();
  // cout << Cats_1L.GetN() << " 1L categories" << endl;

  // Cats_2L_OS.Print();
  // cout << Cats_2L_OS.GetN() << " 2L OS categories" << endl;

  // Cats_2L_SS.Print();
  // cout << Cats_2L_SS.GetN() << " 2L SS categories" << endl;

  FitBuilder FITBuilder;

  // event loops
  int Nsample = g_Samples.size();
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

      //for(int e = 0; e < Nentry; e += SKIP){
      for(int e = 4529; e < Nentry; e += SKIP){
	base->GetEntry(e);
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;


	
	if(base->MET < 200)
	  continue;
	
	if(base->PTISR->at(1) < 200.)
	  continue;

	if(base->RISR->at(1) < 0.5)
	  continue;

	int Nlep  = base->Nlep;

	if(Nlep != 1 && Nlep != 2)
	  continue;

	int NjetS    = base->Njet_S->at(1);
	int NbjetS   = base->Nbjet_S->at(1);
	int NjetISR  = base->Njet_ISR->at(1);
	int NbjetISR = base->Nbjet_ISR->at(1);
	int NSV      = base->NSV_S->at(1);
	   
	LepList list_a;
	LepList list_b;
	
	int index;
	
	if(base->index_lep_a->size() < 2 ||
	   base->index_lep_b->size() < 2)
	  continue;
	
	for(int i = 0; i < base->Nlep_a->at(1); i++){
	  index = base->index_lep_a->at(1)[i];

	  int PDGID  = base->PDGID_lep->at(index);
	  int gindex = base->Index_lep->at(index);
	  int genPDGID;
	  int momPDGID;
	  if(gindex < 0){
	    genPDGID = 0;
	    momPDGID = 0;
	  } else {
	    genPDGID = base->genPDGID_lep->at(gindex);
	    momPDGID = base->genMomPDGID_lep->at(gindex);
	  }

	  LepID id;
	  if(base->ID_lep->at(index) < 3 ||
	     base->MiniIso_lep->at(index)*base->PT_lep->at(index) > 5.)
	    id = kBronze;
	  else if(base->SIP3D_lep->at(index) > 4)
	    id = kSilver;
	  else
	    id = kGold;
	  LepFlavor flavor;
	  if(abs(PDGID) == 11)
	    flavor = kElectron;
	  else
	    flavor = kMuon;
	  LepCharge charge = (base->Charge_lep->at(index) > 0 ? kPos : kNeg);
	  LepSource source = GetLepSource(PDGID, genPDGID, momPDGID); 
	  
	  list_a += Lep(flavor, charge, id, source);
	}
	for(int i = 0; i < base->Nlep_b->at(1); i++){
	  index = base->index_lep_b->at(1)[i];

	  int PDGID  = base->PDGID_lep->at(index);
	  int gindex = base->Index_lep->at(index);
	  int genPDGID = 0;
	  int momPDGID = 0;
	  if(gindex >= 0){
	    genPDGID = base->genPDGID_lep->at(gindex);
	    momPDGID = base->genMomPDGID_lep->at(gindex);
	  }

	  LepID id;
	  if(base->ID_lep->at(index) < 3 ||
	     base->MiniIso_lep->at(index)*base->PT_lep->at(index) > 5.)
	    id = kBronze;
	  else if(base->SIP3D_lep->at(index) > 4)
	    id = kSilver;
	  else
	    id = kGold;
	  LepFlavor flavor;
	  if(abs(PDGID) == 11)
	    flavor = kElectron;
	  else
	    flavor = kMuon;
	  LepCharge charge = (base->Charge_lep->at(index) > 0 ? kPos : kNeg);
	  LepSource source = GetLepSource(PDGID, genPDGID, momPDGID); 
	  
	  list_b += Lep(flavor, charge, id, source);
	}
	
	Category Event(Leptonic(list_a, list_b),
		       Hadronic(NjetS, NbjetS, NSV),
		       Hadronic(NjetISR, NbjetISR, 0));
	Event.AddGenericVal(GenericVal(base->PTISR->at(1)));

	int eindex = Cats_All.Find(Event);
	if(eindex < 0)
	  continue;

	LepList Fakes  = list_a.GetFakes(kQFlip);
	Fakes         += list_b.GetFakes(kQFlip);

	double Mperp = sqrt(base->MX3a_BoostT->at(1)*base->MX3a_BoostT->at(1)+
			    base->MX3b_BoostT->at(1)*base->MX3b_BoostT->at(1))/sqrt(2.);
	if((Nlep == 1) && (NjetS == 0) && (NSV == 0))
	  Mperp = base->EL_BoostT->at(1);
	double RISR  = base->RISR->at(1);
	
	if(Fakes.GetN() > 0){
	  vector<string> flabels = Fakes.GetFakeLabels(kQFlip);
	  int Nf = flabels.size();
	  for(int fl = 0; fl < Nf; fl++)
	    FITBuilder.AddEvent(137.*base->weight/double(Nf), Mperp, RISR,
				Cats_All[eindex], flabels[fl]);
	} else {
	  FITBuilder.AddEvent(137.*base->weight, Mperp, RISR,
			      Cats_All[eindex],
			      g_Samples[s]->GetTitle(), !g_Samples[s]->GetBkg());
	}
      }
    }
  }

  FITBuilder.WriteFit("test.root");
  
}
