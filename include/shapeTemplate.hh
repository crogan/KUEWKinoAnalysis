#include <vector>
#include <iostream>

class shapeTemplate{

	public:
		shapeTemplate(TH1D* hist,TH1D* hist_cons, string ofile);
		virtual ~shapeTemplate();
		vector<pair<int,double>> sortBins(TH1D* hist);
		
		

	private:
		int m_nBins;
		double m_norm;
		TH1D* m_hist_OG;
		TH1D* m_hist_rec;
		TH1D* m_hist_cons;
		double m_minVal = 10.;
		vector<pair<int,double>> m_binsSorted;
		vector<pair<int,double>> m_binsSorted_cons;
		string m_fileName;
		
		double m_unwtNorm;

		
		void replaceBins(int idx=0, double Prem=1., double Prem_cons=1.);
		double compareShapes();
		void setErrors();
		void replaceHistogram();


}


class shapeTemplateTool{
public:
	shapeTemplateTool();
	virtual ~shapeTemplateTool();

	void smoothHistograms(const VS& proc, const CategoryTree& CT,  const string& name);
	vector<TH1D*> getHistograms(const CategoryTree& CT);
	void createTemplates();
	CatgoryTree getCategoryTree();
	VS getProcess();


private:
	string m_file;

	const VS m_proc;
	const CategoryTree m_CT;
	vector<std::pair<TH1D*,string>> m_histsAndLabels;

	map<string,double> m_nameToNorm;
	map<string,TH1D*> m_nameToHist;
	map<string,const char*> m_nameToTitle;
}


