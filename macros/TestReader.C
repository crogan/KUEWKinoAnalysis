#include "TSystem.h"

#include "../include/FitReader.hh"

void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  
  FitReader* FITReader = new FitReader(inputfile, a2, a3);
  FITReader->PrintCategories();
  FITReader->PrintProcesses();

  // FITReader->Plot1Dstack(SL().a("ttbar").a("ST").a("DB").a("ZDY").a("TB")
  // 			 .a("Wjets").a("Fakes").a("T2bW_50000490").a("T2bW_50000480").a("T2bW_50000470"),
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
  // FITReader->Plot2D(VS().a("T2tt_5000470"),
  // 			 VS().a("1L"),
  // 			 VS().a("2j1bS"),
  // 			 VS().a("0bjetISR"),
  // 			 "canvas2");

  // FITReader->Plot2D(VS().a("T2tt_5000480"),
  // 			 VS().a("1L"),
  // 			 VS().a("0jge1svS"),
  // 			 VS().a("0bjetISR"),
  // 			 "canvas3");

  FITReader->Plot2D(VS().a("ttbar"),
  			 VS().a("1L"),
  			 VS().a("0jge1svS"),
  			 VS().a("ge1j0bISR"),
		    "canvas3",
		    "SVeta0");
  FITReader->Plot1Dstack(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB"),
			 VS().a("1L"),
  			 VS().a("0j0svS"),
  			 VS().a("ge1j0bISR"),
  			 "canvas2");
}
