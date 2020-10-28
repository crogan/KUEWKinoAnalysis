#include rooHistMaker.hh

rooHistMaker::rooHistMaker(){

	
	// m_varHist = varHist;
	m_paramHist = RooParametricHist("hist","hist",x,pars,m_nomHist);

}
rooHistMaker::~rooHistMaker(){
	delete m_paramHist;
}

void rooHistMaker::setNomHist(TH1D* nomHist){
	m_nomHist = nomHist;
}

void rooHistMaker::addSysVar(string sysVar, TH1D* varHist){
	m_sysVars.push_back(sysVar);
	m_varHists.push_back(varHist);
}

//make for one bin - all sysVar
RooFormulaVar rooHistMaker::makeRooFormulaVar(int iBin){
	//this should be common among bins - one for each sysVar
	RooFormulaVar rFV;
	// if(m_sysVars.empty()){
	// 	cout << "error: no systematic variations specified" << endl;
	// 	return rFV;
	// }

	// RooRealVar h_var("h_var","h_var",m_varHist->GetBinConent(iBin));
	RooRealVar h_nom("h_nom","h_nom",m_nomHist->GetBinContent(iBin));

	RooArgList vars;

	// vars.add(h_var);
	vars.add(h_nom);

	//for all sysVars
	string inFormula;
	RooRealVar tmpVar


	//VAR HISTS CHANGES WITH EACH SYSVAR - UPDATED ACCORDINGLY
	for(int l = 0; l < m_sysVars.size(); l++){
		string paramName = "alpha_"+m_sysVars[l];
		
		tmpVarHist = RooRealVar(m_varHists[l]->GetName(),m_varHists[l]->GetTitle(),m_varHists[l]->GetBinConent(iBin));
		tmpParam = RooRealVar(paramName.c_str(),paramName.c_str(),1,10);

		if(l == 0) inFormula += paramName+"*("+m_varHists[l]->GetName()+" - h_nom)/h_nom + ";
		else inFormula += " + "+paramName+"*("+m_varHists[l]->GetName()+" - h_nom)/h_nom";
		vars.add(tmpVar,tmpParam);
	}

	rFV = RooFormulaVar("gen",inFormula,vars);

	string varName;

	//for one sysVar
	// for(int i = 0; i < m_sysVars.size(); i++){
	// 	if(!strstr(varHist->GetTitle(), m_sysVars[i].c_str())) continue;
	// 	varName = m_sysVars[i];
	// 	break;
	// }

	//name alpha after sysVar
	RooRealVar alpha("alpha_"+varName,"alpha_"+varName,1,10);

	//this should be for one bin
	// RooRealVar h_var("h_var","h_var",varHist->GetBinConent(iBin));
	// RooRealVar h_nom("h_nom","h_nom",nomHist->GetBinContent(iBin));
	// rFV = RooFormulaVar("gen","alpha*(h_var - h_nom)/h_nom",RooArgList(alpha,h_var,h_nom));

	//there should be one rFV for each bin in the rPH - output one rFV
	return rFV;
	
}



RooParametricHist* rooHistMaker::makeRooParamHist(bool isUp, string fakeProc, string cat){
	RooAbsReal x;
	RooArglist params;
	 
	for(int i = 0; i < m_nomHist->GetBinsX(); i++){		
		params.add(makeRooFormulaVars(i));
	}


	RooParametricHist* rPH = new RooParametricHist(m_nomHist->GetName(),m_nomHist->GetTitle(),x,params,m_nomHist);
	return rPH;


}