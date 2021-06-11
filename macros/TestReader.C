#include "TSystem.h"

#include "../include/FitPlotter.hh"

void TestReader(const string& inputfile = "croganBFI_bkgs.root", const string& a2 = "", const string& a3 = ""){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
FitPlotter* FITPlotter = new FitPlotter(inputfile, a2, a3);
  //FITReader->PrintCategories();
  //FITReader->PrintProcesses();

  CategoryTreeTool CTTool;
  CategoryTree CT_Fakes1L = CTTool.GetCategories_Fakes1L();
  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_QCD0L = CTTool.GetCategories_QCD0L();
  CategoryTree CT_QCD1L = CTTool.GetCategories_QCD1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_Fakes2L = CTTool.GetCategories_Fakes2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();
  CategoryTree CT_Fakes3L = CTTool.GetCategories_Fakes3L();
//FITReader->PrintCategories();
//FITReader->PrintProcesses();
  CategoryTree CT_test = CT_QCD0L;
//CT_test.Print(); 
//vector<const CategoryTree*> catTrees;
//CT_test.GetListDepth(catTrees,1);
//CategoryList catList = FITReader->GetCategories();
//for(int i = 0; i < int(catTrees.size()); i++){
//cout << "list #" << i << " " << catTrees[i]->GetSpecLabel() << endl;
//CategoryList cats = catList.Filter(*catTrees[i]);
//cats.Print();
//}
//  FITReader->PlotYields("0Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_0L);
//  FITReader->PlotYields("1Lyields",
//			VS().a("QCD").a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("Wjets").a("Fakes"),
//			CT_1L);
//  FITReader->PlotYields("1Lyields_HF",
//			VS().a("ttbar_HF").a("ST_HF").a("DB_HF").a("ZDY_HF").a("TB_HF").a("Wjets_HF").a("HF"),
//			CT_1L);
//  FITReader->PlotYields("2Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_2L);
//  FITReader->PlotYields("3Lyields",
//			VS().a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD")
//			.a("Wjets").a("Fakes").a("T2tt_5000490").a("T2tt_5000480").a("T2tt_5000375"),
//			CT_3L);

  VS signals;
  //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160");
  //signals.a("T2tt_5000490").a("T2bW_5000490").a("T2tt_5000480").a("T2bW_5000480");
  //signals.a("TSlepSlep_2000190").a("T2bW_5000480").a("TSlepSlep_2000180").a("T2bW_5000450");
  //signals.a("T2tt_5000480").a("T2bW_5000480").a("T2tt_5000450").a("T2bW_5000450");
  //signals.a("T2tt_5000450").a("T2bW_5000450").a("T2tt_5000420").a("T2bW_5000420");
  signals.a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325");
  //signals.a("T2bb_5000475").a("T2bb_5000450").a("T2bb_5000400").a("T2bb_5000350");
  //signals.a("T2bW_5000480").a("T2bb_5000475").a("T2tt_5000450").a("T2bW_5000420").a("T2bW_5000325"); // 0L
  //signals.a("T2bW_5000480").a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325"); // 1L
  //signals.a("TChiWZ_2500245").a("TChiWZ_2500240").a("TChiWZ_2500230").a("TChiWZ_2500210"); // 2L
  //signals.a("TChiWZ_2500247").a("T2bW_5000490").a("TChiWZ_2500240").a("T2bW_5000480"); // 2L
  //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160"); // 3L
  VS all = signals;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");

<<<<<<< HEAD
  // TCanvas* cv = FITReader->Plot2D(VS().a(proc),
  // 			 VS().a("1L"),
  // 			 VS().a("3j0bS"),
  // 			 VS().a("ge1jISR"),
  // 			 "canvas3");
//the args with multiple entries are the ones that go into the total for the ratio
const string& proc = "ttbar_Fakes_elf0"; 
const string& source = "LF";
const string& numSource = "0";
const string& lep = "el";
const string& hadS = "2jS";
const string& hadI = "ge1jISR";

const string& lep1 = "1Lel";
const string& lep2 = "1Lelsilver";
const string& lep3 = "1Lelbronze";
const string& lep4 = "3Lelbronze";
const string& hadS1 = "2jS";
const string& hadS2 = "2j1bS";
const string& hadS3 = "2j2bS";
  //  TCanvas* cv1 = FITReader->Plot1Dstraight(proc,
  //      		 VS().a(lep1).a(lep2).a(lep3),
  //      		 VS().a(hadS),
  //      		 VS().a(hadI),
  //      		 "canvas2",
  //      		 VS());
    TCanvas* cv = FITPlotter->Plot1Dratio(proc,
        		 VS().a(lep1).a(lep2).a(lep3),
        		 VS().a(hadS1),
 	     		 VS().a(hadI),
  			 "canvas3",
 			 VS());
  //  TCanvas* cv = FITReader->Plot1DratioProc(VS().a("DB_Fakes_"+lep+"f"+numSource).a("ttbar_Fakes_"+lep+"f"+numSource),
  //      		 "1L",
  //      		 hadS,		
  //           		 hadI,
  //			 "canvas3",
  //      		 "");
  //
// 			 "");
 // VS signals;
 // //signals.a("TChiWZ_2500240").a("TChiWZ_2500220").a("TChiWZ_2500200").a("TChiWZ_2500160");
 // signals.a("T2tt_5000490").a("T2bW_5000490").a("T2tt_5000480").a("T2bW_5000480");
 // //signals.a("T2tt_5000450").a("T2bW_5000450").a("T2tt_5000420").a("T2bW_5000420");
 // //signals.a("T2tt_5000450").a("T2bW_5000420").a("T2tt_5000375").a("T2bW_5000325");
 // //signals.a("T2bb_5000475").a("T2bb_5000450").a("T2bb_5000400").a("T2bb_5000350");
 // VS all = signals;
    VS all;
   all.a("QCD").a("Wjets");

 // FITReader->PlotYields("0Lyields",
 // 			all,
 // 			CT_0L);
  //FITReader->PlotYields("1Lyields",
  //			all,
  //			CT_1L);
 // FITReader->PlotYields("2Lyields",
 // 			all,
 // 			CT_2L);
  //FITReader->PlotYields("3Lyields",
  //			all,
  //			CT_3L);
  
 // int depth0 = CT_1L.GetDepth();
 // vector<const CategoryTree*> CTs;
 // CT_1L.GetListDepth(CTs, depth0-2);

  

 // int iCT = 2;
 // int iCTd = 2;
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
