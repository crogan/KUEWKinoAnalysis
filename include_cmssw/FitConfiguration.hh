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

using namespace ch::CombineHarvester;

///////////////////////////////////////////
////////// FitConfiguration class
///////////////////////////////////////////

class FitConfiguration {
public:
  FitConfiguration() {}
  virtual ~FitConfiguration() {}

  void Configure(CombineHarvester& cb, ProcessList& processes);
   
};

#endif
