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

  void PrintCategories();
  void PrintProcesses();

  TCanvas* Plot1Dstack(const vector<string>& proc,
		       const vector<string>& lep_cat,
		       const vector<string>& hadS_cat,
		       const vector<string>& hadI_cat,
		       const string& name);
  
private:
  TFile m_File;

  int m_Nproc;
  int m_Ncat;
  
  map<string,map<string,int> > m_Proc;
  map<string,pair<vector<double>,vector<double> > > m_Cat;

  bool Match(const string& target, const vector<string>& test);

  map<string,string>          m_Title;
  map<string,int>             m_Color;
  vector<int>                 m_ColorDefault;
  map<string,vector<string> > m_Strings;
  vector<string>              m_Sig;
  map<string,string>          m_SignalTitle;
  void InitializeRecipes();
  
};

#endif
