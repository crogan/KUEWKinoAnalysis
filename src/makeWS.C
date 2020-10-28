//macro to make workspace
#include paramHistHelper.hh
#include rooHistHelper.hh

void makeWS(TFile* inFile){

	paramHistHelper shit("someLabel",inFile);
	rooHistMaker poop();
	TFile* fOut = new TFile("param_ws.root","RECREATE");
	
	bool isUp;

	//need to loop through categories (directories) and fake procs here
	string lepFlav = "mu";
	shit.SetLeptonFlavor(lepFlav);
	std::vector<int> fakeProcs = {0,1,2};
	std::vector<string> procs = {"Wjets","ttbar","ZDY"};
	TList* cats = inFile->GetListOfKeys(); //
	string gName;
	TDirectory* dir;
	RooWorkSpace ws("ws","ws");


	for(int i = 0; i < inFile->GetNkeys; i++){
		// shit.SetCategory(cats[i]->GetName());
		// shit.GetDirectory(cats[i]->GetName());
		dir = inFile->GetDirectory(cats->At(i)->GetName());
		for(int k = 0; k < procs.size(); k++){
			for(int j = 0; j < fakeProcs.size(); j++){

				for(int l = 0; l < sysVars.size();l++){

					if(procs[k].empty())
						gName = "Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;
					else
						gName = procs[k]+"_Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;

					// string gName = "h1D_"+proc[k]+lepFlav+"f"+to_string(fakeProcs[k]);
					shit.setGlobalName(gName);

					// shit.SetFakeProc(fakeProcs[j]);
					TH1D* nomHist = shit.GetNormalizedHist(dir);
					poop.SetNomHist(nomHist);
					
					TH1D* varHistUp = shit.GetNormalizedHist(dir,true); //up varHist
					TH1D* varHistDown = shit.GetNormalizedHist(dir,false); //down varHist
					 //nomhist


					poop.AddSysVar(sysVars[l],varHist);
				}

				

				


				RooParametericHist* rPH_up = makeRooParamHist(true, fakeProcs[j], cats->At(i)->GetName());
				RooParametericHist* rPH_down = makeRooParamHist(false, fakeProcs[j], cats->At(i)->GetName());
				ws.import(rPH_up);
				ws.import(rPH_down);
				delete rPH_up;
				delete rPH_down;
				delete nomHist;

			}

		}

	}


	fOut->cd();
	ws.Write();
	fOut->Close();
	
	
	
	
}