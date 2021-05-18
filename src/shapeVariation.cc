#include "shapeVariation.hh"
#include "FitReader.hh"
#include "FitBin.hh"
#include "Process.hh"

#include <TSystem.h>

//////////////////////////////
//shapeVariation
/////////////////////////////



shapeVariation::shapeVariation(CategoryList cats,  TFile* f){
//shapeVariation::shapeVariation(CategoryList cats){
	m_cats = cats;
	m_file = f;
}

shapeVariation::~shapeVariation(){
}



//void shapeVariation::doVariations(Process pp,string procLabel,map<Category,string> catLabels, string file){
void shapeVariation::doVariations(Process pp,string procLabel,map<Category,string> catLabels){
	//cout << "shapeVariation::doVariations" << endl;
	string file = m_file->GetName();
	m_file->Close();
	m_file = new TFile(file.c_str(),"UPDATE");
	if(!m_file->IsOpen()){
		cout << "file is not open" << endl;
		m_file = nullptr;
		return;
	}
//cout << "nCats: " << m_cats.GetN() << endl;
	if(m_cats.GetN() < 1) return;
	const FitBin& fitBin = m_cats[0].GetFitBin();
	//nBins = fitBin.NBins();
	//TFile* file;
	//if(!gSystem->AccessPathName(m_file.c_str())) file = TFile::Open(m_file.c_str(),"UPDATE");
	//else file = new TFile(m_file.c_str(),"RECREATE"); 
	for(int c = 0; c < m_cats.GetN(); c++){
//cout << "cat #" << c << endl;
		string dirName = m_cats[c].Label()+"_"+m_cats[c].GetLabel();
		string histName = pp.Name();
		if(histName == "data_obs" || histName.find("_Fakes_") == string::npos) continue;
		string fakeName;
		if(histName.find("f0") != string::npos) fakeName = "f0";
		else if(histName.find("f1") != string::npos) fakeName = "f1";
		TH1D* hist = (TH1D*)m_file->Get((dirName+"/"+histName).c_str());
		if(hist == NULL) continue;
//cout << "got histogram" << endl;
		string sysName;
		if(procLabel.empty()) sysName = histName+"_"+catLabels[m_cats[c]]+fakeName;
		else sysName = histName+"_"+procLabel+"_"+catLabels[m_cats[c]]+fakeName;
		//cout << "name of original histogram: " << dirName+"/"+histName <<endl;
		//cout << "name of systematic histogram: " << sysName << endl;

		TH1D* histRISRUp = (TH1D*)hist->Clone(Form("%s_RISRUp",sysName.c_str()));
		doVariationRISR(histRISRUp,fitBin);
	
		TH1D* histRISRDown = (TH1D*)hist->Clone(Form("%s_RISRDown",sysName.c_str()));
		doVariationRISR(histRISRUp,fitBin,false);
		
		TH1D* histMperpUp = (TH1D*)hist->Clone(Form("%s_MperpUp",sysName.c_str()));
		doVariationMperp(histMperpUp,fitBin);
		
		TH1D* histMperpDown = (TH1D*)hist->Clone(Form("%s_MperpDown",sysName.c_str()));
		doVariationMperp(histMperpUp,fitBin,false);
//cout << "did variations" << endl;
		
		m_file->cd(dirName.c_str());	
		histRISRUp->Write();
		histRISRDown->Write();
		histMperpUp->Write();
		histMperpDown->Write();
//cout << "write hist variations for: " << dirName+"/"+histName << endl;
	}
	m_file->Close();
}

void shapeVariation::doVariationRISR(TH1D* hist, const FitBin& fb, bool isUp){
	//for histogram bin # get corresponding RISR bin #
	int NRbins = fb.NRBins();
	//vector<RBin*> Rbins = fb.RBins();
	//match histogram bins to RISR bin edges
	//make vector of vector of bin numbers (ie [[1,2],[3,4],[5]] -> RISR bins: [1,2,3])
	vector<vector<int>> bins;
	//for bins in one subvector: vary according to RISR bin (ie bin 1: -10%, bin 2: 0%, bin 3: +10%)	
	int ib = 0;
	for(int r = 0; r < NRbins; r++){
		const RBin rbin = fb[r];
		int NMbins = rbin.NBins();
		vector<int> rbins;
		for(int m = 0; m < NMbins; m++){
			//if for this Mperp bin (also individual hist bin) it has the same RISR edges as the last Mperp bin
			//add this indiv. bin index (ib) to the same subvector as before
			rbins.push_back(ib);
			ib++;
		}
		bins.push_back(rbins);
	}


	int midpoint = int(bins.size()/2) + 1;
	for(int r = 0; r < int(bins.size()); r++){
		// set variation for grouping of RISR bins here -> double var =
		// make sure this is based on the r # 
		// 	ie if RISRbins.size() == 11
		// 	midpoint = bin # 6 (int(RISRbins.size()/2) + 1)
		// 	if r < 6: vary by (maxVar/nBins)*(b-midpoint) % 
		double var;
		if(int(bins.size()) % 2 == 0 && r >= midpoint-1) var = ((maxVar/(midpoint - 1))*(r+1 - midpoint + 1))/100;
		else var = ((maxVar/(midpoint - 1))*(r+1 - midpoint))/100;
		for(int b = 0; b < int(bins[r].size()); b++){
			if(isUp) hist->SetBinContent(b+1,hist->GetBinContent(b+1)*(1+var));
			else hist->SetBinContent(b+1,hist->GetBinContent(b+1)*(1-var));
		}
	}
}

void shapeVariation::doVariationMperp(TH1D* hist, const FitBin& fb, bool isUp){
	int NRbins = fb.NRBins();
	//vector<RBin*> Rbins = fb.RBins();
	//match histogram bins to RISR bin edges
	//make vector of vector of bin numbers (ie [[0,0,0],[1,1],[2,2]] -> RISR bins: [1,2,3])
	//for bins in one subvector: vary according to RISR bin (ie bin 1: -10%, bin 2: 0%, bin 3: +10%)	
	//get vector of Mperp bin edges -> loop through all bins and match bin edges 
	vector<vector<int>> ogBins;
	int maxlenRbin = 0;
	for(int r = 0; r < NRbins; r++){
		MBins mBins = fb[r].Bins();
		int NMbins = mBins.NBins();
		vector<int> rbins;
		for(int m = 0; m < NMbins; m++){
			rbins.push_back(m);
		} 
		ogBins.push_back(rbins);
		if(rbins.size() > maxlenRbin) maxlenRbin = int(rbins.size());
	}
	vector<vector<int>> bins(maxlenRbin,vector<int>());
	
	for(int i = 0; i < ogBins.size(); i++){
		for(int j = 0; j < maxlenRbin; j++){
			if(j >= ogBins[i].size()) continue;
			bins[j].push_back(ogBins[i][j]);
		} 
	
	}	
	//if one Mperp bin, don't do anything
	if(bins.size() < 2) return;

	int midpoint = int(bins.size()/2) + 1;
	for(int r = 0; r < int(bins.size()); r++){
		// set variation for grouping of RISR bins here -> double var =
		// make sure this is based on the r # 
		// 	ie if RISRbins.size() == 11
		// 	midpoint = bin # 6 (int(RISRbins.size()/2) + 1)
		// 	if r < 6: vary by (maxVar/nBins)*(b-midpoint) % 
		double var;
		if(int(bins.size()) % 2 == 0 && r >= midpoint-1) var = ((maxVar/(midpoint - 1))*(r+1 - midpoint + 1))/100;
		else var = ((maxVar/(midpoint - 1))*(r+1 - midpoint))/100;
		for(int b = 0; b < int(bins[r].size()); b++){
			if(isUp) hist->SetBinContent(b+1,hist->GetBinContent(b+1)*(1+var));
			else hist->SetBinContent(b+1,hist->GetBinContent(b+1)*(1-var));
		}
	
	}
}


//////////////////////////////
//shapeVariationTool
/////////////////////////////


shapeVariationTool::shapeVariationTool(CategoryTree ct, ProcessList procs, TFile* file){
//shapeVariationTool::shapeVariationTool(CategoryTree ct, ProcessList procs, string file){
	m_ct = ct;
	string filename = file->GetName();
	//file->Close();
	m_fr = new FitReader(filename);
	m_catList = m_fr->GetCategories();
	m_nProc = procs.GetN();
	m_file = file;
	m_procList = procs;
	for(int i = 0; i < m_nProc; i++) m_proc += procs[i].Name();
	makeCatMaps();
	makeProcessMaps();
}


shapeVariationTool::~shapeVariationTool(){
delete m_fr;
}


void shapeVariationTool::doVariations(){
	vector<const CategoryTree*> CTs;
	m_ct.GetListDepth(CTs,1);
	for(int i = 0; i < m_nProc; i++){
		VS vproc;
		if(m_fr->m_Strings.count(m_proc[i]) != 0)
			vproc = m_fr->m_Strings[m_proc[i]];
		else
			vproc += m_proc[i];
		for(int p = 0; p < int(vproc.size()); p++){
			int index = m_fr->GetProcesses().Find(vproc[p]);
			if(index < 0) continue;
			Process pp = m_fr->GetProcesses()[index];
			for(int list = 0; list < int(CTs.size()); list++){
				CategoryList cats = m_catList.Filter(*CTs[list]);
//				cout << "SVT::doVariations category " << CTs[list]->GetSpecLabel() << " nCats: " << cats.GetN() << endl;
				//shapeVariation sv(cats);
				//sv.doVariations(pp,m_domToRareLabels[pp],m_Cats,m_file);				
				shapeVariation sv(cats,m_file);
				sv.doVariations(pp,m_domToRareLabels[pp],m_Cats);				
			}
		}
	}
}



void shapeVariationTool::makeProcessMaps(){
	int nProc = m_procList.GetN();
	for(int p = 0; p < nProc; p++){
		Process pp = m_procList[p];
		if(pp.Name().find("ttbar") != string::npos || pp.Name().find("ST") != string::npos) m_domToRareLabels[pp] = "ttbarST";
		else if(pp.Name().find("Wjets") != string::npos || pp.Name().find("TB") != string::npos || pp.Name().find("DB") != string::npos) m_domToRareLabels[pp] = "WjetsDBTB";
		else m_domToRareLabels[pp] = pp.Name();
	}
}


void shapeVariationTool::makeCatMaps(){
        vector<const CategoryTree*> catTrees;
        m_ct.GetListDepth(catTrees,1);
 //       FitReader fr(m_file);
        int nCatTrees = catTrees.size();
        CategoryList allCats = m_fr->GetCategories(); 
        for(int list = 0; list < nCatTrees; list++){
                CategoryList cats = allCats.Filter(*catTrees[list]);
                int nCats = cats.GetN();
                for(int c = 0; c < nCats; c++){
                        m_Cats[cats[c]] = catTrees[list]->GetSpecLabel();
                }
        }
}
