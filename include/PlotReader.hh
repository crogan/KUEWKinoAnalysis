#ifndef FitReader_HH
#define FitReader_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>

#include "Process.hh"

using std::map;
using std::string;
using std::pair;
using std::basic_string;
///////////////////////////////////////////
////////// PlotReader class
///////////////////////////////////////////

class PlotReader {
public:
  PlotReader(const string& inputfile);

  virtual ~PlotReader();

  void SetPlotTitle(const string& title){
    m_PlotTitle = title;
  }
  
  TCanvas* Plot1Dstack(const string& can_name,
		       const string& var,
		       const VS& procs,
		       const string& region,
		       bool norm = true, int rebin = 4, double xmin = -1., double xmax = -1.);
 
protected:
  mutable TFile  m_File;

  void Initialize();
  string m_PlotTitle;
  
  VS   m_Hists;
  void ReadHists();
 
  TH1D* GetHist(const string& name,
		const string& var,
		const string& proc,
		const VS& regions) const;

  TH1D* GetHist(const string& name,
		const string& var,
		const string& proc,
		const string& region) const;

  string GetSignalTitle(const string&);
 
  const VS& GetHists() const;

  string m_CMSLabel;
  map<string,VS> m_Strings;
  map<string,string> m_Title;
  map<string,string> m_Label;
  map<string,int> m_Color;
  
};

#endif
