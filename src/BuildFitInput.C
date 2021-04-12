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
#include "ScaleFactorTool.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"
#include "CategoryTree.hh"
#include "FitReader.hh"
#include "shapeTemplate.hh"
//#include "varWeights.hh"
//#include "testClass.hh"

using ROOT::RDataFrame;
using namespace std;
int main(int argc, char* argv[]) {
  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_21_09_20/";
  string OutFile    = "BuildFitInput_output.root";

  bool doSigFile = false;
  string SigFile = "";

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

  bool setLumi = false;
  double lumi;
  
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
    if(strncmp(argv[i],"-lumi", 5) == 0){
      i++;
      setLumi = true;
      lumi = std::stof(argv[i]);
    }
    if(strncmp(argv[i],"-sigfile", 8) == 0){
      i++;
      doSigFile = true;
      SigFile = argv[i];
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
    cout << "   -lumi [lumi]        set luminosity to lumi" << endl;
    cout << "   -sigfile            signal filename must match this string to be included" << endl;

    return 0;
  }

  cout << "Initializing sample maps from path " << NtuplePath << " for year " << year << endl;
  SampleTool ST(NtuplePath, year);

  ScaleFactorTool SF;
  CategoryTreeTool CTTool;
  FitReader FITReader(OutFile);
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
  //systematics += SYS.GetWeightSystematics();

  FitInputBuilder FITBuilder(extrahist);

  //set systematic event weight parameters
  // varWeights vw("varWeights");
  // double maxWeight = 2.;
  // double minWeight = 0.01;
  // vw.setMinMax(minWeight,maxWeight);

  // dummy in case there is no data requested
  Process data_obs("data_obs", kData);

  //testClass tc(OutFile);

  // sample (process) loop
  int Nsample = samples.GetN();
  for(int s = 0; s < Nsample; s++){
    Process proc = samples[s];
    if(doSigFile && proc.Type() == kSig){
      bool keep = false;
      int Nfile = ST.NTrees(proc);
      for(int f = 0; f < Nfile; f++){
	string file = ST.FileName(proc, f);
	if(file.find(SigFile) != string::npos)
	  keep = true;
      }
      if(!keep)
	continue;
    }
    cout << "processing sample " << proc.Name() << endl;
  }
  
  for(int s = 0; s < Nsample; s++){
    Process proc = samples[s];

    if(doSigFile && proc.Type() == kSig){
      bool keep = false;
      int Nfile = ST.NTrees(proc);
      for(int f = 0; f < Nfile; f++){
	string file = ST.FileName(proc, f);
	if(file.find(SigFile) != string::npos)
	  keep = true;
      }
      if(!keep)
	continue;
    }
    
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
      bool is_FastSim = ST.IsFastSim(proc, f);
      bool do_FilterDilepton = ST.FilterDilepton(proc, f);
      double sample_weight = ST.GetSampleWeight(proc, f);

      if(is_signal)
	sample_weight *= SF.GetX20BRSF(file, tree);
      
      cout << "   Processing file " << file << " w/ tree " << tree << endl;
      cout << "      Sample weight is " << sample_weight << endl;
      if(is_FastSim)
	cout << "      Is FastSim" << endl;
      if(do_FilterDilepton)
	cout << "      Filter Out dilepton events" << endl;
    
      TChain* chain = ST.Tree(proc, f);
      ReducedBase* base = new ReducedBase(chain);
      
//variation stuff - not necessary for the time being
//       ROOT::RDataFrame d(*base->fChain);
//       double absEta = 0;
// 	int nLep = 0;
// //cout << "leading lep pt" << endl;
// //for(int i = 0; i < 10; i++){
// //	cout << "a" << endl;
// //	base->GetEntry(i);
// //	cout << i << endl;
// //	if(base->Nlep < 1) continue;
// //	cout << "leading lep pt: " << base->PT_lep->at(0) << endl;
// //}     
// //cout << "a" << endl;
// 	double ptMean = *d.Mean("PT_lep");
// //cout << "Met mean: " << *d.Mean("MET") << endl;
// //cout << "pt mean" << endl;
// //for(int i = 0; i < 10; i++){
// //	base->GetEntry(i);
// //	if(base->Nlep < 1) continue;
// //	cout << "Mini Iso: " << base->MiniIso_lep->at(0) << endl;
// //}     
//  double isoMean = *d.Mean("MiniIso_lep");
// d.Foreach([&absEta, &nLep](vector<double> Eta_lep) {for(int iLep = 0; iLep < Eta_lep.size(); iLep++){if(Eta_lep.at(iLep) < 0) absEta += -(Eta_lep.at(iLep)); else absEta += Eta_lep.at(iLep); nLep += Eta_lep.size();}}, {"Eta_lep"});
// 	double etaMean = absEta/nLep; 
//  double sip3dMean = *d.Mean("SIP3D_lep");

      int Nentry = 1e5;//base->fChain->GetEntries();
      
      int SKIP = 1;

      // event loop
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);

	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;

	if(do_FilterDilepton)
	  if(SF.DileptonEvent(base))
	    continue;
	// apply trigger to data and FullSim events
	if(!base->METORtrigger && !is_FastSim)
	  continue;
	if(base->MET < 150)
	  continue;
	if(base->PTISR < 200.)
	  continue;
	double x = fabs(base->dphiCMI);
	
	if(base->PTCM > 200.)
	  continue;
	if(base->PTCM > -500.*sqrt(std::max(0.,-2.777*x*x+1.388*x+0.8264))+575. &&
	   -2.777*x*x+1.388*x+0.8264 > 0.)
	  continue;
	if(base->PTCM > -500.*sqrt(std::max(0.,-1.5625*x*x+7.8125*x-8.766))+600. &&
	   -1.5625*x*x+7.8125*x-8.766 > 0.)
	  continue;
	  
	if(base->RISR < 0.45 || base->RISR > 1.0)
	  continue;
	if(fabs(base->dphiMET_V) > acos(-1.)/2.)
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
	  if(base->ID_lep->at(index*2) < 3 ||
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
	  LepSource source = LepSource(base->ID_lep->at(2*index+1));
	  list_a += Lep(flavor, charge, id, source);
	}
	for(int i = 0; i < base->Nlep_b; i++){
	  index = (*base->index_lep_b)[i];
	  int PDGID = base->PDGID_lep->at(index);
	  LepID id;
	  if(base->ID_lep->at(index*2) < 3 ||
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

//cout << "event #: " << e << " passed lep selection" << endl;	
	// SV eta
	double SVmaxeta = 1.; // 1 is fine b/c less than 1.5 cutoff
	for(int ie = 0; ie < base->NSV_S; ie++)
	  if(fabs(base->Eta_SV->at(ie)) > SVmaxeta)
	    SVmaxeta = fabs(base->Eta_SV->at(ie));

	// gammaT calc
	double MST =
	  sqrt(base->MX3a_BoostT*base->MX3a_BoostT+base->PX3_BoostT*base->PX3_BoostT) +
	  sqrt(base->MX3b_BoostT*base->MX3b_BoostT+base->PX3_BoostT*base->PX3_BoostT);
	double gammaT = 2.*base->Mperp / MST;
	Category Event(Leptonic(list_a, list_b),
		       Hadronic(NjetS, NbjetS, NSV),
		       Hadronic(NjetISR, NbjetISR, base->NSV_ISR));
	Event.AddGenericVal(GenericVal(base->PTISR));
	Event.AddGenericVal(gammaT);
	Event.AddGenericVal(SVmaxeta);
	int eindex = Categories.Find(Event);
	if(eindex < 0){
	  continue;
	}
		
//cout << "event #: " << e << " passed SV selection" << endl;	
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
	    weight = (setLumi ? lumi : ST.Lumi())*base->weight*sample_weight;
	    
	    if(sys == Systematic("MET_TRIG"))
	      if(sys.IsUp())
		if(is_FastSim)
		  weight *= SF.GetMETEff(base->MET, 1);
		else
		  weight *= SF.GetMETSF(base->MET, 1);
	      else
		if(is_FastSim)
		  weight *= SF.GetMETEff(base->MET, -1);
		else
		  weight *= SF.GetMETSF(base->MET, -1);
	    else 
	      if(is_FastSim)
		weight *= SF.GetMETEff(base->MET);
	      else
		weight *= SF.GetMETSF(base->MET);
	    
	    if(sys == Systematic("BTAG_SF"))
	      if(sys.IsUp())
		weight *= base->BtagSFweight_up;
	 else
		weight *= base->BtagSFweight_down;
	    else 
	      weight *= base->BtagSFweight;
	//   if(sys == Systematic("lepPT_weight"))
	//   	weight *= vw.expWeight(base,ptMean,base->PT_lep,sys);
	//   if(sys == Systematic("lepIso_weight"))
	//   	weight *= vw.expWeight(base,isoMean,base->MiniIso_lep,sys);
	//   if(sys == Systematic("lepEta_weight"))
	//   	weight *= vw.expWeight(base,etaMean, base->Eta_lep,sys);
	// if(sys == Systematic("lepSIP3D_weight"))
	//   	weight *= vw.expWeight(base,sip3dMean,base->SIP3D_lep,sys);

	    // turn off PU systematics for now
	    // if(sys == Systematic("PU_SF"))
	    //   if(sys.IsUp())
	    // 	weight *= base->PUweight_up;
	    //   else
	    // 	weight *= base->PUweight_down;
	    // else
	    //   weight *= base->PUweight;
	  }
//cout << "event #: " << e << " passed systematics" << endl;	
	  LepList Fakes  = list_a.GetFakes();
	  Fakes         += list_b.GetFakes();
	  
	  double Mperp = base->Mperp;
//	 cout << "event #: " << e <<  ", weight: " << weight << endl; 
	  // use Eperp
	  if((Nlep == 1) && (NjetS == 0) && (NSV == 0))
	    Mperp = 2.*base->EL_BoostT;
	  if((Nlep == 0) && (NjetS == 0) && (NSV == 1))
	    Mperp = 2.*base->EJ_BoostT;
	  if((Nlep == 0) && (NjetS == 1) && (NSV == 0))
	    Mperp = base->EJ_BoostT;
	  
	
	  double RISR  = base->RISR;

//cout << "event #: " << e << " passed RISR+Mperp assignment" << endl;	
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
	  if(!addData && is_bkg && (title.find("QCD") == string::npos))
	    FITBuilder.AddEvent(weight, Mperp, RISR,
				Categories[eindex], data_obs, sys);
	}
      }
      delete base;
      delete chain;
    }
  

  FITBuilder.WriteFit(OutFile);
VS fakeProcs;
for(int i = 0; i < int(proc_to_add.size()); i++){
	fakeProcs += proc_to_add[i]+"_Fakes_elf0";
	fakeProcs += proc_to_add[i]+"_Fakes_elf1";
	fakeProcs += proc_to_add[i]+"_Fakes_muf0";
	fakeProcs += proc_to_add[i]+"_Fakes_muf1";
//	fakeProcs += proc_to_add[i]+"_LF";
}
CategoryTree CT_Fakes1L = CTTool.GetCategories_Fakes1L();
CategoryTree CT_Fakes2L = CTTool.GetCategories_Fakes2L();
CategoryTree CT_Fakes3L = CTTool.GetCategories_Fakes3L();
// FITReader.SmoothHistograms(fakeProcs,CT_Fakes1L,OutFile);
shapeTemplateTool STT(OutFile,CT_Fakes1L,fakeProcs);
STT.createTemplates();
//FITReader.SmoothHistograms(fakeProcs,CT_Fakes2L,OutFile);
//FITReader.SmoothHistograms(fakeProcs,CT_Fakes3L,OutFile);
  
}
