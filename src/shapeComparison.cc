//#include <iostream>
#include <random>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>
#include <TLatex.h>
#include "shapeComparison.hh"


shapeComparison::shapeComparison(){
	mHist1 = NULL;
	mHist2 = NULL;
	m_weightedScale = false;
}


shapeComparison::shapeComparison(TH1D* hist1, TH1D* hist2, bool on){
	if(hist1->GetNbinsX() != hist2->GetNbinsX()){
		std::cout << "Error: binnings not the same." << std::endl;
		return;
	}

	mHist1 = (TH1D*)hist1->Clone();
	mHist2 = (TH1D*)hist2->Clone();

	calcWeightsAndScale(mHist1);
	calcWeightsAndScale(mHist2);

	Nu = mHist1->Integral();
	Nv = mHist2->Integral();
	nBins = mHist1->GetNbinsX();

	nDof = mHist1->GetNbinsX() - 1; //mHist1 and mHist2 have to have the same binning

	//weightedScale(); //scale histograms to account for weighting so poisson distribution uses correct rate and therefore has the correct error (sqrt(N) where N = (bin/binErr)^2)
	m_weightedScale = on;
	
}


shapeComparison::~shapeComparison(){
	delete mHist1;
	delete mHist2;
	lambdas.clear();
}


void shapeComparison::calcWeightsAndScale(TH1D* hist){
	double weight = 0;
	for(int i = 0; i < hist->GetNbinsX()+1; i++){
		std::cout << "bin #" << i << std::endl;
		double tmp_b = hist->GetBinContent(i)/hist->GetBinError(i);
		double tmp_w = hist->GetBinError(i)/tmp_b;
		std::cout << "tmp_w: " << tmp_w << " weight from formula: " << pow(hist->GetBinError(i),2)/hist->GetBinContent(i) << std::endl;
		if(tmp_w > weight) weight = tmp_w;
	}
	hist->Scale(1/weight);
}

double shapeComparison::calcLikelihoodScaled(){ //calculates negative log likelihood ratio
//	unweightHistograms();	
//	Nu = mHist1->Integral(); Nv = mHist2->Integral();
	double u;
	double v;
	double t;
	lambda = 0;
	double tmp_lambda;
	double norms = Nv/Nu;
	std::cout << "Nu: " << Nu << " Nv: " << Nv << std::endl;
	for(int i = 0; i < nBins+1; i++){
		std::cout << "bin #" << i << std::endl;
		u = mHist1->GetBinContent(i);
		v = mHist2->GetBinContent(i);
		t = u + v;
		std::cout << "unweighted u: " << u  << " unweighted v: " << v  << " t: " << t << std::endl;
		if(u == 0 && v == 0) { lambdas.push_back(0.); continue;}
		else if(u == 0 && v != 0) tmp_lambda = -2*t*log(Nu/(Nu+Nv));
		else if(u != 0 && v == 0) tmp_lambda = -2*t*log(Nv/(Nu+Nv));
		else tmp_lambda = -2*(t*log( (1 + v/u)/(1 + Nv/Nu) ) + v*log( (Nv/Nu)*(u/v) ));
		lambda += tmp_lambda; 
		lambdas.push_back(tmp_lambda);
		std::cout << "unweighted LH for bin " << i << ": " << tmp_lambda << " total LH so far: " << lambda << std::endl;
	}
	return lambda;
}


void shapeComparison::unweightHistograms(){
	for(int i = 0; i < nBins+1; i++){
		mHist1->SetBinContent(i,mHist1->GetBinContent(i)/mHist1->GetBinError(i));
		mHist2->SetBinContent(i,mHist2->GetBinContent(i)/mHist2->GetBinError(i));
	}
}



double shapeComparison::calcLikelihood(){ //calculates negative log likelihood ratio
	double u; 
	double v; 
	double t; 
	lambda = 0;
	double tmp_lambda;
	double norms = Nv/Nu;
	std::cout << "Nu: " << Nu << " Nv: " << Nv << std::endl;
	for(int i = 0; i < nBins+1; i++){
		std::cout << "bin #" << i << std::endl;
		u = mHist1->GetBinContent(i);
		v = mHist2->GetBinContent(i);
		t = u + v;  
		std::cout << "weighted u: " <<  u  << " weighted v: " << v << std::endl;
		if(u == 0 && v == 0) { lambdas.push_back(0.); continue;}
		else if(u == 0 && v != 0){ tmp_lambda = -2*t*log(Nu/(Nu+Nv)); }
		else if(u != 0 && v == 0){ tmp_lambda = -2*t*log(Nv/(Nu+Nv)); }
		else tmp_lambda = -2*(t*log( (1 + v/u)/(1 + Nv/Nu) ) + v*log( (Nv/Nu)*(u/v) ));
		lambda += tmp_lambda; 
		lambdas.push_back(tmp_lambda);
		std::cout << "weighted LH for bin " << i << ": " << tmp_lambda << " total weighted LH so far: " << lambda <<  std::endl;
	}
	return lambda;
}

double shapeComparison::getPvalue(){
	double LH = calcLikelihood();
	double LH_scaled = calcLikelihoodScaled();
	double cdf = 1 - gammp(nDof/2.0,LH/2.0); //value of cumulative distribution function
	double cdf_scaled = 1 - gammp(nDof/2.0,LH_scaled/2.0);	
std::cout << " weighted pvalue: " << 1 - cdf << " weighted likelihood: " << LH << " unweighted p-value: " << 1 - cdf_scaled << " unweighted likelihood: " << LH_scaled <<  std::endl;
	return 1 - cdf_scaled; //pvalue is inverse of cdf value
}
double shapeComparison::chi2Distribution(){
	std::default_random_engine generator;
	std::chi_squared_distribution<double> distribution(nDof);
	int n = 1000000;
	double chi2[n];

	for(int i = 0; i < n; i++){
		chi2[i] = distribution(generator);
	}
	return *chi2;	
}


std::vector<double> shapeComparison::getBinPvalues(){
	double LH = calcLikelihood();
	std::vector<double> pvals;
	for(int i = 0; i < nBins; i++){
		double cdf = 1 - gammp(nDof/2.0,lambdas[i]/2.0); //value of cumulative distribution function
		pvals.push_back(1 - cdf);//pvalue is inverse of cdf value
//		std::cout << "bin #: " << i << " likelihood: " << lambdas[i] << " pval: " << pvals[i] << std::endl; 
	}
	return pvals; 
}

double shapeComparison::gammp(double a, double x){
	double gamser;
	double gammcf;
	double gln;
	if(x < 0.0 || a <= 0.0){
	 std::cout << "Invalid arguments in gammp." << std::endl;
	 return 0.;
	}
	if(x < (a+1.0)){ //use series representation
		gser(&gamser,a,x,&gln);
		return gamser;
	}
	else{	//use continued fraction representation (optimize run time)
		gcf(&gammcf,a,x,&gln);
		return 1.0 - gammcf;	//take complement
	}
}

void shapeComparison::gser(double *gamser, double a, double x, double *gln){
	double sum, del, ap;
	int ITMAX = 100;
	double EPS = 3.0e-7;
	*gln = gammln(a);

	if(x <= 0.0){
		if(x < 0.0) std::cout << "Error: x less than 0 in gser." << std::endl;
		*gamser=0.0;
		return;
	}
	else{
		ap = a;
		del = sum = 1.0/a;
		for(int n = 1; n < ITMAX; n++){
			++ap;
			del *= x/ap;
			sum += del;
			if(fabs(del) < fabs(sum)*EPS){
				*gamser = sum*exp(-x + a*log(x)-(*gln));
				return;
			}
		}
		std::cout << "Error: a too large or ITMAX too small in gser." << std::endl;
		return;
	}
}

void shapeComparison::gcf(double *gammcf, double a, double x, double *gln){
	float an, b, c, d, del, h;
	double FPMIN = 1.0e-14;
	int ITMAX = 100;
	double EPS = 3.0e-7;
	int i;
	*gln = gammln(a);
	b = x+1.0-a;
	c = 1.0/FPMIN;
	d = 1.0/b;
	h = d;
	for(i = 1; i <= ITMAX; i++){
		an = -i*(i-a);
		b += 2.0;
		d = an*d+b;
		if(fabs(d) < FPMIN) d = FPMIN;
		c = b+an/c;
		if(fabs(c) < FPMIN) c = FPMIN;
		d = 1.0/d;
		del = d*c;
		h *= del;
		if(fabs(del-1.0) < EPS)  break;
	}

	if(i > ITMAX) std::cout << "Error: a too large or ITMAX too small in gcf." << std::endl;
	*gammcf = exp(-x+a*log(x) - (*gln))*h;

}

double shapeComparison::gammln(double xx){ //returns ln(gamma(xx) for xx > 0)
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	y = x = xx;
	tmp = x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser = 1.000000000190015;
	for(int j = 0; j < 5; j++) ser += cof[j]/++y;
	return -tmp+log(2.50666282746310005*ser/x);

}


