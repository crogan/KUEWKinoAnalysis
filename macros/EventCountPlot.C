#include <iostream>
#include <string>
//#include <boost/filesystem.hpp>

#include <TROOT.h>
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
#include <TSystem.h>

#include "../include/ReducedBase_slim.hh"
#include "../include/SampleTool.hh"
#include "../include/CategoryTool.hh"
#include "../include/ScaleFactorTool.hh"
#include "../include/Leptonic.hh"
#include "../include/Hadronic.hh"
#include "../include/FitReader.hh"

#include "RestFrames/RestFrames.hh"

using namespace std;

// global variables
string g_PlotTitle;
string g_Label;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_XN;
string g_Yname;
double g_Ymin;
double g_Ymax;
double g_YN;
string g_dm_Yname;
double g_dm_Ymin;
double g_dm_Ymax;
double g_dm_YN;

using namespace RestFrames;

void format(TH2D* hist, string Xname, string Yname)
{
  hist->GetXaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleSize(0.06);
  hist->GetXaxis()->SetTitleOffset(1.06);
  hist->GetXaxis()->SetLabelFont(42);
  hist->GetXaxis()->SetLabelSize(0.05);
  hist->GetXaxis()->SetTitle(Xname.c_str());
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetYaxis()->SetTitleSize(0.06);
  hist->GetYaxis()->SetTitleOffset(1.12);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelSize(0.05);
  hist->GetYaxis()->SetTitle(Yname.c_str());
  hist->GetZaxis()->CenterTitle();
  hist->GetZaxis()->SetTitleFont(42);
  hist->GetZaxis()->SetTitleSize(0.05);
  hist->GetZaxis()->SetTitleOffset(1.15);
  hist->GetZaxis()->SetLabelFont(42);
  hist->GetZaxis()->SetLabelSize(0.05);
  hist->GetZaxis()->SetTitle("N_{events}");
  hist->GetZaxis()->SetRangeUser(0.9 * hist->GetMinimum(0.0), 1.1 * hist->GetMaximum());
}

void drawLatex(string title, string label)
{
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  //l.DrawLatex(0.17, 0.855, title.c_str());
  l.DrawLatex(0.71, 0.943, title.c_str());
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.01, 0.943, "#bf{CMS} Simulation Preliminary");
  
  l.SetTextSize(0.045);
  l.SetTextFont(42);
  l.DrawLatex(0.7, 0.04, label.c_str());
}

void EventCountPlot()
{
  // speed up by not displaying canvas
  gROOT->SetBatch(kTRUE);
  
  RestFrames::SetStyle();

  //string NtuplePath = "/home/t3-ku/z374f439/storage/crogan/";
  string NtuplePath = "root://xrootd.unl.edu//store/user/zflowers/crogan/";

  vector<VS> signals;
  VS signal_labels;

  VS T2tt;
  VS T2bW;
  VS TChiWZ;
  VS TChipmWW;
  VS TSlepSlep;
  VS T2bb;
  VS T2cc;
  VS T1bbbb;
  VS T1ttbb;
  VS T5tttt;
  VS TChiWH;
  
  // ------------ //
  // --- 2016 --- // 
  // ------------ //
  
  // TODO: Update T2bW_2016 file paths once files are created
  VS T2bW_2016;
  T2bW_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  T2bW_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  //T2bW_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2bW_X05_dM-10to80_2Lfilter_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  T2bW += T2bW_2016;

  VS T2tt_2016;
  T2tt_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  T2tt_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  T2tt_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  T2tt_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2tt_dM-6to8_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  //T2tt_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2tt_dM-10to80_2Lfilter_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  T2tt += T2tt_2016;

  VS TChiWZ_2016;
  //TChiWZ_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TChiWZ_ZToLL_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  //TChiWZ_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TChiWZ_ZToLL_dM-90to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  //TChiWZ_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  TChiWZ_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TChiWZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  TChiWZ_2016.a(NtuplePath+"Summer16_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  TChiWZ += TChiWZ_2016;

  VS TChipmWW_2016;
  TChipmWW_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TChipmWW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  //TChipmWW_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  TChipmWW_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TChipmWW_dM-3to50_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  TChipmWW += TChipmWW_2016;

  VS TSlepSlep_2016;
  TSlepSlep_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TSlepSlep_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  TSlepSlep_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_ext_Summer16_102X.root");
  TSlepSlep_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-TSlepSlep_mSlep-500To1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  TSlepSlep += TSlepSlep_2016;
  
  // TODO: Update T2bb_2016 file paths once files are created
  VS T2bb_2016;
  T2bb_2016.a(NtuplePath+"Summer16_102X_SMS/SMS-T2bb_TuneCP2_13TeV-madgraphMLM-pythia8_Summer16_102X.root");
  T2bb += T2bb_2016;
  
  // ------------ //
  // --- 2017 --- // 
  // ------------ //

  VS T2bW_2017;
  T2bW_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2bW_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  //T2bW_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_2Lfilter_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2bW += T2bW_2017;
  
  VS T2tt_2017;
  T2tt_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2tt_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2tt_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2tt_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-6to8_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  //T2tt_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_2Lfilter_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2tt += T2tt_2017;

  VS TChiWZ_2017;
  //TChiWZ_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  //TChiWZ_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_dM-90to100_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  //TChiWZ_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  //TChiWZ_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_mC1-325to1000_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  TChiWZ_2017.a(NtuplePath+"Fall17_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  TChiWZ += TChiWZ_2017;

  VS TChipmWW_2017;
  TChipmWW_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  //TChipmWW_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  TChipmWW_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_dM-3to50_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  TChipmWW += TChipmWW_2017;

  VS TSlepSlep_2017;
  TSlepSlep_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  TSlepSlep_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_ext_Fall17_102X.root");
  TSlepSlep_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_mSlep-500To1300_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  TSlepSlep += TSlepSlep_2017;

  VS T2bb_2017;
  T2bb_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2bb_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2bb += T2bb_2017;

  VS T2cc_2017;
  T2cc_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T2cc_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T2cc += T2cc_2017;

  VS T1bbbb_2017;
  T1bbbb_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T1bbbb_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T1bbbb += T1bbbb_2017;

  VS T1ttbb_2017;
  T1ttbb_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T1ttbb_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T1ttbb += T1ttbb_2017;

  VS T5tttt_2017;
  T5tttt_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-T5tttt_dM175_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  T5tttt += T5tttt_2017;

  VS TChiWH_2017;
  TChiWH_2017.a(NtuplePath+"Fall17_102X_SMS/SMS-TChiWH_WToLNu_HToBB_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  TChiWH += TChiWH_2017;
  
  // ------------ //
  // --- 2018 --- // 
  // ------------ //
 
  // TODO: Update T2bW_2018 file paths once files are created
  VS T2bW_2018;
  T2bW_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2bW_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  //T2bW_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_2Lfilter_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2bW += T2bW_2018;
  
  VS T2tt_2018;
  T2tt_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2tt_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2tt_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2tt_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2tt_dM-6to8_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  //T2tt_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2tt_dM-10to80_2Lfilter_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2tt += T2tt_2018;

  VS TChiWZ_2018;
  //TChiWZ_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  //TChiWZ_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_dM-90to100_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  //TChiWZ_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  //TChiWZ_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_mC1-325to1000_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  TChiWZ_2018.a(NtuplePath+"Autumn18_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  TChiWZ += TChiWZ_2018;

  VS TChipmWW_2018;
  TChipmWW_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  //TChipmWW_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  TChipmWW_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChipmWW_dM-3to50_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  TChipmWW += TChipmWW_2018;

  VS TSlepSlep_2018;
  TSlepSlep_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  TSlepSlep_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_ext_Autumn18_102X.root");
  TSlepSlep_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TSlepSlep_mSlep-500To1300_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  TSlepSlep += TSlepSlep_2018;

  // TODO: Update T2bb_2018 file paths once files are created
  VS T2bb_2018;
  T2bb_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2bb_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2bb += T2bb_2018;

  VS T2cc_2018;
  T2cc_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T2cc_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T2cc += T2cc_2018;

  VS T1bbbb_2018;
  T1bbbb_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T1bbbb_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T1bbbb += T1bbbb_2018;

  VS T1ttbb_2018;
  T1ttbb_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T1ttbb_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T1ttbb += T1ttbb_2018;

  VS T5tttt_2018;
  T5tttt_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-T5tttt_dM175_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  T5tttt += T5tttt_2018;

  VS TChiWH_2018;
  TChiWH_2018.a(NtuplePath+"Autumn18_102X_SMS/SMS-TChiWH_WToLNu_HToBB_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root");
  TChiWH += TChiWH_2018;

  // --- 2016 --- // 
  signal_labels.a("T2bW_2016");
  signals.push_back(T2bW_2016);
  //signal_labels.a("T2tt_2016");
  //signals.push_back(T2tt_2016);
  //signal_labels.a("TChiWZ_2016");
  //signals.push_back(TChiWZ_2016);
  //signal_labels.a("TChipmWW_2016");
  //signals.push_back(TChipmWW_2016);
  //signal_labels.a("TSlepSlep_2016");
  //signals.push_back(TSlepSlep_2016);
  //signal_labels.a("T2bb_2016");
  //signals.push_back(T2bb_2016);
  
  // --- 2017 --- //
  signal_labels.a("T2bW_2017");
  signals.push_back(T2bW_2017);
  //signal_labels.a("T2tt_2017");
  //signals.push_back(T2tt_2017);
  //signal_labels.a("TChiWZ_2017");
  //signals.push_back(TChiWZ_2017);
  //signal_labels.a("TChipmWW_2017");
  //signals.push_back(TChipmWW_2017);
  //signal_labels.a("TSlepSlep_2017");
  //signals.push_back(TSlepSlep_2017);
  //signal_labels.a("T2bb_2017");
  //signals.push_back(T2bb_2017);
  //signal_labels.a("T2cc_2017");
  //signals.push_back(T2cc_2017);
  //signal_labels.a("T1bbbb_2017");
  //signals.push_back(T1bbbb_2017);
  //signal_labels.a("T1ttbb_2017");
  //signals.push_back(T1ttbb_2017);
  //signal_labels.a("T5tttt_2017");
  //signals.push_back(T5tttt_2017);
  //signal_labels.a("TChiWH_2017");
  //signals.push_back(TChiWH_2017);
  
  // --- 2018 --- //
  signal_labels.a("T2bW_2018");
  signals.push_back(T2bW_2018);
  //signal_labels.a("T2tt_2018");
  //signals.push_back(T2tt_2018);
  //signal_labels.a("TChiWZ_2018");
  //signals.push_back(TChiWZ_2018);
  //signal_labels.a("TChipmWW_2018");
  //signals.push_back(TChipmWW_2018);
  //signal_labels.a("TSlepSlep_2018");
  //signals.push_back(TSlepSlep_2018);
  //signal_labels.a("T2bb_2018");
  //signals.push_back(T2bb_2018);
  //signal_labels.a("T2cc_2018");
  //signals.push_back(T2cc_2018);
  //signal_labels.a("T1bbbb_2018");
  //signals.push_back(T1bbbb_2018);
  //signal_labels.a("T1ttbb_2018");
  //signals.push_back(T1ttbb_2018);
  //signal_labels.a("T5tttt_2018");
  //signals.push_back(T5tttt_2018);
  //signal_labels.a("TChiWH_2018");
  //signals.push_back(TChiWH_2018);

  // --- Run 2 --- //
  signal_labels.a("T2bW");
  signals.push_back(T2bW);
  //signal_labels.a("T2tt");
  //signals.push_back(T2tt);
  //signal_labels.a("TChiWZ");
  //signals.push_back(TChiWZ);
  //signal_labels.a("TChipmWW");
  //signals.push_back(TChipmWW);
  //signal_labels.a("TSlepSlep");
  //signals.push_back(TSlepSlep);
  //signal_labels.a("T2bb");
  //signals.push_back(T2bb);
  //signal_labels.a("T2cc");
  //signals.push_back(T2cc);
  //signal_labels.a("T1bbbb");
  //signals.push_back(T1bbbb);
  //signal_labels.a("T1ttbb");
  //signals.push_back(T1ttbb);
  //signal_labels.a("T5tttt");
  //signals.push_back(T5tttt);
  //signal_labels.a("TChiWH");
  //signals.push_back(TChiWH);
  
  for(int s = 0; s < int(signals.size()); s++)
  {
    g_Label = signal_labels[s];
    
    g_Xname = "M_{NLSP} [GeV]";
    g_Xmin  = 0.;
    g_Xmax  = 1500.; 
    g_XN    = 60;
    //g_XN  = 30;
    
    g_Yname = "M_{LSP} [GeV]";
    g_Ymin  = 0.;
    g_Ymax  = 1000.;
    g_YN    = 40;
    //g_YN  = 20;

    // mass diff plot
    g_dm_Yname = "M_{NLSP} - M_{LSP} [GeV]";
    g_dm_Ymin  = 0.;
    g_dm_Ymax  = 200.;
    g_dm_YN    = 20;
    
    // Change label for T2bW
    if (g_Label.find("T2bW") != string::npos)
    {
      g_Xname    = "M_{stop} [GeV]";
      g_dm_Yname = "M_{stop} - M_{LSP} [GeV]";
    }
    
    cout << "Processing " << g_Label << endl;

    // m_lsp vs m_nlsp
    TH2D* hist = new TH2D(
      (g_Label+"_EventCount").c_str(),
      (g_Label+"_EventCount").c_str(),
      g_XN, g_Xmin, g_Xmax,
      g_YN, g_Ymin, g_Ymax
    );
    // mass diff (dm) vs m_nlsp
    TH2D* hist_dm = new TH2D(
      (g_Label+"_dm_EventCount").c_str(),
      (g_Label+"_dm_EventCount").c_str(),
      g_XN,    g_Xmin,    g_Xmax,
      g_dm_YN, g_dm_Ymin, g_dm_Ymax
    );
    
    //cout << "Processing " << Nfile << " files for process " << title << endl;
    for(int f = 0; f < int(signals[s].size()); f++)
    {
      vector<string> tree_names;
      //TFile* file = new TFile(signals[s][f].c_str(),"READ");
      TFile* file = TFile::Open(signals[s][f].c_str(),"READ");
      if(file == 0 || file->IsZombie()) { cout << "Cannot open: " << signals[s][f] << endl; continue; }
      for (auto&& keyAsObj : *file->GetListOfKeys())
      {
        auto key = (TKey*) keyAsObj;
        string name = key->GetName();
        if (name.find("SMS") == string::npos) continue;
        tree_names.push_back(key->GetName());
      }
      for(int k = 0; k < int(tree_names.size()); k++)
      {
        TChain* chain = new TChain(tree_names[k].c_str());
        chain->Add(signals[s][f].c_str());
        int events = chain->GetEntries(); 
        delete chain;
        string NLSP_Mass_string = tree_names[k].erase(0,4); //remove SMS_
        string LSP_Mass_string = NLSP_Mass_string;
        NLSP_Mass_string = NLSP_Mass_string.substr(0,NLSP_Mass_string.find("_"));
        double NLSP_Mass = std::stod(NLSP_Mass_string);
        size_t pos = LSP_Mass_string.find("_");
        LSP_Mass_string = LSP_Mass_string.erase(0,pos+1);
        double LSP_Mass = std::stod(LSP_Mass_string);
        hist->Fill(NLSP_Mass, LSP_Mass, events);
        hist_dm->Fill(NLSP_Mass, NLSP_Mass - LSP_Mass, events);
      }
      file->Close();
    }

    cout << "Total " << hist->Integral() << endl;
    
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

    // format histograms
    format(hist,    g_Xname, g_Yname);
    format(hist_dm, g_Xname, g_dm_Yname);

    // output plot files
    string plot_dir     = "plots";
    string plot_name    = plot_dir + "/";
    string plot_name_dm = plot_dir + "/";
    plot_name    += hist->GetName();
    plot_name_dm += hist_dm->GetName();
    
    // create plot directory
    //boost::filesystem::create_directories(plot_dir);
    gSystem->mkdir(plot_dir.c_str());
    
    // m_lsp vs m_nlsp
    hist->Draw("COLZ");
    drawLatex(g_PlotTitle, g_Label);
    can->SaveAs((plot_name + ".pdf").c_str());
    
    // mass diff (dm) vs m_nlsp
    hist_dm->Draw("COLZ");
    drawLatex(g_PlotTitle, g_Label);
    can->SaveAs((plot_name_dm + ".pdf").c_str());
    
    // output root file
    TFile* output_file = new TFile("output_EventCountPlot.root", "UPDATE");
    can->Write();
    output_file->Close();
    
    delete can;
  }
}
