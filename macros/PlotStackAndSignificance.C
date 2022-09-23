#include "TSystem.h"

#include "../include/FitPlotter.hh"

TString RmSpace(TString str);

void PlotStackAndSignificance(){

  //Input BFI file
  string inputFile = "/uscms/home/mlazarov/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis/BFI_allBkgs_data_TChiWZ_2017_allchan_maskSR_12_9_21/root/BFI.root";//"/home/t3-ku/aabreuna/WorkDir/CMSSW_10_6_12/src/KUEWKinoAnalysis/BFI_batch_hist_v2.root";

  //Create FitPlotter object to call stack and significance plotting functions
  FitPlotter* FITPlotter = new FitPlotter(inputFile, "", "");
  
  //Create CategoryTreeTool object to call lepton multiplicity category from the CategoryTree class
  CategoryTreeTool CTTool;
  
  //Choosing category 1L with charge separation (defined in CategoryTree)
  CategoryTree CT_1L = CTTool.GetCategories_1L_ChargeSep();
 
  //Assign signals and backgrounds for plot
  VS sigs;
  VS bkgs;
  
  bkgs.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");
  sigs.a("T2tt_5000490").a("T2tt_5000480").a("T2bW_5000490").a("T2bW_5000480");
  
  VS all = sigs;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");
  
  //Find CategoryTree object depth
  int depth0 = CT_1L.GetDepth();
  vector<const CategoryTree*> CTs_1L;
  CT_1L.GetListDepth(CTs_1L, depth0-2);

  cout << CTs_1L.size() << endl;

  //Loop over all regions at specified depth
  for(int i = 0; i < int(CTs_1L.size()); i++){ 

//    //significance plot with Mperp binning    
//    FITPlotter->PlotRegionSignificance(string(RmSpace(CTs_1L[i]->GetPlainLabel(depth0))+"ratioZbin_chargeSep_RISR_Mperp"),
//				       bkgs,                                                                                                                 
//				       sigs,
//				       *CTs_1L[i]);
//
//    //significance plot integrated over Mperp 
//    FITPlotter->PlotRegionSignificance(string(RmSpace(CTs_1L[i]->GetPlainLabel(depth0))+"ratioZbin_chargeSep_RISR"),
//                                       bkgs,
//                                       sigs,
//                                       *CTs_1L[i],
//				       kRISR);
    //stack plot with Mperp binning 
    FITPlotter->Plot1Dstack(string(RmSpace(CTs_1L[i]->GetPlainLabel(depth0))+"stack_chargeSep_RISR_Mperp"),
			    all,                                                                                                          
			    *CTs_1L[i]);

    //stack plot integrated over Mperp
    FITPlotter->Plot1Dstack(string(RmSpace(CTs_1L[i]->GetPlainLabel(depth0))+"stack_chargeSep_RISR"),
                            all,
                            *CTs_1L[i],
			    kRISR);
  }
}

TString RmSpace(TString str){

  TString temp_str = str;
  temp_str.ReplaceAll(" ", "_");

  return temp_str;
}
