#include "../include/SampleTool.hh"

///////////////////////////////////////////
////////// SampleTool class
///////////////////////////////////////////

SampleTool::SampleTool(const string& ntuple_path, int year){
  m_Path  = ntuple_path;

  if(!m_ProcInit)
    InitProcMap();

  m_iYear = YearMap(year);
}

SampleTool::~SampleTool() {}

SampleTool& SampleTool::SetNtuplePath(const string& ntuple_path){
  m_Path = m_iYear;

  return *this;
}

SampleTool& SampleTool::SetYear(int year){
  m_iYear = YearMap(year);

  return *this;
}

double SampleTool::Lumi(){
  return m_Lumi[m_iYear];
}

ProcessList SampleTool::Get(const string& name) const {
  ProcessList list;

  for(auto p = m_Proc[m_iYear].begin(); p != m_Proc[m_iYear].end(); p++){
    if(p->first.Name().find(name) != std::string::npos)
      list += p->first;
  }

  return list;
}

ProcessList SampleTool::Get(ProcessType type) const {
  ProcessList list;

  for(auto p = m_Proc[m_iYear].begin(); p != m_Proc[m_iYear].end(); p++){
    if(p->first.Type() == type)
      list += p->first;
  }

  return list;
}
 
  
int SampleTool::NTrees(const Process& proc){
  if(m_Proc[m_iYear].count(proc) == 0)
    return 0;

  return int(m_Proc[m_iYear][proc].first.size());
}

TChain* SampleTool::Tree(const Process& proc, int itree){
  if(m_Proc[m_iYear].count(proc) == 0)
    return nullptr;

  TChain* chain = new TChain(m_Proc[m_iYear][proc].second.c_str());
  int Nfile = int(m_Proc[m_iYear][proc].first.size());
  
  if(itree < 0 || itree >= Nfile){
    for(int i = 0; i < Nfile; i++)
      chain->AddFile(m_Proc[m_iYear][proc].first[i].c_str());
  } else {
    chain->AddFile(m_Proc[m_iYear][proc].first[itree].c_str());
  }

  return chain;
}

string SampleTool::TreeName(const Process& proc, int itree){
  if(m_Proc[m_iYear].count(proc) == 0)
    return "";
  
  return m_Proc[m_iYear][proc].second;
}

string SampleTool::FileName(const Process& proc, int itree){
  if(m_Proc[m_iYear].count(proc) == 0)
    return "";

  int Nfile = int(m_Proc[m_iYear][proc].first.size());
  if(itree < 0 || itree >= Nfile)
    return "";

  return m_Proc[m_iYear][proc].first[itree];
}

int SampleTool::YearMap(int year){
  int ydef = 1; // 2017 default for now
  if(year < 0)
    return ydef;
  if(year < 3)
    return year;
  if(year < 2016)
    return ydef;
  if(year < 2019)
    return year - 2016;

  return ydef;
}

void SampleTool::InitSMS(const string& prefix, const string& filename){

  TFile file(filename.c_str(), "READ");
  if(!file.IsOpen())
    return;
 
  TIter list(file.GetListOfKeys());
  TKey* key;
  int M0, M1;
  string name;
  VS files;
  while((key = (TKey*)list.Next())){
    name = string(key->GetName());
    if(name.find("SMS") == std::string::npos)
      continue;
    sscanf(name.c_str(), "SMS_%d_%d", &M0, &M1);

    Process proc(Form("%s_%d", prefix.c_str(), 100000*M0+M1), kSig);
    files.clear();
    files += filename;
    m_Proc[m_iYear][proc] = pair<vector<string>,string>(files, name);
  }
  file.Close();
}

void SampleTool::InitProcMap(){
  m_ProcInit = true;

  m_Lumi[0] = 35.922; // 2016 lumi
  m_Lumi[1] = 41.529; // 2017 lumi
  m_Lumi[2] = 59.74;  // 2018 lumi

  VS list;
  
  ///////////////////////////////////////////
  ////////// 2016 samples
  ///////////////////////////////////////////
  m_iYear = 0;

  ///////////////////////////////////////////
  ////////// 2017 samples
  ///////////////////////////////////////////
  m_iYear = 1;
  {
    Process ttbar("ttbar", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    m_Proc[m_iYear][ttbar] = pair<vector<string>,string>(list, "KUAnalysis");

    Process ST("ST", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8_Fall17_102X.root";
    m_Proc[m_iYear][ST] = pair<vector<string>,string>(list, "KUAnalysis");
  
    Process ZDY("ZDY", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-4to50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-4to50_HT-600toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Fall17_102X.root";
    m_Proc[m_iYear][ZDY] = pair<vector<string>,string>(list, "KUAnalysis");

    Process DB("DB", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_v2_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZZTo2L2Nu_13TeV_powheg_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZZTo4L_13TeV_powheg_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root";
    m_Proc[m_iYear][DB] = pair<vector<string>,string>(list, "KUAnalysis");

    Process Wjets("Wjets", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    m_Proc[m_iYear][Wjets] = pair<vector<string>,string>(list, "KUAnalysis");

    Process TB("TB", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WZG_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WWG_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    m_Proc[m_iYear][TB] = pair<vector<string>,string>(list, "KUAnalysis");
  
    Process QCD("QCD", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/QCD_HT100to200_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/QCD_HT200to300_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/QCD_HT300to500_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/QCD_HT500to700_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/QCD_HT700to1000_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    m_Proc[m_iYear][QCD] = pair<vector<string>,string>(list, "KUAnalysis");

    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");

    InitSMS("T2tt", m_Path+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");

    InitSMS("TChiWZ", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");

    InitSMS("TChipmWW", m_Path+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");

    InitSMS("TSlepSlep", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root");

    Process data_obs("data_obs", kData);
    list.clear();
    list += m_Path + "Fall17_102X_Data/MET_Run2017B-Nano25Oct2019-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017C-Nano25Oct2019-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017D-Nano25Oct2019-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017E-Nano25Oct2019-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017F-Nano25Oct2019-v1_2017_Fall17_102X.root";
    m_Proc[m_iYear][data_obs] = pair<vector<string>,string>(list, "KUAnalysis");

  }
  
  ///////////////////////////////////////////
  ////////// 2018 samples
  ///////////////////////////////////////////
  m_iYear = 2;
  
}
  

bool SampleTool::m_ProcInit = false;

std::map<Process, pair<vector<string>,string> > SampleTool::m_Proc[3];

double SampleTool::m_Lumi[3];
