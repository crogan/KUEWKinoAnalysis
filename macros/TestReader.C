#include "TSystem.h"

#include "../include/FitReader.hh"

void TestReader(const string& inputfile = "FitInput17_bkgs_1L.root", const string& a2 = "", const string& a3 = ""){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
FitReader* FITReader = new FitReader(inputfile, a2, a3);
//void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
//  void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  
 // FITReader->PrintCategories();
 // FITReader->PrintProcesses();
void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  
  FitReader* FITReader = new FitReader(inputfile, a2, a3);
  //FITReader->PrintCategories();
  //FITReader->PrintProcesses();

  CategoryTreeTool CTTool;

  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();

  FITReader->PlotYields("0Lyields",
			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
			CT_0L);
  FITReader->PlotYields("1Lyields",
			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
			CT_1L);
  FITReader->PlotYields("2Lyields",
			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
			CT_2L);
  FITReader->PlotYields("3Lyields",
			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
			CT_3L);

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
const string& proc = "ttbar_HF"; 

  // TCanvas* cv = FITReader->Plot2D(VS().a(proc),
  // 			 VS().a("1L"),
  // 			 VS().a("3j0bS"),
  // 			 VS().a("ge1jISR"),
  // 			 "canvas3");
//the args with multiple entries are the ones that go into the total for the ratio
//    TCanvas* cv = FITReader->Plot1Dstraight(proc,
//        		 VS().a("1Lmugold").a("1Lmusilver").a("1Lmubronze"),
//        		 VS().a("3jbS"),
//        		 VS().a("ge1jISR"),
//  			 "canvas3",
//			 VS().a("PTISR0").a("PTISR1"));
//    TCanvas* cv = FITReader->Plot1Dratio("ttbar_HF",
//        		 VS().a("1L"),
//        		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
//   //     		 VS().a("ge4j0bS").a("ge4jge1bS"), 
//        		 VS().a("ge1j0bISR"),
//  			 "canvas3",
//			 VS());
  
if(cv == NULL) return;
// TCanvas* cv = FITReader->Plot2D(VS().a("T1bbbb_13001100"),
//  			 VS().a("1L"),
//        		 //VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
//        		 VS().a("ge4j0bS").a("ge4jge1bS"), 
//        		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
//  			 "canvas3");
  
  // TCanvas* cv = FITReader->Plot2D(VS().a("T2tt_5000325"),
  //			 VS().a("2L"),
  //      		//VS().a("ge4j0bS").a("ge4jge1bS"),
  //      		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
  //      		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
  //			 "canvas3");
 

//   TCanvas* cv = FITReader->Plot2D(VS().a("T5tttt_22001700"),
//			 VS().a("1L"),
//		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
//      		 //VS().a("ge4j0bS").a("ge4jge1bS"), 
//      		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
//			 "canvas3");


if(cv == NULL) return;
 // FITReader->PrintCategories();
 // FITReader->PrintProcesses();
//  FITReader->Plot2D(VS().a("data_obs"),
//  			 VS().a("1L"),
//  			 VS().a("1j0b0svS"),
//  			 VS().a("ge1j0bISR"),
//		    "canvas3");
=======
  // FITReader->Plot2D(VS().a("data_obs"),
  // 			 VS().a("1L"),
  // 			 VS().a("1j0b0svS"),
  // 			 VS().a("ge1j0bISR"),
  // 		    "canvas3");
>>>>>>> crogan-main

   // FITReader->Plot2D(VS().a("ttbar"),
   // 			 VS().a("2L"),
   // 			 VS().a("0j0svS"),
   // 			 VS().a("ge1j0bISR"),
   // 		    "canvas1");

     FITReader->Plot2D(VS().a("T2tt_5000420"),
     			 VS().a("1L"),
     			 VS().a("ge4j0svS"),
     			 VS().a("ge1j0bISR").a("0bjetISR"),
     		    "canvas2");
  // FITReader->Plot1Dstack(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB"),
  // 			 VS().a("1L"),
  // 			 VS().a("0j0svS"),
  // 			 VS().a("ge1j0bISR"),
  // 			 "canvas2");
}
