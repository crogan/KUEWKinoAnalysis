#include "paramHistHelper.hh"
#include "TSystem.h"
#include "TROOT.h"


//FOR ONE HISTOGRAM

paramHistHelper::paramHistHelper(std::string name, std::string iFileName){
	m_name = name;
	mFileName = iFileName;
	if(gSystem->AccessPathName(mFileName.c_str())){
		std::cout << mFileName << " does not exist" << std::endl;
	}
	mFile = TFile::Open(mFileName.c_str());

}

paramHistHelper::paramHistHelper(TFile* file){
	mFile = file;
	if(gSystem->AccessPathName(mFile->GetName())){
		std::cout << mFile->GetName() << " does not exist" << std::endl;
	}

}




paramHistHelper::~paramHistHelper(){
	sigVec.clear();
	m_lepFlav.clear();
	m_Proc.clear();
	//m_fakeProc.clear();
	m_sysVar.clear();
}





// void paramHistHelper::SetNProc(int nProc){
// 	m_nProc = nProc;
// }

// int paramHistHelper::GetNProc(){
// 	return m_nProc;
// }


// void paramHistHelper::SetNFakeProc(int nFakeProc){
// 	m_nFakeProc = nFakeProc;
// }

// int paramHistHelper::GetNFakeProc(){
// 	return m_nFakeProc;
// }


void paramHistHelper::SetLepFlavor(string lep){
	m_lepFlav = lep;
}

// void paramHistHelper::AddLepFlavor(string lep){
// 	m_lepFlavs.push_back(lep);
// }

string paramHistHelper::GetLepFlavor(){
	return m_lepFlav;
}

void paramHistHelper::SetCategory(string cat){
	m_cat = cat;
}

// void paramHistHelper::AddCategory(string cat){
// 	m_cats.push_back(cat);
// }

string paramHistHelper::GetCategory(){
	return m_cat;
}

//void paramHistHelper::AddFakeProc(int fakeProc){
//	m_fakeProcs.push_back(fakeProc);
//}

int paramHistHelper::GetFakeProc(){
	return m_fakeProc;
}

void paramHistHelper::SetVariation(string sysVar){
	m_sysVar = sysVar;
}

// void paramHistHelper::AddVariation(string sysVar){
// 	m_sysVars.push_back(sysVar);
// }
void paramHistHelper::ClearVariation(){
	m_sysVar.clear();
}
string paramHistHelper::GetVariation(){
	return m_sysVar;
}

// int paramHistHelper::getN(std::vector<string> many, string one){
// 	if(!many.empty()) nObj = many.size();
// 	else if(!one.empty()) nObj = 1;
// 	else{
// 		cout << "Error: no objects specified" << endl;
// 		return;
// 	}
// }



// std::vector<float> paramHistHelper::getSigma(TDirectory* dir, string proc, string lepFlav, int fakeProc, string sysVar ){
	
	
// 	// TDirectory* dir = f->GetDirectory(m_cat.c_str());
// 	if(m_Proc[k].empty())
// 			srting histName = "Fakes_"+lepFlav+fakeProc+"_"+sysVar;
// 		else
// 			string histName = proc+"_Fakes_"+lepFlav+"f"+fakeProc+"_"+sysVar;
// 	// string histName = m_proc+"_Fakes_"+m_lepFlav+str(m_fakeProc);
// 	TH1D* histNom = (TH1D*)dir->Get(histName.c_str());
// 	TH1D* histUp = (TH1D*)dir->Get((histName+"_"+m_sysVar+"Up").c_str());
// 	TH1D* histDown = (TH1D*)dir->Get((histName+"_"+m_sysVar+"Down").c_str());

// 	std::vector<float> sigmas = sigmaFunc(histNom,histUp,histDown); //gives sigma_vec{i} for all bins b 
// 	return sigmas;

// }

std::vector<float> paramHistHelper::sigmaFunc(TH1D* varHist, TH1D* nomHist, bool isUp){
	std::vector<float> sigVec;
	if(isUp){
		for(int b = 0; b < nomHist->GetNbinsX(); b++){
			float sig = fabs(varHist->GetBinContent(b) - nomHist->GetBinContent(b));
			sigVec.push_back(sig);
		}
	}

	else{
		for(int b = 0; b < nomHist->GetNbinsX(); b++){
			float sig = fabs(varHist->GetBinContent(b) - nomHist->GetBinContent(b));
			sigVec.push_back(sig);
		}
	}	

	return sigVec;
}


float paramHistHelper::normFunc(TH1D* nomHist){
	
	float norm = nomHist->Integral();
	return norm;
}

float paramHistHelper::normFunc(TH1D* nomHist, std::vector<float> varSigs){
	float norm = 0;
	if(nomHist->GetNbinsX() != varSigs.size()){
		std::cout << "Error: length of variation vector doesn't match number of bins in nominal histogram" << std::endl;
		return 0;
	}
	for(int i = 0; i < nomHist->GetNbinsX(); i++){
		norm += (nomHist->GetBinContent(i) - varSigs[i]);
	}
	return norm;
}

// float paramHistHelper::getNorm(TDirectory* dir){
	
	
// 	// TDirectory* dir = f->GetDirectory(m_cat.c_str());
// 	if(m_Proc.empty())
// 			srting histName = "Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;
// 		else
// 			string histName = m_Proc+"_Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;
// 	// string histName = m_proc+"_Fakes_"+m_lepFlav+str(m_fakeProc);
// 	TH1D* histNom = (TH1D*)dir->Get(histName.c_str());
// 	TH1D* histUp = (TH1D*)dir->Get((histName+"Up").c_str());
// 	TH1D* histDown = (TH1D*)dir->Get((histName+"Down").c_str());

// 	std::vector<float> sigmas = sigmaFunc(histNom,histUp,histDown); //gives sigma_vec{i} for all bins b 
// 	//put in option for nominal histogram norm (no sigma vector)
// 	float norm = normFunc(histNom, sigmas);
// 	return norm;

// }

// void paramHistHelper::setGlobalName(string name){
// 	m_name = name;
// }

TH1D* paramHistHelper::getNormalizedHist(TDirectory* dir, bool isUp){
	
	
	// TDirectory* dir = f->GetDirectory(m_cat.c_str());
	// if(m_Proc.empty())
	// 		srting histName = "Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;
	// else
	// 	string histName = m_Proc+"_Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;
	// string histName = m_proc+"_Fakes_"+m_lepFlav+str(m_fakeProc);
string histName = m_name+"_"+m_sysVar;
	TH1D* histNom = (TH1D*)dir->Get(m_name.c_str());
	TH1D* varHist;

// TH1D* histUp = (TH1D*)dir->Get((histName+"Up").c_str());
// 	// TH1D* histDown = (TH1D*)dir->Get((histName+"Down").c_str());
//
// 		 // sigmaFunc(histNom,histUp,histDown, isUp); //gives sigma_vec{i} for all bins b 
// 		 	//put in option for nominal histogram norm (no sigma vector)

if(isUp)
		varHist = (TH1D*)dir->Get((histName+"Up").c_str());
	else
		varHist = (TH1D*)dir->Get((histName+"Down").c_str());

	std::vector<float> sigs = sigmaFunc(histNom,varHist, isUp);
	float norm = normFunc(histNom, sigs);
	varHist->Scale(norm);
	return varHist;	

}


//for normalized hist
TH1D paramHistHelper::getNormalizedHist(TDirectory* dir){

	// TDirectory* dir = f->GetDirectory(m_cat.c_str());
	// if(m_Proc.empty())
	// 		srting histName = "Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;
	// else
	// 	string histName = m_Proc+"_Fakes_"+m_lepFlav+"f"+to_string(m_fakeProc)+"_"+m_sysVar;
	// string histName = m_proc+"_Fakes_"+m_lepFlav+str(m_fakeProc);

	string histName = m_name;
	TH1D* histNom = (TH1D*)dir->Get(histName.c_str());
	// TH1D* histUp = (TH1D*)dir->Get((histName+"Up").c_str());
	// TH1D* histDown = (TH1D*)dir->Get((histName+"Down").c_str());
	float norm = normFunc(histNom);
	histNom->Scale(norm);
	return *histNom;

}

// TDirectory* paramHistHelper::getDirectory(string cat){
// 	if(mFileName.empty()){
// 		cout << "Error: no file specified" << endl;
// 		return 0;
// 	}
	
// 	// TDirectory* dir;
// 	// int nCat;

// 	if(m_cat.empty()){
// 		cout << "Error: no categories specified" << endl;
// 		return 0;
// 	}
	
// 	// string histName;
	
// //HERE: GET NORM FOR one category, one fake proc, one assoc. proc, one sys. var.

// 	TDirectory* dir = f->GetDirectory(m_cat.c_str());
// 	return dir;
// }








// float paramHistHelper::loopForNorms(){
// 	if(mFileName.empty()){
// 		cout << "Error: no file specified" << endl;
// 		return 0;
// 	}
// 	TFile* f = TFile::Open(mFileName);
// 	TDirectory* dir;
// 	int nCat;

// 	if(!m_cats.empty()) nCat = m_cats.size();
// 	else{
// 		cout << "Error: no categories specified" << endl;
// 		return 0;
// 	}
	
// 	string histName;
	

// //HERE: GET NORM FOR one category, one fake proc, one assoc. proc, one sys. var.

// 	TDirectory* dir = f->GetDirectory(m_cat.c_str());
// 	// float norm = getNorm(dir);
// 	return norm;
	
// }
