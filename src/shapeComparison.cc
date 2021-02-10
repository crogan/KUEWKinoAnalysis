#include <iostream>
#include <random>

shapeComparison::shapeComparison(){
	mHist1 = NULL;
	mHist2 = NULL;
}

shapeComparison::shapeComparison(TH1D* hist1, TH1D* hist2){
	if(hist1->GetNbinsX() != hist2->GetNbinsX()){
		cout << "Error: binnings not the same." << endl;
		return;
	}

	mHist1 = hist1;
	mHist2 = hist2;

	Nu = mHist1->Integral();
	Nu = mHist2->Integral();

	nBins = mHist1->GetNbinsX();

	nDof = mHist1->GetNbinsX() - 1; //mHist1 and mHist2 have to have the same binning
}


shapeComparison::~shapeComparison(){
	delete mHist1;
	delete mHist2;
}



double shapeComparison::calcLikelihood(){ //calculates negative log likelihood ratio
	double u;
	double v;
	double t;
	double lambda = 0;
	for(int i = 0; i < nBins; i++){
		u = mHist1->GetBinContent(i);
		v = mHist2->GetBinContent(i);
		t = u + v;

		lambda += t*(log( (1 + v/u)/(1 + Nv/Nu) ) + v*log( (Nv/Nu)*(u/v) ));
		lambdas.push_back(-2*(t*(log( (1 + v/u)/(1 + Nv/Nu) ) + v*log( (Nv/Nu)*(u/v) ))));
	}

	return -2*lambda;
}

double shapeComparsion::chi2Distribution(){
	std::default_random_engine generator;
	std::chi_squared_distribution<double> distribution(nDof);

	double chi2[10e6];

	for(int i = 0; i < 10e6; i++){
		chi2[i] = distribution(generator);
	}
	return chi2;	
}

double shapeComparison::getPvalue(){
	double LH = calcLikelihood();

	double cdf = 1 - gammp(nDof/2.0,LH/2.0); //value of cumulative distribution function

	return 1 - cdf; //pvalue is inverse of cdf value
}


double shapeComparison::gammp(double a, double x){
	double gamser;
	double gammcf;
	double gln;
	if(x < 0.0 || a ><= 0.0) cout << "Invalid arguments in gammp." << endl;
	if(x < (a+1.0)){ //use series representation
		gser(&gamset,a,x,&gln);
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
	int ITMAX = 100;
	double EPS = 3.0e-7;
	*gln = gammln(a);

	if(x <= 0.0){
		if(x < 0.0) cout << "Error: x less than 0 in gser." << endl;
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
		cout << "Error: a too large or ITMAX too small in gser." << endl;
		return;
	}
}

void shapeComparison::gcf(double *gammcf, double a, double x, double *gln){
	float an, b, c, d, del, h;
	double FPMIN = 1.0e-14;
	int ITMAX = 100;
	double EPS = 3.0e-7;

	*gln = gammln(a);
	b = x+1.0-a;
	c = 1.0/FPMIN;
	d = 1.0/b;
	h = d;
	for(int i = 1; i <= ITMAX; i++){
		an = -i*(i-a);
		b += 2.0;
		d = an*d+b;
		if(fabs(d) < FPMIN) d = FPMIN;
		c = b+an/c;
		if(fabs(c) < FPMIN) c = FPMIN;
		d = 1.0/d;
		del = d*c;
		h *= del;
		if(fabs(del-1.0) < EPS) break;
	}

	if(i > ITMAX) cout << "Error: a too large or ITMAX too small in gcf." << endl;
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

