

void FitDiagCov(){
	//TFile* f = new TFile("fitDiagnostics.Test.root");
	TFile* f = new TFile("/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/BF_B20-31/datacards/all/TChiWZ/4000350/fitDiagnostics.Test.root");
	RooFitResult* r = (RooFitResult*) f->Get("fit_b");
	TMatrixDSym m = r->covarianceMatrix();
	RooArgList l = r->floatParsFinal();
	int n=228;
	double x = 999;
	double x1 = 999;
	double x2 = 999;
	double rho =-1;
	for(int i=0; i<n-1; i++){
		for(int j=i+1; j<n; j++){
			x = m(i,j);
			x1 = sqrt(m(i,i));
			x2 = sqrt(m(j,j));
			rho = x/(x1*x2);
			if( fabs(rho) > 0.7){
				std::cout<<"("<<i<<","<<j<<") Cov: "<< x <<" CorrCoeff: "<<rho<<"\n";
				l[i].Print();
				l[j].Print();
				std::cout<<"\n";
			}
		}		
	}
}
