#ifndef shapeTemplate_HH
#define shapeTemplate_HH


#include <vector>
#include <iostream>

#include "Category.hh"
#include <TH1D.h>
#include "Criteria.hh"
#include "CategoryTree.hh"

using std::map;
using std::string;
using std::pair;
using std::basic_string;
using std::vector;
class shapeTemplate{

	public:
		shapeTemplate(TH1D* hist,TH1D* hist_cons, Category cat,string ofile);
		virtual ~shapeTemplate();
		vector<pair<int,double>> sortBins(vector<pair<int,double>>& array);
		void replaceHistogram();
		double compareShapes();
		
		

	private:
		int m_nBins;
		double m_norm;
		TH1D* m_hist_OG;
		TH1D* m_hist_cons;
		TH1D* m_hist_rec;
		TH1D* m_hist_scaled;
		double m_minVal = 10.;
		vector<pair<int,double>> m_binsSorted;
		vector<pair<int,double>> m_binsSorted_cons;
		string m_fileName;
		string m_dirName;
		//Category m_cat;
		
		double m_unwtNorm;

		
		void replaceBins(int idx=0, double Prem=1., double Prem_cons=1.);
		void setErrors();


};


class shapeTemplateTool{
public:
	shapeTemplateTool(const string &inputfile, const CategoryTree& CT, const VS& proc);
	virtual ~shapeTemplateTool();

	void smoothHistograms();
	void getHistograms();
	void createTemplates();
	CategoryTree getCategoryTree();
	VS getProcess();


private:
	string m_file;

	VS m_proc;
	CategoryTree m_CT;
	vector<std::pair<TH1D*,string>> m_histsAndLabels;

	map<string,double> m_nameToNorm;
	map<string,TH1D*> m_nameToHist;
	map<string,const char*> m_nameToTitle;
};

#endif


