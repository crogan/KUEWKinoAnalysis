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
  

private:
  string m_Path;
  int    m_iYear;

  int YearMap(int year);

  void InitSMS(const string& prefix, const string& filename);
  void InitProcMap();
  static bool m_ProcInit;
  static std::map<Process, pair<vector<string>,string> > m_Proc[3];
  static double m_Lumi[3];
  
};

#endif



