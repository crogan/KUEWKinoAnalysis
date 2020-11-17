#include "rooParamHist.hh"
#include <string>
#include <iostream>
#include <vector>
#include "TFile.h"

int main(int argc, char* argv[]){
	string iFile;
	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i],"-i")){
			iFile = argv[i];	
		}	
	}
	iFile = "fitInputs/FitInput17_expWeights.root";
	TFile* inFile = TFile::Open(iFile.c_str());
	TFile* outFile = new TFile("param_ws.root","RECREATE");


	vector<string> cats;
	cats.push_back("Ch1L_elm-el0-0bjetS-0bjetISR-PTISR0");

	vector<string> sysVars;
	sysVars.push_back("lepPT_weight");
	rooParamHistMaker fallOutBoy(cats,sysVars,inFile);
	//by default this class will loop over all fake processes (0,1,2), but will only look at the combined associated processes histograms

//	fallOutBoy.addProcess("Wjets");
	fallOutBoy.setLepFlavor("el"); 
	//there is an internal check in the class to make sure the lepton flavor specified matches the given category(ies)

	fallOutBoy.makeRooParamHists(outFile); //makes all rooParamHists and imports them to the workspace

}
