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
#include <TEfficiency.h>

#include "RestFrames/RestFrames.hh"
#include "../include/ReducedBase_slim.hh"
#include "../include/SampleSet.hh"

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

void Plot_Btag(){
  RestFrames::SetStyle();

  g_PlotTitle = "DeepJet Medium";

  string NtuplePath = "/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_10_01_20/";

  int BKG_SKIP = 1;

  
  SampleSet ttX;
  ttX.SetBkg(true);
  ttX.SetTitle("t #bar{t} + jets");
  ttX.SetColor(kAzure+1);
  ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  ttX.AddFile(NtuplePath+"Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
  ttX.SetSkip(BKG_SKIP);
  g_Samples.push_back(&ttX);

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
  g_Xname = "p_{T} [GeV]";
  g_Xmin = 20.;
  g_Xmax = 1000.;
  g_NX = 100;


  TH1D* hist_num[3][Nsample];
  TH1D* hist_den[3][Nsample];
  for(int i = 0; i < Nsample; i++){
    for(int j = 0; j < 3; j++){
      hist_num[j][i] = new TH1D(Form("num_%d_%d",j,i),
				Form("num_%d_%d",j,i),
				g_NX,g_Xmin,g_Xmax);
      hist_den[j][i] = new TH1D(Form("den_%d_%d",j,i),
				Form("den_%d_%d",j,i),
				g_NX,g_Xmin,g_Xmax);
    }
  }

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

	if(base->PT_jet == nullptr ||
	   base->Eta_jet == nullptr ||
	   base->Flavor_jet == nullptr ||
	   base->BtagID_jet == nullptr ||
	   base->PT_jet->size() != base->Njet ||
	   base->Eta_jet->size() != base->Njet ||
	   base->Flavor_jet->size() != base->Njet ||
	   base->BtagID_jet->size() != base->Njet)
	  continue;
	for(int j = 0; j < base->Njet; j++){
	  if(base->PT_jet->at(j) < 20.)
	    continue;
	  if(fabs(base->Eta_jet->at(j)) > 2.5)
	    continue;

	  int flavor;
	  if(abs(base->Flavor_jet->at(j)) == 5)
	    flavor = 0;
	  else if(abs(base->Flavor_jet->at(j)) == 4)
	    flavor = 1;
	  else
	    flavor = 2;

	  hist_den[flavor][s]->Fill(base->PT_jet->at(j));
	  if(base->BtagID_jet->at(j) >= 3)
	    hist_num[flavor][s]->Fill(base->PT_jet->at(j));
	}
      }
      
      delete base;
      delete chain;
    }
  }

  TEfficiency* gr[3][Nsample];
  for(int i = 0; i < Nsample; i++){
    for(int j = 0; j < 3; j++){
      gr[j][i] = (TEfficiency*) new TEfficiency(*hist_num[j][i],*hist_den[j][i]);
    }
  }
  
  double max = -1.;
  int imax = -1;
  
  float width = hist_num[0][0]->GetBinWidth(1);
  char *yaxis = new char[100];
  sprintf(yaxis,"Events / %f", width);

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = (TCanvas*) new TCanvas("can","can",600.,500);

  for(int s = 0; s < g_NX; s++){
    hist_num[0][0]->SetBinContent(s+1, 0.);
    hist_num[0][0]->SetBinError(s+1, 0.);
  }
  
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
  hist_num[0][0]->Draw();
  hist_num[0][0]->GetXaxis()->CenterTitle();
  hist_num[0][0]->GetXaxis()->SetTitleFont(42);
  hist_num[0][0]->GetXaxis()->SetTitleSize(0.06);
  hist_num[0][0]->GetXaxis()->SetTitleOffset(1.06);
  hist_num[0][0]->GetXaxis()->SetLabelFont(42);
  hist_num[0][0]->GetXaxis()->SetLabelSize(0.05);
  hist_num[0][0]->GetXaxis()->SetTitle(g_Xname.c_str());
  hist_num[0][0]->GetYaxis()->CenterTitle();
  hist_num[0][0]->GetYaxis()->SetTitleSize(0.06);
  hist_num[0][0]->GetYaxis()->SetTitleOffset(1.1);
  hist_num[0][0]->GetYaxis()->SetLabelFont(42);
  hist_num[0][0]->GetYaxis()->SetLabelSize(0.05);
  hist_num[0][0]->GetYaxis()->SetTitle("a. u.");
  hist_num[0][0]->GetYaxis()->SetRangeUser(0., 1.);
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

  int mymark[8];
  mymark[0] = 30;
  mymark[1] = 26;
  mymark[2] = 25;
 
  
  
  Ntype[0] = 0;
  for(int i = Nsample-1; i >= 0; i--){
    for(int j = 0; j < 3; j++){
      gr[j][i]->SetLineColor(mycolor[i]);
      gr[j][i]->SetLineWidth(2);
      gr[j][i]->SetMarkerStyle(mymark[j]);
      gr[j][i]->SetMarkerColor(mycolor[i]);
      gr[j][i]->SetMarkerSize(2);
      gr[j][i]->SetFillColor(kWhite);
      gr[j][i]->Draw("same P");
    }
  }

  TLegend* leg = new TLegend(0.688,0.22,0.93,0.42);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  for(int i = 0; i < Nsample; i++)
    leg->AddEntry(gr[0][i],g_Samples[i]->GetTitle().c_str());
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
 // l.DrawLatex(0.69,0.85,"#tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm} #rightarrow Z #tilde{#chi}_{1}^{0} W #tilde{#chi}_{1}^{0}");
 // l.DrawLatex(0.69,0.85,"#tilde{#chi}_{1}^{#pm} #tilde{#chi}_{1}^{#pm} #rightarrow W* #tilde{#chi}_{1}^{0} W* #tilde{#chi}_{1}^{0}");
 //  l.DrawLatex(0.69,0.85,"#tilde{t} #tilde{t} #rightarrow b W* #tilde{#chi}_{1}^{0} b W* #tilde{#chi}_{1}^{0}");
 //  l.DrawLatex(0.69,0.85,"#tilde{t} #tilde{t} #rightarrow b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1}) b #tilde{#chi}^{#pm}_{1}(f #bar{f} #tilde{#chi}^{0}_{1})");
 //  l.DrawLatex(0.69,0.85,"#tilde{t} #tilde{t} #rightarrow b f #bar{f} #tilde{#chi}^{0}_{1} b f #bar{f} #tilde{#chi}^{0}_{1}");
 //  l.DrawLatex(0.69,0.85,"#tilde{l} #tilde{l} #rightarrow l #tilde{#chi}^{0}_{1} l #tilde{#chi}^{0}_{1}");

  // l.SetTextSize(0.045);
  // l.SetTextFont(132);
  // string bla = "#scale[0.6]{#int} #it{L dt} = "+to_string(int(g_lumi))+" fb^{-1},  #Delta_{N#scale[0.8]{bkg}} = ";
  // bla += to_string(int(g_deltaNbkg))+" %";
  // l.DrawLatex(0.61,0.943,bla.c_str());

 // TFile* output = new TFile("BtagEff.root", "UPDATE");
 // output->cd();
 // for(int i = 0; i < 3; i++){
 //   gr[i][0]->Write(Form("BtagEff_2017_flavor%d", i), TObject::kOverwrite);
 // }

}

