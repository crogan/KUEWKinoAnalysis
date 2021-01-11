#include "testClass.hh"

testClass::testClass(string OutFile){
	m_CTTool = CategoryTreeTool();
	m_CT = m_CTTool.GetCategories();
	m_outName = OutFile;
}

testClass::testClass(string OutFile, vector<TH1D*> hists){
	m_outName = OutFile;
	for(int i = 0; i < hists.size(); i++){
		m_hists.push_back((TH1D*)hists[i]->Clone());
	}
	m_nHists = m_hists.size();

	m_CTTool = CategoryTreeTool();
	m_CT = m_CTTool.GetCategories();
}


testClass::~testClass(){
	for(int i = 0; i < m_hists.size(); i++){
		delete m_hists[i];
	}
	for(int i = 0; i < m_sumHists.size(); i++){
		delete m_sumHists[i];
	}
	m_outFile.Close();
}


//make maps
void testClass::makeMaps(){
	for(int i = 0; i < m_nHists; i++){
		m_namesToNorms[m_names[i]] = *m_hists[i]->GetIntegral();
		m_namesToHists[m_names[i]] = m_hists[i];
	}
}

//need to sum over histograms that are grouped together (ie sum over btagsS, lepton IDs, PTISRs, and gamTs)
void testClass::sumHistograms(){
	m_CT.Print();
	//should have groupsOfHists from categoryTree (groups of histograms that go together)
	//PSEUDOCODE
	// for group in groupsOfHists:
	// 	totalHist
	// 	for name in group:
	// 		totalHist += m_namesToHist[name]
	// 	m_sumHists.push_back(totalHist); (len(m_sumHists) < len(m_hists))
	//one sumHist per group - map name of each hist to correct sumHist (map<name,TH1D*> m_nameToTotalHist)
	
	// for i, group in groupsOfHists:
	// 	for name in group:
	// 		m_nameToTotalHist[name] = m_sumHists[i] //i^th sumHist corresponds to i^th group => all histograms in that group should get that sumHist

}


//need to scale summed histograms by original normalizations
void testClass::scaleHistograms(){
	//PSEUDOCODE
	// for name in m_names:
	// 	newHist = m_nameToTotalHist[name]
	// 	newHist->Scale(m_namesToNorms[name]);
	// 	newHist->SetName(newName) //name to be picked up by HC
	// 	newHist->SetTitle(newTitle)
	// 	newHist->Write()

	

}
