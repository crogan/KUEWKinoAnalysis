#include "TSystem.h"
#include "TLine.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"

#include "../include/FitPlotter.hh"

void PlotPrePostShapes(const string& inputfile1 = "BuildFitInputs/BFI_allBkgs_data_T2tt_allchan_fakeShapeSyst0p05_maskSR_08_25_2021/root/BFIShapes.root", const string& inputfile2 = "BuildFits/BF_allBkgs_data_T2tt_wo3L_fakeShapeSyst0p05_maskSR_08_25_2021/all/T2tt/6000425/fitDiagnostics08_30_21_wShapes.root"){

	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
	FitPlotter* FITPlotter_pre = new FitPlotter(inputfile1, inputfile2, "shapes_prefit");
	FitPlotter* FITPlotter_bOnly = new FitPlotter(inputfile1, inputfile2, "shapes_fit_b");
	FitPlotter* FITPlotter_sb = new FitPlotter(inputfile1, inputfile2, "shapes_fit_s");
	string lep_label = "1L";
	string hadS_label = "0jS";
	string hadI_label = "ge1jISR";
	string proc = "HF_Fakes";



  TCanvas* prefit = FITPlotter_pre->Plot1DShape(proc,
                      VS().a(lep_label),
                      VS().a(hadS_label),
                      VS().a(hadI_label),
		      "pre");


  TCanvas* b_fit = FITPlotter_bOnly->Plot1DShape(proc,
                      VS().a(lep_label),
                      VS().a(hadS_label),
                      VS().a(hadI_label),
		      "bFit");

  TCanvas* sb_fit = FITPlotter_sb->Plot1DShape(proc,
                      VS().a(lep_label),
                      VS().a(hadS_label),
                      VS().a(hadI_label),
		      "sbFit");


TH1D* hist_pre = (TH1D*)prefit->GetPrimitive("plothist_0_pre");
cout << "got prefit" << endl;
TH1D* hist_bFit = (TH1D*)b_fit->GetPrimitive("plothist_0_bFit");
cout << "got b-fit" << endl;
TH1D* hist_sbFit = (TH1D*)sb_fit->GetPrimitive("plothist_0_sbFit");
cout << "got s+b-fit" << endl;
if(!hist_pre){ cout << "hist_pre null" << endl; return;}
if(!hist_bFit){ cout << "hist_bFit null" << endl; return;}
if(!hist_sbFit){ cout << "hist_sbFit null" << endl; return;}
double hmax = hist_sbFit->GetMaximum();
if(hist_bFit->GetMaximum() > hmax) hmax = hist_bFit->GetMaximum();

hist_pre->SetMarkerStyle(8);
hist_pre->SetMarkerColor(kRed);

TLegend* leg = new TLegend(0.79, 0.4, 0.98, 0.8);
   leg->SetName("legend");
   leg->SetTextFont(42);
   leg->SetTextSize(0.035);
   leg->SetFillColor(kWhite);
   leg->SetLineColor(kWhite);
   leg->SetShadowColor(kWhite);
   leg->AddEntry(hist_pre,"prefit");
   leg->AddEntry(hist_bFit,"b fit");
   leg->AddEntry(hist_sbFit,"s+b fit");

	prefit->cd();
	//hist_pre->Draw("hist");
	hist_bFit->SetMarkerStyle(22);
	hist_bFit->SetMarkerColor(kGreen);
	hist_bFit->SetLineColor(kGreen);
	hist_bFit->SetMarkerSize(2);
	hist_sbFit->SetMarkerStyle(31);
	hist_sbFit->SetMarkerColor(kBlue);
	hist_sbFit->SetLineColor(kBlue);
	hist_sbFit->SetMarkerSize(2);
	hist_bFit->Draw("same p");
	hist_sbFit->Draw("same p");
	hist_pre->GetYaxis()->SetRangeUser(0.001,1.1);
	leg->Draw();
	prefit->Modified();
	prefit->Update();
	b_fit->Close();
	sb_fit->Close();
	//prefit->Draw();





}
