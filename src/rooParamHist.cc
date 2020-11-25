#include "rooParamHist.hh"
#include "paramHistHelper.hh"
#include <algorithm>


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

	std::cout << "name: " << m_name << " title: " << m_title << std::endl;
	m_rPH = new RooParametricHist(m_name, m_title, *m_var, *m_params, m_shape);
	m_rPH->Print();
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
	for(int l = 0; l < m_sysVars.size(); l++){
		m_alphasUp.push_back(new RooRealVar((m_sysVars[l]+"Up").c_str(), (m_sysVars[l]+"Up").c_str(),0.01,10)); //common among histograms
		m_alphasDown.push_back(new RooRealVar((m_sysVars[l]+"Down").c_str(), m_sysVars[l].c_str(),0.01,10)); //common among histograms
	}


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
	std::cout << "a" << std::endl;
	for(int i = 0; i < m_rPHs.size(); i++){
		delete m_rPHs[i];
	}
	std::cout << "b" << std::endl;
	delete m_ws;
	std::cout << "c" << std::endl;
	m_cats.clear();
	std::cout << "d" << std::endl;
	m_fakeProcs.clear();
	std::cout << "e" << std::endl;
	m_procs.clear();
	std::cout << "f" << std::endl;
	m_sysVars.clear();
	std::cout << "g" << std::endl;
	//m_file->Close();
	std::cout << "h" << std::endl;

}


void rooParamHistMaker::addSysVar(string sysVar){
	m_sysVars.push_back(sysVar);
	// m_nSysVar++;
}

void rooParamHistMaker::setLepFlavor(string lep){
	m_lepFlav = lep;
}



void rooParamHistMaker::makeRooParamHists(TFile* oFile){
	std::cout << "makerooParamHists" << std::endl;

	string histName;
	//PUT COUTS TO DEBUG

	for(int i = 0; i < m_cats.size(); i++){
		std::cout << "category: " << m_cats[i] << std::endl;
		TDirectory* dir = m_file->GetDirectory(m_cats[i].c_str());
		//std::cout << "dir: " << dir->GetName() << "lepFlav: " << m_lepFlav << std::endl;
		if(dir == NULL){
		 std::cout << "Error: Directory " << m_cats[i].c_str() << " not found" << std::endl;
			return;
		}
		if(m_lepFlav.empty()){
			std::cout << "Error: lepton flavor not set." << std::endl;
			return;
		}
		if(!strstr(m_cats[i].c_str(),m_lepFlav.c_str())) continue; //check for matching lepton flavor in directory (category) and histname

		for(int k = 0; k < m_procs.size(); k++){
			for(int j = 0; j < m_fakeProcs.size(); j++){
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


				// std::cout << "c" << std::endl;
				makeRooParamHist(nomHist, hSysVarsUp, hSysVarsDown); //pushes back a rooParamHistWrapper to vector
			//	makeRooParamHist(nomHist, hSysVarsDown,false);
				// std::cout << "d" << std::endl;
				
			}

		}
	}


	makeWorkspace(oFile);
}

//TODO: CHANGE HNOM TO AN OBJECT HERE AND IN THE FUNCTION ABOVE. STORE THE OBJECT IN ROOPARAMHISTWRAPPER, AND PASS ITS POINTER TO THE ROOPARAMETRICHIST CONSTRUCTOR
//make vector of rooFormulaVars to fil rPHs - for one cat, one ass. proc., one fake proc.
void rooParamHistMaker::makeRooParamHist(TH1D hNom, std::vector<TH1D*> sysVarsUp, std::vector<TH1D*> sysVarsDown){
	std::cout << "makeRooParamHist" << std::endl;
	int nBins = hNom.GetNbinsX();
	// std::vector<rooFormulaVar*> rFVs;
	RooArgList* rFVs = new RooArgList("params");
	RooArgList varList;
	std::vector<RooRealVar*> hNomVals;
	string histName = hNom.GetName();
	

	if(sysVarsUp.size() != sysVarsDown.size()){
		std::cout << "Error: uneven number of up/down systematic variation histograms given." << std::endl;
		return;
	}
	
	for(int l = 0; l < m_sysVars.size(); l++){
		varList.add(*m_alphasUp[l]);
		varList.add(*m_alphasDown[l]);	
	}
	

	

	for(int b = 0; b < nBins; b++){
		
		string inFormula;
		RooFormulaVar* var;
		std::vector<RooRealVar*> hVarValsUp;
		std::vector<RooRealVar*> hVarValsDown;
		string nomName = hNom.GetName();
		nomName +="Nom";
		nomName += std::to_string(b);	
		std::replace(nomName.begin(),nomName.end(),'-','_');
		hNomVals.push_back(new RooRealVar(nomName.c_str(),nomName.c_str(),hNom.GetBinContent(b)));
		varList.add(*hNomVals[b]);	
			
		for(int l = 0; l < m_sysVars.size(); l++){ //loop over systematic variation histograms
			string alphasNameUp;
			string alphasNameDown;
			alphasNameUp = m_alphasUp[l]->GetName();
			alphasNameDown = m_alphasDown[l]->GetName();
			string sysVarsNameUp = sysVarsUp[l]->GetName();
			string sysVarsNameDown = sysVarsDown[l]->GetName();


			std::replace(sysVarsNameUp.begin(),sysVarsNameUp.end(),'-','_');
			std::replace(sysVarsNameDown.begin(),sysVarsNameDown.end(),'-','_');
			sysVarsNameUp += std::to_string(b);
			sysVarsNameDown += std::to_string(b);
			hVarValsUp.push_back(new RooRealVar(sysVarsNameUp.c_str(),sysVarsNameUp.c_str(),sysVarsUp[l]->GetBinContent(b)));
			hVarValsDown.push_back(new RooRealVar(sysVarsNameDown.c_str(),sysVarsNameDown.c_str(),sysVarsDown[l]->GetBinContent(b)));
			
			////MAKE WEIGHTS UP AND DOWN////
			string weightUp = "("+sysVarsNameUp+" - "+nomName+")/"+nomName;
			string weightDown = "("+sysVarsNameDown+" - "+nomName+")/"+nomName;

			////FIX FORMULA////
			if(l == 0) inFormula += "("+alphasNameUp+"/"+alphasNameDown+")*("weightUp+"/"+weightDown+")";
			else inFormula += "+ ("+alphasNameUp+"/"+alphasNameDown+")*("weightUp+"/"+weightDown+")";

			// if(m_sysVars.size() > 1){
			// 	if(l == 0) inFormula += alphasName+"*("+sysVarsName+" - "+nomName+")/"+nomName +" + ";
			// 	else inFormula += " + "+alphasName+"*("+sysVarsName+" - "+nomName+")/"+nomName;
			// }
			// else inFormula += "("+alphasNameUp+"/"+alphasNameDown+")*("weightUp+"/"+weightDown+")";	

			varList.add(*hVarValsUp[l]);
			varList.add(*hVarValsDown[l]);
			
			
	//		if(b == 0){
			// std::cout << "----------------------------- \n bin #: " << b << " sysVar: " << m_sysVars[l] << std::endl;	
			// std::cout << "nomName: " << nomName << " alphasName: " << alphasName << " sysVarsName: " << sysVarsName << "\n" << std::endl;
	//		}
		}
			//var = new RooFormulaVar((hNom->GetName()+std::to_string(b)),hNom->GetTitle(),inFormula,varList);
			string varName = histName+std::to_string(b);
			var = new RooFormulaVar(varName.c_str(),inFormula.c_str(),varList);
			if(b == 0){ 
			var->Print();
			string testNames = m_alphasUp[0]->GetName();
			if(var->getParameter((nomName+testNames).c_str()) == NULL) std::cout << "buggy thing doesn't exist" << std::endl;
			var->dumpFormula();
			}
			rFVs->add(*var);
	}
	m_rPHs.push_back(new rooParamHistWrapper(histName.c_str(),hNom.GetTitle(),rFVs,hNom));

}

void rooParamHistMaker::makeWorkspace(TFile* oFile){
	oFile->cd();

	for(int i = 0; i < m_rPHs.size(); i++){
		m_ws->import(*m_rPHs[i]->getRooParametricHist());
	}
	std::cout << "imported rPHs to workspace" << std::endl;
	m_ws->Write();
	std::cout << "wrote to file" << std::endl;
	oFile->Close();
	std::cout << "closed file" << std::endl;


}



