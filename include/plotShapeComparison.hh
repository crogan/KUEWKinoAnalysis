#include <vector>
#include "Category.hh"
#include "Process.hh"

class plotShapeComparison{
	public:
		plotShapeComparison(vector<TH1D*> hists, vector<int> colors, VS labels);		
		virtual ~plotShapeComparison();
		void getLHs();
		void plotRatioAndLH();
		void plotDist(std::vector<double> pvals, string name, string title);
		std::vector<TGraphAsymmErrors*> m_graphs;
		std::vector<TGraphAsymmErrors*> m_gLHs;
		VS m_LHlabels;

	private:
		TH1D* histTotal;
		vector<double> m_pvals;
		vector<std::vector<double>> m_LHs;
		std::vector<double> m_binLHs;
		vector<TH1D*> m_hists;
		vector<int> m_colors;
		VS m_labels;
		int m_nHists;

};
