#include <vector>
#include "Category.hh"
#include "Process.hh"

class plotShapeComparison{
	public:
		plotShapeComparison();
		virtual ~plotShapeComparison();
		TCanvas* formatGraphs(std::vector<TGraphAsymmErrors*> graphs, vector<int> colors, VS labels);
		void getLHs();
		void makePlots();



	private:
		vector<double> m_pvals;
		vector<std::vector<double>> m_LHs;
		std::vector<TGraphAsymmErrors*> m_graphs;
		vector<TGraph*> m_gLHs;
		vector<TH1D*> m_hists;
		int m_nHists;

};