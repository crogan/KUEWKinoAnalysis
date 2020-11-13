#include "rooParamHist.hh"
#include "paramHistHelper.hh"



/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//rooParamHistWrapper class
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


rooParamHistWrapper::rooParamHistWrapper(){
	m_params = nullptr;
	m_shape = nullptr;
	m_var = nullptr;
	m_name = nullptr;
	m_title = nullptr;

	m_rPH = nullptr;
}

rooParamHistWrapper::rooParamHistWrapper(const char* name, const char* title, RooArgList params, TH1D* shape){
	m_params = params;
	m_shape = shape;
	m_var = RooAbsReal("var","2D M_perp vs. RISR",m_shape->GetMinimum(), m_shape->GetMaximum());
	m_name = name;
	m_title = title;

	m_rPH = new rooParametricHist(name, title, x, params, shape);
}

rooParamHistWrapper::~rooParamHistWrapper(){
	delete m_rPH;
	delete m_name;
	delete m_title;
	delete m_params;
	delete m_var;
	delete m_shape;
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
	m_ws = nullptr;
	m_alphas = nullptr;
	m_rPHs = nullptr;
	m_cats = nullptr;
	hNom = nullptr;
}



rooParamHistMaker::rooParamHistMaker(std::vector<string> cats, std::vector<string> sysVars,TFile* file){ //should be given global objects, things that are looped over
//give file? - set number of categories, assoc. procs, fake procs and systematics
	//don't do anything here, just set stuff up
	m_ws = new RooWorkspace("ws","ws");
	m_cats = cats;
	m_sysVars = sysVars;
	// m_nSysVar = sysVars.size();
	m_file = file;
	m_fakeProcs = {0,1,2};
	m_procs.push_back("");
	
	for(int l = 0; l < m_nSysVar; l++){
		m_alphas[l] = new RooRealVar(m_sysVars[l], m_sysVars[l],0.01,10); //common among histograms
	}
}


void rooParamHistMaker::addProcess(string proc){
	m_fakeProcs.push_back(proc);
}

void rooParamHistMaker::setFakeProcesses(std::vector<int> fakeProcs){
	m_fakeProcs.clear();
	m_fakeProcs = fakeProcs;
}

rooParamHistMaker::~rooParamHistMaker(){
	alphas.clear();
	for(int i = 0; i < m_rPHs.size(); i++){
		delete m_rPHs[i];
	}
	delete m_ws;
	m_cats.clear();
	m_fakeProcs.clear();
	m_procs.clear();
	m_sysVars.clear();

}


void rooParamHistMaker::addSysVar(string sysVar){
	m_sysVars.push_back(sysVar);
	// m_nSysVar++;
}

void rooParamHist::setLepFlavor(string lep){
	m_lepFlav = lep;
}



void rooParamHistMaker::makeRooParamHists(){
	string histName;
	

	for(int i = 0; i < m_sysVars.size(); i++){
		dir = m_file->GetDirectory(m_cats->At(i)->GetName());
		if(!strstr(dir->GetName(),lepFlav.c_str())) continue; //check for matching lepton flavor in directory (category) and histname

		for(int k = 0; k < m_procs.size(); k++){
			for(int j = 0; j < m_fakeProcs.size(); j++){
				//get ith, jth, kth nominal histogram (hNom) and all associated systematic variation histograms (vector<TH1D*> sysVars)
				//set mFile in constructor so you can just pull those histograms from there
				if(procs[k].empty())
						histName = "Fakes_"+m_lepFlav+"f"+to_string(m_fakeProcs[j]);
					else
						histName = procs[k]+"_Fakes_"+m_lepFlav+"f"+to_string(m_fakeProcs[j]);

				paramHistHelper makeNormHists(histName,m_file);	
				TH1D* nomHist = makeNormHists.GetNormalizedHist(dir);
				std::vector<TH1D*> hSysVarsUp;
				std::vector<TH1D*> hSysVarsDown;

				for(int l = 0; l < m_sysVars.size(); l++){
					makeNormHists.SetVariaion(m_sysVars[l]);
					TH1D* varHistUp = makeNormHists.GetNormalizedHist(dir,true); //up varHist
					TH1D* varHistDown = makeNormHists.GetNormalizedHist(dir,false); //down varHist
					hSysVarsUp.push_back(varHistUp);
					hSysVarsDown.push_back(varHistDown);
					makeNormHists.ClearVariation();
				}



				makeRooParamHist(nomHist, sysVarsUp); //pushes back a rooParamHistWrapper to vector
				makeRooParamHist(nomHist, sysVarsDown);

				
			}

		}
	}


	makeWorkspace();
}


//make vector of rooFormulaVars to fil rPHs - for one cat, one ass. proc., one fake proc.
void rooParamHistMaker::makeRooParamHist(TH1D* hNom, std::vector<TH1D*> sysVars){
	int nBins = hNom->GetNBinsX();
	// std::vector<rooFormulaVar*> rFVs;
	RooArgList rFVs;
	string inFormula;
	RooArgList varList;

	for(int b = 0; b < nBins; b++){
		
		RooFormulaVar* var;
		std::vector<RooRealVar*> hVarVals;
		RooRealVar* h_nom = new RooRealVar("h_nom","h_nom",m_nomHist->GetBinContent(b));
		
		for(int l = 0; l < m_nSysVar; l++){ //loop over systematic variation histograms
			
			hVarVals.push_back(new RooRealVar(sysVars[l]->GetName(),sysVars[l]->GetTitle(),sysVars[l]->GetBinConent(b)));
			if(l == 0) inFormula += m_alphas[l]->GetName()+"*("+sysVars[l]->GetName()+" - h_nom)/h_nom + ";
			else inFormula += " + "+m_alphas[l]->GetName()+"*("+sysVars[l]->GetName()+" - h_nom)/h_nom";

			varList.add(hVarVals[l],m_alphas[l]);

		}
			var = new RooFormulaVar((hNom->GetName()+str(b)).c_str(),inFormula.c_str(),varList);
			rFVs.add(var);
	}


	m_rPHs.push_back(new rooParamHistWrapper(name,title,rFVs,m_hNom));

}

void rooParamHist::makeWorkspace(){
	for(int i = 0; i < m_rPHs.size(); i++){
		ws.import(m_rPHs[i]->getRooParametricHist());
	}
}



