//to compare two histograms
class shapeComparison{
	
	public:
		shapeComparison(TH1D* hist1, TH1D* hist2);
		shapeComparison();
		virtual ~shapeComparison();
		double getPvalue();



	private:
		TH1D* mHist1;
		TH1D* mHist2;

		double Nv;
		double Nu;
		int nBins;

		vector<double> lambdas;

		double nDof;

		double calcLikelihood();
		double chi2Distribution();
		double gammp(double a, double x);
		void gser(double *gamser, double a, double x, double *gln);
		void gcf(double *gammcf, double a, double x, double *gln);
		double gammln(double xx);
		




};