#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "../src/XsecTool.cc"

void CheckEventCount(string filename){

  XsecTool XS;
  
  TFile* f = new TFile(filename.c_str(),"READ");
  TTree* tree = (TTree*) f->Get("EventCount");

  int N = tree->GetEntries();
  
  // Declaration of leaf types
   Double_t        Nevent;
   Double_t        Nweight;
   string          *dataset = new std::string();
   string          *filetag = new std::string();

   // List of branches
   TBranch        *b_Nevent;   //!
   TBranch        *b_Nweight;   //!
   TBranch        *b_dataset;   //!
   TBranch        *b_filetag;   //!
   

   tree->SetMakeClass(1);
   tree->SetBranchAddress("Nevent", &Nevent, &b_Nevent);
   tree->SetBranchAddress("Nweight", &Nweight, &b_Nweight);
   tree->SetBranchAddress("dataset", &dataset, &b_dataset);
   tree->SetBranchAddress("filetag", &filetag, &b_filetag);

   vector<string> datasets;
   map<string,double> mapNevent;
   map<string,double> mapNweight;

   for(int i = 0; i < N; i++){
     tree->GetEntry(i);

     if((*filetag).length() < 5)
       cout << *dataset << " " << *filetag << endl;
     
     if(mapNevent.count(*dataset) == 0){
       mapNevent[*dataset] = Nevent;
       mapNweight[*dataset] = Nweight;
       datasets.push_back(*dataset);
     } else {
       mapNevent[*dataset] += Nevent;
       mapNweight[*dataset] += Nweight;
     }
   }
   
   N = datasets.size();
   for(int i = 0; i < N; i++){
     double xsec = XS.GetXsec_BKG(datasets[i]);
     if((xsec <= 0. && datasets[i].find("SMS") == string::npos))
       cout << datasets[i] << endl;
     //cout << XS.GetXsec_SMS(*dataset, 500) << endl;
   }
   
}
