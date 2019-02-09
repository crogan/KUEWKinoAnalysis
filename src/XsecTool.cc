#include "XsecTool.hh"

XsecTool::XsecTool(){

}

XsecTool::~XsecTool(){

}

std::map<std::string,double> XsecTool::InitMap_Xsec_BKG(){
  std::map<std::string,double> Label2Xsec;
  
  Label2Xsec["DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_120to170_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_15to30_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_170to300_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_300to470_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_30to50_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_470to600_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_50to80_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_600to800_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_800to1000_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_80to120_TuneCP5_13TeV_pythia8"] = 1;
  Label2Xsec["QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8"] = 1;
  Label2Xsec["ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 1;
  Label2Xsec["ST_t-channel_antitop_5f_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 1;
  Label2Xsec["ST_t-channel_top_5f_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 1;
  Label2Xsec["ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 1;
  Label2Xsec["ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8"] = 1;
  Label2Xsec["TTJets_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8"] = 1;
  Label2Xsec["TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["TTToHadronic_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["TTWH_TuneCP5_13TeV-madgraph-pythia8"] = 1;
  Label2Xsec["TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;
  Label2Xsec["TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;
  Label2Xsec["TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;
  Label2Xsec["TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;
  Label2Xsec["TTWW_TuneCP5_13TeV-madgraph-pythia8"] = 1;
  Label2Xsec["TTWZ_TuneCP5_13TeV-madgraph-pythia8"] = 1;
  Label2Xsec["TTZH_TuneCP5_13TeV-madgraph-pythia8"] = 1;
  Label2Xsec["TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["TTZZ_TuneCP5_13TeV-madgraph-pythia8"] = 1;
  Label2Xsec["WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["WJetsToQQ_HT400to600_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["WJetsToQQ_HT600to800_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["WWG_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["WWTo2L2Nu_13TeV-powheg"] = 1;
  Label2Xsec["WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["WWTo4Q_13TeV-powheg"] = 1;
  Label2Xsec["WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["WWToLNuQQ_13TeV-powheg"] = 1;
  Label2Xsec["WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["WW_TuneCP5_13TeV-pythia8"] = 1;
  Label2Xsec["WZ_TuneCP5_13TeV-pythia8"] = 1;
  Label2Xsec["WZ_TuneCUETP8M1_13TeV-pythia8"] = 1;
  Label2Xsec["ZJetsToNuNu_HT-100To200_13TeV-madgraph"] = 1;
  Label2Xsec["ZJetsToNuNu_HT-1200To2500_13TeV-madgraph"] = 1;
  Label2Xsec["ZJetsToNuNu_HT-200To400_13TeV-madgraph"] = 1;
  Label2Xsec["ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph"] = 1;
  Label2Xsec["ZJetsToNuNu_HT-400To600_13TeV-madgraph"] = 1;
  Label2Xsec["ZJetsToNuNu_HT-600To800_13TeV-madgraph"] = 1;
  Label2Xsec["ZJetsToNuNu_HT-800To1200_13TeV-madgraph"] = 1;
  Label2Xsec["ZJetsToQQ_HT-800toInf_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["ZJetsToQQ_HT400to600_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["ZJetsToQQ_HT600to800_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;
  Label2Xsec["ZZTo2L2Nu_13TeV_powheg_pythia8"] = 1;
  Label2Xsec["ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;
  Label2Xsec["ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;
  Label2Xsec["ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;
  Label2Xsec["ZZTo4L_13TeV_powheg_pythia8"] = 1;
  Label2Xsec["ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;
  Label2Xsec["ZZZ_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;
  Label2Xsec["ZZ_TuneCP5_13TeV-pythia8"] = 1;
  Label2Xsec["ttH_M125_TuneCP5_13TeV-powheg-pythia8"] = 1;
  Label2Xsec["ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 1;
  Label2Xsec["ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 1;

  return Label2Xsec;
}

std::map<std::string,double> XsecTool::m_Label2Xsec_BKG = XsecTool::InitMap_Xsec_BKG();
