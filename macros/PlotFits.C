#include "TSystem.h"

#include "../include/FitPlotter.hh"

//void PlotFits(const string& inputfile = "/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/BuildFits/BF_allBkgs_data_T2tt_0L1L_QCDShapesSJet0p20var_QCDnorms0p50_WJetsnorms0p20_otherBkgnorms0p20_maskSR_09_27_21/FitInput_KUEWKino_2017.root", const string& a2 = "/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/BuildFits/BF_allBkgs_data_T2tt_0L1L_QCDShapesSJet0p20var_QCDnorms0p50_WJetsnorms0p20_otherBkgnorms0p20_maskSR_09_27_21/all/T2tt/6000425/fitDiagnostics09_24_21wShapes.root", const string& a3 = "shapes_fit_b"){
void PlotFits(const string& fold1 = "BF_allBkgs_data_TChiWZ_2016_2L3L_FakesShapes0p05_sJetSplitprocsTogether/", const string& fold2 = "all/TChiWZ/4000350", const string& shapesFile = "11_11_21wShapes.root"){
  
  string dateName = shapesFile.substr(0,8);
        string bfName = fold1.substr(2,fold1.size());
        string odir = "prePostStackPlots/prePostStackPlots"+bfName;

        string inputfile1 = "BuildFits/"+fold1+"/FitInput_KUEWKino_2017.root";
        string inputfile2 = "BuildFits/"+fold1+"/"+fold2+"/fitDiagnostics"+shapesFile;
        string lepName;
        if(fold2.find("Ch") != string::npos) lepName = fold2.substr(0,4);
        else lepName = "all";
        string fname = odir+"/"+lepName+"/"+shapesFile;
cout << "input file: " << inputfile2 << endl;
cout << "out directory: " << odir << "/" << lepName << "/" << endl;
  
 if(gSystem->AccessPathName(inputfile2.c_str())){ cout << "file " << inputfile2 << " not found" << endl; return; }
        FitPlotter* FITPlotter_pre = new FitPlotter(inputfile1, inputfile2, "shapes_prefit");
        FitPlotter* FITPlotter_bOnly = new FitPlotter(inputfile1, inputfile2, "shapes_fit_b");
        FitPlotter* FITPlotter_sb = new FitPlotter(inputfile1, inputfile2, "shapes_fit_s");



//  FitPlotter* FITReader = new FitPlotter(inputfile, a2, a3);
  bool ratio = true;

  CategoryTreeTool CTTool;

  CategoryTree CT_0L = CTTool.GetCategories_0L();
  CategoryTree CT_1L = CTTool.GetCategories_1L();
  CategoryTree CT_2L = CTTool.GetCategories_2L();
  CategoryTree CT_3L = CTTool.GetCategories_3L();

  VS all;
  all.a("ttbar_all").a("ST_all").a("DB_all").a("ZDY_all").a("TB_all").a("QCD_all").a("Wjets_all").a("Data");
  //all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes").a("Data");
  string sig;
 // sig = "T2tt_6000425";
  sig = "TChiWZ_3000250";
  all += sig; 

  bool zeroL = false;
  bool oneL = false;
  bool twoL = true;
  bool threeL = false; 
  vector<const CategoryTree*> CTs;
  int depth0, d;
  if(zeroL){
   depth0 = CT_0L.GetDepth();
   d = depth0;    
   CT_0L.GetListDepth(CTs, depth0-3);
  }
  else if(oneL){
   depth0 = CT_1L.GetDepth()-3;
   d = depth0;
   CT_1L.GetListDepth(CTs, 1);
  }
  else if(twoL){
   depth0 = CT_2L.GetDepth();
   d = 4;
   CT_2L.GetListDepth(CTs, 0);
   }
  else if(threeL){
   depth0 = CT_3L.GetDepth();
   d = 3;
   CT_3L.GetListDepth(CTs, depth0-2);
  }
cout << depth0 << " " << CTs.size() << endl;
 if(gSystem->AccessPathName((fname).c_str())){
                gSystem->Exec(("mkdir "+odir).c_str());
                gSystem->Exec(("mkdir "+odir+"/"+lepName).c_str());
        }
        TFile* file = new TFile(fname.c_str(),"RECREATE");
        cout << "Writing to file: " << fname << endl;
        file->cd();
  for(int i = 0; i < CTs.size(); i++){
    string dir = CTs[i]->GetPlainLabel(depth0-d);
while(dir.find(" ") != string::npos) dir.replace(dir.find(" "),1,"_"); 
    cout << dir  << endl; 
if(dir.find("0J") != string::npos) continue; 
  cout << "##############plot prefit#############" << endl;
    TCanvas* prefit_stack = FITPlotter_pre->Plot1Dstack(Form("pre_stack_%d",i),all,*CTs[i],ratio);
    if(prefit_stack == nullptr) continue;
    cout << "##############plot b-fit#############" << endl;
    TCanvas* b_fit_stack = FITPlotter_bOnly->Plot1Dstack(Form("bFit_stack_%d",i),all,*CTs[i],ratio);
    if(b_fit_stack == nullptr) continue;
cout << "writing plots to file" << endl;
    if(!file->GetDirectory(dir.c_str()))
            file->mkdir(dir.c_str());
    file->cd(dir.c_str());
    prefit_stack->Write("prefit_stack",TObject::kOverwrite);
    b_fit_stack->Write("bfit_stack",TObject::kOverwrite);
 cout << "saving plots as pdf" << endl;
   if(all[0].find("all") != std::string::npos){
   prefit_stack->SaveAs((odir+"/"+lepName+"/"+dir+sig+"_fakesIncl_prefit.pdf").c_str());
   b_fit_stack->SaveAs((odir+"/"+lepName+"/"+dir+sig+"_fakesIncl_b-fit.pdf").c_str());
   }
   else{
   prefit_stack->SaveAs((odir+"/"+lepName+"/"+dir+sig+"prefit.pdf").c_str());
   b_fit_stack->SaveAs((odir+"/"+lepName+"/"+dir+sig+"b-fit.pdf").c_str());
   }
   prefit_stack->Close();
   b_fit_stack->Close();


   file->cd();
   cout << "\n" << endl;
 }

}
