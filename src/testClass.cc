#include "testClass.hh"

testClass::testClass(string OutFile){
	m_CTTool = CategoryTreeTool();
	m_CT = m_CTTool.GetCategories();
	m_outName = OutFile;
}

testClass::testClass(string OutFile, vector<TH1D*> hists, vector<string> names){
	m_outName = OutFile;
	for(int i = 0; i < hists.size(); i++){
		m_hists.push_back((TH1D*)hists[i]->Clone());
	}
	m_names  = names;
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
	if(m_names.size() != m_nHists){
		cout << "Error: mismatched amount of names and hists."
		return;
	}
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
	// 	totalHist = Clone(group[0]) // (group[0]) is a name
	//  m_nameToNewHist[group[0]] = totalHist
	// 	for(name = 1, name < group.size(); name++):
	// 		totalHist += m_namesToHist[name]
	// 		m_nameToNewHist[name] = totalHist //i^th sumHist corresponds to i^th group => all histograms in that group should get that sumHist
	


}


//need to scale summed histograms by original normalizations
void testClass::scaleHistograms(){
	//PSEUDOCODE
	// for name in m_names:
	// 	newHist = m_nameToNewHist[name]
	// 	newHist->Scale(m_namesToNorms[name]);
	// 	newHist->SetName(newName) //name to be picked up by HC
	// 	newHist->SetTitle(newTitle)

}


void testClass::Write(){
	sumHistograms();
	scaleHistograms();

	TFile* outFile = TFile::Open(m_outName);

	// for(int i = 0; i < m_names.size(); i++){
	//  outFile->cd(folder);
	// 	m_nameToNewHist[m_names[i]]->Write();
	//  need to write new histograms to correct folders
	// }


}
