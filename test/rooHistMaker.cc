#include "rooHistMaker.hh"
#include "RooRealVar.h"
#include "TSystem.h"
#include "TROOT.h"
#include "RooFormulaVar.h"


rooHistMaker::rooHistMaker(){
//gSystem->Load("libHiggsAnalysisCombinedLimit.so");
	
	// m_varHist = varHist;
	//m_paramHist = RooParametricHist("hist","hist",x,pars,m_nomHist);

}
rooHistMaker::~rooHistMaker(){
	delete m_paramHist;
}

void rooHistMaker::setNomHist(TH1D* nomHist){
	m_nomHist = nomHist;
}

void rooHistMaker::addSysVar(std::string sysVar, TH1D* varHist){
	m_sysVars.push_back(sysVar);
	m_varHists.push_back(varHist);
}

//make for one bin - all sysVar
RooFormulaVar* rooHistMaker::makeRooFormulaVar(int iBin){
	//this should be common among bins - one for each sysVar
	std::cout << "makeRooFormulaVars" << std::endl;
	//RooFormulaVar rFV;
	// if(m_sysVars.empty()){
	// 	cout << "error: no systematic variations specified" << endl;
	// 	return rFV;
	// }

	// RooRealVar h_var("h_var","h_var",m_varHist->GetBinConent(iBin));
	std::cout << "a" << std::endl;
	RooRealVar h_nom("h_nom","h_nom",m_nomHist->GetBinContent(iBin));

	std::cout << "b" << std::endl;
	RooArgList vars;

	std::cout << "c" << std::endl;
	// vars.add(h_var);
	vars.add(h_nom);

	std::cout << "d" << std::endl;
	//for all sysVars
	std::string inFormula; //consider changing this to a TFormula
//	RooRealVar tmpVarHist;
//	RooRealVar tmpParam;
	//inFormula = "2*h_nom";
	std::cout << "e" << std::endl;
	//VAR HISTS CHANGES WITH EACH SYSVAR - UPDATED ACCORDINGLY
//	for(int l = 0; l < m_sysVars.size(); l++){



RooRealVar* alphas[m_sysVars.size()];
RooRealVar* varHists[m_sysVars.size()];

for(int l = 0; l < m_sysVars.size(); l++){
	std::string paramName = "alpha_"+m_sysVars[l];
	alphas[l] = new RooRealVar(paramName.c_str(),paramName.c_str(),1,10);
	varHists[l] = new RooRealVar(m_varHists[l]->GetName(),m_varHists[l]->GetTitle(),m_varHists[l]->GetBinContent(iBin)); 

	if(l == m_sysVars.size() - 1) inFormula += paramName+"*("+m_varHists[l]->GetName()+" - h_nom)/h_nom";
	else inFormula += paramName+"*("+m_varHists[l]->GetName()+" - h_nom)/h_nom + ";

	vars.add(*alphas[l]);
	vars.add(*varHists[l]);
}

//RooRealVar alpha1("alpha1","alpha1",1,10);
//RooRealVar alpha2("alpha2","alpha2",1,10);
//RooRealVar varHist("varHist","varHist",m_varHists[0]->GetBinContent(iBin));
//
//vars.add(alpha1);
//vars.add(varHist);
//inFormula = "alpha1*(varHist - h_nom)/h_nom";




//	int l = 0;
//		std::string paramName = "alpha_"+m_sysVars[l];
//		
//		RooRealVar tmpVarHist(m_varHists[l]->GetName(),m_varHists[l]->GetTitle(),m_varHists[l]->GetBinContent(iBin));
//		RooRealVar tmpParam(paramName.c_str(),paramName.c_str(),1,10);
//
//		if(l == m_sysVars.size() - 1) inFormula += paramName+"*("+m_varHists[l]->GetName()+" - h_nom)/h_nom";
//		else inFormula += paramName+"*("+m_varHists[l]->GetName()+" - h_nom)/h_nom + ";
//		//else inFormula += paramName+"*("+m_varHists[l]->GetName()+" - h_nom)/h_nom + ";
//		vars.add(tmpVarHist);
//		vars.add(tmpParam);
//		std::cout << tmpVarHist.GetTitle() << std::endl;
//
////		tmpVarHist.Delete();
////		tmpParam.Delete();
//	//}
	std::cout << vars.at(1)->GetTitle() << std::endl;
	std::cout << "formula: " << inFormula << std::endl;
	std::cout << "f" << std::endl;
	RooFormulaVar* rFV = new RooFormulaVar("gen","gen",inFormula.c_str(),vars);

	std::cout << "g" << std::endl;
	//string varName;

	//for one sysVar
	// for(int i = 0; i < m_sysVars.size(); i++){
	// 	if(!strstr(varHist->GetTitle(), m_sysVars[i].c_str())) continue;
	// 	varName = m_sysVars[i];
	// 	break;
	// }

	//name alpha after sysVar
	//RooRealVar alpha("alpha_"+varName,"alpha_"+varName,1,10);

	//this should be for one bin
	// RooRealVar h_var("h_var","h_var",varHist->GetBinConent(iBin));
	// RooRealVar h_nom("h_nom","h_nom",nomHist->GetBinContent(iBin));
	// rFV = RooFormulaVar("gen","alpha*(h_var - h_nom)/h_nom",RooArgList(alpha,h_var,h_nom));

	//there should be one rFV for each bin in the rPH - output one rFV
	return rFV;
	
}



RooParametricHist rooHistMaker::makeRooParamHist(bool isUp){
	RooRealVar x;
	RooArgList params;
	std::cout << "makeRooParamHist" << std::endl; 
	std::cout << "a"  << std::endl;
	RooFormulaVar* formulas[m_nomHist->GetNbinsX()];

	for(int i = 0; i < m_nomHist->GetNbinsX(); i++){		
	std::cout << "bin number " << i << std::endl;
	//RooFormulaVar rFV = makeRooFormulaVar(i);
	formulas[i] = makeRooFormulaVar(i);
	std::cout << "add rFV" << std::endl;
	std::cout << formulas[i]->GetName() << std::endl;
		params.add(*formulas[i]);
	}
//	RooFormulaVar rFV = makeRooFormulaVar(0);
//	params.add(rFV);
	std::cout << "b" << std::endl; 
	std::string name = m_nomHist->GetName();
	std::string title = m_nomHist->GetTitle();
	std::cout << "c" << std::endl; 
	if(isUp){
	name += "UP";
	title += "UP";
	}
  	else{
        name += "DOWN";
        title += "DOWN";
        }

	std::cout << "d" << std::endl; 
	RooParametricHist rPH(name.c_str(),title.c_str(),x,params,*m_nomHist);
	std::cout << "e" << std::endl; 
	return rPH;


}
