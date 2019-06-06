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

#include "RestFrames/RestFrames.hh"
#include "include/ReducedBase.hh"

using namespace RestFrames;
using namespace std;

string g_Path;
vector<string> g_File;
vector<string> g_Tree;
vector<int> g_Hist;
vector<string> g_Title;
vector<bool> g_Bkg;
vector<int> g_Color;
double g_Lumi;
string g_PlotTitle;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;

void Plot_1D_stack(){
  RestFrames::SetStyle();

  int ihist = 0;

  g_File.push_back("All_Bkg_2017/TTJets_TuneCP5_13TeV-madgraphMLM-pythia8_TuneCP5.root");
  g_Hist.push_back(ihist);
  g_File.push_back("All_Bkg_2017/ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
  g_Hist.push_back(ihist);
  g_File.push_back("All_Bkg_2017/ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
  g_Hist.push_back(ihist);
  g_Title.push_back("t#bar{t} + X");
  g_Bkg.push_back(true);
  g_Color.push_back(kAzure+1);
  ihist++;

  g_File.push_back("All_Bkg_2017/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  g_Hist.push_back(ihist);
  g_File.push_back("All_Bkg_2017/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17.root");
  g_Hist.push_back(ihist);
  g_Title.push_back("Z/#gamma^{*} + jets");
  g_Bkg.push_back(true);
  g_Color.push_back(kGreen-9);
  ihist++;

  g_File.push_back("bkg/ST_antitop.root");
  g_Hist.push_back(ihist);
  g_File.push_back("bkg/ST_top.root");
  g_Hist.push_back(ihist);
  g_File.push_back("bkg/tHQ.root");
  g_Hist.push_back(ihist);
  g_Title.push_back("single t + X");
  g_Bkg.push_back(true);
  g_Color.push_back(10);
  ihist++;

  g_File.push_back("bkg/DYJets.root");
  g_Hist.push_back(ihist);
  g_File.push_back("bkg/WJets.root");
  g_Hist.push_back(ihist);
  g_File.push_back("bkg/WW.root");
  g_Hist.push_back(ihist);
  g_File.push_back("bkg/WZ.root");
  g_Hist.push_back(ihist);
  g_File.push_back("bkg/ZJets.root");
  g_Hist.push_back(ihist);
  g_File.push_back("bkg/ZZ.root");
  g_Hist.push_back(ihist);
  g_Title.push_back("other");
  g_Bkg.push_back(true);
  g_Color.push_back(kGreen+3);
  ihist++;

   g_File.push_back("signal/TttH_1200_RH.root");
  g_Hist.push_back(ihist);
  g_Title.push_back("TttH RH M_{T'} = 1.2 TeV");
  g_Bkg.push_back(false);
  g_Color.push_back(kBlue+1);
  ihist++;

  g_File.push_back("signal/TttH_1500_RH.root");
  g_Hist.push_back(ihist);
  g_Title.push_back("TttH RH M_{T'} = 1.5 TeV");
  g_Bkg.push_back(false);
  g_Color.push_back(kRed+1);
  ihist++;

  g_File.push_back("signal/TttH_1800_RH.root");
  g_Hist.push_back(ihist);
  g_Title.push_back("TttH RH M_{T'} = 1.8 TeV");
  g_Bkg.push_back(false);
  g_Color.push_back(kMagenta+1);
  ihist++;

  
 
  //////////////////

  int Nsample = g_File.size();
  int Nhist = ihist;

  g_Path = "/Users/crogan/Dropbox/SAMPLES/EWKino/StopNtuple/";
  g_PlotTitle = "Region D Strawberry";
  g_Lumi = 36;

  g_Xname = "#tilde{M}_{T'} [GeV]";
  g_Xmin = 750.;
  g_Xmax = 2500.;

  g_NX = 32;


  TH1D* hist[Nhist];
  for(int i = 0; i < Nhist; i++)
    hist[i] = new TH1D(("h"+to_string(i)).c_str(),
		       ("h"+to_string(i)).c_str(),
		       g_NX,g_Xmin,g_Xmax);

  for(int s = 0; s < Nsample; s++){
    TChain* chain = new TChain("TPrime");
    chain->Add((g_Path+g_File[s]).c_str());

    ReducedBase* base = new ReducedBase(chain);

    int Nentry = base->fChain->GetEntries();

    cout << "Sample " << s << " | " << Nsample << endl;
    for(int e = 0; e < Nentry; e++){
      base->GetEntry(e);
      if(e%(max(1,Nentry/10)) == 0)
	cout << "event " << e << " | " << Nentry << endl;

  

      double weight = fabs(base->weight);

      TLorentzVector H,T;
      H.SetPtEtaPhiM( base->pT_higgs, base->eta_higgs, base->phi_higgs, base->mass_higgs );
      T.SetPtEtaPhiM( base->pT_top, base->eta_top, base->phi_top, base->mass_top );
      TLorentzVector Tp = H+T;
      TLorentzVector q;
      q.SetPtEtaPhiM( base->pT_q, base->eta_q, base->phi_q, base->mass_q );
      

      double RPTtop = base->pT_top / (base->pT_top + base->M_Tp);
      double RPThiggs = base->pT_higgs / (base->pT_higgs + base->M_Tp);

      bool lveto  = false;
      TLorentzVector LEP(0.,0.,0.,0.);
      
      // if(base->pT_mu_clean->size() > 0 && base->pT_mu_clean->at(0) >= 55.){
      // 	lveto = true;
      // 	LEP.SetPtEtaPhiE(base->pT_mu_clean->at(0),
      // 			 base->eta_mu_clean->at(0),
      // 			 base->phi_mu_clean->at(0),
      // 			 base->E_mu_clean->at(0));
      // }
      // if(base->pT_ele_clean->size() > 0 && base->pT_ele_clean->at(0) >= LEP.Pt()){
      // 	lveto = true;
      // 	LEP.SetPtEtaPhiE(base->pT_ele_clean->at(0),
      // 			 base->eta_ele_clean->at(0),
      // 			 base->phi_ele_clean->at(0),
      // 			 base->E_ele_clean->at(0));
      // }

      // if(lveto)
      // 	hist[g_Hist[s]]->Fill(base->tau3_higgs/base->tau2_higgs, weight*g_Lumi);

      int Nj = base->pT_extrajet->size();
      int Nextra = 0;
      int Nbtag = 0;
      vector<TLorentzVector> jets;
      double maxEta = 0;
      TLorentzVector bjet;
      double bjetCSV = 0.;
      bool Hveto = false;
      for(int j = 0; j < Nj; j++){
	TLorentzVector jet;
	jet.SetPtEtaPhiM(base->pT_extrajet->at(j),
			 base->eta_extrajet->at(j),
			 base->phi_extrajet->at(j),
			 base->mass_extrajet->at(j));
	 if(jet.DeltaR(H) > 0.55 && jet.DeltaR(H) < 0.85) Hveto = true;
	if(jet.DeltaR(H) < 1.1 || jet.DeltaR(T) < 1.1) continue;
	Nextra++;
	jets.push_back(jet);
	if(fabs(jet.Eta()) > maxEta)
	  maxEta = fabs(jet.Eta());
	if(base->CSV_extrajet->at(j) > 0.8484)
	  Nbtag++;
	if(base->CSV_extrajet->at(j) > bjetCSV){
	  bjetCSV = base->CSV_extrajet->at(j);
	  bjet = jet;
	}
      }
      double dRapidity = 0;
      double dbEta = 0;
      for(int i = 0; i < Nextra; i++){
	 if(fabs(jets[i].Eta() - bjet.Eta()) > dbEta){
	   dbEta = fabs(jets[i].Eta() - bjet.Eta());
	 }
	for(int j = i+1; j < Nextra; j++)
	  if(fabs(jets[i].Eta()-jets[j].Eta()) > dRapidity)
	    dRapidity = fabs(jets[i].Eta()-jets[j].Eta());
      }

      
      // vanilla selection
      // if(base->tau3_top/base->tau2_top > 0.5)
      // 	continue;
      // if(Nextra < 1)
      // 	continue;
      // if(maxEta < 2.4)
      // 	continue;

       // Chocolate selection
      if(base->tau3_top/base->tau2_top > 0.57)
	continue;
      if(Nextra < 4)
	continue;
      if(Nbtag < 1)
      	continue;
       if(Hveto)
       	continue;
      if(dRapidity < 3.5)
	continue;


      double MT = Tp.M() - T.M() - H.M() + 300.;
      if(weight*g_Lumi > 3.) continue;
      hist[g_Hist[s]]->Fill(MT, weight*g_Lumi);

    }

    delete base;
    delete chain;
  }

  TH1D* h_BKG = nullptr;
  bool isBKG = false;
  for(int i = 0; i < Nhist; i++){
    cout << "Sample " << g_Title[i] << " has " << hist[i]->Integral() << " events" << endl;
    if(g_Bkg[i]){
      if(!isBKG){
	h_BKG = (TH1D*) hist[i]->Clone("TOT_BKG");
	isBKG = true;
      } else {
	for(int k = 0; k < i; k++){
	  hist[k]->Add(hist[i]);
	}
	h_BKG->Add(hist[i]);
      }
    }
  }
  if(h_BKG)
    cout << "Total Background is " << h_BKG->Integral() << " events" << endl;

  double max = -1.;
  int imax = -1;
  for(int i = 0; i < Nhist; i++){
    if(hist[i]->GetMaximum() > max){
      max = hist[i]->GetMaximum();
      imax = i;
    }
  }
  float width = hist[0]->GetBinWidth(1);
  char *yaxis = new char[100];
  //sprintf(yaxis,"Events / %f", width);
  sprintf(yaxis,"Events / bin", width);

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = (TCanvas*) new TCanvas("can","can",600.,500);

  can->SetLeftMargin(0.13);
  can->SetRightMargin(0.04);
  can->SetBottomMargin(0.15);
  can->SetTopMargin(0.085);
  can->SetGridx();
  can->SetGridy();
  can->Draw();
  can->cd();
  hist[imax]->Draw();
  hist[imax]->GetXaxis()->CenterTitle();
  hist[imax]->GetXaxis()->SetTitleFont(132);
  hist[imax]->GetXaxis()->SetTitleSize(0.06);
  hist[imax]->GetXaxis()->SetTitleOffset(1.06);
  hist[imax]->GetXaxis()->SetLabelFont(132);
  hist[imax]->GetXaxis()->SetLabelSize(0.05);
  hist[imax]->GetXaxis()->SetTitle(g_Xname.c_str());
  hist[imax]->GetYaxis()->CenterTitle();
  hist[imax]->GetYaxis()->SetTitleFont(132);
  hist[imax]->GetYaxis()->SetTitleSize(0.06);
  hist[imax]->GetYaxis()->SetTitleOffset(1.);
  hist[imax]->GetYaxis()->SetLabelFont(132);
  hist[imax]->GetYaxis()->SetLabelSize(0.05);
  hist[imax]->GetYaxis()->SetTitle("a. u.");
  hist[imax]->GetYaxis()->SetTitle(yaxis);
  //hist[imax]->GetYaxis()->SetTitle("N_{evt} / fb^{-1}");

  int Ntype[3];

  Ntype[0] = 0;
  for(int i = 0; i < Nhist; i++){
    if(g_Bkg[i]){
      hist[i]->SetLineColor(kBlack);
      // if(style_list[1][Ntype[0]+1] > 1001) 
      // 	hist[i]->SetLineColor(color_list[1][Ntype[0]+1]);
      hist[i]->SetLineWidth(1.0);
      hist[i]->SetFillColor(g_Color[i]);
      hist[i]->SetFillStyle(1001);
      Ntype[0]++;
      hist[i]->Draw("SAME");
    }
  }

  if(Ntype[0] > 0 && h_BKG){
    h_BKG->SetLineWidth(3.0);
    h_BKG->SetLineColor(kRed);
    h_BKG->SetMarkerSize(0);
    h_BKG->Draw("SAME");
  }
	
  Ntype[1] = 0;
  for(int i = 0; i < Nhist; i++){
    if(!g_Bkg[i]){
      hist[i]->SetLineWidth(3.0);
      hist[i]->SetMarkerSize(0.);
      hist[i]->SetMarkerColor(kBlack);
      hist[i]->SetLineStyle(7);
      hist[i]->SetLineColor(g_Color[i]);
      Ntype[1]++;
      hist[i]->Draw("SAME");
    }
  }

  TLegend* leg = new TLegend(0.688,0.22,0.93,0.42);
  leg->SetTextFont(132);
  leg->SetTextSize(0.045);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  if(Ntype[0] > 0) leg->AddEntry(h_BKG, "SM total");
  for(int i = 0; i < Nhist; i++)
    if(g_Bkg[i])
      leg->AddEntry(hist[i],g_Title[i].c_str(),"F");
    else
      leg->AddEntry(hist[i],g_Title[i].c_str());
  leg->SetLineColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->Draw("SAME");

  TLatex l;
  l.SetTextFont(132);
  l.SetNDC();
  l.SetTextSize(0.05);
  l.SetTextFont(132);
  l.DrawLatex(0.65,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.15,0.943,"#bf{#it{CMS}} Internal 13 TeV Simulation");
  l.SetTextSize(0.05);
  l.SetTextFont(132);
  string s_lumi = "#scale[0.6]{#int} #it{L dt} = "+to_string(int(g_Lumi))+" fb^{-1}";
  l.DrawLatex(0.43,0.79,s_lumi.c_str());	

}



