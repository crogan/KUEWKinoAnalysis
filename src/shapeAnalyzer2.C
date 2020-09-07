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
			files.push_back(TFile::Open(inFileName));
		}
		if(strncmp(argv[i],"+fake",5) == 0){
			i++;
			fakesVec.push_back("Fakes_"+argv[i]);
		}
		if(strncmp(argv[i],"++AllFakes",10) == 0){
			i++;
			fakesVec = {"Fakes_elf0","Fakes_elf1","Fakes_elf2"};
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

	if(hprint){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << "  options:" << endl;
    cout << "   --help(-h)          print options" << endl;
    cout << "   -path [dest]        path to input ntuples" << endl;
    cout << "   --ouput(-o) [file]  output root file" << endl;
    cout << "   +proc [label]       add processes matching label (can have >1 of these)" << endl;
    cout << "   +fake [label]       add fake sources (ie. elf0, muf1, elf2)" << endl;
    cout << "   ++sig               add all signal samples" << endl;
    cout << "   ++data              add all background samples" << endl;
    cout << "   ++all               add all samples" << endl;

    return 0;
  }

	if(files.size() < 1){
		cout << "Error: no files provided." << endl;
		break;
	}

	// if(fUp && !fDown || !fUp && fDown){
	// 	cout << "Error: only one (up/down) file provided. Please provide both up and down files, or one input file." << endl;
	// 	break;
	// }

	shapeAnalyzer* shape = new shapeAnalyzer(inFile, oFile);
	// if(fUp) shapeAnalzyer* upShape = new shapeAnalyzer(upFile,oFile);
	// if(fDown) shapeAnalyzer* downShape = new shapeAnalyzer(downFile,oFile);

	Int_t nKeys = inFile->GetNkeys();
	TList* keyList = iFile->GetListOfKeys();;

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

		for(int iSys = 0; iSys < sysVec.size();iSys++){

			for(int iFake = 0; iFake < fakesVec.size(); iFake++){
				if(!listOfHists->Contains(fakesVec.at(iFake))) continue;
				for(int iProc = 0; iProc < procVec.size(); iProc++){
					for(int iSys = 0; iSys , sysVec.size();iSys++){
						String histName = (procVec.at(iProc)+"_"+fakesVec.at(iFake)).c_str();

						

						shape->drawHists(oldDir, iFake,iProc,histName,sys);

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


	



}
