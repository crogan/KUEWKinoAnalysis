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
#include "FitInputBuilder.hh"
#include "Systematics.hh"
#include "SampleTool.hh"
#include "CategoryTool.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"

using namespace std;

int main(int argc, char* argv[]) {
  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_12_05_20/";
  string OutFile    = "BuildFitInput_output.root";

  bool bprint = false;
  int  year   = 2017;
  bool addBkg  = false;
  bool addSig  = false;
  bool addData = false;
  vector<string> proc_to_add;

  
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

  CategoryTool CT;

  CategoryList Categories = CT.GetCategories();

  cout << "Categories:" << endl;
  Categories.Print();

  SystematicsTool SYS;

  Systematics systematics(1);
  systematics += SYS.GetWeightSystematics();

  FitInputBuilder FITBuilder;

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
      
      cout << "   Processing file " << file << " w/ tree " << tree << endl;
    
      TChain* chain = ST.Tree(proc, f);

      ReducedBase* base = new ReducedBase(chain);

      int Nentry = base->fChain->GetEntries();

      int SKIP = 1;

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
	  
	if(base->PTISR < 200.)
	  continue;
	  
	if(base->PTCM/base->PTISR > 0.2)
	  continue;
	  
	if(base->RISR < 0.5)
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
	    
	  int PDGID  = base->PDGID_lep->at(index);
	  int gindex = is_data ? -1 :  base->Index_lep->at(index);
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
	for(int i = 0; i < base->Nlep_b; i++){
	  index = (*base->index_lep_b)[i];
	  
	  int PDGID  = base->PDGID_lep->at(index);
	  int gindex = is_data ? -1 :  base->Index_lep->at(index);
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
	Event.AddGenericVal(GenericVal(base->PTISR));
	
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
	    if(sys == Systematic("PU_SF"))
	      if(sys.IsUp())
		weight *= base->PUweight_up/base->PUweight;
	      else
		weight *= base->PUweight_down/base->PUweight;
	    else
	      weight *= 1.;
	    //weight *= base->PUweight;
	  }
	  
	  LepList Fakes  = list_a.GetFakes(kHF);
	  Fakes         += list_b.GetFakes(kHF);
	  
	  double Mperp = base->Mperp;
	  
	  // use Eperp
	  if((Nlep == 1) && (NjetS == 0) && (NSV == 0))
	    Mperp = base->EL_BoostT;
	  if(((Nlep == 0) && (NjetS == 0) && (NSV == 1)) ||
	     ((Nlep == 0) && (NjetS == 1) && (NSV == 0)))
	    Mperp = base->EJ_BoostT;
	
	  double RISR  = base->RISR;

	  if((std::isnan(RISR) || std::isnan(Mperp)) && Nlep > 1)
	    cout << RISR << " " << base->Mperp << " Nlepa=" << base->Nlep_a << " Nlepb=" << base->Nlep_b << " NjetS=" << base->Njet_S << " Njetb=" << base->Njet_b << endl;

	  if(Fakes.GetN() > 0 && is_bkg){
	    vector<string> flabels = Fakes.GetFakeLabels(kHF);
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
	}
      }
      delete base;
      delete chain;
    }
  }

  FITBuilder.WriteFit(OutFile);
  
}
