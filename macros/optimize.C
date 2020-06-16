#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TH2D.h>
#include <TStyle.h>

double EvaluateMetric(double Nsig, double Nbkg, double sysperc);

void optimize(){
  string ifile = "Tabulate.root";

  string label = "2lep_PT300";
  //string label = "3Slep_PT200";

   vector<string> BKG;
   BKG.push_back("ttbar");
   BKG.push_back("SingleTop");
   BKG.push_back("DY");
   BKG.push_back("DiBoson");
   BKG.push_back("Wjets");
   BKG.push_back("Znunu");

   string SIG = "T2bW_726_706";

   int NBKG = BKG.size();

   TFile* file = new TFile(ifile.c_str(),"READ");
   
   vector<TH2D*> hists_BKG;
   for(int i = 0; i < NBKG; i++)
     hists_BKG.push_back((TH2D*)file->Get(Form("%s_%s_hist",BKG[i].c_str(),label.c_str())));

   TH2D* hist_SIG = (TH2D*)file->Get(Form("%s_%s_hist",SIG.c_str(),label.c_str()));
   
   int Nx = hist_SIG->GetNbinsX();
   int Ny = hist_SIG->GetNbinsY();

   // for(int i = 0; i < Nx; i++)
   //   for(int j = 0; j < Ny; j++)
   //     cout << hists_BKG[4]->GetBinContent(i+1,j+1) << endl;
   // return;

   int bestix = 0;
   int bestiy = 0;
   int bestjx = 0;
   int bestjy = 0;
   double bestsig = 0.;
   
   for(int ibinx = 1; ibinx <= Nx-1; ibinx++){
     cout << "i " << ibinx << endl;
     for(int ibiny = 1; ibiny <= Ny-1; ibiny++){
       //cout << ibiny << " ";
       for(int jbinx = ibinx+1; jbinx <= Nx; jbinx++){
	 double Nsig = 0.;
	 double Nbkg = 0.;

	 for(int x = ibinx; x <= jbinx; x++){
	   Nsig += hist_SIG->GetBinContent(x, ibiny);
	   for(int b = 0; b < NBKG; b++){
	     Nbkg += hists_BKG[b]->GetBinContent(x, ibiny);
	   }
	 }
	 
	 for(int jbiny = ibiny+1; jbiny <= Ny; jbiny++){
	   for(int x = ibinx; x <= jbinx; x++){
	     Nsig += hist_SIG->GetBinContent(x, jbiny);
	     for(int b = 0; b < NBKG; b++){
	       Nbkg += hists_BKG[b]->GetBinContent(x, jbiny);
	     }
	   }

	   if(Nsig > 1. && Nbkg > 1.){
	   
	     double sig = EvaluateMetric(Nsig, Nbkg, 10.);
	   
	     // cout << Nsig << " " << Nbkg << endl;
	     // cout << sig << endl;

	     if(sig > bestsig){
	       bestsig = sig;
	       bestix = ibinx;
	       bestiy = ibiny;
	       bestjx = jbinx;
	       bestjy = jbiny;
	     }
	   }
	 }
       }
     }
   }

   cout << endl << "Best sig " << bestsig << endl;
   cout << "Mperp in [" << hist_SIG->GetXaxis()->GetBinLowEdge(bestix) << " , " << hist_SIG->GetXaxis()->GetBinUpEdge(bestjx) << "]" << endl;
   cout << "RISR in [" << hist_SIG->GetYaxis()->GetBinLowEdge(bestiy) << " , " << hist_SIG->GetYaxis()->GetBinUpEdge(bestjy) << "]" << endl;

   vector<double> Nbkg;
   for(int i = 0; i < NBKG+1; i++)
     Nbkg.push_back(0.);
   double Nsig = 0.;

   for(int ix = bestix; ix <= bestjx; ix++)
     for(int iy = bestiy; iy <= bestjy; iy++){
       Nsig += hist_SIG->GetBinContent(ix,iy);
       for(int b = 0; b < NBKG; b++){
	 Nbkg[b] += hists_BKG[b]->GetBinContent(ix,iy);
	 Nbkg[NBKG] += hists_BKG[b]->GetBinContent(ix,iy);
       }
     }

   cout << "Total signal for " << SIG << " " << Nsig << endl;
   cout << "Total background " << Nbkg[NBKG] << endl;
   for(int i = 0; i < NBKG; i++){
     cout << BKG[i] << " " << Nbkg[i] << endl;
   }

   cout << EvaluateMetric(Nsig, Nbkg[NBKG], 10.) << endl;
   
}

double EvaluateMetric(double Nsig, double Nbkg, double sysperc){
  double Nobs = Nsig+Nbkg;
  double tau = 1./Nbkg/(sysperc*sysperc/10000.);
  double aux = Nbkg*tau;
  double Pvalue = TMath::BetaIncomplete(1./(1.+tau),Nobs,aux+1.);
  return sqrt(2.)*TMath::ErfcInverse(Pvalue*2);
}
