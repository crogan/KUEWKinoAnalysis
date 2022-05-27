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

///////////////////////////////////////////
////////// FitConfiguration class
///////////////////////////////////////////

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
  void AddKinematicSys(ch::CombineHarvester& cb, ProcessList& processes);
  void AddSJetNormSys(string& label, VS& procs, ch::CombineHarvester& cb, ProcessList& processes);

  void AddShapeSysAsNorm(const Systematic& sys, ch::CombineHarvester& cb, FitReader& FIT);
  
};



#endif
