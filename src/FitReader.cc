#include <iostream>
#include <map>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TGraphErrors.h>
#include <TLine.h>

#include "../include/FitReader.hh"
#include "RestFrames/RestFrames.hh"

using std::cout;
using std::endl;

///////////////////////////////////////////
////////// FitReader class
///////////////////////////////////////////

FitReader::FitReader(const string& inputfile)
  : m_File(inputfile.c_str(), "READ") {

  InitializeRecipes();
  
  TTree* tree_proc = (TTree*) m_File.Get("Process");
  m_Nproc = tree_proc->GetEntries();

  string* process = 0;
  vector<string>* vcat = 0;
  TBranch* b_process;
  TBranch* b_vcat;
  tree_proc->SetMakeClass(1);
  tree_proc->SetBranchAddress("process", &process, &b_process);
  tree_proc->SetBranchAddress("cat", &vcat, &b_vcat);

  for(int i = 0; i < m_Nproc; i++){
    tree_proc->GetEntry(i);
    m_Proc[*process] = map<string,int>();
    int Ncat = vcat->size();
    for(int j = 0; j < Ncat; j++)
      m_Proc[*process][(*vcat)[j]] = 1;
  }

  delete tree_proc;

  TTree* tree_cat = (TTree*) m_File.Get("Category");
  m_Ncat = tree_cat->GetEntries();
  
  string* cat = 0;
  vector<double>* bin_edge_x = 0;
  vector<double>* bin_edge_y = 0;
  TBranch* b_cat;
  TBranch* b_bin_edge_x;
  TBranch* b_bin_edge_y;
  tree_cat->SetMakeClass(1);
  tree_cat->SetBranchAddress("cat", &cat, &b_cat);
  tree_cat->SetBranchAddress("bin_edge_x", &bin_edge_x, &b_bin_edge_x);
  tree_cat->SetBranchAddress("bin_edge_y", &bin_edge_y, &b_bin_edge_y);

  for(int i = 0; i < m_Ncat; i++){
    tree_cat->GetEntry(i);
    m_Cat[*cat] = pair<vector<double>,vector<double> >((*bin_edge_x),(*bin_edge_y));
  }
  
  delete tree_cat;
}

FitReader::~FitReader(){
  m_File.Close();
}

void FitReader::PrintCategories(){
  map<string,pair<vector<double>,vector<double> > >::iterator c = m_Cat.begin();
  cout << "*** Fit Categories ***" << endl;
  while(c != m_Cat.end()){
    cout << c->first << endl;
    c++;
  }
  cout << endl;
}

void FitReader::PrintProcesses(){
  map<string,map<string,int> >::iterator p = m_Proc.begin();
  cout << "*** Fit Processes ***" << endl;
  while(p != m_Proc.end()){
    cout << p->first << endl;
    p++;
  }
  cout << endl;
}

TCanvas* FitReader::Plot1Dstack(const vector<string>& proc,
				const vector<string>& lep_cat,
				const vector<string>& hadS_cat,
				const vector<string>& hadI_cat,
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

  // Leptonic
  vector<string> lep_labels;
  vector<string> vlep;
  for(int i = 0; i < Nlep; i++){
    if(m_Title.count(lep_cat[i]) != 0)
      lep_labels.push_back(m_Title[lep_cat[i]]);
    else
      lep_labels.push_back(lep_cat[i]);

    if(m_Strings.count(lep_cat[i]) != 0){
      int N = m_Strings[lep_cat[i]].size();
      for(int j = 0; j < N; j++)
	vlep.push_back(m_Strings[lep_cat[i]][j]);
    } else {
      vlep.push_back(lep_cat[i]);
    }
  }

  // Hadronic S
  vector<string> hadS_labels;
  vector<string> vhadS;
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

  // Hadronic ISR
  vector<string> hadI_labels;
  vector<string> vhadI;
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

  string dum_cat = "";

  // Processes
  vector<string> labels;
  vector<int>    colors;
  vector<TH1D*>  hists;
  vector<string> slabels;
  vector<TH1D*>  shists;
  vector<pair<int,int> > smass;
  
  for(int i = 0; i < Nproc; i++){
    
    bool is_signal = false;
    for(int s = 0; s < int(m_Sig.size()); s++){
      if(proc[i].find(m_Sig[s]) != std::string::npos){
	is_signal = true;
	slabels.push_back(m_SignalTitle[m_Sig[s]]);
	int mass;
	sscanf(proc[i].c_str(), (m_Sig[s]+"_%d").c_str(), &mass);
	smass.push_back(pair<int,int>(mass/100000, mass%10000));
	break;
      }
    }
    if(!is_signal){
      if(m_Title.count(proc[i]) != 0)
	labels.push_back(m_Title[proc[i]]);
      else
	labels.push_back(proc[i]);
      
      if(m_Color.count(proc[i]) != 0)
	colors.push_back(m_Color[proc[i]]);
      else
	colors.push_back(m_ColorDefault[i]);
    }
    
    vector<string> vproc;
    if(m_Strings.count(proc[i]) != 0)
      vproc = m_Strings[proc[i]];
    else
      vproc.push_back(proc[i]);

    if(is_signal)
      shists.push_back(nullptr);
    else
      hists.push_back(nullptr);

    for(int p = 0; p < int(vproc.size()); p++){
      string proc = vproc[p];
      if(m_Proc.count(proc) == 0)
	continue;
      map<string,int>::iterator c = m_Proc[proc].begin();
      while(c != m_Proc[proc].end()){
	string cat = c->first;
	if(Match(cat, vlep) && Match(cat, vhadS) && Match(cat, vhadI)){
	  if(dum_cat == "")
	    dum_cat = cat;
	  string hist = proc+"/"+proc+"_"+cat;
	  if(is_signal){
	    if(shists[int(shists.size())-1] == nullptr){
	      shists[int(shists.size())-1] =
		(TH1D*)((TH1D*)m_File.Get(hist.c_str()))->Clone(Form("shist_p%d_%s", i, name.c_str()));
	    } else {
	      shists[int(shists.size())-1]->Add((TH1D*)m_File.Get(hist.c_str()));
	    }
	  } else {
	    if(hists[i] == nullptr){
	      hists[i] = (TH1D*)((TH1D*)m_File.Get(hist.c_str()))->Clone(Form("hist_p%d_%s", i, name.c_str()));
	    } else {
	      hists[i]->Add((TH1D*)m_File.Get(hist.c_str()));
	    }
	  }
	}
	
	c++;
      }
    }
  }
  
  // sort the histograms by integral (N^2 brute force)
  int Nbkg = hists.size();
  vector<string> vlabels;
  vector<int>    vcolors;
  vector<TH1D*>  vhists;
  string stemp;
  int    itemp;
  TH1D*  htemp;
  int miss = 0;
  for(int i = 0; i < Nbkg+miss; i++){
    if(hists[i] == nullptr){
      Nbkg--;
      miss++;
      continue;
    }
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

  vector<string> vslabels;
  vector<TH1D*>  vshists;
  vector<pair<int,int> > vsmass;
  int Nsig = slabels.size();
  miss = 0;
  for(int s = 0; s < Nsig+miss; s++){
    if(shists[s] == nullptr){
      Nsig--;
      miss++;
      continue;
    }
    vslabels.push_back(slabels[s]);
    vshists.push_back(shists[s]);
    vsmass.push_back(smass[s]);
  }
  
  // "stack" the histograms by adding
  for(int i = Nbkg-2; i >= 0; i--)
    vhists[i]->Add(vhists[i+1]);

  hists  = vhists;
  labels = vlabels;
  colors = vcolors;

  shists  = vshists;
  slabels = vslabels;
  
  vector<string> x_edges;
  vector<string> y_edges;
  int Nx = m_Cat[dum_cat].first.size()  - 1;
  int Ny = m_Cat[dum_cat].second.size() - 1;
  int max_len = 0;
  for(int i = 0; i < Nx+1; i++){
    x_edges.push_back(std::to_string(int(m_Cat[dum_cat].first[i])));
    if(x_edges[i].size() > double(max_len))
      max_len = x_edges[i].size();
  }
  for(int i = 0; i < Ny+1; i++){
    y_edges.push_back(std::to_string(m_Cat[dum_cat].second[i]));
    y_edges[i].erase(y_edges[i].find_last_not_of('0') + 1, std::string::npos);
  }

  string blank = " ";
  for(int i = 0; i < 4*max_len+3; i++)
    blank += " ";

  int Nb = hists[0]->GetNbinsX();

  vector<string> y_labels;
  for(int y = 0; y < Ny; y++){
    y_labels.push_back("["+y_edges[y]+","+y_edges[y+1]+"]");
    for(int x = 0; x < Nx; x++){
      if(x == Nx-1)
	hists[0]->GetXaxis()->
	  SetBinLabel(x+1+Nx*y, ("["+x_edges[x]+",#infty]"+(x%2 == 0 ? "" : blank)).c_str());
      else
	hists[0]->GetXaxis()->
	  SetBinLabel(x+1+Nx*y, ("["+x_edges[x]+","+x_edges[x+1]+"]"+(x%2 == 0 ? "" : blank)).c_str());
    }
  }
  
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
  //can->SetGridx();
  can->SetGridy();
  can->Draw();
  can->cd();
  
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
  hists[0]->GetYaxis()->SetTitleOffset(0.8);
  hists[0]->GetYaxis()->SetLabelFont(42);
  hists[0]->GetYaxis()->SetLabelSize(0.035);
  hists[0]->GetYaxis()->SetTitle("number of events / 137 fb^{-1}");
   
  for(int i = 0; i < Nbkg; i++){
    hists[i]->SetLineColor(kBlack);
    hists[i]->SetLineWidth(1.0);
    hists[i]->SetFillColor(colors[i]);
    hists[i]->SetFillStyle(1001);
    hists[i]->Draw("SAME HIST");
  }

  vector<double> X;
  vector<double> Xerr;
  vector<double> Y;
  vector<double> Yerr;
  for(int i = 0; i < Nb; i++){
    X.push_back(hists[0]->GetXaxis()->GetBinCenter(i+1));
    Xerr.push_back(0.5);
    Y.push_back(hists[0]->GetBinContent(i+1));
    Yerr.push_back(hists[0]->GetBinError(i+1));
  }

  TGraphErrors* gr = new TGraphErrors(Nb, &X[0], &Y[0],  &Xerr[0], &Yerr[0]);
  gr->SetMarkerSize(0);
  gr->SetLineColor(kBlack);
  gr->SetFillColor(kBlack);
  gr->SetFillStyle(3244);
  gr->Draw("same p2");

  for(int i = 0; i < Nsig; i++){
    shists[i]->SetLineColor(7030+i*10);
    shists[i]->SetLineWidth(5);
    shists[i]->SetFillColor(kWhite);
    shists[i]->Draw("SAME HIST");
  }

  TLegend* leg = new TLegend(1.-hhi+0.01, 1.- (Nbkg+Nsig+1)*(1.-0.49)/9., 0.98, 1.-hto-0.005);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->AddEntry(gr, "total uncertainty","F");
  for(int i = 0; i < Nbkg; i++)
    leg->AddEntry(hists[i], labels[i].c_str(), "F");
  for(int i = 0; i < Nsig; i++)
    leg->AddEntry(shists[i], (slabels[i]+" "+std::to_string(smass[i].first)+" "+std::to_string(smass[i].second)).c_str(), "L");
  leg->Draw("SAME");

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  double eps = 0.0015;
  l.SetTextSize(0.025);
  l.SetTextFont(42);
  l.SetTextAlign(20);
  line->SetLineWidth(1);
  for(int y = 0; y < Ny; y++){
    line->SetLineStyle(1);
    line->DrawLineNDC(hlo + eps + (1.-hhi-hlo)/Ny*y, hbo-(2.2*max_len+3.5)*0.021,
		      hlo + eps + (1.-hhi-hlo)/Ny*y, hbo-(2.2*max_len+3.5)*0.021 + 6*eps);
    line->DrawLineNDC(hlo - eps + (1.-hhi-hlo)/Ny*(y+1), hbo-(2.2*max_len+3.5)*0.021,
		      hlo - eps + (1.-hhi-hlo)/Ny*(y+1), hbo-(2.2*max_len+3.5)*0.021 + 6*eps);
    line->DrawLineNDC(hlo + eps + (1.-hhi-hlo)/Ny*y,     hbo-(2.2*max_len+3.5)*0.021,
		      hlo - eps + (1.-hhi-hlo)/Ny*(y+1), hbo-(2.2*max_len+3.5)*0.021);
    line->SetLineStyle(5);
    line->DrawLineNDC(hlo + (1.-hhi-hlo)/Ny*(y+1), hbo,
		      hlo + (1.-hhi-hlo)/Ny*(y+1), 1.-hto);
    line->SetLineStyle(3);
    for(int x = 1; x < Nx; x++)
      if(x%2 == 1)
	line->DrawLineNDC(hlo + (1.-hhi-hlo)/Ny*y + (1.-hhi-hlo)/Ny/Nx*(x+0.5), hbo,
			  hlo + (1.-hhi-hlo)/Ny*y + (1.-hhi-hlo)/Ny/Nx*(x+0.5),
			  hbo-(2.2*max_len+3.5)*0.021/2.-0.005);
    
    l.DrawLatex(hlo + (1.-hhi-hlo)*(0.5+y)/double(Ny),
		hbo-(2.2*max_len+3.5)*0.021-0.03, y_labels[y].c_str());

  }
  l.SetTextAlign(30);
  l.SetTextSize(0.03);
  l.SetTextFont(42);
  l.DrawLatex(hlo, hbo-(2.2*max_len+3.5)*0.021/2.-0.015, "M_{#perp}   [GeV] #in");

  l.SetTextSize(0.03);
  l.SetTextFont(42);
  l.DrawLatex(hlo, hbo-(2.2*max_len+3.5)*0.021-0.036, "#scale[1.15]{R_{ISR}} #in");

  l.SetTextAlign(30);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(1.-hhi-eps*4, 1.-hto+0.02, "2017 MC KUEWKino");
  l.SetTextAlign(10);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(hlo+eps*4, 1.-hto+0.02,"#bf{#it{CMS}} #it{Simulation}");
  l.SetTextSize(0.05);

  l.SetTextColor(7014);
  l.SetTextAlign(22);
  l.SetTextSize(0.038);
  l.SetTextFont(42);
  l.DrawLatex(1-hhi/2., hbo+0.11, lep_labels[0].c_str());

  l.SetTextAlign(22);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.SetTextColor(kBlack);
  l.DrawLatex(1-hhi/2., hbo+0.06, "+");
  l.DrawLatex(1-hhi/2., hbo-0.06, "+");
  l.DrawLatex(1-hhi/2., hbo-0.18, "+");
  l.SetTextColor(7004);
  l.DrawLatex(1-hhi/2., hbo+0.01, hadS_labels[0].c_str());
 
  l.SetTextColor(7024);
  l.SetTextAlign(22);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(1-hhi/2., hbo-0.11, hadI_labels[0].c_str());

  l.SetTextColor(kBlack);
  l.SetTextAlign(22);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(1-hhi/2., hbo-0.24, "p_{T}^{ISR} > 200 GeV");

  return can;
}


bool FitReader::Match(const string& target, const vector<string>& test){
  int N = test.size();
  for(int i = 0; i < N; i++)
    if(target.find(test[i]) != std::string::npos)
      return true;
  return false;
}

void FitReader::InitializeRecipes(){
  // Processes
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
  m_Color["QCD"] = 7033;

  m_Title["Fakes"] = "fake leptons";
  m_Color["Fakes"] = 7021;
  m_Strings["Fakes"] = SL().a("Fakes_elf0").a("Fakes_elf1").a("Fakes_elf2").a("Fakes_muf0").a("Fakes_muf1").a("Fakes_muf2");
  
  m_Title["QFlip"] = "q-flip";
  m_Color["QFlip"] = 7020;
  m_Strings["QFlip"] = SL().a("Fakes_elf0").a("Fakes_muf0");

  m_Title["HF"] = "heavy flavor fakes";
  m_Color["HF"] = 7022;
  m_Strings["HF"] = SL().a("Fakes_elf1").a("Fakes_muf1");

  m_Title["LF"] = "light flavor fakes";
  m_Color["LF"] = 7021;
  m_Strings["LF"] = SL().a("Fakes_elf2").a("Fakes_muf2");

  m_Title["Total"] = "total background";
  m_Color["Total"] = 7000;
  m_Strings["Total"] = SL().a("ttbar").a("ST").a("DB").a("ZDY").a("Wjets").a("Fakes_elf0").a("Fakes_elf1").
                                       a("Fakes_elf2").a("Fakes_muf0").a("Fakes_muf1").a("Fakes_muf2");

  m_Sig.push_back("T2bW");
  m_SignalTitle["T2bW"] = "T2bW";
  
  // leptonic categories
  m_Title["1L"] = "single #it{l}";
  m_Strings["1L"] = SL().a("1L-elp-el0").a("1L-elm-el0").a("1L-mup-mu0").a("1L-mum-mu0");

  m_Title["1Lel"] = "single e";
  m_Strings["1Lel"] = SL().a("1L-elp-el0").a("1L-elm-el0");

  m_Title["1Lmu"] = "single #mu";
  m_Strings["1Lmu"] = SL().a("1L-mup-mu0").a("1L-mum-mu0");

  m_Title["1Lelp"] = "single e^{+}";
  m_Strings["1Lelp"] = SL().a("1L-elp-el0");

  m_Title["1Lelm"] = "single e^{-}";
  m_Strings["1Lelm"] = SL().a("1L-elm-el0");

  m_Title["1Lmup"] = "single #mu^{+}";
  m_Strings["1Lmup"] = SL().a("1L-mup-mu0");

  m_Title["1Lmum"] = "single #mu^{-}";
  m_Strings["1Lmum"] = SL().a("1L-mum-mu0");

  m_Title["1Lp"] = "single #it{l}^{+}";
  m_Strings["1Lp"] = SL().a("1L-elp-el0").a("1L-mup-mu0");

  m_Title["1Lm"] = "single #it{l}^{-}";
  m_Strings["1Lm"] = SL().a("1L-elm-el0").a("1L-mum-mu0");

  m_Title["1Lsilver"] = "single silver #it{l}";
  m_Strings["1Lsilver"] = SL().a("1L-elp-el1").a("1L-elm-el1").a("1L-mup-mu1").a("1L-mum-mu1");
  
  m_Title["1Lelsilver"] = "single silver e";
  m_Strings["1Lelsilver"] = SL().a("1L-elp-el1").a("1L-elm-el1");
  
  m_Title["1Lmusilver"] = "single silver #mu";
  m_Strings["1Lmusilver"] = SL().a("1L-mup-mu1").a("1L-mum-mu1");

  m_Title["1Lbronze"] = "single bronze #it{l}";
  m_Strings["1Lbronze"] = SL().a("1L-elp-el2").a("1L-elm-el2").a("1L-mup-mu2").a("1L-mum-mu2");
  
  m_Title["1Lelbronze"] = "single bronze e";
  m_Strings["1Lelbronze"] = SL().a("1L-elp-el2").a("1L-elm-el2");
  
  m_Title["1Lmubronze"] = "single bronze #mu";
  m_Strings["1Lmubonze"] = SL().a("1L-mup-mu2").a("1L-mum-mu2");

  m_Title["2LOSSF"] = "e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}";
  m_Strings["2LOSSF"] = SL().a("OS-el^el-el0el0").a("OS-mu^mu-mu0mu0").a("OS-elel^0-el0el0").a("OS-mumu^0-mu0mu0");
  
  m_Title["2LOSOF"] = "e^{#pm} #mu^{#mp}";
  m_Strings["2LOSOF"] = SL().a("OS-el^mu-el0mu0").a("OS-elmu^0-el0mu0");

  m_Title["2LSSSF"] = "e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}";
  m_Strings["2LSSSF"] = SL().a("SS-el^el-el0el0").a("SS-mu^mu-mu0mu0").a("SS-elel^0-el0el0").a("SS-mumu^0-mu0mu0");
  
  m_Title["2LSSOF"] = "e^{#pm} #mu^{#pm}";
  m_Strings["2LSSOF"] = SL().a("SS-el^mu-el0mu0").a("SS-elmu^0-el0mu0");

  m_Title["2LOSSFsilver"] = "e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}, #geq 1 silver #it{l}";
  m_Strings["2LOSSFsilver"] = SL().a("OS-el^el-el0el1").a("OS-mu^mu-mu0mu1").a("OS-elel^0-el0el1").a("OS-mumu^0-mu0mu1")
                                  .a("OS-el^el-el1el1").a("OS-mu^mu-mu1mu1").a("OS-elel^0-el1el1").a("OS-mumu^0-mu1mu1")
                                  .a("OS-el^el-el1el2").a("OS-mu^mu-mu1mu2").a("OS-elel^0-el1el2").a("OS-mumu^0-mu1mu2");
  
  m_Title["2LOSOFsilver"] = "e^{#pm} #mu^{#mp}, #geq 1 silver #it{l}";
  m_Strings["2LOSOFsilver"] = SL().a("OS-el^mu-el0mu1").a("OS-elmu^0-el0mu1").a("OS-el^mu-mu0el1").a("OS-elmu^0-mu0el1")
                                  .a("OS-el^mu-el1mu1").a("OS-elmu^0-el1mu1").a("OS-el^mu-mu1el2").a("OS-elmu^0-mu1el2")
                                  .a("OS-el^mu-el1mu2").a("OS-elmu^1-el1mu2");

  m_Title["2LSSSFsilver"] = "e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}, #geq 1 silver #it{l}";
  m_Strings["2LSSSFsilver"] = SL().a("SS-el^el-el0el1").a("SS-mu^mu-mu0mu1").a("SS-elel^0-el0el1").a("SS-mumu^0-mu0mu1")
                                  .a("SS-el^el-el1el1").a("SS-mu^mu-mu1mu1").a("SS-elel^0-el1el1").a("SS-mumu^0-mu1mu1")
                                  .a("SS-el^el-el1el2").a("SS-mu^mu-mu1mu2").a("SS-elel^0-el1el2").a("SS-mumu^0-mu1mu2");
  
  m_Title["2LSSOFsilver"] = "e^{#pm} #mu^{#pm}, #geq 1 silver #it{l}";
  m_Strings["2LSSOFsilver"] = SL().a("SS-el^mu-el0mu1").a("SS-elmu^0-el0mu1").a("SS-el^mu-mu0el1").a("SS-elmu^0-mu0el1")
                                  .a("SS-el^mu-el1mu1").a("SS-elmu^0-el1mu1").a("SS-el^mu-mu1el2").a("SS-elmu^0-mu1el2")
                                  .a("SS-el^mu-el1mu2").a("SS-elmu^1-el1mu2");

  m_Title["2LOSSFbronze"] = "e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}, #geq 1 bronze #it{l}";
  m_Strings["2LOSSFbronze"] = SL().a("OS-el^el-el0el2").a("OS-mu^mu-mu0mu2").a("OS-elel^0-el0el2").a("OS-mumu^0-mu0mu2")
                                  .a("OS-el^el-el1el2").a("OS-mu^mu-mu1mu2").a("OS-elel^0-el1el2").a("OS-mumu^0-mu1mu2")
                                  .a("OS-el^el-el2el2").a("OS-mu^mu-mu2mu2").a("OS-elel^0-el2el2").a("OS-mumu^0-mu2mu2");
  
  m_Title["2LOSOFbronze"] = "e^{#pm} #mu^{#mp}, #geq 1 bronze #it{l}";
  m_Strings["2LOSOFbronze"] = SL().a("OS-el^mu-el0mu2").a("OS-elmu^0-el0mu2").a("OS-el^mu-mu0el2").a("OS-elmu^0-mu0el2")
                                  .a("OS-el^mu-el1mu2").a("OS-elmu^0-el1mu2").a("OS-el^mu-mu1el2").a("OS-elmu^0-mu1el2")
                                  .a("OS-el^mu-el2mu2").a("OS-elmu^1-el2mu2");

  m_Title["2LSSSFbronze"] = "e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}, #geq 1 bronze #it{l}";
  m_Strings["2LSSSFbronze"] = SL().a("SS-el^el-el0el2").a("SS-mu^mu-mu0mu2").a("SS-elel^0-el0el2").a("SS-mumu^0-mu0mu2")
                                  .a("SS-el^el-el1el2").a("SS-mu^mu-mu1mu2").a("SS-elel^0-el1el2").a("SS-mumu^0-mu1mu2")
                                  .a("SS-el^el-el2el2").a("SS-mu^mu-mu2mu2").a("SS-elel^0-el2el2").a("SS-mumu^0-mu2mu2");
  
  m_Title["2LSSOFbronze"] = "e^{#pm} #mu^{#pm}, #geq 1 bronze #it{l}";
  m_Strings["2LSSOFbronze"] = SL().a("SS-el^mu-el0mu2").a("SS-elmu^0-el0mu2").a("SS-el^mu-mu0el2").a("SS-elmu^0-mu0el2")
                                  .a("SS-el^mu-el1mu2").a("SS-elmu^0-el1mu2").a("SS-el^mu-mu1el2").a("SS-elmu^0-mu1el2")
                                  .a("SS-el^mu-el2mu2").a("SS-elmu^1-el2mu2");

  // hadronic categories
  m_Title["0jsvS"] = "#splitline{0 jets}{0 SV-tags} #scale[1.2]{#in S}";

  m_Title["0j1svS"] = "#splitline{0 jets}{1 SV-tag} #scale[1.2]{#in S}";

  m_Title["0jge2svS"] = "#splitline{0 jets}{#geq 2 SV-tags} #scale[1.2]{#in S}";

  m_Title["0bjetISR"] = "#splitline{#geq 1 jet}{0 b-tags} #scale[1.2]{#in ISR}";

  m_Title["ge1bjetISR"] = "#splitline{#geq 1 jet}{#geq 1 b-tags} #scale[1.2]{#in ISR}";

  m_Title["0bjetS"] = "#splitline{#geq 1 jet}{0 b-tags} #scale[1.2]{#in S}";

  m_Title["1bjetS"] = "#splitline{#geq 1 jet}{1 b-tag} #scale[1.2]{#in S}";

  m_Title["ge2bjetS"] = "#splitline{#geq 1 jet}{#geq 2 b-tags} #scale[1.2]{#in S}";


  m_ColorDefault.push_back(kBlue+2);
  m_ColorDefault.push_back(kGreen+3);
  m_ColorDefault.push_back(kRed+1);
  m_ColorDefault.push_back(kYellow+2);
  m_ColorDefault.push_back(kMagenta+1);
  m_ColorDefault.push_back(kMagenta+2);
  m_ColorDefault.push_back(kCyan+2);
  m_ColorDefault.push_back(kCyan+3);
  
}
