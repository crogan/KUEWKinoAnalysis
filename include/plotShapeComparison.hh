#include <vector>
#include "TGraphAsymmErrors.h"

#include "Category.hh"
#include "Process.hh"

class plotShapeComparison{
	public:
		plotShapeComparison(vector<TH1D*> hists, vector<int> colors, VS labels);		
		virtual ~plotShapeComparison();
		void getLHs();
		void getRatioPlots();
		void getLHplots();
		void plotDist(std::vector<double> pvals, string name, string title);
		std::vector<TGraphAsymmErrors*> m_graphs;
		std::vector<TGraphAsymmErrors*> m_gLHs;
		VS m_LHlabels;
		vector<double> m_pvals;

	private:
		TH1D* m_histTotal;
		vector<std::vector<double>> m_LHs;
		std::vector<double> m_binLHs;
		vector<TH1D*> m_hists;
		vector<int> m_colors;
		VS m_labels;
		int m_nHists;
		double m_x = 0.;
		map<string,double> m_histToFudgeFactor;
		
};
