//macro to make workspace
#include "include/paramHistHelper.hh"
#include "include/rooHistMaker.hh"
#include "../HiggsAnalysis/CombinedLimit/interface/RooParametricHist.h"


void makeWS(string iFile){
cout << "file: " << iFile << endl;
	paramHistHelper shit("someLabel",iFile);
	TFile* inFile = TFile::Open(iFile.c_str());
	rooHistMaker poop;
	TFile* fOut = new TFile("param_ws.root","RECREATE");
	
	bool isUp;

	//need to loop through categories (directories) and fake procs here
	string lepFlav = "mu";
	shit.SetLepFlavor(lepFlav);
	std::vector<int> fakeProcs = {0,1,2};
	std::vector<string> procs = {"Wjets","ttbar","ZDY"};
	TList* cats = inFile->GetListOfKeys(); //
	string gName;
	TDirectory* dir;
	RooWorkspace ws("ws","ws");

	std::vector<std::string> sysVars;
	sysVars.push_back("lepEta_weight");

	for(int i = 0; i < inFile->GetNkeys(); i++){
		// shit.SetCategory(cats[i]->GetName());
		// shit.GetDirectory(cats[i]->GetName());
		dir = inFile->GetDirectory(cats->At(i)->GetName());
		for(int k = 0; k < procs.size(); k++){
			for(int j = 0; j < fakeProcs.size(); j++){

				for(int l = 0; l < sysVars.size();l++){

					if(procs[k].empty())
						gName = "Fakes_"+lepFlav+"f"+to_string(fakeProcs[j])+"_"+sysVars[l];
					else
						gName = procs[k]+"_Fakes_"+lepFlav+"f"+to_string(fakeProcs[j])+"_"+sysVars[l];

					// string gName = "h1D_"+proc[k]+lepFlav+"f"+to_string(fakeProcs[k]);
					shit.setGlobalName(gName);

					// shit.SetFakeProc(fakeProcs[j]);
					TH1D* nomHist = shit.getNormalizedHist(dir);
					poop.setNomHist(nomHist);
					
					TH1D* varHistUp = shit.getNormalizedHist(dir,true); //up varHist
					TH1D* varHistDown = shit.getNormalizedHist(dir,false); //down varHist
					 //nomhist


					poop.addSysVar(sysVars[l],varHistUp);
					poop.addSysVar(sysVars[l],varHistDown);
				}

				

				


				RooParametricHist rPH_up = poop.makeRooParamHist();
				RooParametricHist rPH_down = poop.makeRooParamHist();
				ws.import(rPH_up);
				ws.import(rPH_down);
				//delete rPH_up;
				//delete rPH_down;

			}

		}

	}


	fOut->cd();
	ws.Write();
	fOut->Close();
	
	
	
	
}
