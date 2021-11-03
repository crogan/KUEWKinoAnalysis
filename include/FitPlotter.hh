#ifndef FitPlotter_HH
#define FitPlotter_HH

#include <TCanvas.h>
#include <TGraphErrors.h>

#include "FitReader.hh"

using std::map;
using std::string;
using std::pair;

enum SignificanceType {kSB, kSrootB, kZbin};
///////////////////////////////////////////
////////// FitPlotter class
///////////////////////////////////////////

class FitPlotter : public FitReader {
public:
  FitPlotter(const string& inputfile,
	     const string& otherfile = "",
	     const string& otherfold = "");

  virtual ~FitPlotter();
  
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

  TCanvas* PlotCatSignificance(const string& can_name,
			       const VS& proc_bkg,
			       const VS& proc_sig,
			       const CategoryTree& CT,
			       SignificanceType sType = kSB);

  TCanvas* PlotRegionSignificance(const string& can_name,
				  const VS& proc_bkg,
				  const VS& proc_sig,
				  const CategoryTree& CT,
				  SignificanceType sType = kSB);

  void FindBkgZeros(const VS& proc_bkg);

  void FindBkgRare();

  void YieldPerBkg(const VS& proc_bkg);

  void zeroBkgTest();

  TCanvas* Plot1Dstack(const string& can_name,
		       const VS& proc,
		       const CategoryTree& CT);

  TCanvas* Plot2D(const string& can_name,
		  const VS& proc,
		  const CategoryTree& CT);

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
  void DrawMR(const FitBin& fitbin, TCanvas* can);


  double calculateZbi(double Nsig, double Nbkg, double deltaNbkg = 0.2);

  VS AddPrefix(const string& pre, const VS& post) const;

  
};

#endif
