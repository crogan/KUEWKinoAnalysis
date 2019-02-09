// C++ includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <istream>
#include <stdio.h>
#include <dirent.h>
#include <vector>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TList.h>
#include <TLeafElement.h>
#include <TLorentzVector.h>

using namespace std;
using std::vector;

/// Main function that runs the analysis algorithm on the
/// specified input files
int main(int argc, char* argv[]) {

  /// Gets the list of input files and chains
  /// them into a single TChain
  char inputFileName[400];
  char inputListName[400];
  char inputFolderName[400];
  char outputFileName[400];
  char TreeName[400];
  char FileTag[400];

  bool DO_FILE = false;
  bool DO_LIST = false;
  bool DO_FOLDER = false;
  bool DO_TREE = false;

  if ( argc < 2 ){
    cout << "Error at Input: please specify an input file name, a list of input ROOT files and/or a folder path"; 
    cout << " and an output filename:" << endl; 
    cout << "  Example:      ./MakeEventCount.x -ifile=input.root -ofile=output.root -tag=sample_tag"  << endl;
    cout << "  Example:      ./MakeEventCount.x -ilist=input.list -ofile=output.root -tag=sample_tag"  << endl;
    cout << "  Example:      ./MakeEventCount.x -ifold=folder_path -ofile=output.root -tag=sample_tag -tree=treename" << endl;
    
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
    if (strncmp(argv[i],"-tree",5)==0){
      sscanf(argv[i],"-tree=%s",  TreeName);
      DO_TREE = true;
    }
    if (strncmp(argv[i],"-ofile",6)==0)  sscanf(argv[i],"-ofile=%s", outputFileName);
    if (strncmp(argv[i],"-tag",4)==0)  sscanf(argv[i],"-tag=%s", FileTag);
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

  TChain* chain;
  if(DO_TREE)
    chain = (TChain*) new TChain(TreeName);
  else
    chain = (TChain*) new TChain("stopTreeMaker/AUX");
  
  int Nfile = filenames.size();
  for(int i = 0; i < Nfile; i++){
    chain->Add(filenames[i].c_str());
    cout << "   Adding file " << filenames[i] << endl;
  }

  int NEVENT = chain->GetEntries();
  float        stored_weight_f;
  float        evtWeight_f;
  double        stored_weight_d;
  double        evtWeight_d;
  TBranch        *b_stored_weight;   //!
  TBranch        *b_evtWeight;   //!

  vector<int>     *genDecayPdgIdVec;
  TBranch        *b_genDecayPdgIdVec;   //!
  vector<TLorentzVector> *genDecayLVec;
  TBranch        *b_genDecayLVec;   //!

  genDecayPdgIdVec = 0;
  genDecayLVec = 0;
  
  chain->SetMakeClass(1);
  bool is_float = string(chain->GetBranch("evtWeight")->GetLeaf("evtWeight")->GetTypeName()) == "Float_t";
  if(is_float){
    chain->SetBranchAddress("stored_weight", &stored_weight_f, &b_stored_weight);
    chain->SetBranchAddress("evtWeight", &evtWeight_f, &b_evtWeight);
  } else {
    chain->SetBranchAddress("stored_weight", &stored_weight_d, &b_stored_weight);
    chain->SetBranchAddress("evtWeight", &evtWeight_d, &b_evtWeight);
  }
  
  chain->SetBranchAddress("genDecayPdgIdVec", &genDecayPdgIdVec, &b_genDecayPdgIdVec);
  chain->SetBranchAddress("genDecayLVec", &genDecayLVec, &b_genDecayLVec);

  chain->SetBranchStatus("*",0);
  chain->SetBranchStatus("stored_weight",1);
  chain->SetBranchStatus("evtWeight",1);
  chain->SetBranchStatus("genDecayPdgIdVec",1);
  chain->SetBranchStatus("genDecayLVec",1);
  
  
  TFile* fout = new TFile(string(outputFileName).c_str(),"RECREATE");
  TTree* tout = (TTree*) new TTree("EventCount", "EventCount");
  
  double Nevent = 0.;
  double Nweight = 0.;
  double Nabsweight = 0.;
  string dataset = string(FileTag);
  
  tout->Branch("Nevent", &Nevent);
  tout->Branch("Nweight", &Nweight);
  tout->Branch("Nabsweight", &Nabsweight);
  tout->Branch("dataset", &dataset);
  
  for(int e = 0; e < NEVENT; e++){
    if(e%(std::max(1,NEVENT/100)) == 0)
      std::cout << "Processing event " << e << " | " << NEVENT << endl;
    chain->GetEntry(e);
    Nevent += 1.;
    if(is_float){
      Nweight += evtWeight_f;
      Nabsweight += fabs(evtWeight_f);
    } else {
      Nweight += evtWeight_d;
      Nabsweight += fabs(evtWeight_d);
    }

    int Ngen = genDecayPdgIdVec->size();
    for(int i = 0; i < Ngen; i++){
      if(fabs(genDecayPdgIdVec->at(i)) > 1000000 && fabs(genDecayPdgIdVec->at(i)) < 3000000){
	cout << genDecayPdgIdVec->at(i) << " " << genDecayLVec->at(i).M() << endl;
      }
    }
    
    
  }

  tout->Fill();
  fout->cd();
  tout->Write();
  fout->Close();
 
  return 0;
}
