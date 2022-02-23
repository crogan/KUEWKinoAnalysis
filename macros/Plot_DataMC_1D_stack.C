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
#include <TColor.h>
#include <TColorWheel.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TLorentzVector.h>

#include "../include/ReducedBase_slim.hh"
#include "../include/SampleTool.hh"
#include "../include/CategoryTool.hh"
#include "../include/ScaleFactorTool.hh"
#include "../include/Leptonic.hh"
#include "../include/Hadronic.hh"
#include "../include/FitReader.hh"

#include "RestFrames/RestFrames.hh"

using namespace std;

using namespace RestFrames;

string g_PlotTitle;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;

using namespace RestFrames;

void Plot_DataMC_1D_stack(){
  RestFrames::SetStyle();

  
   RestFrames::SetStyle();

  string NtuplePath = "/home/t3-ku/z374f439/storage/crogan/";

  cout << "Initializing sample maps from path " << NtuplePath << " for year " << 2017 << endl;
  SampleTool ST(NtuplePath, 2017);
  
  ScaleFactorTool SF;
  CategoryTool CT;
  CategoryList Categories = CT.GetCategories();

  CategoryTreeTool CTTool;
  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();

  //int depth0 = CT_1L.GetDepth();
  //vector<const CategoryTree*> CTs;
  //CT_1L.GetListDepth(CTs, depth0-2);

  //int iCAT = 2;
  //const CategoryTree* myCT = CTs[iCAT];
  //const CategoryTree* myCT = &CT_0L;
  //Categories = Categories.Filter(CT_2L);

  
  ProcessList backgrounds = ST.Get(kBkg);

  string g_Label = "PreSelection";


  g_Xname = "pT_{ISR}";
  g_Xmin = 250.;
  g_Xmax = 1000.; 
  g_NX = 100;

  double lumi = 41.529152060;


  
   ProcessList samples = backgrounds;
  ProcessList data = ST.Get(kData);
  samples += data;

   g_PlotTitle = samples[0].Name();
   
  int Nsample = samples.GetN();
  TH1D* hist[Nsample];
  
  for(int s = 0; s < Nsample; s++){
    Process proc = samples[s];
    
    string title = proc.Name();
    hist[s] = new TH1D(title.c_str(),title.c_str(),g_NX,g_Xmin,g_Xmax);

    bool is_data   = (proc.Type() == kData);
    bool is_bkg    = (proc.Type() == kBkg);
    bool is_signal = (proc.Type() == kSig);
    
    int Nfile = ST.NTrees(proc);
    
    cout << "Processing " << Nfile << " files for process " << title << endl;
    for(int f = 0; f < Nfile; f++){
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
	
	if(base->PTISR < 250.)
	  continue;

	if(base->RISR < 0.45 || base->RISR > 1.0)
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

	// 2L/3L stuff
	//g_Label = "2L J X";
	//if(Nlep != 2)
	//  continue;
	
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
	  //LepSource source = LepSource(base->SourceID_lep->at(index));
	  LepSource source = LepSource(base->ID_lep->at(index*2+1));
	    
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

	// SV eta
	double SVmaxeta = 0.0001; // 1 is fine b/c less than 1.5 cutoff
	for(int ie = 0; ie < base->NSV_S; ie++)
	  if(fabs(base->Eta_SV->at(ie)) > SVmaxeta)
	    SVmaxeta = fabs(base->Eta_SV->at(ie));

	double sveta = SVmaxeta;

	// gammaT calc
	double MST =
	  sqrt(base->MX3a_BoostT*base->MX3a_BoostT+base->PX3_BoostT*base->PX3_BoostT) +
	  sqrt(base->MX3b_BoostT*base->MX3b_BoostT+base->PX3_BoostT*base->PX3_BoostT);

	double gammaT = 2.*base->Mperp / MST;
	double Mperp = base->Mperp;
	double RISR = base->RISR;
	double PTISR = base->PTISR;

	Category Event(Leptonic(list_a, list_b),
		       Hadronic(NjetS, NbjetS, NSV),
		       Hadronic(NjetISR, NbjetISR, base->NSV_ISR));
	Event.AddGenericVal(GenericVal(base->PTISR));
	Event.AddGenericVal(double(gammaT));
	Event.AddGenericVal(SVmaxeta);
	
	int eindex = Categories.Find(Event);
	if(eindex < 0){
	  continue;
	}	

	/////////////////
	
	double weight = (base->weight != 0.) ? base->weight : 1.;
        if(!is_data)
          weight *= double(SKIP)*lumi;

//if(e > 5) break;	
	hist[s]->Fill(PTISR, weight);
      }
      
      delete base;
      delete chain;
    }
  }

  TH1D* h_BKG = nullptr;
  bool isBKG = false;

  for(int s = 0; s < Nsample; s++){
    Process proc = samples[s];
    
    bool is_data   = (proc.Type() == kData);
    bool is_bkg    = (proc.Type() == kBkg);
    bool is_signal = (proc.Type() == kSig);
    if(is_data) continue;
    
    if(is_bkg){
     if(!isBKG){
       h_BKG = (TH1D*)hist[s]->Clone("TOT_BKG");
       isBKG = true;
     } else {
       for(int k = 0; k < s; k++){
         hist[k]->Add(hist[s]);
       }
       h_BKG->Add(hist[s]);
      }
    }
  }

  double fmax = -1.;
  int imax = -1;
  for(int i = 0; i < Nsample; i++){
    if(hist[i]->GetMaximum() > fmax){
      fmax = hist[i]->GetMaximum();
      imax = i;
    }
  }

  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = (TCanvas*) new TCanvas("can","can",700.,600);

  can->SetLeftMargin(0.15);
  can->SetRightMargin(0.26);
  can->SetBottomMargin(0.15);
  can->SetGridx();
  can->SetGridy();
  can->SetLogy();
  can->Draw();
  can->cd();
  hist[imax]->Draw("hist");
  hist[imax]->GetXaxis()->CenterTitle();
  hist[imax]->GetXaxis()->SetTitleFont(42);
  hist[imax]->GetXaxis()->SetTitleSize(0.06);
  hist[imax]->GetXaxis()->SetTitleOffset(1.06);
  hist[imax]->GetXaxis()->SetLabelFont(42);
  hist[imax]->GetXaxis()->SetLabelSize(0.05);
  hist[imax]->GetXaxis()->SetTitle(g_Xname.c_str());
  hist[imax]->GetYaxis()->CenterTitle();
  hist[imax]->GetYaxis()->SetTitleFont(42);
  hist[imax]->GetYaxis()->SetTitleSize(0.06);
  hist[imax]->GetYaxis()->SetTitleOffset(1.12);
  hist[imax]->GetYaxis()->SetLabelFont(42);
  hist[imax]->GetYaxis()->SetLabelSize(0.05);
  hist[imax]->GetYaxis()->SetTitle("N_{events}");

  for(int i = 0; i < Nsample; i++){
    Process proc = samples[i];
    if(proc.Type() == kBkg){
      if(proc.Name() == "ttbar")
      {
        hist[i]->SetLineColor(7011);
        hist[i]->SetFillColor(7011);
      }
      else if(proc.Name() == "ST")
      {
        hist[i]->SetLineColor(7010);
        hist[i]->SetFillColor(7010);
      }
      else if(proc.Name() == "ZDY")
      {
        hist[i]->SetLineColor(7000);
        hist[i]->SetFillColor(7000);
      }
      else if(proc.Name() == "DB")
      {
        hist[i]->SetLineColor(7051);
        hist[i]->SetFillColor(7051);
      }
      else if(proc.Name() == "TB")
      {
        hist[i]->SetLineColor(7050);
        hist[i]->SetFillColor(7050);
      }
      else if(proc.Name() == "Wjets")
      {
        hist[i]->SetLineColor(7001);
        hist[i]->SetFillColor(7001);
      }
      else if(proc.Name() == "QCD")
      {
        hist[i]->SetLineColor(7023);
        hist[i]->SetFillColor(7023);
      }
      hist[i]->SetLineWidth(1.0);
      hist[i]->SetFillStyle(1001);
      hist[i]->Draw("SAME HIST");
    }
  }

  if(isBKG){
    h_BKG->SetLineWidth(3.0);
    h_BKG->SetLineColor(kRed);
    h_BKG->SetMarkerSize(0);
    h_BKG->Draw("SAME HIST");
  }

  for(int i = 0; i < Nsample; i++){
    Process proc = samples[i];
    if(proc.Type() == kData){
      hist[i]->SetLineColor(kBlack);
      hist[i]->SetLineWidth(1.0);
      hist[i]->SetFillStyle(1001);
      hist[i]->Draw("SAME");
    }
  }

  TLegend* leg = new TLegend(0.75,0.32,0.95,0.62);
  leg->SetTextFont(132);
  leg->SetTextSize(0.044);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  if(isBKG) leg->AddEntry(h_BKG, "SM total");
  for(int i = 0; i < Nsample; i++){
    Process proc = samples[i];
    if(proc.Type() == kData)
      leg->AddEntry(hist[i],"Data");
    else
      if(proc.Name() == "ttbar")
        leg->AddEntry(hist[i],"t #bar{t} + X","F");
      else if(proc.Name() == "ST")
        leg->AddEntry(hist[i],"single top","F");
      else if(proc.Name() == "DB")
        leg->AddEntry(hist[i],"di-bosons","F");
      else if(proc.Name() == "TB")
        leg->AddEntry(hist[i],"tri-bosons","F");
      else if(proc.Name() == "ZDY")
        leg->AddEntry(hist[i],"Z / #gamma* + jets","F");
      else if(proc.Name() == "Wjets")
        leg->AddEntry(hist[i],"W + jets","F");
      else if(proc.Name() == "QCD")
        leg->AddEntry(hist[i],"QCD multi-jets","F");
  }
  leg->SetLineColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->Draw("SAME");

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  // l.DrawLatex(0.17,0.855,g_PlotTitle.c_str());
  //l.DrawLatex(0.71,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.01,0.943,"#bf{CMS} Simulation Preliminary");

  l.SetTextSize(0.045);
  l.SetTextFont(42);
  l.DrawLatex(0.7,0.04,g_Label.c_str());

  can->SaveAs("Plot1D_DataMC_Stack.pdf");

  gApplication->Terminate(0);
}
