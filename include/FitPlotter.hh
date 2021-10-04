#ifndef FitPlotter_HH
#define FitPlotter_HH

#include <TCanvas.h>
#include <TGraphErrors.h>

#include "FitReader.hh"

using std::map;
using std::string;
using std::pair;

///////////////////////////////////////////
////////// FitPlotter class
///////////////////////////////////////////

class FitPlotter : public FitReader {
public:
  FitPlotter(const string& inputfile,
	     const string& otherfile = "",
	     const string& otherfold = "");

  virtual ~FitPlotter();

  TCanvas* Plot1DShape(const string& proc,
        const VS& lep_cat,
        const VS& hadS_cat,
        const VS& hadI_cat,
	const string& name);
  
  TCanvas* Plot1Dstack(const VS& proc,
		       const VS& lep_cat,
		       const VS& hadS_cat,
		       const VS& hadI_cat,
		       const string& canvas);

  TCanvas* Plot2D(const VS& proc,
		  const VS& lep_cat,
		  const VS& hadS_cat,
		  const VS& hadI_cat,
		  const string& canvas,
		  const string& extra = "");

  TCanvas* PlotYields(const string& can_name,
		      const VS& proc,
		      const CategoryTree& CT);

  TCanvas* Plot1Dstack(const string& can_name,
		       const VS& proc,
		       const CategoryTree& CT,
		       bool do_ratio = false);

  TCanvas* Plot2D(const string& can_name,
		  const VS& proc,
		  const CategoryTree& CT);

  TCanvas* Plot1DratioProc(const VS& proc,
           const string& lep_cat,
           const string& hadS_cat,
           const string& hadI_cat,
           const string& name,
           const string& extra);

  TCanvas* Plot1Dratio(const string& proc,
           const VS& lep_cat,
           const VS& hadS_cat,
           const VS& hadI_cat,
           const string& name,
           const VS& extra);

  TCanvas* Plot1DratioSyst(const VS& proc,
     const Systematic& syst,
           const VS& lep_cat,
           const VS& hadS_cat,
           const VS& hadI_cat,
           const string& name,
           const VS& extra);
  TCanvas* PlotRatioSystDist(const VS& proc,
     const Systematic& syst,
           const VS& lep_cat,
           const VS& matchString,
           const string& name);

  map<string,VS> m_Strings;
  
private:
  string m_CMSLabel;

  map<string,string>          m_Title;
  map<string,int>             m_Color;
  vector<int>                 m_ColorDefault;
  vector<int>                 m_SignalColor;
  
  void InitializeRecipes();
  string GetSignalTitle(const string& label);

  TGraphErrors* GetTotalBackground(const CategoryList& cat);

  void DrawCatTree(const CategoryTree& CT, TCanvas* can);
  void DrawMR(const FitBin& fitbin, TCanvas* can, TPad* pad, TPad* pad_ratio = nullptr);

  VS AddPrefix(const string& pre, const VS& post) const;
  
};

#endif
