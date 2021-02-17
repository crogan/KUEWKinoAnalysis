#include <vector>
#include <iostream>
#include "plotShapeComparison.hh"
#include "shapeComparison.hh"



plotShapeComparison::plotShapeComparison(vector<TH1D*> hists,vector<int> colors, VS labels){
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
          if(i <= j) continue; //account for same combinations 
      shapeComparison* sc = new shapeComparison(m_hists[i],m_hists[j]);
      double pval = sc->getPvalue();
      cout << "p-val: " << pval << " for hist " << m_hists[i]->GetTitle() << " and " << m_hists[j]->GetTitle() << endl;
      m_pvals.push_back(pval);
      m_LHs.push_back(sc->lambdas);
    }
  }


}

void plotShapeComparison::makePlots(){
  TH1D* histTotal = new TH1D(*m_hists[0]);

  for(int i = 1; i < m_nHists; i++) histTotal->Add(m_hists[i]);
  double scale;

  for(int i = 0; i < m_nHists; i++){
    TEfficiency* eff = new TEfficiency(m_hists[i],histTotal);
    TH1D* hist = eff->GetPaintedGraph()->GetHistogram();
    scale = histTotal->Integral()/m_hists[i]->Integral();
    hist->Scale(scale);
    m_graphs.push_back(new TGraphAsymmErrors(hist));
   

    Graph* gLH = new TGraph();
    for(int i = 0; i < m_graphs[gr]->GetN(); i++){
      double x = m_graphs[gr]->GetPointX(i);
      m_gLH->SetPoint(i,x,m_LHs[gr][i]); //likelihood for graph gr and bin i
    }
    m_gLHs.push_back(m_gLH);

  }
  
}


TCanvas* plotShapeComparison::formatGraphs(){

	gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = new TCanvas(Form("can_%s", name.c_str()),
           Form("can_%s", name.c_str()),
           1200, 700);
  double hlo = 0.09;
  double hhi = 0.22;
  double hbo = 0.27;
  double hto = 0.07;
  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->SetGridy();
  can->Draw();
  can->cd();

  TPad* pRatio = new TPad("pRatio","pRatio",0,0.35,1.,1.);
  pRatio->SetGridx();
  pRatio->SetGridy();
  pRatio->SetBottomMargin(0.02);
  pRatio->Draw();
  pRatio->cd();
  can->Update();

  TPad* pLH = new TPad("pLikelihood","pLikelihood",0,0.03,1,0.35-0.04);
  pLH->SetGridx();
  pLH->SetGridy();
  pLH->SetTopMargin(0.);
  pLH->SetBottomMargin(0.2);
  pLH->Draw();
  pLH->cd();
  pLH->Update();
  can->Update();

  pRatio->cd();

  m_graphs[0]->Draw();
  m_graphs[0]->GetXaxis()->CenterTitle();
  m_graphs[0]->GetXaxis()->SetTitleFont(42);
  m_graphs[0]->GetXaxis()->SetTitleSize(0.05);
  m_graphs[0]->GetXaxis()->SetTitleOffset(1.0);
  m_graphs[0]->GetXaxis()->SetLabelFont(42);
  m_graphs[0]->GetXaxis()->SetLabelSize(0.04);
  m_graphs[0]->GetXaxis()->SetTitle("");
  m_graphs[0]->GetXaxis()->SetTickSize(0.);
  m_graphs[0]->GetYaxis()->CenterTitle();
  m_graphs[0]->GetYaxis()->SetTitleFont(42);
  m_graphs[0]->GetYaxis()->SetTitleSize(0.04);
  m_graphs[0]->GetYaxis()->SetTitleOffset(0.85);
  m_graphs[0]->GetYaxis()->SetLabelFont(42);
  m_graphs[0]->GetYaxis()->SetLabelSize(0.035);
  m_graphs[0]->GetYaxis()->SetTitle("ratio to total");

  TLegend* leg = new TLegend(1.-hhi+0.01, 1.- (Nhist+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  // if(hist_data)
  // leg->AddEntry(hist_data, "data");
  // leg->AddEntry(gr, "total uncertainty","F");
  for(int i = 0; i < Nhist; i++)
  leg->AddEntry(m_graphs[i], labels[i].c_str(), "LP");
  // for(int i = 0; i < Nsig; i++)
  // leg->AddEntry(hists_sig[i], labels_sig[i].c_str(), "L");
 leg->Draw("SAME");



  for(int i = 0; i < m_nHists; i++){
  	graphs[i]->SetLineColor(colors[i]);
  	graphs[i]->SetMarkerColor(colors[i]);
  	graphs[i]->SetLineWidth(1.0);
  	graphs[i]->SetMarkerStyle(20+i);
  	graphs[i]->SetLineStyle(i);
  	// hists[i]->SetFillColor(colors[i]);
  	// hists[i]->SetFillStyle(1001);
  	graphs[i]->Draw("SAME");
  }

  pLH->cd();
  for(int i = 0; i < m_nHists; i++){
    m_gLHs[i]->SetLineColor(colors[i]);
    m_gLHs[i]->SetMarkerColor(colors[i]);
    m_gLHs[i]->SetLineWidth(1.0);
    m_gLHs[i]->SetMarkerStyle(20+i);
    m_gLHs[i]->SetLineStyle(i);
    m_gLHs[i]->Draw("SAME");
  }

  return can;
	

}