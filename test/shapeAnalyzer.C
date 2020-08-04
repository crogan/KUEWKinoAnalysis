void shapeAnalyzer(TString file){
	TFile* iFile = TFile::Open(file);
	TFile* oFile = new TFile("varHists");

	Int nKeys = iFile->GetNkeys();
	TList* keyList = iFile->GetListOfKeys();
	std::vector<TString> fakesVec{"Fakes_elf0","Fakes_elf1","Fakes_elf2"};
	std::vector<TString> procVec{"","_ttbar_","_ZDY_"};


	for(int iKey = 0; iKey < nKey; i++){
		TKey* key = (TKey*)keyList->At(iKey);
		TDirectory* oldDir = iFile->GetDirectory(key->GetTitle());
		TList* listOfHists = oldDir->GetListOfKeys();


		for(int iHist = 0; iHist < fakesVec.size(); iHist++){
			for(int iProc = 0; iProc < procVec.size(); iProc++){
				TString histName = procVec.at(iProc)+fakesVec.at(iHist);
				TH1D* nomHist = (TH1D*)listOfHists->Get(histName);
				if(nomHist == NULL) continue;

				cout << histName << endl;

				TH1D* upHist = (TH1D*)listOfHists->Get((histName+"_BTAG_SFUp").c_str());
				TH1D* downHist = (TH1D*)listOfHists->Get((histName+"_BTAG_SFDown").c_str());

				nomHist->Scale(1/nomHist->Integral(),"width");
				upHist->Scale(1/upHist->Integral(),"width");
				downHist->Scale(1/downHist->Integral(),"width");

				upHist->SetLineColor(kRed);
				upHist->SetMarkerColor(kRed);
				upHist->SetMarkerStyle(kCircle);

				downHist->SetLineColor(kBlue);
				downHist->SetMarkerColor(kBlue);
				downHist->SetMarkerStyle(kCircle);

				nomHist->SetLineColor(kViolet);
				nomHist->SetMarkerColor(kViolet);
				nomHist->SetMarkerStyle(kCircle);


				TCanvas* c1 = new TCanvas("c1","c1",800,600);

				nomHist->Draw();
				upHist->Draw("same");
				downHist->Draw("same");

				c1->Write();
				c1->Close();

				delete c1;
			}

		}

		TDirectory* newDir = oFile->mkdir(key->GetTitle());
		newDir->cd();
		newDir->Write();
	}
}