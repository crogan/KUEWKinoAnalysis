#include <vector>
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
  m_colors = colors;
  m_labels = labels;
  getLHs();
  makePlots();
  plotPvalues(m_pvals,"Overall p-values");
  plotPvalues(m_binPvals,"Bin-by-bin p-values");


}

plotShapeComparison::~plotShapeComparison(){
  for(int i = 0; i < m_hists.size(); i++) delete m_hists[i];
}




void plotShapeComparison::getLHs(){
  for(int i = 0; i < m_nHists; i++){
   for(int j = 0; j < m_nHists; j++){ 
          if(i >= j) continue; //account for same combinations 
      shapeComparison* sc = new shapeComparison(m_hists[i],m_hists[j],false);
      double pval = sc->getPvalue();
      cout << "p-val: " << pval << " for hist " << m_hists[i]->GetTitle() << " and " << m_hists[j]->GetTitle() << endl;
      m_pvals.push_back(pval);
      vector<double> binPvals = sc->getBinPvalues();
      for(int val = 0; val < binPvals.size(); val++) m_binPvals.push_back(binPvals[i]); //get p-values
      m_LHs.push_back(binPvals); //get likelihoods
      m_LHlabels.push_back("#splitline{"+m_labels[i]+" #scale[1.2]{to} "+m_labels[j]+"}{#scale[1.2]{p-value: "+std::to_string(pval)+"}}");
    }
  }


}

void plotShapeComparison::makePlots(){
  double scale;
  TH1D* histTotal = new TH1D(*m_hists[0]);

  for(int i = 1; i < m_nHists; i++) histTotal->Add(m_hists[i]);


  for(int gr = 0; gr < m_nHists; gr++){
    TEfficiency* eff = new TEfficiency(*m_hists[gr],*histTotal);
    if(eff == NULL){ cout << "efficiency null" << endl; return;}
    eff->Draw("goff");
    gPad->Update();
  TGraphAsymmErrors* assymGraph = eff->GetPaintedGraph();
    if(assymGraph == NULL){ cout << "null asymmgraph" << endl; return;}
  double tmp_y; double tmp_x;
  scale = histTotal->Integral()/m_hists[gr]->Integral();
  tmp_y = 0; tmp_x = 0;
  for(int b = 0; b < assymGraph->GetN(); b++){
  assymGraph->GetPoint(b,tmp_x,tmp_y);
  assymGraph->SetPoint(b,tmp_x,tmp_y*scale);
  }


  m_graphs.push_back(assymGraph);
    TGraphAsymmErrors* gLH = new TGraphAsymmErrors(m_graphs[gr]->GetN());
    for(int i = 0; i < m_graphs[gr]->GetN(); i++){
      double x; double y;
      m_graphs[gr]->GetPoint(i,x,y);
      gLH->SetPoint(i,x,m_LHs[gr][i]); //likelihood for graph gr and bin i
      gLH->SetPointEXhigh(i,m_graphs[gr]->GetErrorXhigh(i));
      gLH->SetPointEXlow(i,m_graphs[gr]->GetErrorXhigh(i));
    }
    m_gLHs.push_back(gLH);
  }
}


void plotShapeComparison::plotPvalues(std::vector<doule> pvals, string title){
  TCanvas* cv = new TCanvas("cv","cv",600,800);
  TH1D* hist = new TH1D("pvals",title.c_str(),0,1,20);
  for(int i = 0; i < pvals.size(); i++) hist->Fill(pvals[i]);
  hist->GetXaxis()->SetLabel("p-value");
  hist->GetYaxis()->SetLabel("a.u.");
  hist->Draw();

}



