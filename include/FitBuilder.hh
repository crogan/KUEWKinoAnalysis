#ifndef FitBuilder_HH
#define FitBuilder_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>

#include "Process.hh"

using std::map;
using std::string;


///////////////////////////////////////////
////////// FitBuilder class
///////////////////////////////////////////

class FitBuilder {
public:
  FitBuilder();

  virtual ~FitBuilder(); 
  
  void AddEvent(double weight, double Mperp, double RISR,
		const Category& cat,
		const Process& proc,
		const Systematic& sys = Systematic::Default());

  const Process& FakeProcess(const string& label);
  
  void WriteFit(const string& outputroot);
  
private:
  map<string,Process*>  m_Proc;
  map<string,Category*> m_Cat;  
  
  TFile* m_OutFile;

  TTree* m_ProcTree;
  ProcessBranch m_ProcBranch;
  void WriteProc();
  
  TTree* m_CatTree;
  CategoryBranch m_CatBranch;
  void WriteCat();
  
};

#endif
