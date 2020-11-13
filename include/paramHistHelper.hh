#ifndef paramHistHelper_HH
#define paramHistHelper_HH
#include <string>
#include <iostream>
#include <stdio.h>
#include <TFile.h>
#include <TString.h>
#include <TH1D.h>


using std::string;
class paramHistHelper{
	public:
		paramHistHelper(std::string label, std::string iFileName);
		paramHistHelper(TFile* file);

		virtual ~paramHistHelper();
		// void loopForNorms();
		float getNorm(TDirectory* dir, std::string proc, std::string lepFlav, int fakeProc, std::string sysVar );
		void SetVariation(std::string sysVar);
		void SetLepFlavor(std::string lep);
		void SetFakeProc(int fakeProc);
		void SetCategory(std::string cat);
		// TDirectory* getDirectory();
		// void setGlobalName(std::string name);




		TH1D* getNormalizedHist(TDirectory* dir);
		TH1D* getNormalizedHist(TDirectory* dir, bool isUp);
	private:
		std::vector<float> sigmaFunc(TH1D* upHist, TH1D* downHist, TH1D* nomHist);
		// float getNorms(TDirectory* dir, string proc, string lepFlav, int fakeProc, string sysVar );
		


		float normFunc(TH1D* nomHist, std::vector<float> varSigs);
		float normFunc(TH1D* nomHist);
		std::vector<float> sigmaFunc(TH1D* varHist, TH1D* nomHist, bool isUp);





		int GetFakeProc();
	std::string GetCategory();
	std::string GetLepFlavor();
	std::string GetVariation();
	std::string m_name;

		// int getN(std::vector<string> many, string one);
		// int GetNFakeProc();
		// void SetNFakeProc(int nFakeProc);
		// int GetNProc();
		// void SetNProc(int nProc);




	std::string mLabel; //label of rooParamHist (cat+fake process+etc.)
		std::vector<float> sigVec;
	
		int m_nSys = -999;
		int m_nProc = -999;
		int m_nFakeProc = -999;
		int m_nCats = -999;
		// std::vector<string> m_cats;
		std::string m_cat;
		// std::vector<string> m_Procs;
		std::string m_Proc;
		std::string  mFileName;
		TFile* mFile;
		std::string  m_lepFlav;
		// std::vector<int> m_fakeProcs;
		int m_fakeProc;
		
		std::string m_sysVar;



};




#endif
