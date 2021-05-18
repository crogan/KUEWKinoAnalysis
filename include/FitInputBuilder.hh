#ifndef FitInputBuilder_HH
#define FitInputBuilder_HH

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
////////// FitInputBuilder class
///////////////////////////////////////////

class FitInputBuilder {
public:
  FitInputBuilder(bool extrahist = false);

  virtual ~FitInputBuilder(); 
  
  double AddEvent(double weight, double Mperp, double RISR,
		const Category& cat,
		const Process& proc,
		const Systematic& sys = Systematic::Default());

  const Process& FakeProcess(const string& label);
 
  void SetFakeCategories(vector<CategoryTree> CTs);
  void SetQCDCategories(vector<CategoryTree> CTs);
 
  void WriteFit(const string& outputroot);
  void AddFakeShapeSystematics(Process proc, Systematics systs); 
private:
  map<string,Process*>  m_Proc;
  map<string,Category*> m_Cat;  

  vector<CategoryTree> m_CT_Fakes;
  vector<CategoryTree> m_CT_QCD;
 
  TFile* m_OutFile;

  TTree* m_ProcTree;
  ProcessBranch m_ProcBranch;
  void WriteProc();

  ProcessList MakeFakeProcesses(Process proc); 
  void WriteFakeShapeSysts(TFile* OutFile);  
  void WriteQCDShapeSysts(TFile* OutFile);  
  
  TTree* m_CatTree;
  CategoryBranch m_CatBranch;
  void WriteCat();

  bool m_BookHist;
  
};

#endif
