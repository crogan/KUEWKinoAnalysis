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
//#include <TIter.h>
#include <TKey.h>

#include "ReducedBase_slim.hh"
#include "FitBuilder.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"
#include "SampleSet.hh"
#include "Systematics.hh"

using namespace std;

int g_iyear;
double g_lumi[3];

map<string, SampleSet*> g_Samples[3];  // [year]
string g_NtuplePath;
int    g_BKG_SKIP;

void InitializeSamples();
vector<SampleSet*> ParseSMS(const string& filename, const string& prefix);
vector<string> GetAllSamples();

CategoryList g_Categories;
void InitializeCategories();

vector<Systematic*> g_Systematics;
void InitializeSystematics();

int main(int argc, char* argv[]) {
  g_iyear = 1;
  g_BKG_SKIP = 1;

  g_NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_26_04_20/";
  InitializeSamples();
  vector<string> samples = GetAllSamples();
  
  InitializeCategories();

  InitializeSystematics();

  FitBuilder FITBuilder;

  // sample (process) loop
  int Nsample = samples.size();
  for(int s = 0; s < Nsample; s++){
    string title = g_Samples[g_iyear][samples[s]]->GetTitle();

    bool is_data   = g_Samples[g_iyear][samples[s]]->GetData();
    bool is_bkg    = g_Samples[g_iyear][samples[s]]->GetBkg();
    bool is_signal = !(is_data || is_bkg);

    Process proc(title, (is_data ? kData : (is_bkg ? kBkg : kSig)));

    int Nsys = (is_data ? 1 : g_Systematics.size());
    
    int Nfile = g_Samples[g_iyear][samples[s]]->GetNFile();
    cout << "Processing " << Nfile << " files for sample " << title << endl;
    for(int f = 0; f < Nfile; f++){
      string file = g_Samples[g_iyear][samples[s]]->GetFile(f);
      string tree = g_Samples[g_iyear][samples[s]]->GetTreeName();
      
      cout << "   Processing file " << file << " w/ tree " << tree << endl;
    
      TChain* chain = new TChain(tree.c_str());
      chain->Add(file.c_str());

      ReducedBase* base = new ReducedBase(chain);

      int Nentry = base->fChain->GetEntries();

      int SKIP = g_Samples[g_iyear][samples[s]]->GetSkip();

      // event loop
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

	// only apply trigger to data for moment
	if(!base->METORtrigger && is_data)
	  continue;
		
	if(base->MET < 200)
	  continue;
	  
	if(base->PTISR->at(1) < 200.)
	  continue;
	  
	if(base->PTCM->at(1)/base->PTISR->at(1) > 0.2)
	  continue;
	  
	if(base->RISR->at(1) < 0.5)
	  continue;
	
	int Nlep     = base->Nlep;
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
	  int gindex = g_Samples[g_iyear][samples[s]]->GetData() ? -1 :  base->Index_lep->at(index);
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
	    
	  list_a += Lep(flavor, charge, id, source);
	}
	for(int i = 0; i < base->Nlep_b->at(1); i++){
	  index = base->index_lep_b->at(1)[i];
	  
	  int PDGID  = base->PDGID_lep->at(index);
	  int gindex = g_Samples[g_iyear][samples[s]]->GetData() ? -1 :  base->Index_lep->at(index);
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
	  
	int eindex = g_Categories.Find(Event);
	if(eindex < 0)
	  continue;

	// systematics loop
	for(int is = 0; is < Nsys; is++){
	  Systematic& sys = *g_Systematics[is];
	  if(!(!sys)){
	    if(sys.IsUp()){
	      sys.Down();
	      is--;
	    } else {
	      sys.Up();
	    }
	  }
	  
	  double weight = 1.;
	  if(!is_data){
	    weight = base->weight*g_lumi[g_iyear];
	    if(sys == Systematic("BTAG_SF"))
	      if(sys.IsUp())
		weight *= base->m_BtagSFweight_up;
	      else
		weight *= base->m_BtagSFweight_down;
	    else 
	      weight *= base->m_BtagSFweight;
	    if(sys == Systematic("PU_SF"))
	      if(sys.IsUp())
		weight *= base->PUweight_up;
	      else
		weight *= base->PUweight_down;
	    else 
	      weight *= base->PUweight;
	  }
	  
	  LepList Fakes  = list_a.GetFakes(kHF);
	  Fakes         += list_b.GetFakes(kHF);

	  // really should define this as variable in tree....
	  double Mperp = sqrt(base->MX3a_BoostT->at(1)*base->MX3a_BoostT->at(1)+
			      base->MX3b_BoostT->at(1)*base->MX3b_BoostT->at(1))/sqrt(2.);
	  // use Eperp
	  if((Nlep == 1) && (NjetS == 0) && (NSV == 0))
	    Mperp = base->EL_BoostT->at(1);
	  if(((Nlep == 0) && (NjetS == 0) && (NSV == 1)) ||
	     ((Nlep == 0) && (NjetS == 1) && (NSV == 0)))
	    Mperp = base->EJ_BoostT->at(1);
	
	  double RISR  = base->RISR->at(1);

	  if(Fakes.GetN() > 0 && is_bkg){
	    vector<string> flabels = Fakes.GetFakeLabels(kHF);
	    int Nf = flabels.size();
	    for(int fl = 0; fl < Nf; fl++){
	      if(title.find("QCD") == string::npos)
		FITBuilder.AddEvent(weight/double(Nf), Mperp, RISR,
				    g_Categories[eindex], proc, sys);
	      
	      FITBuilder.AddEvent(weight/double(Nf), Mperp, RISR,
				  g_Categories[eindex], proc, sys);
	    }
	  } else {
	    FITBuilder.AddEvent(weight, Mperp, RISR,
				g_Categories[eindex], proc, sys);
	  }
	}
      }
      delete base;
      delete chain;
    }
  }

  FITBuilder.WriteFit("test.root");
  
}

void InitializeSamples(){
  g_lumi[0] = 35.922; // 2016 lumi
  g_lumi[1] = 41.529; // 2017 lumi
  g_lumi[2] = 59.74;  // 2018 lumi
  
  // 2017 samples
  
    SampleSet* ttX = new SampleSet();
    g_Samples[1]["ttbar"] = ttX;
    ttX->SetBkg(true);
    ttX->SetTitle("ttbar");
    ttX->AddFile(g_NtuplePath+"Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    ttX->AddFile(g_NtuplePath+"Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    ttX->AddFile(g_NtuplePath+"Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    ttX->SetSkip(g_BKG_SKIP);

    SampleSet* ST = new SampleSet();
    g_Samples[1]["ST"] = ST;
    ST->SetBkg(true);
    ST->SetTitle("ST");
    ST->AddFile(g_NtuplePath+"Fall17_102X/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    ST->AddFile(g_NtuplePath+"Fall17_102X/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
    ST->AddFile(g_NtuplePath+"Fall17_102X/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
    ST->AddFile(g_NtuplePath+"Fall17_102X/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
    ST->AddFile(g_NtuplePath+"Fall17_102X/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8_Fall17_102X.root");
    ST->SetSkip(g_BKG_SKIP);

    SampleSet* DYjets = new SampleSet();
    g_Samples[1]["ZDY"] = DYjets;
    DYjets->SetBkg(true);
    DYjets->SetTitle("ZDY");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-600toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Fall17_102X.root");
    DYjets->AddFile(g_NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Fall17_102X.root");
    DYjets->SetSkip(g_BKG_SKIP);

    SampleSet* DB = new SampleSet();
    g_Samples[1]["DB"] = DB;
    DB->SetBkg(true);
    DB->SetTitle("DB");
    DB->AddFile(g_NtuplePath+"Fall17_102X/WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_v2_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/ZZTo2L2Nu_13TeV_powheg_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/ZZTo4L_13TeV_powheg_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
    DB->AddFile(g_NtuplePath+"Fall17_102X/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
    DB->SetSkip(g_BKG_SKIP);

    SampleSet* Wjets = new SampleSet();
    g_Samples[1]["Wjets"] = Wjets;
    Wjets->SetBkg(true);
    Wjets->SetTitle("Wjets");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->AddFile(g_NtuplePath+"Fall17_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
    Wjets->SetSkip(g_BKG_SKIP);

    SampleSet* TB = new SampleSet();
    g_Samples[1]["TB"] = TB;
    TB->SetBkg(true);
    TB->AddFile(g_NtuplePath+"Fall17_102X/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/WZG_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
    TB->AddFile(g_NtuplePath+"Fall17_102X/WWG_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");

    SampleSet* QCD = new SampleSet();
    g_Samples[1]["QCD"] = QCD;
    QCD->SetBkg(true);
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT100to200_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT200to300_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT300to500_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT500to700_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
    QCD->AddFile(g_NtuplePath+"Fall17_102X/QCD_HT700to1000_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root");
  
  vector<SampleSet*> SMS_T2bW =
    ParseSMS(g_NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", "T2bW");
  for(int i = 0; i < int(SMS_T2bW.size()); i++)
    g_Samples[1][SMS_T2bW[i]->GetTitle()] = SMS_T2bW[i];
  
    vector<SampleSet*> SMS_T2tt =
    ParseSMS(g_NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", "T2tt");
    for(int i = 0; i < int(SMS_T2tt.size()); i++)
    g_Samples[1][SMS_T2tt[i]->GetTitle()] = SMS_T2tt[i];

    vector<SampleSet*> SMS_TChiWZ =
    ParseSMS(g_NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", "TChiWZ");
    for(int i = 0; i < int(SMS_TChiWZ.size()); i++)
    g_Samples[1][SMS_TChiWZ[i]->GetTitle()] = SMS_TChiWZ[i];

    vector<SampleSet*> SMS_TChipmWW =
    ParseSMS(g_NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", "TChipmWW");
    for(int i = 0; i < int(SMS_TChipmWW.size()); i++)
    g_Samples[1][SMS_TChipmWW[i]->GetTitle()] = SMS_TChipmWW[i];

    vector<SampleSet*> SMS_TSlepSlep =
    ParseSMS(g_NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", "TSlepSlep");
    for(int i = 0; i < int(SMS_TSlepSlep.size()); i++)
    g_Samples[1][SMS_TSlepSlep[i]->GetTitle()] = SMS_TSlepSlep[i];

  
    SampleSet* Data = new SampleSet();
    g_Samples[1]["Data"] = Data;
    Data->SetData(true);
    Data->AddFile(g_NtuplePath+"Fall17_102X_Data/MET_Run2017B-Nano25Oct2019-v1_2017_Fall17_102X.root");
    Data->AddFile(g_NtuplePath+"Fall17_102X_Data/MET_Run2017C-Nano25Oct2019-v1_2017_Fall17_102X.root");
    Data->AddFile(g_NtuplePath+"Fall17_102X_Data/MET_Run2017D-Nano25Oct2019-v1_2017_Fall17_102X.root");
    Data->AddFile(g_NtuplePath+"Fall17_102X_Data/MET_Run2017E-Nano25Oct2019-v1_2017_Fall17_102X.root");
    Data->AddFile(g_NtuplePath+"Fall17_102X_Data/MET_Run2017F-Nano25Oct2019-v1_2017_Fall17_102X.root");
  
}

vector<string> GetAllSamples(){
  vector<string> list;
  map<string, SampleSet*>::iterator s = g_Samples[g_iyear].begin();
  while(s != g_Samples[g_iyear].end()){
    //if(s->first.find("T2bW") != std::string::npos)
    list.push_back(s->first);
    s++;
  }

  return list;
}

vector<SampleSet*> ParseSMS(const string& filename, const string& prefix){
  vector<SampleSet*> sslist;

  TFile file(filename.c_str(), "READ");
  if(!file.IsOpen()){
    cout << "can't open " << filename << endl;
    return sslist;
  }
  TIter list(file.GetListOfKeys());
  TKey* key;
  int M0, M1;
  string name;
  while((key = (TKey*)list.Next())){
    name = string(key->GetName());
    if(name.find("SMS") == std::string::npos)
      continue;
    sscanf(name.c_str(), "SMS_%d_%d", &M0, &M1);
    
    SampleSet* ss = new SampleSet();
    ss->SetBkg(false);
    ss->SetTreeName(name);
    ss->AddFile(filename);
    ss->SetTitle(Form("%s_%d", prefix.c_str(), 10000*M0+M1));
    
    sslist.push_back(ss);
  }
  file.Close();

  return sslist;
}

void InitializeCategories(){
  // define leptons
  Lep elp(kElectron, kPos);
  Lep elm(kElectron, kNeg);
  Lep mup(kMuon, kPos);
  Lep mum(kMuon, kNeg);

  // define hadrons
  Had jt(kJet);
  Had bj(kBtag);
  Had sv(kSV);

  // define leptonic categories
  // 0 L
  Leptonic L_0(LepList(), LepList(), "0");

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


  
  // define hadronic categories
  Hadronic          H_0( 0,  0,  0, "0j0sv");
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

  vector<const Hadronic*> H_0L;
  H_0L.push_back(&H_1SV);
  H_0L.push_back(&H_ge2SV);
  H_0L.push_back(&H_0Bjet);
  H_0L.push_back(&H_1Bjet);
  H_0L.push_back(&H_ge2Bjet);
  
  vector<const Hadronic*> H_1L;
  H_1L.push_back(&H_0);
  H_1L.push_back(&H_1SV);
  H_1L.push_back(&H_ge2SV);
  H_1L.push_back(&H_0Bjet);
  H_1L.push_back(&H_1Bjet);
  H_1L.push_back(&H_ge2Bjet);

  vector<const Hadronic*> H_2LOS;
  H_2LOS.push_back(&H_0);
  H_2LOS.push_back(&H_1SV);
  H_2LOS.push_back(&H_0Bjet);
  H_2LOS.push_back(&H_1Bjet);
  H_2LOS.push_back(&H_ge2Bjet);

  vector<const Hadronic*> H_2LSS;
  H_2LSS.push_back(&H_0);
  H_2LSS.push_back(&H_1SV);
  H_2LSS.push_back(&H_0Bjet);
  H_2LSS.push_back(&H_1Bjet);
  H_2LSS.push_back(&H_ge2Bjet);

  vector<double> PTISR_0L;
  PTISR_0L.push_back(200.);
  PTISR_0L.push_back(500.);
  PTISR_0L.push_back(1000.);
  
  vector<double> PTISR_1L;
  PTISR_1L.push_back(200.);
  PTISR_1L.push_back(500.);

  vector<double> PTISR_2LOS;
  PTISR_2LOS.push_back(200.);
  PTISR_2LOS.push_back(300.);

  vector<double> PTISR_2LSS;
  PTISR_2LSS.push_back(200.);

  cout << "Initializing 0L" << endl;
  CategoryList Cats_0L;
  Cats_0L += Category(L_0, H_0, H_0, "0L");

  cout << "Initializing 0L" << endl;
  CategoryList Cats_1L;
  Cats_1L += Category(L_elp_0, H_0, H_0, "1L"); 
  Cats_1L += Category(L_elm_0, H_0, H_0, "1L");
  Cats_1L += Category(L_mup_0, H_0, H_0, "1L");
  Cats_1L += Category(L_mum_0, H_0, H_0, "1L");

  cout << "Initializing 1L" << endl;
  CategoryList Cats_2LOS;
  Cats_2LOS += Category(L_OS_el_el,  H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_mu_mu,  H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_el_mu,  H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_elel_0, H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_mumu_0, H_0, H_0, "2LOS");
  Cats_2LOS += Category(L_OS_elmu_0, H_0, H_0, "2LOS");

  cout << "Initializing 2L" << endl;
  CategoryList Cats_2LSS;
  Cats_2LSS += Category(L_SS_el_el,  H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_mu_mu,  H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_el_mu,  H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_elel_0, H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_mumu_0, H_0, H_0, "2LSS");
  Cats_2LSS += Category(L_SS_elmu_0, H_0, H_0, "2LSS");

  vector<double> BinsM_0L;
  BinsM_0L.push_back(0.);
  BinsM_0L.push_back(5.);
  BinsM_0L.push_back(10.);
  BinsM_0L.push_back(20.);
  BinsM_0L.push_back(50.);
  BinsM_0L.push_back(100.);
  BinsM_0L.push_back(200.);
  BinsM_0L.push_back(400.);
  BinsM_0L.push_back(600.);
  vector<double> BinsR_0L;
  BinsR_0L.push_back(0.5);
  BinsR_0L.push_back(0.6);
  BinsR_0L.push_back(0.7);
  BinsR_0L.push_back(0.8);
  BinsR_0L.push_back(0.85);
  BinsR_0L.push_back(0.9);
  BinsR_0L.push_back(0.925);
  BinsR_0L.push_back(0.95);
  BinsR_0L.push_back(0.975);
  BinsR_0L.push_back(1.);
  BinsR_0L.push_back(1.05);
  FitBin Bin_0L(BinsR_0L, BinsM_0L);

  vector<double> BinsM_1L;
  BinsM_1L.push_back(0.);
  BinsM_1L.push_back(5.);
  BinsM_1L.push_back(10.);
  BinsM_1L.push_back(20.);
  BinsM_1L.push_back(50.);
  BinsM_1L.push_back(100.);
  BinsM_1L.push_back(200.);
  BinsM_1L.push_back(400.);
  BinsM_1L.push_back(600.);
  vector<double> BinsR_1L;
  BinsR_1L.push_back(0.8);
  BinsR_1L.push_back(0.85);
  BinsR_1L.push_back(0.9);
  BinsR_1L.push_back(0.925);
  BinsR_1L.push_back(0.95);
  BinsR_1L.push_back(0.975);
  BinsR_1L.push_back(1.);
  BinsR_1L.push_back(1.05);
  //FitBin Bin_1L(BinsR_1L, BinsM_1L);

  vector<double> BinsM_2L;
  BinsM_2L.push_back(0.);
  BinsM_2L.push_back(5.);
  BinsM_2L.push_back(10.);
  BinsM_2L.push_back(20.);
  BinsM_2L.push_back(50.);
  BinsM_2L.push_back(100.);
  BinsM_2L.push_back(200.);
  BinsM_2L.push_back(400.);
  BinsM_2L.push_back(600.);
  vector<double> BinsR_2L;
  BinsR_2L.push_back(0.8);
  BinsR_2L.push_back(0.85);
  BinsR_2L.push_back(0.9);
  BinsR_2L.push_back(0.925);
  BinsR_2L.push_back(0.95);
  BinsR_2L.push_back(0.975);
  BinsR_2L.push_back(1.);
  BinsR_2L.push_back(1.05);
  //FitBin Bin_2L(BinsR_2L, BinsM_2L);

  
  cout << "Building 0L" << endl;
  Cats_0L = Cats_0L.CreateFitBinRegions(Bin_0L); //
  Cats_0L = Cats_0L.CreateHadronicSRegions(H_0L);
  Cats_0L = Cats_0L.CreateHadronicISRRegions(H_ISR);
  Cats_0L = Cats_0L.CreateGenericRegions("PTISR", PTISR_0L);

  cout << "Building 1L" << endl;
  Cats_1L = Cats_1L.CreateFitBinRegions(Bin_0L);
  Cats_1L = Cats_1L.CreateLeptonIDRegions(3, 2);
  Cats_1L = Cats_1L.CreateHadronicSRegions(H_1L);
  Cats_1L = Cats_1L.CreateHadronicISRRegions(H_ISR);
  Cats_1L = Cats_1L.CreateGenericRegions("PTISR", PTISR_1L);

  cout << "Building 2L0L" << endl;
  Cats_2LOS = Cats_2LOS.CreateFitBinRegions(Bin_0L);
  Cats_2LOS = Cats_2LOS.CreateLeptonIDRegions(3, 2);
  Cats_2LOS = Cats_2LOS.CreateHadronicSRegions(H_2LOS);
  Cats_2LOS = Cats_2LOS.CreateHadronicISRRegions(H_ISR);
  Cats_2LOS = Cats_2LOS.CreateGenericRegions("PTISR", PTISR_2LOS);

  cout << "Building 2LSS" << endl;
  Cats_2LSS = Cats_2LSS.CreateFitBinRegions(Bin_0L);
  Cats_2LSS = Cats_2LSS.CreateLeptonIDRegions(3, 2);
  Cats_2LSS = Cats_2LSS.CreateHadronicSRegions(H_2LSS);
  Cats_2LSS = Cats_2LSS.CreateHadronicISRRegions(H_ISR);
  Cats_2LSS = Cats_2LSS.CreateGenericRegions("PTISR", PTISR_2LSS);
  
  //////////////////////////////////////////////////////

  g_Categories += Cats_0L;
  g_Categories += Cats_1L;
  g_Categories += Cats_2LOS;
  g_Categories += Cats_2LSS;
}

void InitializeSystematics(){
  g_Systematics.push_back(&Systematic::Default()); // no systematic

  g_Systematics.push_back(new Systematic("PU_SF"));
  g_Systematics.push_back(new Systematic("BTAG_SF"));
}
