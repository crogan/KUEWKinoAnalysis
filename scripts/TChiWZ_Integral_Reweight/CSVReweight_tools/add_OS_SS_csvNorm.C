

//zdatatrigtest_201.root
//
#include<string>
#include "TFile.h"
#include "TTree.h"
#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>


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



std::vector<std::vector<std::string>> readCSV(std::string csvfile) {
    std::ifstream file(csvfile);
    std::vector<std::vector<std::string>> data;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> row;
        std::string field;
        //std::cout<<"getline1 "<<line<<"\n";

        while (std::getline(ss, field, ' ')) {
            row.push_back(field);
        }

        data.push_back(row);
    }

    // Process the data as needed
    // ...

    return data;
}

void printCSV( std::vector<std::vector<std::string>> data ){
	
	for(int i = 0; i<data.size(); i++){
		for(int j=0; j<data.at(i).size(); j++){
			std::cout<<data.at(i).at(j)<<" ";
		}
		std::cout<<"\n";
	}

}
void printDict(std::map<std::pair<int,int>,std::pair<double,double> > dict ){
	for (auto& t : dict)
    std::cout << t.first.first << " "
    		  << t.first.second << " " 
              << t.second.first << " " 
              << t.second.second << "\n";

}
//dictionary is {(mN2,mN1): (constPlus, constMinus)}
//constPlus=SS constMinus=OS  SS is first, OS is second
std::map<std::pair<int,int>,std::pair<double,double> > buildDict( std::vector<std::vector<std::string>> data ){
	
	std::map<std::pair<int,int>,std::pair<double,double> > dict;
	
	std::pair<int,int> key;
	std::pair<double,double> value;
	for(int i =1; i<data.size(); i++){
		key.first=std::stoi(data.at(i).at(0));
		key.second=std::stoi(data.at(i).at(1));
		value.first=std::stod(data.at(i).at(2));
		value.second=std::stod(data.at(i).at(4));
		dict[key]=value;
	}
	
	return dict;
	
}

int main(int argc, char *argv[]){
	
	std::vector<std::vector<std::string>> data;
	data = readCSV("TChiWZgridWeights-V1-S13579.dat");
	printCSV(data);
	std::map<std::pair<int,int>,std::pair<double,double> > dict;
	dict = buildDict(data);
	printDict(dict);
	
	std::cout<<"dict printed"<<"\n";
	

    std::string iFileName = std::string(argv[1]);

    TFile *iFile = TFile::Open(iFileName.c_str(),"UPDATE");
    
    std::cout<<"Input file loaded with updated \n";
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
 
  //std::vector<double> wts;
  std::pair<int,int> dictKey;
  double SSwt,OSwt;
  for(int i=0; i< keynames.size(); i++){
  	TTree &t = *(TTree *) iFile->Get(keynames[i].c_str());
    
    //split keyname to get mn2 and mn1
    std::cout<<"AT KEYNAME: "<<keynames[i]<<"\n";
    std::vector<std::string> substrings;
    std::stringstream ss(keynames[i]);
    std::string substring;
    while (std::getline(ss, substring, '_')) {
        substrings.push_back(substring);
    }

    // Print the substrings
    for(int i=0; i<substrings.size(); i++){
    	std::cout<<substrings.at(i)<<" ";
    }
  	std::cout<<"\n";
  	//wts = loopTree(t);
   
    dictKey.first = std::stoi(substrings.at(1));
    dictKey.second = std::stoi(substrings.at(2));
  
  	SSwt = dict[dictKey].first;
  	OSwt = dict[dictKey].second;
  	
  	std::cout<<SSwt<<" "<<OSwt<<"\n";
   TTree* tOut = (TTree *) iFile->Get(keynames[i].c_str());
  // addbranch(ttree , OS, SS)
   addBranch(tOut, 1./dict[dictKey].second, 1./dict[dictKey].first);
    tOut->Write("", TObject::kOverwrite);

  	
  }
  iFile->Close();



}
