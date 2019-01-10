// C++ includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <istream>
#include <stdio.h>
#include <dirent.h>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TList.h>

#include "ReducedNtuple.hh"

using namespace std;

/// Main function that runs the analysis algorithm on the
/// specified input files
int main(int argc, char* argv[]) {

  /// Gets the list of input files and chains
  /// them into a single TChain
  char inputFileName[400];
  char inputListName[400];
  char inputFolderName[400];
  char outputFileName[400];
  char outputFolderName[400];
  char TreeName[400];

  bool DO_FILE = false;
  bool DO_LIST = false;
  bool DO_FOLDER = false;
  bool DO_OFOLD = false;
  bool DO_TREE = false;

  if ( argc < 2 ){
    cout << "Error at Input: please specify an input file name, a list of input ROOT files and/or a folder path"; 
    cout << " and an output filename:" << endl; 
    cout << "  Example:      ./MakeReducedTree.x -ifile=input.root -ofile=output.root"  << endl;
    cout << "  Example:      ./MakeReducedTree.x -ilist=input.list -ofile=output.root"  << endl;
    cout << "  Example:      ./MakeReducedTree.x -ifold=folder_path -ofile=output.root" << endl;
    cout << "  Example:      ./MakeReducedTree.x -ifold=folder_path -ofold=folder_path" << endl;
    cout << "  Example:      ./MakeReducedTree.x -ifold=folder_path -ofold=folder_path -tree=treename" << endl;
    
    return 1;
  }
  for (int i=0;i<argc;i++){
    if (strncmp(argv[i],"-ifile",6)==0){
      sscanf(argv[i],"-ifile=%s",  inputFileName);
      DO_FILE = true;
    }
    if (strncmp(argv[i],"-ilist",6)==0){
      sscanf(argv[i],"-ilist=%s",  inputListName);
      DO_LIST = true;
    }
    if (strncmp(argv[i],"-ifold",6)==0){
      sscanf(argv[i],"-ifold=%s",  inputFolderName);
      DO_FOLDER = true;
    }
    if (strncmp(argv[i],"-ofold",6)==0){
      sscanf(argv[i],"-ofold=%s",  outputFolderName);
      DO_OFOLD = true;
    }
    if (strncmp(argv[i],"-tree",5)==0){
      sscanf(argv[i],"-tree=%s",  TreeName);
      DO_TREE = true;
    }
    if (strncmp(argv[i],"-ofile",6)==0)  sscanf(argv[i],"-ofile=%s",  outputFileName);
  }

  gROOT->ProcessLine("#include <vector>");

  vector<string> filenames;

  char Buffer[500];
  char MyRootFile[2000];  

  if(DO_FOLDER){
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(inputFolderName);
    if(dpdf == NULL){
      cout << "ERROR: " << inputFolderName << " is not a directory" << endl;
      return 1;
    }
    string dname(inputFolderName);
    while ((epdf = readdir(dpdf))){
      if(string(epdf->d_name).find(".root") == string::npos)
	continue;
      string name = dname+"/"+string(epdf->d_name);
      filenames.push_back(name);
    }
  }

  if(DO_LIST){
    ifstream *inputFile = new ifstream(inputListName);
    while( !(inputFile->eof()) ){
      inputFile->getline(Buffer,500);
      if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))){
	sscanf(Buffer,"%s",MyRootFile);
	filenames.push_back(MyRootFile);
      }
    }
    inputFile->close();
    delete inputFile;
  }

  if(DO_FILE){
    filenames.push_back(inputFileName);
  }
  int Nfile = filenames.size();
  for(int i = 0; i < Nfile; i++){

    //TTree* chain = (TTree*) f->Get("anaCHS/tree");
    
    TChain* chain;
    if(DO_TREE)
      chain = (TChain*) new TChain(TreeName);
    else
      chain = (TChain*) new TChain("stopTreeMaker/AUX");
    chain->Add(filenames[i].c_str());
    cout << "   Running file " << filenames[i] << endl;
    if(DO_TREE)
      cout << "   Running tree " << TreeName << " " << chain->GetEntries() << endl;
    else
      cout << "   Running tree " << "stopTreeMaker/AUX" << " " << chain->GetEntries() << endl;
    ReducedNtuple* ntuple = new ReducedNtuple(chain);
    
    // pass filename label
    string label = filenames[i];
    if(label.find(".root") != string::npos )
      label.erase(label.find(".root"));
    while(label.find("/") != string::npos)
      label.erase(0, label.find("/")+1);
    ntuple->AddLabel(label);

    if(label.find("_treeProducerSusyMultilepton_tree") != string::npos)
      label.erase(label.find("_treeProducerSusyMultilepton_tree"));

    cout << label << " label " << endl;
    
    //Get event count
    // TH1D* hevt = nullptr;
    // TFile* f = new TFile(filenames[i].c_str(),"READ");
    // hevt = (TH1D*) f->Get("SumGenWeights");
    // if(hevt){
    //   ntuple->AddNevent( hevt->Integral() );
    //   delete hevt;
    // }
    if(DO_OFOLD)
      ntuple->WriteNtuple(string(outputFolderName)+"/"+label+".root");
    else
      ntuple->WriteNtuple(string(outputFileName));
    delete ntuple;
  }
 
  return 0;
}
