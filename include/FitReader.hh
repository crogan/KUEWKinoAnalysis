#ifndef FitReader_HH
#define FitReader_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <iostream>
#include <vector>

#include "Category.hh"
#include "Process.hh"

using std::map;
using std::string;
using std::pair;

///////////////////////////////////////////
////////// FitReader class
///////////////////////////////////////////

class FitReader {
public:
  FitReader(const string& inputfile);

  virtual ~FitReader();

  void PrintCategories(bool verbose = false);
  void PrintProcesses(bool verbose = false);

  bool IsFilled(const Category&   cat,
		const Process&    proc,
		const Systematic& sys = Systematic::Default()) const;

  const TH1D* GetHistogram(const Category&   cat,
			   const Process&    proc,
			   const Systematic& sys = Systematic::Default()) const;

  double Integral(const Category&   cat,
		  const Process&    proc,
		  const Systematic& sys = Systematic::Default()) const;

  TCanvas* Plot1Dstack(const vector<string>& proc,
		       const vector<string>& lep_cat,
		       const vector<string>& hadS_cat,
		       const vector<string>& hadI_cat,
		       const string& canvas);
  
  const ProcessList&    GetProcesses() const;
  const CategoryList&   GetCategories(const string& channel = "") const;
  vector<string>        GetChannels() const;           
  const Systematics&    GetSystematics() const;
  
private:
  mutable TFile m_File;

  mutable map<Process,Systematics> m_ProcSys;
  mutable map<Process,map<Category,TH1D*> > m_ProcHist;
  mutable map<Process,map<Systematic,map<Category,pair<TH1D*,TH1D*> > > >m_ProcHistSys;

  ProcessList                      m_Proc;
  mutable map<string,CategoryList> m_Chan;
  CategoryList                     m_Cat;
  Systematics                      m_Sys;
  
  ProcessBranch m_ProcBranch;
  void ReadProcesses();
  
  CategoryBranch m_CatBranch;
  void ReadCategories();

  map<string,string>          m_Title;
  map<string,int>             m_Color;
  vector<int>                 m_ColorDefault;
  map<string,vector<string> > m_Strings;
  vector<string>              m_Sig;
  map<string,string>          m_SignalTitle;
  void InitializeRecipes();
  string GetSignalTitle(const string& label);
  
};

#endif
