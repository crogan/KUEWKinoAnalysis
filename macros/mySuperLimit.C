

#include "RestFrames/RestFrames.hh"
TH2D* GetTH2D(std::string fname, std::string objName){
	TFile* f = TFile::Open(fname.c_str());
	TH2D* o =(TH2D*) f->Get(objName.c_str());
	o->SetDirectory(0);
	f->Close();
	return o;
}
TH2D* GetBigTH2D(int xlo,int xup, int ylo, int yup){
	int nbinX=600;
	int nbinY=600;
	double binCenterX;
	double binCenterY;
	TH2D* h = new TH2D("h2","h2",nbinX,xlo,xup,nbinY,ylo,yup);
	h->SetMinimum(0.01);
	h->SetMaximum(1.);

	for(int i=1; i<=nbinX;i++){
		for(int j=1; j<=nbinY; j++){
			binCenterX = h->GetXaxis()->GetBinCenter(i);	
			binCenterY = h->GetYaxis()->GetBinCenter(j);
			if( (binCenterX - binCenterY) <= 0. ){
				h->SetBinContent(i,j,0.8);
			}
			else{
				h->SetBinContent(i,j,0);
			}
		}
	}
	return h;
}
TGraph* GetTGraph(std::string fname, std::string objName){
	
	TFile* f = TFile::Open(fname.c_str());
	TGraph* o =(TGraph*) f->Get(objName.c_str());
	//o->SetDirectory(0);
	f->Close();
	return o;
}
void FormatLine( TGraph* g, int style, int color){
	g->SetLineColor(color);
	g->SetLineWidth(3);
	g->SetLineStyle(style);

}
void FormatTH2D( TH2D* hist, std::string xlabel, std::string ylabel, int mode){
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
  hist->GetZaxis()->SetTitle("95% CL upper limit [fb]");
  int nbinsX = hist->GetXaxis()->GetNbins();
  int nbinsY = hist->GetYaxis()->GetNbins();
  if(mode != 2){
  for(int i=1; i<= nbinsX; i++){
	for(int j=1; j<=nbinsY; j++){
		hist->SetBinContent(i,j,0);		
	}
  }
  }

}
void FormatTGraph( TGraph* hist, std::string xlabel, std::string ylabel){
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
}
enum PlotType { kTChiWZ, kT2tt, kT2bW, kT2bb, kTSlSl, kT2cc, kTChiWW, kHN2C1, kTSlSnu, kNoSig};

void mySuperLimit(PlotType s1=kT2tt, PlotType s2=kT2bW, PlotType s3=kT2cc, PlotType s4=kNoSig){
//void mySuperLimit(PlotType s1=kHN2C1, PlotType s2=kTChiWZ, PlotType s3=kTChiWW, PlotType s4=kTSlSnu){
//void mySuperLimit(PlotType s1=kHN2C1, PlotType s2=kTChiWZ, PlotType s3=kTChiWW, PlotType s4=kNoSig){
  RestFrames::SetStyle();
  PlotType ptype = s1;

  std::string f1="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TChiWZSuper_xsec_smooth_canv.root";
  std::string f2="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TChipmWWSuper_xsec_smooth_canv.root";
  std::string f3="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TSlepSnu_xsec_smooth_canv.root";
  std::string f4="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_T2ttSuper_xsec_smooth_canv.root";
  std::string f5="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BF_B135_bugfix16_T2bb_MCstats/B135_bugfix16_T2bb_limit_canv.root";
  std::string f6="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_T2cc_xsec_smooth_canv.root";
  std::string f7="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_T2bW_xsec_smooth_canv.root";
  std::string f8="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_HinoN2C1Super_xsec_smooth_canv.root"; 
/*
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_HinoN2C1Super_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_HinoN2C1_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_T2bW_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_T2cc_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_T2tt_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TChiWZSuper_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TChiWZ_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TChipmWWSuper_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TChipmWW_xsec_smooth_canv.root
/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/Limit_JSONs/smooth_canvs/B135_bugfix16_TSlepSnu_xsec_smooth_canv.root
*/


  std::map<PlotType,std::string> limitMap{};
  limitMap[kTChiWZ] = f1;
  limitMap[kTChiWW] = f2;
  limitMap[kTSlSnu] = f3;
  limitMap[kT2tt] = f4;
  limitMap[kT2bb] = f5;
  limitMap[kT2cc] = f6;
  limitMap[kT2bW] = f7;
  limitMap[kHN2C1] = f8; 
  
  std::vector<PlotType> signals= {s1,s2,s3,s4};
  std::vector<TGraph*> gexp_vec{};
  std::vector<TGraph*> gobs_vec{};


  //select a mode so we know where to draw legend latex
  int mode = 0;
  std::cout<<"ptype = "<<s1<<"\n";
  if( s1 == kT2tt || s1 == kT2bW || s1 == kT2cc ) mode = 1;
  if( s1 == kHN2C1 || s1 == kTChiWZ || s1 == kTChiWW || s1 == kTSlSnu ) mode = 2;
  if( mode == 2 && s4 == kNoSig ) mode = 3;
  std::cout<<"mode "<<mode<<"\n";
  //z-scale is s1
  TH2D* hist;
  if(mode ==1) hist =  GetTH2D( limitMap[s1], "h2d");
  //if(mode==2) hist = GetTH2D( limitMap[s4],"h2d");
  if(mode==2) hist = GetBigTH2D(50,950,50,200);
  if(mode==3) hist = GetBigTH2D(50,675,5,50);

  for( int i=0; i<signals.size(); i++){
	if( signals[i] != kNoSig ){
		gexp_vec.push_back( GetTGraph( limitMap[ signals[i] ], "gr_mid"));
		gobs_vec.push_back( GetTGraph( limitMap[ signals[i] ], "gr_obs"));
	}
  }

/*  TH2D* hist = GetTH2D(f1,"h2d");
  TGraph* gexp1 =  GetTGraph(f1, "gr_mid");
  TGraph* gexp2 = GetTGraph(f2, "gr_mid");
  TGraph* gobs1 = GetTGraph(f1, "gr_obs");
  TGraph* gobs2 = GetTGraph(f2, "gr_obs");
*/
  gStyle->SetPalette(52);

  TCanvas* c = new TCanvas("c","c",700,700);

//    gStyle->SetPalette(52);
 
//  gStyle->SetPalette(kViridis);
 
  string xlabel = "m_{P} [GeV]";
  string ylabel = "m_{P} - M_{ #tilde{#chi}^{0}_{1}} [GeV]";

  if(ptype == kTChiWZ || ptype == kHN2C1){
    xlabel = "m_{(#tilde{#chi}^{#pm}_{1}, #tilde{#chi}^{0}_{2})} [GeV]";
    //ylabel = "m_{\/#tilde{#chi}^{#pm}_{1}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
    ylabel = "#Delta m(#tilde{#chi}^{#pm}_{1}, #tilde{#chi}^{0}_{1}) [GeV]";
  }
  if(ptype == kT2tt || ptype == kT2bW){
    xlabel = "m_{ #tilde{t}} [GeV]";
    //ylabel = "m_{ \/#tilde{t}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
    ylabel = "#Delta m(#tilde{t}, #tilde{#chi}^{0}_{1}) [GeV]";
  }
  if(ptype == kT2bb){
    xlabel = "m_{ #tilde{b}} [GeV]";
    //ylabel = "m_{ \/#tilde{b}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
    ylabel = "#Delta m(#tilde{b}, m_{#tilde{#chi}^{0}_{1}) [GeV]";
  }
    if(ptype == kT2cc){
    xlabel = "m_{ #tilde{t}} [GeV]";
    //ylabel = "m_{\/#tilde{#chi}^{0}_{1}} [GeV]";
    ylabel = "#Delta m(#tilde{t}, #tilde{#chi}^{0}_{1}) [GeV]";
  }
  if(ptype == kTSlSl){
    xlabel = "m_{ #tilde{#it{l}}} [GeV]";
   // ylabel = "m_{\/#tilde{#it{l}}} - m_{#tilde{#chi}^{0}_{1}} [GeV]";
   ylabel = "#Delta m( ) [GeV]";

 }
  if(ptype == kTChiWW){
    xlabel = "m_{#tilde{#chi}^{#pm}_{1}} [GeV]";
    ylabel = "#Delta m(#tilde{#chi}^{#pm}_{1}, #tilde{#chi}^{0}_{1}) [GeV]";
//    ylabel = "\/#Delta m( ) [GeV]";

 }
  if(ptype == kTSlSnu){
    xlabel = "m_{(#tilde{#chi}^{0}_{2}, #tilde{#chi}^{#pm}_{1} )} [GeV]";
    ylabel = "#Delta m(#tilde{#chi}^{#pm}_{1}, #tilde{#chi}^{0}_{1}) [GeV]";
  }


  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.05);
  c->SetBottomMargin(0.15);
  c->SetTopMargin(0.07);
  c->SetGridx();
  c->SetGridy();
  //c->SetLogz(); 
 // gStyle->SetPalette(52);
  
   FormatTH2D(hist, xlabel, ylabel,mode);
      hist->Draw("COL");
   
    //FormatTGraph(gexp_vec[0], xlabel, ylabel);
    //gexp_vec[0]->Draw("C");
    //gobs_vec[0]->Draw("same C");
    //
   if(mode ==1){
	hist->GetXaxis()->SetRangeUser(251,850);
        hist->GetYaxis()->SetRangeUser(10,80);
   }
   if(mode ==2){
	hist->GetXaxis()->SetRangeUser(50, 925);
        hist->GetYaxis()->SetRangeUser(50,200);
   }
   if(mode ==3){
	hist->GetXaxis()->SetRangeUser(75,675);
	hist->GetYaxis()->SetRangeUser(5,50);
   }

   std::vector<int> colors = {kMagenta+1, kRed+1, kBlue+1, kGreen+1};
   for(int i=0; i<gexp_vec.size(); i++){	
	FormatLine(gexp_vec[i], 7, colors[i]);		
	FormatLine(gobs_vec[i], 1, colors[i]);
   }
   for(int i=0; i<gexp_vec.size(); i++){
	gexp_vec[i]->Draw("same C");
 	gobs_vec[i]->Draw("same C");	
   } 
//   FormatLine(gexp1, 1, kRed);
//   FormatLine(gobs1, 2, kRed+3);
//   FormatLine(gexp2, 1, kBlue);
//   FormatLine(gobs2, 2, kBlue+3);

//   gexp1->Draw("same C");
//   gexp2->Draw("same C");
//   gobs1->Draw("same C");
//   gobs2->Draw("same C");

   TLatex l;
   l.SetTextFont(42);
  l.SetNDC();

  TLine* line = new TLine();
  line->SetLineWidth(2);
  line->SetLineColor(kBlack);
  
  double xpos,ypos;
  if(mode ==1 ){ xpos = 0.18; ypos=0.68; }

  if(mode ==2){ xpos = 0.57; ypos = 0.76; }
  if(mode ==3){ xpos = 0.57; ypos = 0.72; }

  l.SetTextAlign(31);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.95, 0.95, "138 fb^{-1} (13 TeV)");

  l.SetTextAlign(11);
  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.16, 0.95,"#bf{CMS}");
  //l.DrawLatex(0.16, 0.95,"\/#bf{CMS} #it{Preliminary}");
  l.SetTextSize(0.05);
  l.SetTextFont(42);
  l.SetNDC();
  //TLine* line = new TLine();
  l.SetTextAlign(12);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
 // l.SetTextColor(kWhite);
   l.DrawLatex(xpos+0.05, ypos+0.13,"expected");
  line->SetLineColor(kBlack);
  line->SetLineWidth(3);
  line->SetLineStyle(7);
  line->DrawLineNDC(xpos, ypos+0.13, xpos+0.04, ypos+0.13);
  line->SetLineStyle(1);
//  line->DrawLineNDC(xpos, 0.842, xpos+0.04, 0.842);
//  line->DrawLineNDC(xpos, 0.818, xpos+0.04, 0.818);

  l.DrawLatex(xpos+0.05, ypos+0.08,"observed");
  line->SetLineColor(kBlack);
  line->SetLineWidth(3);
  line->SetLineStyle(1);
  line->DrawLineNDC(xpos, ypos+0.08, xpos+0.04,ypos+0.08);
   // SMS info
   
  string SMS4="m_{\\~{\\chi}_{1}^{0}}<0";
  std::map<PlotType,std::string> legendMap{};
  double spacing = 0.07;
  double spacingFactor = 0.;
  for(int i=0; i< signals.size(); i++){
  ptype = signals[i];
  if( ptype == kNoSig) continue;
  string SMS;
  string SMS2="";
  string SMS3="";
  //string SMS4=";
  if(ptype == kTChiWZ){
  //  SMS =  "pp \/#rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm} ";
        SMS =  "pp \\rightarrow \\~{\\chi}_{1}^{\\pm}\\,\\~{\\chi}_{2}^{0}\\,\\, ";

    SMS += "(Wino)";
     SMS2 = "\\~{\\chi}_{1}^{\\pm}\\rightarrow W^*\\,\\~{\\chi}_{1}^{0}, ";
    SMS2 += "\\~{\\chi}_{2}^{0} \\rightarrow Z^*\\,\\~{\\chi}_{1}^{0}";
    //SMS2 += "\\~{\\chi}_{1}^{\\pm}\\rightarrow W^*\\,\\~{\\chi}_{1}^{0}";
   
  }
  if(ptype == kHN2C1){
    SMS =  "pp \\rightarrow \\~{\\chi}_{1}^{\\pm}\\,\\~{\\chi}_{2}^{0}\\,\\, ";
    SMS += "(Higgsino)";
    SMS2 = "\\~{\\chi}_{1}^{\\pm} \\rightarrow W^*\\,\\~{\\chi}_{1}^{0}, ";

    SMS2 += "\\~{\\chi}_{2}^{0} \\rightarrow Z^*\\,\\~{\\chi}_{1}^{0}";
   // SMS2 += "\\~{\\chi}_{1}^{\\pm} \\rightarrow W^*\\,\\~{\\chi}_{1}^{0}";
   

  }
  if(ptype == kT2tt){
    SMS =  "pp \\rightarrow \\~{t}\\,\\bar{\\~{t}}\\,\\,\\, ";
    SMS += "\\~{t} \\rightarrow t\\,\\~{\\chi}_{1}^{0}";
  }
  if(ptype == kT2bW){
    SMS =  "pp \\rightarrow \\~{t}\\,\\bar{\\~{t}}\\,\\,\\, ";
    SMS += "\\~{t} \\rightarrow b\\,\\~{\\chi}_{1}^{\\pm}\\, (W \\~{\\chi}_{1}^{0})";
  }
  if(ptype == kT2bb){
    SMS =  "pp \\rightarrow \\tilde{b} \\tilde{b}; ";
    SMS += "\\tilde{b} \\rightarrow b \\tilde{\\chi}_{1}^{0}";
  }
  if(ptype == kTSlSl){
    SMS =  "pp \\rightarrow \\tilde{\\ell}_{L/R} #tilde{#it{l}}_{L/R}; ";
    SMS += "\\tilde{#it{l}}_{L/R} #rightarrow #it{l} #tilde{#chi}_{1}^{0}";
  }
    if(ptype == kT2cc){
    SMS = "pp \\rightarrow \\~{t}\\,\\bar{\\~{t}}\\,\\,\\, ";
    SMS += "\\~{t} \\rightarrow c\\,\\~{\\chi}_{1}^{0}";
 }
 if(ptype == kTChiWW){
    SMS = "pp \\rightarrow \\~{\\chi}_{1}^{+}\\,\\~{\\chi}_{1}^{-}\\,\\, (Wino) ";
    SMS2 = "\\~{\\chi}_{1}^{\\pm} \\rightarrow W^*\\,\\~{\\chi}_{1}^{0}";
 }
 if(ptype == kTSlSnu){
    //SMS = "pp \/#rightarrow #tilde{#chi}_{2}^{0} #tilde{#chi}_{1}^{#pm}; ";
    //SMS += "\/#tilde{#chi}_{2}^{0} #rightarrow #tilde{#it{l}} #it{l}; ";
    //SMS += "\/#tilde{#chi}_{1}^{#pm} #rightarrow #tilde{#nu} #it{l}; ";
    //SMS2 += "\/#tilde{#it{l}} #rightarrow \ell #tilde{#chi}_{1}^{0}; ";
    //SMS2 += "\/#tilde{#nu} #rightarrow #it{#nu} #tilde{#chi}_{1}^{0}; ";
    SMS = "pp \\rightarrow \\~{\\chi}^{+}_{1}\\,\\~{\\chi}_{1}^{-}\\,\\, (Wino)";
	SMS2 = "\\~{\\chi}^{\\pm}_{1} \\rightarrow \\~{\\ell}\\, \\nu, \\, \\, ";
	SMS2 += "\\~{\\chi}_{1}^{\\pm} \\rightarrow \\~{\\nu} \\ell ";
	SMS3 = "\\~{\\ell} \\rightarrow \\ell \\,\\~{\\chi}_{1}^{0}, \\,\\,";
	SMS3 += "\\~{\\nu} \\rightarrow \\nu \\,\\~{\\chi}_{1}^{0} ";


  }
  TMathText l2;
  l2.SetNDC(); 
  l2.SetTextSize(0.035);
  //l.SetTextFont(42);
  l2.SetTextColor(colors[i]);
  
  l2.DrawMathText(xpos, ypos-(i+spacingFactor)*spacing,SMS.c_str());
  if( ptype == kTSlSnu || ptype == kTChiWZ ||  ptype == kHN2C1 || ptype == kTChiWW ){
  l2.DrawMathText(xpos, ypos-(i+1+spacingFactor)*(spacing),SMS2.c_str());
  //  spacingFactor +=1.;
   if( ptype == kTSlSnu ){
  l2.DrawMathText(xpos, ypos-(i+2+spacingFactor)*(spacing),SMS3.c_str());
   spacingFactor +=1.;
  }
   spacingFactor +=1;
   }
 
 }

  if(mode ==2 ){
        TMathText l2;
  l2.SetNDC();
  l2.SetTextSize(0.030);
	
	l2.SetTextAngle(78);
	l2.SetTextColor(kBlack);
	l2.DrawMathText(0.21,0.63,SMS4.c_str());
 } 
  //gexp1->Draw("SAME");
  //gexp2->Draw("SAME");
  std::string outname;
  if(mode == 1) outname = "Stop_dmLow.root";
  if(mode ==2 ) outname = "EWK_dmHigh.root";
  if(mode ==3 ) outname = "EWK_dmLow.root";
  TFile* out = new TFile(outname.c_str(), "RECREATE");
  out->WriteTObject(c,"super_canv");
  out->Close();
/*
   TLatex l;
  l.SetTextFont(42);
  l.SetNDC();
  TLine* line = new TLine();
  l.SetTextAlign(12);
  l.SetTextSize(0.035);
  l.SetTextFont(42);
 // l.SetTextColor(kWhite);
  l.DrawLatex(0.23, 0.83,"expected \/#pm 1 #sigma_{expm}");
  line->SetLineColor(7043);
  line->SetLineWidth(2);
  line->SetLineStyle(1);
  line->DrawLineNDC(0.18, 0.83, 0.22, 0.83);
  line->SetLineStyle(3);
  line->DrawLineNDC(0.18, 0.842, 0.22, 0.842);
  line->DrawLineNDC(0.18, 0.818, 0.22, 0.818);
// l.SetTextColor(kWhite);                  
  l.DrawLatex(0.23, 0.78,"observed");
  line->SetLineColor(kBlack);
//  line->SetLineColor(kWhite);
  line->SetLineWidth(2);
  line->SetLineStyle(1);
  line->DrawLineNDC(0.18, 0.78, 0.22, 0.78);

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

*/

}
