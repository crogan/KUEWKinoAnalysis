#include "TSystem.h"

#include "../include/FitPlotter.hh"

//void myPlotFits(const string& inputfile = "", const string& a2 = "", const string& a3 = ""){
void myPlotFits(){
//is it BF input?
//const string& inputfile = "/home/t3-ku/janguian/CMSSW_10_6_1/src/Experimental_Ewkino_Builds/BTest_Files/BF_BT-4_TChiWZ17/FitInput_KUEWKino_2017.root";
//or is it BFI input?
const string& inputfile = "/home/t3-ku/janguian/CMSSW_10_6_1/src/Experimental_Ewkino_Builds/BTest_Files/BFI_BT-4_TChiWZ17/BFS_BT-4_TChiWZ17.root";
//input fit diagnostic file
const string& a2 = "/home/t3-ku/janguian/CMSSW_10_6_1/src/Experimental_Ewkino_Builds/BTest_Files/BF_BT-4_TChiWZ17/FitDiagnostic/fitDiagnostics.Test.root";
const string& a3 = "shapes_fit_b";

  FitPlotter* FITReader = new FitPlotter(inputfile, a2, a3);

  CategoryTreeTool CTTool;

  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();

  VS all;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("Data");
  
//  int depth0 = CT_0L.GetDepth();
//  vector<const CategoryTree*> CTs;
//  CT_0L.GetListDepth(CTs, depth0-3);

//  vector<const CategoryTree*> CTs_deep;
//  CT_0L.GetListDepth(CTs_deep, depth0-3);
  
  // int depth0 = CT_1L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_1L.GetListDepth(CTs, depth0-1);

  // vector<const CategoryTree*> CTs_deep;
  // CT_1L.GetListDepth(CTs_deep, depth0-2);

   int depth0 = CT_2L.GetDepth();
   std::cout<<"depth0 "<<depth0<<"\n";
   vector<const CategoryTree*> CTs;
   CT_2L.GetListDepth(CTs, depth0-3);
  // for(int i=0; i<CTs.size(); i++){
	CTs[0]->Print();
//	std::cout<<"\n";
  // }

  // vector<const CategoryTree*> CTs_deep;
  // CT_2L.GetListDepth(CTs_deep, depth0-2);

  // int depth0 = CT_3L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_3L.GetListDepth(CTs, depth0-2);

  // vector<const CategoryTree*> CTs_deep;
  // CT_3L.GetListDepth(CTs_deep, depth0-2);

//  for(int i = 0; i < CTs.size(); i++){
int i = 0;
    FITReader->Plot1Dstack(Form("hstack_%d", i),
			   all,
			   *CTs[0], true);
  //}
  
}
