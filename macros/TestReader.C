#include "TSystem.h"

#include "../include/FitReader.hh"

void TestReader(const string& inputfile = "BuildFit_test.root"){
  
  FitReader* FITReader = new FitReader(inputfile);
  FITReader->PrintCategories();
  FITReader->PrintProcesses();

  // FITReader->Plot1Dstack(SL().a("ttbar").a("ST").a("DB").a("ZDY").a("TB")
  // 			 .a("Wjets").a("Fakes").a("T2bW_50000490").a("T2bW_50000480").a("T2bW_50000470"),
  FITReader->Plot1Dstack(SL().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("Data")
			 .a("Wjets").a("Fake").a("HF").a("LF").a("T2bW_50000470").a("QCD"),
			 SL().a("1Lsilver"),
			 SL().a("0j0svS"),
			 SL().a("0bjetISR"),
			 "1L-0S-0bISR");
  
}
