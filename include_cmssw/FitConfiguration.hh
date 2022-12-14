#ifndef FitConfiguration_HH
#define FitConfiguration_HH

#include <iostream>
#include <vector>

#include "FitReader.hh"
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"
using ch::syst::SystMapFunc;
///////////////////////////////////////////
////////// FitConfiguration class
///////////////////////////////////////////
typedef std::map< std::string , std::vector<std::pair<int,int>>> SystDict;
class FitConfiguration {
public:
  FitConfiguration() {}
  virtual ~FitConfiguration() {}

  void Configure(ch::CombineHarvester& cb, ProcessList& processes);

  void AddFloatingNorms(VS& float_procs, ch::CombineHarvester& cb, ProcessList& all_procs);
  void AddRareNorms(VS& rare_procs, double uncert, ch::CombineHarvester& cb, ProcessList& all_procs);

  void AddCommonSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddFakeLeptonSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddSVSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddBJetSys(ch::CombineHarvester& cb, ProcessList& processes);
  void Add0LBJetSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void Add1LBJetSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void AddCommonBJetSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddFakeSSSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddKinematicSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddPTISRSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void Add0LPTISRSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void Add1LPTISRSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void AddgamTSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void Add0LgamTSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void Add1LgamTSys(ch::CombineHarvester& cb, ProcessList& processes, string label);
  void AddCommongamTSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddSJetNormSys(const string& label, VS& procs, ch::CombineHarvester& cb, ProcessList& processes);
  void AddQCDNormSys(const string& label, VS& procs, ch::CombineHarvester& cb, ProcessList& processes);
  void AddLeptonQualityNormSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddSJetLeptonQualityNormSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddLeptonCategoryNormSys(ch::CombineHarvester& cb, ProcessList& processes);

  void AddShapeSysAsNorm(const Systematic& sys, ch::CombineHarvester& cb, FitReader& FIT);

  void AddLNormSys(const string& label, VS& procs, ch::CombineHarvester& cb, ProcessList& processes, std::vector<std::string> Nlep, double errorInit);
  void AddNormHierarchy( SystDict& sm, VS& proc, ch::CombineHarvester& cb, ProcessList& processes);
  
  void appSystDict( SystDict& sm , std::string label, std::vector<int> list);
  void initSystDictW( SystDict& sm );
  void initSystDictW0L( SystDict& sd);
  void initSystDictTtbar( SystDict& sm);
};

#endif
