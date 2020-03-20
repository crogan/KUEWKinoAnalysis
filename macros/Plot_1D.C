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

string g_PlotTitle;
string g_Xname;
double g_Xmin;
double g_Xmax;
double g_NX;
string g_Yname;
double g_Ymin;
double g_Ymax;
double g_NY;

void Plot_1D(){
  RestFrames::SetStyle();

  //g_PlotTitle = "Baseline Selection";
  g_PlotTitle = "di-lepton selection";
  //g_PlotTitle = "tri-lepton selection";
  //g_PlotTitle = "1 lepton selection";

  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_10_01_20/";

  int BKG_SKIP = 1;

  
  // SampleSet ttX;
  // ttX.SetBkg(true);
  // ttX.SetTitle("t #bar{t} + jets");
  // ttX.SetColor(kAzure+1);
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // ttX.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&ttX);

  // SampleSet ST;
  // ST.SetBkg(true);
  // ST.SetTitle("Single Top");
  // ST.SetColor(kAzure+1);
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // ST.AddFile(NtuplePath+"Fall17_102X/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8_Fall17_102X.root");
  // ST.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&ST);

  // SampleSet DYjets;
  // DYjets.SetBkg(true);
  // DYjets.SetTitle("Z/#gamma * + jets");
  // DYjets.SetColor(kGreen-9);
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-4to50_HT-600toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.AddFile(NtuplePath+"Fall17_102X/DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // DYjets.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&DYjets);

  // SampleSet DB;
  // DB.SetBkg(true);
  // DB.SetTitle("DiBoson");
  // DB.SetColor(kOrange);
  // DB.AddFile(NtuplePath+"Fall17_102X/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_v2_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2L2Nu_13TeV_powheg_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root");
  // DB.AddFile(NtuplePath+"Fall17_102X/ZZTo4L_13TeV_powheg_pythia8_Fall17_102X.root");
  // DB.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&DB);

  // SampleSet Wjets;
  // Wjets.SetBkg(true);
  // Wjets.SetTitle("W + jets");
  // Wjets.SetColor(kRed);
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.AddFile(NtuplePath+"Fall17_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // Wjets.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&Wjets);

  // SampleSet Znunu;
  // Znunu.SetBkg(true);
  // Znunu.SetTitle("Z(#nu #nu) + jets");
  // Znunu.SetColor(kRed);
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Fall17_102X.root");
  // Znunu.AddFile(NtuplePath+"Fall17_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Fall17_102X.root");
  // Znunu.SetSkip(BKG_SKIP);
  // g_Samples.push_back(&Znunu);
  
  
  // SampleSet SIG1;
  // SIG1.SetBkg(false);
  // SIG1.SetTitle("#scale[0.8]{#tilde{t} #tilde{t} #rightarrow b f #bar{f} #tilde{#chi}^{0}_{1} b f #bar{f} #tilde{#chi}^{0}_{1}}; m_{t} = 500, m_{#chi^{0}_{1}} = 420 GeV");
  // SIG1.SetTreeName("SMS_500_420");
  // SIG1.SetColor(kMagenta);
  // SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2-4bd_genMET-80_mStop-500_mLSP-420_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  // SIG1.SetSkip(1);
  // g_Samples.push_back(&SIG1);

  /*
  SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 420 GeV");
  SIG1.SetTreeName("SMS_500_420");
  SIG1.SetColor(kMagenta);
  SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);
  
  SampleSet SIG2p5;
  SIG2p5.SetBkg(false);
  SIG2p5.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 460 GeV");
  SIG2p5.SetTreeName("SMS_500_460");
  SIG2p5.SetColor(kMagenta);
  SIG2p5.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG2p5.SetSkip(1);
  g_Samples.push_back(&SIG2p5);

  SampleSet SIG3;
  SIG3.SetBkg(false);
  SIG3.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 480 GeV");
  SIG3.SetTreeName("SMS_500_480");
  SIG3.SetColor(kMagenta);
  SIG3.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG3.SetSkip(1);
  g_Samples.push_back(&SIG3);

  SampleSet SIG4;
  SIG4.SetBkg(false);
  SIG4.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 490 GeV");
  SIG4.SetTreeName("SMS_500_490");
  SIG4.SetColor(kMagenta);
  SIG4.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG4.SetSkip(1);
  g_Samples.push_back(&SIG4);
  */

  /*
  SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 420 GeV");
  SIG1.SetTreeName("SMS_500_420");
  SIG1.SetColor(kMagenta);
  SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);

  
  SampleSet SIG2p5;
  SIG2p5.SetBkg(false);
  SIG2p5.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 460 GeV");
  SIG2p5.SetTreeName("SMS_500_460");
  SIG2p5.SetColor(kMagenta);
  SIG2p5.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG2p5.SetSkip(1);
  g_Samples.push_back(&SIG2p5);

  SampleSet SIG3;
  SIG3.SetBkg(false);
  SIG3.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 480 GeV");
  SIG3.SetTreeName("SMS_500_480");
  SIG3.SetColor(kMagenta);
  SIG3.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG3.SetSkip(1);
  g_Samples.push_back(&SIG3);

  SampleSet SIG4;
  SIG4.SetBkg(false);
  SIG4.SetTitle("m_{t} = 500, m_{#chi^{0}_{1}} = 490 GeV");
  SIG4.SetTreeName("SMS_500_490");
  SIG4.SetColor(kMagenta);
  SIG4.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG4.SetSkip(1);
  g_Samples.push_back(&SIG4);
  */
  
  /*
 SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("m_{#chi^{#pm}_{1}} = 300, m_{#chi^{0}_{1}} = 230 GeV");
  SIG1.SetTreeName("SMS_300_230");
  SIG1.SetColor(kMagenta);
  SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);
  
  SampleSet SIG2;
  SIG2.SetBkg(false);
  SIG2.SetTitle("m_{#chi^{#pm}_{1}} = 300, m_{#chi^{0}_{1}} = 260 GeV");
  SIG2.SetTreeName("SMS_300_260");
  SIG2.SetColor(kMagenta);
  SIG2.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG2.SetSkip(1);
  g_Samples.push_back(&SIG2);

   SampleSet SIG3;
  SIG3.SetBkg(false);
  SIG3.SetTitle("m_{#chi^{#pm}_{1}} = 300, m_{#chi^{0}_{1}} = 280 GeV");
  SIG3.SetTreeName("SMS_300_280");
  SIG3.SetColor(kMagenta);
  SIG3.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG3.SetSkip(1);
  g_Samples.push_back(&SIG3);

  SampleSet SIG4;
  SIG4.SetBkg(false);
  SIG4.SetTitle("m_{#chi^{#pm}_{1}} = 300, m_{#chi^{0}_{1}} = 290 GeV");
  SIG4.SetTreeName("SMS_300_290");
  SIG4.SetColor(kMagenta);
  SIG4.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG4.SetSkip(1);
  g_Samples.push_back(&SIG4);
  */

  /*
  SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("m_{ #tilde{l}} = 300, m_{#tilde{#chi}^{0}_{1}} = 250 GeV");
  SIG1.SetTreeName("SMS_300_250");
  SIG1.SetColor(kMagenta);
  SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);

  SampleSet SIG2;
  SIG2.SetBkg(false);
  SIG2.SetTitle("m_{ #tilde{l}} = 300, m_{#tilde{#chi}^{0}_{1}} = 270 GeV");
  SIG2.SetTreeName("SMS_300_270");
  SIG2.SetColor(kMagenta);
  SIG2.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG2.SetSkip(1);
  g_Samples.push_back(&SIG2);

  SampleSet SIG3;
  SIG3.SetBkg(false);
  SIG3.SetTitle("m_{ #tilde{l}} = 300, m_{#tilde{#chi}^{0}_{1}} = 280 GeV");
  SIG3.SetTreeName("SMS_300_280");
  SIG3.SetColor(kMagenta);
  SIG3.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG3.SetSkip(1);
  g_Samples.push_back(&SIG3);

  SampleSet SIG4;
  SIG4.SetBkg(false);
  SIG4.SetTitle("m_{ #tilde{l}} = 300, m_{#tilde{#chi}^{0}_{1}} = 290 GeV");
  SIG4.SetTreeName("SMS_300_290");
  SIG4.SetColor(kMagenta);
  SIG4.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG4.SetSkip(1);
  g_Samples.push_back(&SIG4);
  */

  /*
  SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 300, m_{#chi^{0}_{1}} = 240 GeV");
  SIG1.SetTreeName("SMS_300_230");
  SIG1.SetColor(kMagenta);
  SIG1.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);

  SampleSet SIG2;
  SIG2.SetBkg(false);
  SIG2.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 300, m_{#chi^{0}_{1}} = 260 GeV");
  SIG2.SetTreeName("SMS_300_260");
  SIG2.SetColor(kMagenta);
  SIG2.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG2.SetSkip(1);
  g_Samples.push_back(&SIG2);

  SampleSet SIG3;
  SIG3.SetBkg(false);
  SIG3.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 300, m_{#chi^{0}_{1}} = 280 GeV");
  SIG3.SetTreeName("SMS_300_280");
  SIG3.SetColor(kMagenta);
  SIG3.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG3.SetSkip(1);
  g_Samples.push_back(&SIG3);

  SampleSet SIG4;
  SIG4.SetBkg(false);
  SIG4.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 300, m_{#chi^{0}_{1}} = 290 GeV");
  SIG4.SetTreeName("SMS_300_290");
  SIG4.SetColor(kMagenta);
  SIG4.AddFile(NtuplePath+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  SIG4.SetSkip(1);
  g_Samples.push_back(&SIG4);
  */

  int Nsample = g_Samples.size();
 
  // g_Xname = "RISR;
  // //g_Xname = "p_{#font[132]{#it{l^{+}l^{ -}}}}^{#tilde{#chi}_{2}^{0}} [GeV]";
  // g_Xmin = 0.;
  // g_Xmax = 200.;
  // g_NX = 32;
  g_Xname = "M_{#perp}    [GeV]";
  g_Xmin = 0.;
  g_Xmax = 100.;
  g_NX = 30;


  TH1D* hist[Nsample];
  for(int i = 0; i < Nsample; i++)
    hist[i] = new TH1D(("h"+to_string(i)).c_str(),
		       ("h"+to_string(i)).c_str(),
		       g_NX,g_Xmin,g_Xmax);

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

	///////////// 1 lepton
	// if(base->Nlep != 1)
	//   continue;

	// if(base->ID_lep->at(0) < 3)
	//   continue;

	// if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 6.)
	//   continue;

	// if(base->SIP3D_lep->at(0) > 4)
	//   continue;

	//g_Label = "1L Region";
	// if(base->Njet_S->at(1) > 2)
	//   continue;
	// if(base->NSV_S->at(1) != 0)
	//   continue;

	// g_Label = "1L-1SV Region";
	// if(base->Njet_S->at(1) != 0)
	//   continue;
	// if(base->NSV_S->at(1) != 1)
	//   continue;
	/////////////////
	
	///////////// 2 lepton
	if(base->Nlep != 2)
	  continue;

	if(base->ID_lep->at(0) < 3 ||
	   base->ID_lep->at(1) < 3)
	  continue;

	if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 6. ||
	   base->MiniIso_lep->at(1)*base->PT_lep->at(1) > 6.)
	  continue;

	if(base->SIP3D_lep->at(0) > 4 ||
	   base->SIP3D_lep->at(1) > 4)
	continue;

	// TLorentzVector lep1, lep2;
	// TVector3 MET;

	// lep1.SetPtEtaPhiM(base->PT_lep->at(0),base->Eta_lep->at(0),base->Phi_lep->at(0),base->M_lep->at(0));
	// lep2.SetPtEtaPhiM(base->PT_lep->at(1),base->Eta_lep->at(1),base->Phi_lep->at(1),base->M_lep->at(1));
	// MET.SetPtEtaPhi(base->MET,0.,base->MET_phi);

	// double MT2 = Calc_MT2(lep1,lep2,MET);

	// g_Label = "1L-1L Region";
	if(base->Njet_S->at(1) != 0)
	  continue;
	if(base->NSV_S->at(1) != 0)
	  continue;
	/////////////////
	
	/////////////// 3 lepton
	// if(base->Nlep != 3)
	//   continue;

	// if(base->ID_lep->at(0) < 3 ||
	//    base->ID_lep->at(1) < 3 ||
	//    base->ID_lep->at(2) < 3)
	//   continue;

	// if(base->MiniIso_lep->at(0)*base->PT_lep->at(0) > 6. ||
	//    base->MiniIso_lep->at(1)*base->PT_lep->at(1) > 6. ||
	//    base->MiniIso_lep->at(2)*base->PT_lep->at(2) > 6.)
	//   continue;

	// if(base->SIP3D_lep->at(0) > 4 ||
	//    base->SIP3D_lep->at(1) > 4 ||
	//    base->SIP3D_lep->at(2) > 4)
	//   continue;
	//////////////////////////

	 
	if(base->MET < 200)
	  continue;
	
	if(base->PTISR->at(1) < 200)
	  continue;

	if(base->RISR->at(1) < 0.5)
	  continue;

	double MXperp = sqrt(base->MX3a_BoostT->at(1)*base->MX3a_BoostT->at(1)+base->MX3b_BoostT->at(1)*base->MX3b_BoostT->at(1))/sqrt(2.);
	
	// if(base->Nbjet_ISR->at(1) > 0)
	//   continue;

	hist[s]->Fill(MXperp, base->weight*double(SKIP));
      }
      
      delete base;
      delete chain;
    }
  }
      

  
  double max = -1.;
  int imax = -1;
  for(int i = 0; i < Nsample; i++){
    cout << g_Samples[i]->GetTitle().c_str() << " " << hist[i]->Integral()*137 << " total" << endl;
    hist[i]->Scale(1./hist[i]->Integral());
    //hist[i]->Scale(1./hist[i]->GetMaximum());
    if(hist[i]->GetMaximum() > max){
      max = hist[i]->GetMaximum();
      imax = i;
    }
  }
  
  float width = hist[0]->GetBinWidth(1);
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
  can->Draw();
  can->cd();
  // for(int i = 0; i < hist[imax]->GetNbinsX(); i++){
  //   char* sbin = new char[20];
  //   sprintf(sbin,"#geq %d", i);
  //   hist[imax]->GetXaxis()->SetBinLabel(i+1,sbin);
  // }
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
  hist[imax]->GetYaxis()->SetTitleOffset(1.1);
  hist[imax]->GetYaxis()->SetLabelFont(42);
  hist[imax]->GetYaxis()->SetLabelSize(0.05);
  hist[imax]->GetYaxis()->SetTitle("a. u.");
  hist[imax]->GetYaxis()->SetRangeUser(0., hist[imax]->GetMaximum()*1.1);
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
    hist[i]->SetLineColor(mycolor[i]);
    hist[i]->SetLineWidth(3);
    if(i >= 4 && false){
      if(i%2 == 0)
	hist[i]->SetLineStyle(7);
      if(i%2 == 1)
	hist[i]->SetLineStyle(9);
      hist[i]->SetLineWidth(4);
    }
    hist[i]->SetMarkerColor(mycolor[i]);
    hist[i]->SetMarkerSize(0);
    hist[i]->SetFillColor(kWhite);
    Ntype[0]++;
    hist[i]->Draw("hist SAME");
  }

  TLegend* leg = new TLegend(0.688,0.22,0.93,0.42);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  for(int i = 0; i < Nsample; i++)
    leg->AddEntry(hist[i],g_Samples[i]->GetTitle().c_str());
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
l.SetTextSize(0.04);
 l.DrawLatex(0.69,0.85,"#tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm} #rightarrow Z #tilde{#chi}_{1}^{0} W #tilde{#chi}_{1}^{0}");
 l.DrawLatex(0.69,0.85,"#tilde{#chi}_{1}^{#pm} #tilde{#chi}_{1}^{#pm} #rightarrow W* #tilde{#chi}_{1}^{0} W* #tilde{#chi}_{1}^{0}");
  l.DrawLatex(0.69,0.85,"#tilde{t} #tilde{t} #rightarrow b W* #tilde{#chi}_{1}^{0} b W* #tilde{#chi}_{1}^{0}");
  l.DrawLatex(0.69,0.85,"#tilde{t} #tilde{t} #rightarrow b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1}) b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1})");
  l.DrawLatex(0.69,0.85,"#tilde{t} #tilde{t} #rightarrow b f #bar{f} #tilde{#chi}^{0}_{1} b f #bar{f} #tilde{#chi}^{0}_{1}");
  l.DrawLatex(0.69,0.85,"#tilde{l} #tilde{l} #rightarrow l #tilde{#chi}^{0}_{1} l #tilde{#chi}^{0}_{1}");

  // l.SetTextSize(0.045);
  // l.SetTextFont(132);
  // string bla = "#scale[0.6]{#int} #it{L dt} = "+to_string(int(g_lumi))+" fb^{-1},  #Delta_{N#scale[0.8]{bkg}} = ";
  // bla += to_string(int(g_deltaNbkg))+" %";
  // l.DrawLatex(0.61,0.943,bla.c_str());


}

