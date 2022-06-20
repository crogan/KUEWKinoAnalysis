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
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TH2D.h>
#include <TTree.h>
#include <TChain.h>
#include <TList.h>
#include <TLeafElement.h>
#include <TLorentzVector.h>
//#include <TIter.h>
#include <TKey.h>

#include "RestFrames/RestFrames.hh"

#include "ReducedBase_slim.hh"
#include "FitInputBuilder.hh"
#include "Systematics.hh"
#include "SampleTool.hh"
#include "CategoryTool.hh"
#include "ScaleFactorTool.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"
#include "METTriggerTool.hh"

using namespace RestFrames;
using namespace std;
int main() {
RestFrames::SetStyle();
  int ifile = -1;
  //string NtuplePath = "root://xrootd.unl.edu//store/user/zflowers/crogan/";
  string NtuplePath = "root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v0/";
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

  bool cat0L = false;
  bool cat1L = false;
  bool cat2L = false;
  bool cat3L = false;

  bool setLumi = false;
  double lumi;

  bool doSys = false;

  bool maskSR = false;
  
      
  if((proc_to_add.size() == 0) &&
     (addBkg  == false) &&
     (addSig  == false) &&
     (addData == false))
    bprint = true;


  if(cat0L)
    Categories += CT.GetCategories_0L(maskSR);
  if(cat1L)
    Categories += CT.GetCategories_1L(maskSR);
  if(cat2L)
    Categories += CT.GetCategories_2L(maskSR);
  if(cat3L)
    Categories += CT.GetCategories_3L(maskSR);
  
  cout << "Initializing sample maps from path " << NtuplePath << " for year " << year << endl;
  
  SampleTool ST(NtuplePath, year);

  ScaleFactorTool SF;
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
cout << "# processes: " << samples.GetN() << endl;
  if(Categories.GetN() == 0)
    Categories += CT.GetCategories(maskSR);
  
  //if a lepton region wasn't specified, turn them all on 
  if(!cat0L && !cat1L && !cat2L && !cat3L){
    cat0L = true;
    cat1L = true;
    cat2L = true;
    cat3L = true;
  }
  
  // cout << "Categories:" << endl;
  // Categories.Print();

  SystematicsTool SYS;
  Systematics systematics(1);

  int underflow = 0.;
 ProcessList backgrounds = ST.Get(kBkg);

VS vsignals;
  vsignals.a("TChiWZ_4000350");

ProcessList signals;
  for(auto s : vsignals)
    signals += ST.Get(s);

samples = backgrounds;
//samples = signals;

string  g_Yname = "p_{T}^{ISR} [GeV]";
double  g_Ymin = 250.;
double  g_Ymax = 800.;
string   g_Xname = "R_{ISR}";
double   g_Xmin = 0.5;
double   g_Xmax = 1.;
//string  g_Yname = "#gamma_{T}";
//double  g_Ymin = 0.;
//double  g_Ymax = 1.;
//string   g_Xname = "M_{#perp}";
//double   g_Xmin = 0.;
//double   g_Xmax = 180.;
double   g_NX = 32;
double  g_NY = 32;
string g_PlotTitle = "Region 2L J X";
string g_Label;
if(samples[0].Name() == "DB") g_Label = "total background";
else g_Label = samples[0].Name();

TH2D* hist = new TH2D("hist", "hist",
                        g_NX, g_Xmin, g_Xmax,
                        g_NY, g_Ymin, g_Ymax);



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
      if(ifile != -1)
        f = ifile;
      string file = ST.FileName(proc, f);
      string tree = ST.TreeName(proc, f);

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
      int Nentry = base->fChain->GetEntries();
     cout << "Nentry: " << Nentry << endl;
if(chain == nullptr) cout << "chain null" << endl;
if(base == nullptr) cout << "base null" << endl; 
      int SKIP = 1;
      // event loop
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	 cout << "      event " << e << " | " << Nentry  << endl;
//	cout << "event #" << e << " run #" << base->runnum << endl;
	if(!base->EventFilter)	  
		continue;
//cout << "passed EventFilter" << endl;
//if(base->runnum != 0){cout << "EventFilter cut: " << base->runnum << endl;}	

        if(base->runnum >= 319077 && is_data && year == 2018)
          if(base->HEM_Veto)
            continue;
	if(do_FilterDilepton)
	  if(SF.DileptonEvent(base))
	    continue;
//cout << "passed dilepton event filter" << endl;
//if(base->runnum != 0){cout << "dilepton filter cut: " << base->runnum << endl;}	
	// apply trigger to data and FullSim events
	if(!base->METORtrigger && !is_FastSim)
	  continue;
//cout << "event #" << e << " MET:" << base->MET << endl;
//cout << "passed MET Trigger or is not FastSim" << endl;
//if(base->runnum != 0){cout << "met trigger cut: " << base->runnum << endl;}	
	if(base->MET < 150)
	  continue;
//if(base->runnum != 0){cout << "met cut: " << base->runnum << endl;}	
	  
	if(base->PTISR < 250.)
	  continue; 
//if(base->runnum != 0){cout << "ptisr cut: " << base->runnum << endl;}	
 
	if(base->RISR < 0.45)
	  continue;
//if(base->runnum != 0){cout << "risr cut: " << base->runnum << endl;}	
	double x = fabs(base->dphiCMI);
	
	if(base->PTCM > 200.)
	  continue;
//cout << "ptcm cut " << endl;	
	if(base->PTCM > -500.*sqrt(std::max(0.,-2.777*x*x+1.388*x+0.8264))+575. &&
	   -2.777*x*x+1.388*x+0.8264 > 0.)
	  continue;
//cout << "dphicmi cut" << endl;	
	if(base->PTCM > -500.*sqrt(std::max(0.,-1.5625*x*x+7.8125*x-8.766))+600. &&
	   -1.5625*x*x+7.8125*x-8.766 > 0.)
	  continue;
//if(base->runnum != 0){cout << "dphicmi cut 2: " << base->runnum << endl;}	
	  
	if(base->RISR < 0.45 || base->RISR > 1.0)
	  continue;
//if(base->runnum != 0){cout << "risr cut 2: " << base->runnum << endl;}	
	if(fabs(base->dphiMET_V) > acos(-1.)/2.)
	  continue;
//cout << "passed analysis selection" << endl;
//if(base->runnum != 0){cout << "dphiMET_V cut: " << base->runnum << endl;}	
	int Nlep     = base->Nlep;
	int NjetS    = base->Njet_S;
	int NbjetS   = base->Nbjet_S;
	int NjetISR  = base->Njet_ISR;
	int NbjetISR = base->Nbjet_ISR;
	int NSV      = base->NSV_S;
	if(Nlep + NjetS + NSV < 1)
	  continue;
//cout <<"passed object count cut" << endl;

//lepton # selection
if(Nlep != 2) continue;

//if(base->runnum != 0){cout << "object counting cut: " << base->runnum << endl;}	
	LepList list_a;
	LepList list_b;
//if(e % 1000 == 0 && base->runnum != 0) cout << "runnum: " << base->runnum << endl;	
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
	  //LepSource source = LepSource(base->SourceID_lep->at(index));
	  LepSource source = LepSource(base->ID_lep->at(index*2+1)); // fix for current ntuple version
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
	  LepSource source = LepSource(base->ID_lep->at(index*2+1)); // fix for current ntuple version
	  
	  list_b += Lep(flavor, charge, id, source);
	}

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
cout << "passed eindex " << eindex << endl;
	  if(Nlep > 3)
	    continue;
cout << "passed Nlep " << Nlep << endl;
	  if(base->PTISR < 250. && Nlep >= 2)
	     continue;
cout << "passed 2+L PTISR " << Nlep << " " << base->PTISR << endl;
	  if(base->PTISR < 400. && Nlep < 2)
	     continue;
cout << "passed 0-1L PTISR " << Nlep << " " << base->PTISR << endl;
cout << "passed lepton specific PTISR cuts" << endl;
	  int Nbron = 0;
	  int Nslvr = 0;
	  for(int i = 0; i < Nlep; i++){
	    if(i < base->Nlep_a){
	      if(list_a[i].ID() == kBronze)
		Nbron++;
	      if(list_a[i].ID() == kSilver)
		Nslvr++;
	    } else {
	      if(list_b[i-base->Nlep_a].ID() == kBronze)
		Nbron++;
	      if(list_b[i-base->Nlep_a].ID() == kSilver)
		Nslvr++;
	    }
	  }
	  if(Nbron >= 2)
	    continue;
	  if(Nbron+Nslvr >= 3)
	    continue;
	  cout << "Nlep = " << Nlep << " PTISR = " << base->PTISR << " NjetS = " << NjetS << " NSV = " << NSV << endl;

	  for(int i = 0; i < Nlep; i++){
	    if(i < base->Nlep_a)
	      cout << list_a[i].ID() << " " << list_a[i].IDLabel() << " " << list_a[i].Charge() << " a" << endl;
	    else
	      cout << list_b[i-base->Nlep_a].ID() << " " << list_b[i-base->Nlep_a].IDLabel() << " " << list_b[i-base->Nlep_a].Charge() << " b" << endl;
	//     int Nlep     = base->Nlep;
	// int NjetS    = base->Njet_S;
	// int NbjetS   = base->Nbjet_S;
	// int NjetISR  = base->Njet_ISR;
	// int NbjetISR = base->Nbjet_ISR;
	// int NSV      = base->NSV_S;
	  }
   	  
	  continue;
	}

	// systematics loop
	for(int is = 0; is < Nsys; is++){
	  Systematic& sys = systematics[is];
//cout << "systematic: " << sys.Label()<< endl;	
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
	   //cout << "sample tool lumi: " << ST.Lumi() << " ntuple weight:" << base->weight << " sample weight:" << sample_weight << endl; 
	    if(sys == Systematic("MET_TRIG"))
            {
/*
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
*/	    
             // HERE BE DRAGONS

//                         ^    ^
//                        / \  //\
//          |\___/|      /   \//  .\
//          /O  O  \__  /    //  | \ \
//         /     /  \/_/    //   |  \  \
//         @___@'    \/_   //    |   \   \ 
//            |       \/_ //     |    \    \ 
//            |        \///      |     \     \ 
//           _|_ /   )  //       |      \     _\
//          '/,_ _ _/  ( ; -.    |    _ _\.-~        .-~~~^-.
//          ,-{        _      `-.|.-~-.           .~         `.
//           '/\      /                 ~-. _ .-~      .-~^-.  \
//              `.   {            }                   /      \  \
//            .----~-.\        \-'                 .~         \  `. \^-.
//           ///.----..>    c   \             _ -~             `.  ^-`   ^-_
//             ///-._ _ _ _ _ _ _}^ - - - - ~                     ~--,   .-~
//                                                                   /.-'
//         
             METTriggerTool m_METTriggerTool;
             m_METTriggerTool.BuildMap("Parameters.csv");
             if(sys.IsUp())
               if(is_FastSim)
                 weight *= m_METTriggerTool.Get_EFF(base->MET, base->PTISR, year, (base->Nele > 0), (base->Nmu > 0), false, 1)*
                 m_METTriggerTool.Get_SF(base->MET, base->PTISR, year, (base->Nele > 0), (base->Nmu > 0), false, 1);
               else
                 weight *= m_METTriggerTool.Get_SF(base->MET, base->PTISR, year, (base->Nele > 0), (base->Nmu > 0), false, 1);
             else
               if(is_FastSim)
                 weight *= m_METTriggerTool.Get_EFF(base->MET, base->PTISR, year, (base->Nele > 0), (base->Nmu > 0), false, -1)*
                 m_METTriggerTool.Get_SF(base->MET, base->PTISR, year, (base->Nele > 0), (base->Nmu > 0), false, -1);
               else
                 weight *= m_METTriggerTool.Get_SF(base->MET, base->PTISR, year, (base->Nele > 0), (base->Nmu > 0), false, -1);
            }
//	    if(sys == Systematic("BTAGHF_SF"))
//	      if(sys.IsUp())
//		weight *= base->BtagSFweight_up;
//	      else
//		weight *= base->BtagSFweight_down;
//	    else 
//	      weight *= base->BtagSFweight;
//
//	    if(sys == Systematic("BTAGLF_SF"))
//	      if(sys.IsUp())
//		weight *= base->BtagSFweight_up;
//	      else
//		weight *= base->BtagSFweight_down;
//	    else 
//	      weight *= base->BtagSFweight;

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
	    Mperp = 2.*base->EL_BoostT;
	  if((Nlep == 0) && (NjetS == 0) && (NSV == 1))
	    Mperp = 2.*base->EJ_BoostT;
	  if((Nlep == 0) && (NjetS == 1) && (NSV == 0))
	    Mperp = 2.*base->EJ_BoostT;
	 

 
	  double RISR  = base->RISR;
	  double PTISR = base->PTISR;
hist->Fill(RISR,PTISR,weight);	


	}
      }
      delete base;
      delete chain;
      if(ifile != -1)
        break;
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
  hist->SetTitle("");
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
  hist->GetZaxis()->SetRangeUser(0.9*hist->GetMinimum(0.0),1.01*hist->GetMaximum());



TLatex l;
  l.SetTextFont(132);
  l.SetNDC();
//  l.SetTextSize(0.05);
//  l.SetTextFont(132);
//  l.DrawLatex(0.42,0.94,g_Label.c_str());
  l.SetTextSize(0.03);
  l.SetTextFont(42);

l.DrawLatex(0.71,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.01,0.943,"#bf{CMS} work-in-progress");
l.SetTextSize(0.045);
  l.SetTextFont(42);
  l.DrawLatex(0.7,0.04,g_Label.c_str());

  can->SaveAs("can.pdf");
  TFile* file = new TFile("output_Plot_2D.root","RECREATE");
  can->Write();
  file->Close();
  
return 0;




}
