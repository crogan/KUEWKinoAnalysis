#include "TSystem.h"

#include "../include/FitPlotter.hh"

void TestReader(const string& inputfile = "croganBFI_bkgs.root", const string& a2 = "", const string& a3 = ""){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
FitReader* FITReader = new FitReader(inputfile, a2, a3);
//void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
//  void TestReader(const string& inputfile = "test/FitInput_test.root", const string& a2 = "", const string& a3 = ""){
  
  CategoryTreeTool CTTool;
  CategoryTree CT_Fakes1L = CTTool.GetCategories_Fakes1L();
 // CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_Fakes2L = CTTool.GetCategories_Fakes2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();
  CategoryTree CT_Fakes3L = CTTool.GetCategories_Fakes3L();
//FITReader->PrintCategories();
//CT_Fakes1L.Print(); 
//vector<const CategoryTree*> catTrees;
//CT_Fakes1L.GetListDepth(catTrees,1);
//for(int i = 0; i < int(catTrees.size()); i++){
//cout << "list #" << i << endl;
//catTrees[i]->Print();
//}
//  FITReader->PlotYields("0Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_0L);
//  FITReader->PlotYields("1Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_1L);
//  FITReader->PlotYields("2Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_2L);
//  FITReader->PlotYields("3Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_3L);

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

  // TCanvas* cv = FITReader->Plot2D(VS().a(proc),
  // 			 VS().a("1L"),
  // 			 VS().a("3j0bS"),
  // 			 VS().a("ge1jISR"),
  // 			 "canvas3");
//the args with multiple entries are the ones that go into the total for the ratio
const string& proc = "ttbar_HF"; 
const string& source = "1";
const string& lep = "el";
const string& hadS = "1jS";
const string& hadI = "ge1jISR";
const string& lep1 = "1Lgold";
const string& lep2 = "1Lsilver";
const string& lep3 = "1Lbronze";
const string& lep4 = "3Lmumumu";
const string& hadS1 = "1jS";
const string& hadS2 = "2jS";
const string& hadS3 = "3jS";
 //   TCanvas* cv1 = FITReader->Plot1Dstraight(proc,
 //       		 VS().a(lep1).a(lep2).a(lep3),
 //       		 VS().a(hadS),
 //       		 VS().a(hadI),
 // 			 "canvas2",
 //       		 VS());
    TCanvas* cv = FITReader->Plot1Dratio(proc,
        		 VS().a(lep1).a(lep2).a(lep3),
        		 VS().a(hadS),
 	     		 VS().a(hadI),
  			 "canvas3",
 			 VS());
//    TCanvas* cv = FITReader->Plot1DratioProc(VS().a("ttbar_Fakes_"+lep+"f"+source).a("Wjets_Fakes_"+lep+"f"+source).a("ZDY_Fakes_"+lep+"f"+source).a("ST_Fakes_"+lep+"f"+source).a("TB_Fakes_"+lep+"f"+source),
//        		 "1L",
//        		 hadS,		
// 	     		 hadI,
//  			 "canvas3",
// 			 "");
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


//if(cv == NULL) return;
 // FITReader->PrintCategories();

//     FITReader->Plot2D(VS().a("T2tt_5000420"),
//     			 VS().a("1L"),
//     			 VS().a("ge4j0svS"),
//     			 VS().a("ge1j0bISR").a("0bjetISR"),
//     		    "canvas2");
  // FITReader->Plot1Dstack(VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB"),
  // 			 VS().a("1L"),
  // 			 VS().a("0j0svS"),
  // 			 VS().a("ge1j0bISR"),
  // 			 "canvas2");
}
