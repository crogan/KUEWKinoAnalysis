#include <iostream>
#include <fstream>

#include "TSystem.h"
#include "../include/FitReader.hh"
#include "../include/Process.hh"
#include "../include/Systematics.hh"


void checkBadBins(const string& inputfile="BuildFits/BF_allBkgs_data_TChiWZincl_2016_allchan_maskSR_7_19_22/FitInput_KUEWKino_2016.root"){
	//gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
	FitReader* FITReader = new FitReader(inputfile);//,"BuildFits/BF_allBkgs_data_TChiWZincl_2016_allchan_maskSR_7_19_22/datacards/all/TChiWZ/4000350/fitDiagnostics7_27_22wShapes.root","shapes_prefit");
	CategoryList cats = FITReader->GetCategories();
	int Ncats = cats.GetN();
	ProcessList procs = FITReader->GetProcesses();
	int Nproc = procs.GetN();
	double binMin = 1e-5;
	
	ofstream myfile;
	myfile.open("badBins.csv");
	myfile << inputfile << "\n";
	myfile << "category,process,type,bin #,bin content,eff. bin content\n";
	
	for(int i = 0; i < Ncats; i++){
		for(int p = 0; p < Nproc; p++){
		if(procs[p].Type() == kSig) continue;
		if(procs[p].Name().find("Fake") == string::npos) continue;
		//if(procs[p].Name() != "total_background") continue;
			int Nsys = FITReader->m_ProcSys[procs[p]].GetN();
				const TH1D* hist = FITReader->GetHistogram(cats[i],procs[p]);
				if(!hist) continue;
				int Nbins = hist->GetNbinsX();
				for(int b = 0; b < Nbins; b++){
					double bin = hist->GetBinContent(b+1);
					if(bin <= 1e-7) continue;
					double err = hist->GetBinError(b+1);
					double eff = (bin*bin)/(err*err);
					if(bin/eff < 1e-7) myfile << cats[i].FullLabel()+"," << procs[p].Name()+"," << "nominal," << b+1 << "," << bin << "," << eff << "\n";
					//if(bin < 0 || bin < binMin) myfile << cats[i].FullLabel()+"," << procs[p].Name()+"," << "nominal," << b+1 << "," << bin << "\n";

				}
			//for(int s = 0; s < Nsys; s++){
			//	Systematic sys = FITReader->m_ProcSys[procs[p]][s];
			//	const TH1D* hist_up = FITReader->GetHistogram(cats[i],procs[p],sys.Up());
			//	const TH1D* hist_down = FITReader->GetHistogram(cats[i],procs[p],sys.Down());
			//	if(!hist_up || !hist_down) continue;
			//	for(int b = 0; b < Nbins; b++){
			//		double bin_up = hist_up->GetBinContent(b+1);
			//		double bin_down = hist_down->GetBinContent(b+1);
//			//		if(bin_up < 0 || bin_up < binMin) myfile << cats[i].FullLabel()+"," << procs[p].Name()+"," << sys.Label()+"Up,"<< b+1<< "," << bin_up << "\n";
//			//		if(bin_down < 0 || bin_down < binMin) myfile << cats[i].FullLabel()+"," << procs[p].Name()+"," << sys.Label()+"Down," << b+1 << "," << bin_down << "\n";
			//	}
		//	}
		}
	} 




myfile.close();
}
