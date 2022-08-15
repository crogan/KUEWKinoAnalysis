#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraph2D.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLine.h>
#include <TLatex.h>
#include <TH2D.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string>
#include "TSystem.h"

#include "RestFrames/RestFrames.hh"

#include "../include/XsecTool.hh"

//gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
using namespace std;
using namespace RestFrames;

XsecTool g_Xsec;

double popdouble(std::string& line);
std::string popstring(std::string& line);

enum LimitType { kObs, kExp, kExpUp, kExpDn };
enum PlotType { kTChiWZ, kT2tt, kT2bW, kT2bb, kTSlSl, kT2cc};

TCanvas* Plot2DHist_MCvMP(const string& can, TH2D* hist, PlotType ptype);
TCanvas* Plot2DHist_dMvMP(const string& can, TH2D* hist, PlotType ptype);

void Invert2DHist(TH2D* hist);

map<int,int> MP;
map<int,int> MC;
int min_MP = -1;
int max_MP = -1;
int min_MC = -1;
int max_MC = -1;
int min_dM = -1;
int max_dM = -1;

class Limit {
public:
  Limit(const string& json){
    
    m_min_MP = -1;
    m_max_MP = -1;
    m_min_MC = -1;
    m_max_MC = -1;
    m_min_dM = -1;
    m_max_dM = -1;
    
    std::ifstream ifile(json.c_str());
    if(!ifile.is_open()){
      std::cout << "can't open json file " << json << std::endl;
      return;
    }
    
    string line, name;
    size_t found, end;
    int mass;
    
    //discard first line
    getline(ifile,line);
    while(getline(ifile,line)){
      if(line.find("}") != string::npos)
	continue;
      while(line.find("\"") != string::npos){
	found = line.find("\"");
	line.replace(found,1," ");
      }
      while(line.find(":") != string::npos){
	found = line.find(":");
	line.replace(found,1," ");
      }
      while(line.find(",") != string::npos){
	found = line.find(",");
	line.replace(found,1," ");
      }
      
      if(line.find("{") != string::npos){
	mass = int(popdouble(line));
	m_MP[mass] = mass/10000;
	m_MC[mass] = mass%10000;
	
	if(m_MP[mass] > m_max_MP || m_max_MP < 0)
	  m_max_MP = m_MP[mass];
	if(m_MP[mass] < m_min_MP || m_min_MP < 0)
	  min_MP = MP[mass];
	if(m_MC[mass] > m_max_MC || m_max_MC < 0)
	  m_max_MC = m_MC[mass];
	if(m_MC[mass] < m_min_MC || m_min_MC < 0)
	  m_min_MC = m_MC[mass];
	if(m_MP[mass]-m_MC[mass] > m_max_dM || m_max_dM < 0)
	  m_max_dM = m_MP[mass]-m_MC[mass];
	if(m_MP[mass]-m_MC[mass] < m_min_dM || m_min_dM < 0)
	  m_min_dM = m_MP[mass]-m_MC[mass];
      
	cout << "MP = " << m_MP[mass] << " MC = " << m_MC[mass] << endl;
	continue;
      }
      if(line.find("exp0") != string::npos){
	popstring(line);
	double r = popdouble(line);
	m_R_exp0.push_back(pair<int,double>(mass, r));
	m_iR_exp0.push_back(pair<int,double>(mass, 1./r));
      }
      if(line.find("exp+1") != string::npos){
	popstring(line);
	double r = popdouble(line);
	m_R_exp_p1.push_back(pair<int,double>(mass, r));
	m_iR_exp_p1.push_back(pair<int,double>(mass, 1./r));
      }
      if(line.find("exp-1") != string::npos){
	popstring(line);
	double r = popdouble(line);
	m_R_exp_m1.push_back(pair<int,double>(mass, r));
	m_iR_exp_m1.push_back(pair<int,double>(mass, 1./r));
      }
      if(line.find("obs") != string::npos){
	popstring(line);
	double r = popdouble(line);
	m_R_obs.push_back(pair<int,double>(mass, r));
	m_iR_obs.push_back(pair<int,double>(mass, 1./r));
      }
    } 
  }
  
  virtual ~Limit(){ }

  TGraph* Get2DContour_MCvMP(LimitType type = kExp){
    TH2D* hist = Get2DHist_MCvMP("temp_2D", type);

    int Nx = hist->GetNbinsX();
    int Ny = hist->GetNbinsY();

    vector<double> X;
    vector<double> Y;

    for(int y = 0; y < Ny; y++){
      bool b_zero = false;
      bool b_on = false;
      for(int x = 0; x < Nx; x++){
	if(hist->GetBinContent(x+1,y+1) <= 0.){
	  b_zero = true;
	  b_on = false;
	}
	if(hist->GetBinContent(x+1,y+1) > 0. && b_zero){
	  if(hist->GetBinContent(x+1,y+1) <= 1.){
	    X.push_back(hist->GetXaxis()->GetBinCenter(x+1));
	    Y.push_back(hist->GetYaxis()->GetBinCenter(y+1));
	    break;
	  } else {
	    b_zero = false;
	    b_on = true;
	  }
	}
	if(hist->GetBinContent(x+1,y+1) <= 1. && b_on){
	  X.push_back(hist->GetXaxis()->GetBinCenter(x+1));
	  Y.push_back(hist->GetYaxis()->GetBinCenter(y+1));
	  break;
	}
      }
    }
    for(int y = Ny-1; y >= 0; y--){
      bool b_on = false;
      for(int x = Nx-1; x >= 0; x--){
	if(hist->GetBinContent(x+1,y+1) > 1.)
	  b_on = true;
	if(hist->GetBinContent(x+1,y+1) <= 1. &&
	   hist->GetBinContent(x+1,y+1) > 0. && b_on){
	  X.push_back(hist->GetXaxis()->GetBinCenter(x+1));
	  Y.push_back(hist->GetYaxis()->GetBinCenter(y+1));
	  break;
	}
      }
    }
	
    delete hist;
    TGraph* gr = new TGraph(X.size(), &X[0], &Y[0]);
    return gr;
  }

  TGraph* Get2DContour_dMvMP(LimitType type = kExp){
    TH2D* hist = Get2DHist_dMvMP("temp_2D", type);

    int Nx = hist->GetNbinsX();
    int Ny = hist->GetNbinsY();

    vector<double> X;
    vector<double> Y;

    for(int y = 0; y < Ny; y++){
      bool b_zero = false;
      bool b_on = false;
      for(int x = 0; x < Nx; x++){
	if(hist->GetBinContent(x+1,y+1) <= 0.){
	  b_zero = true;
	  b_on = false;
	}
	if(hist->GetBinContent(x+1,y+1) > 0. && b_zero){
	  if(hist->GetBinContent(x+1,y+1) <= 1.){
	    X.push_back(hist->GetXaxis()->GetBinCenter(x+1));
	    Y.push_back(hist->GetYaxis()->GetBinCenter(y+1));
	    break;
	  } else {
	    b_zero = false;
	    b_on = true;
	  }
	}
	if(hist->GetBinContent(x+1,y+1) <= 1. && b_on){
	  X.push_back(hist->GetXaxis()->GetBinCenter(x+1));
	  Y.push_back(hist->GetYaxis()->GetBinCenter(y+1));
	  break;
	}
      }
    }
    for(int y = Ny-1; y >= 0; y--){
      bool b_on = false;
      for(int x = Nx-1; x >= 0; x--){
	if(hist->GetBinContent(x+1,y+1) > 1.)
	  b_on = true;
	if(hist->GetBinContent(x+1,y+1) <= 1. &&
	   hist->GetBinContent(x+1,y+1) > 0. && b_on){
	  X.push_back(hist->GetXaxis()->GetBinCenter(x+1));
	  Y.push_back(hist->GetYaxis()->GetBinCenter(y+1));
	  break;
	}
      }
    }
	
    delete hist;
    TGraph* gr = new TGraph(X.size(), &X[0], &Y[0]);
    return gr;
  }
  
  TH2D* Get2DHist_MCvMP(const string& name, LimitType type = kExp){
	bool b_inv = true;
    
    vector<pair<int,double> > vec;

    if(type == kObs)
      vec = m_R_obs;
    if(type == kExp)
      vec = m_R_exp0;
    if(type == kExpUp)
      vec = m_R_exp_p1;
    if(type == kExpDn)
      vec = m_R_exp_m1;

    int N = vec.size();
    TGraph2D* gr = new TGraph2D();
    for(int i = 0; i < N; i++){
      if(b_inv && vec[i].second > 0)
	gr->SetPoint(i, m_MP[vec[i].first], m_MC[vec[i].first], 1./vec[i].second);
      else
	gr->SetPoint(i, m_MP[vec[i].first], m_MC[vec[i].first], vec[i].second);
	}
    int Nx = m_max_MP+1 - m_min_MP;
    int Ny = m_max_MP+1 - m_min_MC;
    double xmin = m_min_MP+1;
    double xmax = m_max_MP+1;
    double ymin = m_min_MC+1;
    double ymax = m_max_MC+1;
    TH2D* hist = new TH2D(name.c_str(),name.c_str(),
			  Nx, xmin, xmax,
			  Ny, ymin, ymax);
  
    for(int x = 0; x < Nx; x++){
      double mp = hist->GetXaxis()->GetBinCenter(x+1);
      for(int y = 0; y < Ny; y++){
	double mc = hist->GetYaxis()->GetBinCenter(y+1);
	bool doBreak = mp-mc < m_min_dM;
	if(mp-mc < m_min_dM)
	  break;
	hist->SetBinContent(x+1,y+1,gr->Interpolate(mp,mc));
      }
    }
  
    // fill in missing entries
    for(int x = 0; x < Nx; x++){
      bool filled = false;
      for(int y = Ny-1; y >= 0; y--){
	if(hist->GetBinContent(x+1,y+1) > 0)
	  filled = true;
      
	if(hist->GetBinContent(x+1,y+1) <= 0 && filled){
	  for(int xx = x+1; xx < Nx; xx++){
	    if(hist->GetBinContent(xx+1,y+1) > 0){
	      hist->SetBinContent(x+1,y+1, hist->GetBinContent(xx+1,y+1));
	      break;
	    }
	  }
	}
      }
    }
    for(int y = 0; y < Ny; y++){
      bool filled = false;
      for(int x = 0; x < Nx; x++){
	if(hist->GetBinContent(x+1,y+1) > 0)
	  filled = true;
      
	if(hist->GetBinContent(x+1,y+1) <= 0 && filled){
	  for(int yy = y+1; yy < Ny; yy++){
	    if(hist->GetBinContent(x+1,yy+1) > 0){
	      hist->SetBinContent(x+1,y+1, hist->GetBinContent(x+1,yy+1));
	      break;
	    }
	  }
	}
      }
    }

    if(b_inv)
      Invert2DHist(hist);
    
    delete gr;
    return hist;
  }

  TH2D* Get2DHist_dMvMP(const string& name, LimitType type = kExp){
    bool b_inv = true;
    
    vector<pair<int,double> > vec;

    if(type == kObs)
      vec = m_R_obs;
    if(type == kExp)
      vec = m_R_exp0;
    if(type == kExpUp)
      vec = m_R_exp_p1;
    if(type == kExpDn)
      vec = m_R_exp_m1;

    int N = vec.size();
    TGraph2D* gr = new TGraph2D();
    
    for(int i = 0; i < N; i++){
      if(b_inv && vec[i].second > 0)
	gr->SetPoint(i, m_MP[vec[i].first], m_MP[vec[i].first]-m_MC[vec[i].first], 1./vec[i].second);
      else
	gr->SetPoint(i, m_MP[vec[i].first], m_MP[vec[i].first]-m_MC[vec[i].first], vec[i].second);
    }

    int Nx = m_max_MP+1 - m_min_MP;
    int Ny = m_max_dM+1 - m_min_dM;
    double xmin = m_min_MP+1;
    double xmax = m_max_MP+1;
    double ymin = m_min_dM+1;
    double ymax = m_max_dM+1;
    
    TH2D* hist = new TH2D(name.c_str(),name.c_str(),
			  Nx, xmin, xmax,
			  Ny, ymin, ymax);

    for(int x = 0; x < Nx; x++){
      double mp = hist->GetXaxis()->GetBinCenter(x+1);
      for(int y = 0; y < Ny; y++){
	double dm = hist->GetYaxis()->GetBinCenter(y+1);
	hist->SetBinContent(x+1,y+1,gr->Interpolate(mp,dm));
      }
    }
  
    // fill in missing entries
    for(int x = 0; x < Nx; x++){
      bool filled = false;
      for(int y = Ny-1; y >= 0; y--){
	if(hist->GetBinContent(x+1,y+1) > 0)
	  filled = true;
      
	if(hist->GetBinContent(x+1,y+1) <= 0 && filled){
	  for(int xx = x+1; xx < Nx; xx++){
	    if(hist->GetBinContent(xx+1,y+1) > 0){
	      hist->SetBinContent(x+1,y+1, hist->GetBinContent(xx+1,y+1));
	      break;
	    }
	  }
	}
      }
    }
    for(int y = 0; y < Ny; y++){
      bool filled = false;
      for(int x = 0; x < Nx; x++){
	if(hist->GetBinContent(x+1,y+1) > 0)
	  filled = true;
      
	if(hist->GetBinContent(x+1,y+1) <= 0 && filled){
	  for(int yy = y+1; yy < Ny; yy++){
	    if(hist->GetBinContent(x+1,yy+1) > 0){
	      hist->SetBinContent(x+1,y+1, hist->GetBinContent(x+1,yy+1));
	      break;
	    }
	  }
	}
      }
    }

    if(b_inv)
      Invert2DHist(hist);

    delete gr;
    return hist;
  }

private:
  map<int,int> m_MP;
  map<int,int> m_MC;
  int m_min_MP;
  int m_max_MP;
  int m_min_MC;
  int m_max_MC;
  int m_min_dM;
  int m_max_dM;

  vector<pair<int,double> > m_R_exp0;
  vector<pair<int,double> > m_R_exp_p1;
  vector<pair<int,double> > m_R_exp_m1;
  vector<pair<int,double> > m_R_obs;

  vector<pair<int,double> > m_iR_exp0;
  vector<pair<int,double> > m_iR_exp_p1;
  vector<pair<int,double> > m_iR_exp_m1;
  vector<pair<int,double> > m_iR_obs;
  
};



void myParseLimitJSON(const string& json, bool inclObs = false, PlotType ptype = kT2tt, const string& outfile="out.root"){
  RestFrames::SetStyle();
  
  Limit* limit_def = new Limit(json);
  if(limit_def == NULL) return;
  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  TLine* line = new TLine();

  /////////////
  // MC vs. MP
  /////////////
  TH2D*   hist_exp_MC = limit_def->Get2DHist_MCvMP("h_exp_MC", kExp);
  TGraph* gr_exp_MC   = limit_def->Get2DContour_MCvMP(kExp);
  TGraph* gr_exp_MC_up   = limit_def->Get2DContour_MCvMP(kExpUp);
  TGraph* gr_exp_MC_dn   = limit_def->Get2DContour_MCvMP(kExpDn);
  TGraph* gr_exp_MC_obs   = limit_def->Get2DContour_MCvMP(kObs);
  
  TCanvas* can_MC = Plot2DHist_MCvMP("can_MC", hist_exp_MC, ptype);
  can_MC->cd();
  gr_exp_MC->SetLineColor(7043);
  gr_exp_MC->SetLineWidth(5);
  gr_exp_MC->SetLineStyle(1);
  gr_exp_MC->Draw("same C");

  gr_exp_MC_up->SetMarkerColor(kWhite);
  gr_exp_MC_up->SetLineColor(7043);
  gr_exp_MC_up->SetLineWidth(4);
  gr_exp_MC_up->SetLineStyle(7);
  gr_exp_MC_up->Draw("same C");
  gr_exp_MC_dn->SetMarkerColor(kWhite);
  gr_exp_MC_dn->SetLineColor(7043);
  gr_exp_MC_dn->SetLineWidth(4);
  gr_exp_MC_dn->SetLineStyle(7);
  gr_exp_MC_dn->Draw("same C");

  gr_exp_MC_obs->SetMarkerColor(kWhite);
  gr_exp_MC_obs->SetLineColor(kBlack);
  gr_exp_MC_obs->SetLineWidth(4);
  gr_exp_MC_obs->SetLineStyle(1);
 if(inclObs) gr_exp_MC_obs->Draw("same C");

  l.SetTextAlign(12);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(0.23, 0.83,"expected #pm 1 #sigma_{expm}");
  line->SetLineColor(7043);
  line->SetLineWidth(2);
  line->SetLineStyle(1);
  line->DrawLineNDC(0.18, 0.83, 0.22, 0.83);
  line->SetLineStyle(3);
  line->DrawLineNDC(0.18, 0.842, 0.22, 0.842);
  line->DrawLineNDC(0.18, 0.818, 0.22, 0.818);
		    
  l.DrawLatex(0.23, 0.78,"observed");
  line->SetLineColor(kBlack);
  line->SetLineWidth(2);
  line->SetLineStyle(1);
  line->DrawLineNDC(0.18, 0.78, 0.22, 0.78);

  /////////////
  // dM vs. MP
  /////////////
  TH2D*   hist_exp_dM = limit_def->Get2DHist_dMvMP("h_exp_dM", kExp);
  TGraph* gr_exp_dM   = limit_def->Get2DContour_dMvMP(kExp);
  TGraph* gr_exp_dM_up   = limit_def->Get2DContour_dMvMP(kExpUp);
  TGraph* gr_exp_dM_dn   = limit_def->Get2DContour_dMvMP(kExpDn);
  TGraph* gr_exp_dM_obs   = limit_def->Get2DContour_dMvMP(kObs);
  
  TCanvas* can_dM = Plot2DHist_dMvMP("can_dM", hist_exp_dM, ptype);
  can_dM->cd();
  
  gr_exp_dM->SetLineColor(7043);
  gr_exp_dM->SetLineWidth(5);
  gr_exp_dM->SetLineStyle(1);
  gr_exp_dM->Draw("same C");

  gr_exp_dM_up->SetMarkerColor(kWhite);
  gr_exp_dM_up->SetLineColor(7043);
  gr_exp_dM_up->SetLineWidth(4);
  gr_exp_dM_up->SetLineStyle(7);
  gr_exp_dM_up->Draw("same C");
  gr_exp_dM_dn->SetMarkerColor(kWhite);
  gr_exp_dM_dn->SetLineColor(7043);
  gr_exp_dM_dn->SetLineWidth(4);
  gr_exp_dM_dn->SetLineStyle(7);
  gr_exp_dM_dn->Draw("same C");
  gr_exp_dM_obs->SetMarkerColor(kWhite);
  gr_exp_dM_obs->SetLineColor(kBlack);
  gr_exp_dM_obs->SetLineWidth(4);
  gr_exp_dM_obs->SetLineStyle(1);
  if(inclObs) gr_exp_dM_obs->Draw("same C");

  l.SetTextAlign(12);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(0.23, 0.83,"expected #pm 1 #sigma_{expm}");
  line->SetLineColor(7043);
  line->SetLineWidth(2);
  line->SetLineStyle(1);
  line->DrawLineNDC(0.18, 0.83, 0.22, 0.83);
  line->SetLineStyle(3);
  line->DrawLineNDC(0.18, 0.842, 0.22, 0.842);
  line->DrawLineNDC(0.18, 0.818, 0.22, 0.818);
  
  l.DrawLatex(0.23, 0.78,"observed");
  line->SetLineColor(kBlack);
  line->SetLineWidth(2);
  line->SetLineStyle(1);
  line->DrawLineNDC(0.18, 0.78, 0.22, 0.78);
 
  TFile* out = new TFile(outfile.c_str(),"RECREATE");
  out->WriteTObject(can_MC);
  out->WriteTObject(can_dM);
  out->Close();
 
}




double popdouble(std::string& line){
  // remove leading whitespace
  while(line[0] == string(" ")[0])
    line.erase(0,1);
  
  string num;
  if(line.find(" ") != string::npos){
    size_t p = line.find(" ");
    num = line.substr(0,p);
    line.erase(0,p+1);
    while(line[0] == string(" ")[0])
      line.erase(0,1);
  } else
    num = line;

  return stod(num);
}

std::string popstring(std::string& line){
  // remove leading whitespace
  while(line[0] == string(" ")[0])
    line.erase(0,1);
  
  string ret;
  if(line.find(" ") != string::npos){
    size_t p = line.find(" ");
    ret = line.substr(0,p);
    line.erase(0,p+1);
    while(line[0] == string(" ")[0])
      line.erase(0,1);
  } else
    ret = line;

  return ret;
}

void Invert2DHist(TH2D* hist){
  int Nx = hist->GetNbinsX();
  int Ny = hist->GetNbinsY();

  for(int x = 0; x < Nx; x++){
    for(int y = 0; y < Ny; y++){
      if(hist->GetBinContent(x+1,y+1) > 0.)
	hist->SetBinContent(x+1,y+1, 1./hist->GetBinContent(x+1,y+1));
    }
  }
}

TCanvas* Plot2DHist_MCvMP(const string& name, TH2D* hist, PlotType ptype){
//gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
  TCanvas* can = (TCanvas*) new TCanvas(name.c_str(),name.c_str(),700.,600);

  string xlabel = "m_{P} [GeV]";
  string ylabel = "m_{#tilde{#chi}^{0}_{1}} [GeV]";

  if(ptype == kTChiWZ){
    xlabel = "m_{#tilde{#chi}^{#pm}_{1}} [GeV]";
    ylabel = "m_{#tilde{#chi}^{0}_{1}} [GeV]";
  }
  if(ptype == kT2tt || ptype == kT2bW){
    xlabel = "m_{ #tilde{t}} [GeV]";
    ylabel = "m_{#tilde{#chi}^{0}_{1}} [GeV]";
  }
  if(ptype == kT2bb){
    xlabel = "m_{ #tilde{b}} [GeV]";
    ylabel = "m_{#tilde{#chi}^{0}_{1}} [GeV]";
  }
  if(ptype == kT2cc){
    xlabel = "m_{ #tilde{t}} [GeV]";
    ylabel = "m_{#tilde{#chi}^{0}_{1}} [GeV]";
  } 
  if(ptype == kTSlSl){
    xlabel = "m_{ #tilde{#it{l}}} [GeV]";
    ylabel = "m_{#tilde{#chi}^{0}_{1}} [GeV]";
 }
                  
  can->SetLeftMargin(0.15);
  can->SetRightMargin(0.2);
  can->SetBottomMargin(0.15);
  can->SetTopMargin(0.07);
  //can->SetGridx();
  //can->SetGridy();
  can->SetLogz();
  can->Draw();
  can->cd();

  // scale r limits to xsec limits
  int Nx = hist->GetNbinsX();
  int Ny = hist->GetNbinsY();

  for(int x = 0; x < Nx; x++){
    double MP = hist->GetXaxis()->GetBinCenter(x+1);
    double xsec = 1.;
    if(ptype == kTChiWZ)
      xsec = g_Xsec.GetXsec_SMS("TChiWZ", MP);
    if(ptype == kT2tt)
      xsec = g_Xsec.GetXsec_SMS("T2tt", MP);
    if(ptype == kT2bW)
      xsec = g_Xsec.GetXsec_SMS("T2bW", MP);
    if(ptype == kT2bb)
      xsec = g_Xsec.GetXsec_SMS("T2bb", MP);
    if(ptype == kTSlSl)
      xsec = g_Xsec.GetXsec_SMS("TSlepSlep", MP);
    if(ptype == kT2cc)
      xsec = g_Xsec.GetXsec_SMS("T2cc", MP);   
 
    for(int y = 0; y < Ny; y++){
      if(hist->GetBinContent(x+1,y+1) > 0.){
	hist->SetBinContent(x+1,y+1, hist->GetBinContent(x+1,y+1)*xsec);
      }
    }
  }
  
  hist->GetXaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleSize(0.05);
  hist->GetXaxis()->SetTitleOffset(1.08);
  hist->GetXaxis()->SetLabelFont(42);
  hist->GetXaxis()->SetLabelSize(0.045);
  hist->GetXaxis()->SetTitle(xlabel.c_str());
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetYaxis()->SetTitleSize(0.05);
  hist->GetYaxis()->SetTitleOffset(1.12);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelSize(0.045);
  hist->GetYaxis()->SetTitle(ylabel.c_str());
  hist->GetZaxis()->CenterTitle();
  hist->GetZaxis()->SetTitleFont(42);
  hist->GetZaxis()->SetTitleSize(0.05);
  hist->GetZaxis()->SetTitleOffset(1.15);
  hist->GetZaxis()->SetLabelFont(42);
  hist->GetZaxis()->SetLabelSize(0.045);
  hist->GetZaxis()->SetTitle("95% C.L. cross-section U.L. [fb]");
  hist->Draw("COLZ");

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.8, 0.95, "137 fb^{-1} (13 TeV)");
  l.SetTextAlign(11);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.16, 0.95,"#bf{#it{CMS}} work-in-progress");
  l.SetTextSize(0.05);

  // SMS info
  string SMS;
  if(ptype == kTChiWZ){
    SMS =  "pp #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}; ";
    SMS += "#tilde{#chi}_{2}^{0} #rightarrow Z*#tilde{#chi}_{1}^{0}, ";
    SMS += "#tilde{#chi}_{1}^{#pm} #rightarrow W*#tilde{#chi}_{1}^{0}";
  }
  if(ptype == kT2tt){
    SMS =  "pp #rightarrow #tilde{t} #tilde{t}; ";
    SMS += "#tilde{t} #rightarrow t #tilde{#chi}_{1}^{0}";
  }
  if(ptype == kT2bW){
    SMS =  "pp #rightarrow #tilde{t} #tilde{t}; ";
    SMS += "#tilde{t} #rightarrow b #tilde{#chi}_{1}^{#pm}(W #tilde{#chi}_{1}^{0})";
  }
  if(ptype == kT2bb){
    SMS =  "pp #rightarrow #tilde{b} #tilde{b}; ";
    SMS += "#tilde{b} #rightarrow b #tilde{#chi}_{1}^{0}";
  }
  if(ptype == kTSlSl){
    SMS =  "pp #rightarrow #tilde{#it{l}}_{L/R} #tilde{#it{l}}_{L/R}; ";
    SMS += "#tilde{#it{l}}_{L/R} #rightarrow #it{l} #tilde{#chi}_{1}^{0}";
  }
  if(ptype == kT2cc){
    SMS = "pp #rightarrow #tilde{t} #tilde{t}; ";
    SMS += "#tilde{t} #rightarrow c #tilde{#chi}_{1}^{0}";
 }

  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(0.18, 0.87,SMS.c_str());

  return can;
}

TCanvas* Plot2DHist_dMvMP(const string& name, TH2D* hist, PlotType ptype){
//gSystem->Load("/home/t3-ku/mlazarov/Ewkinos/CMSSW_10_6_5/src/KUEWKinoAnalysis/lib/libKUEWKino.so");
  TCanvas* can = (TCanvas*) new TCanvas(name.c_str(),name.c_str(),700.,600);

  string xlabel = "m_{P} [GeV]";
  string ylabel = "m_{P} - M_{ #tilde{#chi}^{0}_{1}} [GeV]";

  if(ptype == kTChiWZ){
    xlabel = "m_{#tilde{#chi}^{#pm}_{1}} [GeV]";
    ylabel = "m_{#tilde{#chi}^{#pm}_{1}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
  }
  if(ptype == kT2tt || ptype == kT2bW){
    xlabel = "m_{ #tilde{t}} [GeV]";
    ylabel = "m_{ #tilde{t}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
  }
  if(ptype == kT2bb){
    xlabel = "m_{ #tilde{b}} [GeV]";
    ylabel = "m_{ #tilde{b}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
  }
    if(ptype == kT2cc){
    xlabel = "m_{ #tilde{t}} [GeV]";
    ylabel = "m_{#tilde{#chi}^{0}_{1}} [GeV]";
  }
  if(ptype == kTSlSl){
    xlabel = "m_{ #tilde{#it{l}}} [GeV]";
    ylabel = "m_{#tilde{#it{l}}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
 }

  
  can->SetLeftMargin(0.15);
  can->SetRightMargin(0.2);
  can->SetBottomMargin(0.15);
  can->SetTopMargin(0.07);
  can->SetGridx();
  can->SetGridy();
  can->SetLogz();
  can->Draw();
  can->cd();

  // scale r limits to xsec limits
  int Nx = hist->GetNbinsX();
  int Ny = hist->GetNbinsY();
  
  for(int x = 0; x < Nx; x++){
    double MP = hist->GetXaxis()->GetBinCenter(x+1);
    double xsec = 1.;
    if(ptype == kTChiWZ)
      xsec = g_Xsec.GetXsec_SMS("TChiWZ", MP);
    if(ptype == kT2tt)
      xsec = g_Xsec.GetXsec_SMS("T2tt", MP);
    if(ptype == kT2bW)
      xsec = g_Xsec.GetXsec_SMS("T2bW", MP);
    if(ptype == kT2bb)
      xsec = g_Xsec.GetXsec_SMS("T2bb", MP);
    if(ptype == kTSlSl)
      xsec = g_Xsec.GetXsec_SMS("TSlepSlep", MP);
    if(ptype == kT2cc)
      xsec = g_Xsec.GetXsec_SMS("T2cc", MP);
    
    for(int y = 0; y < Ny; y++){
      if(hist->GetBinContent(x+1,y+1) > 0.){
	hist->SetBinContent(x+1,y+1, hist->GetBinContent(x+1,y+1)*xsec);
      }
    }
  }
  
  hist->GetXaxis()->CenterTitle();
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleSize(0.05);
  hist->GetXaxis()->SetTitleOffset(1.08);
  hist->GetXaxis()->SetLabelFont(42);
  hist->GetXaxis()->SetLabelSize(0.045);
  hist->GetXaxis()->SetTitle(xlabel.c_str());
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetYaxis()->SetTitleSize(0.05);
  hist->GetYaxis()->SetTitleOffset(1.12);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelSize(0.045);
  hist->GetYaxis()->SetTitle(ylabel.c_str());
  hist->GetZaxis()->CenterTitle();
  hist->GetZaxis()->SetTitleFont(42);
  hist->GetZaxis()->SetTitleSize(0.05);
  hist->GetZaxis()->SetTitleOffset(1.15);
  hist->GetZaxis()->SetLabelFont(42);
  hist->GetZaxis()->SetLabelSize(0.045);
  hist->GetZaxis()->SetTitle("95% C.L. cross-section U.L. [fb]");
  hist->Draw("COLZ");

  TLatex l;
  l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);

  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.8, 0.95, "137 fb^{-1} (13 TeV)");
  l.SetTextAlign(11);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.16, 0.95,"#bf{#it{CMS}} work-in-progress");
  l.SetTextSize(0.05);

  // SMS info
  string SMS;
  if(ptype == kTChiWZ){
    SMS =  "pp #rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}; ";
    SMS += "#tilde{#chi}_{2}^{0} #rightarrow Z*#tilde{#chi}_{1}^{0}, ";
    SMS += "#tilde{#chi}_{1}^{#pm} #rightarrow W*#tilde{#chi}_{1}^{0}";
  }
  if(ptype == kT2tt){
    SMS =  "pp #rightarrow #tilde{t} #tilde{t}; ";
    SMS += "#tilde{t} #rightarrow t #tilde{#chi}_{1}^{0}";
  }
  if(ptype == kT2bW){
    SMS =  "pp #rightarrow #tilde{t} #tilde{t}; ";
    SMS += "#tilde{t} #rightarrow b #tilde{#chi}_{1}^{#pm}(W #tilde{#chi}_{1}^{0})";
  }
  if(ptype == kT2bb){
    SMS =  "pp #rightarrow #tilde{b} #tilde{b}; ";
    SMS += "#tilde{b} #rightarrow b #tilde{#chi}_{1}^{0}";
  }
  if(ptype == kTSlSl){
    SMS =  "pp #rightarrow #tilde{#it{l}}_{L/R} #tilde{#it{l}}_{L/R}; ";
    SMS += "#tilde{#it{l}}_{L/R} #rightarrow #it{l} #tilde{#chi}_{1}^{0}";
  }
    if(ptype == kT2cc){
    SMS = "pp #rightarrow #tilde{t} #tilde{t}; ";
    SMS += "#tilde{t} #rightarrow c #tilde{#chi}_{1}^{0}";
 }


  l.SetTextSize(0.035);
  l.SetTextFont(42);
  l.DrawLatex(0.18, 0.87,SMS.c_str());
  
  return can;
}

// vector<TGraph*> GetContourGraph(TH2D* hist, double rval){
//   TH2D* h = (TH2D*) hist->Clone("myclone");

//   int Nx = h->GetNbinsX();
//   int Ny = h->GetNbinsY();
//   for(int x = 0; x < Nx; x++){
//     for(int y = 0; y < Ny; y++){
//       if(h->GetBinContent(x+1,y+1) <= 0.)
// 	h->SetBinContent(x+1,y+1,1000.);
//     }
//   }

//   TCanvas* can = (TCanvas*) new TCanvas("dum","dum",700.,600);
//   can->Draw();
//   can->cd();
  
//   h->SetContour(1);
//   h->SetContourLevel(0, rval);
  
//   h->Draw("CONT LIST");
//   gPad->Update();
//   TObjArray *conts = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
//   TList *l = (TList*) conts->At(0);

//   int N = l->GetEntries();

//   vector<TGraph*> gr;
//   for(int i = 0; i < N; i++){
//     gr.push_back((TGraph*) l->At(i)->Clone());
//   }
  
//   delete l;
//   delete can;
//   delete h;
//   return gr;
// }
