//to compare two histograms
#include <iostream>
#include <vector>
#include "TH1D.h"
class shapeComparison{
	
	public:
		shapeComparison(TH1D* hist1, TH1D* hist2);
		shapeComparison();
		virtual ~shapeComparison();
		double getPvalue(std::vector<double> &lambdas);
		double getPvalue();
		double getSigma();
		double getSigma(std::vector<double> &lambdas);
		//std::vector<double> getBinPvalues();
		//double lambda;


	private:
		TH1D* mHist1;
		TH1D* mHist2;

		void calcWeightsAndScale(TH1D* hist);
		bool m_weightedScale;
		double Nv;
		double Nu;
		int nBins;

		double nDof;

		double calcLikelihood(std::vector<double> &lambdas);
		double calcLikelihood();
		double chi2Distribution();
		double gammp(double a, double x);
		void gser(double *gamser, double a, double x, double *gln);
		void gcf(double *gammcf, double a, double x, double *gln);
		double gammln(double xx);
		void weightedScale();
		




};
