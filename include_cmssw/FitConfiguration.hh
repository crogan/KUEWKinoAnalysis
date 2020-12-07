#ifndef FitConfiguration_HH
#define FitConfiguration_HH

#include <iostream>
#include <vector>

#include "FitReader.hh"
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"

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
