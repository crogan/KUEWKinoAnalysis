#include "TSystem.h"

#include "../include/FitReader.hh"

void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  
  FitReader* FITReader = new FitReader(inputfile, a2, a3);
  FITReader->PrintCategories();
  FITReader->PrintProcesses();

  // FITReader->Plot1Dstack(SL().a("ttbar").a("ST").a("DB").a("ZDY").a("TB")
  // 			 .a("Wjets").a("Fakes").a("T2bW_50000490").a("T2bW_50000480").a("T2bW_50000470"),
  FITReader->Plot1Dstack(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("Data")
			 .a("Wjets").a("Fake").a("HF").a("LF").a("T2bW_50000480").a("QCD"),
			 VS().a("1Lm"),
			 VS().a("0j1svS"),
			 VS().a("0bjetISR"),
			 "canvas");
  
}
