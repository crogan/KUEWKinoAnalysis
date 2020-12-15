#include "TSystem.h"

#include "../include/FitReader.hh"

void Reader2D(const string& lep,
	      const string& hadS,
	      const string& hadISR,
	      const string& signal = "",
	      const string& extra = "",
	      const string& inputfile = "BIG_wed.root"){
  
  FitReader* FITReader = new FitReader(inputfile);
  //FITReader->PrintCategories();
  //FITReader->PrintProcesses();

  FITReader->Plot1Dstack(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("Wjets").a("T2tt_6500600").a("T2tt_6500550").a("T2tt_6500475"),
  			 VS().a(lep),
  			 VS().a(hadS),
  			 VS().a(hadISR),
  			 "cstack2");

  FITReader->Plot1Dstack(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("Wjets").a(signal),
  			 VS().a(lep),
  			 VS().a(hadS),
  			 VS().a(hadISR),
  			 "cstack");
  FITReader->Plot2D(VS().a("Wjets"),
		    VS().a(lep),
		    VS().a(hadS),
		    VS().a(hadISR),
		    "canvas1",
		    extra);
  FITReader->Plot2D(VS().a("data_obs"),
		    VS().a(lep),
		    VS().a(hadS),
		    VS().a(hadISR),
		    "canvas2",
		    extra);
  FITReader->Plot2D(VS().a(signal),
		    VS().a(lep),
		    VS().a(hadS),
		    VS().a(hadISR),
		    "canvas3",
		    extra);


   // FITReader->Plot2D(VS().a("ttbar"),
   // 			 VS().a("2L"),
   // 			 VS().a("0j0svS"),
   // 			 VS().a("ge1j0bISR"),
   // 		    "canvas1");

    // FITReader->Plot2D(VS().a("T2tt_600_570"),
    // 			 VS().a("2L"),
    // 			 VS().a("0j0svS"),
    // 			 VS().a("ge1j0bISR"),
    // 		    "canvas2");
  
}
