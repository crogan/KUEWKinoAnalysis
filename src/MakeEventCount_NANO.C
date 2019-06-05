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
  char DataSet[400];
  char FileTag[400];

  bool DO_FILE = false;
  bool DO_LIST = false;
  bool DO_FOLDER = false;
  bool DO_TREE = false;
  bool DO_SMS = false;

  if ( argc < 2 ){
    cout << "Error at Input: please specify an input file name, a list of input ROOT files and/or a folder path"; 
    cout << " and an output filename:" << endl; 
    cout << "  Example:      ./MakeEventCount_NANO.x -ifile=input.root -ofile=output.root -dataset=dataset_name -filetag=sample_tag"  << endl;
    cout << "  Example:      ./MakeEventCount_NANO.x -ilist=input.list -ofile=output.root -dataset=dataset_name -filetag=sample_tag"  << endl;
    cout << "  Example:      ./MakeEventCount_NANO.x -ifold=folder_path -ofile=output.root -dataset=dataset_name -filetag=sample_tag -tree=treename --sms" << endl;
    
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
    if (strncmp(argv[i],"-ofile",6)==0) sscanf(argv[i],"-ofile=%s", outputFileName);
    if (strncmp(argv[i],"-dataset",8)==0)   sscanf(argv[i],"-dataset=%s", DataSet);
    if (strncmp(argv[i],"-filetag",8)==0)   sscanf(argv[i],"-filetag=%s", FileTag);
    if (strncmp(argv[i],"--sms",5)==0)  DO_SMS = true;
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
    chain = (TChain*) new TChain("Events");
  
  int Nfile = filenames.size();
  for(int i = 0; i < Nfile; i++){
    chain->Add(filenames[i].c_str());
    cout << "   Adding file " << filenames[i] << endl;
  }

  Float_t genWeight;
  TBranch *b_genWeight;

  UInt_t  nGenPart;
  Float_t GenPart_mass[200];  
  Int_t   GenPart_pdgId[200];
  TBranch *b_nGenPart;
  TBranch *b_GenPart_mass;
  TBranch *b_GenPart_pdgId;

  
  chain->SetBranchAddress("genWeight", &genWeight, &b_genWeight);
  chain->SetBranchAddress("nGenPart", &nGenPart, &b_nGenPart);
  chain->SetBranchAddress("GenPart_mass", GenPart_mass, &b_GenPart_mass);
  chain->SetBranchAddress("GenPart_pdgId", GenPart_pdgId, &b_GenPart_pdgId);
  chain->SetBranchStatus("*",0);
  chain->SetBranchStatus("genWeight", 1);
  chain->SetBranchStatus("nGenPart", 1);
  chain->SetBranchStatus("GenPart_mass", 1);
  chain->SetBranchStatus("GenPart_pdgId", 1);

  double Nevent = 0.;
  double Nweight = 0.;

  int MP = 0;
  int MC = 0;
  int PDGID;
  std::vector<std::pair<int,int> > masses;
  std::map<std::pair<int,int>,double > mapNevent;
  std::map<std::pair<int,int>,double > mapNweight;
   
  int NEVENT = chain->GetEntries();
  cout << "TOTAL of " << NEVENT << " entries" << endl;
  for(int e = 0; e < NEVENT; e++){
    cout << "event " << e << " | " << NEVENT << endl;
    chain->GetEntry(e);

    cout << "event " << e << " | " << NEVENT << endl;
  
    Nevent += 1.;
    Nweight += genWeight;

    if(DO_SMS){
      MP = 0;
      MC = 0;
      int Ngen = nGenPart;
      for(int i = 0; i < Ngen; i++){
	PDGID = fabs(GenPart_pdgId[i]);
	if(PDGID > 1000000 && PDGID < 3000000){
	  int mass = int(GenPart_mass[i]+0.5);
	  if(PDGID == 1000022)
	    MC = mass;
	  else
	    if(mass > MP)
	      MP = mass;
	}
      }
      std::pair<int,int> masspair(MP,MC);
      if(mapNevent.count(masspair) == 0){
	masses.push_back(masspair);
	mapNevent[masspair]    = 0.;
	mapNweight[masspair]   = 0.;
      }

      mapNevent[masspair]  += 1.;
      mapNweight[masspair] += genWeight;
    }
  }

  TFile* fout = new TFile(string(outputFileName).c_str(),"RECREATE");
  TTree* tout = (TTree*) new TTree("EventCount", "EventCount");
  
  string dataset = string(DataSet);
  string filetag = string(FileTag);
  tout->Branch("Nevent", &Nevent);
  tout->Branch("Nweight", &Nweight);
  tout->Branch("filetag", &filetag);
  tout->Branch("dataset", &dataset);
  tout->Branch("MP", &MP);
  tout->Branch("MC", &MC);
  if(DO_SMS){
    int Nmass = masses.size();
    for(int i = 0; i < Nmass; i++){
      Nevent     = mapNevent[masses[i]];
      Nweight    = mapNweight[masses[i]];
      MP = masses[i].first;
      MC = masses[i].second;
      tout->Fill();
    }
  } else {
    tout->Fill();
  }
 
  tout->Fill();

  fout->cd();
  tout->Write();
  fout->Close();
 
  return 0;
}
