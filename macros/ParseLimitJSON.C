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
#include <TLatex.h>
#include <TH2D.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string>

#include "RestFrames/RestFrames.hh"

using namespace std;
using namespace RestFrames;

double popdouble(std::string& line);
std::string popstring(std::string& line);

TGraph2D* Get2DGraph_MCvMP(const vector<pair<int,double> >& vec);

TGraph* GetContourGraph_MCvMP(TH2D* hist, double rval = 1.);

TH2D* Get2DHist_MCvMP(const vector<pair<int,double> >& vec, const string& name);

TCanvas* Plot2DHist_MCvMP(const string& can, TH2D* hist);

void Invert2DHist(TH2D* hist);

map<int,int> MP;
map<int,int> MC;
int min_MP = -1;
int max_MP = -1;
int min_MC = -1;
int max_MC = -1;
int min_dM = -1;
int max_dM = -1;

void ParseLimitJSON(const string& json){
  RestFrames::SetStyle();

  std::ifstream ifile(json.c_str());
  if(!ifile.is_open()){
    std::cout << "can't open json file " << json << std::endl;
    return;
  }

  string line, name;
  size_t found, end;

  vector<pair<int,double> > R_exp0;
  vector<pair<int,double> > R_exp_p1;
  vector<pair<int,double> > R_exp_m1;
  vector<pair<int,double> > R_obs;

  vector<pair<int,double> > iR_exp0;
  vector<pair<int,double> > iR_exp_p1;
  vector<pair<int,double> > iR_exp_m1;
  vector<pair<int,double> > iR_obs;

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
      MP[mass] = mass/10000;
      MC[mass] = mass%10000;

      if(MP[mass] > max_MP || max_MP < 0)
	max_MP = MP[mass];
      if(MP[mass] < min_MP || min_MP < 0)
	min_MP = MP[mass];
      if(MC[mass] > max_MC || max_MC < 0)
	max_MC = MC[mass];
      if(MC[mass] < min_MC || min_MC < 0)
	min_MC = MC[mass];
      if(MP[mass]-MC[mass] > max_dM || max_dM < 0)
	max_dM = MP[mass]-MC[mass];
      if(MP[mass]-MC[mass] < min_dM || min_dM < 0)
	min_dM = MP[mass]-MC[mass];
      
      cout << "MP = " << MP[mass] << " MC = " << MC[mass] << endl;
      continue;
    }

    if(line.find("exp0") != string::npos){
      popstring(line);
      double r = popdouble(line);
      R_exp0.push_back(pair<int,double>(mass, r));
      iR_exp0.push_back(pair<int,double>(mass, 1./r));
    }
    if(line.find("exp+1") != string::npos){
      popstring(line);
      double r = popdouble(line);
      R_exp_p1.push_back(pair<int,double>(mass, r));
      iR_exp_p1.push_back(pair<int,double>(mass, 1./r));
    }
    if(line.find("exp-1") != string::npos){
      popstring(line);
      double r = popdouble(line);
      R_exp_m1.push_back(pair<int,double>(mass, r));
      iR_exp_m1.push_back(pair<int,double>(mass, 1./r));
    }
    if(line.find("obs") != string::npos){
      popstring(line);
      double r = popdouble(line);
      R_obs.push_back(pair<int,double>(mass, r));
      iR_obs.push_back(pair<int,double>(mass, 1./r));
    }
  }

  TH2D* hist_Rexp0 = Get2DHist_MCvMP(R_exp0, "R_exp0");
  Plot2DHist_MCvMP("norm", hist_Rexp0);

  TH2D* hist_iRexp0 = Get2DHist_MCvMP(iR_exp0, "iR_exp0");
  Invert2DHist(hist_iRexp0);

  TGraph* gr_iRexp0 = (TGraph*) GetContourGraph_MCvMP(hist_iRexp0, 1.);
  
  TCanvas* can_inv = Plot2DHist_MCvMP("inv", hist_iRexp0);
  can_inv->cd();
  gr_iRexp0->SetMarkerColor(kWhite);
  gr_iRexp0->Draw("same P");
 
  
  cout << "HERE" << endl;
  
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

TGraph2D* Get2DGraph_MCvMP(const vector<pair<int,double> >& vec){
  int N = vec.size();

  TGraph2D* gr = new TGraph2D();
  for(int i = 0; i < N; i++){
    gr->SetPoint(i, MP[vec[i].first], MC[vec[i].first], vec[i].second);
  }

  return gr;
}

TH2D* Get2DHist_MCvMP(const vector<pair<int,double> >& vec, const string& name){
  TGraph2D* gr = Get2DGraph_MCvMP(vec);

  int Nx = max_MP+1 - min_MP;
  int Ny = max_MC+1 - min_MC;
  double xmin = min_MP+1;
  double xmax = max_MP+1;
  double ymin = min_MC+1;
  double ymax = max_MC+1;
  
  TH2D* hist = new TH2D(name.c_str(),name.c_str(),
			Nx, xmin, xmax,
			Ny, ymin, ymax);

  for(int x = 0; x < Nx; x++){
    double mp = hist->GetXaxis()->GetBinCenter(x+1);
    for(int y = 0; y < Ny; y++){
      double mc = hist->GetYaxis()->GetBinCenter(y+1);
      if(mp-mc < min_dM)
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
      if(hist->GetBinContent(x+1,y+1) > 0 || filled == false)
	continue;
      
      if(x-1 >= 0){
	if(y-1 >= 0){
	  if(hist->GetBinContent(x, y) > 0.){
	    hist->SetBinContent(x+1,y+1, hist->GetBinContent(x, y));
	  }
	} else {
	  if(hist->GetBinContent(x, y+1) > 0.){
	    hist->SetBinContent(x+1,y+1, hist->GetBinContent(x, y+1));
	  }	    
	}
      }
    }
  }
  for(int x = Nx-1; x >= 0; x--){
    bool filled = false;
    for(int y = Ny-1; y >= 0; y--){
      if(hist->GetBinContent(x+1,y+1) > 0)
	filled = true;
      if(hist->GetBinContent(x+1,y+1) > 0 || filled == false)
	continue;
      
      if(x+1 < Nx){
	if(y-1 >= 0){
	  if(hist->GetBinContent(x+2, y) > 0.){
	    hist->SetBinContent(x+1,y+1, hist->GetBinContent(x+2, y));
	  }
	} else {
	  if(hist->GetBinContent(x+2, y+1) > 0.){
	    hist->SetBinContent(x+1,y+1, hist->GetBinContent(x+2, y+1));
	  }	    
	}
      }
    }
  }
	    
	
  delete gr;
  return hist;
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

TCanvas* Plot2DHist_MCvMP(const string& name, TH2D* hist){
  TCanvas* can = (TCanvas*) new TCanvas(name.c_str(),name.c_str(),700.,600);

   can->SetLeftMargin(0.15);
   can->SetRightMargin(0.18);
   can->SetBottomMargin(0.15);
   can->SetGridx();
   can->SetGridy();
   can->SetLogz();
   can->Draw();
   can->cd();

   hist->Draw("COLZ");

   return can;
}

TGraph* GetContourGraph_MCvMP(TH2D* hist, double rval){
  TH2D* h = (TH2D*) hist->Clone("myclone");

  int Nx = h->GetNbinsX();
  int Ny = h->GetNbinsY();
  for(int x = 0; x < Nx; x++){
    for(int y = 0; y < Ny; y++){
      if(h->GetBinContent(x+1,y+1) <= 0.)
	h->SetBinContent(x+1,y+1,1000.);
    }
  }

  TCanvas* can = (TCanvas*) new TCanvas("dum","dum",700.,600);
  can->Draw();
  can->cd();
  
  h->SetContour(1);
  h->SetContourLevel(0, rval);
  
  h->Draw("CONT LIST");
  gPad->Update();
  TObjArray *conts = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  TList *l = (TList*) conts->At(0);
  
  TGraph *gr = (TGraph*) l->At(0)->Clone();

  delete l;
  delete can;
  delete h;
  return gr;
}
