#ifndef rooParamHist_HH
#define rooParamHist_HH


#include "RooParametricHist.h"
#include "RooFormulaVar.h"
#include "TH1D.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
using std::vector;
using std::string;


//class RooParametricHist;
//class RooWorkspace;

class rooParamHistWrapper{
	public:

		rooParamHistWrapper();
		rooParamHistWrapper(const char* name, const char* title, RooArgList* params, TH1D shape);
		virtual ~rooParamHistWrapper();

		RooParametricHist* m_rPH;

		RooParametricHist* getRooParametricHist();

	private:
		RooArgList* m_params;
		TH1D m_shape;
		RooRealVar* m_var;
		const char* m_name;
		const char* m_title;


};

class rooParamHistMaker{
	public:
		rooParamHistMaker();
		rooParamHistMaker(std::vector<string> cats, std::vector<string> sysVars,TFile* file);
		virtual ~rooParamHistMaker();
		
		void makeRooParamHists(TFile* oFile);
		void makeRooParamHist(TH1D hNom, std::vector<TH1D*> sysVars, bool isUp);

		void addSysVar(string sysVar);
		void addProcess(string proc);
		void setLepFlavor(string lep);
		void setFakeProcesses(std::vector<int> fakeProcs);




		//function that makes rooFormulaVar for each rPH bin
		//function that calls rooParamHistWrapper and fills it with rooFormulaVars (loops over everything)



		
	private:
		void makeWorkspace(TFile* oFile);
		RooWorkspace* m_ws;	
		TFile* m_file;
	
		vector<RooRealVar*> m_alphasUp;
		vector<RooRealVar*> m_alphasDown;
		vector<rooParamHistWrapper*> m_rPHs;
		std::vector<string> m_cats;
		std::vector<int> m_fakeProcs;
		std::vector<string> m_procs;
		std::vector<string> m_sysVars;
		string m_lepFlav;
		// int m_nSysVar;
};

#endif

