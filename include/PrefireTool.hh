#ifndef PrefireTool_h
#define PrefireTool_h

#include <string>

#include <TH2.h>
#include <TFile.h>

// derived from: https://github.com/cms-nanoAOD/nanoAOD-tools/blob/master/python/postprocessing/modules/common/PrefireCorr.py
class PrefireTool {
  
public:

  PrefireTool(){
  }

  PrefireTool(int year, bool UseEMpT, std::string rootfile);

  std::string Get_rootfile();

  double GetPrefireProbability(bool jet, double eta, double pt, double maxpt, int updown);
  bool Get_UseEMpT();
  
  virtual ~PrefireTool();

private:

  TH2F m_jetmap;
  TH2F m_photonmap;
  bool m_UseEMpT;
  int m_year;

};

#endif

