#include "TSystem.h"
#include "TROOT.h"
#include <TStyle.h>
#include "../include/FitPlotter.hh"

void PlotPrePostStack_wrapper(const string& fold1 = "BF_allBkgs_data_T2tt_0L1L_QCDShapesSJet0p20var_QCDnorms0p50_WJetsnorms0p20_otherBkgnorms0p20_maskSR_09_27_21", const string& fold2 = "all/T2tt/6000425", const string& shapesFile = "09_24_21wShapes.root"){

	string dateName = shapesFile.substr(0,8);
	string bfName = fold1.substr(2,fold1.size());
	string odir = "prePostStackPlots"+bfName;

	string inputfile1 = "BuildFits/"+fold1+"/FitInput_KUEWKino_2017.root";
	string inputfile2 = "BuildFits/"+fold1+"/"+fold2+"/fitDiagnostics"+shapesFile;
	string lepName;
	if(fold2.find("Ch") != string::npos) lepName = fold2.substr(0,4);
	else lepName = "all";
	string fname = odir+"/"+lepName+"/"+shapesFile;
cout << "input file: " << inputfile2 << endl;
cout << "out directory: " << odir << "/" << lepName << "/" << endl;
	if(gSystem->AccessPathName(inputfile2.c_str())){ cout << "file " << inputfile2 << " not found" << endl; return; }
	FitPlotter* FITPlotter_pre = new FitPlotter(inputfile1, inputfile2, "shapes_prefit");
        FitPlotter* FITPlotter_bOnly = new FitPlotter(inputfile1, inputfile2, "shapes_fit_b");
        FitPlotter* FITPlotter_sb = new FitPlotter(inputfile1, inputfile2, "shapes_fit_s");
	
        VS lep_label = VS().a("0L").a("1L");
	VS hadS_label = VS();
//	hadS_label += VS().a("0j0svS").a("0j1svS").a("0jge1svS").a("0jge2svS").a("0jS");
	hadS_label += VS().a("1j1b0svS").a("1j0bge1svS").a("1j0b1svS").a("1j1bge1svS").a("1j0svS").a("1jge1svS").a("1jS");
//	hadS_label += VS().a("2j0bS").a("2j1bS").a("2j2bS").a("2jS");
//	hadS_label += VS().a("3j0bS").a("3j1bS").a("3jge2bS").a("3jS");
	//hadS_label += VS().a("4j0bS").a("4j1bS").a("4jge2bS").a("4jS");
	//hadS_label += VS().a("5j0bS").a("5j1bS").a("5jge2bS").a("5jS");
	VS hadI_label = VS().a("ge1jISR").a("ge1j0bISR").a("ge1jge1bISR");
	VS extra_label = VS().a("").a("SVeta0").a("SVeta1");
        VS proc = VS();
        proc += "data_obs";
        proc += "ttbar";
        proc += "ZDY";
        proc += "DB";
        proc += "Wjets";
        proc += "QCD";
        proc += "ST";
        proc += "TB";
        proc += "HF_Fakes";
        proc += "LF_Fakes";
	string sig = "T2tt_6000425";
	proc += sig;

	if(gSystem->AccessPathName((fname).c_str())){
		gSystem->Exec(("mkdir "+odir).c_str());
		gSystem->Exec(("mkdir "+odir+"/"+lepName).c_str());
	}
	TFile* file = new TFile(fname.c_str(),"RECREATE");
	cout << "Writing to file: " << fname << endl;
	file->cd();
	int nLep = lep_label.size();
	int nHadS = hadS_label.size();
	int nExtra = extra_label.size();
	int nISR = hadI_label.size();
	for(int i = 0; i < nLep; i++){
		for(int j = 0; j < nHadS; j++){
		//	if(hadS_label[j].find("sv") != std::string::npos){
		//		extra_label += VS().a("SVeta0").a("SVeta1");
		//		nExtra = extra_label.size();
		//	}
			//need to loop through SV regions for *j*svS categories only
			for(int k = 0; k < nExtra; k++){
				for(int l = 0; l < nISR; l++){
				string dir = lep_label[i]+"_"+hadS_label[j]+"_"+hadI_label[l];
				if(extra_label[k] != "") dir += "_"+extra_label[k];
				cout << lep_label[i] << "_" << hadS_label[j] << "_" << extra_label[k] << "_" << hadI_label[l] << endl;
				cout << "##############plot prefit#############" << endl;
				 TCanvas* prefit_stack = FITPlotter_pre->Plot1Dstack(proc,
	                	      VS().a(lep_label[i]),
	                	      VS().a(hadS_label[j]),
	                	      VS().a(hadI_label[l]),
	                	      "pre_stack",
				      VS().a(extra_label[k]));
				if(prefit_stack == nullptr) continue;
				cout << "##############plot b-fit#############" << endl;
				TCanvas* b_fit_stack = FITPlotter_bOnly->Plot1Dstack(proc,
	                	      VS().a(lep_label[i]),
	                	      VS().a(hadS_label[j]),
	                	      VS().a(hadI_label[l]),
	                	      "bFit_stack",
				      VS().a(extra_label[k]));
				if(b_fit_stack == nullptr) continue;
//				cout << "##############plot s+b fit#############" << endl;
//	 			TCanvas* sb_fit_stack = FITPlotter_sb->Plot1Dstack(proc,
//	                	      VS().a(lep_label[i]),
//	                	      VS().a(hadS_label[j]),
//	                	      VS().a(hadI_label),
//	                	      "sbFit_stack",
//				      VS().a(extra_label[k]));
//				if(sb_fit_stack == nullptr) continue;
//
					
				if(!file->GetDirectory(dir.c_str()))
	        			file->mkdir(dir.c_str());
				file->cd(dir.c_str());
				prefit_stack->Write("prefit_stack",TObject::kOverwrite);
				b_fit_stack->Write("bfit_stack",TObject::kOverwrite);
				//sb_fit_stack->Write("sbfit_stack",TObject::kOverwrite);
	
				prefit_stack->SaveAs((odir+"/"+lepName+"/"+dir+"_"+sig+"prefit.pdf").c_str());
				b_fit_stack->SaveAs((odir+"/"+lepName+"/"+dir+"_"+sig+"b-fit.pdf").c_str());
				//sb_fit_stack->SaveAs((odir+"/"+lepName+"/"+dir+"_"+sig+"sb-fit.pdf").c_str());
		
				prefit_stack->Close();
				b_fit_stack->Close();
				//sb_fit_stack->Close();
	
				file->cd();
				cout << "\n" << endl;
				}
			}
		}
	}


file->Close();


	
	



}
