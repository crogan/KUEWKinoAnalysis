#include "TSystem.h"

#include "../include/FitPlotter.hh"

void PlotFits(const string& inputfile = "test/FitInput_test.root", const string& fitfile = "", const string& fitfold = ""){

  FitPlotter* FITReader = new FitPlotter(inputfile, fitfile, fitfold);

  CategoryTreeTool CTTool;

  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();

  VS all;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("Data");
  
  int depth0 = CT_0L.GetDepth();
  vector<const CategoryTree*> CTs;
  CT_0L.GetListDepth(CTs, depth0-3);

  vector<const CategoryTree*> CTs_deep;
  CT_0L.GetListDepth(CTs_deep, depth0-3);
  
  // int depth0 = CT_1L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_1L.GetListDepth(CTs, depth0-1);

  // vector<const CategoryTree*> CTs_deep;
  // CT_1L.GetListDepth(CTs_deep, depth0-2);

  // int depth0 = CT_2L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_2L.GetListDepth(CTs, depth0-2);

  // vector<const CategoryTree*> CTs_deep;
  // CT_2L.GetListDepth(CTs_deep, depth0-2);

  // int depth0 = CT_3L.GetDepth();
  // vector<const CategoryTree*> CTs;
  // CT_3L.GetListDepth(CTs, depth0-2);

  // vector<const CategoryTree*> CTs_deep;
  // CT_3L.GetListDepth(CTs_deep, depth0-2);

  for(int i = 0; i < CTs.size(); i++){
    FITReader->Plot1Dstack(Form("hstack_%d", i), all, *CTs[i], true);
  }
  
}
