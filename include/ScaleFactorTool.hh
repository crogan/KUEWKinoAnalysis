#ifndef ScaleFactorTool_HH
#define ScaleFactorTool_HH

#include <iostream>
#include <vector>

#include "Particle.hh"
#include "BtagSFTool.hh"

using std::string;

class ReducedBase;

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

  void AddBtagFolder( const string& btagfold, const string& proc_rootfile, int year=1);
  //double GetBtagSFWeight(ReducedBase* base, int year,  bool fastsim, bool HForLF, int updown = 0, ParticleIDType tag = kMedium);
  double GetBtagSFWeight( std::vector<double>& PT_jet, int year, bool fastsim, bool HForLF, int updown,ParticleIDType tag = kMedium,  double v2_weight=1. );
  

private:
  BtagSFTool m_BtagSFTool;

};

#endif
