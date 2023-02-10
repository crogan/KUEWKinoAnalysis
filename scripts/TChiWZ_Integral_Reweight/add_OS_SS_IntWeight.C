

//zdatatrigtest_201.root
//
#include<string>
#include "TFile.h"
#include "TTree.h"
#include <iostream>

std::vector<double> loopTree(TTree& t){
//void loopTree(TTree& t, TTree& tOut){  
	double wt_OS,wt_SS;
		
	t.SetBranchAddress("wt_OS", &wt_OS);
	t.SetBranchAddress("wt_SS", &wt_SS);
	
	//weight integral is sum of weights?
	double OSintegral=0.0;
	double SSintegral=0.0;
	double Defintegral = t.GetEntries();
	for(int entry=0;entry < t.GetEntries(); entry++){
		t.GetEntry(entry);
		OSintegral+=wt_OS;
		SSintegral+=wt_SS;	
	}
	
	std::cout<<"Integrals "<<Defintegral<<" "<<OSintegral<<" "<<SSintegral<<"\n";
	
	double wt_int_SS, wt_int_OS;
	wt_int_SS = Defintegral/SSintegral;
	wt_int_OS = Defintegral/OSintegral;
	
	std::vector<double> wts = {wt_int_OS, wt_int_SS};
	return wts;
}

/* this is the old way, new stuff on official docs
void addToClone(TTree& tOut, double wt_OS, double wt_SS){
    double wt_int_OS=wt_OS;
  	double wt_int_SS=wt_SS;
  	tOut.Branch("wt_int_OS", &wt_int_OS);
  	tOut.Branch("wt_int_SS", &wt_int_SS);
  	for(int entry=0; entry <tOut.GetEntries(); entry++){
  		tOut.GetEntry(entry);
  		tOut.Fill();
  	}
  	
}*/
void addBranch(TTree* tOut, double wt_OS, double wt_SS) {

    auto b_wt_int_OS = tOut->Branch("wt_int_OS", &wt_OS);
    auto b_wt_int_SS = tOut->Branch("wt_int_SS", &wt_SS);
 
 
    Long64_t nentries = tOut->GetEntries(); // read the number of entries in the t3
 
    for (Long64_t i = 0; i < nentries; i++) {
        b_wt_int_OS->Fill();
        b_wt_int_SS->Fill();
    }
 
    // save only the new version of the tree
}

int main(int argc, char *argv[]){


    std::string iFileName = std::string(argv[1]);

    TFile *iFile = TFile::Open(iFileName.c_str(),"UPDATE");
    
    //get the list of Tree names, save onlly SMS trees
    TList* keylist = iFile->GetListOfKeys();
    keylist->Print();
    
    std::vector<std::string> keynames;
    for(int i=0; i< keylist->GetSize(); i++){
    	 std::string keyname = keylist->At(i)->GetName();
    	 size_t found = keyname.find("SMS");
         if (found != std::string::npos)
         	keynames.push_back(keyname);
    }
    
   //debugging keynames 
  //  for(int i=0; i< keynames.size(); i++){
  //  	std::cout<<keynames[i]<<" ";
  //  }
    
   
  //Loop over keynames and open each tree
  //make 3 mff distrbutions, find factor to normalize OS and SS weighted dists to match default integral
 
  //testing with 350 340
 // TTree &t = *(TTree *) iFile->Get("SMS_350_340");
 // std::vector<double> wts;
 // wts = loopTree(t);
 // std::cout<<"wts OS,SS "<<wts[0]<<" "<<wts[1]<<"\n";
 
  //TFile* oFile = new TFile(oFileName.c_str(),"RECREATE");
  
  std::vector<double> wts;
  for(int i=0; i< keynames.size(); i++){
  	TTree &t = *(TTree *) iFile->Get(keynames[i].c_str());
  	//TTree &tOut = *(TTree *) t.Clone();//*(t.CloneTree(0));
  	//TTree* tOut = t.CloneTree();
  	wts = loopTree(t);
    //cant do this in the cool way with old shitty version of ROOT
  //  auto tOut = iFile->Get<TTree>(keynames[i].c_str());
   TTree* tOut = (TTree *) iFile->Get(keynames[i].c_str());
    addBranch(tOut, wts[0], wts[1]);
    tOut->Write("", TObject::kOverwrite);
   // addToClone(tOut, wts[0], wts[1]);
  	//tOut->AutoSave();
  	//tOut->Write();
  	
  }
  iFile->Close();
  //oFile->Close();


}
