#include <vector>
#include <iostream>

class shapeTemplate{

	public:
		shapeTemplate();
		virtual ~shapeTemplate();
		vector<pair<int,double>> sortBins(TH1D* hist);
		
		
		//replace low statistics bins recursively - with normalization constraint
		//do error tweaking to cover changes
		//check agreement with original histogram - LLR test
		//
		//return frankenstein histogram to file

	private:
		int m_nBins;
		double m_norm;
		TH1D* m_hist_OG;
		TH1D* m_hist_rec;
		TH1D* m_hist_cons;
		double m_minVal = 0.1;
		vector<pair<int,double>> m_binsSorted;
		vector<pair<int,double>> m_binsSorted_cons;
		string m_fileName;
		
		double m_unwtNorm;

		
		void replaceBins(int idx=0, double Prem=1., double Prem_cons=1.);
		double compareShapes();
		void setErrors();
		void replaceHistogram();








}


