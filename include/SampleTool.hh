#ifndef SampleTool_h
#define SampleTool_h

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <TChain.h>

#include "Process.hh"

using std::string;
using std::vector;
using std::pair;

class SampleTool {

public:
  SampleTool(const string& ntuple_path, int year);
  virtual ~SampleTool();

  SampleTool& SetNtuplePath(const string& ntuple_path);
  SampleTool& SetYear(int year);

  double Lumi();

  ProcessList Get(const string& name) const;
  ProcessList Get(ProcessType type) const;
  
  int NTrees(const Process& proc);
  TChain* Tree(const Process& proc, int itree = -1); // remember to delete TChain!
  string TreeName(const Process& proc, int itree);
  string FileName(const Process& proc, int itree);
  
  bool IsFastSim(const Process& proc, int itree);
  bool FilterDilepton(const Process& proc, int itree);
  double GetSampleWeight(const Process& proc, int itree);

  static const int m_total_years = 10;
  
private:
  string m_Path;
  int    m_iYear;

  int YearMap(int year);

  void InitSMS(const string& prefix, const string& filename, double weight = 1., bool FS = false, bool DL = false);
  void InitProcMap();
  static bool m_ProcInit;
  static std::map<Process, pair<vector<string>,string> > m_Proc[m_total_years];
  static double m_Lumi[m_total_years];

  // signal only
  void InitSignalProc(const Process& proc);
  static std::map<Process, bool> m_SProcInit[m_total_years]; // checked combined normalizations already?
  static std::map<Process, std::map<string,bool> >   m_SProcFS[m_total_years]; // FastSim?
  static std::map<Process, std::map<string,bool> >   m_SProcDL[m_total_years]; // di-lepton filter (ZToLL or dilepton filter);
  static std::map<Process, std::map<string,double> > m_SProcW[m_total_years];  // some additional weight to apply
  
  
};

#endif



