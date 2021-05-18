#ifndef shapeVariation_HH
#define shapeVariation_HH

#include "FitReader.hh"
#include "CategoryTree.hh"
#include "FitBin.hh"
#include "Category.hh"
#include "Process.hh"
#include "Systematics.hh"

class shapeVariationTool{
	public:
		shapeVariationTool(CategoryTree ct, ProcessList procs, TFile* file);
		//shapeVariationTool(CategoryTree ct, ProcessList procs, string file);
		virtual ~shapeVariationTool();
		void doVariations();
	protected:
		TFile* m_file;
		//string m_file;
		FitReader* m_fr;
	
	private:
		void makeCatMaps();
		void makeProcessMaps();
		CategoryTree m_ct;
		map<Category,string> m_Cats;
		map<Process,string> m_domToRareLabels;
		CategoryList m_catList;
		int m_nProc;
		VS m_proc;
		ProcessList m_procList;
};

class shapeVariation {
	public:
		shapeVariation(CategoryList cats, TFile* f);
		//shapeVariation(CategoryList cats);
		virtual ~shapeVariation();
		void doVariationRISR(TH1D* hist,const FitBin& fb, bool isUp=true);
		void doVariationMperp(TH1D* hist, const FitBin& fb, bool isUp=true);
		void doVariationDown();
		//void doVariations(Process pp, string procLabel, map<Category,string> catLabels, string file);
		void doVariations(Process pp, string procLabel, map<Category,string> catLabels);
	private:
		double maxVar = 50;
		CategoryList m_cats;
		TFile* m_file;
};


#endif
