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
      //m_LHs.push_back(sc->getBinPvalues()); //plot p-values
      m_LHs.push_back(sc->lambdas); //plot likelihoods
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
//for(int i = 0; i < assymGraph->GetN(); i++) { assymGraph->GetPoint(i,tmp_x,tmp_y); cout << "TGraphAsymmErrors bin #" << i << " bin content: " << tmp_y << endl;}


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
for(int i = 0; i < gLH->GetN(); i++) { gLH->GetPoint(i,tmp_x,tmp_y); cout << "likelihood plot bin #" << i << " bin content: " << m_LHs[gr][i]<< endl;}
  }
}


//TCanvas* plotShapeComparison::formatPlots(const string& name){
//
//	gStyle->SetOptTitle(0);
//  gStyle->SetOptStat(0);
//  gStyle->SetOptFit(11111111);
//  TCanvas* can = new TCanvas(Form("can_%s", name.c_str()),
//           Form("can_%s", name.c_str()),
//           1200, 700);
//  double hlo = 0.09;
//  double hhi = 0.22;
//  double hbo = 0.27;
//  double hto = 0.07;
//  can->SetLeftMargin(hlo);
//  can->SetRightMargin(hhi);
//  can->SetBottomMargin(hbo);
//  can->SetTopMargin(hto);
//  can->SetGridy();
//  can->Draw();
//  can->cd();
//
//  TPad* pRatio = new TPad("pRatio","pRatio",0,0.35,1.,1.);
//  pRatio->SetGridx();
//  pRatio->SetGridy();
//  pRatio->SetBottomMargin(0.02);
//  pRatio->Draw();
//  pRatio->cd();
//  can->Update();
//
//  TPad* pLH = new TPad("pLikelihood","pLikelihood",0,0.03,1,0.35-0.04);
//  pLH->SetGridx();
//  pLH->SetGridy();
//  pLH->SetTopMargin(0.);
//  pLH->SetBottomMargin(0.2);
//  pLH->Draw();
//  pLH->cd();
//  pLH->Update();
//  can->Update();
//
//  pRatio->cd();
//
//  m_graphs[0]->Draw();
//  m_graphs[0]->GetXaxis()->CenterTitle();
//  m_graphs[0]->GetXaxis()->SetTitleFont(42);
//  m_graphs[0]->GetXaxis()->SetTitleSize(0.05);
//  m_graphs[0]->GetXaxis()->SetTitleOffset(1.0);
//  m_graphs[0]->GetXaxis()->SetLabelFont(42);
//  m_graphs[0]->GetXaxis()->SetLabelSize(0.04);
//  m_graphs[0]->GetXaxis()->SetTitle("");
//  m_graphs[0]->GetXaxis()->SetTickSize(0.);
//  m_graphs[0]->GetYaxis()->CenterTitle();
//  m_graphs[0]->GetYaxis()->SetTitleFont(42);
//  m_graphs[0]->GetYaxis()->SetTitleSize(0.04);
//  m_graphs[0]->GetYaxis()->SetTitleOffset(0.85);
//  m_graphs[0]->GetYaxis()->SetLabelFont(42);
//  m_graphs[0]->GetYaxis()->SetLabelSize(0.035);
//  m_graphs[0]->GetYaxis()->SetTitle("ratio to total");
//  for(int i = 0; i < m_nHists; i++){
//  	m_graphs[i]->SetLineColor(m_colors[i]);
//  	m_graphs[i]->SetMarkerColor(m_colors[i]);
//  	m_graphs[i]->SetLineWidth(1.0);
//  	m_graphs[i]->SetMarkerStyle(20+i);
//  	m_graphs[i]->SetLineStyle(i);
//  	// hists[i]->SetFillColor(colors[i]);
//  	// hists[i]->SetFillStyle(1001);
//  	m_graphs[i]->Draw("SAME");
//  }
//
//  TLegend* leg = new TLegend(1.-hhi+0.01, 1.- (m_nHists+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
//  leg->SetTextFont(42);
//  leg->SetTextSize(0.035);
//  leg->SetFillColor(kWhite);
//  leg->SetLineColor(kWhite);
//  leg->SetShadowColor(kWhite);
//  // if(hist_data)
//  // leg->AddEntry(hist_data, "data");
//  // leg->AddEntry(gr, "total uncertainty","F");
//  for(int i = 0; i < m_nHists; i++)
//  leg->AddEntry(m_graphs[i], m_labels[i].c_str(), "LP");
//  // for(int i = 0; i < Nsig; i++)
//  // leg->AddEntry(hists_sig[i], labels_sig[i].c_str(), "L");
// leg->Draw("SAME");
//
//  pLH->cd();
//  m_gLHs[0]->SetTitle("Likelihoods");
//  m_gLHs[0]->GetXaxis()->SetTitle("Bin number");
//  m_gLHs[0]->GetYaxis()->SetTitle("Log Likelihood");
//  for(int i = 0; i < m_nHists; i++){
//    m_gLHs[i]->SetLineColor(m_colors[i]);
//    m_gLHs[i]->SetMarkerColor(m_colors[i]);
//    m_gLHs[i]->SetLineWidth(1.0);
//    m_gLHs[i]->SetMarkerStyle(20+i);
//    m_gLHs[i]->SetLineStyle(i);
//    m_gLHs[i]->Draw("SAME");
//  }
//
//  return can;
//	
//
//}
