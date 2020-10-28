#ifndef rooHistMaker_HH
#define rooHistMaker_HH

#include <../../HiggsAnalysis/CombinedLimit/interface/RooParametricHist.h>
class rooHistMaker{
public:

	rooHistMaker();

	virtual ~rooHistMaker();
	RooFormulaVar makeRooFormulaVar(int iBin);

	void addSysVar(string sysVar, TH1D* varHist);
	RooParametricHist makeRooParamHist();
	void setNomHist(TH1D* nomHist);




	RooParametricHist* m_paramHist;
	//RooAbsReal x;
	RooArgList pars; //bin contents
	TH1D* m_nomHist;
	TH1D* m_varHist;





private:

	std::vector<string> m_sysVars;
	std::vector<TH1D*> m_varHists;
};

#endif
