#include "TSystem.h"

#include "../include/FitPlotter.hh"

TString RmSpace(TString str);

void TestReader_test(const string& inputfile = "/home/t3-ku/aabreuna/WorkDir/CMSSW_10_6_12/src/KUEWKinoAnalysis/BFI_batch_Nov2021.root", const string& a2 = "", const string& a3 = ""){
  
  FitPlotter* FITPlotter = new FitPlotter(inputfile, a2, a3);
  //FITPlotter->PrintCategories();
  //FITReader->PrintProcesses();

  CategoryTreeTool CTTool;

  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();



  VS sigs;
  sigs.a("ttbar");
  VS bkgs;
  bkgs.a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes"); 

  VS sigs2;
  VS bkgs2;
  sigs2.a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000450").a("T2tt_5000375");
  bkgs2.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");

  VS  sigs3;
  sigs3.a("TChiWZ_3000270").a("T2bW_5000490").a("TSlepSlep_2000180");
  VS sig_lowDm, sig_midDm, sig_midDm_v2, sig_highDm;

  sig_lowDm.a("T2tt_5000490").a("T2tt_5000480").a("T2bW_5000490").a("T2bW_5000480");
  sig_midDm.a("T2tt_5000450").a("T2tt_5000400").a("T2bW_5000450").a("T2bW_5000420");
  sig_midDm_v2.a("T2tt_5000450").a("T2tt_5000400").a("T2bW_5000450").a("T2bW_5000420").a("T2bb_5000450").a("T2bb_5000400");
  sig_highDm.a("T2tt_6000425").a("T2tt_7000450").a("T2bW_6000425").a("T2bW_7000450");

  VS all_lowDm = sig_lowDm;
  all_lowDm.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");
  VS all_midDm = sig_midDm;
  all_midDm.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");
  VS all_midDm_v2 = sig_midDm_v2;
  all_midDm_v2.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");
  VS all_highDm = sig_highDm;
  all_highDm.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");

  VS signals;
  //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160");
  //signals.a("T2tt_5000490").a("T2bW_5000490").a("T2tt_5000480").a("T2bW_5000480");
  //signals.a("T2tt_5000450").a("T2bW_5000450").a("T2tt_5000420").a("T2bW_5000420");
  signals.a("T2tt_5000450").a("T2tt_5000480").a("T2tt_5000420").a("T2tt_5000375");
  //signals.a("T2bb_5000475").a("T2bb_5000450").a("T2bb_5000400").a("T2bb_5000350");
  VS all = signals;
  //all.a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("Fakes");
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");
 int depth0 = CT_2L.GetDepth();
  vector<const CategoryTree*> CTs_2L;
  CT_2L.GetListDepth(CTs_2L, depth0-4);
  for(int i = 0; i < int(CTs_2L.size()); i++){
    TCanvas* cv = FITPlotter->Plot1Dstack(string(RmSpace(CTs_2L[i]->GetPlainLabel(depth0))+"stack_lowDm_RISR_Mperp"),
                                          all_lowDm,
                                          *CTs_2L[i]);
	if(cv == nullptr){ cout << "cv null for " << CTs_2L[i]->GetPlainLabel(depth0) << endl; continue; }
    //cv->SaveAs(string("plots/"+RmSpace(CTs_2L[i]->GetPlainLabel(depth0))+"stack_lowDm_RISR_Mperp.gif").c_str());
 cv->Close(); 
  }
}
TString RmSpace(TString str){
  TString temp_str = str;
  temp_str.ReplaceAll(" ", "_");
 
  return temp_str;
}
