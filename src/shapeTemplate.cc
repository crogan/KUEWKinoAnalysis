#include "shapeTemplate.hh"
#include "shapeComparison.hh"
#include <stdlib.h>

shapeTemplate::shapeTemplate(TH1D* hist,TH1D* hist_cons, string ofile){
	if(hist == NULL){ std::cout << "Histogram not found." << std::endl; return;}

	m_hist_OG = (TH1D*)hist->Clone();
	m_hist_scaled = (TH1D*)hist->Clone();
	m_hist_rec = (TH1D*)hist->Clone();
	int m_nBins = hist->GetNbinsX();
	m_norm = hist->Integral(1,m_nBins);
	m_hist_cons = hist_cons;

	m_fileName = ofile;
	

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

	vector<pair<int,double>> m_binsSorted = sortBins(m_hist_scaled);
	vector<pair<int,double>> m_binsSorted_cons = sortBins(m_hist_cons);
	//first in the pair is the true bin index, second is the unweighted, normalized bin content

}

shapeTemplate::~shapeTemplate(){
	delete m_hist_OG;
	delete m_hist_rec;
	delete m_hist_cons;
	m_binsSorted.clear();	
}

void shapeTemplate::replaceBins(int idx, double Prem, double Prem_cons){
		if(idx > m_nBins) return;
		else{
			
		
			double bin = m_binsSorted[idx].second/Prem; //ratio of probability bw current bin and remaining probability
			double bin_cons = m_binsSorted_cons[idx]/Prem_cons;
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
	
	double pValue = compareShapes();
	setErrors();

	//write recreated histogram to file

	TFile* f = TFile::Open(m_fileName.c_str());
	m_hist_rec->Write();
	f->Close();

}

vector<pair<int,double>> shapeTemplate::sortBins(TH1D* hist){
	int N = hist->GetNbinsX();

	vector<pair<int,double>> array;
	for(int i = 0; i < N; i++){
		array.push_back(make_pair(i+1,hist->GetBinContent(i+1)));
	}

	// If the input array (histogram) contains fewer than two elements (bins),
	// then return array with only two elements (bins)
	if(N < 2)
	return array;

	vector<pair<int,double>> low;
	vector<pair<int,double>> same;
	vector<pair<int,double>> high;

	// Select your `pivot` element randomly
	double pivot = array[int(rand()*N)];

	for(int i = 0; i < N; i++){
		// Elements that are smaller than the `pivot` go to
		// the `low` list. Elements that are larger than
		// `pivot` go to the `high` list. Elements that are
		// equal to `pivot` go to the `same` list.
		if(array[i].second < pivot){
			low.push_back(array[i]);
		} else if(array[i] == pivot){
			same.push_back(array[i]);
		} else {
			high.push_back(array[i]);
		}
	}

	// The final result combines the sorted `low` list
	// with the `same` list and the sorted `high` list
	QuickSort(low);
	QuickSort(high);
	// recreate sorted array from pieces
	array.clear();
	for(auto val : high)
	array.push_back(val);
	for(auto val : same)
	array.push_back(val);
	for(auto val : low)
	array.push_back(val);
}
