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
//#include <TIter.h>
#include <TKey.h>


#include "shapeAnalyzer.hh"


using namespace std;

int main(int argc, char* argv[]){
	std::vector<string> fakesVec;
	std::vector<string> procVec;
	std::vector<string> sysVec;
	string oFileName;
	string inFileName;
	bool hprint = false;
	// string upFile;
	// string downFile;
	// bool fUp = false;
	// bool fDown = false;
	procVec.push_back("");
	// vector<TFile*> files;
	char buffer[100];
	char charFake[] = {"Fakes_"};
	gStyle->SetOptStat(0);


	for(int i = 0; i < argc; i++){
		if(strncmp(argv[i],"--help", 6) == 0){
			hprint = true;
		}
		if(strncmp(argv[i],"-h", 2) == 0){
			hprint = true;
		}
		if(strncmp(argv[i],"-o",2) == 0){
			i++;
			oFileName = string(argv[i]);
		}
		if(strncmp(argv[i],"--output", 8) == 0){
			i++;
			oFileName = string(argv[i]);
	    }
		if(strncmp(argv[i],"-i",2) == 0){
			i++;
			inFileName = string(argv[i]);
	//		files.push_back(TFile::Open(inFileName));
		}
	

 		if(strncmp(argv[i],"--input",7) == 0){
                        i++;
                        inFileName = string(argv[i]);
        //              files.push_back(TFile::Open(inFileName));
          	 }


		if(strncmp(argv[i],"+fake",5) == 0){
			i++;
			strncpy(buffer,charFake,sizeof(buffer));
			strncat(buffer,argv[i],sizeof(buffer));
			fakesVec.push_back(buffer);
			memset(buffer,0,sizeof(buffer));
			//fakesVec.push_back(strncat("Fakes_",argv[i],10));
		}
		if(strncmp(argv[i],"++AllFakes",10) == 0){
			i++;
			fakesVec = {"Fakes_elf0","Fakes_elf1","Fakes_elf2","Fakes_muf0","Fakes_muf1","Fakes_muf2"};
		}
		if(strncmp(argv[i],"+proc",5) == 0){
			i++;
			procVec.push_back(argv[i]);
		}
		if(strncmp(argv[i],"+sys",4) == 0){
			i++;
			sysVec.push_back(argv[i]);
		}

		// if(strncmp(argv[i],"+upFile",7) == 0){
		// 	i++;
		// 	upFile = string(argv[i]);
		// 	fUp = true;
		// 	files.push_back(TFile::Open(upFile));
		// }
		// if(strncmp(argv[i],"+downFile",9) == 0){
		// 	i++;
		// 	downFile = string(argv[i]);
		// 	fDown = true;
		// 	files.push_back(TFile::Open(downFile));
		// }
	}
	
	if(hprint || argc < 2){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << "  options:" << endl;
    cout << "   --help(-h)          print options" << endl;
   // cout << "   -path [dest]        path to input ntuples" << endl;
    cout << "   --input(-i) [file]  input file where histograms are" << endl;
    cout << "   --ouput(-o) [file]  output root file" << endl;
    cout << "   +proc [label]       add processes label (can have >1 of these)" << endl;
    cout << "   +fake [label]       add fake sources (ie. elf0, muf1, elf2, can have >1 of these)" << endl;
    cout << "   ++AllFakes          add all fake sources" << endl;
    cout << "   +sys                add systematics" << endl;
    //cout << "   ++all               add all samples" << endl;

    return 0;
  }

	//if(files.size() < 1){
	//	cout << "Error: no files provided." << endl;
	//	break;
	//}

	// if(fUp && !fDown || !fUp && fDown){
	// 	cout << "Error: only one (up/down) file provided. Please provide both up and down files, or one input file." << endl;
	// 	break;
	// }

	TFile* iFile = TFile::Open(inFileName.c_str());
	shapeAnalyzer* shape = new shapeAnalyzer(iFile);
	// if(fUp) shapeAnalzyer* upShape = new shapeAnalyzer(upFile,oFile);
	// if(fDown) shapeAnalyzer* downShape = new shapeAnalyzer(downFile,oFile);

	Int_t nKeys = iFile->GetNkeys();
	TList* keyList = iFile->GetListOfKeys();

	TFile* oFile = new TFile((oFileName+".root").c_str(),"RECREATE");

	// vector<vector<TH1D*>> vecHists;


	//one file
	for(int iKey = 0; iKey < nKeys-2; iKey++){

		// shape->getKeys(iKey);
		TKey* key = (TKey*)keyList->At(iKey); //list of dirs
		if(key == NULL) continue;
		cout << key->GetTitle() << endl;

		TDirectory* oldDir = iFile->GetDirectory(key->GetTitle());
		TList* listOfHists = oldDir->GetListOfKeys();
		// if(!listOfHists->Contains("Fakes_elf0")) continue;

		TDirectory* newDir = oFile->mkdir(key->GetTitle());
		newDir->cd();

		

		for(int iFake = 0; iFake < fakesVec.size(); iFake++){ //loop through fake sources
			if(!listOfHists->Contains(fakesVec.at(iFake).c_str())) continue;
			for(int iProc = 0; iProc < procVec.size(); iProc++){ //loop through processes
				for(int iSys = 0; iSys , sysVec.size();iSys++){ //loop through systematics
					if(iProc == 0) string histName = procVec.at(iProc)+fakesVec.at(iFake);
					else string histName = procVec.at(iProc)+"_"+fakesVec.at(iFake);

					

					shape->drawHists(oldDir, iFake,iProc,histName,sysVec.at(iSys));

					// TH1D* hist = (TH1D*)oldDir->Get(histName);
					// if(hist == NULL) continue;

					// cout << histName << endl;

					// hist->SetTitle(histName);
					// hist->Scale(1/hist->Integral(),"width");
				}

			}
		}
		
	}


	



}
