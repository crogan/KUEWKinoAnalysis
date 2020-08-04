void shapeAnalyzer(TString file){
	TFile* iFile = TFile::Open(file);
	TFile* oFile = new TFile("varHists17.root","RECREATE");

	Int_t nKeys = iFile->GetNkeys();
	TList* keyList = iFile->GetListOfKeys();
	std::vector<TString> fakesVec{"Fakes_elf0","Fakes_elf1","Fakes_elf2"};
	std::vector<TString> procVec{"","_ttbar_","_ZDY_"};

	gStyle->SetOptStat(0);


	for(int iKey = 0; iKey < 3; iKey++){
		TKey* key = (TKey*)keyList->At(iKey);
		TDirectory* oldDir = iFile->GetDirectory(key->GetTitle());
		TList* listOfHists = oldDir->GetListOfKeys();


		for(int iHist = 0; iHist < fakesVec.size(); iHist++){
			for(int iProc = 0; iProc < procVec.size(); iProc++){
				cout << procVec.at(iProc) << fakesVec.at(iHist) << endl;
				TString histName = procVec.at(iProc)+fakesVec.at(iHist);
				TH1D* nomHist = (TH1D*)oldDir->Get(histName);
				if(nomHist == NULL) continue;

				cout << histName << endl;

				nomHist->SetTitle(histName);

				TH1D* upHist = (TH1D*)oldDir->Get(histName+"_BTAG_SFUp");
				TH1D* downHist = (TH1D*)oldDir->Get(histName+"_BTAG_SFDown");

				upHist->SetTitle(histName+"_BTAG_SFUp");
				downHist->SetTitle(histName+"_BTAG_SFDown");

				nomHist->Scale(1/nomHist->Integral(),"width");
				upHist->Scale(1/upHist->Integral(),"width");
				downHist->Scale(1/downHist->Integral(),"width");

				upHist->SetLineColor(kRed);
				upHist->SetMarkerColor(kRed);
				upHist->SetMarkerStyle(10);

				downHist->SetLineColor(kBlue);
				downHist->SetMarkerColor(kBlue);
				downHist->SetMarkerStyle(10);

				nomHist->SetLineColor(kGreen);
				nomHist->SetMarkerColor(kGreen);
				nomHist->SetMarkerStyle(10);


				TCanvas* c1 = new TCanvas(histName,histName,800,600);
				TLegend* leg = new TLegend(0.65,0.7,0.9,0.9);

				leg->AddEntry(nomHist);
				leg->AddEntry(upHist);
				leg->AddEntry(downHist);

				nomHist->Draw("goff");
				upHist->Draw("same goff");
				downHist->Draw("same goff");

				leg->Draw("same goff");

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