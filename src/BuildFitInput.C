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
#include <ROOT/RDataFrame.hxx>



//#include "ReducedBase_slim.hh"
#include "FitInputBuilder.hh"
#include "Systematics.hh"
#include "SampleTool.hh"
#include "CategoryTool.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"
#include "varWeights.hh"

using ROOT::RDataFrame;
using namespace std;
int main(int argc, char* argv[]) {
  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_21_09_20/";
  string OutFile    = "BuildFitInput_output.root";

  bool bprint = false;
  int  year   = 2017;
  bool addBkg  = false;
  bool addSig  = false;
  bool addData = false;
  bool extrahist = false;
  vector<string> proc_to_add;
  float PTvar;
  CategoryTool CT;
  CategoryList Categories;
  
  for(int i = 0; i < argc; i++){
    if(strncmp(argv[i],"--help", 6) == 0){
      bprint = true;
    }
    if(strncmp(argv[i],"-h", 2) == 0){
      bprint = true;
    }
    if(strncmp(argv[i],"-path", 5) == 0){
      i++;
      NtuplePath = string(argv[i]);
    }
    if(strncmp(argv[i],"-o", 2) == 0){
      i++;
      OutFile = string(argv[i]);
    }
    if(strncmp(argv[i],"--output", 8) == 0){
      i++;
      OutFile = string(argv[i]);
    }
    if(strncmp(argv[i],"-year", 5) == 0){
      i++;
      year = std::atoi(argv[i]);
    }
    if(strncmp(argv[i],"+proc", 5) == 0){
      i++;
      proc_to_add.push_back(argv[i]);
    }
    if(strncmp(argv[i],"++bkg", 5) == 0){
      addBkg = true;
    }
    if(strncmp(argv[i],"++sig", 5) == 0){
      addSig = true;
    }
    if(strncmp(argv[i],"++data", 6) == 0){
      addData = true;
    }
    if(strncmp(argv[i],"++all", 5) == 0){
      addBkg  = true;
      addSig  = true;
      addData = true;
    }
    if(strncmp(argv[i],"+hist", 5) == 0){
      extrahist  = true;
    }
    if(strncmp(argv[i],"+cat0L", 6) == 0){
      Categories += CT.GetCategories_0L();
    }
    if(strncmp(argv[i],"+cat1L", 6) == 0){
      Categories += CT.GetCategories_1L();
    }
    if(strncmp(argv[i],"+cat2L", 6) == 0){
      Categories += CT.GetCategories_2L();
    }
    if(strncmp(argv[i],"+cat3L", 6) == 0){
      Categories += CT.GetCategories_3L();
    }
  }
      
  if((proc_to_add.size() == 0) &&
     (addBkg  == false) &&
     (addSig  == false) &&
     (addData == false))
    bprint = true;

  if(bprint){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << "  options:" << endl;
    cout << "   --help(-h)          print options" << endl;
    cout << "   -path [dest]        path to input ntuples" << endl;
    cout << "   --ouput(-o) [file]  output root file" << endl;
    cout << "   -year [year]        year to process" << endl;
    cout << "   +proc [label]       add processes matching label (can have >1 of these)" << endl;
    cout << "   ++bkg               add all background samples for year" << endl;
    cout << "   ++sig               add all signal samples" << endl;
    cout << "   ++data              add all background samples" << endl;
    cout << "   ++all               add all samples" << endl;
    cout << "   +cat0L              add 0L categories" << endl;
    cout << "   +cat1L              add 1L categories" << endl;
    cout << "   +cat2L              add 2L categories" << endl;
    cout << "   +cat3L              add 3L categories" << endl;
    cout << "   +hist               book 2D histograms also" << endl;

    return 0;
  }

  cout << "Initializing sample maps from path " << NtuplePath << " for year " << year << endl;
  SampleTool ST(NtuplePath, year);

  ProcessList samples;
  if(addBkg){
    cout << "Adding all background processes" << endl;
    samples += ST.Get(kBkg);
  }
  if(addSig){
    cout << "Adding all signal processes" << endl;
    samples += ST.Get(kSig);
  }
  if(addData){
    cout << "Adding all data for year " << year << endl;
    samples += ST.Get(kData);
  }
  for(int p = 0; p < int(proc_to_add.size()); p++){
    cout << "Adding processes that match \"" << proc_to_add[p] << "\"" << endl;
    samples += ST.Get(proc_to_add[p]);
  }

  if(Categories.GetN() == 0)
    Categories += CT.GetCategories();
  
  // cout << "Categories:" << endl;
  // Categories.Print();

  SystematicsTool SYS;

  Systematics systematics(1);
  systematics += SYS.GetWeightSystematics();

  FitInputBuilder FITBuilder(extrahist);

  //set systematic event weight parameters
  varWeights vw("varWeights");
  double maxWeight = 5.;
  double minWeight = 0.01;
  vw.setMinMax(minWeight,maxWeight);

  // dummy in case there is no data requested
  Process data_obs("data_obs", kData);

  // sample (process) loop
  int Nsample = samples.GetN();
  for(int s = 0; s < Nsample; s++){
    Process proc = samples[s];
    
    string title = proc.Name();

    bool is_data   = (proc.Type() == kData);
    bool is_bkg    = (proc.Type() == kBkg);
    bool is_signal = (proc.Type() == kSig);

    int Nsys = (is_data ? 1 : systematics.GetN());
    
    int Nfile = ST.NTrees(proc);

    cout << "Processing " << Nfile << " files for process " << title << endl;
    for(int f = 0; f < Nfile; f++){ 
      string file = ST.FileName(proc, f);
      string tree = ST.TreeName(proc, f);

	//skip lowest HT slices for Wjets and Znunu
//	if(strstr(file.c_str(),"WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root")) continue;
//	if(strstr(file.c_str(),"WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root")) continue;      
//	if(strstr(file.c_str(),"ZJetsToNuNu_HT-100To200_13TeV-madgraph_Fall17_102X.root")) continue;      
      cout << "   Processing file " << file << " w/ tree " << tree << endl;
    
      TChain* chain = ST.Tree(proc, f);
      ReducedBase* base = new ReducedBase(chain);
//cout << "reducedBase" << endl;
      ROOT::RDataFrame d(*base->fChain);
//cout << "a" << endl;
      double absEta = 0;
	int nLep = 0;
//cout << "b" << endl;
	double ptMean = *d.Mean("PT_lep");
//cout << "pt mean" << endl;
      double isoMean = *d.Mean("MiniIso_lep");
//cout << "iso mean" << endl;      
d.Foreach([&absEta, &nLep](vector<double> Eta_lep) {for(int iLep = 0; iLep < Eta_lep.size(); iLep++){if(Eta_lep.at(iLep) < 0) absEta += -(Eta_lep.at(iLep)); else absEta += Eta_lep.at(iLep); nLep += Eta_lep.size();}}, {"Eta_lep"});
//cout << "eta mean 1" << endl;
	double etaMean = absEta/nLep; 
//cout << "eta mean 2" << endl;     
 double sip3dMean = *d.Mean("SIP3D_lep");
//cout << "sip3d mean" << endl;     
 int Nentry = base->fChain->GetEntries();
//cout << "setting means" << endl;

      int Nentry = base->fChain->GetEntries();
      
      int SKIP = 1;

      // event loop
      for(int e = 0; e < Nentry; e += SKIP){
	cout << "GetEntry" << endl;
	base->GetEntry(e);

	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

	// only apply trigger to data for moment
	if(!base->METORtrigger && is_data)
	  continue;
		
	if(base->MET < 175)
	  continue;
	  
	if(base->PTISR < 200.)
	  continue;

	// current cut
	if(base->PTCM > 75. && fabs(base->dphiCMI) < acos(-1.)/4.)
	  continue;
	if(base->PTCM > 50. && fabs(base->dphiCMI) > 3.*acos(-1.)/4.)
	  continue;
	  
	if(base->RISR < 0.6 || base->RISR > 1.0)
	  continue;

	
	int Nlep     = base->Nlep;
	int NjetS    = base->Njet_S;
	int NbjetS   = base->Nbjet_S;
	int NjetISR  = base->Njet_ISR;
	int NbjetISR = base->Nbjet_ISR;
	int NSV      = base->NSV_S;

	if(Nlep + NjetS + NSV < 1)
	  continue;
	LepList list_a;
	LepList list_b;
	  
	int index;
	for(int i = 0; i < base->Nlep_a; i++){
	  index = (*base->index_lep_a)[i];

	    
	  int PDGID = base->PDGID_lep->at(index);
	    
	  LepID id;
	  if(base->ID_lep->at(index) < 3 ||
	     base->MiniIso_lep->at(index)*base->PT_lep->at(index) >= 4. ||
	     base->RelIso_lep->at(index)*base->PT_lep->at(index) >= 4.)
	    id = kBronze;
	  else if(base->SIP3D_lep->at(index) > 2.)
	    id = kSilver;
	  else
	    id = kGold;
	  LepFlavor flavor;
	  if(abs(PDGID) == 11)
	    flavor = kElectron;
	  else
	    flavor = kMuon;
	  LepCharge charge = (base->Charge_lep->at(index) > 0 ? kPos : kNeg);
	  //LepSource source = LepSource(base->SourceID_lep->at(index));
	  LepSource source = LepSource(base->ID_lep->at(index*2+1));
	    
	  list_a += Lep(flavor, charge, id, source);
	}
	for(int i = 0; i < base->Nlep_b; i++){
	  index = (*base->index_lep_b)[i];
	  
	  int PDGID = base->PDGID_lep->at(index);

	  LepID id;
	  if(base->ID_lep->at(index) < 3 ||
	     base->MiniIso_lep->at(index)*base->PT_lep->at(index) >= 4. ||
	     base->RelIso_lep->at(index)*base->PT_lep->at(index) >= 4.)
	    id = kBronze;
	  else if(base->SIP3D_lep->at(index) > 2.)
	    id = kSilver;
	  else
	    id = kGold;
	  LepFlavor flavor;
	  if(abs(PDGID) == 11)
	    flavor = kElectron;
	  else
	    flavor = kMuon;
	  LepCharge charge = (base->Charge_lep->at(index) > 0 ? kPos : kNeg);
	  //LepSource source = LepSource(base->SourceID_lep->at(index));
	  LepSource source = LepSource(base->ID_lep->at(index*2+1));
	  
	  list_b += Lep(flavor, charge, id, source);
	}

	// SV eta
	double SVmaxeta = 1.; // 1 is fine b/c less than 1.5 cutoff
	for(int ie = 0; ie < base->NSV_S; ie++)
	  if(fabs(base->Eta_SV->at(ie)) > SVmaxeta)
	    SVmaxeta = fabs(base->Eta_SV->at(ie));
	
	Category Event(Leptonic(list_a, list_b),
		       Hadronic(NjetS, NbjetS, NSV),
		       Hadronic(NjetISR, NbjetISR, base->NSV_ISR));
	Event.AddGenericVal(GenericVal(base->PTISR));
	Event.AddGenericVal(SVmaxeta);
	
	int eindex = Categories.Find(Event);
	if(eindex < 0){
	  continue;
	}	
	// systematics loop
	
	for(int is = 0; is < Nsys; is++){
	  Systematic& sys = systematics[is];
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
	    weight = base->weight*ST.Lumi();
	    if(sys == Systematic("BTAG_SF"))
	      if(sys.IsUp())
		weight *= base->BtagSFweight_up;
	 else
		weight *= base->BtagSFweight_down;
	    else 
	      weight *= base->BtagSFweight;
	  if(sys == Systematic("lepPT_weight"))
	  	weight *= vw.expWeight(base,ptMean,base->PT_lep,sys);
	  if(sys == Systematic("lepIso_weight"))
	  	weight *= vw.expWeight(base,isoMean,base->MiniIso_lep,sys);
	  if(sys == Systematic("lepEta_weight"))
	  	weight *= vw.expWeight(base,etaMean, base->Eta_lep,sys);
	if(sys == Systematic("lepSIP3D_weight"))
	  	weight *= vw.expWeight(base,sip3dMean,base->SIP3D_lep,sys);

	    // turn off PU systematics for now
	    // if(sys == Systematic("PU_SF"))
	    //   if(sys.IsUp())
	    // 	weight *= base->PUweight_up;
	    //   else
	    // 	weight *= base->PUweight_down;
	    // else
	    //   weight *= base->PUweight;
	  }
	  
	  LepList Fakes  = list_a.GetFakes();
	  Fakes         += list_b.GetFakes();
	  
	  double Mperp = base->Mperp;
	  
	  // use Eperp
	  if((Nlep == 1) && (NjetS == 0) && (NSV == 0))
	    Mperp = base->EL_BoostT;
	  if(((Nlep == 0) && (NjetS == 0) && (NSV == 1)) ||
	     ((Nlep == 0) && (NjetS == 1) && (NSV == 0)))
	    Mperp = base->EJ_BoostT;
	
	  double RISR  = base->RISR;

	  if(Fakes.GetN() > 0 && is_bkg){
	    vector<string> flabels = Fakes.GetFakeLabels();
	    int Nf = flabels.size();
	    for(int fl = 0; fl < Nf; fl++){
	      if(title.find("QCD") == string::npos)
		FITBuilder.AddEvent(weight/double(Nf), Mperp, RISR,
				    Categories[eindex], FITBuilder.FakeProcess(flabels[fl]), sys);
	      
	      FITBuilder.AddEvent(weight/double(Nf), Mperp, RISR,
				  Categories[eindex], proc.FakeProcess(flabels[fl]), sys);
	    }
	  } else {
	    FITBuilder.AddEvent(weight, Mperp, RISR,
				Categories[eindex], proc, sys);
	  }
	  
	  // dummy data
	  if(!addData && is_bkg)
	    FITBuilder.AddEvent(weight, Mperp, RISR,
				Categories[eindex], data_obs, sys);
	}
      }
      delete base;
      delete chain;
    }
  }

  FITBuilder.WriteFit(OutFile);
  
}
