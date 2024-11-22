#include "TSystem.h"

#include "../include/FitPlotter.hh"

TString RmSpace(TString str);

void PlotStackAndSignificance(){

  //Input BFI file
  //string inputFile = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B120/BFS_B120_BKG161718_DATA161718_TChiWZ161718.root";
  string inputFile = "BFI_Output_root/BFS_B20-101_BKG161718_DATA161718_TSlepSlep161718_TChiWZ161718_T2tt161718_Ldef.root";
  //"/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/BFI_B20-101/BFS_B20-101_BKG161718_DATA161718_TSlepSlep161718_Ldef.root";
  string fitDiagnostic = "fitDiagnostics/fitDiagnostics.Test.root";
  //string fitDiagnostic = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BF_B120_TChiWZ/datacards/all/TChiWZ/3000270/fitDiagnosticsDiag.BO.root";
  //"/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/BFI_B20-92_BKG17_DATA17_Ldef/BFI_B20-92_BKG17_DATA17_Ldef.root";
  //"/uscms/home/mlazarov/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/BFI_allBkgs_data_TChiWZ_2017_allchan_maskSR_12_9_21/root/BFI.root";//"/home/t3-ku/aabreuna/WorkDir/CMSSW_10_6_12/src/KUEWKinoAnalysis/BFI_batch_hist_v2.root";

  TFile *file = new TFile("paper_plots.root", "RECREATE");

  //Create FitPlotter object to call stack and significance plotting functions
  FitPlotter* FITPlotter = new FitPlotter(inputFile, fitDiagnostic, "shapes_fit_s");
  
  //Create CategoryTreeTool object to call lepton multiplicity category from the CategoryTree class
  CategoryTreeTool CTTool;
  
  //Choosing category 1L with charge separation (defined in CategoryTree)
  CategoryTree CT_SVonly = CTTool.GetCategories_SVonly();//
  CategoryTree CT_SVConly = CTTool.GetCategories_SVConly();
  CategoryTree CT_flavorSep = CTTool.GetCategories_2L_flavorSepAndSS();
  CategoryTree CT_StopMultiJet = CTTool.GetCategories_0LMultiJetBsplit();
  //CategoryTree CT_StopMultiJet = CTTool.GetCategories_StopMultiJet(); 
  //Assign signals and backgrounds for plot
  VS sigs_flavorSep, sigs_SVonly, sigs_StopMultiJet;
    
  //sigs_flavorSep.a("TSlepSlep_2000190").a("TChiWZ_2500240");
  //sigs_flavorSep.a("TChiWZ_2500240");
  //sigs_SVonly.a("T2tt_5500540").a("T2tt_5000480");
  //sigs_StopMultiJet.a("T2tt_6000520").a("T2tt_6500570");
  VS all_flavorSep = sigs_flavorSep;
  VS all_SVonly = sigs_SVonly;
  VS all_StopMultiJet = sigs_StopMultiJet;
  all_flavorSep.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("data_obs");
  all_SVonly.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("data_obs");
  all_StopMultiJet.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("data_obs");

  //Find CategoryTree object depth
  int depth_flavorSep = CT_flavorSep.GetDepth();
  int depth_SVonly = CT_SVonly.GetDepth();
  int depth_SVConly = CT_SVConly.GetDepth();
  int depth_StopMultiJet = CT_StopMultiJet.GetDepth();
  vector<const CategoryTree*> CTs_flavorSep;
  vector<const CategoryTree*> CTs_SVonly;
  vector<const CategoryTree*> CTs_SVConly;
  vector<const CategoryTree*> CTs_StopMultiJet;
  CT_flavorSep.GetListDepth(CTs_flavorSep, depth_flavorSep-2);
  CT_SVonly.GetListDepth(CTs_SVonly, depth_SVonly-2);
  CT_SVConly.GetListDepth(CTs_SVConly, depth_SVConly-2);
  CT_StopMultiJet.GetListDepth(CTs_StopMultiJet, depth_StopMultiJet-2);

  //Loop over all regions at specified depth
  for(int i = 0; i < int(CTs_flavorSep.size()); i++){ 
    //stack plot integrated over Mperp
    auto canvas = FITPlotter->Plot1Dstack(string(RmSpace(CTs_flavorSep[i]->GetPlainLabel(depth_flavorSep))+"stack_flavorSep_RISR"),
					  all_flavorSep,
					  *CTs_flavorSep[i],
					  kRISR,
					  true);

    file->cd();
    canvas->Write();

  }

  for(int i = 0; i < int(CTs_SVConly.size()); i++){
    //stack plot integrated over Mperp
    auto canvas  = FITPlotter->Plot1Dstack(string(RmSpace(CTs_SVConly[i]->GetPlainLabel(depth_SVConly))+"stack_SVConly_Inv"),
					   all_SVonly,
					   *CTs_SVConly[i],
					   kInv,
					   true);
    if(canvas) {
      file->cd();
      canvas->Write();
    }
  }
  
  for(int i = 0; i < int(CTs_SVonly.size()); i++){
    //stack plot integrated over Mperp
    auto canvas  = FITPlotter->Plot1Dstack(string(RmSpace(CTs_SVonly[i]->GetPlainLabel(depth_SVonly))+"stack_SVonly_Inv"),
                                           all_SVonly,
                                           *CTs_SVonly[i],
                                           kInv,
                                           true);
    if(canvas) {
      file->cd();
      canvas->Write();
    }
  }

  for(int i = 0; i < int(CTs_StopMultiJet.size()); i++){
    //stack plot integrated over Mperp
    auto canvas = FITPlotter->Plot1Dstack(string(RmSpace(CTs_StopMultiJet[i]->GetPlainLabel(depth_StopMultiJet))+"stack_0LMultiJetBsplit_Inv"),
					  all_StopMultiJet,
					  *CTs_StopMultiJet[i],
					  kInv,
					  true);
    if(canvas) {
      file->cd();
      canvas->Write();
    }
  }

  file->Close();

  delete file;
}

TString RmSpace(TString str){

  TString temp_str = str;
  temp_str.ReplaceAll(" ", "_");

  return temp_str;
}
