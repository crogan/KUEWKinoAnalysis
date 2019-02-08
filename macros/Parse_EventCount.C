#include <iostream>
#include <map>
#include <string>
#include "TFile.h"
#include "TTree.h"

void Parse_EventCount(string filename){

  TFile* f = new TFile(filename.c_str(),"READ");
  TTree* tree = (TTree*) f->Get("EventCount");

  int N = tree->GetEntries();
  
  // Declaration of leaf types
   Double_t        Nevent;
   Double_t        Nweight;
   Double_t        Nabsweight;
   string          *dataset = new std::string();

   // List of branches
   TBranch        *b_Nevent;   //!
   TBranch        *b_Nweight;   //!
   TBranch        *b_Nabsweight;   //!
   TBranch        *b_dataset;   //!

   tree->SetMakeClass(1);
   tree->SetBranchAddress("Nevent", &Nevent, &b_Nevent);
   tree->SetBranchAddress("Nweight", &Nweight, &b_Nweight);
   tree->SetBranchAddress("Nabsweight", &Nabsweight, &b_Nabsweight);
   tree->SetBranchAddress("dataset", &dataset, &b_dataset);

   vector<string> datasets;
   map<string,double> mapNevent;

   for(int i = 0; i < N; i++){
     tree->GetEntry(i);
     if(mapNevent.count(*dataset) == 0){
       mapNevent[*dataset] = Nevent;
       datasets.push_back(*dataset);
     } else {
       mapNevent[*dataset] += Nevent;
     }
   }
   
   N = datasets.size();
   for(int i = 0; i < N; i++){
     cout << datasets[i] << " " << mapNevent[datasets[i]] << endl;
   }

}
