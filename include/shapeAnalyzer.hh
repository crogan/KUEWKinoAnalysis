#ifndef shapeAnalyzer_HH
#define shapeAnalyzer_HH


#include <TStyle.h>
#include <TH1D.h>
#include <TString.h>
#include <vector>
#include <TList.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <iostream>
#include <TKey.h>
#include <TString.h>
class shapeAnalyzer{
	public:

	explicit shapeAnalyzer(TString ifile, TString ofile); //for histograms within one file
	// shapeAnalyzer(TString fNom, TString fUp, TString fDown); //for histograms across files (looking at ie pT variations)

	virtual ~shapeAnalyzer();

//	void formatPlots(std::vector<TH1D*> hists);
	void drawHists(TDirectory* oldDir, int iHist, int iProc,std::string histName,std::string sys);
//	void getHist(Int_t iHist, Int_t iProc,TString histName);
	// void getKeys(Int_t iKey);

	// void Analyze();

	//think about a generic "init" function
	//think about initializing process names and histogram names (Fakes_elf0, etc.) separately
	//
	private:
		TFile* iFile;
		// TFile* oFile;
		// std::vector<TH1D*> hists;
		// std::vector<TString> fakesVec;
		// std::vector<TString> procVec;
		// TList* keyList;
		// Int_t nKeys;
		// TDirectory* oldDir;
		// TDirectory* newDir;
		// TH1D* nomHist;
		// TH1D* upHist;
		// TH1D* downHist;
};



#endif
