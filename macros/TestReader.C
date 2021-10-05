#include "TSystem.h"

#include "../include/FitPlotter.hh"

void TestReader(const string& inputfile = "croganBFI_bkgs.root", const string& a2 = "", const string& a3 = ""){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
FitReader* FITReader = new FitReader(inputfile, a2, a3);
//void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
//  void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  
  CategoryTreeTool CTTool;
  CategoryTree CT_Fakes1L = CTTool.GetCategories_Fakes1L();
 // CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_Fakes2L = CTTool.GetCategories_Fakes2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();
  CategoryTree CT_Fakes3L = CTTool.GetCategories_Fakes3L();
//FITReader->PrintCategories();
//CT_Fakes1L.Print(); 
//vector<const CategoryTree*> catTrees;
//CT_Fakes1L.GetListDepth(catTrees,1);
//for(int i = 0; i < int(catTrees.size()); i++){
//cout << "list #" << i << endl;
//catTrees[i]->Print();
//}
//  FITReader->PlotYields("0Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_0L);
//  FITReader->PlotYields("1Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_1L);
//  FITReader->PlotYields("2Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_2L);
//  FITReader->PlotYields("3Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_3L);

  VS signals;
  //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160");
  //signals.a("T2tt_5000490").a("T2bW_5000490").a("T2tt_5000480").a("T2bW_5000480");
  //signals.a("TSlepSlep_2000190").a("T2bW_5000480").a("TSlepSlep_2000180").a("T2bW_5000450");
  //signals.a("T2tt_5000480").a("T2bW_5000480").a("T2tt_5000450").a("T2bW_5000450");
  //signals.a("T2tt_5000450").a("T2bW_5000450").a("T2tt_5000420").a("T2bW_5000420");
  signals.a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325");
  //signals.a("T2bb_5000475").a("T2bb_5000450").a("T2bb_5000400").a("T2bb_5000350");
  //signals.a("T2bW_5000480").a("T2bb_5000475").a("T2tt_5000450").a("T2bW_5000420").a("T2bW_5000325"); // 0L
  //signals.a("T2bW_5000480").a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325"); // 1L
  //signals.a("TChiWZ_2500245").a("TChiWZ_2500240").a("TChiWZ_2500230").a("TChiWZ_2500210"); // 2L
  //signals.a("TChiWZ_2500247").a("T2bW_5000490").a("TChiWZ_2500240").a("T2bW_5000480"); // 2L
  //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160"); // 3L
  VS all = signals;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("Data");

  // FITReader->PlotYields("0Lyields",
  // 			all,
  // 			CT_0L);
  // FITReader->PlotYields("1Lyields",
  // 			all,
  // 			CT_1L);
  // FITReader->PlotYields("2Lyields",
  // 			all,
  // 			CT_2L);
  // FITReader->PlotYields("3Lyields",
  // 			all,
  // 			CT_3L);
  
  // int depth0 = CT_0L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_0L.GetListDepth(CTs, depth0-3);

  // vector<const CategoryTree*> CTs_deep;
  // CT_0L.GetListDepth(CTs_deep, depth0-3);
  
  int depth0 = CT_1L.GetDepth();
  vector<const CategoryTree*> CTs;
  CT_1L.GetListDepth(CTs, depth0-1);

  vector<const CategoryTree*> CTs_deep;
  CT_1L.GetListDepth(CTs_deep, depth0-2);

  // int depth0 = CT_2L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_2L.GetListDepth(CTs, depth0-2);

//   TCanvas* cv = FITReader->Plot2D(VS().a("T5tttt_22001700"),
//			 VS().a("1L"),
//		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
//      		 //VS().a("ge4j0bS").a("ge4jge1bS"), 
//      		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
//			 "canvas3");


//if(cv == NULL) return;
 // FITReader->PrintCategories();

  int iCT = 1;
  int iCTd = 0;
  
  FITReader->Plot1Dstack("0Lstack2",
  			 all,
  			 *CTs[iCT], true);
  // FITReader->Plot2D("2D0",
  // 		    VS().a("data_obs"),
  // 		    *CTs_deep[iCTd]);
  // for(int s = 0; s < signals.size(); s++){
  //   FITReader->Plot2D(Form("2D%d",s+1),
  // 		      VS().a(signals[s]),
  // 		      *CTs_deep[iCTd]);
  // }

  FITReader->Plot1Dstack(all,
			 VS().a("1L"),
			 VS().a("1j1b0svS"),
			 VS().a("ge1jISR").a("ge1j0bISR").a("ge1jge1bISR"),
			 "pre_stack");
  
}
