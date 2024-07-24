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
#include <TH2D.h>
#include <TStyle.h>
#include <TLorentzVector.h>

#include "../include/CascadeBase.hh"
#include "../include/SampleTool.hh"
#include "../include/Leptonic.hh"
#include "../include/Hadronic.hh"

#include "RestFrames/RestFrames.hh"

using namespace std;


string g_PlotTitle;
string g_Label;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;
string g_Yname;
double g_Ymin;
double g_Ymax;
double g_NY;

using namespace RestFrames;
void Plot_Hist(TH2D* hist);

void Plot_2D_Cascade(){

  
   RestFrames::SetStyle();

  string NtuplePath = "/ospool/cms-user/zflowers/CMSSW_13_3_1/src/SUSYCascades/local_test_root_files/";

  SampleTool ST(NtuplePath, 2023);

  VS vsignals;
  // VS all = signals;
  // all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("Fakes");

  vsignals.a("Cascade_3000000");
  //vsignals.a("ttbar");
  
  //ProcessList backgrounds = ST.Get(kBkg).Remove("QCD");
  ProcessList backgrounds = ST.Get(kBkg).Filter("ttbar");

  ProcessList signals;
  for(auto s : vsignals)
    signals += ST.Get(s);
  
  g_Label = "No selection";
  //g_Label = "1 lepton";
  g_Ymin = 0.;
  g_Ymax = 200.;
  g_NY = 64;

  g_Xmin = 0.;
  g_Xmax = 200.; 
  g_NX = 64;

  
  int SKIP = 1;
  ProcessList samples = signals;
  samples += backgrounds;

   //g_PlotTitle = "#tilde{#chi}_{1}^{#pm} #tilde{#chi}_{2}^{0} -> #tilde{#chi}_{1}^{0} #tilde{#chi}_{1}^{0}, m_{#tilde{#chi}_{1}^{#pm}} = m_{#tilde{#chi}_{2}^{0}} = 275, m_{#tilde{#chi}_{1}^{0}} = 245";
   
  int Nsample = samples.GetN();
  vector<TH2D*> hists;  

  for(int s = 0; s < Nsample; s++){
    hists.clear();
    Process proc = samples[s];
    
    string title = proc.Name();
    g_PlotTitle = title;

    bool is_data   = (proc.Type() == kData);
    bool is_bkg    = (proc.Type() == kBkg);
    bool is_signal = (proc.Type() == kSig);
    
    int Nfile = ST.NTrees(proc);
    
    cout << "Processing " << Nfile << " files for process " << title << endl;
    TH2D* hist_MX3a_S_v_MX3b_S = new TH2D((title+"_MX3a_S_v_MX3b_S").c_str(), (title+"_MX3a_S_v_MX3b_S;c_MX3a_S;c_MX3b_S").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX3a_S_v_MX3b_S);
    TH2D* hist_MX3a_Vis_v_MX3b_Vis = new TH2D((title+"_MX3a_Vis_v_MX3b_Vis").c_str(), (title+"_MX3a_Vis_v_MX3b_Vis;c_MX3a_Vis;c_MX3b_Vis").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX3a_Vis_v_MX3b_Vis);
    TH2D* hist_MX2a_S_v_MX2b_S = new TH2D((title+"_MX2a_S_v_MX2b_S").c_str(), (title+"_MX2a_S_v_MX2b_S;MX2a_S;MX2b_S").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX2a_S_v_MX2b_S);
    TH2D* hist_MX2a_Vis_v_MX2b_Vis = new TH2D((title+"_MX2a_Vis_v_MX2b_Vis").c_str(), (title+"_MX2a_Vis_v_MX2b_Vis;MX2a_Vis;MX2b_Vis").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX2a_Vis_v_MX2b_Vis);
    TH2D* hist_MX3a_S_v_MX3a_Vis = new TH2D((title+"_MX3a_S_v_MX3a_Vis").c_str(), (title+"_MX3a_S_v_MX3a_Vis;MX3a_S;MX3a_Vis").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX3a_S_v_MX3a_Vis);
    TH2D* hist_MX3b_S_v_MX3b_Vis = new TH2D((title+"_MX3b_S_v_MX3b_Vis").c_str(), (title+"_MX3b_S_v_MX3b_Vis;MX3b_S;MX3b_Vis").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX3b_S_v_MX3b_Vis);
    TH2D* hist_MX2a_S_v_MX2a_Vis = new TH2D((title+"_MX2a_S_v_MX2a_Vis").c_str(), (title+"_MX2a_S_v_MX2a_Vis;MX2a_S;MX2a_Vis").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX2a_S_v_MX2a_Vis);
    TH2D* hist_MX2b_S_v_MX2b_Vis = new TH2D((title+"_MX2b_S_v_MX2b_Vis").c_str(), (title+"_MX2b_S_v_MX2b_Vis;MX2b_S;MX2b_Vis").c_str(), g_NX, g_Xmin, g_Xmax, g_NY, g_Ymin, g_Ymax);
    hists.push_back(hist_MX2b_S_v_MX2b_Vis);

    for(int f = 0; f < Nfile; f++){
      string file = ST.FileName(proc, f);
      string tree = ST.TreeName(proc, f);
      
      bool is_FastSim = ST.IsFastSim(proc, f);
      bool do_FilterDilepton = ST.FilterDilepton(proc, f);
      double sample_weight = ST.GetSampleWeight(proc, f);
      
      //if(is_signal)
      // sample_weight *= SF.GetX20BRSF(file, tree);
      
      cout << "   Processing file " << file << " w/ tree " << tree << endl;
      cout << "      Sample weight is " << sample_weight << endl;
      if(is_FastSim)
	cout << "      Is FastSim" << endl;
      if(do_FilterDilepton)
	cout << "      Filter Out dilepton events" << endl;
      
      TChain* chain = ST.Tree(proc, f);
      
      CascadeBase* base = new CascadeBase(chain);
      
      int Nentry = base->fChain->GetEntries(); 
      
      // event loop
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;
	
        //if(base->PTISR > 300.) continue;
        //if(base->MET < 50.) continue;
        //if(base->MET > 200.) continue;

	int Nlep     = base->Nlep;
	int NjetS    = base->Njet_S;
	int NbjetS   = base->Nbjet_S;
	int NjetISR  = base->Njet_ISR;
	int NbjetISR = base->Nbjet_ISR;

	LepList list_a;
	LepList list_b;
	  
	int index = 0;
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
	    flavor = LepFlavor::kElectron;
	  else
	    flavor = LepFlavor::kMuon;
	  LepCharge charge = (base->Charge_lep->at(index) > 0 ? kPos : kNeg);
	  LepSource source = LepSource(base->SourceID_lep->at(index));
	    
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
	    flavor = LepFlavor::kElectron;
	  else
	    flavor = LepFlavor::kMuon;
	  LepCharge charge = (base->Charge_lep->at(index) > 0 ? kPos : kNeg);
	  LepSource source = LepSource(base->SourceID_lep->at(index));
	  
	  list_b += Lep(flavor, charge, id, source);
	}

	double gammaT = base->gammaT;
	double Mperp = base->Mperp;
	double RISR = base->RISR;
	double PTISR = base->PTISR;

        // Cascades
        int c_Njet_a = base->c_Njet_a;
        int c_Njet_b = base->c_Njet_b;
        int c_Nlep_1a = base->c_Nlep_1a;
        int c_Nlep_1b = base->c_Nlep_1b;
        int c_Nlep_2b = base->c_Nlep_2b;

        int c_Nobj_a = c_Njet_a+c_Nlep_1a+1;
        int c_Nobj_b = c_Njet_b+c_Nlep_1b+c_Nlep_2b;
        //if(c_Nobj_a >= c_Nobj_b) continue;

	/////////////////
	
	double weight = 1.; //(base->weight != 0.) ? base->weight : 1.;

	hist_MX3a_S_v_MX3b_S->Fill(base->c_MX3a_S, base->c_MX3b_S, weight*double(SKIP));
	hist_MX3a_Vis_v_MX3b_Vis->Fill(base->c_MX3a_Vis, base->c_MX3b_Vis, weight*double(SKIP));
	hist_MX2a_S_v_MX2b_S->Fill(base->c_MX2a_S, base->c_MX2b_S, weight*double(SKIP));
	hist_MX2a_Vis_v_MX2b_Vis->Fill(base->c_MX2a_Vis, base->c_MX2b_Vis, weight*double(SKIP));
	hist_MX3a_S_v_MX3a_Vis->Fill(base->c_MX3a_S, base->c_MX3a_Vis, weight*double(SKIP));
	hist_MX3b_S_v_MX3b_Vis->Fill(base->c_MX3b_S, base->c_MX3b_Vis, weight*double(SKIP));
	hist_MX2a_S_v_MX2a_Vis->Fill(base->c_MX2a_S, base->c_MX2a_Vis, weight*double(SKIP));
	hist_MX2b_S_v_MX2b_Vis->Fill(base->c_MX2b_S, base->c_MX2b_Vis, weight*double(SKIP));


	/////////////////
      }
      
      delete base;
      delete chain;
    }
    cout << "Total for: " << title << " " << hists[0]->Integral() << endl;
    
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(11111111);
    for(int h = 0; h < int(hists.size()); h++) Plot_Hist(hists[h]);
  }

}

void Plot_Hist(TH2D* h){
  string title = h->GetTitle();
  TCanvas* can = (TCanvas*) new TCanvas(("can_"+title).c_str(),("can_"+title).c_str(),700.,600);

  can->SetLeftMargin(0.15);
  can->SetRightMargin(0.18);
  can->SetBottomMargin(0.15);
  can->SetGridx();
  can->SetGridy();
  can->SetLogz();
  can->Draw();
  can->cd();
  h->Draw("COLZ");
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleFont(42);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetTitleOffset(1.06);
  h->GetXaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelSize(0.05);
  //h->GetXaxis()->SetTitle(g_Xname.c_str());
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleOffset(1.12);
  h->GetYaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelSize(0.05);
  //h->GetYaxis()->SetTitle(g_Yname.c_str());
  h->GetZaxis()->CenterTitle();
  h->GetZaxis()->SetTitleFont(42);
  h->GetZaxis()->SetTitleSize(0.055);
  h->GetZaxis()->SetTitleOffset(1.05);
  h->GetZaxis()->SetLabelFont(42);
  h->GetZaxis()->SetLabelSize(0.05);
  //h->GetZaxis()->SetTitle("N_{events} / 137 fb^{-1}");
  h->GetZaxis()->SetRangeUser(0.9*h->GetMinimum(0.0),1.1*h->GetMaximum());

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  // l.DrawLatex(0.17,0.855,g_PlotTitle.c_str());
  l.DrawLatex(0.71,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.01,0.943,"#bf{CMS} Simulation Preliminary");

  l.SetTextSize(0.045);
  l.SetTextFont(42);
  l.DrawLatex(0.7,0.04,g_Label.c_str());

  string pdf_title = can->GetTitle();
  can->SaveAs((pdf_title+".pdf").c_str());
  TFile* file = new TFile("output_Plot_2D_Cascade.root","UPDATE");
  can->Write();
  file->Close();
  delete h;
  delete can;
}
