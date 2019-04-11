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
   string          *dataset = new std::string();
   int             MP;
   int             MC;

   // List of branches
   TBranch        *b_Nevent;   //!
   TBranch        *b_Nweight;   //!
   TBranch        *b_dataset;   //!
   TBranch        *b_MP;   //!
   TBranch        *b_MC;   //!

   tree->SetMakeClass(1);
   tree->SetBranchAddress("Nevent", &Nevent, &b_Nevent);
   tree->SetBranchAddress("Nweight", &Nweight, &b_Nweight);
   tree->SetBranchAddress("dataset", &dataset, &b_dataset);
   tree->SetBranchAddress("MP", &MP, &b_MP);
   tree->SetBranchAddress("MC", &MC, &b_MC);

   vector<string> datasets;
   map<string,vector<pair<int,int> > >    mapMasses;
   map<string,map<pair<int,int>,double> > mapNevent;
   map<string,map<pair<int,int>,double> > mapNweight;
   
   for(int i = 0; i < N; i++){
     tree->GetEntry(i);
     pair<int,int> masses(MP,MC);
     if(mapMasses.count(*dataset) == 0){
       datasets.push_back(*dataset);
       mapMasses[*dataset] = vector<pair<int,int> >();
       mapNevent[*dataset] = map<pair<int,int>,double>();
       mapNweight[*dataset] = map<pair<int,int>,double>();
     }
     if(mapNevent[*dataset].count(masses) == 0){
       mapMasses[*dataset].push_back(masses);
       mapNevent[*dataset][masses] = 0.;
       mapNweight[*dataset][masses] = 0.;
     }
     mapNevent[*dataset][masses] += Nevent;
     mapNweight[*dataset][masses] += Nweight;
   }
   
   N = datasets.size();
   for(int i = 0; i < N; i++){
     //cout << "Label2Nevent[\"" << datasets[i] << "\"] = std::map<std::pair<int,int>,double>();" << endl;
     cout << "Label2Nweight[\"" << datasets[i] << "\"] = std::map<std::pair<int,int>,double>();" << endl;
     int M = mapMasses[datasets[i]].size();
     for(int j = 0; j < M; j++){
       pair<int,int> m = mapMasses[datasets[i]][j];
       //cout << "Label2Nevent[\"" << datasets[i] << "\"][std::pair<int,int>(" << m.first << "," << m.second << ")] = ";
       //cout << std::setprecision(12) << mapNevent[datasets[i]][m] << ";" << endl;
       cout << "Label2Nweight[\"" << datasets[i] << "\"][std::pair<int,int>(" << m.first << "," << m.second << ")] = ";
       cout << std::setprecision(12) << mapNweight[datasets[i]][m] << ";" << endl;
     }
     cout << endl;
   }
   
}
