#ifndef paramHistHelper
#define paramHistHelper

class paramHistHelper{
	public:
		paramHistHelper(string label, string iFileName);
		paramHistHelper(TFile* file);

		virtual ~paramHistHelper();
		// void loopForNorms();
		float getNorm(TDirectory* dir, string proc, string lepFlav, int fakeProc, string sysVar );
		void SetVariation(string sysVar);
		void SetCategory(string cat);
		void SetLepFlavor(string lep);
		void SetFakeProc(int fakeProc);
		void SetCategory(string cat);
		// TDirectory* getDirectory();
		void setGlobalName(string name);




	private:
		std::vector<float> sigmaFunc(TH1D* upHist, TH1D* downHist, TH1D* nomHist);
		float normFunc(TH1D* nomHist, std::vector<float> varSigs);
		float normFunc(TH1D* nomHist);
		// float getNorms(TDirectory* dir, string proc, string lepFlav, int fakeProc, string sysVar );
		


		float normFunc(TH1D* nomHist, std::vector<float> varSigs);
		float normFunc(TH1D* nomHist);
		std::vector<float> sigmaFunc(TH1D* varHist, TH1D* nomHist, bool isUp);


		TH1D* getNormalizedHist(TDirectory* dir);
		TH1D* getNormalizedHist(TDirectory* dir, bool isUp);



		int GetFakeProc();
		string GetCategory();
		string GetLepFlavor();
		string GetVariation();
		string m_name;

		// int getN(std::vector<string> many, string one);
		// int GetNFakeProc();
		// void SetNFakeProc(int nFakeProc);
		// int GetNProc();
		// void SetNProc(int nProc);




		string mLabel; //label of rooParamHist (cat+fake process+etc.)
		std::vector<float> sigVec;
	
		int m_nSys = -999;
		int m_nProc = -999;
		int m_nFakeProc = -999;
		int m_nCats = -999;
		// std::vector<string> m_cats;
		string m_cat;
		// std::vector<string> m_Procs;
		string m_Proc;
		string mFile;
		string m_lepFlav;
		// std::vector<int> m_fakeProcs;
		int m_fakeProc;
		
		string m_sysVar;



}




#endif