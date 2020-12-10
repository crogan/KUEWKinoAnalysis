#include "TSystem.h"

#include "../include/FitReader.hh"

void TestReader(const string& inputfile = "T2tt_5000325FitInput17_2L_R5R6slopeBinning.root", const string& a2 = "", const string& a3 = ""){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
FitReader* FITReader = new FitReader(inputfile, a2, a3);
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

  //  TCanvas* cv = FITReader->Plot2D(VS().a("ttbar"),
  //      		 VS().a("1L"),
  //      		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
  // //     		 VS().a("ge4j0bS").a("ge4jge1bS"), 
  //      		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
  //			 "canvas3");
  

// TCanvas* cv = FITReader->Plot2D(VS().a("T1bbbb_13001100"),
//  			 VS().a("1L"),
//        		 //VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
//        		 VS().a("ge4j0bS").a("ge4jge1bS"), 
//        		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
//  			 "canvas3");
  
   TCanvas* cv = FITReader->Plot2D(VS().a("T2tt_5000325"),
  			 VS().a("2L"),
        		//VS().a("ge4j0bS").a("ge4jge1bS"),
        		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
        		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
  			 "canvas3");
 

//   TCanvas* cv = FITReader->Plot2D(VS().a("T5tttt_22001700"),
//			 VS().a("1L"),
//		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
//      		 //VS().a("ge4j0bS").a("ge4jge1bS"), 
//      		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
//			 "canvas3");


if(cv == NULL) return;
 // FITReader->PrintCategories();
 // FITReader->PrintProcesses();
}
