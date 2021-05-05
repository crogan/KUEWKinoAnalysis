#include <iostream>
#include <stdlib.h>
#include <vector>
#include <random>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>
#include <TLatex.h>
#include "shapeComparison.hh"

using std::cout; using std::endl;

shapeComparison::shapeComparison(){
	mHist1 = NULL;
	mHist2 = NULL;
	m_weightedScale = false;
}


shapeComparison::shapeComparison(TH1D* hist1, TH1D* hist2){
	if(hist1->GetNbinsX() != hist2->GetNbinsX()){
		std::cout << "Error: binnings not the same." << std::endl;
		return;
	}
	if(hist1->Integral() > hist2->Integral()){ 
		mHist1 = (TH1D*)hist2->Clone();
		mHist2 = (TH1D*)hist1->Clone();
	}
	else{ //if(hist1->Integral() < hist2->Integral()){ 
		mHist1 = (TH1D*)hist1->Clone();
		mHist2 = (TH1D*)hist2->Clone();
	}
	
	calcWeightsAndScale(mHist1);
	calcWeightsAndScale(mHist2);

	nBins = mHist1->GetNbinsX();
	//Nu > Nv s.t. Nv/Nu < 1
	Nu = mHist1->Integral(1,nBins);
	Nv = mHist2->Integral(1,nBins);
	

	nDof = nBins - 1; //mHist1 and mHist2 have to have the same binning

	
}


shapeComparison::~shapeComparison(){
	delete mHist1;
	delete mHist2;
}


void shapeComparison::calcWeightsAndScale(TH1D* hist){
	double weight = 0;
	if(hist == NULL) cout << "hist null" << endl;
	for(int i = 0; i < hist->GetNbinsX(); i++){
		double tmp_w = pow(hist->GetBinError(i+1),2)/hist->GetBinContent(i+1);
		if(tmp_w > weight) weight = tmp_w;
	}
	hist->Scale(1/weight);
}


double shapeComparison::calcLikelihood() { //calculates negative log likelihood ratio
	double u; 
	double v; 
	double t; 
	double lambda = 0;
	double tmp_lambda;
//std::cout << "x: " << x << std::endl;
	//double norms = Nv/Nu;
//std::cout << "nBins " << nBins << std::endl;
	for(int i = 1; i < nBins+1; i++){ //i starting at i discards bin #0 underflow bin (should be empty anyways)
		u = mHist1->GetBinContent(i);
		v = mHist2->GetBinContent(i);
		t = u + v;  
		if(u == 0 && v == 0) { tmp_lambda = 0.; }
		else if(u == 0 && v != 0){ tmp_lambda = -2*t*log(Nv/(Nu+Nv)); }
		else if(u != 0 && v == 0){ tmp_lambda = -2*t*log(Nu/(Nu+Nv)); }
		else tmp_lambda = -2*(t*log( (1 + v/u)/(1 + Nv/Nu) ) + v*log( (Nv/Nu)*(u/v) ));
		lambda += tmp_lambda; 
	}
	return lambda;
}


double shapeComparison::calcLikelihood(std::vector<double> &lambdas) { //calculates negative log likelihood ratio
	if(lambdas.size() > 0) lambdas.clear();
	double u; 
	double v; 
	double t; 
	double lambda = 0;
	double tmp_lambda;
//std::cout << "x: " << x << std::endl;
	//double norms = Nv/Nu;
//std::cout << "nBins " << nBins << std::endl;
	for(int i = 1; i < nBins+1; i++){ //i starting at i discards bin #0 underflow bin (should be empty anyways)
		u = mHist1->GetBinContent(i);
		v = mHist2->GetBinContent(i);
		t = u + v;  
		if(u == 0 && v == 0) { lambdas.push_back(0.); continue;}
		else if(u == 0 && v != 0){ tmp_lambda = -2*t*log(Nv/(Nu+Nv)); }
		else if(u != 0 && v == 0){ tmp_lambda = -2*t*log(Nu/(Nu+Nv)); }
		else tmp_lambda = -2*(t*log( (1 + v/u)/(1 + Nv/Nu) ) + v*log( (Nv/Nu)*(u/v) ));
		lambda += tmp_lambda; 
		//std::cout << "hist1 bin #" << i << ": " << u << " hist2 bin#" << i << ": " << v << " bin LH: " << tmp_lambda << " LH so far: " << lambda << std::endl;
		lambdas.push_back(tmp_lambda);
	}
	return lambda;
}

//original function
double shapeComparison::getPvalue(std::vector<double> &lambdas){
	double LH = calcLikelihood(lambdas); //fill vector of LHs (bin-by-bin) by passing empty vector by reference to calcLikelihood
	for(int i = 0; i < lambdas.size(); i++) lambdas[i] = sqrt(lambdas[i]);
	double pval = 1 - gammp(nDof/2.0,LH/2.0); //value of cumulative distribution function - take inverse b/c we want the probability of a value equal to or greater than our test statistic - this is our p-value
	std::cout << "LH: " << LH <<  " pval: " << pval << std::endl;
	return pval;
}

double shapeComparison::getPvalue(){
	double LH = calcLikelihood(); //fill vector of LHs (bin-by-bin) by passing empty vector by reference to calcLikelihood
	//cout << "likelihood: " << LH << endl;
	double pval = 1 - gammp(nDof/2.0,LH/2.0); //value of cumulative distribution function - take inverse b/c we want the probability of a value equal to or greater than our test statistic - this is our p-value
	// std::cout << "LH: " << LH <<  " pval: " << pval << std::endl;
	return pval;
}

double shapeComparison::getSigma(std::vector<double> &lambdas){
	double LH = sqrt(calcLikelihood(lambdas)); //fill vector of LHs (bin-by-bin) by passing empty vector by reference to calcLikelihood

	double pval = 1 - gammp(nDof/2.0,LH/2.0); //value of cumulative distribution function - take inverse b/c we want the probability of a value equal to or greater than our test statistic - this is our p-value
	std::cout << "LH: " << LH <<  " pval: " << pval << std::endl;
	return pval;
}
double shapeComparison::getSigma(){
	double LH = sqrt(calcLikelihood()); //fill vector of LHs (bin-by-bin) by passing empty vector by reference to calcLikelihood
	//cout << "likelihood: " << LH << endl;
	double pval = 1 - gammp(nDof/2.0,LH/2.0); //value of cumulative distribution function - take inverse b/c we want the probability of a value equal to or greater than our test statistic - this is our p-value
	// std::cout << "LH: " << LH <<  " pval: " << pval << std::endl;
	return pval;
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


//std::vector<double> shapeComparison::getBinPvalues(){
//	double LH = calcLikelihood();
//	std::vector<double> pvals;
//	for(int i = 0; i < nBins; i++){
//		double cdf = 1 - gammp(nDof/2.0,lambdas[i]/2.0); //value of cumulative distribution function
//		pvals.push_back(1 - cdf);//pvalue is inverse of cdf value
////		std::cout << "bin #: " << i << " likelihood: " << lambdas[i] << " pval: " << pvals[i] << std::endl; 
//	}
//	return pvals; 
//}

double shapeComparison::gammp(double a, double x){
	double gamser;
	double gammcf;
	double gln;
	if(x < 0.0 || a <= 0.0){
	 std::cout << "Invalid arguments in gammp. x: " << x << " a:" << a  << std::endl;
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


