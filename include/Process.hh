#ifndef Process_HH
#define Process_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>

#include "Category.hh"
#include "Systematics.hh"

///////////////////////////////////////////
////////// Process class
///////////////////////////////////////////

enum ProcessType { kBkg, kSig, kData };

class Process {
public:
  Process(const string& title, ProcessType type);
  Process(const Process& proc); // shallow copy

  const Process& operator = (const Process&); // shallow copy
  
  virtual ~Process();
  
  const string& Name() const;
  ProcessType Type() const;

  void AddEvent(double weight, double Mperp, double RISR,
		const Category& cat,
		const Systematic& sys = Systematic::Default());

  Process FakeProcess(const string& label) const;

  bool operator <  (const Process&) const;
  bool operator >  (const Process&) const;
  bool operator == (const Process&) const;

  SM GetSM() const;

private:
  string m_Title;
  ProcessType m_Type;

  map<string,map<string,FitBin*> > m_ProcBins; // [proc_sys][cat][bin]
  
  void Clear();

  friend class ProcessBranch;
};

///////////////////////////////////////////
////////// ProcessList class
///////////////////////////////////////////

class ProcessList {
public:
  ProcessList();
  ProcessList(const ProcessList&);

  virtual ~ProcessList();

  ProcessList& operator =  (const ProcessList&);
  ProcessList& operator += (const ProcessList&);
  ProcessList& operator += (const Process&);
  ProcessList  operator +  (const ProcessList&) const;

  ProcessList Filter(ProcessType type) const;
  ProcessList Filter(const string& label) const;
  ProcessList Remove(ProcessType type) const;
  ProcessList Remove(const string& label) const;
  ProcessList FilterOR(VS& labels) const;
  ProcessList FilterAND(VS& labels) const;
  ProcessList RemoveOR(VS& labels) const;
  ProcessList RemoveAND(VS& labels) const;

  int Find(const string& label) const;
 
  int GetN() const;
  Process operator [] (int i) const;

  VS  GetProcesses() const;
  VSM GetSignalMasses() const;

private:
  int m_N;
  vector<Process>   m_Proc;
  map<Process,bool> m_ProcMap;
  
};

///////////////////////////////////////////
////////// ProcessBranch class
///////////////////////////////////////////

class ProcessBranch {
public:
  ProcessBranch();

  virtual ~ProcessBranch();

  void InitFill(TTree* tree);
  void FillProcess(Process& proc, TFile& file);
  
  void InitGet(TTree* tree);
  Process GetProcess();

private:
  TTree* m_Tree;
  
  string m_Proc;
  string m_SubProc;
  int    m_ProcType;

  string*  m_ProcPtr;
  string*  m_SubProcPtr;
  TBranch* m_b_Proc;  
  TBranch* m_b_SubProc;  
  TBranch* m_b_ProcType;  
   
};

#endif
