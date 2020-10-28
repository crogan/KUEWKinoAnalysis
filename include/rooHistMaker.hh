#ifndef rooHistMaker
#define rooHistMaker


class rooHistMaker{
public:

	rooHistMaker(TH1D* nomHist, TH1D* varHist);

	virtual ~rooHistMaker();
	RooFormulaVar makeRooFormulaVar(int iBin);

	void addSysVar(string sysVar, TH1D* varHist);
	RooParametricHist* makeRooParamHist(bool isUp, string fakeProc, string cat);
	void setNomHist(TH1D* nomHist);




	RooParametrichist* m_paramHist;
	RooAbsReal x;
	RooListArg pars; //bin contents
	TH1D* m_nomHist;
	TH1D* m_varHist;





private:

	std::vector<string> m_sysVars;
	std::vector<TH1D*> m_varHists;
}

#endif