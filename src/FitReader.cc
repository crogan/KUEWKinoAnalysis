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
 if(m_FilePtr) m_Proc += Process("total_background",kBkg); 
 
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
 //cout << cat.GetLabel() << " " << proc.Name() << " hist integral: " << m_ProcHist_2D[proc][cat]->Integral() << endl;   
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
      //if there is no file pointer, just take histogram from original file
      if(!m_FilePtr)
	m_ProcHist[proc][cat] = (TH1D*) m_File.Get(shist.c_str());
      //if there is another file, and the histogram is data, get the info from the TGraphAsymmErrors
      else if(m_FilePtr && proc.Type() == kData){
	TGraphAsymmErrors* gr = (TGraphAsymmErrors*)m_FilePtr->Get((m_FileFold+cat.Label()+"_"+cat.GetLabel()+"/data").c_str());
	if(gr == nullptr){ 
		cout << "gr null" << endl;
		m_ProcHist[proc][cat] = (TH1D*) m_FilePtr->Get((m_FileFold+shist).c_str());
	}
     //else if TGraphAsymmErrors is found	
     else{
	double x, y;
	m_ProcHist[proc][cat] = new TH1D((m_FileFold+cat.Label()+"_"+cat.GetLabel()+"/data").c_str(),"data",(int)gr->GetN(),0.,(double)gr->GetN());
	for(int i = 0; i < gr->GetN(); i++){
	 gr->GetPoint(i,x,y);
	 m_ProcHist[proc][cat]->SetBinContent(i+1,y);
	 m_ProcHist[proc][cat]->SetBinError(i+1,gr->GetErrorY(i));
	}
	}

	}
      //else if there is another file but it's not data, just get the histogram
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

bool FitReader::IsThere(const Category&   cat,
			const Process&    proc,
			const Systematic& sys) const {
  TH1D* hist = nullptr;
  TH1D* hist2 = nullptr;
  if(!sys){
    if(m_ProcHist.count(proc) == 0)
      m_ProcHist[proc] = map<Category,TH1D*>();
    if(m_ProcHist[proc].count(cat) == 0){
      string shist = cat.Label()+"_"+cat.GetLabel()+"/"+proc.Name();
      if(proc.Type() == kData || !m_FilePtr)
	hist = (TH1D*) m_File.Get(shist.c_str());
      else
	hist = (TH1D*) m_FilePtr->Get((m_FileFold+shist).c_str());

      if(hist){
	delete hist;
	return true;
      } else
	return false;
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
       
      hist  = m_ProcHistSys[proc][sys][cat].first  = (TH1D*) m_File.Get(shistUp.c_str());
      hist2 = m_ProcHistSys[proc][sys][cat].second = (TH1D*) m_File.Get(shistDown.c_str());

      bool there = true;
      if(hist)
	delete hist;
      else
	there = false;
      if(hist2)
	delete hist2;
      else
	there = false;
      return there;
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

TCanvas* FitReader::Plot1Dstack(const VS& proc,
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
  //cat.Print();
  
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

TCanvas* FitReader::Plot2D(const VS& proc,
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
  //cat.Print();
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
 GetProcesses().Print(); 
  for(int i = 0; i < Nproc; i++){
    VS vproc;
    if(m_Strings.count(proc[i]) != 0)
      vproc = m_Strings[proc[i]];
    else
      vproc += proc[i];

    ProcessType type = kBkg;
    for(int p = 0; p < int(vproc.size()); p++){
      
      int index = GetProcesses().Find(vproc[p]);
	cout << "index: " << index << endl;
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

	cout << "filled " << cat[c].GetLabel() << " " << pp.Name() << endl;
	
	if(!hist){
	  hist = (TH2D*) GetHistogram2D(cat[c], pp)->Clone(Form("plothist_%dhis_%s_2D", i, name.c_str()));
	} else {
	  hist->Add(GetHistogram2D(cat[c], pp));
	}
      }
    }

    if(hist == nullptr || hist->Integral() == 0)
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
if(hist == NULL) return NULL; 
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
  can->SetLogz();
  can->Draw();
  can->cd();
if(hist == NULL) {cout << "null hist" << endl; return can;}  
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

TCanvas* FitReader::PlotYields(const string& can_name,
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
>>>>>>> crogan-main

  








