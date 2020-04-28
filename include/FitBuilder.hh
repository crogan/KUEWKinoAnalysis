#ifndef FitBuilder_HH
#define FitBuilder_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>

#include "Category.hh"

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
		const Category& cat, const string& process, bool is_signal = false);
  
  void WriteFit(const string& outputroot);
  
private:
  map<string,map<string,FitBin*> > m_Proc[2];
  map<string,Category*> m_Cat;

  

  mutable TFile* m_OutFile;
  mutable TTree* m_ProcTree;
  mutable string m_sProc;
  mutable bool m_ProcIsSig;
  mutable vector<string> m_ProcCat;
  void FillProc(const string& proc, bool is_signal = false);
  mutable TTree* m_CatTree;
  mutable string m_sCat;
  mutable string m_sBin;
  mutable vector<double> m_BinX;
  mutable vector<double> m_BinY;
  void FillCat(const Category& cat);
  
};

#endif
