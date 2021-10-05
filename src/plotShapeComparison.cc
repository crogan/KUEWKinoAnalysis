#include <vector>
#include <algorithm>
#include <iostream>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>

#include "plotShapeComparison.hh"
#include "shapeComparison.hh"



plotShapeComparison::plotShapeComparison(vector<TH1D*> hists, vector<int> colors, VS labels){
  if(colors.size() < hists.size()){
    cout << "Error: number of hists doesn't match number of colors." << endl;
    int diff = fabs(hists.size() - colors.size());
    for(int i = 0; i < diff; i++){
      colors.push_back(kPink+7); 
    }
  }
  else if(labels.size() != hists.size()) cout << "Error: number of hists doesn't match number of labels." << endl;
  else if(colors.size() != labels.size()) cout << "Error: number of labels doesn't match number of colors." << endl;
  m_hists = hists;
  m_nHists = m_hists.size();
  m_histTotal = new TH1D(*m_hists[0]);
  for(int i = 1; i < m_nHists; i++) m_histTotal->Add(m_hists[i]);
  
  m_colors = colors;
  m_labels = labels;
  getLHs();
  getRatioPlots();
  getLHplots();
  //plotDist(m_binLHs,"binLHs","bin by bin likelihoods");

}

plotShapeComparison::~plotShapeComparison(){
  for(int i = 0; i < m_hists.size(); i++) delete m_hists[i];
}






void plotShapeComparison::getLHs(){
   for(int i = 0; i < m_nHists; i++){
      shapeComparison* sc = new shapeComparison(m_hists[i],m_histTotal);
      vector<double> binLHs;
      double pval = sc->getPvalue(binLHs);
      cout << "p-val: " << pval << " for hist " << m_hists[i]->GetTitle() << " and total \n" << endl;
      m_pvals.push_back(pval);
for(int val = 0; val < binLHs.size(); val++) m_binLHs.push_back(binLHs[val]);    //get p-values
      m_LHs.push_back(binLHs); //get likelihoods
      m_LHlabels.push_back("#splitline{"+m_labels[i]+" to total}{#scale[1.2]{p-value: "+std::to_string(pval)+"}}");
   }
}





//void plotShapeComparison::getLHs(){
//  for(int i = 0; i < m_nHists; i++){
//   for(int j = 0; j < m_nHists; j++){ 
//          if(i >= j) continue; //account for same combinations 
//      shapeComparison* sc = new shapeComparison(m_hists[i],m_hists[j]);
//      vector<double> binLHs;
////      double x;
//      double pval = sc->getPvalue(binLHs);
//      cout << "p-val: " << pval << " for hist " << m_hists[i]->GetTitle() << " and " << m_hists[j]->GetTitle() << "\n" << endl;
//      m_pvals.push_back(pval);
// //     if(m_histToFudgeFactor.count(m_hists[j]->GetTitle()) != 0) m_histToFudgeFactor[m_hists[j]->GetTitle()] = std::max(m_histToFudgeFactor[m_hists[j]->GetTitle()],x);
//   //   else m_histToFudgeFactor[m_hists[j]->GetTitle()] = x;
//
////for(int val = 0; val < binLHs.size(); val++){ m_binLHs.push_back(binLHs[val]); cout << "LH for bin #" << val+1 << ": "<< binLHs[val] << endl; }cout << "---------------" << endl;  //get p-values
//      m_LHs.push_back(binLHs); //get likelihoods
//      m_LHlabels.push_back("#splitline{"+m_labels[i]+" #scale[1.2]{to} "+m_labels[j]+"}{#scale[1.2]{p-value: "+std::to_string(pval)+"}}");
//    }
//  }
//
//
//}

void plotShapeComparison::getRatioPlots(){
  double scale;
  TH1D* histTotal = new TH1D(*m_hists[0]);
  int nBins = histTotal->GetNbinsX();
  for(int i = 1; i < m_nHists; i++) histTotal->Add(m_hists[i]);
 // cout << "histogram 0 has " << m_hists[0]->GetNbinsX() << " bins"  << endl;
 // for(int i = 0; i < m_nHists; i++){
 //cout << m_hists[i]->GetTitle() << endl;
 // for(int b = 0; b < m_hists[i]->GetNbinsX()+2; b++){ 
 // cout << "histogram #" << i << " bin #" << b << " value:" << m_hists[i]->GetBinContent(b) << endl;
 // }
 // }
 //for(int b = 0; b < histTotal->GetNbinsX()+2; b++) cout << "histTotal bin #" << b << " value: " << histTotal->GetBinContent(b) << endl;

  for(int gr = 0; gr < m_nHists; gr++){
	//set underflow and overflow bin contents to 0
	m_hists[gr]->SetBinContent(0,0.);
	m_hists[gr]->SetBinContent(m_hists[gr]->GetNbinsX()+1,0);
    TEfficiency* eff = new TEfficiency(*m_hists[gr],*histTotal);
    if(eff == NULL){ cout << "efficiency null" << endl; return;}
    eff->Draw("goff");
    gPad->Update();
  TGraphAsymmErrors* assymGraph = eff->GetPaintedGraph();
    if(assymGraph == NULL){ cout << "null asymmgraph" << endl; return;}
  double tmp_y; double tmp_x;
  scale = histTotal->Integral(1,nBins)/m_hists[gr]->Integral(1,nBins);
  tmp_y = 0; tmp_x = 0;
  for(int b = 0; b < assymGraph->GetN(); b++){
  assymGraph->GetPoint(b,tmp_x,tmp_y);
  if(histTotal->GetBinContent(b+1) < 1e-5){ 
	assymGraph->SetPoint(b,tmp_x,0.);
	continue;
  }
  
	//cout << "graph #" << gr <<" point #" << b << " x:"<< tmp_x << " y:" << tmp_y << " scaled y:" << tmp_y*scale << " scale: " << scale << endl;
  tmp_y *= scale;//*m_histToFudgeFactor[m_hists[gr]->GetTitle()];
  //cout << "scaled y: " << tmp_y << endl;
  assymGraph->SetPoint(b,tmp_x,tmp_y);
  }
assymGraph->Draw();
  m_graphs.push_back(assymGraph);
//double checkX; double checkY;
//for(int i = 0; i < m_graphs[gr]->GetN(); i++){m_graphs[gr]->GetPoint(i,checkX, checkY); cout << "gr #" << gr << " retrieved point #" << i << " x:" << checkX << " y:" << checkY << endl;} 
  }
}


void plotShapeComparison::getLHplots(){
  for(int gr = 0; gr < m_nHists; gr++){
if(m_LHs[gr].size() < 1) continue;
    TGraphAsymmErrors* gLH = new TGraphAsymmErrors(m_graphs[gr]->GetN());
    for(int i = 0; i < m_graphs[gr]->GetN(); i++){
      double x; double y;
      m_graphs[gr]->GetPoint(i,x,y);
      gLH->SetPoint(i,x,m_LHs[gr][i]); //likelihood for graph gr and bin i
      gLH->SetPointEXhigh(i,m_graphs[gr]->GetErrorXhigh(i));
      gLH->SetPointEXlow(i,m_graphs[gr]->GetErrorXlow(i));
    }
    m_gLHs.push_back(gLH);
  }
}


void plotShapeComparison::plotDist(std::vector<double> vals, string name, string title){
  TCanvas* cv = new TCanvas(name.c_str(),title.c_str(),600,600);
  double max = *std::max_element(std::begin(vals),std::end(vals));
  TH1D* hist = new TH1D(name.c_str(),title.c_str(),20,0.,max);
  for(int i = 0; i < vals.size(); i++) hist->Fill(vals[i]); 
  hist->GetXaxis()->SetTitle(name.c_str());
  hist->GetYaxis()->SetTitle("a.u.");
  hist->SetTitle(title.c_str());
  hist->Draw();
}



