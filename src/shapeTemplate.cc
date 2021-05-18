#include <stdlib.h>
#include <iostream>
#include <random>

#include <TSystem.h>

#include "shapeTemplate.hh"
#include "shapeComparison.hh"
#include "FitReader.hh"
#include "CategoryTree.hh"
#include "Process.hh"


////////////////////////////////////////////////
//////////////////shapeTemplate class
////////////////////////////////////////////////


shapeTemplate::shapeTemplate(TH1D* hist, TH1D* hist_cons){
	if(hist == NULL){ std::cout << "Histogram not found." << std::endl; return;}

	m_hist_OG = (TH1D*)hist->Clone();
	m_hist_scaled = (TH1D*)hist->Clone();
	m_hist_rec = (TH1D*)hist->Clone();
	m_nBins = hist->GetNbinsX();
	m_norm = hist->Integral(1,m_nBins);
	m_hist_cons = (TH1D*)hist_cons->Clone();
	unweightHistogram(m_hist_scaled);
	unweightHistogram(m_hist_cons);
	normalizeHistogram(m_hist_scaled);
	normalizeHistogram(m_hist_cons);
}


shapeTemplate::~shapeTemplate(){
	delete m_hist_OG;
	delete m_hist_rec;
	delete m_hist_cons;
	m_binsSorted.clear();	
}

void shapeTemplate::sortHistograms(){
	for(int i = 0; i < m_nBins; i++){
		m_binsSorted.push_back(std::make_pair(i+1,m_hist_scaled->GetBinContent(i+1)));
	}
	vector<pair<int,double>> array_cons;
	for(int i = 0; i < m_nBins; i++){
		m_binsSorted_cons.push_back(std::make_pair(i+1,m_hist_cons->GetBinContent(i+1)));
	}
	sortBins(m_binsSorted);
	sortBins(m_binsSorted_cons);
}

void shapeTemplate::normalizeHistogram(TH1D* hist){
	double norm = hist->Integral(1,hist->GetNbinsX());
	hist->Scale(1/norm);
}

void shapeTemplate::unweightHistogram(TH1D* hist){
	double weight = 0;
	for(int i = 0; i < m_hist_scaled->GetNbinsX(); i++){
		double tmp_w = sqrt(pow(hist->GetBinError(i+1),2)/hist->GetBinContent(i+1));
		if(tmp_w > weight) weight = tmp_w;
	}
	if(weight == 0) return;
	else hist->Scale(1/weight);
}


void shapeTemplate::replaceBins(int idx, double Prem, double Prem_cons){
	//idx is index of SORTED histogram; m_binsSorted[idx].first is index of true histogram
		if(idx >= m_nBins) return;
			double bin = m_hist_OG->GetBinContent(m_binsSorted[idx].first);
			double err = m_hist_OG->GetBinError(m_binsSorted[idx].first);
			double effBin = (bin/err)*(bin/err);
			double bin_cons = m_binsSorted_cons[idx].second;
			double bin_repl = bin_cons*(Prem/Prem_cons);
			if(bin < m_minVal){
				//replace bin
				if(bin == 1e-8){
					m_hist_rec->SetBinContent(m_binsSorted[idx].first,bin_cons);
				}
				else{
					m_hist_rec->SetBinContent(m_binsSorted[idx].first,bin_repl); 
				}
				m_hist_rec->SetBinError(m_binsSorted[idx].first,m_hist_cons->GetBinError(m_binsSorted[idx].first));
			}
			//else take bin value and error from original histogram
			else{
				m_hist_rec->SetBinContent(m_binsSorted[idx].first,m_binsSorted[idx].second);
				m_hist_rec->SetBinError(m_binsSorted[idx].first,m_hist_scaled->GetBinError(m_binsSorted[idx].first));
			}

			Prem -= m_binsSorted[idx].second;
			Prem_cons -= m_binsSorted_cons[idx].second;
			replaceBins(idx+1,Prem,Prem_cons);
		
}


double shapeTemplate::compareShapes(){
	//check m_hist_rec is all filled
	if(m_hist_rec->Integral() < 1e-7){ cout << "Error: recreated histogram empty." << endl; return -999.;}
	for(int i = 1; i < m_hist_rec->GetNbinsX()+1; i++){
		double bin_rec = m_hist_rec->GetBinContent(i);
		double err_rec = m_hist_rec->GetBinError(i);
		double fracErr_rec = err_rec/bin_rec;
		double effBin_rec = fracErr_rec*fracErr_rec;
		double bin_OG = m_hist_OG->GetBinContent(i);
		double err_OG = m_hist_OG->GetBinError(i);
		double fracErr_OG = err_OG/bin_OG;
		double effBin_OG = fracErr_OG*fracErr_OG;
	}
	shapeComparison* sc = new shapeComparison(m_hist_OG,m_hist_rec);
	double pval = sc->getPvalue();
	return pval;
}

void shapeTemplate::setErrors(){
	for(int b = 0; b < m_nBins; b++){
		double oldBin = m_hist_OG->GetBinContent(b+1);
		double newBin = m_hist_rec->GetBinContent(b+1);
		double oldErr = m_hist_OG->GetBinError(b+1);
		double newErr = m_hist_rec->GetBinError(b+1);
		
		double err = newErr;// = m_hist_OG->GetBinError(b+1);
		double pull = (oldBin - newBin)/sqrt(oldErr*oldErr + newErr*newErr);
		if(std::fabs(pull) > 1){
			err = sqrt(pow((oldBin - newBin),2) - oldErr*oldErr);
		
		}
		else{
			err = newErr;//sqrt(newBin);
		}
		if(oldBin == 1e-8 && newErr > sqrt(newBin)) err = sqrt(newBin);
		if(newBin < 1e-6 && err == 0.) err = 0.;	
		m_hist_rec->SetBinError(b+1,err);
		
	}


}

TH1D* shapeTemplate::replaceHistogram(){
	sortHistograms();
	if(m_binsSorted.size() == 0) return NULL;
	//if original histogram is empty, just take that histogram as the replacement
	replaceBins();

	if(m_hist_rec->Integral() == 0.){ cout << "Error: recreated histogram empty." << endl; return NULL;}
	//scale to 1 once all bins have been replaced
	m_hist_rec->Scale(1/m_hist_rec->Integral());
	//scale to original histogram	
	m_hist_rec->Scale(m_norm);
	setErrors();
	return m_hist_rec;
}




void shapeTemplate::sortBins(vector<pair<int,double>>& array){
	int N = array.size();
	for(int i = 1; i < N; i++){
		pair<int,double> key_item = array[i];
		int j = i - 1;
		while(j >= 0 && array[j].second < key_item.second){
			array[j+1] = array[j];
			j -= 1;
		}
		array[j+1] = key_item;
	}
}





////////////////////////////////////////////////
//////////////////shapeTemplateTool class
////////////////////////////////////////////////



shapeTemplateTool::shapeTemplateTool(const CategoryTree& CT, ProcessList procs, TFile* inputfile){
	m_OutFile = inputfile;
	m_CT = CT;
	for(int i = 0; i < procs.GetN(); i++) m_proc += procs[i].Name();

	m_domToRare["ttbar_Fakes_elf0"] = VS().a("ST_Fakes_elf0"); 
	m_domToRare["ttbar_Fakes_elf1"] = VS().a("ST_Fakes_elf1"); 
	m_domToRare["ttbar_Fakes_muf0"] = VS().a("ST_Fakes_muf0"); 
	m_domToRare["ttbar_Fakes_muf1"] = VS().a("ST_Fakes_muf1"); 

	m_domToRare["Wjets_Fakes_elf0"] = VS().a("TB_Fakes_elf0").a("DB_Fakes_elf0"); 
	m_domToRare["Wjets_Fakes_elf1"] = VS().a("TB_Fakes_elf1").a("DB_Fakes_elf1"); 
	m_domToRare["Wjets_Fakes_muf0"] = VS().a("TB_Fakes_muf0").a("DB_Fakes_muf0"); 
	m_domToRare["Wjets_Fakes_muf1"] = VS().a("TB_Fakes_muf1").a("DB_Fakes_muf1"); 
	//m_proc = proc;
//	m_OutFile = nullptr;

}

shapeTemplateTool::~shapeTemplateTool(){

}


VS shapeTemplateTool::getProcess(){
	return m_proc;
}


CategoryTree shapeTemplateTool::getCategoryTree(){
	return m_CT;
}



void shapeTemplateTool::createTemplates(bool smooth){
string file = m_OutFile->GetName();
m_OutFile->Close();
FitReader fitReader(file);
vector<const CategoryTree*> catTrees;
m_CT.GetListDepth(catTrees,1);
//PrintCategories();
  CategoryList catList = fitReader.GetCategories();
//  if(!gSystem->AccessPathName(m_file.c_str()))
//     f = TFile::Open(m_file.c_str(),"UPDATE");
//  else
//     f = new TFile(m_file.c_str(),"RECREATE");
//if(m_OutFile){
//    if(m_OutFile->IsOpen()){
//      m_OutFile->Close();
//}
//    delete m_OutFile;
//  }

  m_OutFile = new TFile(file.c_str(), "UPDATE");
 if(!m_OutFile->IsOpen()){
	cout << "outfile is not open" << endl;  
  m_OutFile = nullptr;
    return;
  }
  int depth = (int)catTrees.size();
  int nCat;
  int nProc = m_proc.size();
  vector<TH1D*> totalHists; 

for(int i = 0; i < nProc; i++){
    VS vproc;
    if(fitReader.m_Strings.count(m_proc[i]) != 0)
      vproc = fitReader.m_Strings[m_proc[i]];
    else
      vproc += m_proc[i];
    for(int p = 0; p < int(vproc.size()); p++){
      int index = fitReader.GetProcesses().Find(vproc[p]);
      if(index < 0) continue;
      Process pp = fitReader.GetProcesses()[index];
      vector<int> ridxs;
      for(int r = 0; r < m_domToRare[vproc[p]].size(); r++)
      ridxs.push_back(fitReader.GetProcesses().Find(m_domToRare[vproc[p]][r]));
      for(int list = 0; list < depth; list++){
 CategoryList cats = catList.Filter(*catTrees[list]);
	nCat = cats.GetN();
//cout << "shapeTemplate category " << catTrees[list]->GetSpecLabel()  << " nCats: " << nCat<< endl;  
       	ProcessList ppp;
	ppp += pp;

	//if process pp == ST or TB or DB, combine with another background (ST with ttbar, TB+DB with Wjets)
	if(ridxs.size() > 0) {
        	for(int r = 0; r < ridxs.size(); r++){
			if(ridxs[r] < 0) continue;
 			ppp += fitReader.GetProcesses()[ridxs[r]];
	
		}
	
	}	
	 
	//one total hist per list per process
	TH1D* totalHist = fitReader.GetAddedHist(pp.Name()+"_smoothed",cats,ppp);
	if(totalHist == nullptr) continue;
	
	m_listToHist[list] = totalHist;
	m_listToHist[list]->SetName((pp.Name()+"_smoothed").c_str());
	//scale hists once all summed
	for(int c = 0; c < nCat; c++){
	//cout << "cat #" << c << endl;
		if(!fitReader.IsFilled(cats[c],pp)) continue;
		TH1D* oldHist = (TH1D*)fitReader.GetHistogram(cats[c],pp);
		if(oldHist->GetName() == "data_obs") continue;
		TH1D* newHist;
		oldHist->SetName((pp.Name()+"_raw").c_str());
	 	//continue;
		if(oldHist->Integral() > 1e-6){ 
			shapeTemplate st(oldHist,m_listToHist[list]); 
			newHist = (TH1D*)st.replaceHistogram();
			st.compareShapes();
			newHist->SetName((pp.Name()).c_str());
			oldHist->SetName((pp.Name()+"_raw").c_str());
			m_OutFile->cd((cats[c].Label()+"_"+cats[c].GetLabel()).c_str());
			newHist->Write();
			oldHist->Write();
//cout << "write new hist and rename old hist cat: " << cats[c].Label()+"_"+cats[c].GetLabel()+"/"+pp.Name() << endl;
		}	
		else{
			newHist = oldHist; 
			if(newHist == NULL) { cout << "newHist null" << endl; return;}
			newHist->SetName((pp.Name()).c_str());
			oldHist->SetName((pp.Name()+"_raw").c_str());
			m_OutFile->cd((cats[c].Label()+"_"+cats[c].GetLabel()).c_str());
			newHist->Write();
			oldHist->Write();
//cout << "write new hist and rename old hist cat: " << cats[c].Label()+"_"+cats[c].GetLabel()+"/"+pp.Name() << endl;
        	}
		if(smooth){
			m_OutFile->cd((cats[c].Label()+"_"+cats[c].GetLabel()).c_str());
			m_listToHist[list]->Write();
		}
	}
    }



       
    }
  }

//cout << "shapeTemplate::createTemplates end" << endl;
 m_OutFile->Close();

}
