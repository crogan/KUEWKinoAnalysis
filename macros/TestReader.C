#include "TSystem.h"

#include "../include/FitPlotter.hh"

void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){

  FitPlotter* FITReader = new FitPlotter(inputfile, a2, a3);
  //FITReader->PrintCategories();
  //FITReader->PrintProcesses();

  CategoryTreeTool CTTool;

  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();

  VS signals;
  //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160");
  //signals.a("T2tt_5000490").a("T2bW_5000490").a("T2tt_5000480").a("T2bW_5000480");
  //signals.a("TSlepSlep_2000190").a("T2bW_5000480").a("TSlepSlep_2000180").a("T2bW_5000450");
  //signals.a("T2tt_5000480").a("T2bW_5000480").a("T2tt_5000450").a("T2bW_5000450");
  //signals.a("T2tt_5000450").a("T2bW_5000450").a("T2tt_5000420").a("T2bW_5000420");
  //signals.a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325");
  //signals.a("T2bb_5000475").a("T2bb_5000450").a("T2bb_5000400").a("T2bb_5000350");
  signals.a("T2bW_5000480").a("T2bW_5000450").a("T2bW_5000420").a("T2bW_5000325"); // 0L
  //signals.a("T2bW_5000480").a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325"); // 1L
  //signals.a("TChiWZ_2500245").a("TChiWZ_2500240").a("TChiWZ_2500230").a("TChiWZ_2500210"); // 2L
  //signals.a("TChiWZ_2500247").a("T2bW_5000490").a("TChiWZ_2500240").a("T2bW_5000480"); // 2L
  //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160"); // 3L
  VS all = signals;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("Data");

  FITReader->PlotYields("0Lyields",
  			all,
  			CT_0L);
  // FITReader->PlotYields("1Lyields",
  // 			all,
  // 			CT_1L);
  FITReader->PlotYields("2Lyields",
  			all,
  			CT_2L);
  FITReader->PlotYields("3Lyields",
  			all,
  			CT_3L);
  
  // int depth0 = CT_0L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_0L.GetListDepth(CTs, depth0-3);

  // vector<const CategoryTree*> CTs_deep;
  // CT_0L.GetListDepth(CTs_deep, depth0-3);
  
  // int depth0 = CT_1L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_1L.GetListDepth(CTs, depth0-1);

  // vector<const CategoryTree*> CTs_deep;
  // CT_1L.GetListDepth(CTs_deep, depth0-2);

  // int depth0 = CT_2L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_2L.GetListDepth(CTs, depth0-2);

  // vector<const CategoryTree*> CTs_deep;
  // CT_2L.GetListDepth(CTs_deep, depth0-2);

  // int depth0 = CT_3L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_3L.GetListDepth(CTs, depth0-2);

  // vector<const CategoryTree*> CTs_deep;
  // CT_3L.GetListDepth(CTs_deep, depth0-2);

  // int iCT = 0;
  // int iCTd = 0;
  
  // FITReader->Plot1Dstack("0Lstack2",
  // 			 all,
  // 			 *CTs[iCT], true);
  // FITReader->Plot2D("2D0",
  // 		    VS().a("data_obs"),
  // 		    *CTs_deep[iCTd]);
  // for(int s = 0; s < signals.size(); s++){
  //   FITReader->Plot2D(Form("2D%d",s+1),
  // 		      VS().a(signals[s]),
  // 		      *CTs_deep[iCTd]);
  //  }

  // FITReader->Plot1Dstack(all,
  // 			 VS().a("1L"),
  // 			 VS().a("1j1b0svS"),
  // 			 VS().a("ge1jISR").a("ge1j0bISR").a("ge1jge1bISR"),
  // 			 "pre_stack");
  
}
