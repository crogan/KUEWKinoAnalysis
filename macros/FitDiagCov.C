

void FitDiagCov(){
	TFile* f = new TFile("fitDiagnostics.Test.root");
	RooFitResult* r = (RooFitResult*) f->Get("fit_b");
	TMatrixDSym m = r->covarianceMatrix();
	RooArgList l = r->floatParsFinal();
	int n=228;
	double x = 999;
	for(int i=0; i<n-1; i++){
		for(int j=i+1; j<n; j++){
			x = m(i,j);
			if( fabs(x) > 0.1){
				std::cout<<"("<<i<<","<<j<<") "<< x <<"\n";
				l[i].Print();
				l[j].Print();
				std::cout<<"\n";
			}
		}		
	}
}
