#include <iostream>
#include <map>
#include <TStyle.h>
#include <TROOT.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLine.h>

#include "FitPlotter.hh"

#include "RestFrames/RestFrames.hh"

///////////////////////////////////////////
////////// FitPlotter class
///////////////////////////////////////////

FitPlotter::FitPlotter(const string& inputfile,
		       const string& otherfile,
		       const string& otherfold)
  : FitReader(inputfile) {

  InitializeRecipes();
}

FitPlotter::~FitPlotter(){
  
}

TGraphErrors* FitPlotter::GetTotalBackground(const CategoryList& cat){
  TH1D* hist = nullptr;
  int Ncat = cat.GetN();
  for(int i = 0; i < Ncat; i++){
    string shist = m_FileFold+cat[i].Label()+"_"+cat[i].GetLabel()+"/total_background";
    cout << shist << endl;
    if(hist == nullptr)
      hist = (TH1D*) m_FilePtr->Get((m_FileFold+"/"+shist).c_str())->Clone((shist+"_total").c_str());
    else
      hist->Add((TH1D*) m_FilePtr->Get((m_FileFold+"/"+shist).c_str()));
  }

  int NB = hist->GetNbinsX();
  
  vector<double> X;
  vector<double> Xerr;
  vector<double> Y;
  vector<double> Yerr;
  for(int i = 0; i < NB; i++){
    X.push_back(0.5 + i);
    Xerr.push_back(0.5);
    Y.push_back(hist->GetBinContent(i+1));
    Yerr.push_back(hist->GetBinError(i+1));
  }
   
  TGraphErrors* gr = new TGraphErrors(NB, &X[0], &Y[0],  &Xerr[0], &Yerr[0]);

  return gr;
   
}

string FitPlotter::GetSignalTitle(const string& label){
  size_t p = label.rfind("_");
  if(p == std::string::npos)
    return label;

  string title = label.substr(0, p);
  int    mass  = stoi(label.substr(p+1,label.length()-p));
  return title+" "+std::to_string(mass/10000)+" "+std::to_string(mass%100000);
}

TCanvas* FitPlotter::Plot1Dstack(const VS& proc,
				const VS& lep_cat,
				const VS& hadS_cat,
				const VS& hadI_cat,
				const string& name){
  RestFrames::SetStyle();

  
  int Nproc = proc.size();
  int Nlep  = lep_cat.size();
  int NhadS = hadS_cat.size();
  int NhadI = hadI_cat.size();
  if(Nproc == 0 ||
     Nlep  == 0 ||
     NhadS == 0 ||
     NhadI == 0)
    return nullptr;

  CategoryList cat = GetCategories();
  cat.Print();
  
  // Leptonic
  VS lep_labels;
  VS vlep;
  for(int i = 0; i < Nlep; i++){
    if(m_Title.count(lep_cat[i]) != 0)
      lep_labels.push_back(m_Title[lep_cat[i]]);
    else
      lep_labels.push_back(lep_cat[i]);

    if(m_Strings.count(lep_cat[i]) != 0){
      int N = m_Strings[lep_cat[i]].size();
      for(int j = 0; j < N; j++){
	vlep.push_back(m_Strings[lep_cat[i]][j]);
      }
    } else {
      vlep.push_back(lep_cat[i]);
    }
  }

  cat = cat.FilterOR(vlep);

  // Hadronic S
  VS hadS_labels;
  VS vhadS;
  for(int i = 0; i < NhadS; i++){
    if(m_Title.count(hadS_cat[i]) != 0)
      hadS_labels.push_back(m_Title[hadS_cat[i]]);
    else
      hadS_labels.push_back(hadS_cat[i]);

    if(m_Strings.count(hadS_cat[i]) != 0){
      int N = m_Strings[hadS_cat[i]].size();
      for(int j = 0; j < N; j++)
	vhadS.push_back(m_Strings[hadS_cat[i]][j]);
    } else {
      vhadS.push_back(hadS_cat[i]);
    }
  }

  cat = cat.FilterOR(vhadS);

  // Hadronic ISR
  VS hadI_labels;
  VS vhadI;
  for(int i = 0; i < NhadI; i++){
    if(m_Title.count(hadI_cat[i]) != 0)
      hadI_labels.push_back(m_Title[hadI_cat[i]]);
    else
      hadI_labels.push_back(hadI_cat[i]);

    if(m_Strings.count(hadI_cat[i]) != 0){
      int N = m_Strings[hadI_cat[i]].size();
      for(int j = 0; j < N; j++)
	vhadI.push_back(m_Strings[hadI_cat[i]][j]);
    } else {
      vhadI.push_back(hadI_cat[i]);
    }
  }

  cat = cat.FilterOR(vhadI);

  int Ncat = cat.GetN();
  
  if(Ncat < 1)
    return nullptr;
  
  // Processes
  VS            labels;
  vector<int>   colors;
  vector<TH1D*> hists;
  
  VS             labels_sig;
  vector<TH1D*>  hists_sig;

  TH1D* hist_data = nullptr;
  
  for(int i = 0; i < Nproc; i++){
    VS vproc;
    if(m_Strings.count(proc[i]) != 0)
      vproc = m_Strings[proc[i]];
    else
      vproc += proc[i];

    ProcessType type = kBkg;
    TH1D*       hist = nullptr;
    for(int p = 0; p < int(vproc.size()); p++){
      
      int index = GetProcesses().Find(vproc[p]);
      if(index < 0)
	continue;
      
      Process pp = GetProcesses()[index];

      if(pp.Type() == kSig)
	type = kSig;
      if(pp.Type() == kData)
	type = kData;
      
      for(int c = 0; c < Ncat; c++){
	cout << cat[c].GetLabel() << " " << pp.Name() << endl;
	if(!IsFilled(cat[c], pp))
	  continue;

	cout << "filled " << cat[c].GetLabel() << " " << pp.Name() << endl;
	
	if(!hist){
	  hist = (TH1D*) GetHistogram(cat[c], pp)->Clone(Form("plothist_%d_%s", i, name.c_str()));
	} else {
	  hist->Add(GetHistogram(cat[c], pp));
	}
      }
    }

    if(hist == nullptr)
      continue;
    
    if(type == kData){
      hist_data = hist;
    }

    if(type == kSig){
      labels_sig.push_back(GetSignalTitle(proc[i]));
      
      hists_sig.push_back(hist);
    }

    if(type == kBkg){
      if(m_Title.count(proc[i]) != 0)
	labels += m_Title[proc[i]];
      else
	labels += proc[i];
      
      if(m_Color.count(proc[i]) != 0)
	colors.push_back(m_Color[proc[i]]);
      else
	colors.push_back(m_ColorDefault[i]);
      
      hists.push_back(hist);
    } 
  }

  int Nsig = hists_sig.size();
  
  // sort the histograms by integral (N^2/2 brute force)
  int Nbkg = hists.size();
  VS             vlabels;
  vector<int>    vcolors;
  vector<TH1D*>  vhists;
  string stemp;
  int    itemp;
  TH1D*  htemp;
  
  for(int i = 0; i < Nbkg; i++){
    vlabels.push_back(labels[i]);
    vcolors.push_back(colors[i]);
    vhists.push_back(hists[i]);
    for(int j = vhists.size()-2; j >= 0; j--){
      if(vhists[j]->Integral() < vhists[j+1]->Integral()){
	stemp = vlabels[j+1];
	itemp = vcolors[j+1];
	htemp = vhists[j+1];
	vlabels[j+1] = vlabels[j];
	vcolors[j+1] = vcolors[j];
	vhists[j+1]  = vhists[j];
	vlabels[j] = stemp;
	vcolors[j] = itemp;
	vhists[j]  = htemp;
      } else {
	break;
      }
    }
  }
  
  // "stack" the histograms by adding
  for(int i = Nbkg-2; i >= 0; i--)
    vhists[i]->Add(vhists[i+1]);

  hists  = vhists;
  labels = vlabels;
  colors = vcolors;

  const FitBin& bin = cat[0].GetFitBin();

  int NR = bin.NRBins();
  int NB = bin.NBins();
  VS blabels;
  for(int r = 0; r < NR; r++)
    blabels += bin[r].GetMBinLabels();

  int lmax = 0;
  for(int b = 0; b < NB; b++){
    int len = blabels[b].length();
    if(blabels[b].find("#infty") != std::string::npos)
      len -= 5;
    if(len > lmax)
      lmax = len;
  }
  string space = "";
  for(int l = 0; l < 1.6*lmax; l++)
    space += " ";
  
  for(int b = 0; b < NB; b++){
    if(b%2 == 1)
      hists[0]->GetXaxis()->SetBinLabel(b+1, (blabels[b]+space).c_str());
    else
      hists[0]->GetXaxis()->SetBinLabel(b+1, blabels[b].c_str());
  }
  
  blabels.clear();

  for(int r = 0; r < NR; r++)
    blabels += bin[r].GetRBinLabel();
  
  hists[0]->LabelsOption("v","X");
  
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

  double hmax = hists[0]->GetMaximum();
  
  hists[0]->Draw("hist");
  hists[0]->GetXaxis()->CenterTitle();
  hists[0]->GetXaxis()->SetTitleFont(42);
  hists[0]->GetXaxis()->SetTitleSize(0.05);
  hists[0]->GetXaxis()->SetTitleOffset(1.0);
  hists[0]->GetXaxis()->SetLabelFont(42);
  hists[0]->GetXaxis()->SetLabelSize(0.04);
  hists[0]->GetXaxis()->SetTitle("");
  hists[0]->GetXaxis()->SetTickSize(0.);
  hists[0]->GetYaxis()->CenterTitle();
  hists[0]->GetYaxis()->SetTitleFont(42);
  hists[0]->GetYaxis()->SetTitleSize(0.04);
  hists[0]->GetYaxis()->SetTitleOffset(0.85);
  hists[0]->GetYaxis()->SetLabelFont(42);
  hists[0]->GetYaxis()->SetLabelSize(0.035);
  hists[0]->GetYaxis()->SetTitle("number of events");
   
  for(int i = 0; i < Nbkg; i++){
    hists[i]->SetLineColor(kBlack);
    hists[i]->SetLineWidth(1.0);
    hists[i]->SetFillColor(colors[i]);
    hists[i]->SetFillStyle(1001);
    hists[i]->Draw("SAME HIST");
  }

  TGraphErrors* gr = nullptr;
  if(!m_FilePtr){
    vector<double> X;
    vector<double> Xerr;
    vector<double> Y;
    vector<double> Yerr;
    for(int i = 0; i < NB; i++){
      X.push_back(hists[0]->GetXaxis()->GetBinCenter(i+1));
      Xerr.push_back(0.5);
      Y.push_back(hists[0]->GetBinContent(i+1));
      Yerr.push_back(hists[0]->GetBinError(i+1));
    }
    gr = (TGraphErrors*) new TGraphErrors(NB, &X[0], &Y[0],  &Xerr[0], &Yerr[0]);
  } else {
    cout << "here " << gr << endl;
    gr = (TGraphErrors*) GetTotalBackground(cat);
    cout << "here " << gr << endl;
  }
    
  gr->SetMarkerSize(0);
  gr->SetLineColor(kBlack);
  gr->SetFillColor(kBlack);
  gr->SetFillStyle(3244);
  gr->Draw("same p2");

  for(int i = 0; i < Nsig; i++){
    hists_sig[i]->SetLineColor(7030+i*10);
    hists_sig[i]->SetLineWidth(5);
    hists_sig[i]->SetFillColor(kWhite);
    hists_sig[i]->Draw("SAME HIST");
    if(hists_sig[i]->GetMaximum() > hmax)
      hmax = hists_sig[i]->GetMaximum();
  }

  if(hist_data){
    hist_data->SetLineColor(kBlack);
    hist_data->SetFillColor(kWhite);
    hist_data->SetMarkerStyle(8);
    hist_data->SetMarkerSize(1.);
    hist_data->SetLineWidth(2);
    hist_data->Draw("SAME ep");
    if(hist_data->GetMaximum() > hmax)
      hmax = hist_data->GetMaximum();
  }

  hists[0]->GetYaxis()->SetRangeUser(0.05, 1.1*hmax);

  TLegend* leg = new TLegend(1.-hhi+0.01, 1.- (Nbkg+Nsig+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);

  if(hist_data)
    leg->AddEntry(hist_data, "data");
  leg->AddEntry(gr, "total uncertainty","F");
  for(int i = 0; i < Nbkg; i++)
    leg->AddEntry(hists[i], labels[i].c_str(), "F");
  for(int i = 0; i < Nsig; i++)
    leg->AddEntry(hists_sig[i], labels_sig[i].c_str(), "L");
  leg->Draw("SAME");

  double eps = 0.0015;
  
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  // line->DrawLineNDC(hlo, hbo-0.024*lmax, 1-hhi, hbo-0.0235*lmax);
 
  l.SetTextSize(0.025);
  l.SetTextFont(42);
  l.SetTextAlign(23);
  line->SetLineWidth(1);
  double lo = hlo;
  double hi = hlo;
  double yline = hbo-0.024*lmax;
  int ib = 0;
  for(int r = 0; r < NR; r++){
    int NM = bin[r].NBins();
    lo = hi;
    hi = double(NM)/double(NB)*(1.-hhi-hlo) + lo;
    
    line->SetLineStyle(1);
    line->DrawLineNDC(lo + eps, yline,
  		      lo + eps, yline + 6*eps);
    line->DrawLineNDC(hi - eps, yline,
  		      hi - eps, yline + 6*eps);
    line->DrawLineNDC(lo + eps, yline,
  		      hi - eps, yline);
    line->SetLineStyle(5);
    line->DrawLineNDC(hi, hbo, hi, 1.-hto);
    line->SetLineStyle(3);
    for(int b = 0; b < NM; b++){
      if(ib%2 == 1)
  	line->DrawLineNDC(lo + (hi-lo)*(b+0.5)/double(NM), hbo,
  			  lo + (hi-lo)*(b+0.5)/double(NM), (hbo+yline)/2.+eps);
      ib++;
    }
    
    l.DrawLatex((hi+lo)/2., yline - 8*eps, blabels[r].c_str());
  }
       
  l.SetTextAlign(32);
  l.SetTextSize(0.03);
  l.SetTextFont(42);
  l.DrawLatex(hlo, (hbo+yline)/2.+eps, "M_{#perp}   [GeV] #in");

  l.SetTextSize(0.03);
  l.SetTextFont(42);
  l.DrawLatex(hlo, yline - 15*eps, "#scale[1.15]{R_{ISR}} #in");
 
  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, "2017 MC KUEWKino");
  l.SetTextAlign(11);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02,"#bf{#it{CMS}} work-in-progress");
  l.SetTextSize(0.05);

  string plotlabel = "#color[7014]{"+lep_labels[0]+"} + ";
  plotlabel += "#color[7004]{"+hadS_labels[0]+"} + ";
  plotlabel += "#color[7024]{"+hadI_labels[0]+"} + ";
  plotlabel += "p_{T}^{ISR} > 300 GeV";
  
  l.SetTextColor(kBlack);
  l.SetTextAlign(13);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(hlo+0.02, 1-hto-0.012, plotlabel.c_str());
  
  return can;
  
}

TCanvas* FitPlotter::Plot2D(const VS& proc,
			   const VS& lep_cat,
			   const VS& hadS_cat,
			   const VS& hadI_cat,
			   const string& name,
			   const string& extra){
  RestFrames::SetStyle();

  
  int Nproc = proc.size();
  int Nlep  = lep_cat.size();
  int NhadS = hadS_cat.size();
  int NhadI = hadI_cat.size();
  if(Nproc == 0 ||
     Nlep  == 0 ||
     NhadS == 0 ||
     NhadI == 0)
    return nullptr;

  CategoryList cat = GetCategories();
  cat.Print();
  
  // Leptonic
  VS lep_labels;
  VS vlep;
  for(int i = 0; i < Nlep; i++){
    if(m_Title.count(lep_cat[i]) != 0)
      lep_labels.push_back(m_Title[lep_cat[i]]);
    else
      lep_labels.push_back(lep_cat[i]);

    if(m_Strings.count(lep_cat[i]) != 0){
      int N = m_Strings[lep_cat[i]].size();
      for(int j = 0; j < N; j++){
	vlep.push_back(m_Strings[lep_cat[i]][j]);
      }
    } else {
      vlep.push_back(lep_cat[i]);
    }
  }

  cat = cat.FilterOR(vlep);

  // Hadronic S
  VS hadS_labels;
  VS vhadS;
  for(int i = 0; i < NhadS; i++){
    if(m_Title.count(hadS_cat[i]) != 0)
      hadS_labels.push_back(m_Title[hadS_cat[i]]);
    else
      hadS_labels.push_back(hadS_cat[i]);

    if(m_Strings.count(hadS_cat[i]) != 0){
      int N = m_Strings[hadS_cat[i]].size();
      for(int j = 0; j < N; j++)
	vhadS.push_back(m_Strings[hadS_cat[i]][j]);
    } else {
      vhadS.push_back(hadS_cat[i]);
    }
  }

  cat = cat.FilterOR(vhadS);

  // Hadronic ISR
  VS hadI_labels;
  VS vhadI;
  for(int i = 0; i < NhadI; i++){
    if(m_Title.count(hadI_cat[i]) != 0)
      hadI_labels.push_back(m_Title[hadI_cat[i]]);
    else
      hadI_labels.push_back(hadI_cat[i]);

    if(m_Strings.count(hadI_cat[i]) != 0){
      int N = m_Strings[hadI_cat[i]].size();
      for(int j = 0; j < N; j++)
	vhadI.push_back(m_Strings[hadI_cat[i]][j]);
    } else {
      vhadI.push_back(hadI_cat[i]);
    }
  }

  cat = cat.FilterOR(vhadI);

  if(extra != "")
    cat = cat.Filter(extra);

  int Ncat = cat.GetN();
  
  if(Ncat < 1)
    return nullptr;
  
  // Processes
  string label;
  TH2D* hist = nullptr;
  
  for(int i = 0; i < Nproc; i++){
    VS vproc;
    if(m_Strings.count(proc[i]) != 0)
      vproc = m_Strings[proc[i]];
    else
      vproc += proc[i];

    ProcessType type = kBkg;
    for(int p = 0; p < int(vproc.size()); p++){
      
      int index = GetProcesses().Find(vproc[p]);
      if(index < 0)
	continue;
      
      Process pp = GetProcesses()[index];

      if(pp.Type() == kSig)
	type = kSig;
      if(pp.Type() == kData)
	type = kData;
      
      for(int c = 0; c < Ncat; c++){
	//cout << cat[c].GetLabel() << " " << pp.Name() << endl;
	if(!IsFilled(cat[c], pp))
	  continue;

	//cout << "filled " << cat[c].GetLabel() << " " << pp.Name() << endl;
	
	if(!hist){
	  hist = (TH2D*) GetHistogram2D(cat[c], pp)->Clone(Form("plothist_%dhis_%s_2D", i, name.c_str()));
	} else {
	  hist->Add(GetHistogram2D(cat[c], pp));
	}
      }
    }

    if(hist == nullptr)
      continue;
    
    if(type == kData){
      label = "Data";
    }

    if(type == kSig){
      label = GetSignalTitle(proc[i]);
    }

    if(type == kBkg){
      if(m_Title.count(proc[i]) != 0)
	label = m_Title[proc[i]];
      else
	label = proc[i];
    } 
  }

  const FitBin& bin = cat[0].GetFitBin();

  int NR = bin.NRBins();
  int NB = bin.NBins();
  // VS blabels;
  // for(int r = 0; r < NR; r++)
  //   blabels += bin[r].GetMBinLabels();

  // int lmax = 0;
  // for(int b = 0; b < NB; b++){
  //   int len = blabels[b].length();
  //   if(blabels[b].find("#infty") != std::string::npos)
  //     len -= 5;
  //   if(len > lmax)
  //     lmax = len;
  // }
  // string space = "";
  // for(int l = 0; l < 1.6*lmax; l++)
  //   space += " ";
  
  // for(int b = 0; b < NB; b++){
  //   if(b%2 == 1)
  //     hists[0]->GetXaxis()->SetBinLabel(b+1, (blabels[b]+space).c_str());
  //   else
  //     hists[0]->GetXaxis()->SetBinLabel(b+1, blabels[b].c_str());
  // }
  
  // blabels.clear();

  // for(int r = 0; r < NR; r++)
  //   blabels += bin[r].GetRBinLabel();
  
  // hists[0]->LabelsOption("v","X");
  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = new TCanvas(Form("can_%s", name.c_str()),
			     Form("can_%s", name.c_str()),
			     700, 700);
  double eps = 0.0015;

  double hlo = 0.14;
  double hhi = 0.18;
  double hbo = 0.13;
  double hto = 0.17;
  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->Draw();
  can->cd();
  
  hist->Draw("colz");
  hist->GetXaxis()->SetTitle("M_{#perp}   [GeV]");
  hist->GetYaxis()->SetTitle("R_{ISR}");
  hist->GetZaxis()->SetTitle("number of events");
  
  hist->GetXaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleSize(0.05);
  hist->GetXaxis()->SetTitleOffset(1.15);
  hist->GetXaxis()->SetLabelFont(42);
  hist->GetXaxis()->SetLabelSize(0.04);
  hist->GetXaxis()->SetTickSize(0.);
  
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetYaxis()->SetTitleSize(0.05);
  hist->GetYaxis()->SetTitleOffset(1.15);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelSize(0.035);
  hist->GetYaxis()->SetTickSize(0.);
  
  hist->GetZaxis()->CenterTitle();
  hist->GetZaxis()->SetTitleFont(42);
  hist->GetZaxis()->SetTitleSize(0.05);
  hist->GetZaxis()->SetTitleOffset(0.9);

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(7024);

  // line->DrawLineNDC(hlo, hbo-0.024*lmax, 1-hhi, hbo-0.0235*lmax);
 
  l.SetTextSize(0.025);
  l.SetTextFont(42);
  l.SetTextAlign(23);
  line->SetLineWidth(3);
  line->SetLineColor(7024);
  line->SetLineStyle(7);
 
  for(int r = 0; r < NR; r++){
    line->DrawLine(hist->GetXaxis()->GetXmin(), bin[r].Rlow(),
		   hist->GetXaxis()->GetXmax(), bin[r].Rlow());
    line->DrawLine(hist->GetXaxis()->GetXmin(), bin[r].Rhigh(),
		   hist->GetXaxis()->GetXmax(), bin[r].Rhigh());

    for(int b = 0; b <  bin[r].BinEdges().size()-1; b++){
      line->DrawLine(bin[r].BinEdges()[b], bin[r].Rlow(),
		     bin[r].BinEdges()[b], bin[r].Rhigh());
    }
    
    // int NM = bin[r].NBins();
    // lo = hi;
    // hi = double(NM)/double(NB)*(1.-hhi-hlo) + lo;
    
    // line->SetLineStyle(1);
    // line->DrawLineNDC(lo + eps, yline,
    // 		      lo + eps, yline + 6*eps);
    // line->DrawLineNDC(hi - eps, yline,
    // 		      hi - eps, yline + 6*eps);
    // line->DrawLineNDC(lo + eps, yline,
    // 		      hi - eps, yline);
    // line->SetLineStyle(5);
    // line->DrawLineNDC(hi, hbo, hi, 1.-hto);
    // line->SetLineStyle(3);
    // for(int b = 1; b < NM; b++)
    //   if(b%2 == 1)
    // 	line->DrawLineNDC(lo + (hi-lo)*(b+0.5)/double(NM), hbo,
    // 			  lo + (hi-lo)*(b+0.5)/double(NM), (hbo+yline)/2.+eps);
    
    // l.DrawLatex((hi+lo)/2., yline - 8*eps, blabels[r].c_str());
  }
 
 
  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  //l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.12, "2017 MC KUEWKino");
  l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, label.c_str());
  l.SetTextAlign(11);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02,"#bf{#it{CMS}} work-in-progress");
  l.SetTextSize(0.05);

  string plotlabel = "#color[7014]{"+lep_labels[0]+"} + ";
  plotlabel += "#color[7004]{"+hadS_labels[0]+"} + ";
  plotlabel += "#color[7024]{"+hadI_labels[0]+"} + ";
  plotlabel += "p_{T}^{ISR} > 300 GeV";
  
  l.SetTextColor(kBlack);
  l.SetTextAlign(13);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(hlo+0.02, 1-hto+0.15, plotlabel.c_str());
  
  
  return can;
  
}

TCanvas* FitPlotter::PlotYields(const string& can_name,
			       const VS& proc,
			       const CategoryTree& CT){

  RestFrames::SetStyle();

  int Nproc = proc.size();
  if(Nproc == 0)
    return nullptr;

  int Nvis = CT.GetNVisible();
  if(Nvis < 1)
    return nullptr;

  vector<const CategoryTree*> CatTrees;
  CT.GetListVisible(CatTrees);
  
  CategoryList CatList = GetCategories();

  if(CatList.GetN() < 1)
    return nullptr;

  // Processes
  VS                  labels;
  vector<int>         colors;
  vector<TH1D*>       hists[Nvis];
  vector<double>      total;
  
  VS                  labels_sig;
  vector<TH1D*>       hists_sig[Nvis];

  TH1D* hist_data[Nvis];
  double total_data = 0.;

  for(int i = 0; i < Nproc; i++){
    VS vproc;
    if(m_Strings.count(proc[i]) != 0)
      vproc = m_Strings[proc[i]];
    else
      vproc += proc[i];

    double itot = 0.;
    
    ProcessList procs;
    
    ProcessType type = kBkg;
    TH1D* hist[Nvis];
    for(int p = 0; p < int(vproc.size()); p++){
      
      int index = GetProcesses().Find(vproc[p]);
      if(index < 0)
	continue;
      
      Process pp = GetProcesses()[index];

      procs += pp;

      if(pp.Type() == kSig){
	type = kSig;
      }
      
      if(pp.Type() == kData){
	type = kData;
      }
    }

    for(int v = 0; v < Nvis; v++){
      CategoryList cat = CatList.Filter(*CatTrees[v]);
      
      TH1D* h = GetIntegralHist(Form("plothist_%d_%d_%s", i, v, can_name.c_str()), cat, procs);
      if(h)
	itot += h->Integral();
      
      hist[v] = h;
    }
    
    if(itot <= 1e-4)
      continue;
    
    if(type == kData){
      for(int v = 0; v < Nvis; v++)
	hist_data[v] = hist[v];
      total_data = itot;
    }
    
    if(type == kSig){
      labels_sig.push_back(GetSignalTitle(proc[i]));
      for(int v = 0; v < Nvis; v++)
	hists_sig[v].push_back(hist[v]);
    }
    
    if(type == kBkg){
      if(m_Title.count(proc[i]) != 0)
	labels += m_Title[proc[i]];
      else
	labels += proc[i];
      
      if(m_Color.count(proc[i]) != 0)
	colors.push_back(m_Color[proc[i]]);
      else
	colors.push_back(m_ColorDefault[i]);
      
      for(int v = 0; v < Nvis; v++)
	hists[v].push_back(hist[v]);
      total.push_back(itot);
    } 
  }
  
  int Nsig = hists_sig[0].size();
  
  // sort the histograms by integral (N^2/2 brute force)
  int Nbkg = total.size();
  VS             vlabels;
  vector<int>    vcolors;
  vector<TH1D*>  vhists[Nvis];
  vector<double> vtotal;
  string stemp;
  int    itemp;
  TH1D*  htemp[Nvis];
  double ttemp;
  
  for(int i = 0; i < Nbkg; i++){
    vlabels.push_back(labels[i]);
    vcolors.push_back(colors[i]);
    for(int v = 0; v < Nvis; v++)
      vhists[v].push_back(hists[v][i]);
    vtotal.push_back(total[i]);
    for(int j = vtotal.size()-2; j >= 0; j--){
      if(vtotal[j] < vtotal[j+1]){
	stemp = vlabels[j+1];
	itemp = vcolors[j+1];
	for(int v = 0; v < Nvis; v++)
	  htemp[v] = vhists[v][j+1];
	ttemp = vtotal[j+1];
	vlabels[j+1] = vlabels[j];
	vcolors[j+1] = vcolors[j];
	for(int v = 0; v < Nvis; v++)
	  vhists[v][j+1]  = vhists[v][j];
	vtotal[j+1]  = vtotal[j];
	vlabels[j] = stemp;
	vcolors[j] = itemp;
	for(int v = 0; v < Nvis; v++)
	  vhists[v][j]  = htemp[v];
	vtotal[j] = ttemp;
      } else {
	break;
      }
    }
  }
  
  vector<TH1D*> fhists;
  vector<TH1D*> fhists_sig;
  TH1D*         fhist_data = nullptr;

  for(int i = 0; i < Nsig; i++){
    fhists_sig.push_back(new TH1D(Form("fhistsig_%d_%s", i, can_name.c_str()),
				  Form("fhistsig_%d_%s", i, can_name.c_str()),
				  Nvis, 0., Nvis));
    for(int v = 0; v < Nvis; v++){
      if(hists_sig[v][i]){
	fhists_sig[i]->SetBinContent(v+1, hists_sig[v][i]->GetBinContent(1));
	fhists_sig[i]->SetBinError(v+1, hists_sig[v][i]->GetBinError(1));
      }
    }
  }
 
  if(total_data > 0.){
    fhist_data = new TH1D(Form("fhistdata_%s", can_name.c_str()),
			  Form("fhistdata_%s", can_name.c_str()),
			  Nvis, 0., Nvis);
    for(int v = 0; v < Nvis; v++){
      if(hist_data[v]){
	fhist_data->SetBinContent(v+1, hist_data[v]->GetBinContent(1));
	fhist_data->SetBinError(v+1, hist_data[v]->GetBinError(1));
      }
    }
  }

  for(int i = 0; i < Nbkg; i++){
    for(int v = 0; v < Nvis; v++){
      if(vhists[v][i]){
	for(int j = i+1; j < Nbkg; j++)
	  if(vhists[v][j])
	    vhists[v][i]->Add(vhists[v][j]);
      }
    }
  }

  for(int i = 0; i < Nbkg; i++){
    fhists.push_back(new TH1D(Form("fhistsbkg_%d_%s", i, can_name.c_str()),
			      Form("fhistsbkg_%d_%s", i, can_name.c_str()),
			      Nvis, 0., Nvis));
    for(int v = 0; v < Nvis; v++){
      int j = i;
      TH1D* hptr = nullptr;
      while(j < Nbkg && hptr == nullptr){
	hptr = vhists[v][j];
	j++;
      }
      if(hptr){
	fhists[i]->SetBinContent(v+1, hptr->GetBinContent(1));
	fhists[i]->SetBinError(v+1, hptr->GetBinError(1));
      }
    }
  }

  labels = vlabels;
  colors = vcolors;

  for(int b = 0; b < Nvis; b++){
    fhists[0]->GetXaxis()->SetBinLabel(b+1, CatTrees[b]->GetSpectroscopicLabel().c_str());
  }
  
  // for(int b = 0; b < NB; b++){
  //   if(b%2 == 1)
  //     hists[0]->GetXaxis()->SetBinLabel(b+1, (blabels[b]+space).c_str());
  //   else
  //     hists[0]->GetXaxis()->SetBinLabel(b+1, blabels[b].c_str());
  // }

  fhists[0]->LabelsOption("v","X");
  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = new TCanvas(Form("can_%s", can_name.c_str()),
			     Form("can_%s", can_name.c_str()),
			     1200, 700);
  double hlo = 0.09;
  double hhi = 0.2;
  double hbo = 0.19;
  double hto = 0.07;
  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->SetGridy();
  can->SetLogy();
  can->Draw();
  can->cd();

  double hmax = fhists[0]->GetMaximum();
  double hmin = std::max(0.1, fhists[Nbkg-1]->GetMinimum());
  
  fhists[0]->Draw("hist");
  fhists[0]->GetXaxis()->CenterTitle();
  fhists[0]->GetXaxis()->SetTitleFont(42);
  fhists[0]->GetXaxis()->SetTitleSize(0.05);
  fhists[0]->GetXaxis()->SetTitleOffset(1.0);
  fhists[0]->GetXaxis()->SetLabelFont(42);
  fhists[0]->GetXaxis()->SetLabelSize(std::min(2./double(Nvis), 0.04));
  fhists[0]->GetXaxis()->SetTitle("");
  fhists[0]->GetXaxis()->SetTickSize(0.);
  fhists[0]->GetYaxis()->CenterTitle();
  fhists[0]->GetYaxis()->SetTitleFont(42);
  fhists[0]->GetYaxis()->SetTitleSize(0.04);
  fhists[0]->GetYaxis()->SetTitleOffset(0.85);
  fhists[0]->GetYaxis()->SetLabelFont(42);
  fhists[0]->GetYaxis()->SetLabelSize(0.035);
  fhists[0]->GetYaxis()->SetTitle("number of events");
   
  for(int i = 0; i < Nbkg; i++){
    fhists[i]->SetLineColor(kBlack);
    fhists[i]->SetLineWidth(1.0);
    fhists[i]->SetFillColor(colors[i]);
    fhists[i]->SetFillStyle(1001);
    fhists[i]->Draw("SAME HIST");
  }

  TGraphErrors* gr = nullptr;
  if(true){// !m_FilePtr
    vector<double> X;
    vector<double> Xerr;
    vector<double> Y;
    vector<double> Yerr;
    for(int i = 0; i < Nvis; i++){
      X.push_back(fhists[0]->GetXaxis()->GetBinCenter(i+1));
      Xerr.push_back(0.5);
      Y.push_back(fhists[0]->GetBinContent(i+1));
      Yerr.push_back(fhists[0]->GetBinError(i+1));
    }
    gr = (TGraphErrors*) new TGraphErrors(Nvis, &X[0], &Y[0],  &Xerr[0], &Yerr[0]);
  } else {
    // cout << "here " << gr << endl;
    // gr = (TGraphErrors*) GetTotalBackground(cat);
    // cout << "here " << gr << endl;
  }
    
  gr->SetMarkerSize(0);
  gr->SetLineColor(kBlack);
  gr->SetFillColor(kBlack);
  gr->SetFillStyle(3244);
  gr->Draw("same p2");
  
  for(int i = 0; i < Nsig; i++){
    fhists_sig[i]->SetLineColor(m_SignalColor[i]);
    fhists_sig[i]->SetLineWidth(8);
    fhists_sig[i]->SetFillColor(kWhite);
    fhists_sig[i]->Draw("SAME HIST");
    if(fhists_sig[i]->GetMaximum() > hmax)
      hmax = fhists_sig[i]->GetMaximum();
  }

  if(fhist_data){
    fhist_data->SetLineColor(kBlack);
    fhist_data->SetFillColor(kWhite);
    fhist_data->SetMarkerStyle(8);
    fhist_data->SetMarkerSize(1.);
    fhist_data->SetLineWidth(2);
    fhist_data->Draw("SAME ep");
    if(fhist_data->GetMaximum() > hmax)
      hmax = fhist_data->GetMaximum();
  }

  double logrange = log(hmax) - log(hmin);
  hmax = exp(log(hmax) + logrange*0.3);
  fhists[0]->GetYaxis()->SetRangeUser(hmin/1.5, hmax);

  TLegend* leg = new TLegend(1.-hhi+0.007, 1.- (Nbkg+Nsig+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);

  if(fhist_data)
    leg->AddEntry(fhist_data, "data");
  leg->AddEntry(gr, "total uncertainty","F");
  for(int i = 0; i < Nbkg; i++)
    leg->AddEntry(fhists[i], labels[i].c_str(), "F");
  for(int i = 0; i < Nsig; i++)
    leg->AddEntry(fhists_sig[i], labels_sig[i].c_str(), "L");
  leg->Draw("SAME");

  DrawCatTree(CT, can);
  
  
  double eps = 0.0015;
  
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  // line->DrawLineNDC(hlo, hbo-0.024*lmax, 1-hhi, hbo-0.0235*lmax);
 
  l.SetTextSize(0.025);
  l.SetTextFont(42);
  l.SetTextAlign(23);
  line->SetLineWidth(1);
  double lo = hlo;
  double hi = hlo;
 
 
  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, string("Regions "+CT.GetSpectroscopicLabel()).c_str());
  l.SetTextAlign(11);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02, m_CMSLabel.c_str());
  l.SetTextSize(0.05);
  
  return can;

}

void FitPlotter::DrawCatTree(const CategoryTree& CT, TCanvas* can){
  if(!can)
    return;

  double hlo = can->GetLeftMargin();
  double hhi = 1. - can->GetRightMargin();
  double hbo = can->GetBottomMargin();
  double hto = 1. - can->GetTopMargin();

  double frac = 0.15;
  
  can->cd();
  //const CategoryTree* GetParent()
  //GetBareLabel()
  int Depth = CT.GetDepth();
  int Nvis = CT.GetNVisible();
  int ibin = 0;
  int irun = 0;
  int brun = 0;

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  l.SetTextSize(0.03);
  l.SetTextAlign(33);
  l.SetTextAngle(90);

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(7024);
  line->SetLineStyle(7);
  
  for(int i = 0; i < Depth; i++){
    int icolor = 7004 + 10*((i)%8);
    //int icolor = 7024;
    l.SetTextColor(icolor);
    line->SetLineColor(icolor);
    vector<const CategoryTree*> cats;
    CT.GetListDepth(cats, i);

    const CategoryTree* CurPar = cats[0]->GetVisibleParent();
    
    int Nc = cats.size();
    for(int c = 0; c < Nc; c++){
      irun += 1;
      ibin += 1;
      if(c+1 >= Nc ||
	 cats[c+1]->GetVisibleParent() != CurPar){
	if(irun > 1 && CurPar){
	  line->DrawLineNDC(hlo + (hhi-hlo)*double(brun)/double(Nvis), hto - double(i)/double(Depth)*frac*(hto-hbo),
	  		    hlo + (hhi-hlo)*double(brun)/double(Nvis), hbo);
	  l.DrawLatex(hlo + (hhi-hlo)*double(brun)/double(Nvis) + 0.0015,
		      hto - double(i)/double(Depth)*frac*(hto-hbo),
		      CurPar->GetSpectroscopicLabel().c_str());
	}
	irun = 0;
	brun = ibin;
	if(c+1 < Nc)
	  CurPar = cats[c+1]->GetVisibleParent();
      }
    }
  }
}
//////////////////////////////////////////////////
//modify
TCanvas* FitPlotter::Plot1Dstack(const string& can_name,
				const VS& proc,
				const CategoryTree& CT){
  RestFrames::SetStyle();

  int Nproc = proc.size();
  if(Nproc == 0)
    return nullptr;

  vector<const CategoryTree*> CatTrees;
  CT.GetListDeepest(CatTrees);

  int Nvis = CatTrees.size();
  
  if(Nvis < 1)
    return nullptr;
  
  CategoryList CatList = GetCategories();

  if(CatList.GetN() < 1)
    return nullptr;

  // Processes
  VS                  labels;
  vector<int>         colors;
  vector<TH1D*>       hists[Nvis];
  vector<double>      total;
  
  VS                  labels_sig;
  vector<TH1D*>       hists_sig[Nvis];

  TH1D* hist_data[Nvis];
  double total_data = 0.;
  
  for(int i = 0; i < Nproc; i++){
    VS vproc;
    if(m_Strings.count(proc[i]) != 0)
      vproc = m_Strings[proc[i]];
    else
      vproc += proc[i];

    double itot = 0.;
    
    ProcessList procs;
    
    ProcessType type = kBkg;
    TH1D* hist[Nvis];
  
    for(int p = 0; p < int(vproc.size()); p++){
      
      int index = GetProcesses().Find(vproc[p]);
      if(index < 0)
	continue;
      
      Process pp = GetProcesses()[index];

      procs += pp;

      if(pp.Type() == kSig){
	type = kSig;
      }
      
      if(pp.Type() == kData){
	type = kData;
      }
    }

    for(int v = 0; v < Nvis; v++){
      CategoryList cat = CatList.Filter(*CatTrees[v]);
      
      TH1D* h = GetAddedHist(Form("plothist_%d_%d_%s", i, v, can_name.c_str()), cat, procs);
      if(h)
	itot += h->Integral();
      
      hist[v] = h;
    }
    
    if(itot <= 1e-4)
      continue;
    
    if(type == kData){
      for(int v = 0; v < Nvis; v++)
	hist_data[v] = hist[v];
      total_data = itot;
    }
    
    if(type == kSig){
      labels_sig.push_back(GetSignalTitle(proc[i]));
      for(int v = 0; v < Nvis; v++)
	hists_sig[v].push_back(hist[v]);
    }
    
    if(type == kBkg){
      if(m_Title.count(proc[i]) != 0)
	labels += m_Title[proc[i]];
      else
	labels += proc[i];
      
      if(m_Color.count(proc[i]) != 0)
	colors.push_back(m_Color[proc[i]]);
      else
	colors.push_back(m_ColorDefault[i]);
      
      for(int v = 0; v < Nvis; v++)
	hists[v].push_back(hist[v]);
      total.push_back(itot);
    } 
  }
  
  int Nsig = hists_sig[0].size();
  
  // sort the histograms by integral (N^2/2 brute force)
  int Nbkg = total.size();
  VS             vlabels;
  vector<int>    vcolors;
  vector<TH1D*>  vhists[Nvis];
  vector<double> vtotal;
  string stemp;
  int    itemp;
  TH1D*  htemp[Nvis];
  double ttemp;
  
  for(int i = 0; i < Nbkg; i++){
    vlabels.push_back(labels[i]);
    vcolors.push_back(colors[i]);
    for(int v = 0; v < Nvis; v++)
      vhists[v].push_back(hists[v][i]);
    vtotal.push_back(total[i]);
    for(int j = vtotal.size()-2; j >= 0; j--){
      if(vtotal[j] < vtotal[j+1]){
	stemp = vlabels[j+1];
	itemp = vcolors[j+1];
	for(int v = 0; v < Nvis; v++)
	  htemp[v] = vhists[v][j+1];
	ttemp = vtotal[j+1];
	vlabels[j+1] = vlabels[j];
	vcolors[j+1] = vcolors[j];
	for(int v = 0; v < Nvis; v++)
	  vhists[v][j+1]  = vhists[v][j];
	vtotal[j+1]  = vtotal[j];
	vlabels[j] = stemp;
	vcolors[j] = itemp;
	for(int v = 0; v < Nvis; v++)
	  vhists[v][j]  = htemp[v];
	vtotal[j] = ttemp;
      } else {
	break;
      }
    }
  }
  
  vector<TH1D*> fhists;
  vector<TH1D*> fhists_sig;
  TH1D*         fhist_data = nullptr;

  CategoryList dumcat = CatList.Filter(*CatTrees[0]);
  const FitBin& fitbin = dumcat[0].GetFitBin();
  int Nbin = fitbin.NBins();
  
  for(int i = 0; i < Nsig; i++){
    fhists_sig.push_back(new TH1D(Form("fhistsig_%d_%s", i, can_name.c_str()),
				  Form("fhistsig_%d_%s", i, can_name.c_str()),
				  Nbin*Nvis, 0., Nbin*Nvis));
    for(int b = 0; b < Nbin; b++){
      for(int v = 0; v < Nvis; v++){
	if(hists_sig[v][i]){
	  fhists_sig[i]->SetBinContent(b*Nvis+v+1, hists_sig[v][i]->GetBinContent(b+1));
	  fhists_sig[i]->SetBinError(b*Nvis+v+1, hists_sig[v][i]->GetBinError(b+1));
	}
      }
    }
  }
 
  if(total_data > 0.){
    fhist_data = new TH1D(Form("fhistdata_%s", can_name.c_str()),
			  Form("fhistdata_%s", can_name.c_str()),
			  Nbin*Nvis, 0., Nbin*Nvis);
    for(int b = 0; b < Nbin; b++){
      for(int v = 0; v < Nvis; v++){
	if(hist_data[v]){
	  fhist_data->SetBinContent(b*Nvis+v+1, hist_data[v]->GetBinContent(b+1));
	  fhist_data->SetBinError(b*Nvis+v+1, hist_data[v]->GetBinError(b+1));
	}
      }
    }
  }

  for(int i = 0; i < Nbkg; i++){
    for(int v = 0; v < Nvis; v++){
      if(vhists[v][i]){
	for(int j = i+1; j < Nbkg; j++)
	  if(vhists[v][j])
	    vhists[v][i]->Add(vhists[v][j]);
      }
    }
  }
  
  for(int i = 0; i < Nbkg; i++){
    fhists.push_back(new TH1D(Form("fhistsbkg_%d_%s", i, can_name.c_str()),
			      Form("fhistsbkg_%d_%s", i, can_name.c_str()),
			      Nbin*Nvis, 0., Nbin*Nvis));
    for(int b = 0; b < Nbin; b++){
      for(int v = 0; v < Nvis; v++){
	int j = i;
	TH1D* hptr = nullptr;
	while(j < Nbkg && hptr == nullptr){
	  hptr = vhists[v][j];
	  j++;
	}
	if(hptr){
	  fhists[i]->SetBinContent(Nvis*b+v+1, hptr->GetBinContent(b+1));
	  fhists[i]->SetBinError(Nvis*b+v+1, hptr->GetBinError(b+1));
	}
      }
    }
  }
 
  labels = vlabels;
  colors = vcolors;

  for(int b = 0; b < Nbin*Nvis; b++)
    fhists[0]->GetXaxis()->SetBinLabel(b+1, "");
  
  fhists[0]->LabelsOption("v","X");
  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = new TCanvas(Form("can_%s", can_name.c_str()),
			     Form("can_%s", can_name.c_str()),
			     1200, 700);
  double hlo = 0.105;
  double hhi = 0.2;
  double hbo = 0.19;
  double hto = 0.07;
  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->SetGridy();
  can->SetLogy();
  can->Draw();
  can->cd();

  double hmax = fhists[0]->GetMaximum();
  double hmin = std::max(0.1, fhists[Nbkg-1]->GetMinimum());
  
  fhists[0]->Draw("hist");
  fhists[0]->GetXaxis()->CenterTitle();
  fhists[0]->GetXaxis()->SetTitleFont(42);
  fhists[0]->GetXaxis()->SetTitleSize(0.045);
  fhists[0]->GetXaxis()->SetTitleOffset(1.0);
  fhists[0]->GetXaxis()->SetLabelFont(42);
  fhists[0]->GetXaxis()->SetLabelSize(0.05);
  fhists[0]->GetXaxis()->SetTitle("");
  fhists[0]->GetXaxis()->SetTickSize(0.);
  fhists[0]->GetYaxis()->CenterTitle();
  fhists[0]->GetYaxis()->SetTitleFont(42);
  fhists[0]->GetYaxis()->SetTitleSize(0.04);
  fhists[0]->GetYaxis()->SetTitleOffset(0.85);
  fhists[0]->GetYaxis()->SetLabelFont(42);
  fhists[0]->GetYaxis()->SetLabelSize(0.035);
  fhists[0]->GetYaxis()->SetTitle("number of events");
   
  for(int i = 0; i < Nbkg; i++){
    fhists[i]->SetLineColor(kBlack);
    fhists[i]->SetLineWidth(1.0);
    fhists[i]->SetFillColor(colors[i]);
    fhists[i]->SetFillStyle(1001);
    fhists[i]->Draw("SAME HIST");
  }
  
  TGraphErrors* gr = nullptr;
  if(true){// !m_FilePtr
    vector<double> X;
    vector<double> Xerr;
    vector<double> Y;
    vector<double> Yerr;
    for(int i = 0; i < Nvis*Nbin; i++){
      X.push_back(fhists[0]->GetXaxis()->GetBinCenter(i+1));
      Xerr.push_back(0.5);
      Y.push_back(fhists[0]->GetBinContent(i+1));
      Yerr.push_back(fhists[0]->GetBinError(i+1));
    }
    gr = (TGraphErrors*) new TGraphErrors(Nvis*Nbin, &X[0], &Y[0],  &Xerr[0], &Yerr[0]);
  } else {
    // cout << "here " << gr << endl;
    // gr = (TGraphErrors*) GetTotalBackground(cat);
    // cout << "here " << gr << endl;
  }
    
  gr->SetMarkerSize(0);
  gr->SetLineColor(kBlack);
  gr->SetFillColor(kBlack);
  gr->SetFillStyle(3244);
  gr->Draw("same p2");
  
  for(int i = 0; i < Nsig; i++){
    fhists_sig[i]->SetLineColor(m_SignalColor[i]);
    fhists_sig[i]->SetMarkerColor(m_SignalColor[i]);
    fhists_sig[i]->SetLineWidth(8);
    fhists_sig[i]->SetMarkerStyle(22);
    fhists_sig[i]->SetFillColor(kWhite);
    fhists_sig[i]->Draw("SAME hist");
    if(fhists_sig[i]->GetMaximum() > hmax)
      hmax = fhists_sig[i]->GetMaximum();
  }

  if(fhist_data){
    fhist_data->SetLineColor(kBlack);
    fhist_data->SetFillColor(kWhite);
    fhist_data->SetMarkerStyle(8);
    fhist_data->SetMarkerSize(1.);
    fhist_data->SetLineWidth(2);
    fhist_data->Draw("SAME ep");
    if(fhist_data->GetMaximum() > hmax)
      hmax = fhist_data->GetMaximum();
  }

  double logrange = log(hmax) - log(hmin);
  hmin = exp(log(hmin) - logrange*0.2);
  fhists[0]->GetYaxis()->SetRangeUser(hmin, hmax*1.5);

  TLegend* leg = new TLegend(1.-hhi+0.007, 1.- (Nbkg+Nsig+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);

  if(fhist_data)
    leg->AddEntry(fhist_data, "data");
  leg->AddEntry(gr, "total uncertainty","F");
  for(int i = 0; i < Nbkg; i++)
    leg->AddEntry(fhists[i], labels[i].c_str(), "F");
  for(int i = 0; i < Nsig; i++)
    leg->AddEntry(fhists_sig[i], labels_sig[i].c_str(), "L");
  leg->Draw("SAME");

  DrawMR(fitbin, can);
  
  double eps = 0.0015;
  
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  l.SetTextSize(std::min(0.03, 1.5/double(Nvis*Nbin)));
  l.SetTextFont(42);
  l.SetTextAlign(12);
  l.SetTextAngle(90);
  int Depth = CT.GetDepth();
  for(int b = 0; b < Nvis*Nbin; b++){
    // if(colors[Nbkg-1]%1000 >=3)
    //   l.SetTextColor(7000 + 10*((b%Nvis)%8));
    // else
    l.SetTextColor(7004 + 10*((b%Nvis)%8));
    l.DrawLatex(hlo+(1.-hhi-hlo)/double(Nvis*Nbin)*(0.5+b), hbo + 4*eps,
		CatTrees[b%Nvis]->GetPlainLabel(Depth).c_str());
  }

  l.SetTextAngle(0);
  l.SetTextColor(kBlack);
  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, string("Regions "+CT.GetSpectroscopicLabel()).c_str());
  l.SetTextAlign(11);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02, m_CMSLabel.c_str());
  
  return can;
}

void FitPlotter::DrawMR(const FitBin& fitbin, TCanvas* can){
  double eps = 0.0015;

  double hlo = can->GetLeftMargin();
  double hhi = can->GetRightMargin();
  double hbo = can->GetBottomMargin();
  double hto = can->GetTopMargin();
  
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  int NR = fitbin.NRBins();
  int NB = fitbin.NBins();
  
  VS mlabels;
  VS rlabels;
  for(int r = 0; r < NR; r++)
    mlabels += fitbin[r].GetMBinLabels();

  int lmax = 0;
  for(int b = 0; b < NB; b++){
    int len = mlabels[b].length();
    if(mlabels[b].find("#infty") != std::string::npos)
      len -= 5;
    if(len > lmax)
      lmax = len;
  }
  
  string space = "";
  for(int l = 0; l < 1.6*lmax; l++)
    space += " ";

  for(int r = 0; r < NR; r++)
    rlabels += fitbin[r].GetRBinLabel();
  
  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);
  
  l.SetTextSize(0.033);
  l.SetTextFont(42);
  l.SetTextAlign(23);
  line->SetLineWidth(2);
  double lo = hlo;
  double hi = hlo;
  double yline = hbo-0.018*lmax;
  int ib = 0;
  for(int r = 0; r < NR; r++){
    int NM = fitbin[r].NBins();
    lo = hi;
    hi = double(NM)/double(NB)*(1.-hhi-hlo) + lo;
    
    line->SetLineStyle(1);
    line->DrawLineNDC(lo + eps, yline,
  		      lo + eps, yline + 6*eps);
    line->DrawLineNDC(hi - eps, yline,
  		      hi - eps, yline + 6*eps);
   
    line->DrawLineNDC(lo + eps, yline,
  		      hi - eps, yline);
    line->SetLineStyle(1);
    if(r < NR-1)
      line->DrawLineNDC(hi, yline + 10*eps , hi, 1.-hto);
    l.DrawLatex((hi+lo)/2., yline - 8*eps, rlabels[r].c_str());
  }

  line->SetLineStyle(1);
  l.SetTextAngle(90);
  l.SetTextAlign(32);
  for(int b = 0; b < NB; b++){
    l.DrawLatex(hlo + (1.-hhi-hlo)*(0.5+b)/double(NB), hbo - 4*eps, mlabels[b].c_str());
    if(b > 0)
      line->DrawLineNDC(hlo + (1.-hhi-hlo)/double(NB)*b , hbo , hlo + (1.-hhi-hlo)/double(NB)*b, 1.-hto);
  }

  l.SetTextAngle(0);
  l.SetTextAlign(32);
  l.SetTextSize(0.035);
  l.DrawLatex(hlo, (hbo+yline)/2.+eps, "M_{#perp}   [GeV] #in");
  l.DrawLatex(hlo, yline - 16*eps, "#scale[1.15]{R_{ISR}} #in");
}


TCanvas* FitPlotter::Plot2D(const string& can_name,
			   const VS& proc,
			   const CategoryTree& CT){
  RestFrames::SetStyle();

  int Nproc = proc.size();
  if(Nproc == 0)
    return nullptr;
  
  vector<const CategoryTree*> CatTrees;
  CT.GetListDeepest(CatTrees);
  
  int Nvis = CatTrees.size();
  
  if(Nvis < 1)
    return nullptr;
  
  CategoryList CatList = GetCategories();
  
  if(CatList.GetN() < 1)
    return nullptr;
  
  // Processes
  string label;
  TH2D* hist = nullptr;

  CategoryList cat = CatList.Filter(CT);
  int Ncat = cat.GetN();
  
  for(int i = 0; i < Nproc; i++){
    VS vproc;
    if(m_Strings.count(proc[i]) != 0)
      vproc = m_Strings[proc[i]];
    else
      vproc += proc[i];
    
    ProcessList procs;
    
    ProcessType type = kBkg;
    
    for(int p = 0; p < int(vproc.size()); p++){
      
      int index = GetProcesses().Find(vproc[p]);
      if(index < 0)
	continue;
      
      Process pp = GetProcesses()[index];

      procs += pp;

      if(pp.Type() == kSig){
	type = kSig;
      }
      
      if(pp.Type() == kData){
	type = kData;
      }

      for(int c = 0; c < Ncat; c++){
	if(GetHistogram2D(cat[c],pp)){
	  if(!hist)
	    hist = (TH2D*) GetHistogram2D(cat[c],pp)
	      ->Clone(Form("plothist2D_%s", can_name.c_str()));
	  else
	    hist->Add(GetHistogram2D(cat[c], pp));
	}
      }
    }
    if(hist == nullptr)
      continue;
    
    if(type == kData){
      label = "Data";
    }

    if(type == kSig){
      label = GetSignalTitle(proc[i]);
    }

    if(type == kBkg){
      if(m_Title.count(proc[i]) != 0)
	label = m_Title[proc[i]];
      else
	label = proc[i];
    }
  }

  if(!hist)
    return nullptr;
  
  const FitBin& bin = cat[0].GetFitBin();

  int NR = bin.NRBins();
  int NB = bin.NBins();
  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = new TCanvas(Form("can_%s", can_name.c_str()),
			     Form("can_%s", can_name.c_str()),
			     700, 700);
  double eps = 0.0015;

  double hlo = 0.14;
  double hhi = 0.18;
  double hbo = 0.13;
  double hto = 0.17;
  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->Draw();
  can->cd();
  
  hist->Draw("colz");
  hist->GetXaxis()->SetTitle("M_{#perp}   [GeV]");
  hist->GetYaxis()->SetTitle("R_{ISR}");
  hist->GetZaxis()->SetTitle("number of events");
  
  hist->GetXaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleSize(0.05);
  hist->GetXaxis()->SetTitleOffset(1.15);
  hist->GetXaxis()->SetLabelFont(42);
  hist->GetXaxis()->SetLabelSize(0.04);
  hist->GetXaxis()->SetTickSize(0.);
  
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetYaxis()->SetTitleSize(0.05);
  hist->GetYaxis()->SetTitleOffset(1.15);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelSize(0.035);
  hist->GetYaxis()->SetTickSize(0.);
  
  hist->GetZaxis()->CenterTitle();
  hist->GetZaxis()->SetTitleFont(42);
  hist->GetZaxis()->SetTitleSize(0.05);
  hist->GetZaxis()->SetTitleOffset(0.9);

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(7024);
 
  l.SetTextSize(0.025);
  l.SetTextFont(42);
  l.SetTextAlign(23);
  line->SetLineWidth(3);
  line->SetLineColor(7024);
  line->SetLineStyle(7);
 
  for(int r = 0; r < NR; r++){
    line->DrawLine(hist->GetXaxis()->GetXmin(), bin[r].Rlow(),
		   hist->GetXaxis()->GetXmax(), bin[r].Rlow());
    line->DrawLine(hist->GetXaxis()->GetXmin(), bin[r].Rhigh(),
		   hist->GetXaxis()->GetXmax(), bin[r].Rhigh());

    for(int b = 0; b <  bin[r].BinEdges().size()-1; b++){
      line->DrawLine(bin[r].BinEdges()[b], bin[r].Rlow(),
		     bin[r].BinEdges()[b], bin[r].Rhigh());
    }
  }

  l.SetTextAngle(0);
  l.SetTextColor(kBlack);
  l.SetTextSize(0.04);
  l.SetTextAlign(31);
  l.DrawLatex(1.-hhi+eps*10, 1.-hto+0.02, string(CT.GetSpectroscopicLabel()).c_str());
  l.SetTextAlign(11);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02, m_CMSLabel.c_str());
  l.SetTextAlign(33);
  l.DrawLatex(1.-hhi-eps*6, 1.-hto-0.02, label.c_str());
  
  return can;
}




void FitPlotter::InitializeRecipes(){
  // Processes

  m_Strings["Data"] = VS().a("data_obs");
  
  m_Title["ttbar"] = "t #bar{t} + jets";
  m_Color["ttbar"] = 7011;

  m_Title["ST"] = "single top";
  m_Color["ST"] = 7010;

  m_Title["DB"] = "di-bosons";
  m_Color["DB"] = 7051;

  m_Title["TB"] = "tri-bosons / t #bar{t} + V";
  m_Color["TB"] = 7050;

  m_Title["ZDY"] = "Z / #gamma* + jets";
  m_Color["ZDY"] = 7000;

  m_Title["Wjets"] = "W + jets";
  m_Color["Wjets"] = 7001;

  m_Title["QCD"] = "QCD multi-jets";
  m_Color["QCD"] = 7023;
  m_Strings["QCD"] = VS().a("QCD_Fakes_elf0").a("QCD_Fakes_elf1").a("QCD_Fakes_elf2")
    .a("QCD_Fakes_muf0").a("QCD_Fakes_muf1").a("QCD_Fakes_muf2").a("QCD");

  m_Title["ttbar_all"] = "t #bar{t} + jets W/Fakes";
  m_Color["ttbar_all"] = 7011;
  m_Strings["ttbar_all"] = VS().a("ttbar").a("ttbar_Fakes_elf0").a("ttbar_Fakes_muf0").a("ttbar_Fakes_elf1").a("ttbar_Fakes_muf1");

  m_Title["ST_all"] = "single top W/Fakes";
  m_Color["ST_all"] = 7010;
  m_Strings["ST_all"] = VS().a("ST").a("ST_Fakes_elf0").a("ST_Fakes_muf0").a("ST_Fakes_elf1").a("ST_Fakes_muf1");

  m_Title["DB_all"] = "di-bosons W/Fakes";
  m_Color["DB_all"] = 7051;
  m_Strings["DB_all"] = VS().a("DB").a("DB_Fakes_elf0").a("DB_Fakes_muf0").a("DB_Fakes_elf1").a("DB_Fakes_muf1");

  m_Title["TB_all"] = "tri-bosons / t #bar{t} + V W/Fakes";
  m_Color["TB_all"] = 7050;
  m_Strings["TB_all"] = VS().a("TB").a("TB_Fakes_elf0").a("TB_Fakes_muf0").a("TB_Fakes_elf1").a("TB_Fakes_muf1");

  m_Title["ZDY_all"] = "Z / #gamma* + jets W/Fakes";
  m_Color["ZDY_all"] = 7000;
  m_Strings["ZDY_all"] = VS().a("ZDY").a("ZDY_Fakes_elf0").a("ZDY_Fakes_muf0").a("ZDY_Fakes_elf1").a("ZDY_Fakes_muf1");

  m_Title["Wjets_all"] = "W + jets W/Fakes";
  m_Color["Wjets_all"] = 7001;
  m_Strings["Wjets_all"] = VS().a("Wjets").a("Wjets_Fakes_elf0").a("Wjets_Fakes_muf0").a("Wjets_Fakes_elf1").a("Wjets_Fakes_muf1");

  m_Title["Non-Rare"] = "Non-Rare Backgrounds";
  m_Color["Non-Rare"] = 7001;
  m_Strings["Non-Rare"] = VS().a("QCD_Fakes_elf0").a("QCD_Fakes_elf1").a("QCD_Fakes_elf2")
    .a("QCD_Fakes_muf0").a("QCD_Fakes_muf1").a("QCD_Fakes_muf2").a("QCD").a("ttbar").a("ttbar_Fakes_elf0")
    .a("ttbar_Fakes_muf0").a("ttbar_Fakes_elf1").a("ttbar_Fakes_muf1").a("DB").a("DB_Fakes_elf0").a("DB_Fakes_muf0")
    .a("DB_Fakes_elf1").a("DB_Fakes_muf1").a("ZDY").a("ZDY_Fakes_elf0").a("ZDY_Fakes_muf0").a("ZDY_Fakes_elf1").a("ZDY_Fakes_muf1")
    .a("Wjets").a("Wjets_Fakes_elf0").a("Wjets_Fakes_muf0").a("Wjets_Fakes_elf1").a("Wjets_Fakes_muf1");

  m_Title["HF_Fakes"] = "HF leptons";
  m_Color["HF_Fakes"] = 7022;
  m_Strings["HF_Fakes"] = VS().a("ttbar_Fakes_elf0").a("ttbar_Fakes_muf0").a("ST_Fakes_elf0").a("ST_Fakes_muf0")
    .a("DB_Fakes_elf0").a("DB_Fakes_muf0").a("TB_Fakes_elf0").a("TB_Fakes_muf0").a("ZDY_Fakes_elf0")
    .a("ZDY_Fakes_muf0").a("Wjets_Fakes_elf0").a("Wjets_Fakes_muf0");

  m_Title["LF_Fakes"] = "LF/fake leptons";
  m_Color["LF_Fakes"] = 7021;
  m_Strings["LF_Fakes"] = VS().a("ttbar_Fakes_elf1").a("ttbar_Fakes_muf1").a("ST_Fakes_elf1").a("ST_Fakes_muf1")
    .a("DB_Fakes_elf1").a("DB_Fakes_muf1").a("TB_Fakes_elf1").a("TB_Fakes_muf1").a("ZDY_Fakes_elf1")
    .a("ZDY_Fakes_muf1").a("Wjets_Fakes_elf1").a("Wjets_Fakes_muf1");
  
  m_Title["Fakes"] = "fake leptons";
  m_Color["Fakes"] = 7021;
  m_Strings["Fakes"] = VS().a("Fakes_elf0").a("Fakes_elf1").a("Fakes_elf2").a("Fakes_muf0").a("Fakes_muf1").a("Fakes_muf2");
  
  m_Title["Fake"] = "fake";
  m_Color["Fake"] = 7020;
  m_Strings["Fake"] = VS().a("Fakes_elf2").a("Fakes_muf2");

  m_Title["HF"] = "heavy flavor";
  m_Color["HF"] = 7022;
  m_Strings["HF"] = VS().a("Fakes_elf0").a("Fakes_muf0");

  m_Title["LF"] = "light flavor";
  m_Color["LF"] = 7021;
  m_Strings["LF"] = VS().a("Fakes_elf1").a("Fakes_muf1");

  m_Title["Total"] = "total background";
  m_Color["Total"] = 7000;
  m_Strings["Total"] = VS().a("ttbar").a("ST").a("DB").a("ZDY").a("Wjets").a("Fakes_elf0").a("Fakes_elf1").
    a("Fakes_elf2").a("Fakes_muf0").a("Fakes_muf1").a("Fakes_muf2");
  
  // leptonic categories
  m_Title["1L"] = "#scale[1.2]{single #it{l}}";
  m_Strings["1L"] = VS().a("1L_elm_elG").a("1L_elp_elG").a("1L_elpm_elG").a("1L_mupm_muG").a("1L_mup_muG").a("1L_mum_muG");
  
  m_Title["1Lel"] = "#scale[1.2]{single e}";
  m_Strings["1Lel"] = VS().a("1L_elp_elG").a("1L_elm_elG");

  m_Title["1Lmu"] = "#scale[1.2]{single #mu}";
  m_Strings["1Lmu"] = VS().a("1L_mup_muG").a("1L_mum_mG");

  m_Title["1Lelp"] = "#scale[1.2]{single e^{+}}";
  m_Strings["1Lelp"] = VS().a("1L_elp_elG");

  m_Title["1Lelm"] = "#scale[1.2]{single e^{-}}";
  m_Strings["1Lelm"] = VS().a("1L_elm_elG");

  m_Title["1Lmup"] = "#scale[1.2]{single #mu^{+}}";
  m_Strings["1Lmup"] = VS().a("1L_mup_muG");

  m_Title["1Lmum"] = "#scale[1.2]{single #mu^{-}}";
  m_Strings["1Lmum"] = VS().a("1L_mum_muG");

  m_Title["1Lp"] = "#scale[1.2]{single #it{l}^{+}}";
  m_Strings["1Lp"] = VS().a("1L_elp_elG").a("1L_mup_muG");

  m_Title["1Lm"] = "#scale[1.2]{single #it{l}^{-}}";
  m_Strings["1Lm"] = VS().a("1L_elm_elG").a("1L_mum_muG");

  m_Title["1Lsilver"] = "#scale[1.2]{single silver #it{l}}";
  m_Strings["1Lsilver"] = VS().a("1L_elp_elS").a("1L_elm_elS").a("1L_mup_muS").a("1L_mum_muS");
  
  m_Title["1Lelsilver"] = "#scale[1.2]{single silver e}";
  m_Strings["1Lelsilver"] = VS().a("1L_elp_elS").a("1L_elm_elS");
  
  m_Title["1Lmusilver"] = "#scale[1.2]{single silver #mu}";
  m_Strings["1Lmusilver"] = VS().a("1L_mup_muS").a("1L_mum_muS");

  m_Title["1Lbronze"] = "#scale[1.2]{single bronze #it{l}}";
  m_Strings["1Lbronze"] = VS().a("1L_elp_elB").a("1L_elm_el2").a("1L_mup_muB").a("1L_mum_muB");
  
  m_Title["1Lelbronze"] = "#scale[1.2]{single bronze e}";
  m_Strings["1Lelbronze"] = VS().a("1L_elp_elB").a("1L_elm_elB");
  
  m_Title["1Lmubronze"] = "#scale[1.2]{single bronze #mu}";
  m_Strings["1Lmubonze"] = VS().a("1L_mup_muB").a("1L_mum_muB");

  m_Title["2LOSSF"] = "#scale[1.2]{e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}}";
  m_Strings["2LOSSF"] = VS().a("2LOS_el^el_elGelG").a("2LOS_mu^mu_muGmuG").a("2LOS_elel^0_elGelG").a("2LOS_mumu^0_muGmuG");
  
  m_Title["2LOSOF"] = "#scale[1.2]{e^{#pm} #mu^{#mp}}";
  m_Strings["2LOSOF"] = VS().a("2LOS_el^mu_elGmuG").a("2LOS_elmu^0_elGmuG");

  m_Title["2LSSSF"] = "#scale[1.2]{e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}}";
  m_Strings["2LSSSF"] = VS().a("2LOS_el^el_elGelG").a("2LOS_mu^mu_muGmuG").a("2LOS_elel^0_elGelG").a("2LOS_mumu^0_muGmuG");
  
  m_Title["2LSSOF"] = "#scale[1.2]{e^{#pm} #mu^{#pm}}";
  m_Strings["2LSSOF"] = VS().a("2LSS_el^mu_elGmuG").a("2LSS_elmu^0_elGmuG");

  m_Title["2LOSSFsilver"] = "#scale[1.2]{e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}, #geq 1 silver #it{l}}";
  m_Strings["2LOSSFsilver"] = VS().a("2LOS_el^el_el0el1").a("2LOS_mu^mu_mu0mu1").a("2LOS_elel^0_el0el1").a("2LOS_mumu^0_mu0mu1")
    .a("2LOS_el^el_el1el1").a("2LOS_mu^mu_mu1mu1").a("2LOS_elel^0_el1el1").a("2LOS_mumu^0_mu1mu1")
    .a("2LOS_el^el_el1el2").a("2LOS_mu^mu_mu1mu2").a("2LOS_elel^0_el1el2").a("2LOS_mumu^0_mu1mu2");
  
  m_Title["2LOSOFsilver"] = "#scale[1.2]{e^{#pm} #mu^{#mp}, #geq 1 silver #it{l}}";
  m_Strings["2LOSOFsilver"] = VS().a("2LOS_el^mu_el0mu1").a("2LOS_elmu^0_el0mu1").a("2LOS_el^mu_mu0el1").a("2LOS_elmu^0_mu0el1")
    .a("2LOS_el^mu_el1mu1").a("2LOS_elmu^0_el1mu1").a("2LOS_el^mu_mu1el2").a("2LOS_elmu^0_mu1el2")
    .a("2LOS_el^mu_el1mu2").a("2LOS_elmu^1_el1mu2");

  m_Title["2LSSSFsilver"] = "#scale[1.2]{e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}, #geq 1 silver #it{l}}";
  m_Strings["2LSSSFsilver"] = VS().a("2LSS_el^el_el0el1").a("2LSS_mu^mu_mu0mu1").a("2LSS_elel^0_el0el1").a("2LSS_mumu^0_mu0mu1")
    .a("2LSS_el^el_el1el1").a("2LSS_mu^mu_mu1mu1").a("2LSS_elel^0_el1el1").a("2LSS_mumu^0_mu1mu1")
    .a("2LSS_el^el_el1el2").a("2LSS_mu^mu_mu1mu2").a("2LSS_elel^0_el1el2").a("2LSS_mumu^0_mu1mu2");
  
  m_Title["2LSSOFsilver"] = "#scale[1.2]{e^{#pm} #mu^{#pm}, #geq 1 silver #it{l}}";
  m_Strings["2LSSOFsilver"] = VS().a("2LSS_el^mu_el0mu1").a("2LSS_elmu^0_el0mu1").a("2LSS_el^mu_mu0el1").a("2LSS_elmu^0_mu0el1")
    .a("2LSS_el^mu_el1mu1").a("2LSS_elmu^0_el1mu1").a("2LSS_el^mu_mu1el2").a("2LSS_elmu^0_mu1el2")
    .a("2LSS_el^mu_el1mu2").a("2LSS_elmu^1_el1mu2");

  m_Title["2LOSSFbronze"] = "#scale[1.2]{e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}, #geq 1 bronze #it{l}}";
  m_Strings["2LOSSFbronze"] = VS().a("2LOS_el^el_el0el2").a("2LOS_mu^mu_mu0mu2").a("2LOS_elel^0_el0el2").a("2LOS_mumu^0_mu0mu2")
    .a("2LOS_el^el_el1el2").a("2LOS_mu^mu_mu1mu2").a("2LOS_elel^0_el1el2").a("2LOS_mumu^0_mu1mu2")
    .a("2LOS_el^el_el2el2").a("2LOS_mu^mu_mu2mu2").a("2LOS_elel^0_el2el2").a("2LOS_mumu^0_mu2mu2");
  
  m_Title["2LOSOFbronze"] = "#scale[1.2]{e^{#pm} #mu^{#mp}, #geq 1 bronze #it{l}}";
  m_Strings["2LOSOFbronze"] = VS().a("2LOS_el^mu_el0mu2").a("2LOS_elmu^0_el0mu2").a("2LOS_el^mu_mu0el2").a("2LOS_elmu^0_mu0el2")
    .a("2LOS_el^mu_el1mu2").a("2LOS_elmu^0_el1mu2").a("2LOS_el^mu_mu1el2").a("2LOS_elmu^0_mu1el2")
    .a("2LOS_el^mu_el2mu2").a("2LOS_elmu^1_el2mu2");

  m_Title["2LSSSFbronze"] = "#scale[1.2]{e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}, #geq 1 bronze #it{l}}";
  m_Strings["2LSSSFbronze"] = VS().a("2LSS_el^el_el0el2").a("2LSS_mu^mu_mu0mu2").a("2LSS_elel^0_el0el2").a("2LSS_mumu^0_mu0mu2")
    .a("2LSS_el^el_el1el2").a("2LSS_mu^mu_mu1mu2").a("2LSS_elel^0_el1el2").a("2LSS_mumu^0_mu1mu2")
    .a("2LSS_el^el_el2el2").a("2LSS_mu^mu_mu2mu2").a("2LSS_elel^0_el2el2").a("2LSS_mumu^0_mu2mu2");
  
  m_Title["2LSSOFbronze"] = "#scale[1.2]{e^{#pm} #mu^{#pm}, #geq 1 bronze #it{l}}";
  m_Strings["2LSSOFbronze"] = VS().a("2LSS_el^mu_el0mu2").a("2LSS_elmu^0_el0mu2").a("2LSS_el^mu_mu0el2").a("2LSS_elmu^0_mu0el2")
    .a("2LSS_el^mu_el1mu2").a("2LSS_elmu^0_el1mu2").a("2LSS_el^mu_mu1el2").a("2LSS_elmu^0_mu1el2")
    .a("2LSS_el^mu_el2mu2").a("2LSS_elmu^1_el2mu2");

  // hadronic categories
  m_Title["0j0svS"] = "#splitline{0 jets}{0 SV-tags} #scale[1.2]{#in S}";

  m_Title["0j1svS"] = "#splitline{0 jets}{1 SV-tag} #scale[1.2]{#in S}";

  m_Title["0jge1svS"] = "#splitline{0 jets}{#geq 1 SV-tag} #scale[1.2]{#in S}";

  m_Title["0jge2svS"] = "#splitline{0 jets}{#geq 2 SV-tags} #scale[1.2]{#in S}";

  m_Title["1j0bge1svS"] = "#splitline{1 jet, 0 b-tags}{#geq 1 SV-tag} #scale[1.2]{#in S}";

  m_Title["2j0bS"] = "#splitline{2 jets}{0 b-tags} #scale[1.2]{#in S}";

  m_Title["2j1bS"] = "#splitline{2 jets}{1 b-tags} #scale[1.2]{#in S}";

  m_Title["2j2bS"] = "#splitline{2 jets}{2 b-tags} #scale[1.2]{#in S}";

  m_Title["3j0bS"] = "#splitline{3 jets}{0 b-tags} #scale[1.2]{#in S}";

  m_Title["3j1bS"] = "#splitline{3 jets}{1 b-tags} #scale[1.2]{#in S}";

  m_Title["3j2bS"] = "#splitline{3 jets}{#geq 2 b-tags} #scale[1.2]{#in S}";
  
  m_Title["ge1j0bISR"] = "#splitline{#geq 1 jet}{0 b-tags} #scale[1.2]{#in ISR}";

  m_Title["ge1jge1bISR"] = "#splitline{#geq 1 jet}{#geq 1 b-tags} #scale[1.2]{#in ISR}";

  m_Title["ge1jISR"] = "#splitline{#geq 1 jet}{incl. b-tags} #scale[1.2]{#in ISR}";

  m_Title["ge1j0bS"] = "#splitline{#geq 1 jet}{0 b-tags} #scale[1.2]{#in S}";

  m_Title["ge1j1bS"] = "#splitline{#geq 1 jet}{1 b-tag} #scale[1.2]{#in S}";

  m_Title["ge2bjetS"] = "#splitline{#geq 1 jet}{#geq 2 b-tags} #scale[1.2]{#in S}";

  m_ColorDefault.clear();
  for(int i = 0; i < 8; i++)
    m_ColorDefault.push_back(7002+i*10);
  for(int i = 0; i < 8; i++)
    m_ColorDefault.push_back(7000+i*10);
  for(int i = 0; i < 8; i++)
    m_ColorDefault.push_back(7004+i*10);

  m_SignalColor.clear();
  m_SignalColor.push_back(7043);
  m_SignalColor.push_back(7071);
  m_SignalColor.push_back(7041);
  m_SignalColor.push_back(7061);
  m_SignalColor.push_back(7040);

  m_CMSLabel = "#bf{#it{CMS}} work-in-progress";
}

TCanvas* FitPlotter::PlotCatSignificance(const string& can_name,
					 const VS& proc_bkg,
					 const VS& proc_sig,
					 const CategoryTree& CT,
					 SignificanceType sType){

  RestFrames::SetStyle();

  ProcessList bkgs;
  ProcessList sigs;

  int Nproc_bkg = proc_bkg.size();
  if(Nproc_bkg == 0)
    return nullptr;

  int Nproc_sig = proc_sig.size();
  if(Nproc_sig == 0)
    return nullptr;

  int Nvis = CT.GetNVisible();
  if(Nvis < 1)
    return nullptr;

  vector<const CategoryTree*> CatTrees;
  CT.GetListVisible(CatTrees);
  CategoryList CatList = GetCategories();

  for(int b = 0; b < Nproc_bkg; b++){

    int index = GetProcesses().Find(proc_bkg[b]);
    if(index < 0)
      continue;

    Process pp = GetProcesses()[index];
    bkgs += pp;
  }

  for(int s = 0; s < Nproc_sig; s++){

    int index = GetProcesses().Find(proc_sig[s]);
    if(index < 0)
      continue;

    Process pp = GetProcesses()[index];
    sigs +=pp;
  }

  int Nbkg = bkgs.GetN();
  int Nsig = sigs.GetN();
  int Ncat = 0;

  double nBkgEvts[Nvis]; // vector over groups of categories
  double nSigEvts[Nsig][Nvis];
  //vector<double> X;

  for(int v = 0; v < Nvis; v++){
    CategoryList cat = CatList.Filter(*CatTrees[v]);

    Ncat = cat.GetN();

    //X.push_back(v);

    nBkgEvts[v] = 0.;
    for(int s = 0; s < Nsig; s++)
      nSigEvts[s][v] = 0.;

    for(int c = 0; c < Ncat; c++){
      // bkg loop
      for(int b = 0; b < Nbkg; b++){
        nBkgEvts[v] += Integral(cat[c], bkgs[b]);
      }

      // sig loop
      for(int s = 0; s < Nsig; s++){
        nSigEvts[s][v] += Integral(cat[c], sigs[s]);
      }
    }
  }

  vector<double> sOverB;
  vector<double> maxSB;
  double tempSB = 0.;
  double max = 0.;

  for(int s = 0; s < Nsig; s++){
    tempSB = 0.;
    for(int v = 0; v < Nvis; v++){

      if(sType == kSB && tempSB < nSigEvts[s][v]/nBkgEvts[v])
	  tempSB = nSigEvts[s][v]/nBkgEvts[v];
      
      if(sType == kSrootB && tempSB < nSigEvts[s][v]/TMath::Sqrt(nBkgEvts[v]))
	tempSB = nSigEvts[s][v]/TMath::Sqrt(nBkgEvts[v]);
      
      if(sType == kZbin && tempSB < calculateZbi(nSigEvts[s][v],nBkgEvts[v]))
	tempSB = calculateZbi(nSigEvts[s][v],nBkgEvts[v]);
    }
    if(max < tempSB)
      max = tempSB; 

    maxSB.push_back(tempSB);
  }

  //Plotting
  double ratioType = -999.;
  vector<int> clrs = {kBlue+2,kGreen+2,kRed+2,kOrange-3,kMagenta-1,kBlack,kAzure+2,kGray};
  vector<int> markers = {20,21,22,23,29,33,34,43,49};
  vector<TH1D*> fhists;

  for(int s = 0; s < Nsig; s++){
    fhists.push_back(new TH1D(Form("fhist_%d_%s", s, can_name.c_str()),
				  Form("fhist_%d_%s", s, can_name.c_str()),
				  Nvis, 0., Nvis));

    for(int v = 0; v < Nvis; v++){
      //if(nSigEvts[s][v]/nBkgEvts[v]){

      if(sType == kSB)
	ratioType = nSigEvts[s][v]/nBkgEvts[v];

      if(sType == kSrootB)
	ratioType = nSigEvts[s][v]/TMath::Sqrt(nBkgEvts[v]);

      if(sType == kZbin)
        ratioType = calculateZbi(nSigEvts[s][v],nBkgEvts[v]);

      fhists[s]->SetBinContent(v+1, ratioType);
      fhists[s]->SetBinError(v+1, ratioType);
	//}
    }
  }

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = new TCanvas(Form("can_%s", can_name.c_str()),
                             Form("can_%s", can_name.c_str()),
                             1200, 700);
  double hlo = 0.09;
  double hhi = 0.2;
  double hbo = 0.19;
  double hto = 0.07;
  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->Draw();
  can->cd();

  TString yLabel = "";
  if(sType == kSB)
    yLabel = "S/B";
  if(sType == kSrootB)
    yLabel = "S/#sqrt{B}";
  if(sType == kZbin)
    yLabel = "Z_{bi}";

  fhists[0]->SetMarkerSize(2);
  fhists[0]->SetMarkerColor(m_SignalColor[0]);
  fhists[0]->SetMarkerStyle(markers[0]);
  fhists[0]->Draw("hist p");
  fhists[0]->GetXaxis()->CenterTitle();
  fhists[0]->GetXaxis()->SetTitleFont(42);
  fhists[0]->GetXaxis()->SetTitleSize(0.05);
  fhists[0]->GetXaxis()->SetTitleOffset(1.0);
  fhists[0]->GetXaxis()->SetLabelFont(42);
  fhists[0]->GetXaxis()->SetLabelSize(std::min(2./double(Nvis), 0.04));
  fhists[0]->GetXaxis()->SetTitle("");
  fhists[0]->GetXaxis()->SetTickSize(0.);
  fhists[0]->GetYaxis()->CenterTitle();
  fhists[0]->GetYaxis()->SetTitleFont(42);
  fhists[0]->GetYaxis()->SetTitleSize(0.04);
  fhists[0]->GetYaxis()->SetTitleOffset(0.85);
  fhists[0]->GetYaxis()->SetLabelFont(42);
  fhists[0]->GetYaxis()->SetLabelSize(0.035);
  fhists[0]->GetYaxis()->SetRangeUser(0.,max*1.2);
  fhists[0]->GetYaxis()->SetTitle(yLabel);

  for(int v = 0; v < Nvis; v++){
    fhists[0]->GetXaxis()->SetBinLabel(v+1, CatTrees[v]->GetSpectroscopicLabel().c_str());
  }
  fhists[0]->LabelsOption("v","X");

  for(int s = 1; s < Nsig; s++){
    fhists[s]->SetMarkerSize(2);
    fhists[s]->SetMarkerColor(m_SignalColor[s]);
    fhists[s]->SetMarkerStyle(markers[s]);  
    fhists[s]->Draw("same hist p");
  }

  TLegend* leg = new TLegend(1.-hhi+0.007, 1.- (Nproc_sig+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);

  for(int s = 0; s < Nsig; s++){
    if(sigs[s].Type() == kSig)
      leg->AddEntry(fhists[s],TString(GetSignalTitle(proc_sig[s])), "P");
    if(sigs[s].Type() == kBkg)
      leg->AddEntry(fhists[s],TString(m_Title[proc_sig[s].c_str()]), "P");
  }
  leg->Draw("SAME");

  double eps = 0.0015;
  
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  //line->DrawLineNDC(hlo, hbo-0.024*lmax, 1-hhi, hbo-0.0235*lmax);
 
  l.SetTextSize(0.025);
  l.SetTextFont(42);
  l.SetTextAlign(23);
  line->SetLineWidth(1);
  double lo = hlo;
  double hi = hlo;
 
 
  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, string("Regions "+CT.GetSpectroscopicLabel()).c_str());
  l.SetTextAlign(11);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02, m_CMSLabel.c_str());
  l.SetTextSize(0.05);

  can->SetGridy();
  can->SetGridx();
  can->Update(); 
  can->SaveAs("plots/"+TString(can_name)+".pdf");

  return can;

}

TCanvas* FitPlotter::PlotRegionSignificance(const string& can_name,
					    const VS& proc_bkg,
					    const VS& proc_sig,
					    const CategoryTree& CT,
					    SignificanceType sType){

  RestFrames::SetStyle();

  ProcessList bkgs;
  ProcessList sigs;

  int Nbkg = proc_bkg.size();
  if(Nbkg == 0)
    return nullptr;

  int Nsig = proc_sig.size();
  if(Nsig == 0)
    return nullptr;

  vector<const CategoryTree*> CatTrees;
  CT.GetListDeepest(CatTrees);

  int Nvis = CatTrees.size();
  
  if(Nvis < 1)
    return nullptr;
  
  CategoryList CatList = GetCategories();

  if(CatList.GetN() < 1)
    return nullptr;

  //cout << Nvis << endl;

  vector<TH1D*> hists_bkg[Nvis];
  vector<TH1D*> hists_sig[Nvis];

  CategoryList dumcat = CatList.Filter(*CatTrees[0]);
  const FitBin& fitbin = dumcat[0].GetFitBin();
  int Nbin = fitbin.NBins();
  int Ncat = 0;

  /*
  for(int v = 0; v < Nvis; v++){
    CategoryList cat = CatList.Filter(*CatTrees[v]);

    Ncat = cat.GetN();
    cout << "Ncat: " << Ncat << endl;
  }

  cout << "Nbin: " << Nbin << endl;
  cout << "Nvis: " << Nvis << endl;
  cout << "Nsig: " << Nsig << endl;
  cout << "Nbkg: " << Nbkg << endl;
  */

  for(int b = 0; b < Nbkg; b++){

    int index = GetProcesses().Find(proc_bkg[b]);
    if(index < 0)
      continue;

    double itot = 0.;

    Process pp = GetProcesses()[index];
    bkgs += pp;
  }

  for(int s = 0; s < Nsig; s++){

    int index = GetProcesses().Find(proc_sig[s]);
    if(index < 0)
      continue;

    Process pp = GetProcesses()[index];
    sigs +=pp;
  }

  Nbkg = bkgs.GetN();
  Nsig = sigs.GetN();

  double nBkgEvts[Nvis][Nbin]; 
  double nSigEvts[Nsig][Nvis][Nbin];

  //cout << Nvis << endl;

  for(int v = 0; v < Nvis; v++){

    CategoryList cat = CatList.Filter(*CatTrees[v]);

    Ncat = cat.GetN();

    TH1D* hbkg = GetAddedHist(Form("plothist_%d_%s", v, can_name.c_str()), cat, bkgs);

    if(!hbkg)
      continue;

    for(int b = 0; b < Nbin; b++){ 

      nBkgEvts[v][b] = hbkg->GetBinContent(b+1);
    }

    for(int s = 0; s < Nsig; s++){
      ProcessList list;
      list += sigs[s];
      TH1D* hsig = GetAddedHist(Form("plothist_%d_%d_%s", s, v, can_name.c_str()), cat, list);
      if(!hsig)
	continue;
      for(int b = 0; b < Nbin; b++){
	nSigEvts[s][v][b] = hsig->GetBinContent(b+1);
      }
      if(hsig)
	delete hsig;
    }
    if(hbkg)
      delete hbkg;
  }
  
  double tempSB = 0.;
  double max = 0.;
  
  //Get max plot value
  for(int s = 0; s < Nsig; s++){
    tempSB = 0.;

    for(int v = 0; v < Nvis; v++){
      for (int b = 0; b < Nbin; b++){
	if(tempSB < nSigEvts[s][v][b]/nBkgEvts[v][b] && sType == kSB)
	  tempSB = nSigEvts[s][v][b]/nBkgEvts[v][b];

	if(tempSB < nSigEvts[s][v][b]/TMath::Sqrt(nBkgEvts[v][b]) && sType == kSrootB)
          tempSB = nSigEvts[s][v][b]/TMath::Sqrt(nBkgEvts[v][b]);

	if(tempSB < calculateZbi(nSigEvts[s][v][b],nBkgEvts[v][b]) && sType == kZbin){
          tempSB = calculateZbi(nSigEvts[s][v][b],nBkgEvts[v][b]);
	  //if(tempSB < 0.)
	  //cout << tempSB << endl;
	}
      }
    }
    if(max < tempSB)
      max = tempSB;

  }
  
  //plot
  double ratioType = -999.;
  vector<TH1D*> fhists;
  vector<int> markers = {20,21,22,23,29,33,34,43,49};

  for(int s = 0; s < Nsig; s++){
    fhists.push_back(new TH1D(Form("fhist_%d_%s", s, can_name.c_str()),
			      Form("fhist_%d_%s", s, can_name.c_str()),
			      Nvis*Nbin, 0., Nvis*Nbin));

    for(int v = 0; v < Nvis; v++){
      //if(nBkgEvts[v].size()<1)
      //continue;
      for(int b = 0; b < Nbin; b++){
	
	if(sType == kSB)
	  ratioType = nSigEvts[s][v][b]/nBkgEvts[v][b];

	if(sType == kSrootB)
          ratioType = nSigEvts[s][v][b]/TMath::Sqrt(nBkgEvts[v][b]);

	if(sType == kZbin)
          ratioType = calculateZbi(nSigEvts[s][v][b],nBkgEvts[v][b]);
	
	fhists[s]->SetBinContent(b*Nvis+v+1, ratioType);
	fhists[s]->SetBinError(b*Nvis+v+1, ratioType);
      }                                                                          
    }
  }

  TString yLabel = "";
  if(sType == kSB)
    yLabel = "S/B";
  if(sType == kSrootB)
    yLabel = "S/#sqrt{B}";
  if(sType == kZbin)
    yLabel = "Z_{bi}";
    
  for(int b = 0; b < Nbin*Nvis; b++)
    fhists[0]->GetXaxis()->SetBinLabel(b+1, "");
  
  fhists[0]->LabelsOption("v","X");

  //gROOT->SetBatch(kTRUE);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = new TCanvas(Form("can_%s", can_name.c_str()),
                             Form("can_%s", can_name.c_str()),
                             1200, 700);
  double hlo = 0.1;
  double hhi = 0.19;
  double hbo = 0.19;
  double hto = 0.07;
  can->SetLeftMargin(hlo);
  can->SetRightMargin(hhi);
  can->SetBottomMargin(hbo);
  can->SetTopMargin(hto);
  can->SetGridx();
  can->SetGridy();
  can->Draw();
  can->cd();

  fhists[0]->SetMarkerSize(2);
  fhists[0]->SetMarkerColor(m_SignalColor[0]);
  fhists[0]->SetMarkerStyle(markers[0]);
  fhists[0]->Draw("hist p");
  fhists[0]->GetXaxis()->CenterTitle();
  fhists[0]->GetXaxis()->SetTitleFont(42);
  fhists[0]->GetXaxis()->SetTitleSize(0.045);
  fhists[0]->GetXaxis()->SetTitleOffset(1.0);
  fhists[0]->GetXaxis()->SetLabelFont(42);
  fhists[0]->GetXaxis()->SetLabelSize(0.05);
  fhists[0]->GetXaxis()->SetTitle("");
  fhists[0]->GetXaxis()->SetTickSize(0.);
  fhists[0]->GetYaxis()->CenterTitle();
  fhists[0]->GetYaxis()->SetTitleFont(42);
  fhists[0]->GetYaxis()->SetTitleSize(0.04);
  fhists[0]->GetYaxis()->SetTitleOffset(0.85);
  fhists[0]->GetYaxis()->SetLabelFont(42);
  fhists[0]->GetYaxis()->SetLabelSize(0.035);
  fhists[0]->GetYaxis()->SetRangeUser(0.,max*1.35);
  fhists[0]->GetYaxis()->SetTitle(yLabel);
  
  for(int s = 1; s < Nsig; s++){
    fhists[s]->SetMarkerSize(2);
    fhists[s]->SetMarkerColor(m_SignalColor[s]);
    fhists[s]->SetMarkerStyle(markers[s]);
    fhists[s]->Draw("same hist p");
  }

  TLegend* leg = new TLegend(1.-hhi+0.007, 1.- (Nsig+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);

  for(int s = 0; s < Nsig; s++){
    if(sigs[s].Type() == kSig)
      leg->AddEntry(fhists[s],TString(GetSignalTitle(proc_sig[s])), "P");
    if(sigs[s].Type() == kBkg)
      leg->AddEntry(fhists[s],TString(m_Title[proc_sig[s].c_str()]), "P");
    //leg->AddEntry(fhists[s], TString(m_Title[proc_sig[s].c_str()]), "P");
  }
  leg->Draw("SAME");

  DrawMR(fitbin, can);

  double eps = 0.0015;
  
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  l.SetTextSize(std::min(0.03, 1.5/double(Nvis*Nbin)));
  l.SetTextFont(42);
  l.SetTextAlign(32);
  l.SetTextAngle(90);
  int Depth = CT.GetDepth();
  for(int b = 0; b < Nvis*Nbin; b++){
    // if(colors[Nbkg-1]%1000 >=3)
    //   l.SetTextColor(7000 + 10*((b%Nvis)%8));
    // else
    l.SetTextColor(7004 + 10*((b%Nvis)%8));
    l.DrawLatex(hlo+(1.-hhi-hlo)/double(Nvis*Nbin)*(0.5+b), 1 - hto - 4*eps,
		CatTrees[b%Nvis]->GetPlainLabel(Depth).c_str());
  }

  l.SetTextAngle(0);
  l.SetTextColor(kBlack);
  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, string("Regions "+CT.GetSpectroscopicLabel()).c_str());
  l.SetTextAlign(11);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02, m_CMSLabel.c_str());

  can->Update();
  can->SaveAs("plots/"+TString(can_name)+".pdf");

  return can;

}

void FitPlotter::FindBkgZeros(const VS& proc_bkg){

  RestFrames::SetStyle();

  double thresh = 1e-5;

  ProcessList bkgs;
  int Nbkg = proc_bkg.size();

  CategoryList CatList = GetCategories();

  for(int b = 0; b < Nbkg; b++){

    VS vproc;

    if(m_Strings.count(proc_bkg[b]) != 0)
      vproc = m_Strings[proc_bkg[b]];
    else
      vproc += proc_bkg[b];

    for(int v = 0; v < vproc.size(); v++){
      
      int index = GetProcesses().Find(vproc[v]);
      if(index < 0)
	continue;
      
      double itot = 0.;
    
      Process pp = GetProcesses()[index];
      bkgs += pp;
    }
  }

  int count = 0;
  int totCat = 0;
  string tempStr = "";
  vector<double> bkgVec;
  VS catZeros;

  for(int c = 0; c < CatList.GetN(); c++){
    Category cat = CatList[c];
    CategoryList catlist;
    catlist += cat;
    const FitBin& fitbin = CatList[c].GetFitBin();
    
    bkgVec = GetAddedHistValues(catlist, bkgs);

    int ibin = 0;
    int NR = fitbin.NRBins();

    for(int r = 0; r < NR; r++){
      int NM =  fitbin.NMBins(r);
      //VS mlabels;
      VS mlabels = fitbin[r].GetMBinLabels();
      //VS rlabels;

      string rlabels = fitbin[r].GetRBinLabel();

      for(int m = 0; m < NM; m++){

	double numBkg = bkgVec[ibin];
	if(numBkg < thresh){

	  count++;
          cout << endl;
          cout << "zero number " << count << ": " << endl;
	  cout << "nBkg: " << numBkg << endl;
	  cout << "bkg bin: " << ibin << endl;
	  cout << "Mperp bin: " << mlabels[m] << endl;
          cout << "RISR bin: " << rlabels << endl;
          cout << cat.FullLabel() << endl;
	  /*
	  for(int b = 0; b < bkgs.GetN(); b++){

	    //cout << endl;
	    cout << "  Processing " << bkgs[b].Name() << " background: " << endl;

	    ProcessList bkgsZero;
	    vector<double> tempZero;

	    bkgsZero += bkgs[b];
	    //cout << "here" << endl;
	    tempZero = GetAddedHistValues(catlist, bkgsZero);
	    //cout << tempZero.size() << endl;
	    for(int i = 0; i < tempZero.size(); i++){
	      cout << "   Background bin " << i << ": " << endl;
	      cout << "    Value: " << tempZero[i] << endl;
	    }
	    tempZero.clear();

	  }
	  */
	  if(tempStr != cat.FullLabel()){
	    totCat++;
	    catZeros += cat.FullLabel();
	  }
	  tempStr = cat.FullLabel();
	}
	ibin++;
      }
    }
    bkgVec.clear();
  }
  cout << "Total zeros: " << count << endl;
  cout << "Total categories: " << totCat << endl;

  for(int i = 0; i < catZeros.size(); i++){
    cout << catZeros[i] << endl;
  }
}

void FitPlotter::FindBkgRare(){

  VS proc_bkg;
  proc_bkg.a("Non-Rare").a("ST_all").a("TB_all");

  int Nbkg = proc_bkg.size();

  CategoryList CatList = GetCategories();

  vector<double> bkgVec;

  VS rareOnlyCat;
  string tmpStr = "";

  for(int c = 0; c < CatList.GetN(); c++){

    bool foundZero = false;
    vector<int> binVec;

    Category cat = CatList[c];

    CategoryList catlist;
    catlist += cat;
    const FitBin& fitbin = CatList[c].GetFitBin();

    for(int b = 0; b < Nbkg; b++){

      ProcessList bkgs;
      VS vproc;

      if(m_Strings.count(proc_bkg[b]) != 0)
	vproc = m_Strings[proc_bkg[b]];
      else
	vproc += proc_bkg[b];

      for(int v = 0; v < vproc.size(); v++){

	int index = GetProcesses().Find(vproc[v]);
	if(index < 0)
	  continue;

	double itot = 0.;

	Process pp = GetProcesses()[index];
	bkgs += pp;
      }

      bkgVec = GetAddedHistValues(catlist, bkgs); 

      for(int i = 0; i < bkgVec.size(); i++){
	if(proc_bkg[b] == "Non-Rare" && bkgVec[i] < 1e-5){
	  foundZero = true;
	  binVec.push_back(i);
	}
      }

      if(foundZero){
	for(int j = 0; j < binVec.size(); j++){
	  if((proc_bkg[b] == "ST_all" || proc_bkg[b] == "TB_all") && bkgVec[binVec[j]] > 1e-5 && bkgVec.size()>1){
	    cout << endl;
	    cout << cat.FullLabel() << endl;
	    cout << "  " << proc_bkg[b] << ": " << endl;
	    cout << "      bin " << binVec[j] <<endl;           
	    cout << "       value: " << bkgVec[binVec[j]] << endl;
	    if(tmpStr != cat.FullLabel()){
	      rareOnlyCat +=  cat.FullLabel();
	      tmpStr = cat.FullLabel();
	    }
	  }
	}
      }    
      
    }
    foundZero = false;
  }

  cout << "Total categories with only rare backgrounds: " << rareOnlyCat.size() << endl;
  for(int i = 0; i < rareOnlyCat.size(); i++)
    cout << rareOnlyCat[i] << endl; 

}

void FitPlotter::zeroBkgTest(){

  RestFrames::SetStyle();

  VS proc_bkg;
  proc_bkg.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("HF_Fakes").a("LF_Fakes");

  //ProcessList bkgs;
  int Nbkg = proc_bkg.size();

  CategoryList CatList = GetCategories();

  Category cat = CatList[0];
  CategoryList catlist;
  catlist += cat;

  vector<double> bkgVec;
  VS catZeros;


  for(int b = 0; b < Nbkg; b++){

    cout << endl;
    cout << "Processing " << proc_bkg[b] << " background: " << endl;

    ProcessList bkgs;

    int index = GetProcesses().Find(proc_bkg[b]);
    if(index < 0)
      continue;

    double itot = 0.;

    Process pp = GetProcesses()[index];
    bkgs += pp;

    bkgVec = GetAddedHistValues(catlist, bkgs);

    for(int i = 0; i < bkgVec.size(); i++){
      cout << " Background bin " << i << ": " << endl;
      cout << " Value: " << bkgVec[i] << endl;
    }
    bkgVec.clear();
  }
}

double FitPlotter::calculateZbi(double Nsig, double Nbkg, double deltaNbkg){
  double Nobs = Nsig+Nbkg;
  double tau = 1./Nbkg/(deltaNbkg*deltaNbkg);
  double aux = Nbkg*tau;
  double Pvalue = TMath::BetaIncomplete(1./(1.+tau),Nobs,aux+1.);

  return sqrt(2.)*TMath::ErfcInverse(Pvalue*2);
}
