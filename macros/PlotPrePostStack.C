#include "TSystem.h"
#include "../include/FitPlotter.hh"

void PlotPrePostStack(const string& inputfile1 = "BuildFits/BF_allBkgs_data_T2tt_0L1L_QCDShapeSyst0p05_maskSR_09_02_2021/FitInput_KUEWKino_2017.root", const string& inputfile2 = "BuildFits/BF_allBkgs_data_T2tt_0L1L_QCDShapeSyst0p05_maskSR_09_02_2021/Ch0L/T2tt/6000425/fitDiagnostics.09_08_21wShapes.root"){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
	FitPlotter* FITPlotter_pre = new FitPlotter(inputfile1, inputfile2, "shapes_prefit");
        FitPlotter* FITPlotter_bOnly = new FitPlotter(inputfile1, inputfile2, "shapes_fit_b");
        FitPlotter* FITPlotter_sb = new FitPlotter(inputfile1, inputfile2, "shapes_fit_s");


	string lep_label = "0L";
        string hadS_label = "5j0bS";
        string hadI_label = "ge1jISR";
        VS proc = VS();
	proc += "data_obs";
	proc += "ttbar";
	proc += "ZDY";
	proc += "DB";
	proc += "Wjets";
	proc += "QCD";
	proc += "ST";
	proc += "TB";
	proc += "HF_Fakes";
	proc += "LF_Fakes";
	proc += "T2tt_6000425";
	//proc += "T2tt_6000590";

CategoryTreeTool CTTool;
//CategoryTree catTree = CTTool.GetCategories_1L();
cout << "##############plot prefit#############" << endl;
  TCanvas* prefit_stack = FITPlotter_pre->Plot1Dstack(proc,
		      VS().a(lep_label),
                      VS().a(hadS_label),
                      VS().a(hadI_label),
                      "pre_stack");

//  TCanvas* prefit_yields = FITPlotter_pre->PlotYields("pre_yields",
//		      proc,
//                      catTree);
cout << "##############plot b-fit#############" << endl;

  TCanvas* b_fit = FITPlotter_bOnly->Plot1Dstack(proc,
                      VS().a(lep_label),
                      VS().a(hadS_label),
                      VS().a(hadI_label),
                      "bFit_stack");

//  TCanvas* b_fit_yields = FITPlotter_bOnly->PlotYields("bFit_yields",
//		      proc,
//                      catTree);
cout << "##############plot s+b fit#############" << endl;
  TCanvas* sb_fit = FITPlotter_sb->Plot1Dstack(proc,
                      VS().a(lep_label),
                      VS().a(hadS_label),
                      VS().a(hadI_label),
                      "sbFit_stack");

//  TCanvas* sb_fit_yields = FITPlotter_sb->PlotYields("sbFit_yields",
//		      proc,
//                      catTree);
//



}
