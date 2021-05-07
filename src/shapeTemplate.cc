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
//	cout << "create shapeTemplate" << endl;
	if(hist == NULL){ std::cout << "Histogram not found." << std::endl; return;}

	m_hist_OG = (TH1D*)hist->Clone();
	m_hist_scaled = (TH1D*)hist->Clone();
	m_hist_rec = (TH1D*)hist->Clone();
	m_nBins = hist->GetNbinsX();
	m_norm = hist->Integral(1,m_nBins);
	m_hist_cons = (TH1D*)hist_cons->Clone();
	//cout << hist->GetName() << endl;
	//for(int i = 0; i < hist->GetNbinsX(); i++) cout << "bin #" << i+1 << ": " << m_hist_OG->GetBinContent(i+1) << endl;
	
//	cout << "name: " << m_hist_OG->GetName() << " title: " << m_hist_OG->GetTitle() << endl;
//	string recName = std::string(m_hist_OG->GetName())+"_recreated"; 
//	m_hist_rec->SetName(recName.c_str());//set m_hist_rec's Name to name of OG+"_recreated"	
//	cout << "set member histograms" << endl;
	//cout << "raw integral: " << m_norm << endl;
	//cout << "raw histogram: " << m_hist_scaled->GetBinContent(m_idx) << endl;
	//cout << "raw cons histogram: " << m_hist_cons->GetBinContent(m_idx) << endl;

	unweightHistogram(m_hist_scaled);
	unweightHistogram(m_hist_cons);
	//cout << "unweighted histogram: " << m_hist_scaled->GetBinContent(m_idx) << endl;
	//cout << "unweighted cons histogram: " << m_hist_cons->GetBinContent(m_idx) << endl;
	//cout << "unweighted integral: " << m_hist_scaled->Integral() << endl;
	//cout << "unweighted cons integral: " << m_hist_cons->Integral() << endl;
//cout << "unweighted histograms" << endl;
//	//normalize histogram
	normalizeHistogram(m_hist_scaled);
	normalizeHistogram(m_hist_cons);
//	cout << "normalized histogram: " << m_hist_scaled->GetBinContent(m_idx) << endl;
//	cout << "normalized cons histogram: " << m_hist_cons->GetBinContent(m_idx) << endl;
//cout << "normalized histogram" << endl;
//	cout << m_hist_scaled->GetBinContent(1) << endl;
}


shapeTemplate::~shapeTemplate(){
	delete m_hist_OG;
	delete m_hist_rec;
	delete m_hist_cons;
	m_binsSorted.clear();	
}

void shapeTemplate::sortHistograms(){
//cout << "sortHistograms" << endl;
	vector<pair<int,double>> array;
	for(int i = 0; i < m_nBins; i++){
		//cout << "array entry #" << i << endl;
		array.push_back(std::make_pair(i+1,m_hist_scaled->GetBinContent(i+1)));
	}
	vector<pair<int,double>> array_cons;
	for(int i = 0; i < m_nBins; i++){
		array_cons.push_back(std::make_pair(i+1,m_hist_cons->GetBinContent(i+1)));
	}
	sortBins(array);
	sortBins(array_cons);
	m_binsSorted = array;//sortBins(array);
	m_binsSorted_cons = array_cons;//sortBins(array_cons);
	//first in the pair is the true bin index, second is the unweighted, normalized bin content
//cout << "created index/bin content pairs" << endl;



}

void shapeTemplate::normalizeHistogram(TH1D* hist){
	double norm = hist->Integral(1,hist->GetNbinsX());
	hist->Scale(1/norm);
}

void shapeTemplate::unweightHistogram(TH1D* hist){
//cout << "unweightHistogram " << hist->GetName() << endl;
	double weight = 0;
	for(int i = 0; i < m_hist_scaled->GetNbinsX(); i++){
		double tmp_w = sqrt(pow(hist->GetBinError(i+1),2)/hist->GetBinContent(i+1));
		if(tmp_w > weight) weight = tmp_w;
	}
if(weight == 0) return;
//for(int i = 0; i < m_hist_scaled->GetNbinsX(); i++)
//cout << "bin #" << i << " error: " << hist->GetBinError(i+1) << " bin content: " << hist->GetBinContent(i+1) << endl;
//	cout << "max weight: " << weight << endl;
//	cout << "scale histogram by " << 1/weight << endl;
//}
else	hist->Scale(1/weight);
}


void shapeTemplate::replaceBins(int idx, double Prem, double Prem_cons){
	//idx is index of SORTED histogram; m_binsSorted[idx].first is index of true histogram
		if(idx >= m_nBins) return;
			double bin = m_hist_OG->GetBinContent(m_binsSorted[idx].first);
			double err = m_hist_OG->GetBinError(m_binsSorted[idx].first);
			double effBin = (bin/err)*(bin/err);
			double bin_cons = m_binsSorted_cons[idx].second;
			double bin_repl = bin_cons*(Prem/Prem_cons);
	if(m_binsSorted[idx].first == m_idx){
	//	cout << "sorted histogram: " << m_binsSorted[idx].second << endl;
	//	cout << "bin to compare to minVal: " << bin << endl;
	//	cout << "sorted cons histogram: " << m_binsSorted_cons[idx].second << endl;
	//	cout << "Prem: " << Prem << " Prem_cons: " << Prem_cons << endl;
	}
			//cout <<  "idx: " << idx << " bin #: " << m_binsSorted[idx].first <<  " OG bin value: " << bin << " bin_repl: " << bin_repl  << " bin_cons normalized: " << bin_cons << " Prem: " << Prem << " m_binSorted_cons: " << m_binsSorted_cons[idx].second << endl; 	

			//replace bin and error from consolidated if the normalized, unweighted bin content is less than a certain value
			if(bin < m_minVal){
	//			cout << "replaced bin #" << m_binsSorted[idx].first << " original: " << m_binsSorted[idx].second << " set to: " << bin_repl << endl;
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
	//			cout << "did not replaced bin #" << m_binsSorted[idx].first << " set content to: " << m_binsSorted[idx].second  << endl;
				m_hist_rec->SetBinContent(m_binsSorted[idx].first,m_binsSorted[idx].second);
				m_hist_rec->SetBinError(m_binsSorted[idx].first,m_hist_scaled->GetBinError(m_binsSorted[idx].first));
			}

			Prem -= m_binsSorted[idx].second;
			Prem_cons -= m_binsSorted_cons[idx].second;
			replaceBins(idx+1,Prem,Prem_cons);
		
}


double shapeTemplate::compareShapes(){
	//check m_hist_rec is all filled
//cout << "compareShapes" << endl;
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
	//cout << "bin #" << i << endl;
	//cout << "rec bin - " << bin_rec << " rec err - " << err_rec << " | og bin - " << bin_OG << " OG err - " << err_OG << endl;
	//cout <<  "rec effective bin count - " << effBin_rec << " rec frational error - " << fracErr_rec << " | og effective bin count - " << effBin_OG << " og fractional error - " << fracErr_OG << endl;
	}
	shapeComparison* sc = new shapeComparison(m_hist_OG,m_hist_rec);
	double pval = sc->getPvalue();
//cout << m_hist_rec->GetTitle() << " pval: " << pval << endl;
	return pval;
}

void shapeTemplate::setErrors(){
//cout << "setErrors" << endl;
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
		//cout << "bin #" << b << " old bin: " << oldBin << " recBin: " << newBin << " oldErr: " << oldErr << " recErr: " << newErr << " pull: " << pull << " new error: " << err << endl; 
//err = m_hist_cons->GetBinError(b+1); cout << "bin #" << b+1 << " error: " << err << endl;}
		if(newBin < 1e-6 && err == 0.) err = 0.;	
		m_hist_rec->SetBinError(b+1,err);
		
	}


}

TH1D* shapeTemplate::replaceHistogram(){
	sortHistograms();
	if(m_binsSorted.size() == 0) return NULL;
	//if original histogram is empty, just take that histogram as the replacement
	replaceBins();

	//cout << "recreated histogram: " << m_hist_rec->GetBinContent(m_idx) << endl;
	//cout << "cons histogram: " << m_hist_cons->GetBinContent(m_idx) << endl;
	if(m_hist_rec->Integral() == 0.){ cout << "Error: recreated histogram empty." << endl; return NULL;}
	//scale to 1 once all bins have been replaced
	//cout << "scale back to 1 with: " << m_hist_rec->Integral() << endl;
	m_hist_rec->Scale(1/m_hist_rec->Integral());
	//cout << "normalized to 1 histogram: " << m_hist_rec->GetBinContent(m_idx) << endl;
	//scale to original histogram	
	//cout << "original norm: " << m_norm << endl;
	m_hist_rec->Scale(m_norm);
	setErrors();
	//cout << "normalized to data histogram: " << m_hist_rec->GetBinContent(m_idx) << endl;
	return m_hist_rec;
}




void shapeTemplate::sortBins(vector<pair<int,double>>& array){
	//cout << "sortBins" << endl;
	int N = array.size();
	//cout << "array size: " << N << endl;
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


shapeTemplateTool::shapeTemplateTool(const string& inputfile, const CategoryTree& CT, const VS& proc){
	m_file = inputfile;
	m_CT = CT;
	m_proc = proc;

}

shapeTemplateTool::shapeTemplateTool(const string& inputfile, const CategoryTree& CT, ProcessList procs){
	m_file = inputfile;
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

}

shapeTemplateTool::~shapeTemplateTool(){

}


VS shapeTemplateTool::getProcess(){
	return m_proc;
}


CategoryTree shapeTemplateTool::getCategoryTree(){
	return m_CT;
}



void shapeTemplateTool::createTemplates(){
FitReader fitReader(m_file);
vector<const CategoryTree*> catTrees;
m_CT.GetListDepth(catTrees,1);
//PrintCategories();
TFile* f = nullptr;
  CategoryList catList = fitReader.GetCategories();
  if(!gSystem->AccessPathName(m_file.c_str()))
     f = TFile::Open(m_file.c_str(),"UPDATE");
  else
     f = new TFile(m_file.c_str(),"RECREATE");
//f->ls();
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
   //cout << pp.Name() << endl;
      vector<int> ridxs;
      for(int r = 0; r < m_domToRare[vproc[p]].size(); r++)
      ridxs.push_back(fitReader.GetProcesses().Find(m_domToRare[vproc[p]][r]));
      for(int list = 0; list < depth; list++){
   CategoryList cats = catList.Filter(*catTrees[list]);
//	cats.Print();
	//cout << "\n" << endl;
	nCat = cats.GetN();
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
	if(totalHist == nullptr) continue;//cout << "null consolidated histogram" << endl; continue;}
	
	m_listToHist[list] = totalHist;
	m_listToHist[list]->SetName((pp.Name()+"_smoothed").c_str());
	//scale hists once all summed
	for(int c = 0; c < nCat; c++){
		if(!fitReader.IsFilled(cats[c],pp)) continue;
		TH1D* oldHist = (TH1D*)fitReader.GetHistogram(cats[c],pp);
		TH1D* newHist;
		oldHist->SetName((pp.Name()+"_raw").c_str());
	 	continue;
	}
	//cout << cats[c].Label() << "_" << cats[c].GetLabel() << endl;
		//m_listToHist[list]->Scale(oldHist->Integral()/m_listToHist[list]->Integral());
		if(oldHist->Integral() > 1e-6){ 
			shapeTemplate st(oldHist,m_listToHist[list]); 
			newHist = (TH1D*)st.replaceHistogram();
			st.compareShapes();
			newHist->SetName((pp.Name()).c_str());
			oldHist->SetName((pp.Name()+"_raw").c_str());
			f->cd((cats[c].Label()+"_"+cats[c].GetLabel()).c_str());
			newHist->Write();
			oldHist->Write();
		}	
		else{
			newHist = oldHist; 
			if(newHist == NULL) { cout << "newHist null" << endl; return;}
			newHist->SetName((pp.Name()).c_str());
			oldHist->SetName((pp.Name()+"_raw").c_str());
			f->cd((cats[c].Label()+"_"+cats[c].GetLabel()).c_str());
			newHist->Write();
			oldHist->Write();
        	}
		f->cd((cats[c].Label()+"_"+cats[c].GetLabel()).c_str());
		m_listToHist[list]->Write();
	}
    }



       
    }
  }

  f->Close();

}
