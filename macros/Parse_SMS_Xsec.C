#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>
#include "TFile.h"
#include "TTree.h"

void Parse_SMS_Xsec(const string& grid, const string& component){
  TFile* f = new TFile(Form("root/%s_%s_13TeV.root",grid.c_str(),component.c_str()),"READ");
  if(!f->IsOpen())
    return;

  cout << "Reading file " << Form("root/%s_%s_13TeV.root",grid.c_str(),component.c_str()) << endl;
  
  TTree* tree = nullptr;
  tree = (TTree*) f->Get("parameters");
  if(tree == nullptr)
    return;
  
  std::vector<double>* masses = 0;
  std::vector<double>* xsecs = 0;
  std::vector<double>* xsecUncs = 0;

  tree->SetBranchAddress("mass",&masses);
  tree->SetBranchAddress("xsec",&xsecs);
  tree->SetBranchAddress("xsecUnc",&xsecUncs);
  tree->GetEntry(0);

  string label = Form("%s_%s",grid.c_str(),component.c_str());
  
  int N = masses->size();
  cout << "N_SMS[\"" << label << "\"] = " << N << ";" << endl << endl;
  /// cout << "Label2Mass[\"" << label << "\"] = std::vector<double>();" << endl;
  /// cout << "Label2Xsec[\"" << label << "\"] = std::vector<double>();" << endl;
  cout << "Label2XsecUnc[\"" << label << "\"] = std::vector<double>();" << endl;
  for(int i = 0; i < N; i++){
    // cout << "Label2Mass[\"" << label << "\"].push_back(" << masses->at(i) << ");" << endl;
    // cout << "Label2Xsec[\"" << label << "\"].push_back(" << xsecs->at(i) << ");" << endl;
    cout << "Label2XsecUnc[\"" << label << "\"].push_back(" << xsecUncs->at(i) << ");" << endl;
  }

}
