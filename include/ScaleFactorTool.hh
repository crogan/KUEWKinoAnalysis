#ifndef ScaleFactorTool_HH
#define ScaleFactorTool_HH

#include <iostream>
#include <vector>

#include "ReducedBase_slim.hh"

using std::string;

///////////////////////////////////////////
////////// ScaleFactorTool class
///////////////////////////////////////////

class ScaleFactorTool {
public:
  ScaleFactorTool() {}
    
  virtual ~ScaleFactorTool() {}

  double GetX20BRSF(const string& filename, const string& treename);

  double GetMETEff(double MET, int updown = 0);
  double GetMETSF(double MET, int updown = 0);

  bool DileptonEvent(ReducedBase* base);

};

#endif
