#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include "TFile.h"
#include "TTree.h"

void DO_FILE(string filename, string filetag);
// example syntax: root -l 'macros/MakeFilterEff.C++("test_list.txt","Fall17_102X")' // yes '_SMS' is not used here
// where list is list of filter eff text files
//     ls ../ListMaker/GeneratorInterface/Core/test/Fall17_102X_SMS/** > test_list.txt

void MakeFilterEff(string listname, string filetag){
  string line;
  ifstream ifile(listname.c_str());

  if(ifile.is_open()){
    while(getline(ifile,line)){
      DO_FILE(line, filetag);
    }
    ifile.close();
  }
  
}

void DO_FILE(string filename, string filetag){

  string dataset = filename;
  int lumiblock;
  double efficiency;

  size_t found;
  while(dataset.find("/") != std::string::npos){
    found = dataset.find("/");
    dataset.erase(0,found+1);
  }
  while(dataset.find(".") != std::string::npos){
    found = dataset.find(".");
    dataset.erase(found);
  }

  
  TFile* fout = new TFile((dataset+"_"+filetag+".root").c_str(),"RECREATE");
  TTree* tout = (TTree*) new TTree("FilterEff", "FilterEff");
  
  tout->Branch("efficiency", &efficiency);
  tout->Branch("lumiblock", &lumiblock);
  tout->Branch("filetag", &filetag);
  tout->Branch("dataset", &dataset);

  
  string line;
  ifstream ifile(filename.c_str());

  int dum[3];
  char eff[20];
  if(ifile.is_open()){

    while(getline(ifile,line)){
      sscanf(line.c_str(),"%d,%s,%d,%d,%d", &lumiblock, eff, &dum[0],&dum[1],&dum[2] );
      efficiency = std::atof(eff);
      if(efficiency < 0.)
	efficiency = 1.;
      // cout << line << endl;
      // cout << lumiblock << " " << efficiency << " " << dum[0] << " " << dum[1] << " " << dum[2] << endl << endl;
      tout->Fill();
      
    }
    ifile.close();
  }

  fout->cd();
  tout->Write("",TObject::kOverwrite);
  fout->Close();
  
   
}
