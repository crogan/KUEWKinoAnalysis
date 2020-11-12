//macro to make workspace
#include "paramHistHelper.hh"
#include "rooHistMaker.hh"
//#include "../HiggsAnalysis/CombinedLimit/interface/RooParametricHist.h"
#include <string>
#include <iostream>
#include "RooWorkspace.h"

int main(int argc, char* argv[]){
	string iFile;
	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i],"-i")){
			iFile = argv[i];	
		}	
	}
	//gSystem->Load("libHiggsAnalysisCombinedLimit.so");
	iFile = "fitInputs/FitInput17_expWeights.root";
	paramHistHelper shit("someLabel",iFile);
	TFile* inFile = TFile::Open(iFile.c_str());
	std::cout << "here" << std::endl;
	rooHistMaker poopUp;
	rooHistMaker poopDown;
	std::cout << "Here1" << std::endl;
	TFile* fOut = new TFile("param_ws.root","RECREATE");
	
	std::cout << "Here2" << std::endl;
	bool isUp;

	//need to loop through categories (directories) and fake procs here
	string lepFlav = "el";
	shit.SetLepFlavor(lepFlav);
	std::vector<int> fakeProcs = {0,1,2};
	std::vector<string> procs = {"","Wjets","ttbar","ZDY"};
	TList* cats = inFile->GetListOfKeys(); //
	string gName;
	TDirectory* dir;
	RooWorkspace ws("ws","ws");

	std::vector<std::string> sysVars;
	sysVars.push_back("lepEta_weight");
	//i < inFile->GetNkeys()
	for(int i = 0; i < 1; i++){
		// shit.SetCategory(cats[i]->GetName());
		// shit.GetDirectory(cats[i]->GetName());
		dir = inFile->GetDirectory(cats->At(i)->GetName());
		std::cout << "directory: " << dir->GetName() << std::endl;
		if(!strstr(dir->GetName(),lepFlav.c_str())) continue;
		for(int k = 0; k < 1; k++){
			for(int j = 0; j < 1; j++){
			
				for(int l = 0; l < sysVars.size();l++){
				
					if(procs[k].empty())
						gName = "Fakes_"+lepFlav+"f"+std::to_string(fakeProcs[j]);
					else
						gName = procs[k]+"_Fakes_"+lepFlav+"f"+std::to_string(fakeProcs[j]);

					// string gName = "h1D_"+proc[k]+lepFlav+"f"+to_string(fakeProcs[k]);
					shit.setGlobalName(gName);
					shit.SetVariation(sysVars[l]);
					// shit.SetFakeProc(fakeProcs[j]);
					TH1D* nomHist = shit.getNormalizedHist(dir);
					std::cout << "1" << std::endl;
					poopUp.setNomHist(nomHist);
					poopDown.setNomHist(nomHist);
					
					std::cout << "2" << std::endl;

					TH1D* varHistUp = shit.getNormalizedHist(dir,true); //up varHist
				std::cout << "3" << std::endl;
					TH1D* varHistDown = shit.getNormalizedHist(dir,false); //down varHist
					 //nomhist
				
				std::cout << "4" << std::endl;
					poopUp.addSysVar(sysVars[l],varHistUp);
					poopDown.addSysVar(sysVars[l],varHistDown);
				std::cout << "5" << std::endl;
				}

				std::cout << "6" << std::endl;
				

				RooParametricHist rPH_up = poopUp.makeRooParamHist(true);
		//		RooParametricHist rPH_down = poop.makeRooParamHist(false);
				std::cout << ws.GetName() << std::endl;
				std::cout << rPH_up.GetName() << std::endl;
				std::cout << "7" << std::endl;
				ws.import(rPH_up);
		//		ws.import(rPH_down);
				//delete rPH_up;
				//delete rPH_down;

				std::cout << "8" << std::endl;
			}

		}

	}

				std::cout << "end for loops" << std::endl;

	fOut->cd();
	ws.Write();
	fOut->Close();
	
				std::cout << "end" << std::endl;
	
	
	
}
