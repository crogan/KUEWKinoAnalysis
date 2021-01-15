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

  // FITReader->PlotYields("0Lyields",
  // 			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  // 			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
  // 			CT_0L);
  // FITReader->PlotYields("1Lyields",
  // 			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  // 			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
  // 			CT_1L);
  FITReader->PlotYields("2Lyields",
  			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  			.a("Wjets").a("Fakes").a("TChiWZ_3000295").a("TChiWZ_3000250").a("TChiWZ_3000200"),
  			CT_2L);
  // FITReader->PlotYields("3Lyields",
  // 			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  // 			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
  // 			CT_3L);

  // FITReader->PlotYields("0Lyields",
  // 			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  // 			.a("Wjets").a("Fakes").a("TChiWZ_3000295").a("TChiWZ_3000250").a("TChiWZ_3000200"),
  // 			CT_0L);
  // FITReader->PlotYields("1Lyields",
  // 			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  // 			.a("Wjets").a("Fakes").a("TChiWZ_3000295").a("TChiWZ_3000250").a("TChiWZ_3000200"),
  // 			CT_1L);
  // FITReader->PlotYields("2Lyields",
  // 			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  // 			.a("Wjets").a("Fakes").a("TChiWZ_3000295").a("TChiWZ_3000250").a("TChiWZ_3000200"),
  // 			CT_2L);
  // FITReader->PlotYields("3Lyields",
  // 			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
  // 			.a("Wjets").a("Fakes").a("TChiWZ_3000295").a("TChiWZ_3000250").a("TChiWZ_3000200"),
  // 			CT_3L);
  
  int depth0 = CT_0L.GetDepth();
  vector<const CategoryTree*> CTs;
  CT_2L.GetListDepth(CTs, depth0-1);
  cout << "SIZE " << CTs.size() << endl;
  FITReader->Plot1Dstack("0Lstack2",
			 VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			 .a("Wjets").a("Fakes").a("TChiWZ_3000290").a("TChipmWW_2000180").a("TChiWZ_3000200"),
			 *CTs[10]);
				   
				   FITReader->Plot1Dstack("0Lstack3",
			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			.a("Wjets").a("Fakes").a("TChiWZ_3000290").a("TChipmWW_2000180").a("TChiWZ_3000200"),
			*CTs[11]);
		     FITReader->Plot1Dstack("0Lstack0",
			 VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			 .a("Wjets").a("Fakes").a("TChiWZ_3000290").a("TChipmWW_2000180").a("TChiWZ_3000200"),
			 *CTs[12]);
				   
				   FITReader->Plot1Dstack("0Lstack1",
			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			.a("Wjets").a("Fakes").a("TChiWZ_3000290").a("TChipmWW_2000180").a("TChiWZ_3000200"),
			*CTs[13]);
  //*CTs[16]
  
  // FITReader->Plot1Dstack(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB")
  // 			 .a("Wjets").a("Fake").a("HF").a("LF").a("T2tt_5000470").a("T2tt_5000450").a("T2tt_5000420"),
  // 			 VS().a("1L"),
  // 			 VS().a("2j1bS"),
  // 			 VS().a("0bjetISR"),
  // 			 "canvas0");
  // FITReader->Plot2D(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB")
  // 			 .a("Wjets").a("Fake").a("HF").a("LF"),
  // 			 VS().a("1L"),
  // 			 VS().a("2j1bS"),
  // 			 VS().a("0bjetISR"),
  // 			 "canvas1");
  
}
