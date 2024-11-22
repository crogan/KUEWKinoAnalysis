

void CreatePrePostCSV(std::string inputDiagnostic, std::string csvname="prefit.csv"){


	TFile* fitDiagnosticFile = TFile::Open(inputDiagnostic.c_str());
	//assume there is a signal
	//generate keylist of regions
	TDirectory* shapes_prefit =(TDirectory*) fitDiagnosticFile->Get("shapes_prefit");
//	TDirectory* shapes_fit_b = (TDirectory*) fitDiagnosticFile->Get("shapes_fit_b");
	//hack prefit is actually toy postfit
	TDirectory* shapes_fit_b = (TDirectory*) fitDiagnosticFile->Get("shapes_prefit");

	//TDirectory* shapes_fit_s = (TDirectory*) fitDiagnosticFile->Get("shapes_fit_s");
	TList* keyList = shapes_prefit->GetListOfKeys();
	//keyList->Print();
	//keyList = keyList[0].GetListOfKeys();
	
	TH1F *sb_prefit, *b_prefit;
	TH1F *sb_postfit, *b_postfit;
	int Nbins;
	TGraphAsymmErrors *data;	
	ofstream csvfile;
  	csvfile.open (csvname);
//	csvfile<<"RegionName BinNumber sbprefit sbprefit_err sbpostfit sbpostfit_err bprefit bprefit_err bpostfit bpostfit_err data data_err"<<"\n";
    csvfile<<"RegionName BinNumber bprefit bprefit_err bpostfit bpostfit_err data data_err"<<"\n";
	for(int i=0; i<keyList->GetSize(); i++){
		//do prefit
		//sb_prefit =(TH1F*)  shapes_prefit->Get( (std::string(keyList->At(i)->GetName())+"/total").c_str());	
		
		b_prefit =(TH1F*)  shapes_prefit->Get( (std::string(keyList->At(i)->GetName())+"/total_background").c_str());
		data =(TGraphAsymmErrors*)  shapes_prefit->Get( (std::string(keyList->At(i)->GetName())+"/data").c_str());
		
		//do postfit
		//sb_postfit =(TH1F*)  shapes_fit_s->Get( (std::string(keyList->At(i)->GetName())+"/total").c_str());
		b_postfit =(TH1F*)  shapes_fit_b->Get( (std::string(keyList->At(i)->GetName())+"/total_background").c_str());

		Nbins = b_prefit->GetNbinsX();
		//std::cout<<Nbins<<" ";
		for(int j=1; j<=Nbins; j++){
			csvfile<<keyList->At(i)->GetName()<<" "<<j<<" ";
			//csvfile<<sb_prefit->GetBinContent(j)<<" "<<sb_prefit->GetBinError(j)<<" ";
			//csvfile<<sb_postfit->GetBinContent(j)<<" "<<sb_postfit->GetBinError(j)<<" ";
			csvfile<<b_prefit->GetBinContent(j)<<" "<<b_prefit->GetBinError(j)<<" ";
			csvfile<<b_postfit->GetBinContent(j)<<" "<<b_postfit->GetBinError(j)<<" ";
			csvfile<<data->GetPointY(j-1)<<" "<<data->GetErrorY(j-1)<<"\n";
		}
		

	}
	csvfile.close();
	//b_postfit->Draw();

//TDIR
//shapes_prefit
//shapes_fit_b
//shapes_fit_s
//    KEY: TGraphAsymmErrors	data;1	DB in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
/*    KEY: TH1F	DB;1	DB in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	ST;1	ST in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	TB;1	TB in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	TB_Fakes_elf1G;1	TB_Fakes_elf1G in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	TChiWZ_;1	TChiWZ_ in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	Wjets_Fakes_elf1G;1	Wjets_Fakes_elf1G in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	ZDY;1	ZDY in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	ZDY_Fakes_elf0G_elf1G;1	ZDY_Fakes_elf0G_elf1G in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	ttbar;1	ttbar in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	ttbar_Fakes_elf0G;1	ttbar_Fakes_elf0G in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	ttbar_Fakes_elf1G;1	ttbar_Fakes_elf1G in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	total;1	Total signal+background in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	total_signal;1	Total signal in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH1F	total_background;1	Total background in Ch2L_OSelel_gold_0j0svS_ge1j0bISR_PTISR0_gamT0
    KEY: TH2F	total_covar;1	Covariance signal+background
*/
}
