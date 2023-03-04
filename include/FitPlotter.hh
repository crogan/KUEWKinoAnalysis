#ifndef FitPlotter_HH
#define FitPlotter_HH

#include <TCanvas.h>
#include <TGraphErrors.h>

#include "FitReader.hh"
#include "SuperBin.hh"

using std::map;
using std::string;
using std::pair;

enum SignificanceType {kSB, kSrootB, kZbin};
enum PlotType{kFull, kRISR, kInv};
enum CombineType{kMR,kVis};
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
			const string& name,
			const VS& extra);

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
				  PlotType pType = kFull,
				  SignificanceType sType = kZbin);

  void CombineBins(const string& can_name,
		   const VS& proc_bkg,
		   const VS& proc_sig,
		   const CategoryTree& CT,
		   CombineType cType = kVis,
		   PlotType pType = kFull);

  SuperBinList CombineBinsSigAvg(const string& can_name,
				 const VS& proc_bkg,
				 const VS& proc_sig,
				 const CategoryTree& CT,
				 const double SF = 3,
				 const double sys = 0.1,
				 CombineType cType = kVis,
				 PlotType pType = kFull);
  
  void FindBkgZeros(const VS& proc_bkg);
  void FindBkgRare();
  void YieldPerBkg(const VS& proc_bkg);
  void zeroBkgTest();

  TCanvas* Plot1Dstack(const string& can_name,
		       const VS& proc,
		       const CategoryTree& CT,
		       PlotType pType = kFull,
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

  //VS GetSignalProcs(int min_mass_diff, int max_mass_diff, int exclude_below)
  map<string,VS> m_Strings;

private:
  string m_CMSLabel;

  map<string,string>          m_Title;
  map<string,int>             m_Color;
  vector<int>                 m_ColorDefault;
  vector<int>                 m_SignalColor;
  
  void InitializeRecipes();
  string GetSignalTitle(const string& label);

  TGraphErrors *GetTotalBackground(const CategoryList& cat);

  void DrawCatTree(const CategoryTree& CT, TCanvas* can);
  void DrawMR(const FitBin& fitbin, TCanvas* can, TPad* pad, TPad* pad_ratio = nullptr);
  void DrawRM(const FitBin& fitbin, TCanvas* can, TPad* pad, TPad* pad_ratio = nullptr);
  void DrawRM(const FitBin& fitbin, TCanvas* can, TPad* pad, VS labels, TPad* pad_ratio = nullptr);
  void DrawRM(vector<CategoryList> cats, TCanvas* can, TPad* pad, VS labels, TPad* pad_ratio = nullptr);
  double CalculateZbi(double Nsig, double Nbkg, double deltaNbkg = 0.2);

  ProcessList FetchProcs(VS proc_list);
  VS FetchLabels(VS categories);
  VS FetchCats(VS categories);

  vector<double> IntegrateMperp(const FitBin fitBin, const vector<double> histVec) const;
  TH1D* IntegrateMperp(const TString& name, const FitBin fitBin, TH1D* hist) const;

  VS AddPrefix(const string& pre, const VS& post) const;

  //std::tuple<int, int, std::string> SigMass(const std::string sig) const;
  //std::string SigType(const std::string sig) const;
  
};

#endif
