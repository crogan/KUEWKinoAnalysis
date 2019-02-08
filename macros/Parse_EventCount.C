#include <iostream>
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

   cout << "HERE" << endl;
   tree->SetMakeClass(1);
   tree->SetBranchAddress("Nevent", &Nevent, &b_Nevent);
   tree->SetBranchAddress("Nweight", &Nweight, &b_Nweight);
   tree->SetBranchAddress("Nabsweight", &Nabsweight, &b_Nabsweight);
   tree->SetBranchAddress("dataset", &dataset, &b_dataset);

   cout << Nevent << endl;
   tree->GetEntry(0);
   cout << Nevent << endl;
   cout << *dataset << endl;

}
