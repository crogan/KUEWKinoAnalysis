#include <stdlib.h>
#include <iostream>

#include <TSystem.h>

#include "shapeTemplate.hh"
#include "shapeComparison.hh"
#include "FitReader.hh"
#include "CategoryTree.hh"



////////////////////////////////////////////////
//////////////////shapeTemplate class
////////////////////////////////////////////////


shapeTemplate::shapeTemplate(TH1D* hist, TH1D* hist_cons, Category cat, string ofile){
	//should take CategoryTree as input
	if(hist == NULL){ std::cout << "Histogram not found." << std::endl; return;}

	m_hist_OG = (TH1D*)hist->Clone();
	m_hist_scaled = (TH1D*)hist->Clone();
	m_hist_rec = (TH1D*)hist->Clone();
	int m_nBins = hist->GetNbinsX();
	m_norm = hist->Integral(1,m_nBins);
	m_hist_cons = hist_cons;
	m_cat = cat;

	m_fileName = ofile;
	m_dirName = cat.Label()+"_"+m_cat.GetLabel();
	

	//unweight histogram 
	double m_weight = 0;
	for(int i = 0; i < m_hist_scaled->GetNbinsX(); i++){
		double tmp_w = pow(m_hist_scaled->GetBinError(i+1),2)/m_hist_scaled->GetBinContent(i+1);
		if(tmp_w > m_weight) m_weight = tmp_w;
	}
	m_hist_scaled->Scale(1/m_weight);

	m_unwtNorm = m_hist_scaled->Integral(1,m_hist_scaled->GetNbinsX());

	//normalize histogram
	m_hist_scaled->Scale(1/m_unwtNorm);

	vector<pair<int,double>> array;
	for(int i = 0; i < m_nBins; i++){
		array.push_back(std::make_pair(i+1,m_hist_scaled->GetBinContent(i+1)));
	}
	vector<pair<int,double>> array_cons;
	for(int i = 0; i < m_nBins; i++){
		array_cons.push_back(std::make_pair(i+1,m_hist_cons->GetBinContent(i+1)));
	}
	vector<pair<int,double>> m_binsSorted = sortBins(array);
	vector<pair<int,double>> m_binsSorted_cons = sortBins(array_cons);
	//first in the pair is the true bin index, second is the unweighted, normalized bin content

}

shapeTemplate::~shapeTemplate(){
	delete m_hist_OG;
	delete m_hist_rec;
	delete m_hist_cons;
	m_binsSorted.clear();	
}

void shapeTemplate::replaceBins(int idx, double Prem, double Prem_cons){
	//idx is index of SORTED histogram; m_binsSorted[idx].first is index of true histogram
		if(idx > m_nBins) return;
		else{
			
		
			double bin = m_hist_OG->GetBinContent(m_binsSorted[idx].first);
			double bin_cons = m_binsSorted_cons[idx].second/Prem_cons;
			double bin_repl = bin_cons*Prem;
			//replace bin and error from consolidated if the normalized, unweighted bin content is less than a certain value
			if(bin < m_minVal){
				//replace bin
				m_hist_rec->SetBinContent(m_binsSorted[idx].first,bin_repl); 
				m_hist_rec->SetBinError(m_binsSorted[idx].first,m_hist_cons->GetBinError(m_binsSorted[idx].first));
			}

			//else take bin value and error from original histogram
			else{
				m_hist_rec->SetBinContent(m_binsSorted[idx].first,m_binsSorted[idx].second);
				m_hist_rec->SetBinError(m_binsSorted[idx].first,m_unwtNorm*m_hist_scaled->GetBinError(m_binsSorted[idx].first));
			}

			// double Prem = 0.; //m_hist_OG->Integral(idx+1,m_nBins); //this should be integral of sorted histogram
			// for(int i = idx; i < m_nBins; i++){
			// 	Prem += m_binsSorted[i].second;
			// }

			Prem -= m_binsSorted[idx].second;
			Prem_cons -= m_binsSorted_cons[idx].second;

			replaceBins(idx+1,Prem,Prem_cons);
		}
}


double shapeTemplate::compareShapes(){
	//check m_hist_rec is all filled

	shapeComparison* sc = new shapeComparison(m_hist_OG,m_hist_rec);
	double pval = sc->getPvalue();
	return pval;
}

void shapeTemplate::setErrors(){
	for(int b = 0; b < m_nBins; b++){
		double oldBin = m_hist_OG->GetBinContent(b+1);
		double newBin = m_hist_rec->GetBinContent(b+1);
		double oldErr = m_hist_rec->GetBinError(b+1);
		double newErr = m_hist_rec->GetBinError(b+1);

		double pull = (oldBin - newBin)/sqrt(oldErr*oldErr - newErr*newErr);

		if(pull > 1){
			newErr = sqrt(pow((oldBin - newBin),2) - oldErr*oldErr);
		}

		m_hist_rec->SetBinError(b+1,newErr);
	}

}

void shapeTemplate::replaceHistogram(){
	replaceBins();

	if(m_hist_rec->Integral() == 0.){ cout << "Error: recreated histogram empty." << endl; return;}
	
	m_hist_rec->Scale(m_norm);
	
	// double pValue = compareShapes();
	setErrors();

	//write recreated histogram to file

	TFile* f = TFile::Open(m_fileName.c_str());
	f->cd(m_dirName.c_str());
	m_hist_rec->Write();
	f->Close();

}




//vector<pair<int,double>> shapeTemplate::sortBins(TH1D* hist){
vector<pair<int,double>> shapeTemplate::sortBins(vector<pair<int,double>>& array){
	int N = array.size();


	// If the input array (histogram) contains fewer than two elements (bins),
	// then return array with only two elements (bins)
	if(N < 2)
	return array;

	vector<pair<int,double>> low;
	vector<pair<int,double>> same;
	vector<pair<int,double>> high;

	// Select your `pivot` element randomly
	double pivot = array[int(rand()*N)].second;

	for(int i = 0; i < N; i++){
		// Elements that are smaller than the `pivot` go to
		// the `low` list. Elements that are larger than
		// `pivot` go to the `high` list. Elements that are
		// equal to `pivot` go to the `same` list.
		if(array[i].second < pivot){
			low.push_back(array[i]);
		} else if(array[i].second == pivot){
			same.push_back(array[i]);
		} else {
			high.push_back(array[i]);
		}
	}

	// The final result combines the sorted `low` list
	// with the `same` list and the sorted `high` list
	sortBins(low);
	sortBins(high);
	// recreate sorted array from pieces
	array.clear();
	for(auto val : high)
	array.push_back(val);
	for(auto val : same)
	array.push_back(val);
	for(auto val : low)
	array.push_back(val);
}







////////////////////////////////////////////////
//////////////////shapeTemplateTool class
////////////////////////////////////////////////


shapeTemplateTool::shapeTemplateTool(const string& inputfile, const CategoryTree& CT, const VS& proc){
	m_file = inputfile;
	m_CT = CT;
	m_proc = proc;

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
	cout << "smoothHistograms" << endl;
	smoothHistograms();
	cout << "getHistograms" << endl;
	getHistograms();
	vector<double> pvals;

	std::map<string,TH1D*>::key_compare comp = m_nameToHist.key_comp();

//	cout << "map contains:\n";
//	string first = m_nameToHist.rbegin()->first;
//
//	std::map<string,TH1D*>::iterator it = m_nameToHist.begin();
//	do{
//		std::cout << it->first << " => " << it->second->GetName() << '\n';
//		}while( comp((*it++).first,first));
//	std::cout << '\n';

//	cout << m_histsAndLabels[0].first << " " << m_histsAndLabels[0].second << " " << m_nameToHist[m_histsAndLabels[0].second] << endl;
//	if(m_nameToHist[m_histsAndLabels[0].second] == NULL) cout << "null hist" << endl;
//cout << m_histsAndLabels[0].first->GetTitle() << endl;
//return;


//cout << "shapeTemplate" << endl;
//	for(int h = 0; h < m_histsAndLabels.size(); h++){
//		cout << "hist # " << h << " cons hist name: " << m_histsAndLabels[h].second <<  " " << m_nameToHist[m_histsAndLabels[h].second]->GetName() << endl;
//
//	if(m_nameToHist[m_histsAndLabels[h].second] == NULL){ cout << m_histsAndLabels[h].second << " not consolidated hist" << endl; return;}
//
//	
//				    	//individual hist, 			label -> to cons. hist
//		shapeTemplate st(m_histsAndLabels[h].first,m_nameToHist[m_histsAndLabels[h].second], m_file);
//		cout << "compareShapes" << endl;
//		pvals.push_back(st.compareShapes());
//		cout << "replaceHistogram" << endl;
//		st.replaceHistogram();
//	}
}

void shapeTemplateTool::getHistograms(){
	FitReader fitReader(m_file);
	vector<const CategoryTree*> catTrees;
	vector<TH1D*> hists;
	m_CT.GetListDepth(catTrees,1);
	//PrintCategories();
	TFile* f = nullptr;
	CategoryList catList = fitReader.GetCategories();
	if(!gSystem->AccessPathName(m_file.c_str()))
	 f = TFile::Open(m_file.c_str(),"UPDATE");
	else
	 f = new TFile(m_file.c_str(),"RECREATE");

	int depth = (int)catTrees.size();
	int nCat;
	int nProc = m_proc.size();

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
			for(int list = 0; list < depth; list++){
				CategoryList cats = catList.Filter(*catTrees[list]);
				nCat = cats.GetN();
				string slabel;
				//add hists
				for(int c = 0; c < nCat; c++){
					if(!fitReader.IsFilled(cats[c],pp)) continue;
					slabel = cats[c].GetLabel()+"_"+pp.Name();
		//			cout << "slabel: " << slabel << endl;
			//		m_histsAndLabels.push_back(std::make_pair((TH1D*)fitReader.GetHistogram(cats[c],pp),slabel));
					shapeTemplate st((TH1D*)fitReader.GetHistogram(cats[c],pp),m_nameToHist[slabel],cats[c], m_file);
			//		cout << "compareShapes" << endl;
			//		pvals.push_back(st.compareShapes());
			//		cout << "replaceHistogram" << endl;
//		st.replaceHistogram();
				}
			}
		}
	}
}




void shapeTemplateTool::smoothHistograms(){
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

  int depth = (int)catTrees.size();
  int nCat;
  int nProc = m_proc.size();
  
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
      for(int list = 0; list < depth; list++){
   CategoryList cats = catList.Filter(*catTrees[list]);
        nCat = cats.GetN();
        TH1D* totalHist = nullptr; //one total histogram per list per process
string slabel;
        //add hists
        for(int c = 0; c < nCat; c++){
			if(!fitReader.IsFilled(cats[c],pp)) continue;
			if(!totalHist) totalHist = (TH1D*)fitReader.GetHistogram(cats[c],pp)->Clone(Form("plothist_%d_%s", i, m_file.c_str()));
			else totalHist->Add(fitReader.GetHistogram(cats[c],pp));
			slabel = cats[c].GetLabel()+"_"+pp.Name();
			m_nameToNorm[slabel] = fitReader.GetHistogram(cats[c],pp)->Integral();
			m_nameToTitle[slabel] = fitReader.GetHistogram(cats[c],pp)->GetTitle();
			m_nameToHist[slabel] = totalHist;
        }
        //scale hists once all summed
        for(int c = 0; c < nCat; c++){
			if(!fitReader.IsFilled(cats[c],pp)) continue;
			slabel = cats[c].GetLabel()+"_"+pp.Name();
			m_nameToHist[slabel]->Scale(m_nameToNorm[slabel]/m_nameToHist[slabel]->Integral());
			m_nameToHist[slabel]->SetTitle(m_nameToTitle[slabel]);
			m_nameToHist[slabel]->SetName((pp.Name()+"_smoothed").c_str());
			//f->cd((cats[c].Label()+"_"+cats[c].GetLabel()).c_str());
			m_nameToHist[slabel]->Write();
        }

      } 
    }
  }

  f->Close();

}
