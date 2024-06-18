#include "../include/SampleTool.hh"
#include <TSystem.h>
#include <TKey.h>

///////////////////////////////////////////
////////// SampleTool class ///////////////
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
double SampleTool::HEMLumi(bool HEM_Veto){
	if(HEM_Veto){
		return m_HEMLumi[0]; //prehem lumi
	}
	else{
		return m_HEMLumi[1]; //full lumi
	}
}

ProcessList SampleTool::Get(const string& name) const {
  ProcessList list;

  for(auto p = m_Proc[m_iYear].begin(); p != m_Proc[m_iYear].end(); p++){
    if(p->first.Name().find(name) != std::string::npos)
      list += p->first;
  }

  return list;
}
ProcessList SampleTool::GetStrictSignalMatch( const string& name) const {
  ProcessList list;
  bool isSigSysProc= false;
  std::string token1,token2,token3;
  //look for up down or GenMET, if they are in the string its a sig sys process
  if(name.find("Up") != std::string::npos || name.find("Down") != std::string::npos || name.find("GenMET") != std::string::npos){
    isSigSysProc = true;
  }
  bool isThisProcSys=false;
  std::string thisprocname;
  //cout<<" processing string: "<<name<<" isSigSysProc="<<isSigSysProc<<"\n";
  for(auto p = m_Proc[m_iYear].begin(); p != m_Proc[m_iYear].end(); p++){
    thisprocname = p->first.Name();
    if(thisprocname.find("Up") != std::string::npos || thisprocname.find("Down") != std::string::npos || thisprocname.find("GenMET") != std::string::npos){
     isThisProcSys = true;
    }
	//tokenize name if sys
	//std::string nameCopy = name;
   if(isThisProcSys){
	std::stringstream iss(name);
	std::getline(iss,token1,'_');
	std::getline(iss,token2,'_');
	std::getline(iss,token3,'_');
	//std::cout<<"got tokens "<<token1<<" "<<token2<<" "<<token3<<" \n";
   }
   // cout<<"perform comparisons for:"<<thisprocname<<" isThisProcSys="<<isThisProcSys<<"\n";
    //if it is not signal sys then so should be the proc
    if(!isSigSysProc &&  !isThisProcSys ){
      if(p->first.Name().find(name) != std::string::npos)
        list += p->first;
    }
    //if it is a singal sys then the the proc must also be
    if(isSigSysProc && isThisProcSys ){
      //if(p->first.Name().find(name) != std::string::npos)
      	if(p->first.Name().find(token1) != std::string::npos &&p->first.Name().find(token2) != std::string::npos && p->first.Name().find(token3) != std::string::npos)
	list += p->first;
    }
	//reset this proc flag
      isThisProcSys=false;
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

bool SampleTool::IsFastSim(const Process& proc, int itree){
  if(m_SProcInit[m_iYear].count(proc) == 0)
    return false;

  if(!m_SProcInit[m_iYear][proc])
    InitSignalProc(proc);

  return m_SProcFS[m_iYear][proc][FileName(proc, itree)];
}

bool SampleTool::FilterDilepton(const Process& proc, int itree){
  if(m_SProcInit[m_iYear].count(proc) == 0)
    return false;

  if(!m_SProcInit[m_iYear][proc])
    InitSignalProc(proc);

  return m_SProcDL[m_iYear][proc][FileName(proc, itree)];
}

SleptonFlavor SampleTool::FilterSleptons(const Process& proc, int itree){
  if(m_SProcInit[m_iYear].count(proc) == 0)
    return kSmuSel;

  if(!m_SProcInit[m_iYear][proc])
    InitSignalProc(proc);

  return m_SProcSlepFlavor[m_iYear][proc][FileName(proc, itree)];
}

double SampleTool::GetSampleWeight(const Process& proc, int itree){
  if(m_SProcInit[m_iYear].count(proc) == 0)
    return 1.;

  if(!m_SProcInit[m_iYear][proc])
    InitSignalProc(proc);
  
  return m_SProcW[m_iYear][proc][FileName(proc, itree)];
}

void SampleTool::InitSignalProc(const Process& proc){
  if(m_SProcInit[m_iYear].count(proc) == 0)
    return;
  if(m_SProcInit[m_iYear][proc])
    return;

  int countDL = 0;
  int countIN = 0;
  vector<int> count;
  bool isDL = false;
  int Nfile = int(m_Proc[m_iYear][proc].first.size());
  
  for(int f = 0; f < Nfile; f++){
    TChain* chain = new TChain(m_Proc[m_iYear][proc].second.c_str());
    string filename = m_Proc[m_iYear][proc].first[f];
    chain->AddFile(filename.c_str());
    count.push_back(chain->GetEntries());
    if(chain)
      delete chain;
    
    if(count[f] <= 0)
      continue;
    
    if(m_SProcDL[m_iYear][proc][filename]){
      countDL += count[f];
      isDL = true;
    } else
      countIN += count[f];
  }

  for(int f = 0; f < Nfile; f++){
    string filename = m_Proc[m_iYear][proc].first[f].c_str();
    if(m_SProcDL[m_iYear][proc][filename]){
      m_SProcDL[m_iYear][proc][filename] = false;
      m_SProcW[m_iYear][proc][filename] *= double(count[f])/double(countDL);
    } else {
      if(isDL)
	m_SProcDL[m_iYear][proc][filename] = true;
      m_SProcW[m_iYear][proc][filename] *= double(count[f])/double(countIN);
    }
  }
  m_SProcInit[m_iYear][proc] = true;
}
  
int SampleTool::NTrees(const Process& proc){
  if(m_Proc[m_iYear].count(proc) == 0)
    return 0;

  return int(m_Proc[m_iYear][proc].first.size());
}

TChain* SampleTool::Tree(const Process& proc, int itree, std::string treeName){
  if(m_Proc[m_iYear].count(proc) == 0)
    return nullptr;

  if(m_SProcInit[m_iYear].count(proc) > 0)
    if(!m_SProcInit[m_iYear][proc])
      InitSignalProc(proc);
  TChain* chain;
  if(treeName==""){
  	chain = new TChain(m_Proc[m_iYear][proc].second.c_str());
  }
  else{
	chain = new TChain(treeName.c_str());
  }
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
void SampleTool::InitSMS_treeSys(const string& treeSys, const string& prefix, const string& filename, double weight, bool FS, bool DL, SleptonFlavor kFlavor){
   //duplicate initSMS but this time load tree sys as a separate process
   if(gSystem->AccessPathName(filename.c_str())) return;
  TFile* file = TFile::Open(filename.c_str(), "READ");
  //cout << "is " << filename << " open? " << file->IsOpen() << endl;
  if(!file->IsOpen()) return;

  TIter list(file->GetListOfKeys());
  TKey* key;
  int M0, M1;
  string name;
  VS files;
  while((key = (TKey*)list.Next())){
    name = string(key->GetName());
    if(name.find("SMS") == std::string::npos)
      continue;
    if(name.find(treeSys.c_str()) ==std::string::npos)
      continue;
    // skip nominal trees
    if(isdigit(name.back()))
      continue;
    sscanf(name.c_str(), "SMS_%d_%d", &M0, &M1);
    
  //  std::cout<<"scanning "<<name.c_str()<<"\n";
  //  std::cout<<"found: "<<M0<<" "<<M1<<"\n";
    const std::string uscore = "_";
    const std::string newlit = treeSys;
   // const std::string new_prefix = ((prefix+uscore)+newlit);
  //  std::cout<<"loading new prefix "<< new_prefix.c_str()<< std::endl;    
    
      std::string new_prefix = Form("%s_%d", prefix.c_str(), 10000*M0+M1);
      std::string newtreesys = std::string(treeSys);
      new_prefix = new_prefix+"_"+newtreesys;
    // std::cout<<"created process prefix "<< new_prefix<<"\n";
   // Process proc(Form("%s_%d", new_prefix.c_str(), 10000*M0+M1), kSig);
   	Process proc(new_prefix,kSig);
    //Process proc(Form("%s_%d", prefix.c_str(), 10000*M0+M1), kSig);
    files.clear();
    if(m_Proc[m_iYear].count(proc) == 0){
      files += filename;
      m_Proc[m_iYear][proc] = pair<vector<string>,string>(files, name);
      m_SProcInit[m_iYear][proc] = false;
      m_SProcFS[m_iYear][proc] = std::map<string,bool>();
      m_SProcDL[m_iYear][proc] = std::map<string,bool>();
      m_SProcSlepFlavor[m_iYear][proc] = std::map<string,SleptonFlavor>();
      m_SProcW[m_iYear][proc] = std::map<string,double>();
      m_SProcFS[m_iYear][proc][filename] = FS;
      m_SProcDL[m_iYear][proc][filename] = DL;
      m_SProcSlepFlavor[m_iYear][proc][filename] = kFlavor;
      m_SProcW[m_iYear][proc][filename] = weight;
    } else {
      m_Proc[m_iYear][proc].first.push_back(filename);
      m_SProcFS[m_iYear][proc][filename] = FS;
      m_SProcDL[m_iYear][proc][filename] = DL;
      m_SProcSlepFlavor[m_iYear][proc][filename] = kFlavor;
      m_SProcW[m_iYear][proc][filename] = weight;
    }
 
  }
  file->Close();


}
//void SampleTool::InitSMS(const string& prefix, const string& filename, double weight, bool FS, bool DL){
void SampleTool::InitSMS(const string& prefix, const string& filename, double weight, bool FS, bool DL, SleptonFlavor kFlavor){
  //TFile file;
  //file.Open(filename.c_str(), "READ"); //
  if(gSystem->AccessPathName(filename.c_str())) return;
  TFile* file = TFile::Open(filename.c_str(), "READ");  
  //cout << "is " << filename << " open? " << file->IsOpen() << endl;
  if(!file->IsOpen()) return;
 
  TIter list(file->GetListOfKeys());
  TKey* key;
  int M0, M1;
  string name;
  VS files;
  while((key = (TKey*)list.Next())){
    name = string(key->GetName());
    if(name.find("SMS") == std::string::npos)
      continue;
    // skip systematics trees
    if(!isdigit(name.back()))
      continue;
    sscanf(name.c_str(), "SMS_%d_%d", &M0, &M1);

    Process proc(Form("%s_%d", prefix.c_str(), 10000*M0+M1), kSig);
    files.clear();
    if(m_Proc[m_iYear].count(proc) == 0){
      files += filename;
      m_Proc[m_iYear][proc] = pair<vector<string>,string>(files, name);
      m_SProcInit[m_iYear][proc] = false;
      m_SProcFS[m_iYear][proc] = std::map<string,bool>();
      m_SProcDL[m_iYear][proc] = std::map<string,bool>();
      m_SProcSlepFlavor[m_iYear][proc] = std::map<string,SleptonFlavor>();
      m_SProcW[m_iYear][proc] = std::map<string,double>();
      m_SProcFS[m_iYear][proc][filename] = FS;
      m_SProcDL[m_iYear][proc][filename] = DL;
      m_SProcSlepFlavor[m_iYear][proc][filename] = kFlavor;
      m_SProcW[m_iYear][proc][filename] = weight;
    } else {
      m_Proc[m_iYear][proc].first.push_back(filename);
      m_SProcFS[m_iYear][proc][filename] = FS;
      m_SProcDL[m_iYear][proc][filename] = DL;
      m_SProcSlepFlavor[m_iYear][proc][filename] = kFlavor;
      m_SProcW[m_iYear][proc][filename] = weight;
    }
    
   
  }
  file->Close();
  //register signal sys trees as different procs
  std::vector<std::string> sysTrees = {"JESUncer_TotalUp", "JESUncer_TotalDown","JERUncer_TotalUp", "JERUncer_TotalDown", "METUncer_UnClustUp", "METUncer_UnClustDown", "METUncer_GenMET"};

  for(int i=0; i< sysTrees.size(); i++){
    std::cout<<"Initializing SMS treesys: "<<prefix.c_str()<< "_"<<sysTrees.at(i)<<"\n";
    InitSMS_treeSys(sysTrees.at(i), prefix, filename, weight, FS, DL, kFlavor);
//    break;
  }

}
void SampleTool::InitProcMap(){
  m_ProcInit = true;

  m_Lumi[0] = 36.333380074;    // 2016 lumi
  m_Lumi[1] = 41.479680529;    // 2017 lumi
  //m_Lumi[2] = 59.832475339;     // 2018 lumi
  //m_Lumi[2] = 21.077794578;    // 2018 preHEM lumi
  m_Lumi[2] = 21.077794578 + 0.95*38.662770624;    // 2018 preHEM + post w/ rough veto eff 
  
  m_HEMLumi[0] = 21.077794578; //pre hem
  m_HEMLumi[1] = 59.832475339; // full lumi

  // 2018 lumi ---> 21.077794578, 38.662770624 (pre/post HEM)

  VS list;
 //bool DEV=false;  
  ///////////////////////////////////////////
  ////////// 2016 samples ///////////////////
  ///////////////////////////////////////////
  m_iYear = 0;
  {
//	if(DEV){
//    Process ZDY("ZDY", kBkg);
 //   list.clear();
 //   list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
   //     m_Proc[m_iYear][ZDY] = pair<vector<string>,string>(list, "KUAnalysis");
 //   return;
 //   }

    // ------------------ //
    // --- Background --- //
    // ------------------ //
    
    Process ttbar("ttbar", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_Summer16_102X.root";
    //still processing//list += m_Path + "Summer16_102X/TTWW_TuneCUETP8M2T4_13TeV-madgraph-pythia8.root";
    list += m_Path + "Summer16_102X/TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_Summer16_102X.root";
    m_Proc[m_iYear][ttbar] = pair<vector<string>,string>(list, "KUAnalysis");

    Process ST("ST", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_Summer16_102X.root";
    //list += m_Path + "Summer16_102X/ST_s-channel_4f_hadronicDecays_TuneCP5_PSweights_13TeV-amcatnlo-pythia8_Summer16_102X.root"; //to be used with new ntuples
    list += m_Path + "Summer16_102X/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_Summer16_102X.root";
    m_Proc[m_iYear][ST] = pair<vector<string>,string>(list, "KUAnalysis");
  
    Process ZDY("ZDY", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/DYJetsToLL_M-5to50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-5to50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-5to50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-5to50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Summer16_102X.root";
    m_Proc[m_iYear][ZDY] = pair<vector<string>,string>(list, "KUAnalysis");

    Process DB("DB", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/WWTo2L2Nu_13TeV-powheg_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WWTo4Q_13TeV-powheg_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WWToLNuQQ_13TeV-powheg_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZZTo2L2Nu_13TeV_powheg_pythia8_ext1_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZZTo2L2Q_13TeV_powheg_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/GluGluHToTauTau_M125_13TeV_powheg_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/GluGluHToWWTo2L2Nu_M125_13TeV_powheg_JHUgenv628_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/GluGluHToWWToLNuQQ_M125_13TeV_powheg_JHUGenV628_pythia8_Summer16_102X.root ";
    list += m_Path + "Summer16_102X/GluGluHToZZTo2L2Nu_M125_TuneCUETP8M1_13TeV_powheg2_JHUGenV735_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WminusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WplusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZH_HToBB_ZToNuNu_M125_13TeV_powheg_pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_Summer16_102X.root";
    m_Proc[m_iYear][DB] = pair<vector<string>,string>(list, "KUAnalysis");



    Process Wjets("Wjets", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    m_Proc[m_iYear][Wjets] = pair<vector<string>,string>(list, "KUAnalysis");


/*

    Process Wjets("Wjets", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/WJetsToLNu_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_Pt-400To600_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WJetsToLNu_Pt-600ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_Summer16_102X.root";
    m_Proc[m_iYear][Wjets] = pair<vector<string>,string>(list, "KUAnalysis");
*/


    Process TB("TB", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8_Summer16_102X.root";
    m_Proc[m_iYear][TB] = pair<vector<string>,string>(list, "KUAnalysis");
  
    Process QCD("QCD", kBkg);
    list.clear();
    list += m_Path + "Summer16_102X/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    list += m_Path + "Summer16_102X/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root";
    m_Proc[m_iYear][QCD] = pair<vector<string>,string>(list, "KUAnalysis");
    
    // -------------- //
    // --- Signal --- //
    // -------------- //


    ////InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    ////InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    ////InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-10to80_2Lfilter_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", (3.*0.1086)*(3.*0.1086), true, true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_0.root",1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_1.root",1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_2.root",1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_3.root",1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_4.root", 1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_5.root", 1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_6.root", 1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_7.root", 1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_8.root", 1., true);
    InitSMS("T2bW", m_Path+"Summer16_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_9.root", 1., true);
 


    InitSMS("T2tt", m_Path+"Summer16_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    ////InitSMS("T2tt", m_Path+"Summer16_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    ////InitSMS("T2tt", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    InitSMS("T2tt", m_Path+"Summer16_102X_SMS/SMS-T2tt_dM-6to8_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    ////InitSMS("T2tt", m_Path+"Summer16_102X_SMS/SMS-T2tt_dM-10to80_2Lfilter_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", (3.*0.1086)*(3.*0.1086), true, true);
    ////InitSMS("T2tt", m_Path+"Summer16_102X_SMS/Summer16_102X_SMS/SMS-T2tt_3J-LH_xqcut-20_top-corridorRefMasses_2Lfilter_TuneCP5_13TeV-madgraphMLM-pythia8_Summer16_102X.root", (3.*0.1086)*(3.*0.1086), false, true);
    //
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_0.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_1.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_2.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_3.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_4.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_5.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_6.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_7.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_8.root",1.,true);
    InitSMS("tthighdM", m_Path+"Summer16_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X_9.root",1.,true);

    ////InitSMS("TChiWZ", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_ZToLL_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true, true);
    ////InitSMS("TChiWZ", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_ZToLL_dM-90to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true, true);
    ////InitSMS("TChiWZ", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., false, true);
    ////InitSMS("TChiWZ", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_mC1-325to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., false, true);
    InitSMS("TChiWZ", m_Path+"Summer16_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    InitSMS("TChiWZ", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_dM-60to90_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root",1.,true);
    InitSMS("HinoN2C1", m_Path+"Summer16_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 0.2352, true);
    InitSMS("HinoN2C1",  m_Path+"Summer16_102X_SMS/SMS-TChiWZ_dM-60to90_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root",0.2352,true);

    InitSMS("WZhighdM", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root",1.,true);
    InitSMS("HinohighdM", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root",0.2352,true);

    ////InitSMS("TChiWZ", m_Path+"Summer16_102X_SMS/SMS-TChiWZ_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    //
    ////InitSMS("TChipmWW", m_Path+"Summer16_102X_SMS/SMS-TChipmWW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    InitSMS("TChipmWW", m_Path+"Summer16_102X_SMS/SMS-TChipmWW_dM-3to50_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    InitSMS("TChipmWW", m_Path+"Summer16_102X_SMS/SMS-TChipmWW_dM-60to90_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root",1.,true);    

    InitSMS("WWhighdM", m_Path+"Summer16_102X_SMS/SMS-TChipmWW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root",1.,true);

////InitSMS("TChipmWW", m_Path+"Summer16_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true, true);

    ////InitSMS("TSlepSlep", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2.77, true);
    ////InitSMS("TSlepSlep", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_ext_Summer16_102X.root", 2.77, true);
    ////InitSMS("TSlepSlep", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_mSlep-500To1300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2.77, true);
    InitSMS("TSlepSleptot", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2.77, true);

    InitSMS("TSlepSlepmueL", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2, true);

    InitSMS("TSlepSlepmueR", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2*0.385, true);

    InitSMS("TSlepSlepmuLR", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1.385*2, true, false, kSel);

    InitSMS("TSlepSlepeLR", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1.385*2, true, false, kSmu);

    InitSMS("TSlepSlepMUL", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2., true, false, kSel);

    InitSMS("TSlepSlepMUR", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2*0.385, true, false, kSel);

    InitSMS("TSlepSlepEL", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2., true, false, kSmu);

    InitSMS("TSlepSlepER", m_Path+"Summer16_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 2*0.385, true, false, kSmu);

    ////InitSMS("T2bb", m_Path+"Summer16_102X_SMS/SMS-T2bb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);

    ////InitSMS("T2cc", m_Path+"Summer16_102X_SMS/SMS-T2cc_genHT-160_genMET-80_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    //
    ////InitSMS("T1bbbb", m_Path+"Summer16_102X_SMS/SMS-T1bbbb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);

  
    ////InitSMS("T1ttbb", m_Path+"Summer16_102X_SMS/SMS-T1ttbb_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    //
    ////InitSMS("T5tttt", m_Path+"Summer16_102X_SMS/SMS-T5tttt_dM175_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);
    //
    ////InitSMS("TChiWH", m_Path+"Summer16_102X_SMS/SMS-TChiWH_WToLNu_HToBB_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_Summer16_102X.root", 1., true);

    // ------------ //
    // --- Data --- //
    // ------------ //
    
    Process data_obs("data_obs", kData);
    list.clear();
      
    list += m_Path + "Summer16_102X_Data/MET_Run2016B-02Apr2020_ver2-v1_2016_Summer16_102X.root";
    list += m_Path + "Summer16_102X_Data/MET_Run2016C-02Apr2020-v1_2016_Summer16_102X.root";
    list += m_Path + "Summer16_102X_Data/MET_Run2016D-02Apr2020-v1_2016_Summer16_102X.root";
    list += m_Path + "Summer16_102X_Data/MET_Run2016E-02Apr2020-v1_2016_Summer16_102X.root";
    list += m_Path + "Summer16_102X_Data/MET_Run2016F-02Apr2020-v1_2016_Summer16_102X.root";
    list += m_Path + "Summer16_102X_Data/MET_Run2016G-02Apr2020-v1_2016_Summer16_102X.root";
    list += m_Path + "Summer16_102X_Data/MET_Run2016H-02Apr2020-v2_2016_Summer16_102X.root";



    m_Proc[m_iYear][data_obs] = pair<vector<string>,string>(list, "KUAnalysis");

  }
  //return because only testing with 2016 samples
 
  ///////////////////////////////////////////
  ////////// 2017 samples ///////////////////
  ///////////////////////////////////////////
  m_iYear = 1;
  {
    // ------------------ //
    // --- Background --- //
    // ------------------ //
 
    Process ttbar("ttbar", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
//    list += m_Path + "Summer20UL17_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Summer20UL17_102X.root";
    list += m_Path + "Fall17_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTWW_TuneCP5_13TeV-madgraph-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
 //using only ttjets dilept sample for alice low pt elec analysis
    m_Proc[m_iYear][ttbar] = pair<vector<string>,string>(list, "KUAnalysis");

    Process ST("ST", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root";
    //list += m_Path + "Fall17_102X/ST_s-channel_4f_hadronicDecays_TuneCP5_13TeV-amcatnlo-pythia8_Fall17_102X.root"; // use only with new ntuples
    list += m_Path + "Fall17_102X/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8_Fall17_102X.root";
     
    list += m_Path + "Fall17_102X/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
    //corrupted?
    //ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root
    //list += m_Path + "Fall17_102X/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
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
    list += m_Path + "Fall17_102X/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Fall17_102X.root";
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
    list += m_Path + "Fall17_102X/GluGluHToTauTau_M125_13TeV_powheg_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/GluGluHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/GluGluHToWWToLNuQQ_M125_NNPDF31_TuneCP5_PSweights_13TeV_powheg_JHUGen710_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/GluGluHToZZTo2L2Nu_M125_TuneCP5_13TeV_powheg2_JHUGenV735_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WminusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WplusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/ZH_HToBB_ZToNuNu_M125_13TeV_powheg_pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_Fall17_102X.root";
    m_Proc[m_iYear][DB] = pair<vector<string>,string>(list, "KUAnalysis");

/* unused block, we use PT binned

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

*/


    Process Wjets("Wjets", kBkg);
    list.clear();
    list += m_Path + "Fall17_102X/WJetsToLNu_Pt-100To250_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_Pt-250To400_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_Pt-400To600_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_Pt-50To100_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root";
    list += m_Path + "Fall17_102X/WJetsToLNu_Pt-600ToInf_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17_102X.root";
    m_Proc[m_iYear][Wjets] = pair<vector<string>,string>(list, "KUAnalysis");




    Process TB("TB", kBkg);
    list.clear();
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
  
    ///2017 debugging samples
    //"old" ntuples
/*    Process debug1("debug1", kBkg);
    list.clear();
    list += m_Path + "debug_std/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_all_std.root";
    list += m_Path + "debug_std/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_all_std.root";	
    m_Proc[m_iYear][debug1] = pair<vector<string>,string>(list, "KUAnalysis");

   //"new" ntuples
    Process debug2("debug2", kBkg);
    list.clear();
    list += m_Path + "debug_slm/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_all_slm.root";
    list += m_Path + "debug_slm/DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8_all_slm.root";
    m_Proc[m_iYear][debug2] = pair<vector<string>,string>(list, "KUAnalysis");
*/
    // -------------- //
    // --- Signal --- //
    // -------------- //

    //Special 4bd sample for alice
    //InitSMS("T4bd_TuneCP2",m_Path+"Fall17_102X_SMS/SMS-T2-4bd_genMET-80_mStop-500_mLSP-490_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root",1,false,false);
    //InitSMS("T4bd_TuneCP5",m_Path+"Fall17_102X_SMS/SMS-T2-4bd_genMET-80_mStop-500_mLSP-490_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root",1,false,false);
    //InitSMS("T4bd_UL",m_Path+"Summer20UL17_102X_SMS/SMS-T2-4bd_genMET-80_mStop-500_mLSP-490_TuneCP5_13TeV-madgraphMLM-pythia8_Summer20UL17_102X.root",1,false,false);

 

    //InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    //InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    ////InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_2Lfilter_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", (3.*0.1086)*(3.*0.1086), true, true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_0.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_1.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_2.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_3.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_4.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_5.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_6.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_7.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_8.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_9.root",1.,true);

    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_0.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_1.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_2.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_3.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_4.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_5.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_6.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_7.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_8.root",1.,true);
    InitSMS("T2bW", m_Path+"Fall17_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_9.root",1.,true);


    InitSMS("T2tt", m_Path+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    ////InitSMS("T2tt", m_Path+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    InitSMS("T2tt", m_Path+"Fall17_102X_SMS/SMS-T2tt_dM-6to8_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_0.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_1.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_2.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_3.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_4.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_5.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_6.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_7.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_8.root",1.,true);
    InitSMS("tthighdM", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_9.root",1.,true);
  
////InitSMS("T2tt", m_Path+"Fall17_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    ////InitSMS("T2tt", m_Path+"Fall17_102X_SMS/SMS-T2tt_dM-10to80_2Lfilter_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", (3.*0.1086)*(3.*0.1086), true, true);
    ////InitSMS("T2tt", m_Path+"Fall17_102X_SMS/Fall17_102X_SMS/SMS-T2tt_3J-LH_xqcut-20_top-corridorRefMasses_2Lfilter_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root", (3.*0.1086)*(3.*0.1086), false, true);
    //
    ////InitSMS("TChiWZ", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true, true);
    ////InitSMS("TChiWZ", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_dM-90to100_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true, true);
    ////InitSMS("TChiWZ", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., false, true);
    ////InitSMS("TChiWZ", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_mC1-325to1000_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., false, true);
   
    InitSMS("TChiWZ", m_Path+"Fall17_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    InitSMS("TChiWZ", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_dM-60to90_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root",1.,true);
    InitSMS("HinoN2C1", m_Path+"Fall17_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 0.2352, true);
    InitSMS("HinoN2C1", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_dM-60to90_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root",0.2352,true);

    InitSMS("WZhighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_1.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_2.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_3.root",1.,true);

    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_1.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_2.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X_3.root",0.2352,true);


    //
    //InitSMS("TChipmWW", m_Path+"Fall17_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    //InitSMS("TChipmWW", m_Path+"Fall17_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true, true);
    InitSMS("TChipmWW", m_Path+"Fall17_102X_SMS/SMS-TChipmWW_dM-3to50_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    InitSMS("TChipmWW", m_Path+"Fall17_102X_SMS/SMS-TChipmWW_dM-60to90_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);	
    
    InitSMS("WWhighdM", m_Path+"Fall17_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_1.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Fall17_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_2.root",1.,true);

    ////InitSMS("TSlepSlep", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2.77, true);
    ////InitSMS("TSlepSlep", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_ext_Fall17_102X.root", 2.77, true);
    ////InitSMS("TSlepSlep", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_mSlep-500To1300_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2.77, true);
    InitSMS("TSlepSleptot", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2.77, true);

    InitSMS("TSlepSlepmueL", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2., true);

    InitSMS("TSlepSlepmueR", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2*0.385, true);

    InitSMS("TSlepSlepmuLR", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1.385*2, true, false, kSel);

    InitSMS("TSlepSlepeLR", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1.385*2, true, false, kSmu);

    InitSMS("TSlepSlepMUL", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2., true, false, kSel);

    InitSMS("TSlepSlepMUR", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2*0.385, true, false, kSel);

    InitSMS("TSlepSlepEL", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2., true, false, kSmu);

    InitSMS("TSlepSlepER", m_Path+"Fall17_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 2*0.385, true, false, kSmu);

    //InitSMS("T2bb", m_Path+"Fall17_102X_SMS/SMS-T2bb_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);


    //InitSMS("T2cc", m_Path+"Fall17_102X_SMS/SMS-T2cc_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    //
    //InitSMS("T1bbbb", m_Path+"Fall17_102X_SMS/SMS-T1bbbb_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);

  
    //InitSMS("T1ttbb", m_Path+"Fall17_102X_SMS/SMS-T1ttbb_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    //
    //InitSMS("T5tttt", m_Path+"Fall17_102X_SMS/SMS-T5tttt_dM175_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);
    //
    ////InitSMS("TChiWH", m_Path+"Fall17_102X_SMS/SMS-TChiWH_WToLNu_HToBB_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17_102X.root", 1., true);

    // ------------ //
    // --- Data --- //
    // ------------ //
    
    Process data_obs("data_obs", kData);
    list.clear();
    //list += m_Path + "Fall17_102X_Data/MET_Run2017B-Nano25Oct2019-v1_2017_Fall17_102X.root";
    //list += m_Path + "Fall17_102X_Data/MET_Run2017C-Nano25Oct2019-v1_2017_Fall17_102X.root";
    //list += m_Path + "Fall17_102X_Data/MET_Run2017D-Nano25Oct2019-v1_2017_Fall17_102X.root";
    //list += m_Path + "Fall17_102X_Data/MET_Run2017E-Nano25Oct2019-v1_2017_Fall17_102X.root";
    //list += m_Path + "Fall17_102X_Data/MET_Run2017F-Nano25Oct2019-v1_2017_Fall17_102X.root";
    // list += m_Path + "DoubleEG_Run2017B-02Apr2020-v1_2017_Fall17_102X.root";    
    // list += m_Path + "DoubleEG_Run2017C-02Apr2020-v1_2017_Fall17_102X.root"; 
    // list += m_Path + "DoubleEG_Run2017D-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "DoubleEG_Run2017D-02Apr2020-v1_2017_Fall17_102X.root"; 
    // list += m_Path + "DoubleEG_Run2017D-02Apr2020-v1_2017_Fall17_102X.root"; 
    // list += m_Path + "DoubleEG_Run2017D-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "DoubleEG_Run2017E-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "DoubleEG_Run2017F-02Apr2020-v1_2017_Fall17_102X.root";  
      
    list += m_Path + "Fall17_102X_Data/MET_Run2017B-02Apr2020-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017C-02Apr2020-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017D-02Apr2020-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017E-02Apr2020-v1_2017_Fall17_102X.root";
    list += m_Path + "Fall17_102X_Data/MET_Run2017F-02Apr2020-v1_2017_Fall17_102X.root";

    // list += m_Path + "DoubleMuon_Run2017B-02Apr2020-v1_2017_Fall17_102X.root"; 
    // list += m_Path + "DoubleMuon_Run2017D-02Apr2020-v1_2017_Fall17_102X.root";  
    // list += m_Path + "DoubleMuon_Run2017E-02Apr2020-v1_2017_Fall17_102X.root"; 
    // list += m_Path + "DoubleMuon_Run2017F-02Apr2020-v1_2017_Fall17_102X.root";  

    // list += m_Path + "SingleElectron_Run2017B-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleElectron_Run2017C-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleElectron_Run2017D-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleElectron_Run2017E-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleElectron_Run2017F-02Apr2020-v1_2017_Fall17_102X.root";

    // list += m_Path + "JetHT_Run2017B-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "JetHT_Run2017C-02Apr2020-v1_2017_Fall17_102X.root";       
    // list += m_Path + "JetHT_Run2017D-02Apr2020-v1_2017_Fall17_102X.root";       
    // list += m_Path + "JetHT_Run2017E-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "JetHT_Run2017F-02Apr2020-v1_2017_Fall17_102X.root";  

    // list += m_Path + "SingleMuon_Run2017B-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleMuon_Run2017C-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleMuon_Run2017D-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleMuon_Run2017E-02Apr2020-v1_2017_Fall17_102X.root";
    // list += m_Path + "SingleMuon_Run2017F-02Apr2020-v1_2017_Fall17_102X.root";


    m_Proc[m_iYear][data_obs] = pair<vector<string>,string>(list, "KUAnalysis");

  }
  
  ///////////////////////////////////////////
  ////////// 2018 samples ///////////////////
  ///////////////////////////////////////////
  m_iYear = 2;
  { 
    // ------------------ //
    // --- Background --- //
    // ------------------ //
    
    Process ttbar("ttbar", kBkg);
    list.clear();
    list += m_Path + "Autumn18_102X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTWW_TuneCP5_13TeV-madgraph-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTTT_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8_Autumn18_102X.root";
    m_Proc[m_iYear][ttbar] = pair<vector<string>,string>(list, "KUAnalysis");

    Process ST("ST", kBkg);
    list.clear();
    list += m_Path + "Autumn18_102X/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-madgraph-pythia8_Autumn18_102X.root";
    //list += m_Path + "Autumn18_102X/ST_s-channel_4f_hadronicDecays_TuneCP5_13TeV-madgraph-pythia8_Autumn18_102X.root"; //added in new ntuples
    list += m_Path + "Autumn18_102X/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_Autumn18_102X.root";
    m_Proc[m_iYear][ST] = pair<vector<string>,string>(list, "KUAnalysis");
  
    Process ZDY("ZDY", kBkg);
    list.clear();
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-4to50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-4to50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-4to50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-4to50_HT-600toInf_TuneCP5_PSWeights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZJetsToNuNu_HT-100To200_13TeV-madgraph_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZJetsToNuNu_HT-200To400_13TeV-madgraph_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZJetsToNuNu_HT-400To600_13TeV-madgraph_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZJetsToNuNu_HT-600To800_13TeV-madgraph_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZJetsToNuNu_HT-800To1200_13TeV-madgraph_Autumn18_102X.root";
    m_Proc[m_iYear][ZDY] = pair<vector<string>,string>(list, "KUAnalysis");

    Process DB("DB", kBkg);
    list.clear();
    list += m_Path + "Autumn18_102X/WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZZTo4L_TuneCP5_13TeV_powheg_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/GluGluHToTauTau_M125_13TeV_powheg_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/GluGluHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/GluGluHToWWToLNuQQ_M125_13TeV_powheg2_JHUGenV714_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/GluGluHToZZTo2L2Nu_M125_TuneCP5_13TeV_powheg2_JHUGenV735_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WminusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WplusH_HToBB_WToLNu_M125_13TeV_powheg_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/ZH_HToBB_ZToNuNu_M125_13TeV_powheg_pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_Autumn18_102X.root";
    m_Proc[m_iYear][DB] = pair<vector<string>,string>(list, "KUAnalysis");


/* disabled, we use pt binned in 2018

    Process Wjets("Wjets", kBkg);
    list.clear();
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    m_Proc[m_iYear][Wjets] = pair<vector<string>,string>(list, "KUAnalysis");

*/

    Process Wjets("Wjets", kBkg);
    list.clear();
    //list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-100To250_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X.root";//this is now split
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-100To250_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_1.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-100To250_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_2.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-100To250_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_3.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-100To250_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_4.root";
    //list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-250To400_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-250To400_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_1.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-250To400_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_2.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-250To400_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_3.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-250To400_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X_4.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-400To600_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-50To100_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WJetsToLNu_Pt-600ToInf_TuneCP5_13TeV-amcatnloFXFX-pythia8_Autumn18_102X.root";
    m_Proc[m_iYear][Wjets] = pair<vector<string>,string>(list, "KUAnalysis");





    Process TB("TB", kBkg);
    list.clear();
    list += m_Path + "Autumn18_102X/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WZZ_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WZG_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WWZ_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/WWG_TuneCP5_13TeV-amcatnlo-pythia8_Autumn18_102X.root";
    m_Proc[m_iYear][TB] = pair<vector<string>,string>(list, "KUAnalysis");
  
    Process QCD("QCD", kBkg);
    list.clear();
    list += m_Path + "Autumn18_102X/QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/QCD_HT100to200_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/QCD_HT300to500_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/QCD_HT500to700_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X/QCD_HT700to1000_TuneCP5_13TeV-madgraphMLM-pythia8_Autumn18_102X.root";
    m_Proc[m_iYear][QCD] = pair<vector<string>,string>(list, "KUAnalysis");

    // -------------- //
    // --- Signal --- //
    // -------------- //
	
   //adding 2018 t2bw
   // InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
   // InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_0.root",1., true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_1.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_2.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_3.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_4.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_5.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_6.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_7.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_8.root",1., true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_9.root",1., true);

   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_0.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_1.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_2.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_3.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_4.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_5.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_6.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_7.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_8.root",1.,true);
   InitSMS("T2bW", m_Path+"Autumn18_102X_SMS/SMS-T2bW_X05_dM-90to170_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_9.root",1.,true);

    InitSMS("T2tt", m_Path+"Autumn18_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    //InitSMS("T2tt", m_Path+"Autumn18_102X_SMS/SMS-T2tt_dM-10to80_genHT-160_genMET-80_mWMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    ////InitSMS("T2tt", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    InitSMS("T2tt", m_Path+"Autumn18_102X_SMS/SMS-T2tt_dM-6to8_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);

    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_0.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_1.root",1.,true);   
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_2.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_3.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_4.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_5.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_6.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_7.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_8.root",1.,true);
    InitSMS("tthighdM", m_Path+"Autumn18_102X_SMS/SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_9.root",1.,true);
    ////InitSMS("TChiWZ", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true, true);
    ////InitSMS("TChiWZ", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_dM-90to100_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true, true);
    ////InitSMS("TChiWZ", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., false, true);
    ////InitSMS("TChiWZ", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_ZToLL_mZMin-0p1_mC1-325to1000_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., false, true);
    
    InitSMS("TChiWZ", m_Path+"Autumn18_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    InitSMS("TChiWZ", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_dM-60to90_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root",1.,true);
    InitSMS("HinoN2C1",m_Path+"Autumn18_102X_SMS/TChiWZ_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 0.2352, true);
    InitSMS("HinoN2C1", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_dM-60to90_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root",0.2352,true);

    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_0.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_1.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_2.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_3.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_4.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_5.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_6.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_7.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_8.root",1.,true);
    InitSMS("WZhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_9.root",1.,true);
 
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_0.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_1.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_2.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_3.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_4.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_5.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_6.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_7.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_8.root",0.2352,true);
    InitSMS("HinohighdM", m_Path+"Fall17_102X_SMS/SMS-TChiWZ_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_9.root",0.2352,true);


   /*
	//
    ////InitSMS("TChipmWW", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    */
    InitSMS("TChipmWW", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_dM-3to50_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    InitSMS("TChipmWW", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_dM-60to90_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root",1.,true);
    ////InitSMS("TChipmWW", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_WWTo2LNu_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true, true);

    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_0.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_1.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_2.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_3.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_4.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_5.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_6.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_7.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_8.root",1.,true);
    InitSMS("WWhighdM", m_Path+"Autumn18_102X_SMS/SMS-TChipmWW_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X_9.root",1.,true);

    ////InitSMS("TSlepSlep", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2.77, true);
    ////InitSMS("TSlepSlep", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_TuneCP2_13TeV-madgraphMLM-pythia8_ext_Autumn18_102X.root", 2.77, true);
    ////InitSMS("TSlepSlep", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_mSlep-500To1300_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2.77, true);
    InitSMS("TSlepSleptot", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2.77, true);

    InitSMS("TSlepSlepmueL", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2., true);

    InitSMS("TSlepSlepmueR", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2*0.385, true);

    InitSMS("TSlepSlepmuLR", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1.385*2, true, false, kSel);

    InitSMS("TSlepSlepeLR", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1.385*2, true, false, kSmu);

    InitSMS("TSlepSlepMUL", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2., true, false, kSel);

    InitSMS("TSlepSlepMUR", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2*0.385, true, false, kSel);

    InitSMS("TSlepSlepEL", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2., true, false, kSmu);

    InitSMS("TSlepSlepER", m_Path+"Autumn18_102X_SMS/SMS-TSlepSlep_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 2*0.385, true, false, kSmu);

    ////InitSMS("T2bb", m_Path+"Autumn18_102X_SMS/SMS-T2bb_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);

    ////InitSMS("T2cc", m_Path+"Autumn18_102X_SMS/SMS-T2cc_genHT-160_genMET-80_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    //
    ////InitSMS("T1bbbb", m_Path+"Autumn18_102X_SMS/SMS-T1bbbb_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
  
    ////InitSMS("T1ttbb", m_Path+"Autumn18_102X_SMS/SMS-T1ttbb_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    //
    ////InitSMS("T5tttt", m_Path+"Autumn18_102X_SMS/SMS-T5tttt_dM175_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    //
    ////InitSMS("TChiWH", m_Path+"Autumn18_102X_SMS/SMS-TChiWH_WToLNu_HToBB_TuneCP2_13TeV-madgraphMLM-pythia8_Autumn18_102X.root", 1., true);
    
    // ------------ //
    // --- Data --- //
    // ------------ //

    Process data_obs("data_obs", kData);
    list.clear();
      
    list += m_Path + "Autumn18_102X_Data/MET_Run2018A-02Apr2020-v1_2018_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X_Data/MET_Run2018B-02Apr2020-v1_2018_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X_Data/MET_Run2018C-02Apr2020-v1_2018_Autumn18_102X.root";
    list += m_Path + "Autumn18_102X_Data/MET_Run2018D-02Apr2020-v2_2018_Autumn18_102X.root";


    m_Proc[m_iYear][data_obs] = pair<vector<string>,string>(list, "KUAnalysis");

  }
}  

bool SampleTool::m_ProcInit = false;

std::map<Process, pair<vector<string>,string> > SampleTool::m_Proc[3];
std::map<Process, bool> SampleTool::m_SProcInit[3]; // checked combined normalizations already?
std::map<Process, std::map<string,bool> >   SampleTool::m_SProcFS[3]; // FastSim?
std::map<Process, std::map<string,bool> >   SampleTool::m_SProcDL[3]; // di-lepton filter (ZToLL or dilepton filter);
std::map<Process, std::map<string,SleptonFlavor> >   SampleTool::m_SProcSlepFlavor[3]; // filter sleptons
std::map<Process, std::map<string,double> > SampleTool::m_SProcW[3];  // some additional weight to apply

double SampleTool::m_Lumi[3];
double SampleTool::m_HEMLumi[2];
