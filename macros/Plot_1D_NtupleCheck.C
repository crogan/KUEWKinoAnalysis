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
#include <TLorentzVector.h>

#include "RestFrames/RestFrames.hh"
#include "../include/ReducedBase_slim.hh"
#include "../include/SampleSet.hh"

#include "../include/mt2_bisect.hh"

using namespace std;

using namespace RestFrames;

vector<SampleSet*> g_Samples;


void Plot_1D_NtupleCheck(){
  RestFrames::SetStyle();

  int Nvar = 5; 
  string g_Xname[Nvar];
  double g_Xmin[Nvar];
  double g_Xmax[Nvar];
  double g_NX[Nvar];
  string g_PlotTitle;
  string g_Yname;
  double g_Ymin;
  double g_Ymax;
  double g_NY;

  g_PlotTitle = "Baseline Selection";

  string NtuplePath = "";

  int BKG_SKIP = 1;


  SampleSet SS_ttbar_old;
  SS_ttbar_old.SetBkg(true);
  SS_ttbar_old.SetTitle("2018 ttbar Old");
  SS_ttbar_old.SetTreeName("KUAnalysis");
  SS_ttbar_old.SetColor(kBlue);
  SS_ttbar_old.AddFile("root://cmseos.fnal.gov//store/group/lpcsusylep/NTUPLES_v0/Autumn18_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  SS_ttbar_old.SetSkip(BKG_SKIP);
  g_Samples.push_back(&SS_ttbar_old);

  SampleSet SS_ttbar_new;
  SS_ttbar_new.SetBkg(true);
  SS_ttbar_new.SetTitle("2018 ttbar New");
  SS_ttbar_new.SetTreeName("KUAnalysis");
  SS_ttbar_new.SetColor(kRed);
  SS_ttbar_new.AddFile("/stash/user/zflowers/NTUPLES/HADD/Autumn18_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  SS_ttbar_new.SetSkip(BKG_SKIP);
  g_Samples.push_back(&SS_ttbar_new);

  SampleSet SS_data_old;
  SS_data_old.SetBkg(true);
  SS_data_old.SetTitle("2017 data Old");
  SS_data_old.SetTreeName("KUAnalysis");
  SS_data_old.SetColor(kBlue);
  SS_data_old.AddFile("root://cmseos.fnal.gov//store/group/lpcsusylep/NTUPLES_v0/Fall17_102X_Data/MET_Run2017D-02Apr2020-v1_2017_Fall17_102X.root");
  SS_data_old.SetSkip(BKG_SKIP);
  //g_Samples.push_back(&SS_data_old);

  SampleSet SS_data_new;
  SS_data_new.SetBkg(true);
  SS_data_new.SetTitle("2017 data New");
  SS_data_new.SetTreeName("KUAnalysis");
  SS_data_new.SetColor(kRed);
  SS_data_new.AddFile("/stash/user/zflowers/NTUPLES/HADD/Fall17_102X_Data/MET_Run2017D-02Apr2020-v1_2017_Fall17_102X.root");
  SS_data_new.SetSkip(BKG_SKIP);
  //g_Samples.push_back(&SS_data_new);

  SampleSet SS_TChiWZ_old;
  SS_TChiWZ_old.SetBkg(true);
  SS_TChiWZ_old.SetTitle("2016 TChiWZ 300 290 Old");
  SS_TChiWZ_old.SetTreeName("SMS_300_290");
  SS_TChiWZ_old.SetColor(kBlue);
  SS_TChiWZ_old.AddFile("root://cmseos.fnal.gov//store/group/lpcsusylep/NTUPLES_v0/Summer16_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  SS_TChiWZ_old.SetSkip(BKG_SKIP);
  //g_Samples.push_back(&SS_TChiWZ_old);

  SampleSet SS_TChiWZ_new;
  SS_TChiWZ_new.SetBkg(true);
  SS_TChiWZ_new.SetTitle("2016 TChiWZ 300 290 New");
  SS_TChiWZ_new.SetTreeName("SMS_300_290");
  SS_TChiWZ_new.SetColor(kRed);
  SS_TChiWZ_new.AddFile("/stash/user/zflowers/NTUPLES/HADD/Summer16_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  SS_TChiWZ_new.SetSkip(BKG_SKIP);
  //g_Samples.push_back(&SS_TChiWZ_new);

  int Nsample = g_Samples.size();
  TH1D* hist[Nsample][Nvar];

  g_Xname[0] = "Mperp";
  g_Xmin[0] = 0.;
  g_Xmax[0] = 250.;
  g_NX[0] = 50;

  g_Xname[1] = "RISR";
  g_Xmin[1] = 0.;
  g_Xmax[1] = 1.;
  g_NX[1] = 50;

  g_Xname[2] = "PTISR";
  g_Xmin[2] = 0.;
  g_Xmax[2] = 1000.;
  g_NX[2] = 50;

  g_Xname[3] = "MET";
  g_Xmin[3] = 100.;
  g_Xmax[3] = 1000.;
  g_NX[3] = 50;

  g_Xname[4] = "PTCM";
  g_Xmin[4] = 0.;
  g_Xmax[4] = 500.;
  g_NX[4] = 50;

 
 for(int v = 0; v < Nvar; v++)
  for(int i = 0; i < Nsample; i++)
    hist[i][v] = new TH1D(("h"+to_string(i)+to_string(v)).c_str(),
		       ("h"+to_string(i)+to_string(v)).c_str(),
		       g_NX[v],g_Xmin[v],g_Xmax[v]);

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

	hist[s][0]->Fill(base->Mperp, base->weight*double(SKIP));
	hist[s][1]->Fill(base->RISR, base->weight*double(SKIP));
	hist[s][2]->Fill(base->PTISR, base->weight*double(SKIP));
	hist[s][3]->Fill(base->MET, base->weight*double(SKIP));
	hist[s][4]->Fill(base->PTCM, base->weight*double(SKIP));
      }
      
      delete base;
      delete chain;
    }
  }
       
  double max = -1.;
  int imax = -1;
  for(int v = 0; v < Nvar; v++){
  for(int i = 0; i < Nsample; i++){
    cout << g_Samples[i]->GetTitle().c_str() << " " << hist[i][v]->Integral()*137 << " total" << endl;
    //hist[i][v]->Scale(1./hist[i][v]->Integral());
    ////hist[i][v]->Scale(1./hist[i][v]->GetMaximum());
    if(hist[i][v]->GetMaximum() > max){
      max = hist[i][v]->GetMaximum();
      imax = i;
    }
  }
  
  float width = hist[0][v]->GetBinWidth(1);
  char *yaxis = new char[100];
  sprintf(yaxis,"Events / %f", width);

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = (TCanvas*) new TCanvas("can","can",600.,500);

  can->SetLeftMargin(0.13);
  can->SetRightMargin(0.04);
  can->SetBottomMargin(0.16);
  can->SetTopMargin(0.085);
  can->SetGridx();
  can->SetGridy();
  can->SetLogy();
  can->Draw();
  can->cd();
  // for(int i = 0; i < hist[imax]->GetNbinsX(); i++){
  //   char* sbin = new char[20];
  //   sprintf(sbin,"#geq %d", i);
  //   hist[imax]->GetXaxis()->SetBinLabel(i+1,sbin);
  // }
  hist[imax][v]->Draw("hist");
  hist[imax][v]->GetXaxis()->CenterTitle();
  hist[imax][v]->GetXaxis()->SetTitleFont(42);
  hist[imax][v]->GetXaxis()->SetTitleSize(0.06);
  hist[imax][v]->GetXaxis()->SetTitleOffset(1.06);
  hist[imax][v]->GetXaxis()->SetLabelFont(42);
  hist[imax][v]->GetXaxis()->SetLabelSize(0.05);
  hist[imax][v]->GetXaxis()->SetTitle(g_Xname[v].c_str());
  hist[imax][v]->GetYaxis()->CenterTitle();
  hist[imax][v]->GetYaxis()->SetTitleFont(42);
  hist[imax][v]->GetYaxis()->SetTitleSize(0.06);
  hist[imax][v]->GetYaxis()->SetTitleOffset(1.1);
  hist[imax][v]->GetYaxis()->SetLabelFont(42);
  hist[imax][v]->GetYaxis()->SetLabelSize(0.05);
  hist[imax][v]->GetYaxis()->SetTitle("a. u.");
  //hist[imax][v]->GetYaxis()->SetRangeUser(0., hist[imax][v]->GetMaximum()*1.1);
  //hist[imax]->GetYaxis()->SetTitle(yaxis);
  //hist[imax]->GetYaxis()->SetTitle("N_{evt} / fb^{-1}");
  int Ntype[3];

  int mycolor[8];
  mycolor[0] = kBlue+2;
  mycolor[1] = kGreen+3;
  mycolor[2] = kRed+1;
  mycolor[3] = kYellow+2;
  mycolor[4] = kMagenta+1;
  mycolor[5] = kMagenta+2;
  mycolor[6] = kCyan+2;
  mycolor[7] = kCyan+3;
  
  Ntype[0] = 0;
  for(int i = Nsample-1; i >= 0; i--){
    hist[i][v]->SetLineColor(mycolor[i]);
    hist[i][v]->SetLineWidth(3);
    if(i >= 4 && false){
      if(i%2 == 0)
	hist[i][v]->SetLineStyle(7);
      if(i%2 == 1)
	hist[i][v]->SetLineStyle(9);
      hist[i][v]->SetLineWidth(4);
    }
    hist[i][v]->SetMarkerColor(mycolor[i]);
    hist[i][v]->SetMarkerSize(0);
    hist[i][v]->SetFillColorAlpha(mycolor[i],.3+i*.3);
    hist[i][v]->SetFillStyle(3006+i);
    Ntype[0]++;
    hist[i][v]->Draw("hist SAME");
  }

  TLegend* leg = new TLegend(0.688,0.22,0.93,0.42);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  for(int i = 0; i < Nsample; i++)
    leg->AddEntry(hist[i][v],g_Samples[i]->GetTitle().c_str());
  leg->SetLineColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->Draw("SAME");

  TLatex l;
  l.SetTextFont(132);
  l.SetNDC();
  l.SetTextSize(0.045);
  l.SetTextFont(42);
  // l.DrawLatex(0.17,0.855,g_PlotTitle.c_str());
  l.DrawLatex(0.6,0.943,g_PlotTitle.c_str());
  l.SetTextSize(0.045);
  l.SetTextFont(42);
  l.DrawLatex(0.135,0.943,"#bf{CMS} Simulation Preliminary");
  string pdf_str = "_FlipMET_CutsAfterJER";
  can->SaveAs(("ttbar_2018_log_"+g_Xname[v]+pdf_str+".pdf").c_str());
  //can->SaveAs(("data_2017_"+g_Xname[v]+pdf_str+".pdf").c_str());
  //can->SaveAs(("TChiWZ_2016_300_290_log_"+g_Xname[v]+pdf_str+".pdf").c_str());
 }
}
 //
