#include "TSystem.h"
#include "TLatex.h"
#include "TLegend.h"

#include "../include/FitPlotter.hh"

//void TestReader(const string& inputfile = "BFI_allBkgs17_shapeNoFakeSysts/root/BFI_0.root", const string& a2 = "", const string& a3 = ""){
void TestReader(const string& inputfile = "/home/t3-ku/aabreuna/WorkDir/CMSSW_10_6_12/src/KUEWKinoAnalysis/BFI_batch_Nov2021.root", const string& a2 = "", const string& a3 = ""){
	gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
FitPlotter* FITPlotter = new FitPlotter(inputfile, a2, a3);
  //FITReader->PrintCategories();
  //FITReader->PrintProcesses();

  CategoryTreeTool CTTool;
//  CategoryTree CT_Fakes1L = CTTool.GetCategories_Fakes1L();
  CategoryTree CT = CTTool.GetCategories_Fakes2L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();
//FITReader->PrintCategories();
//CT_Fakes1L.Print(); 
vector<const CategoryTree*> catTrees;
CT.GetListDepth(catTrees,1);
//for(int i = 0; i < int(catTrees.size()); i++){
//cout << "list #" << i << ": " << catTrees[i]->GetBareLabel()  << endl;
////catTrees[i]->Print();
//}

   int depth0 = CT_2L.GetDepth();
   vector<const CategoryTree*> CTs;
   CT_2L.GetListDepth(CTs, 2);
   vector<const CategoryTree*> CT_3Ls;
   CT_3L.GetListDepth(CT_3Ls, 1);

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
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("Data");
  int iCT = 1;
  int iCTd = 0;
cout << "2L plots" << endl;
for(int i = 0; i < CTs.size(); i++){  
cout << "list #" << i << endl; 
 TCanvas* cv = FITPlotter->Plot1Dstack(Form("2Lstack_%d",i),
  			 all,
  			 *CTs[i], true);
   cv->Close();
}
cout << "3L plots" << endl;
for(int i = 0; i < CT_3Ls.size(); i++){  
cout << "list #" << i << endl; 
 TCanvas* cv = FITPlotter->Plot1Dstack(Form("3Lstack_%d",i),
  			 all,
  			 *CT_3Ls[i], true);
   cv->Close();
}
  // FITReader->PlotYields("0Lyields",
  // 			all,
  // 			CT_0L);
  // FITReader->PlotYields("1Lyields",
  // 			all,
  // 			CT_1L);
  // FITReader->PlotYields("2Lyields",
  // 			all,
  // 			CT_2L);
  // FITReader->PlotYields("3Lyields",
  // 			all,
  // 			CT_3L);
  
  // int depth0 = CT_0L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_0L.GetListDepth(CTs, depth0-3);

  // vector<const CategoryTree*> CTs_deep;
  // CT_0L.GetListDepth(CTs_deep, depth0-3);
  
//  int depth0 = CT_1L.GetDepth();
//  vector<const CategoryTree*> CTs;
//  CT_1L.GetListDepth(CTs, depth0-1);
//
//  vector<const CategoryTree*> CTs_deep;
//  CT_1L.GetListDepth(CTs_deep, depth0-2);

//   TCanvas* cv = FITReader->Plot2D(VS().a("T5tttt_22001700"),
//			 VS().a("1L"),
//		 VS().a("3j0bS").a("3j1bS").a("3jge2bS"),
//      		 //VS().a("ge4j0bS").a("ge4jge1bS"), 
//      		 VS().a("ge1j0bISR").a("ge1jge1bISR"),
//			 "canvas3");


//if(cv == NULL) return;
 // FITReader->PrintCategories();

  // FITReader->Plot2D("2D0",
  // 		    VS().a("data_obs"),
  // 		    *CTs_deep[iCTd]);
  // for(int s = 0; s < signals.size(); s++){
  //   FITReader->Plot2D(Form("2D%d",s+1),
  // 		      VS().a(signals[s]),
  // 		      *CTs_deep[iCTd]);
  // }

//  FITReader->Plot1Dstack(all,
//			 VS().a("1L"),
//			 VS().a("1j1b0svS"),
//			 VS().a("ge1jISR").a("ge1j0bISR").a("ge1jge1bISR"),
//			 "pre_stack");
//  
}
