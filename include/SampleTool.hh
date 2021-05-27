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
  
private:
  string m_Path;
  int    m_iYear;

  int YearMap(int year);

  void InitSMS(const string& prefix, const string& filename, double weight = 1., bool FS = false, bool DL = false);
  void InitProcMap();
  static bool m_ProcInit;
  static std::map<Process, pair<vector<string>,string> > m_Proc[3];
  static double m_Lumi[3];

  // signal only
  void InitSignalProc(const Process& proc);
  static std::map<Process, bool> m_SProcInit[3]; // checked combined normalizations already?
  static std::map<Process, std::map<string,bool> >   m_SProcFS[3]; // FastSim?
  static std::map<Process, std::map<string,bool> >   m_SProcDL[3]; // di-lepton filter (ZToLL or dilepton filter);
  static std::map<Process, std::map<string,double> > m_SProcW[3];  // some additional weight to apply
  
  
};

#endif



