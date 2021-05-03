#ifndef shapeVariation_HH
#define shapeVariation_HH

#include "FitReader.hh"
#include "CategoryTree.hh"
#include "FitBin.hh"
#include "Category.hh"
#include "Process.hh"
#include "Systematics.hh"

//this class should vary Mperp + RISR (separately) according to some formula (see lepton systematics class)
//go through, grab histograms for one group (ie one categoryList), vary Mperp OR RISR, save new histograms with systematic tag (created by bookkeepingClass, "_group#proc#RISRUP")
class shapeVariationTool{
	public:
//		shapeVariationTool();
		shapeVariationTool(CategoryTree ct, ProcessList procs, string file);
		virtual ~shapeVariationTool();
		void doVariations();
	protected:
		string m_file;
		FitReader* m_fr;
	
	private:
		void makeCatMaps();
		CategoryTree m_ct;
		map<Category,string> m_Cats;
		//vector<const CategoryTree*> m_CTs;
		CategoryList m_catList;
		int m_nProc;
		VS m_proc;
};

class shapeVariation {
	public:
		shapeVariation(CategoryList cats,  string f);
		virtual ~shapeVariation();
		void doVariationRISR(TH1D* hist,const FitBin& fb, bool isUp=true);
		void doVariationMperp(TH1D* hist, const FitBin& fb, bool isUp=true);
		void doVariationDown();
		void doVariations(Process pp, map<Category,string> labels);
	private:
		double maxVar = 50;
		CategoryList m_cats;
		string m_file;
};


#endif
