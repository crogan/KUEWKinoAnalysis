#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include "TFile.h"
#include "TTree.h"

void Parse_EventCount_SMS(string filename){

  TFile* f = new TFile(filename.c_str(),"READ");
  TTree* tree = (TTree*) f->Get("EventCount");

  int N = tree->GetEntries();
  
  // Declaration of leaf types
   Double_t        Nevent;
   Double_t        Nweight;
   Double_t        Nabsweight;
   string          *dataset = new std::string();
   int             MP;
   int             MC;

   // List of branches
   TBranch        *b_Nevent;   //!
   TBranch        *b_Nweight;   //!
   TBranch        *b_Nabsweight;   //!
   TBranch        *b_dataset;   //!
   TBranch        *b_MP;   //!
   TBranch        *b_MC;   //!

   tree->SetMakeClass(1);
   tree->SetBranchAddress("Nevent", &Nevent, &b_Nevent);
   tree->SetBranchAddress("Nweight", &Nweight, &b_Nweight);
   tree->SetBranchAddress("Nabsweight", &Nabsweight, &b_Nabsweight);
   tree->SetBranchAddress("dataset", &dataset, &b_dataset);
   tree->SetBranchAddress("MP", &MP, &b_MP);
   tree->SetBranchAddress("MC", &MC, &b_MC);

   vector<string> datasets;
   map<string,double> mapNevent;
   map<string,double> mapNweight;
   map<string,double> mapNabsweight;

   for(int i = 0; i < N; i++){
     tree->GetEntry(i);
     if(mapNevent.count(*dataset) == 0){
       mapNevent[*dataset] = Nevent;
       mapNweight[*dataset] = Nweight;
       mapNabsweight[*dataset] = Nabsweight;
       datasets.push_back(*dataset);
     } else {
       mapNevent[*dataset] += Nevent;
       mapNweight[*dataset] += Nweight;
       mapNabsweight[*dataset] += Nabsweight;
     }
   }
   
   N = datasets.size();
   for(int i = 0; i < N; i++){
     cout << "Label2Nevent[\"" << datasets[i] << "\"] = " << std::setprecision(12) << mapNevent[datasets[i]] << ";" << endl;
     cout << "Label2Nweight[\"" << datasets[i] << "\"] = " << std::setprecision(12) << mapNweight[datasets[i]] << ";" << endl;
     cout << "Label2Nabsweight[\"" << datasets[i] << "\"] = " << std::setprecision(12) << mapNabsweight[datasets[i]] << ";" << endl;
     cout << endl;
   }

   if(!xsec_template)
     return;

   for(int i = 0; i < N; i++){
     cout << "Label2Xsec[\"" << datasets[i] << "\"] = 1;" << endl;
     //cout << endl;
   }
   
}
