void compareLepIDShapes(){
	// TFile* file = TFile::Open("fitInputs/testweightsFitInput17.root");
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	TFile* file = TFile::Open("FitInput17_expWeights.root");

	string cGold = "Ch1L_elm-el0-0bjetS-0bjetISR-PTISR0";
	string cSilver = "Ch1L_elm-el1-0bjetS-0bjetISR-PTISR0";
	string cBronze = "Ch1L_elm-el2-0bjetS-0bjetISR-PTISR0";

	
	TDirectory* dGold = file->GetDirectory(cGold.c_str());
	TDirectory* dSilver = file->GetDirectory(cSilver.c_str());
	TDirectory* dBronze = file->GetDirectory(cBronze.c_str());
	

	//gold
	TH1D* hGold = (TH1D*)dGold->Get("Fakes_elf0");
	hGold->SetName("Gold");
	hGold->SetTitle("Gold");

	//silver
	TH1D* hSilver = (TH1D*)dSilver->Get("Fakes_elf0");
	
	hSilver->SetTitle("Silver");
	hSilver->SetName("Silver");

	//bronze
	TH1D* hBronze = (TH1D*)dBronze->Get("Fakes_elf0");
	
	hBronze->SetName("Bronze");
	hBronze->SetTitle("Bronze");

	// file->Close();


// std::cout << "hGold Integral before norm: " << hGold->Integral() << std::endl;

	TH1D* hAll = new TH1D(*hGold);
	hAll->Add(hSilver);
	hAll->Add(hBronze);
	
	hAll->Scale(1/hAll->Integral());
	hBronze->Scale(1/hBronze->Integral());
	hSilver->Scale(1/hSilver->Integral());
	hGold->Scale(1/hGold->Integral());

	// std::cout << "hGold Integral after norm: " << hGold->Integral() << std::endl;

	

	hGold->Divide(hAll);
	hSilver->Divide(hAll);
	hBronze->Divide(hAll);
	
	int max;
	if(hSilver->GetMaximum() > hGold->GetMaximum() && hSilver->GetMaximum() > hBronze->GetMaximum()) max = hSilver->GetMaximum();
	else if(hGold->GetMaximum() > hSilver->GetMaximum() && hGold->GetMaximum() > hBronze->GetMaximum()) max = hGold->GetMaximum();
	else if(hBronze->GetMaximum() > hGold->GetMaximum() && hBronze->GetMaximum() > hSilver->GetMaximum()) max = hBronze->GetMaximum();


	TCanvas* cv = new TCanvas("cv","cv",800,600);
	TLegend* leg = new TLegend(0.688,0.65,0.8,0.85);
	// cv->SetTitle(cat.c_str());
	// cv->cd();

	hGold->SetMarkerColor(kOrange);
	hGold->SetLineColor(kOrange);
	hGold->SetMarkerStyle(21);
	hGold->GetYaxis()->SetRangeUser(0,max+1);


	hSilver->SetMarkerColor(kBlue);
	hSilver->SetLineColor(kBlue);
	hSilver->SetMarkerStyle(22);
	hSilver->GetYaxis()->SetRangeUser(0,max+1);

	hBronze->SetMarkerColor(kRed);
	hBronze->SetLineColor(kRed);
	hBronze->SetMarkerStyle(20);
	hBronze->GetYaxis()->SetRangeUser(0,max+1);

	

	
	hGold->Draw("hist H goff");
	hSilver->Draw("hist goff H same");
	hBronze->Draw("hist goff H same");

	leg->AddEntry(hGold);
	leg->AddEntry(hSilver);
	leg->AddEntry(hBronze);
	leg->AddEntry(hAll);


	leg->Draw("same");


	
	std::replace(cBronze.begin(),cBronze.end(),'2','X');

	 TLatex l;
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(20,max+1,cBronze.c_str());




	// cv->BuildLegend();









}