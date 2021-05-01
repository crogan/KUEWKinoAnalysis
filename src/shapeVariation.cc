#include "shapeVariation.hh"
#include "FitReader.hh"
#include "FitBin.hh"
#include "Process.hh"

#include <TSystem.h>

//////////////////////////////
//shapeVariation
/////////////////////////////



shapeVariation::shapeVariation(CategoryList cats,  string f){
	m_cats = cats;
	m_file = f;
}

shapeVariation::~shapeVariation(){
	m_file = "";
}



void shapeVariation::doVariations(Process pp,map<Category,string> labels){
	const FitBin& fitBin = m_cats[0].GetFitBin();
	//cout << "nBins" << endl;	
	//nBins = fitBin.NBins();
	TFile* file;
	if(!gSystem->AccessPathName(m_file.c_str())) file = TFile::Open(m_file.c_str(),"UPDATE");
	else file = new TFile(m_file.c_str(),"RECREATE");
	for(int c = 0; c < m_cats.GetN(); c++){
		//cout << "cat #" << c << endl;
		string dirName = m_cats[c].Label()+"_"+m_cats[c].GetLabel();
		
		string histName = pp.Name();

		TH1D* hist = (TH1D*)file->Get((dirName+"/"+histName).c_str());
		if(hist == NULL) continue;
		string sysName = histName+"_"+labels[m_cats[c]];
		cout << sysName << endl;
		TH1D* histRISRUp = (TH1D*)hist->Clone(Form("%s_RISRUp",sysName.c_str()));
		doVariationRISR(histRISRUp,fitBin);
		
		TH1D* histRISRDown = (TH1D*)hist->Clone(Form("%s_RISRDown",sysName.c_str()));
		doVariationRISR(histRISRUp,fitBin,false);
		
		TH1D* histMperpUp = (TH1D*)hist->Clone(Form("%s_MperpUp",sysName.c_str()));
		doVariationMperp(histMperpUp,fitBin);
		
		TH1D* histMperpDown = (TH1D*)hist->Clone(Form("%s_MperpDown",sysName.c_str()));
		doVariationMperp(histMperpUp,fitBin,false);
		
		file->cd(dirName.c_str());	
		histRISRUp->Write();
		histRISRDown->Write();
		histMperpUp->Write();
		histMperpDown->Write();
	}

	file->Close();
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
	double maxVar = 50.;
	for(int r = 0; r < int(bins.size()); r++){
		// set variation for grouping of RISR bins here -> double var =
		// make sure this is based on the r # 
		// 	ie if RISRbins.size() == 11
		// 	midpoint = bin # 6 (int(RISRbins.size()/2) + 1)
		// 	if r < 6: vary by (maxVar/nBins)*(b-midpoint) % 
		double var;
		if(int(bins.size()) % 2 == 0 && r >= midpoint-1) var = ((maxVar/midpoint - 1)*(r+1 - midpoint + 1))/100;
		else var = ((maxVar/midpoint - 1)*(r+1 - midpoint))/100;
//		cout << "RISR bin #" << r << " var: " << var << endl;
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
	//make vector of vector of bin numbers (ie [[1,2],[3,4],[5]] -> RISR bins: [1,2,3])
	vector<vector<int>> bins;
	//for bins in one subvector: vary according to RISR bin (ie bin 1: -10%, bin 2: 0%, bin 3: +10%)	
	//get vector of Mperp bin edges -> loop through all bins and match bin edges 
	vector<VD> MbinEdges;
	for(int r = 0; r < NRbins; r++){
		MBins mBins = fb[r].Bins();
		MbinEdges.push_back(mBins.BinEdges());
		for(int m = 0; m < mBins.BinEdges().size(); m++){
			cout << "Rbin #" << r << " Mbin #" << m << " mBinEdges: " << mBins.BinEdges()[m] << endl;
		}
	}
	

	for(int m = 0; m < MbinEdges.size(); m++){
		VD edgesM = MbinEdges[m];
		vector<int> mbins;
		int ib = 0;
		for(int r = 0; r < NRbins; r++){
			const RBin rbin = fb[r];
			int NMbins = rbin.NBins();
			MBins mmbins = rbin.Bins();
			for(int mm = 0; mm < NMbins; mm++){
				if(mmbins.BinEdges() == edgesM) mbins.push_back(ib); 
				ib++;
			}
	
		}
		bins.push_back(mbins);
	}


	int midpoint = int(bins.size()/2) + 1;
	double maxVar = 50.;
	for(int r = 0; r < int(bins.size()); r++){
		// set variation for grouping of RISR bins here -> double var =
		// make sure this is based on the r # 
		// 	ie if RISRbins.size() == 11
		// 	midpoint = bin # 6 (int(RISRbins.size()/2) + 1)
		// 	if r < 6: vary by (maxVar/nBins)*(b-midpoint) % 
		double var;
		if(int(bins.size()) % 2 == 0 && r >= midpoint-1) var = ((maxVar/midpoint - 1)*(r+1 - midpoint + 1))/100;
		else var = ((maxVar/midpoint - 1)*(r+1 - midpoint))/100;
		cout << "Mperp bin #" << r << " var: " << var << endl;
		for(int b = 0; b < int(bins[r].size()); b++){
			if(isUp) hist->SetBinContent(b+1,hist->GetBinContent(b+1)*(1+var));
			else hist->SetBinContent(b+1,hist->GetBinContent(b+1)*(1-var));
		}
	
	}
}


//////////////////////////////
//shapeVariationTool
/////////////////////////////


shapeVariationTool::shapeVariationTool(CategoryTree ct, ProcessList procs, string file){
	m_ct = ct;
	m_fr = new FitReader(file);
	m_catList = m_fr->GetCategories();
	cout << "number of total cats:" << m_catList.GetN() << endl;
	m_nProc = procs.GetN();
	m_file = file;
	for(int i = 0; i < m_nProc; i++) m_proc += procs[i].Name();
	makeCatMaps();
}


shapeVariationTool::~shapeVariationTool(){
delete m_fr;
}


void shapeVariationTool::doVariations(){
	vector<const CategoryTree*> CTs;
	m_ct.GetListDepth(CTs,2);
	cout << "doVariations" << endl;
	cout << "CTs size: " << CTs.size() << endl;
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
			cout << pp.Name() << endl;
			for(int list = 0; list < int(CTs.size()); list++){
				cout << "list #" << list << endl;
				for(int i = 0; i < CTs[list]->GetMatchString().size(); i++) cout << CTs[list]->GetMatchString()[i] << endl;
				CategoryList cats = m_catList.Filter(*CTs[list]);
				cout << "nCats: " << cats.GetN() << endl;
				shapeVariation sv(cats, m_file);
				sv.doVariations(pp,m_Cats);				
			}
		}
	}
}

void shapeVariationTool::makeCatMaps(){
        vector<const CategoryTree*> catTrees;
        m_ct.GetListDepth(catTrees,2);
        FitReader fr(m_file);
        int nCatTrees = catTrees.size();
        CategoryList allCats = fr.GetCategories(); 
        for(int list = 0; list < nCatTrees; list++){
                CategoryList cats = allCats.Filter(*catTrees[list]);
                int nCats = cats.GetN();
                for(int c = 0; c < nCats; c++){
                        m_Cats[cats[c]] = catTrees[list]->GetSpecLabel();
                }
        }
}
