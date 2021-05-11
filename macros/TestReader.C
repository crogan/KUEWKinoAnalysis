#include "TSystem.h"

#include "../include/FitReader.hh"

void TestReader(const string& inputfile = "bkgFitInput17_1L_R5R6slopeBinning.root", const string& a2 = "", const string& a3 = ""){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
FitReader* FITReader = new FitReader(inputfile, a2, a3);
//void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
//  void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  void TestReader(const string& inputfile = "TEST4jbinning_1L.root", const string& a2 = "", const string& a3 = ""){
  
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
  signals.a("T2tt_5000490").a("T2bW_5000490").a("T2tt_5000480").a("T2bW_5000480");
  //signals.a("T2tt_5000450").a("T2bW_5000450").a("T2tt_5000420").a("T2bW_5000420");
  //signals.a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325");
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
  
  int depth0 = CT_1L.GetDepth();
  vector<const CategoryTree*> CTs;
  CT_1L.GetListDepth(CTs, depth0-2);

  vector<const CategoryTree*> CTs_deep;
  CT_1L.GetListDepth(CTs_deep, depth0-2);

  int iCT = 2;
  int iCTd = 2;
  

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
