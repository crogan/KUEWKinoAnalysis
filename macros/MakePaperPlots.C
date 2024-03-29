#include "TSystem.h"

#include "../include/FitPlotter.hh"

TString RmSpace(TString str);
void MakePaperPlot(TFile *saveFile, 
		   FitPlotter *fitPlotter, 
		   const CategoryTree &categoryTree, 
		   const string name, 
		   const PlotType &type,
		   const VS &signal = VS());


void MakePaperPlots() {

  gROOT->SetBatch(kTRUE);

  // Input BFI file and fit diagnostic
  string inputFile = "/uscms_data/d3/anazario/YOURWORKINGAREA/CMSSW_10_6_5/src/KUEWKinoAnalysis/BFS_B120_BKG161718_DATA161718_TChiWZ161718_T2tt161718_TChipmWW161718_TSlepSlep161718.root";
  string fitDiagnostic = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BF_B120_TChiWZ/datacards/all/TChiWZ/3000270/fitDiagnosticsDiag.BO.root";

  // Create output file
  TFile *file = new TFile("paper_plots.root", "RECREATE");

  // Create FitPlotter object to call stack and significance plotting functions
  FitPlotter* FITPlotter = new FitPlotter(inputFile, fitDiagnostic, "shapes_fit_b");
  
  // Create CategoryTreeTool object to call lepton multiplicity category from the CategoryTree class
  CategoryTreeTool CTTool;
  
  // Add signals for plots here. NOTE: If signal is not in the input BFI file it will segfault!
  VS sigs_flavorSep, sigs_SVonly, sigs_StopMultiJet;
  sigs_flavorSep.a("TSlepSlep_2000190").a("TChiWZ_2500240");
  sigs_SVonly.a("T2tt_5500540").a("T2tt_5000480");
  sigs_StopMultiJet.a("T2tt_6000520").a("T2tt_6500570");

  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_0L_Summary(), "stack_summary0L_Inv", kInv);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_1L_plotFormat(), "stack_summary1L_Inv", kInv);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_2L_plotFormat(), "stack_summary2L_Inv", kInv);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_3L_plotFormat(), "stack_summary3L_Inv", kInv);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_SVonly(), "stack_SVonly_Inv", kInv, sigs_SVonly);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_SVConly(), "stack_SVConly_Inv", kInv, sigs_SVonly);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_2L_flavorSepAndSS(), "stack_flavorSep_RISR", kRISR, sigs_flavorSep);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_0LMultiJetBsplit(), "stack_0LMultiJetBsplit_Inv", kInv, sigs_StopMultiJet);
  MakePaperPlot(file, FITPlotter, CTTool.GetCategories_StopMultiJet(), "stack_StopMultiJet_Inv", kInv, sigs_StopMultiJet);

  file->Close();
  delete file;  
}

TString RmSpace(TString str){

  TString temp_str = str;
  temp_str.ReplaceAll(" ", "_");

  return temp_str;
}

void MakePaperPlot(TFile *saveFile, 
		   FitPlotter *fitPlotter, 
		   const CategoryTree &categoryTree, 
		   const string name, 
		   const PlotType &type,
		   const VS &signal) {


  VS bkgs;
  bkgs.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("data_obs");
  bkgs += signal;

  const int depth = categoryTree.GetDepth();
  vector<const CategoryTree*> CTs;
  categoryTree.GetListDepth(CTs, depth-2);

  auto canvas = fitPlotter->Plot1Dstack(string(RmSpace(CTs[0]->GetPlainLabel(depth))+name.c_str()),
                                          bkgs,
                                          *CTs[0],
                                          type,
                                          true);

  saveFile->cd();
  canvas->Write();
}
