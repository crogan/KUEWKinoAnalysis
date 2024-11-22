#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <set>

#include "TH1D.h"
#include "TFile.h"

std::string eos = "root://cmseos.fnal.gov/";


std::vector<double> GetBinRange(std::string file, std::string catName){

	std::cout<<"analyzing histogram in "<< eos+file <<" \n";	
	TFile* f = TFile::Open((eos+file).c_str());
	std::string hpath= "shapes_prefit/"+catName+"/total_background";
	std::cout<<"finding histogram: "<<hpath<<" \n";
	TH1D* h = (TH1D*) f->Get(hpath.c_str());
	//h->Draw("text");
	//double up = h->GetMaximum();
	//double down = h->GetMinimum();
        double up = h->GetBinContent(h->GetMaximumBin());
	double down = h->GetBinContent(h->GetMinimumBin());
	//triple the max
	up = 50.*up;
	down = down/50.;



	if(down < 0) down = 0.;
	if(down < 1) down = 0.;

	double nbins = (double) h->GetXaxis()->GetNbins();

	std::vector<double> updown = {up,down,nbins};
	f->Close();
	std::cout<<" found histogram with nbins="<<nbins<<" setting axisrange: "<<down<<" "<<up<<"\n";
	return updown;
}


void FillHists(std::string file, std::string catName, std::vector<TH1D*>& hvec ){

	//std::cout<<"analyzing histogram in "<< eos+file <<" \n";
        TFile* f = TFile::Open((eos+file).c_str());
        std::string hpath= "shapes_prefit/"+catName+"/total_background";
 	//std::cout<<"finding histogram: "<<hpath<<" \n";
        TH1D* h = (TH1D*) f->Get(hpath.c_str());
	double nbins = (double) h->GetXaxis()->GetNbins();
	
	double binContent=-1.;
	for( int i=1; i<=nbins; i++){
		binContent = h->GetBinContent(i);
		hvec[i-1]->Fill(binContent);
	}
	f->Close();

}

std::vector<std::string> ReadList( std::string fileList){
	std::vector<std::string> filevec{};
	std::ifstream f;
	f.open(fileList);
	std::string ifile{};
	if( f.is_open()){
		while( f >> ifile ) {
		//	std::cout << "adding item: "<<ifile<<"\n";
			filevec.push_back(ifile);
		}
		f.close();
	}
	else{ std::cout<< "filelist not found\n"; }
	return filevec;
}
int main(int argc, const char *argv[]){
	int nBins = 300;
	std::string fileList = argv[1];
	std::string catName = argv[2];
	std::cout<<"Processing: "<<fileList<<" with Category: "<<catName<<"\n";
	std::vector<double> histInfo;
	std::vector<std::string> filevec;

	filevec = ReadList(fileList);
	histInfo = GetBinRange(filevec[0], catName);
	int hbins = (int)histInfo[2];
	double xup = histInfo[1];
	double xdn = histInfo[0];

	std::string outname = catName+".root";
	TFile* out = new TFile(outname.c_str(),"RECREATE");
	std::cout<<"Initializing output root file: "<< outname <<"\n";

	std::cout<<"Making histogram vector\n";
	std::vector<TH1D*> hvec(hbins);
	std::string iname{};
	for(int i=1; i<=hbins; i++){
		iname = catName+"_"+std::to_string(i);
		hvec[i-1] = new TH1D(iname.c_str(), iname.c_str(),nBins,xdn,xup);
	}
	
	std::cout<<"Looping File list\n";
	int step = 1000;
	std::cout<<"Total files in list: "<<filevec.size()<<"\n";
	for(int i=0; i<filevec.size(); i++){
		if( i== step){
			std::cout<<"Processing @ "<<step<<"\n";
			step+=1000;
		}
		FillHists(filevec[i], catName, hvec );
	}
	out->Write();
	out->Close();

}

