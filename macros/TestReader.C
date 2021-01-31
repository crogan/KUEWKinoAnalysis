#include "TSystem.h"

#include "../include/FitReader.hh"

void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  
  FitReader* FITReader = new FitReader(inputfile, a2, a3);
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
  //signals.a("T2tt_5000450").a("T2bW_5000450").a("T2tt_5000420").a("T2bW_5000420");
  signals.a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325");
  //signals.a("T2bb_5000475").a("T2bb_5000450").a("T2bb_5000400").a("T2bb_5000350");
  VS all = signals;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("Fakes");

  FITReader->PlotYields("0Lyields",
  			all,
  			CT_0L);
  FITReader->PlotYields("1Lyields",
  			all,
  			CT_1L);
  FITReader->PlotYields("2Lyields",
  			all,
  			CT_2L);
  FITReader->PlotYields("3Lyields",
  			all,
  			CT_3L);
  
  int depth0 = CT_0L.GetDepth();
  vector<const CategoryTree*> CTs;
  CT_0L.GetListDepth(CTs, depth0-2);

  vector<const CategoryTree*> CTs_deep;
  CT_0L.GetListDepth(CTs_deep, depth0-1);

  int iCT = 0;
  int iCTd = 0;
  
  FITReader->Plot1Dstack("0Lstack2",
			 all,
			 *CTs[iCT]);
  FITReader->Plot2D("2D0",
		    VS().a("data_obs"),
		    *CTs_deep[iCTd]);
  for(int s = 0; s < signals.size(); s++){
    FITReader->Plot2D(Form("2D%d",s+1),
		      VS().a(signals[s]),
		      *CTs_deep[iCTd]);
  }
  
}
