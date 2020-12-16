#include "rooParamHist.hh"
#include "paramHistHelper.hh"
#include <algorithm>
#include <iostream>


using std::to_string;
using std::cout;
using std::endl;
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//rooParamHistWrapper class
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


rooParamHistWrapper::rooParamHistWrapper(){
	m_params = nullptr;
	//m_shape = nullptr;
	m_var = nullptr;
	m_name = nullptr;
	m_title = nullptr;

	m_rPH = nullptr;
}

rooParamHistWrapper::rooParamHistWrapper(const char* name, const char* title, RooArgList* params, TH1D shape){
	m_params = params;//RooArgList(params, params.GetName());
	m_shape = shape;
	m_var = new RooRealVar("MperpRISR","2D M_perp vs. RISR",m_shape.GetMinimum(), m_shape.GetMaximum());
	m_name = name;
	m_title = title;

	// std::cout << "name: " << m_name << " title: " << m_title << std::endl;
	m_rPH = new RooParametricHist(m_name, m_title, *m_var, *m_params, m_shape);
}

rooParamHistWrapper::~rooParamHistWrapper(){
	//std::cout << "rooParamHistWrapper destructor" << std::endl;
	delete m_rPH;
	//std::cout << "1" << std::endl;
//	delete m_name;
	//std::cout << "2" << std::endl;
//	delete m_title;
	//std::cout << "3" << std::endl;
	delete m_params;
	//std::cout << "4" << std::endl;
	delete m_var;
//	std::cout << "5" << std::endl;
	//delete m_shape;
//	std::cout << "6" << std::endl;
}



RooParametricHist* rooParamHistWrapper::getRooParametricHist(){
	return m_rPH;
}





/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//rooParamHistMaker class
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
rooParamHistMaker::rooParamHistMaker(){
	//m_ws = nullptr;
//	m_alphas = nullptr;
//	m_rPHs = nullptr;
//	m_cats = nullptr;
}



rooParamHistMaker::rooParamHistMaker(std::vector<string> cats, std::vector<string> sysVars,TFile* file){ //should be given global objects, things that are looped over
//give file? - set number of categories, assoc. procs, fake procs and systematics
	//don't do anything here, just set stuff up
	m_ws = new RooWorkspace("wspace","wspace");
	m_cats = cats;
	m_sysVars = sysVars;
	// m_nSysVar = sysVars.size();
	m_file = file;
	m_fakeProcs = {0};//,1,2};
	m_procs.push_back("");
	for(int i = 0; i < m_cats.size(); i++) cout << m_cats[i] << endl;
	// for(int l = 0; l < m_sysVars.size(); l++){
	// 	m_alphasUp.push_back(new RooRealVar((m_sysVars[l]+"Up").c_str(), (m_sysVars[l]+"Up").c_str(),0.01,10)); //common among histograms
	// 	m_alphasDown.push_back(new RooRealVar((m_sysVars[l]+"Down").c_str(), m_sysVars[l].c_str(),0.01,10)); //common among histograms
	// }


}


void rooParamHistMaker::addProcess(string proc){
	m_procs.push_back(proc);
}

void rooParamHistMaker::setFakeProcesses(std::vector<int> fakeProcs){
	m_fakeProcs.clear();
	m_fakeProcs = fakeProcs;
}

rooParamHistMaker::~rooParamHistMaker(){
//	std::cout << "rooParamHistMaker destructor" << std::endl;
	//m_alphas.clear();
	for(int i = 0; i < m_alphasUp.size(); i++){
		delete m_alphasUp[i];
	}
	for(int i = 0; i < m_alphasDown.size(); i++){
		delete m_alphasDown[i];
	}
	// std::cout << "a" << std::endl;
	for(int i = 0; i < m_rPHs.size(); i++){
		delete m_rPHs[i];
	}
	// std::cout << "b" << std::endl;
	delete m_ws;
	// std::cout << "c" << std::endl;
	m_cats.clear();
	// std::cout << "d" << std::endl;
	m_fakeProcs.clear();
	// std::cout << "e" << std::endl;
	m_procs.clear();
	// std::cout << "f" << std::endl;
	m_sysVars.clear();
	// std::cout << "g" << std::endl;
	//m_file->Close();
	// std::cout << "h" << std::endl;

}


void rooParamHistMaker::addSysVar(string sysVar){
	m_sysVars.push_back(sysVar);
	// m_nSysVar++;
}


void rooParamHistMaker::addCategory(string cat){
	m_cats.push_back(cat);
	// m_nSysVar++;
}

void rooParamHistMaker::setLepFlavor(string lep){
	m_lepFlav = lep;
}

void rooParamHistMaker::setLepNumber(int lepNumber){
	m_lepNum = to_string(lepNumber);
}



void rooParamHistMaker::makeRooParamHists(TFile* oFile){
	std::cout << "makerooParamHists" << std::endl;

	string histName;
	string dirName;
	//PUT COUTS TO DEBUG
	 //loop over lepton ID
		
	if(m_lepFlav.empty()){
		std::cout << "Error: lepton flavor not set." << std::endl;
		return;
	}

	if(m_lepNum.empty()){
		std::cout << "Error: lepton number not set." << std::endl;
		return;
	}

	if(m_cats[0].empty()){
		std::cout << "Error: no category specified." << std::endl;
		return;
	}

			
			
			// if(!strstr(dirName.c_str(),m_lepFlav.c_str())) continue; //check for matching lepton flavor in directory (category) and histname


			//TODO: figure out how to add interpolations from different lepton IDs (like in slides)
			for(int k = 0; k < m_procs.size(); k++){
				for(int j = 0; j < m_fakeProcs.size(); j++){
					std::vector<std::vector<TH1D*>> hSysVarsUp_ID;
					std::vector<std::vector<TH1D*>> hSysVarsDown_ID;
					vector<TH1D> hNom_ID;
					for(int i = 0; i < m_cats.size(); i++){
						for(int q = 0; q < 3; q++){
						dirName = "Ch"+m_lepNum+"L_"+m_lepFlav+"pm-"+m_lepFlav+to_string(q)+"-"+m_cats[i];
						cout << dirName << endl;
						TDirectory* dir = m_file->GetDirectory(dirName.c_str());
						//std::cout << "dir: " << dir->GetName() << "lepFlav: " << m_lepFlav << std::endl;
						if(dir == NULL){
						 std::cout << "Error: Directory " << dirName << " not found" << std::endl;
							return;
						}
					//get ith, jth, kth nominal histogram (hNom) and all associated systematic variation histograms (vector<TH1D*> sysVars)
					//set mFile in constructor so you can just pull those histograms from there
				std::cout << "process: " << m_procs[k] << " fake process: " << m_fakeProcs[j] << std::endl;
						if(m_procs[k].empty())
							histName = "Fakes_"+m_lepFlav+"f"+std::to_string(m_fakeProcs[j]);
						else
							histName = m_procs[k]+"_Fakes_"+m_lepFlav+"f"+std::to_string(m_fakeProcs[j]);
					// std::cout << "a" << std::endl;
					paramHistHelper makeNormHists(histName,m_file->GetName());	
					TH1D nomHist = makeNormHists.getNormalizedHist(dir);
					hNom_ID.push_back(nomHist);
					std::vector<TH1D*> hSysVarsUp;
					std::vector<TH1D*> hSysVarsDown;
					// std::cout << "b" << std::endl;
					for(int l = 0; l < m_sysVars.size(); l++){
						makeNormHists.SetVariation(m_sysVars[l]);
						TH1D* varHistUp = makeNormHists.getNormalizedHist(dir,true); //up varHist
						TH1D* varHistDown = makeNormHists.getNormalizedHist(dir,false); //down varHist
						hSysVarsUp.push_back(varHistUp);
						hSysVarsDown.push_back(varHistDown);
						makeNormHists.ClearVariation();
					}
					hSysVarsUp_ID.push_back(hSysVarsUp);
					hSysVarsDown_ID.push_back(hSysVarsDown);



					// std::cout << "c" << std::endl;
					//turn on later!
					// makeRooParamHist(nomHist, hSysVarsUp, hSysVarsDown); //pushes back a rooParamHistWrapper to vector
				//	makeRooParamHist(nomHist, hSysVarsDown,false);
					// std::cout << "d" << std::endl;
					
				}

				makeRooParamHist(hNom_ID,hSysVarsUp_ID,hSysVarsDown_ID);

			}
		}

	}


	makeWorkspace(oFile);
}




double rooParamHistMaker::makeIDNormFactors(vector<TH1D> hNoms){
	TH1D* hTotal = hNoms[0].Clone();
	hTotal->Add(hNoms[1]);
	hTotal->Add(hNoms[2]);
	
	double norm = 1;

	for(int i = 0; i < hNoms.size(); i++){
		norm *= hNoms[i]->Integral()/hTotal->Integral();
	}

	return norm;

}


//for one lepton ID, for one sys var
string rooParamHistMaker::makeInterpolation(RooRealVar* alpha, double nomVal, double upVal, double downVal){
	string formula;
	double deltaUp = upVal - nomVal;
	double deltaDown = downVal - nomVal;
	
	string paramName = alpha->GetName();
	

	string interp = "0.125 * "+paramName+" * ("+paramName+"*"+paramName+" * (3.*"+paramName+"*"+paramName+" - 10.) + 15)";
	string upInterp = to_string(nomVal)+"+ "+paramName+"*"+to_string(deltaUp);
	string downInterp = to_string(nomVal)+"+ "+paramName+"*"+to_string(deltaDown);
	

	interpFormula += "abs("+paramName+") <= 1 ? "+interp+" : "+paramName+" > 1 ? "+upInterp+ " : "+downInterp+")";
	fracFormula += "("+interpFormula+")/"+nomVal;
	return formula;

}


string rooParamHistMaker::getVarNorm(vector<string> fNoHats, TH1D hTotal){
	string hVarNorm;
	for(int b = 0; b < fNoHats.size(); b++){
		if(b == 0) hVarNorm += "("+to_string(hTotal.GetBinContent(b))+"*"+fNoHats[b]+"+ ";
		else hVarNorm += "+"+to_string(hTotal.GetBinContent(b))+"*"+fNoHats[b];
	}

	return hVarNorm;
}




//returns vector of strings (fNoHats - product over sys var, sum over lepton ID), one per bin for rPH
void rooParamHistMaker::makeRooParamHist(std::vector<TH1D> hNoms, std::vector<std::vector<TH1D*>> sysVarsUp, std::vector<std::vector<TH1D*>> sysVarsDown){
std::cout << "makeFormulaBins" << std::endl;

	if(hNoms[0].GetNbinsX() != hNoms[1].GetNbinsX() || hNoms[0].GetNbinsX() != hNoms[2].GetNbinsX() || hNoms[1].GetNbinsX() != hNoms[2].GetNbinsX()){
		cout << "Error: uneven number of bins among lepton ID nominal histograms" << endl;
		return;
	}


	int nBins = hNoms[0].GetNbinsX();
	RooArgList* rFVs = new RooArgList("params");
	vector<RooArgList> varLists;
	std::vector<std::vector<RooRealVar*>> alphas_ID;
	string histName = hNoms[0].GetName();
	double nomVal;
	double upVal;
	double downVal;

	//contraints on floating bronze parameter
	double alphaB_max = 10;
	double alphaB_min = -10;

	vector<string> fNoHats;

	//add nominal histograms to get total histogram over lepton IDs
	TH1D hTotal = TH1D(hNoms[0]);
	hTotal.Add(*hNoms[1]);
	hTotal.Add(*hNoms[2]);
	
	

	for(int q = 0; q < hNoms.size(); q++){
		double norm = hNoms[q].Integral()/hTotal.Integral();
		IDNorm *= norm;
	}


	

	if(sysVarsUp.size() != sysVarsDown.size()){
		std::cout << "Error: uneven number of up/down systematic variation histograms given." << std::endl;
		return;
	}
	
	

	//make floating parameters
	for(int l = 0; l < m_sysVars.size(); l++){
		std::vector<RooRealVar*> alphas;
		//TODO: figure out exactly how to connect the lepton ID parameters? this is a stand in for now
		alphas.push_back(new RooRealVar((m_sysVars[l]+"_bronze").c_str(),(m_sysVars[l]+"_bronze").c_str(),alphaB_max,alphaB_min)); //bronze
		alphas.push_back(new RooRealVar((m_sysVars[l]+"_gold").c_str(),(m_sysVars[l]+"_gold").c_str(),alphas[l]->getValV(),alphas[l]->getValV()-alphas[l]->getValV()*0.1,alphas[l]->getValV()+alphas[l]->getValV()*0.1)); //gold
		alphas.push_back(new RooRealVar((m_sysVars[l]+"_silver").c_str(),(m_sysVars[l]+"_silver").c_str(),alphas[l]->getValV(),alphas[l]->getValV()-alphas[l]->getValV()*0.1,alphas[l]->getValV()+alphas[l]->getValV()*0.1)); //silver
		
		alphas_ID.push_back(alphas);
	}
		
	
	
	

	for(int b = 0; b < nBins; b++){
		
		string fNoHat;
		

		//MODULARIZE BETTER???? - WRITE OUT ON WHITEBOARD (start with what i currently have and fix from there)
	
		
		
			

		for(int l = 0; l < m_sysVars.size(); l++){ //loop over systematic variation histograms
			string formula_IDs;
			for(int q = 0; q < 3; q++){
				nomVal = hNoms[q].GetBinContent(b);
				upVal = sysVarsUp[q][l]->GetBinContent(b);
				downVal = sysVarsDown[q][l]->GetBinContent(b);
			
				string formula_ID += makeInterpolation(alphas_ID[q][l],nomVal[q],upVal[q],downVal[q]);
			
				if(q == 0) formula_IDs += "("+formula_ID; //"("+formula_bronze+" + "+formula_gold+" + "+formula_silver+")";
				else formula_IDs += "+"+formula_ID;

				varLists[b].add(*alphas_ID[q][l])
			}
			formula_IDs += ")";
			if(l == 0) fNoHat += "("+formula_IDs;
			else fNoHat += "*"+formula_IDs;
						
		}
		fNoHat += ")";

		// fNoHat += "*"+to_string(nomVal);
		fNoHat += "*"+to_string(hTotal.GetBinContent(b)); //n^MC in final equation

		fNoHats.push_back(fNoHat);

	}

	string hVarNorm = getVarNorm(fNoHats,hTotal); //sum over bins in denominator
	double IDNorm = makeIDNormFactors(hNoms); //omega
	double totalNorm = hTotal.Integral(); //N_cps in numerator

	vector<string> inFormula = fNoHats;


//loop back over bins to add in normalizations to formula, create rFV and add to list for rPH
	for(int b = 0; b < fNoHats.size(); b++){
		RooFormulaVar* var;
		inFormula[b] += "*"+to_string(IDNorm)+"*"+to_string(totalNorm)+"/"+hVarNorm;
		//add fake source scale factors here

		string varName = histName+"_bin"+std::to_string(b);
		var = new RooFormulaVar(varName.c_str(),inFormula[b].c_str(),varLists[b]);
		rFVs->add(*var);
		// if(b == 0){ 
		// var->Print();
		// string testNames = m_alphasUp[0]->GetName();
		// var->dumpFormula();
		// }
		// rFVs->add(*var);
	}

	

	m_rPHs.push_back(new rooParamHistWrapper(histName.c_str(),hNoms[0].GetTitle(),rFVs,hNoms[0]));


}





void rooParamHistMaker::makeWorkspace(TFile* oFile){
	oFile->cd();

	for(int i = 0; i < m_rPHs.size(); i++){
		m_ws->import(*m_rPHs[i]->getRooParametricHist(),RooFit::Silence);
	}
	std::cout << "imported rPHs to workspace" << std::endl;
	m_ws->Write();
	std::cout << "wrote to file" << std::endl;
	oFile->Close();
	std::cout << "closed file" << std::endl;


}




// //make vector of rooFormulaVars to fil rPHs - for one cat, one ass. proc., one fake proc.
// void rooParamHistMaker::makeRooParamHist(TH1D hNom, std::vector<TH1D*> sysVarsUp, std::vector<TH1D*> sysVarsDown){
// 	std::cout << "makeRooParamHist" << std::endl;
// 	int nBins = hNom.GetNbinsX();
// 	// std::vector<rooFormulaVar*> rFVs;
// 	RooArgList* rFVs = new RooArgList("params");
// 	RooArgList varList;
// 	// std::vector<RooRealVar*> hNomVals;
// 	std::vector<RooRealVar*> alphas;
// 	string histName = hNom.GetName();
// 	double nomVal;
// 	double upVal;
// 	double downVal;
	

// 	if(sysVarsUp.size() != sysVarsDown.size()){
// 		std::cout << "Error: uneven number of up/down systematic variation histograms given." << std::endl;
// 		return;
// 	}
	
// 	// for(int l = 0; l < m_sysVars.size(); l++){
// 	// 	varList.add(*m_alphasUp[l]);
// 	// 	varList.add(*m_alphasDown[l]);	
// 	// }

// 	double hNomNorm = hNom->Integral();
	
// 	string hVarNorm;
	

// 	for(int b = 0; b < nBins; b++){
		
// 		string inFormula;
// 		RooFormulaVar* var;
// 		// std::vector<RooRealVar*> hVarValsUp;
// 		// std::vector<RooRealVar*> hVarValsDown;
// 		// string nomName = hNom.GetName();
// 		// nomName +="Nom";
// 		// nomName += std::to_string(b);	
// 		// std::replace(nomName.begin(),nomName.end(),'-','_');
// 		// hNomVals.push_back(new RooRealVar(nomName.c_str(),nomName.c_str(),hNom.GetBinContent(b)));
// 		// varList.add(*hNomVals[b]);	
// 		nomVal = hNom.GetBinContent(b);


// 		string inFormula;
// 		double alphaB_max = 10;
// 		double alphaB_min = -10;
// 		for(int q = 0; q < 3; q++){

// 			for(int l = 0; l < m_sysVars.size(); l++){ //loop over systematic variation histograms
// 				upVal = sysVarsUp[l]->GetBinContent(b);
// 				downVal = sysVarsDown[l]->GetBinContent(b);
// 				//figure out exactly how to connect the lepton ID parameters? this is a stand in for now
// 				alphas.push_back(new RooRealVar((m_sysVars[l]+"_bronze").c_str(),(m_sysVars[l]+"_bronze").c_str(),alphaB_max,alphaB_min)); //bronze
// 				alphas.push_back(new RooRealVar((m_sysVars[l]+"_gold").c_str(),(m_sysVars[l]+"_gold").c_str(),alphas[l]->getValV(),alphas[l]->getValV()-alphas[l]->getValV()*0.1,alphas[l]->getValV()+alphas[l]->getValV()*0.1)); //gold
// 				alphas.push_back(new RooRealVar((m_sysVars[l]+"_silver").c_str(),(m_sysVars[l]+"_silver").c_str(),alphas[l]->getValV(),alphas[l]->getValV()-alphas[l]->getValV()*0.1,alphas[l]->getValV()+alphas[l]->getValV()*0.1)); //silver


// 				string formula_bronze = makeInterpolation(alphas[l],nomVal,upVal,downVal);
// 				string formula_gold = makeInterpolation(alphas[l+1],nomVal,upVal,downVal);
// 				string formula_silver = makeInterpolation(alphas[l+2],nomVal,upVal,downVal);

// 				string formula_ID = "("+formula_bronze+" + "+formula_gold+" + "+formula_silver+")";
			
// 				if(l == 0) inFormula += "("+formula_ID+"* ";
// 				else inFormula += "*"+formula_ID;
// 				inFormula += ")"

				

// 				varList.add(*alphas[l]); //bronze
// 				varList.add(*alphas[l+1]); //gold
// 				varList.add(*alphas[l+2]); //silver
// 			}			
// 		}
// 			inFormula += "*"+to_string(nomVal);
// 			if(b == 0) hVarNorm += "("+to_string(nomVal)+"*"+inFormula+"+ ";
// 			else hVarNorm += "+"+to_string(nomVal);

// 			//var = new RooFormulaVar((hNom->GetName()+std::to_string(b)),hNom->GetTitle(),inFormula,varList);
// 			string varName = histName+"_bin"+std::to_string(b);
// 			var = new RooFormulaVar(varName.c_str(),inFormula.c_str(),varList);
// 			// if(b == 0){ 
// 			// var->Print();
// 			// string testNames = m_alphasUp[0]->GetName();
// 			// var->dumpFormula();
// 			// }
// 			rFVs->add(*var);

// 	}
	

// 	m_rPHs.push_back(new rooParamHistWrapper(histName.c_str(),hNom.GetTitle(),rFVs,hNom));

// }





