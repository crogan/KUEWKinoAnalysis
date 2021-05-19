#include <iostream>
#include <map>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TSystem.h>
#include <TMultiGraph.h>
#include <TEfficiency.h>

#include "FitReader.hh"

///////////////////////////////////////////
////////// FitReader class
///////////////////////////////////////////

FitReader::FitReader(const string& inputfile,
		     const string& otherfile,
		     const string& otherfold)
  : m_File(inputfile.c_str(), "READ") {

  if(otherfile != ""){
    m_FilePtr = new TFile(otherfile.c_str(), "READ");
    if(!m_FilePtr || !m_FilePtr->IsOpen())
      m_FilePtr = nullptr;
    m_FileFold = otherfold+"/";
  } else {
    m_FilePtr = nullptr;
  }
  
  ReadProcesses();
  
  ReadCategories();
}

FitReader::~FitReader(){
  m_File.Close();
}

void FitReader::ReadProcesses(){
  if(!m_File.IsOpen())
    return;
  
  TTree* tree = (TTree*)m_File.Get("Process");
  if(!tree)
    return;

  m_ProcBranch.InitGet(tree);

  ProcessList ProcSys;

  int N = tree->GetEntries();
  for(int i = 0; i < N; i++){
    tree->GetEntry(i);
    
    Process p = m_ProcBranch.GetProcess();
	//cout << "ProcSys name: " << p.Name() << endl;    
 if((p.Name().find("Up") != std::string::npos) ||
       (p.Name().find("Down") != std::string::npos))
      ProcSys += p;
    else
      m_Proc += p;
  }
  
  delete tree;

  int Nproc = m_Proc.GetN();
  int Nsys  = ProcSys.GetN();
  for(int p = 0; p < Nproc; p++){
    Systematics sys;
    string proc = m_Proc[p].Name();
//if(proc.find("Fakes") != std::string::npos) continue;
//cout << "proc #" << p << ": " << proc << endl;
    for(int s = 0; s < Nsys; s++){
      string label = ProcSys[s].Name();
      if((proc.find("Fakes") == std::string::npos) &&
	 (label.find("Fakes") != std::string::npos))
	continue;
      if(label.find(proc) == 0){
	label.replace(0,proc.length()+1,"");
	if(label.find("Up") != std::string::npos)
	  label.replace(label.find("Up"),2,"");
	if(label.find("Down") != std::string::npos)
	  label.replace(label.find("Down"),4,"");
	sys += Systematic(label);
      }
    }
    if(sys.GetN() > 0)
      m_ProcSys[m_Proc[p]] = sys;
    m_Sys += sys;
  }
}

void FitReader::ReadCategories(){
  if(!m_File.IsOpen())
    return;
  
  TTree* tree = (TTree*)m_File.Get("Category");
  if(!tree)
    return;
  
  m_CatBranch.InitGet(tree);

  int N = tree->GetEntries();
  for(int i = 0; i < N; i++){
    tree->GetEntry(i);
    Category cat = m_CatBranch.GetCategory();
    if(m_CatLabel.count(cat.GetLabel()) == 0)
      m_CatLabel[cat.GetLabel()] = true;
    else
      continue;
    m_Cat += cat;
    if(m_Chan.count(cat.Label()) == 0)
      m_Chan[cat.Label()] = CategoryList();
    m_Chan[cat.Label()] += cat;
  }

  delete tree;
}

bool FitReader::HasSystematic(const Process& proc, const Systematic& sys) const {
  if(m_ProcSys.count(proc) == 0)
    return false;

  return m_ProcSys[proc].Contains(sys);
}

double FitReader::Integral(const Category&   cat,
			   const Process&    proc,
			   const Systematic& sys) const {
  const TH1D* hist = GetHistogram(cat, proc, sys);
  if(!hist)
    return 0.;

  return hist->Integral();
}

TH1D* FitReader::GetIntegralHist(const string& name,
				 const CategoryList& cats,
				 const ProcessList&  procs,
				 const Systematic&   sys) const {
  int Np = procs.GetN();
  int Nc = cats.GetN();

  TH1D* hist = nullptr;
  TH1D* histd = nullptr;

  for(int p = 0; p < Np; p++){
    for(int c = 0; c < Nc; c++){
      if(!IsFilled(cats[c], procs[p], sys))
	continue;
    
      if(!hist){
	histd = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone("dum");
	histd->Rebin(histd->GetNbinsX());
	hist = (TH1D*) new TH1D(name.c_str(),name.c_str(), 1, 0., 1.);
	hist->SetBinContent(1, histd->GetBinContent(1));
	hist->SetBinError(1, histd->GetBinError(1));
	delete histd;
      } else {
	TH1D* dum = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone("dum");
	dum->Rebin(dum->GetNbinsX());
	hist->SetBinContent(1, hist->GetBinContent(1)+dum->GetBinContent(1));
	hist->SetBinError(1, sqrt(hist->GetBinError(1)*hist->GetBinError(1)+dum->GetBinError(1)*dum->GetBinError(1)));
	delete dum;
      }
    }
  }
   
  return hist;
}

TH1D* FitReader::GetAddedHist(const string&       name,
			      const CategoryList& cats,
			      const ProcessList&  procs,
			      const Systematic&   sys) const {
  int Np = procs.GetN();
  int Nc = cats.GetN();
  
  TH1D* hist = nullptr;

  for(int p = 0; p < Np; p++){
    for(int c = 0; c < Nc; c++){
      if(!IsFilled(cats[c], procs[p], sys))
	continue;
    
      if(!hist){
	hist = (TH1D*) GetHistogram(cats[c], procs[p], sys)->Clone(name.c_str());
      } else {
	hist->Add(GetHistogram(cats[c], procs[p], sys));
      }
    }
  }
   
  return hist;
}

const TH1D* FitReader::GetHistogram(const Category&   cat,
				    const Process&    proc,
				    const Systematic& sys) const {
  if(!IsFilled(cat, proc, sys))
    return nullptr;

  if(!sys){
    return m_ProcHist[proc][cat];
  } else {
    return (sys.IsUp() ? m_ProcHistSys[proc][sys][cat].first :
	    m_ProcHistSys[proc][sys][cat].second);
  }
}

const TH2D* FitReader::GetHistogram2D(const Category&   cat,
				      const Process&    proc,
				      const Systematic& sys) const {
  if(!IsFilled2D(cat, proc, sys))
    return nullptr;
 cout << cat.GetLabel() << " " << proc.Name() << " hist integral: " << m_ProcHist_2D[proc][cat]->Integral() << endl;   
  if(!sys){
    return m_ProcHist_2D[proc][cat];
  } else {
    return (sys.IsUp() ? m_ProcHistSys_2D[proc][sys][cat].first :
	    m_ProcHistSys_2D[proc][sys][cat].second);
  }
}

bool FitReader::IsFilled(const Category&   cat,
			 const Process&    proc,
			 const Systematic& sys) const {
 
  if(!sys){
    if(m_ProcHist.count(proc) == 0)
      m_ProcHist[proc] = map<Category,TH1D*>();
    if(m_ProcHist[proc].count(cat) == 0){
      string shist = cat.Label()+"_"+cat.GetLabel()+"/"+proc.Name();
      if(proc.Type() == kData || !m_FilePtr)
	m_ProcHist[proc][cat] = (TH1D*) m_File.Get(shist.c_str());
      else
	m_ProcHist[proc][cat] = (TH1D*) m_FilePtr->Get((m_FileFold+shist).c_str());
    }
    
    return m_ProcHist[proc][cat];
    
  } else {
    if(m_ProcHistSys.count(proc) == 0)
      m_ProcHistSys[proc] = map<Systematic,map<Category,pair<TH1D*,TH1D*> > >();
    if(m_ProcHistSys[proc].count(sys) == 0)
      m_ProcHistSys[proc][sys] = map<Category,pair<TH1D*,TH1D*> >();
    if(m_ProcHistSys[proc][sys].count(cat) == 0){
      m_ProcHistSys[proc][sys][cat] = pair<TH1D*,TH1D*>(nullptr,nullptr);
       
      string label = cat.Label()+"_"+cat.GetLabel();
      string shistUp   = label+"/"+proc.Name()+"_"+sys.Label()+"Up";
      string shistDown = label+"/"+proc.Name()+"_"+sys.Label()+"Down";
       
      m_ProcHistSys[proc][sys][cat].first  = (TH1D*) m_File.Get(shistUp.c_str());
      m_ProcHistSys[proc][sys][cat].second = (TH1D*) m_File.Get(shistDown.c_str());
    }
     
    return (sys.IsUp() ? m_ProcHistSys[proc][sys][cat].first :
	    m_ProcHistSys[proc][sys][cat].second);
  }    
}

bool FitReader::IsFilled2D(const Category&   cat,
			   const Process&    proc,
			   const Systematic& sys) const {
  
  if(!sys){
    if(m_ProcHist_2D.count(proc) == 0)
      m_ProcHist_2D[proc] = map<Category,TH2D*>();
    if(m_ProcHist_2D[proc].count(cat) == 0){
      string shist = cat.Label()+"_"+cat.GetLabel()+"/"+proc.Name()+"_2D";
      if(proc.Type() == kData || !m_FilePtr)
	m_ProcHist_2D[proc][cat] = (TH2D*) m_File.Get(shist.c_str());
      else
	m_ProcHist_2D[proc][cat] = (TH2D*) m_FilePtr->Get((m_FileFold+shist).c_str());
    }
    
    return m_ProcHist_2D[proc][cat];
    
  } else {
    if(m_ProcHistSys_2D.count(proc) == 0)
      m_ProcHistSys_2D[proc] = map<Systematic,map<Category,pair<TH2D*,TH2D*> > >();
    if(m_ProcHistSys_2D[proc].count(sys) == 0)
      m_ProcHistSys_2D[proc][sys] = map<Category,pair<TH2D*,TH2D*> >();
    if(m_ProcHistSys_2D[proc][sys].count(cat) == 0){
      m_ProcHistSys_2D[proc][sys][cat] = pair<TH2D*,TH2D*>(nullptr,nullptr);
       
      string label = cat.Label()+"_"+cat.GetLabel();
      string shistUp   = label+"/"+proc.Name()+"_"+sys.Label()+"Up_2D";
      string shistDown = label+"/"+proc.Name()+"_"+sys.Label()+"Down_2D";
       
      m_ProcHistSys_2D[proc][sys][cat].first  = (TH2D*) m_File.Get(shistUp.c_str());
      m_ProcHistSys_2D[proc][sys][cat].second = (TH2D*) m_File.Get(shistDown.c_str());
    }
     
    return (sys.IsUp() ? m_ProcHistSys[proc][sys][cat].first :
	    m_ProcHistSys[proc][sys][cat].second);
  }    
}

void FitReader::PrintCategories(bool verbose){
  cout << "*** Fit Categories ***" << endl;
  int N = m_Cat.GetN();
  for(int i = 0; i < N; i++)
    cout << m_Cat[i].Label()+"_"+m_Cat[i].GetLabel() << endl;
  cout << endl;
}

void FitReader::PrintProcesses(bool verbose){
  cout << "*** Fit Processes ***" << endl;
  int N = m_Proc.GetN();
  for(int i = 0; i < N; i++){
    cout << m_Proc[i].Name() << endl;
    if(m_ProcSys.count(m_Proc[i]) == 0 || !verbose)
      continue;
    cout << "  Sys:  " << m_ProcSys[m_Proc[i]][0].Label() << endl;
    int Nsys = m_ProcSys[m_Proc[i]].GetN();
    for(int s = 1; s < Nsys; s++)
      cout << "        " << m_ProcSys[m_Proc[i]][s].Label() << endl;
  }
  cout << endl;
}

const ProcessList&  FitReader::GetProcesses() const {
  return m_Proc;
}

const CategoryList& FitReader::GetCategories(const string& channel) const {
  if(m_Chan.count(channel) > 0)
    return m_Chan[channel];
  
  return m_Cat;
}

VS FitReader::GetChannels() const {
  VS list;
  for(auto c : m_Chan)
    list += c.first;
  
  return list;
}

const Systematics& FitReader::GetSystematics() const {
  return m_Sys;
}

string FitReader::GetSignalTitle(const string& label){
  size_t p = label.rfind("_");
  if(p == std::string::npos)
    return label;

  string title = label.substr(0, p);
  int    mass  = stoi(label.substr(p+1,label.length()-p));
  return title+" "+std::to_string(mass/10000)+" "+std::to_string(mass%100000);
}

  
void FitReader::DrawCatTree(const CategoryTree& CT, TCanvas* can){
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



void FitReader::DrawMR(const FitBin& fitbin, TCanvas* can){
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






void FitReader::InitializeRecipes(){
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
  m_Strings["QCD"] = VS().a("QCD_Fakes_elf0").a("QCD_Fakes_elf1").a("QCD_Fakes_muf0").a("QCD_Fakes_muf1").a("QCD");

  
  m_Title["Fakes"] = "fake leptons";
  m_Color["Fakes"] = 7021;
  m_Strings["Fakes"] = VS().a("Fakes_elf0").a("Fakes_elf1").a("Fakes_elf2").a("Fakes_muf0").a("Fakes_muf1").a("Fakes_muf2");
  
  m_Title["ttbar_Fakes"] = "ttbar, all fakes";
  m_Color["Fakes"] = 7022;
  m_Strings["Fakes"] = VS().a("ttbar_Fakes_elf0").a("ttbar_Fakes_elf1").a("ttbar_Fakes_muf0").a("ttbar_Fakes_muf1");
  
  m_Title["Fake"] = "fake";
  m_Color["Fake"] = 7020;
  m_Strings["Fake"] = VS().a("Fakes_elf2").a("Fakes_muf2");

  m_Title["HF"] = "heavy flavor";
  m_Color["HF"] = 7022;
  m_Strings["HF"] = VS().a("Fakes_elf0").a("Fakes_muf0");

  m_Title["ttbar_HF"] = "ttbar, heavy flavor";
  m_Color["ttbar_HF"] = 7022;
  m_Strings["ttbar_HF"] = VS().a("ttbar_Fakes_elf0").a("ttbar_Fakes_muf0");
  
  m_Title["Wjets_HF"] = "W + jets, heavy flavor";
  m_Color["Wjets_HF"] = 7022;
  m_Strings["Wjets_HF"] = VS().a("Wjets_Fakes_elf0").a("Wjets_Fakes_muf0");
  
  m_Title["QCD_HF"] = "QCD fakes, heavy flavor";
  m_Color["QCD_HF"] = 7022;
  m_Strings["ttbar_HF"] = VS().a("QCD_Fakes_elf0").a("QCD_Fakes_muf0");
  
  m_Title["LF"] = "light flavor";
  m_Color["LF"] = 7021;
  m_Strings["LF"] = VS().a("Fakes_elf1").a("Fakes_muf1");

  m_Title["ttbar_LF"] = "ttbar, light flavor + unm.";
  m_Color["ttbar_LF"] = 7021;
  m_Strings["ttbar_LF"] = VS().a("ttbar_Fakes_elf1").a("ttbar_Fakes_muf1");
  
  m_Title["Wjets_LF"] = "W + jets, light flavor";
  m_Color["Wjets_LF"] = 7021;
  m_Strings["Wjets_LF"] = VS().a("Wjets_Fakes_elf1").a("Wjets_Fakes_muf1");
  
  m_Title["QCD_LF"] = "QCD fakes, light flavor + unm.";
  m_Color["QCD_LF"] = 7021;
  m_Strings["QCD_LF"] = VS().a("QCD_Fakes_elf1").a("QCD_Fakes_muf1");

  m_Title["ZDY_LF_el"] = "Z+DY, light flavor + unm.";
  m_Color["ZDY_LF_el"] = 7021;
  m_Strings["ZDY_LF_el"] = VS().a("ZDY_Fakes_elf1");
  
  m_Title["ZDY_HF_el"] = "Z+DY, heavy flavor + unm.";
  m_Color["ZDY_HF_el"] = 7021;
  m_Strings["ZDY_HF_el"] = VS().a("ZDY_Fakes_elf0");
  
  m_Title["ZDY_LF_mu"] = "Z+DY, light flavor + unm.";
  m_Color["ZDY_LF_mu"] = 7021;
  m_Strings["ZDY_LF_mu"] = VS().a("ZDY_Fakes_muf1");
  
  m_Title["ZDY_HF_mu"] = "Z+DY, heavy flavor + unm.";
  m_Color["ZDY_HF_mu"] = 7021;
  m_Strings["ZDY_HF_mu"] = VS().a("ZDY_Fakes_muf0");

  m_Title["DB+rare_LF_el"] = "Di-boson + tri-boson + rare, light flavor";
  m_Color["DB+rare_LF_el"] = 7021;
  m_Strings["DB+rare_LF_el"] = VS().a("TB_Fakes_elf1").a("DB_Fakes_elf1");
  
  m_Title["DB+rare_LF_mu"] = "Di-boson + tri-boson + rare, light flavor";
  m_Color["DB+rare_LF_mu"] = 7021;
  m_Strings["DB+rare_LF_mu"] = VS().a("TB_Fakes_muf1").a("DB_Fakes_muf1");
  
  m_Title["DB+rare_HF_el"] = "Di-boson + tri-boson + rare, heavy flavor";
  m_Color["DB+rare_HF_el"] = 7021;
  m_Strings["DB+rare_HF_el"] = VS().a("TB_Fakes_elf0").a("DB_Fakes_elf0");

  m_Title["DB+rare_HF_mu"] = "Di-boson + tri-boson + rare, heavy flavor";
  m_Color["DB+rare_HF_mu"] = 7021;
  m_Strings["DB+rare_HF_mu"] = VS().a("TB_Fakes_muf0").a("DB_Fakes_muf0");



  m_Title["Total"] = "total background";
  m_Color["Total"] = 7000;
  m_Strings["Total"] = VS().a("ttbar").a("ST").a("DB").a("ZDY").a("Wjets").a("Fakes_elf0").a("Fakes_elf1").
    a("Fakes_elf2").a("Fakes_muf0").a("Fakes_muf1").a("Fakes_muf2");
  
  // leptonic categories
  m_Title["1L"] = "#scale[1.2]{single #it{l}}";
  m_Strings["1L"] = VS().a("1L_elm-elG").a("1L_elp-elG").a("1L_elpm-elG").a("1L_mupm-muG").a("1L_mup-muG").a("1L_mum-muG").a("1L_elm-elS").a("1L_elp-elS").a("1L_elpm-elS").a("1L_mup-muS").a("1L_mum-muS").a("1L_mupm-muS").a("1L_elp-elB").a("1L_elm-elB").a("1L_elpm-elB").a("1L_mup-muB").a("1L_mum-muB").a("1L_mupm-muB");
 
  m_Title["1Lgold"] = "#scale[1.2]{single gold #it{l}}";
  m_Strings["1Lgold"] = VS().a("1L_elm-elG").a("1L_elp-elG").a("1L_elpm-elG").a("1L_mupm-muG").a("1L_mup-muG").a("1L_mum-muG");

  m_Title["1Lsilver"] = "#scale[1.2]{single silver #it{l}}";
  m_Strings["1Lsilver"] = VS().a("1L_elm-elS").a("1L_elp-elS").a("1L_elpm-elS").a("1L_mupm-muS").a("1L_mup-muS").a("1L_mum-muS");
 
  m_Title["1Lbronze"] = "#scale[1.2]{single bronze #it{l}}";
  m_Strings["1Lbronze"] = VS().a("1L_elm-elB").a("1L_elp-elB").a("1L_elpm-elB").a("1L_mupm-muB").a("1L_mup-muB").a("1L_mum-muB");
  
  m_Title["1Lel"] = "#scale[1.2]{single e}";
  m_Strings["1Lel"] = VS().a("1L_elp-elG").a("1L_elm-elG").a("1L_elpm-elG").a("1L_elp-elS").a("1L_elm-elS").a("1L_elpm-elS").a("1L_elp-elB").a("1L_elm-elB").a("1L_elpm-elB");

  m_Title["1Lmu"] = "#scale[1.2]{single #mu}";
  m_Strings["1Lmu"] = VS().a("1L_mup-muG").a("1L_mum-muG").a("1L_mupm-muG").a("1L_mup-muS").a("1L_mum-muS").a("1L_mupm-muS").a("1L_mup-muB").a("1L_mum-muB").a("1L_mupm-muB");

  m_Title["1Lelp"] = "#scale[1.2]{single e^{+}}";
  m_Strings["1Lelp"] = VS().a("1L_elp-elG");

  m_Title["1Lelm"] = "#scale[1.2]{single e^{-}}";
  m_Strings["1Lelm"] = VS().a("1L_elm-elG");

  m_Title["1Lmup"] = "#scale[1.2]{single #mu^{+}}";
  m_Strings["1Lmup"] = VS().a("1L_mup-muG");

  m_Title["1Lmum"] = "#scale[1.2]{single #mu^{-}}";
  m_Strings["1Lmum"] = VS().a("1L_mum-muG");

  m_Title["1Lp"] = "#scale[1.2]{single #it{l}^{+}}";
  m_Strings["1Lp"] = VS().a("1L_elp-elG").a("1L_mup-muG");

  m_Title["1Lm"] = "#scale[1.2]{single #it{l}^{-}}";
  m_Strings["1Lm"] = VS().a("1L_elm-elG").a("1L_mum-muG");

  m_Title["1Lelgold"] = "#scale[1.2]{single gold e}";
  m_Strings["1Lelgold"] = VS().a("1L_elp-elG").a("1L_elm-elG").a("1L_elpm-elG");
  
  m_Title["1Lelsilver"] = "#scale[1.2]{single silver e}";
  m_Strings["1Lelsilver"] = VS().a("1L_elp-elS").a("1L_elm-elS").a("1L_elpm-elS");
  
  m_Title["1Lmusilver"] = "#scale[1.2]{single silver #mu}";
  m_Strings["1Lmusilver"] = VS().a("1L_mup-muS").a("1L_mum-muS");

  
  m_Title["1Lelbronze"] = "#scale[1.2]{single bronze e}";
  m_Strings["1Lelbronze"] = VS().a("1L_elp-elB").a("1L_elm-elB").a("1L_elpm-elB");
  
  m_Title["1Lmubronze"] = "#scale[1.2]{single bronze #mu}";
  m_Strings["1Lmubonze"] = VS().a("1L_mup-muB").a("1L_mum-muB");

  m_Title["2LOSSF"] = "#scale[1.2]{e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}}";
  m_Strings["2LOSSF"] = VS().a("2LOS_el^el-elGelG").a("2LOS_mu^mu-muGmuG").a("2LOS_elel^0-elGelG").a("2LOS_mumu^0-muGmuG");
  
  m_Title["2LOSOF"] = "#scale[1.2]{e^{#pm} #mu^{#mp}}";
  m_Strings["2LOSOF"] = VS().a("2LOS_el^mu-elGmuG").a("2LOS_elmu^0-elGmuG");

  m_Title["2LSSSF"] = "#scale[1.2]{e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}}";
  m_Strings["2LSSSF"] = VS().a("2LOS_el^el-elGelG").a("2LOS_mu^mu-muGmuG").a("2LOS_elel^0-elGelG").a("2LOS_mumu^0-muGmuG");
  
  m_Title["2LSSOF"] = "#scale[1.2]{e^{#pm} #mu^{#pm}}";
  m_Strings["2LSSOF"] = VS().a("2LSS_el^mu-elGmuG").a("2LSS_elmu^0-elGmuG");

  m_Title["2LOSSFsilver"] = "#scale[1.2]{e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}, #geq 1 silver #it{l}}";
  m_Strings["2LOSSFsilver"] = VS().a("2LOS_el^el-el0el1").a("2LOS_mu^mu-mu0mu1").a("2LOS_elel^0-el0el1").a("2LOS_mumu^0-mu0mu1")
    .a("2LOS_el^el-el1el1").a("2LOS_mu^mu-mu1mu1").a("2LOS_elel^0-el1el1").a("2LOS_mumu^0-mu1mu1")
    .a("2LOS_el^el-el1el2").a("2LOS_mu^mu-mu1mu2").a("2LOS_elel^0-el1el2").a("2LOS_mumu^0-mu1mu2");
  
  m_Title["2LOSOFsilver"] = "#scale[1.2]{e^{#pm} #mu^{#mp}, #geq 1 silver #it{l}}";
  m_Strings["2LOSOFsilver"] = VS().a("2LOS_el^mu-el0mu1").a("2LOS_elmu^0-el0mu1").a("2LOS_el^mu-mu0el1").a("2LOS_elmu^0-mu0el1")
    .a("2LOS_el^mu-el1mu1").a("2LOS_elmu^0-el1mu1").a("2LOS_el^mu-mu1el2").a("2LOS_elmu^0-mu1el2")
    .a("2LOS_el^mu-el1mu2").a("2LOS_elmu^1-el1mu2");

  m_Title["2LSSSFsilver"] = "#scale[1.2]{e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}, #geq 1 silver #it{l}}";
  m_Strings["2LSSSFsilver"] = VS().a("2LSS_el^el-el0el1").a("2LSS_mu^mu-mu0mu1").a("2LSS_elel^0-el0el1").a("2LSS_mumu^0-mu0mu1")
    .a("2LSS_el^el-el1el1").a("2LSS_mu^mu-mu1mu1").a("2LSS_elel^0-el1el1").a("2LSS_mumu^0-mu1mu1")
    .a("2LSS_el^el-el1el2").a("2LSS_mu^mu-mu1mu2").a("2LSS_elel^0-el1el2").a("2LSS_mumu^0-mu1mu2");
  
  m_Title["2LSSOFsilver"] = "#scale[1.2]{e^{#pm} #mu^{#pm}, #geq 1 silver #it{l}}";
  m_Strings["2LSSOFsilver"] = VS().a("2LSS_el^mu-el0mu1").a("2LSS_elmu^0-el0mu1").a("2LSS_el^mu-mu0el1").a("2LSS_elmu^0-mu0el1")
    .a("2LSS_el^mu-el1mu1").a("2LSS_elmu^0-el1mu1").a("2LSS_el^mu-mu1el2").a("2LSS_elmu^0-mu1el2")
    .a("2LSS_el^mu-el1mu2").a("2LSS_elmu^1-el1mu2");

  m_Title["2LOSSFbronze"] = "#scale[1.2]{e^{#pm} e^{#mp} or #mu^{#pm} #mu^{#mp}, #geq 1 bronze #it{l}}";
  m_Strings["2LOSSFbronze"] = VS().a("2LOS_el^el-el0el2").a("2LOS_mu^mu-mu0mu2").a("2LOS_elel^0-el0el2").a("2LOS_mumu^0-mu0mu2")
    .a("2LOS_el^el-el1el2").a("2LOS_mu^mu-mu1mu2").a("2LOS_elel^0-el1el2").a("2LOS_mumu^0-mu1mu2")
    .a("2LOS_el^el-el2el2").a("2LOS_mu^mu-mu2mu2").a("2LOS_elel^0-el2el2").a("2LOS_mumu^0-mu2mu2");
  
  m_Title["2LOSOFbronze"] = "#scale[1.2]{e^{#pm} #mu^{#mp}, #geq 1 bronze #it{l}}";
  m_Strings["2LOSOFbronze"] = VS().a("2LOS_el^mu-el0mu2").a("2LOS_elmu^0-el0mu2").a("2LOS_el^mu-mu0el2").a("2LOS_elmu^0-mu0el2")
    .a("2LOS_el^mu-el1mu2").a("2LOS_elmu^0-el1mu2").a("2LOS_el^mu-mu1el2").a("2LOS_elmu^0-mu1el2")
    .a("2LOS_el^mu-el2mu2").a("2LOS_elmu^1-el2mu2");

  m_Title["2LSSSFbronze"] = "#scale[1.2]{e^{#pm} e^{#pm} or #mu^{#pm} #mu^{#pm}, #geq 1 bronze #it{l}}";
  m_Strings["2LSSSFbronze"] = VS().a("2LSS_el^el-el0el2").a("2LSS_mu^mu-mu0mu2").a("2LSS_elel^0-el0el2").a("2LSS_mumu^0-mu0mu2")
    .a("2LSS_el^el-el1el2").a("2LSS_mu^mu-mu1mu2").a("2LSS_elel^0-el1el2").a("2LSS_mumu^0-mu1mu2")
    .a("2LSS_el^el-el2el2").a("2LSS_mu^mu-mu2mu2").a("2LSS_elel^0-el2el2").a("2LSS_mumu^0-mu2mu2");
  
  m_Title["2LSSOFbronze"] = "#scale[1.2]{e^{#pm} #mu^{#pm}, #geq 1 bronze #it{l}}";
  m_Strings["2LSSOFbronze"] = VS().a("2LSS_el^mu-el0mu2").a("2LSS_elmu^0-el0mu2").a("2LSS_el^mu-mu0el2").a("2LSS_elmu^0-mu0el2")
    .a("2LSS_el^mu-el1mu2").a("2LSS_elmu^0-el1mu2").a("2LSS_el^mu-mu1el2").a("2LSS_elmu^0-mu1el2")
    .a("2LSS_el^mu-el2mu2").a("2LSS_elmu^1-el2mu2");

  m_Title["3Lelgold"] = "#scale[1.2]{e^{#pm}G e^{#pm}G e^{#pm}G}";
  m_Strings["3Lelgold"] = VS().a("elGelGelG");

  m_Title["3Lelsilver"] = "#scale[1.2]{e^{#pm}G e^{#pm}G e^{#pm}S}";
  m_Strings["3Lelsilver"] = VS().a("elGelGelS");

  m_Title["3Lelbronze"] = "#scale[1.2]{e^{#pm}G e^{#pm}G e^{#pm}B}";
  m_Strings["3Lelbronze"] = VS().a("elGelGelB");
  
  m_Title["3Leemugold"] = "#scale[1.2]{e^{#pm}G e^{#pm}G #mu^{#pm}G}";
  m_Strings["3Leemugold"] = VS().a("elGelGmuG");
  
  m_Title["3Leemusilver"] = "#scale[1.2]{e^{#pm}G (e^{#pm} #mu^{#pm})S}";
  m_Strings["3Leemusilver"] = VS().a("elGelGmuS").a("elGmuGelS");
  
  m_Title["3Leemubronze"] = "#scale[1.2]{e^{#pm}G (e^{#pm} #mu^{#pm})B}";
  m_Strings["3Leemubronze"] = VS().a("elGelGmuB").a("elGmuGelB");

  m_Title["3Lemumugold"] = "#scale[1.2]{e^{#pm}G #mu^{#pm}G #mu^{#pm}G}";
  m_Strings["3Lemumugold"] = VS().a("elGmuGmuG");
  
  m_Title["3Lemumusilver"] = "#scale[1.2]{#mu^{#pm}G (e^{#pm} #mu^{#pm})S}";
  m_Strings["3Lemumusilver"] = VS().a("elGmuGmuS").a("muGmuGelS");

  m_Title["3Lemumubronze"] = "#scale[1.2]{#mu^{#pm}G (e^{#pm} #mu^{#pm})B}";
  m_Strings["3Lemumubronze"] = VS().a("elGmuGmuB").a("muGmuGelB");

  m_Title["3Lmugold"] = "#scale[1.2]{#mu^{#pm}G #mu^{#pm}G #mu^{#pm}G}";
  m_Strings["3Lmugold"] = VS().a("muGmuGmuG");

  m_Title["3Lmusilver"] = "#scale[1.2]{#mu^{#pm}G #mu^{#pm}G #mu^{#pm}S}";
  m_Strings["3Lmusilver"] = VS().a("muGmuGmuS");

  m_Title["3Lmubronze"] = "#scale[1.2]{#mu^{#pm}G #mu^{#pm}G #mu^{#pm}B}";
  m_Strings["3Lmubronze"] = VS().a("muGmuGmuB");



  // hadronic categories
  m_Title["0jS"] = "#splitline{0 jet}{incl. SVs} #scale[1.2]{#in S}";
  m_Strings["0jS"] = VS().a("0jS").a("0j0svS").a("0j1svS").a("0jge1svS").a("0jge2svS"); 
 
  m_Title["0j0svS"] = "#splitline{0 jets}{0 SV-tags} #scale[1.2]{#in S}";

  m_Title["0j1svS"] = "#splitline{0 jets}{1 SV-tag} #scale[1.2]{#in S}";

  m_Title["0jge1svS"] = "#splitline{0 jets}{#geq 1 SV-tag} #scale[1.2]{#in S}";
  m_Strings["0jge1svS"] = VS().a("0jge1svS");

  m_Title["0jge2svS"] = "#splitline{0 jets}{#geq 2 SV-tags} #scale[1.2]{#in S}";

  m_Title["1j0bge1svS"] = "#splitline{1 jet, 0 b-tags}{#geq 1 SV-tag} #scale[1.2]{#in S}";
  
  m_Title["1j0svS"] = "#splitline{1 jet}{#geq 1 SV-tag} #scale[1.2]{#in S}";
  m_Strings["1j0svS"] = VS().a("1j0b0svS").a("1j1b0svS");

  m_Title["1jge1svS"] = "#splitline{1 jet}{#geq 1 SV-tag} #scale[1.2]{#in S}";
  m_Strings["1jge1svS"] = VS().a("1jge1svS");

  m_Title["1jS"] = "#splitline{1 jet}{incl. b-tags} #scale[1.2]{#in S}";
  m_Strings["1jS"] = VS().a("1j0svS").a("1jge1svS").a("1j0b0svS").a("1j0bge1svS").a("1j1b0svS").a("1j1bge1svS");

  m_Title["2j0bS"] = "#splitline{2 jets}{0 b-tags} #scale[1.2]{#in S}";

  m_Title["2j1bS"] = "#splitline{2 jets}{1 b-tags} #scale[1.2]{#in S}";

  m_Title["2j2bS"] = "#splitline{2 jets}{2 b-tags} #scale[1.2]{#in S}";
  
  m_Title["2jS"] = "#splitline{2 jet}{incl. b-tags} #scale[1.2]{#in S}";
  m_Strings["2jS"] = VS().a("2j0bS").a("2j1bS").a("2j2bS");

  m_Title["3j0bS"] = "#splitline{3 jets}{0 b-tags} #scale[1.2]{#in S}";

  m_Title["3j1bS"] = "#splitline{3 jets}{1 b-tags} #scale[1.2]{#in S}";

  m_Title["3jge2bS"] = "#splitline{3 jets}{#geq 2 b-tags} #scale[1.2]{#in S}";
  
  m_Title["3jS"] = "#splitline{3 jet}{incl. b-tags} #scale[1.2]{#in S}";
  m_Strings["3jS"] = VS().a("3jS").a("3j0bS").a("3j1bS").a("3jge2bS");
  
  m_Title["ge1j0bISR"] = "#splitline{#geq 1 jet}{0 b-tags} #scale[1.2]{#in ISR}";

  m_Title["ge1jge1bISR"] = "#splitline{#geq 1 jet}{#geq 1 b-tags} #scale[1.2]{#in ISR}";

  m_Title["ge1jISR"] = "#splitline{#geq 1 jet}{incl. b-tags} #scale[1.2]{#in ISR}";
  m_Strings["ge1jISR"] = VS().a("ge1jISR").a("ge1j0bISR").a("ge1jge1bISR");

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
