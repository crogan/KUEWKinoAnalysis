#include <iostream>
#include <map>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TGraphErrors.h>
#include <TLine.h>

#include "CategoryTree.hh"
#include "FitInputEditor.hh"

///////////////////////////////////////////
////////// FitInputEditor class
///////////////////////////////////////////

FitInputEditor::FitInputEditor(const string& inputfile, bool procSplit)
  : FitReader(inputfile) {
  m_OutFile = nullptr;
  m_procSplit = procSplit;

  InitShapeGroups();
}

FitInputEditor::~FitInputEditor(){
  
}

void FitInputEditor::InitShapeGroups(){
  m_FakeGroups["ttbar"] = "top";
  m_FakeGroups["ST"]    = "top";

  m_FakeGroups["Wjets"] = "boson";
  m_FakeGroups["TB"]    = "boson";
  m_FakeGroups["DB"]    = "boson";
  m_FakeGroups["ZDY"]   = "boson";

  ProcessList processes = GetProcesses();
  int Nproc = processes.GetN();

  for(int p = 0; p < Nproc; p++){
    Process proc = processes[p];
    string pname = proc.Name();
    if(pname.find("_Fakes_") == string::npos ||
       pname.find("QCD") != string::npos)
      continue;

    int idx = pname.find("_Fakes_");
    string sproc = pname.substr(0,idx);
    string sfake = pname.substr(idx).substr(7);
      
    if(m_FakeGroups.count(sproc) == 0)
      continue;

    string sgroup;
   
    // order sets hierachy of assignments for Nfake > 1 (2L/3L)
    m_FakeSources.push_back("muf1");
    m_FakeSources.push_back("elf1");
    m_FakeSources.push_back("elf0");
    m_FakeSources.push_back("muf0");
    
    for(auto s : m_FakeSources){
      if(sfake.find(s) != string::npos){
	if(m_procSplit) sgroup = m_FakeGroups[sproc]+"_"+s;
	else sgroup = s;
	break;
      }
    }
    if(m_FakeGroupLists.count(sgroup) == 0)
      m_FakeGroupLists[sgroup] = ProcessList();

    m_FakeGroupLists[sgroup] += proc;
  }
}

void FitInputEditor::WriteFit(const string& OutputFile){
  if(m_OutFile){
    if(m_OutFile->IsOpen())
      m_OutFile->Close();
    delete m_OutFile;
  }
  
  m_OutFile = new TFile(OutputFile.c_str(), "RECREATE");
  if(!m_OutFile->IsOpen()){
    m_OutFile = nullptr;
    return;
  }

  std::cout << "writing Processes to ouput..." << std::endl;
  WriteProc();
  std::cout << "...done" << std::endl;

  std::cout << "writing Categories to ouput" << std::endl;
  WriteCat();
  std::cout << "...done" << std::endl;

  m_OutFile->Close();
  delete m_OutFile;
  m_OutFile = nullptr;
}

void FitInputEditor::WriteProc(){
  if(!m_OutFile)
    return;
  
  // write Process tree
  TTree* ProcTree = new TTree("Process", "Process");
  m_ProcBranch.InitFill(ProcTree);

  int Nproc = m_Proc.GetN();
  for(int p = 0; p < Nproc; p++){
    Process proc = m_Proc[p];
    m_ProcBranch.FillProcess(proc);
    if(m_ProcSys.count(proc) > 0){
      int Nsys = m_ProcSys[proc].GetN();
      for(int s = 0; s < Nsys; s++){
	m_ProcBranch.FillProcess(proc, m_ProcSys[proc][s]);
      }
    }
  }

  m_OutFile->cd();
  ProcTree->Write("", TObject::kOverwrite);
 
  if(ProcTree)
    delete ProcTree;

  // writing histograms
  int Ncat  = m_Cat.GetN();
  
  for(int c = 0; c < Ncat; c++){
    string scat = m_Cat[c].Label()+"_"+m_Cat[c].GetLabel();
    m_OutFile->cd();
    if(!m_OutFile->GetDirectory(scat.c_str()))
      m_OutFile->mkdir(scat.c_str());
    m_OutFile->cd(scat.c_str());

    for(int i = 0; i < Nproc; i++){
      Process proc = m_Proc[i];
      // write nominal histogams
      const TH1D* hist = GetHistogram(m_Cat[c], proc);
      if(hist){
	hist->Write(proc.Name().c_str(), TObject::kOverwrite);
	delete hist;
      } else{
	continue;
      }
	// write systematics histograms
      if(m_ProcSys.count(proc) > 0){
	int Nsys = m_ProcSys[proc].GetN();
	for(int s = 0; s < Nsys; s++){
	  Systematic sys = m_ProcSys[proc][s];
	  const TH1D* hist_d = GetHistogram(m_Cat[c], proc, sys.Down());
	  const TH1D* hist_u = GetHistogram(m_Cat[c], proc, sys.Up());
	  if(hist_d && hist_u){
	    hist_d->Write((proc.Name()+"_"+sys.Label()+"Down").c_str(), TObject::kOverwrite);
	    hist_u->Write((proc.Name()+"_"+sys.Label()+"Up").c_str(), TObject::kOverwrite);
	  }
	  if(hist_d)
	    delete hist_d;
	  if(hist_u)
	    delete hist_u;
	}
      }
      // write 2D histograms
      const TH2D* hist_2D = GetHistogram2D(m_Cat[c], proc);
      if(hist_2D){
	hist_2D->Write((proc.Name()+"_2D").c_str(), TObject::kOverwrite);
	delete hist_2D;
      }
    }
  }
}

void FitInputEditor::WriteCat(){
  TTree* CatTree = new TTree("Category", "Category");
  m_CatBranch.InitFill(CatTree);

  int Ncat = m_Cat.GetN();
  for(int c = 0; c < Ncat; c++){
    m_CatBranch.FillCategory(m_Cat[c]);
  }

  if(m_OutFile){
    m_OutFile->cd();
    CatTree->Write("", TObject::kOverwrite);
  }

  if(CatTree)
    delete CatTree;
}

void FitInputEditor::SmoothFakes(){
//cout << "SmoothFakes" << endl;
  CategoryTreeTool CTTool;
  vector<const CategoryTree*> catTrees;
  vector<const CategoryTree*> CT_groups;
    
  vector<CategoryTree> CTs;
  CTs.push_back(CTTool.GetCategories_Fakes1L());
  CTs.push_back(CTTool.GetCategories_Fakes2L());
  CTs.push_back(CTTool.GetCategories_Fakes3L());

  for(int i = 0; i < 3; i++){
    catTrees.clear();
    CTs[i].GetListDepth(catTrees,1);
    for(int j = 0; j < int(catTrees.size()); j++)
      CT_groups.push_back(catTrees[j]);
  }

  int NCT = CT_groups.size();

  int icat = 0;
  
  // loop through category groups
  for(int g = 0; g < NCT; g++){
    CategoryList cats = m_Cat.Filter(*CT_groups[g]);
    int Ncat = cats.GetN();
    
    icat += Ncat;
    //cout << CT_groups[g]->GetNVisible() << endl;
    //cats.Print();
    
    if(Ncat == 0)
      continue;
//cout << "group #" << g << ":" << CT_groups[g]->GetSpectroscopicLabel() << endl;
    // loop through process groups
    auto pr = m_FakeGroupLists.begin();
    while(pr != m_FakeGroupLists.end()){
      ProcessList procs = pr->second;
      int Nproc = procs.GetN();

      // loop through systematics
      int Nsys = m_Sys.GetN();
      for(int s = -1; s < Nsys; s++){
	Systematic sys = (s < 0 ? Systematic::Default() : m_Sys[s]);
	for(int ud = 0; ud < 2; ud++){
	  if(s < 0 && ud == 0)
	    continue;
	  if(ud == 0)
	    sys.Down();
	  else
	    sys.Up();
	  
	  TH1D* hist_tot = GetAddedHist(pr->first+"_"+cats[0].GetLabel()+"_"+sys.Label(), cats, procs, sys);
	  if(!hist_tot)
	    continue;
	    
	  // loop through individual categories and processes
	  for(int c = 0; c < Ncat; c++){
	    Category cat = cats[c];
//cout << "Category: " << cats[c].GetLabel() << endl;
	    for(int p = 0; p < Nproc; p++){
	      Process proc = procs[p];

	      if(!IsFilled(cat, proc, sys))
		continue;

	      string hname = pr->first+"_"+proc.Name()+"_"+cat.GetLabel()+"_"+sys.Label()+(ud < 1 ? "Dn" : "Up");
	      TH1D* hist = (TH1D*) GetHistogram(cat, proc, sys)->Clone(hname.c_str());

	      SmoothHist(hist, hist_tot);

	      // replace original histogram with "smoothed" one
	      if(!sys){
		delete m_ProcHist[proc][cat];
		m_ProcHist[proc][cat] = hist;
	      } else {
		if(sys.IsUp()){
		  delete m_ProcHistSys[proc][sys][cat].first;
		  m_ProcHistSys[proc][sys][cat].first = hist;
		} else {
		  delete m_ProcHistSys[proc][sys][cat].second;
		  m_ProcHistSys[proc][sys][cat].second = hist;
		}
	      }
	    }
	  }
	  
	  delete hist_tot;
	}
      }
      pr++;
    }
  }

  //cout << icat << " Categories found" << endl;
  
  CTs.clear();
}

void FitInputEditor::SmoothQCD(){
  CategoryTreeTool CTTool;
  vector<const CategoryTree*> catTrees;
  vector<const CategoryTree*> CT_groups;
    
  vector<CategoryTree> CTs;
  CTs.push_back(CTTool.GetCategories_QCD0L());
  CTs.push_back(CTTool.GetCategories_QCD1L());

  for(int i = 0; i < 2; i++){
    catTrees.clear();
    CTs[i].GetListDepth(catTrees,1);
    for(int j = 0; j < int(catTrees.size()); j++)
      CT_groups.push_back(catTrees[j]);
  }

  int NCT = CT_groups.size();

  ProcessList processes = GetProcesses();
  int Nproc = processes.GetN();

  ProcessList procs;
  
  for(int p = 0; p < Nproc; p++){
    Process proc = processes[p];
    string pname = proc.Name();
    if(pname.find("QCD") != string::npos)
      procs += proc;
  }

  Nproc = procs.GetN();

  // loop through category groups
  for(int g = 0; g < NCT; g++){
    CategoryList cats = m_Cat.Filter(*CT_groups[g]);
    int Ncat = cats.GetN();
    if(Ncat == 0)
      continue;

    // loop through systematics
    int Nsys = m_Sys.GetN();
    for(int s = -1; s < Nsys; s++){
      Systematic sys = (s < 0 ? Systematic::Default() : m_Sys[s]);
      for(int ud = 0; ud < 2; ud++){
	if(s < 0 && ud == 0)
	  continue;
	if(ud == 0)
	  sys.Down();
	else
	  sys.Up();
	
	TH1D* hist_tot = GetAddedHist("QCD_"+cats[0].GetLabel()+"_"+sys.Label(), cats, procs, sys);
	if(!hist_tot)
	  continue;
	
	// loop through individual categories and processes
	for(int c = 0; c < Ncat; c++){
	  Category cat = cats[c];
	  for(int p = 0; p < Nproc; p++){
	    Process proc = procs[p];
	    
	    if(!IsFilled(cat, proc, sys))
	      continue;
	    
	    string hname = "QCD_"+proc.Name()+"_"+cat.GetLabel()+"_"+sys.Label()+(ud < 1 ? "Dn" : "Up");
	    TH1D* hist = (TH1D*) GetHistogram(cat, proc, sys)->Clone(hname.c_str());
	    
	    SmoothHist(hist, hist_tot);
	    
	    // replace original histogram with "smoothed" one
	    if(!sys){
	      delete m_ProcHist[proc][cat];
	      m_ProcHist[proc][cat] = hist;
	    } else {
	      if(sys.IsUp()){
		delete m_ProcHistSys[proc][sys][cat].first;
		m_ProcHistSys[proc][sys][cat].first = hist;
	      } else {
		delete m_ProcHistSys[proc][sys][cat].second;
		m_ProcHistSys[proc][sys][cat].second = hist;
	      }
	    }
	  }
	}
	  
	delete hist_tot;
      }
    }
  }
  
  CTs.clear();
}

void FitInputEditor::SmoothHist(TH1D* hist, TH1D* hist_tot){
  double hnorm = hist->Integral();
  if(hnorm < 1e-6)
    return;

  int Nbin = hist->GetNbinsX();
  if(Nbin != hist_tot->GetNbinsX())
    return;
  
  m_hist     = (TH1D*) hist->Clone("hist_OG");
  m_hist_tot = (TH1D*) hist_tot->Clone("hist_TOT");

  // Normalize histograms to unit area
  m_hist->Scale(1./hnorm);
  m_hist_tot->Scale(1./m_hist_tot->Integral());

  // Get arrays of bin effective sqrt(N) and sort
  m_binN.clear();
  m_binN_tot.clear();
  for(int i = 0; i < Nbin; i++){
    if(m_hist->GetBinContent(i+1) > 1e-6 &&
       m_hist->GetBinError(i+1) > 0.) // use bin value
      m_binN.push_back(std::make_pair(i+1, m_hist->GetBinContent(i+1)/m_hist->GetBinError(i+1)));
    else if(m_hist_tot->GetBinContent(i+1) > 1e-6 &&
	    m_hist_tot->GetBinError(i+1) > 0.) // use total value
      m_binN_tot.push_back(std::make_pair(i+1, m_hist_tot->GetBinContent(i+1)/m_hist_tot->GetBinError(i+1)));
    else
      m_binN_tot.push_back(std::make_pair(i+1, 0.));
  }
  sortBins(m_binN);
  sortBins(m_binN_tot);
  for(int i = 0; i < int(m_binN_tot.size()); i++)
    m_binN.push_back(m_binN_tot[i]);

  // evaluate/replace bins 1-by-1
  replaceBins();

  // scale new hist back to unity
  m_hist->Scale(1./m_hist->Integral());

  // inflate errors for consistency with original
  checkErrors(m_hist, hist);
  
  // rescale histogram to original norm
  m_hist->Scale(hnorm);

  // replace bins of hist with new ones
  for(int b = 0; b < Nbin; b++){
    hist->SetBinContent(b+1, m_hist->GetBinContent(b+1));
    hist->SetBinError(b+1, m_hist->GetBinError(b+1));
  }
    
  delete m_hist;
  delete m_hist_tot;

}

void FitInputEditor::sortBins(vector<pair<int,double> >& array){
  int N = array.size();
  for(int i = 1; i < N; i++){
    pair<int,double> key_item = array[i];
    int j = i-1;
    while(j >= 0 && array[j].second < key_item.second){
      array[j+1] = array[j];
      j -= 1;
    }
    array[j+1] = key_item;
  }
}

void FitInputEditor::replaceBins(int idx, double Prem, double Prem_tot){
  //idx is index of SORTED histogram; m_binsSorted[idx].first is index of true histogram
  if(idx >= int(m_binN.size())) return;

  if(Prem < 0. || Prem_tot < 0.){
    cout << "uh oh " << Prem << " " << Prem_tot << endl;
    return;
  }
  
  int ibin = m_binN[idx].first;
  
  double bin = m_hist->GetBinContent(ibin);
  double err = m_hist->GetBinError(ibin);
  double effN = (err > 0 ? (bin/err)*(bin/err) : 0.);
  double bin_tot = m_hist_tot->GetBinContent(ibin);
  double err_tot = m_hist_tot->GetBinError(ibin);
  double effN_tot = (err_tot > 0 ? (bin_tot/err_tot)*(bin_tot/err_tot) : 0);

  double new_bin;
  double new_err;
  
  if((effN < m_minN) ||
     (bin >= Prem && bin_tot < Prem_tot)){ // replace with total

    new_bin = std::min(Prem, bin_tot*(Prem/Prem_tot));
    new_err = std::min(new_bin-1e-8, err_tot*(Prem/Prem_tot));

    m_hist->SetBinContent(ibin, new_bin);
    m_hist->SetBinError(ibin, new_err);
    
  } else { // keep value
    new_bin = bin;
    new_err = err;
  }

  Prem     -= new_bin;
  Prem_tot -= bin_tot;
  
  replaceBins(idx+1, Prem, Prem_tot);
}

void FitInputEditor::checkErrors(TH1D* hist_new, TH1D* hist_old){
  int Nbin = hist_new->GetNbinsX();
  for(int i = 0; i < Nbin; i++){
    double bin_new = hist_new->GetBinContent(i+1);
    double err_new = hist_new->GetBinError(i+1);
    double bin_old = hist_old->GetBinContent(i+1);
    double err_old = hist_old->GetBinError(i+1);

    if(err_new <= 0. || err_new)
      continue;

    double pull = (bin_new-bin_old)*(bin_new-bin_old)/(err_new*err_new + err_old*err_old);
    if(pull < 1.)
      continue;

    err_new = std::min(bin_new-1e-8, sqrt((bin_new-bin_old)*(bin_new-bin_old) - err_old*err_old));
    hist_new->SetBinError(i+1, err_new);
  }
}

void FitInputEditor::AddShapeSysFakes(bool sJetSplit){
//cout << "AddShapeSysFakes" << endl;
  CategoryTreeTool CTTool;
  vector<const CategoryTree*> CT_groups;
  
  vector<CategoryTree> CTs;
  CTs.push_back(CTTool.GetCategories_Fakes1L());
  CTs.push_back(CTTool.GetCategories_Fakes2L());
  CTs.push_back(CTTool.GetCategories_Fakes3L());

  // loop through lepton multiplicity
  for(int i = 0; i < 3; i++){
    string sgroup = Form("Fake_%dL", i+1);
//    string sgroup = "Fake_";
    CT_groups.clear();
    CTs[i].GetListDepth(CT_groups,1);

    int NCT = CT_groups.size();

    // loop through category groups
    for(int g = 0; g < NCT; g++){
      CategoryList cats = m_Cat.Filter(*CT_groups[g]);
      int Ncat = cats.GetN();
      if(Ncat == 0)
	continue;
      //cout << "group #" << g << ": " << CT_groups[g]->GetBareLabel() << endl; 
      // loop through process groups
      auto pr = m_FakeGroupLists.begin();
      while(pr != m_FakeGroupLists.end()){
	ProcessList procs = pr->second;
	int Nproc = procs.GetN();
	bool b_addSys_RISR  = false;
	bool b_addSys_Mperp = false;
	string proc_name = pr->first;
	proc_name.pop_back();
	string sys_name = proc_name+"_"+sgroup;
	if(sJetSplit) sys_name += "_"+CT_groups[g]->GetSpecLabel();
	Systematic sys_RISR(sys_name+"_RISR");
	Systematic sys_Mperp(sys_name+"_Mperp");
	//cout << "systematic: " << sys_name << endl;
	// loop through individual categories and processes
	for(int c = 0; c < Ncat; c++){
	  Category cat = cats[c];
	  for(int p = 0; p < Nproc; p++){
	    Process proc = procs[p];

	    bool b_addProcSys_RISR  = false;
	    bool b_addProcSys_Mperp = false;
	    
	    if(!IsFilled(cat, proc))
	      continue;
	    
	    string hname = "h_"+proc.Name()+"_"+cat.GetLabel()+"_"+sys_name;
	    TH1D* hist = (TH1D*) GetHistogram(cat, proc)->Clone(hname.c_str());

	    TH1D* hist_RISRUp = getVariation_RISR(hname+"_RISRUp", hist, cat, 0.05);
	    TH1D* hist_RISRDn = getVariation_RISR(hname+"_RISRDn", hist, cat, -0.05);

	    TH1D* hist_MperpUp = getVariation_Mperp(hname+"_MperpUp", hist, cat, 0.05);
	    TH1D* hist_MperpDn = getVariation_Mperp(hname+"_MperpDn", hist, cat, -0.05);

	    // add systematic histograms to editor
	    if(m_ProcHistSys.count(proc) == 0)
	      m_ProcHistSys[proc] = map<Systematic,map<Category,pair<TH1D*,TH1D*> > >();

	    if(hist_RISRUp && hist_RISRDn){
	      if(m_ProcHistSys[proc].count(sys_RISR) == 0)
		m_ProcHistSys[proc][sys_RISR] = map<Category,pair<TH1D*,TH1D*> >();
	      if(m_ProcHistSys[proc][sys_RISR].count(cat) == 0){
		m_ProcHistSys[proc][sys_RISR][cat] = pair<TH1D*,TH1D*>(hist_RISRUp, hist_RISRDn);
	      }
	      if(!b_addProcSys_RISR){
		if(m_ProcSys.count(proc) == 0)
		  m_ProcSys[proc] = Systematics();
		m_ProcSys[proc] += sys_RISR;
	      }
	      b_addSys_RISR = true;
	      b_addProcSys_RISR = true;
	    }

	    if(hist_MperpUp && hist_MperpDn){
	      if(m_ProcHistSys[proc].count(sys_Mperp) == 0)
		m_ProcHistSys[proc][sys_Mperp] = map<Category,pair<TH1D*,TH1D*> >();
	      if(m_ProcHistSys[proc][sys_Mperp].count(cat) == 0){
		m_ProcHistSys[proc][sys_Mperp][cat] = pair<TH1D*,TH1D*>(hist_MperpUp, hist_MperpDn);
	      }
	      if(!b_addProcSys_Mperp){
		if(m_ProcSys.count(proc) == 0)
		  m_ProcSys[proc] = Systematics();
		m_ProcSys[proc] += sys_Mperp;
	      }
	      b_addSys_Mperp = true;
	      b_addProcSys_Mperp = true;
	    } 
	  }
	}

	if(b_addSys_RISR){
	  m_Sys += sys_RISR;
	}
	if(b_addSys_Mperp){
	  m_Sys += sys_Mperp;
	}

	pr++;
      }   
   //cout << "\n" << endl;
    }
  }
  
  CTs.clear();
}

void FitInputEditor::AddShapeSysQCD(){
  CategoryTreeTool CTTool;
  vector<const CategoryTree*> CT_groups;
  vector<const CategoryTree*> catTrees;

  vector<CategoryTree> CTs;
  
  CTs.push_back(CTTool.GetCategories_QCD0L());
  //CTs.push_back(CTTool.GetCategories_QCD1L());
  
  ProcessList processes = GetProcesses();
  int Nproc = processes.GetN();

  ProcessList procs;
  
  for(int p = 0; p < Nproc; p++){
    Process proc = processes[p];
    string pname = proc.Name();
    if(pname.find("QCD") != string::npos)
      procs += proc;
  }

  Nproc = procs.GetN();
  if(Nproc == 0)
    return;

  for(int i = 0; i < 2; i++){
    CT_groups.clear();
    CTs[i].GetListDepth(CT_groups,0);
  //  for(int j = 0; j < int(catTrees.size()); j++)
  //    CT_groups.push_back(catTrees[j]);
   //string sgroup = Form("QCD_%dL",i+1);
   int NCT = CT_groups.size();
  // loop through category groups
  for(int g = 0; g < NCT; g++){
  bool b_addSys_RISR  = false;
  bool b_addSys_Mperp = false;
  string sys_name = "QCD_"+CT_groups[g]->GetSpecLabel();
  Systematic sys_RISR(sys_name+"_RISR");
  Systematic sys_Mperp(sys_name+"_Mperp");
  
    CategoryList cats = m_Cat.Filter(*CT_groups[g]);
    int Ncat = cats.GetN();
    if(Ncat == 0)
      continue;

    // loop through individual categories and processes
    for(int c = 0; c < Ncat; c++){
      Category cat = cats[c];
      for(int p = 0; p < Nproc; p++){
	Process proc = procs[p];

	bool b_addProcSys_RISR  = false;
	bool b_addProcSys_Mperp = false;
	    
	if(!IsFilled(cat, proc))
	  continue;
	    
	string hname = "h_"+proc.Name()+"_"+cat.GetLabel()+"_"+sys_name;
	TH1D* hist = (TH1D*) GetHistogram(cat, proc)->Clone(hname.c_str());

	TH1D* hist_RISRUp = getVariation_RISR(hname+"_RISRUp", hist, cat, 0.20);
	TH1D* hist_RISRDn = getVariation_RISR(hname+"_RISRDn", hist, cat, -0.20);

	TH1D* hist_MperpUp = getVariation_Mperp(hname+"_MperpUp", hist, cat, 0.20);
	TH1D* hist_MperpDn = getVariation_Mperp(hname+"_MperpDn", hist, cat, -0.20);

	// add systematic histograms to editor
	if(m_ProcHistSys.count(proc) == 0)
	  m_ProcHistSys[proc] = map<Systematic,map<Category,pair<TH1D*,TH1D*> > >();

	if(hist_RISRUp && hist_RISRDn){
	  if(m_ProcHistSys[proc].count(sys_RISR) == 0)
	    m_ProcHistSys[proc][sys_RISR] = map<Category,pair<TH1D*,TH1D*> >();
	  if(m_ProcHistSys[proc][sys_RISR].count(cat) == 0){
	    m_ProcHistSys[proc][sys_RISR][cat] = pair<TH1D*,TH1D*>(hist_RISRUp, hist_RISRDn);
	  }
	  if(!b_addProcSys_RISR){
	    if(m_ProcSys.count(proc) == 0)
	      m_ProcSys[proc] = Systematics();
	    m_ProcSys[proc] += sys_RISR;
	  }
	  b_addSys_RISR = true;
	  b_addProcSys_RISR = true;
	}

	if(hist_MperpUp && hist_MperpDn){
	  if(m_ProcHistSys[proc].count(sys_Mperp) == 0)
	    m_ProcHistSys[proc][sys_Mperp] = map<Category,pair<TH1D*,TH1D*> >();
	  if(m_ProcHistSys[proc][sys_Mperp].count(cat) == 0){
	    m_ProcHistSys[proc][sys_Mperp][cat] = pair<TH1D*,TH1D*>(hist_MperpUp, hist_MperpDn);
	  }
	  if(!b_addProcSys_Mperp){
	    if(m_ProcSys.count(proc) == 0)
	      m_ProcSys[proc] = Systematics();
	    m_ProcSys[proc] += sys_Mperp;
	  }
	  b_addSys_Mperp = true;
	  b_addProcSys_Mperp = true;
	} 
      
    }	
  }

  if(b_addSys_RISR){
    m_Sys += sys_RISR;
  }
  if(b_addSys_Mperp){
    m_Sys += sys_Mperp;
  }
  }
  } 
  CTs.clear();
}
void FitInputEditor::AddShapeSysWjets(){
  CategoryTreeTool CTTool;
  vector<const CategoryTree*> CT_groups;
  vector<const CategoryTree*> catTrees;

  vector<CategoryTree> CTs;
  
  CTs.push_back(CTTool.GetCategories_QCD0L());
  CTs.push_back(CTTool.GetCategories_Fakes1L());
  CTs.push_back(CTTool.GetCategories_Fakes2L());
  CTs.push_back(CTTool.GetCategories_Fakes3L());
  
  ProcessList processes = GetProcesses();
  int Nproc = processes.GetN();

  ProcessList procs;
  
  for(int p = 0; p < Nproc; p++){
    Process proc = processes[p];
    string pname = proc.Name();
    if(pname.find("Wjets") != string::npos)
      procs += proc;
  }

  Nproc = procs.GetN();
  if(Nproc == 0)
    return;

  for(int i = 0; i < 4; i++){
    CT_groups.clear();
    CTs[i].GetListDepth(CT_groups,1);
   int NCT = CT_groups.size();
   string sgroup = Form("Wjets_%dL",i);
  // loop through category groups
  for(int g = 0; g < NCT; g++){
  bool b_addSys_RISR  = false;
  bool b_addSys_Mperp = false;
  string sys_name = sgroup+"_"+CT_groups[g]->GetSpecLabel();
  Systematic sys_RISR(sys_name+"_RISR");
  Systematic sys_Mperp(sys_name+"_Mperp");
    CategoryList cats = m_Cat.Filter(*CT_groups[g]);
    int Ncat = cats.GetN();
    if(Ncat == 0)
      continue;

    // loop through individual categories and processes
    for(int c = 0; c < Ncat; c++){
      Category cat = cats[c];
      for(int p = 0; p < Nproc; p++){
	Process proc = procs[p];

	bool b_addProcSys_RISR  = false;
	bool b_addProcSys_Mperp = false;
	    
	if(!IsFilled(cat, proc))
	  continue;
	    
	string hname = "h_"+proc.Name()+"_"+cat.GetLabel()+"_"+sys_name;
	TH1D* hist = (TH1D*) GetHistogram(cat, proc)->Clone(hname.c_str());
//cout << hname << endl;
	TH1D* hist_RISRUp = getVariation_RISR(hname+"_RISRUp", hist, cat, 0.20);
	TH1D* hist_RISRDn = getVariation_RISR(hname+"_RISRDn", hist, cat, -0.20);

	TH1D* hist_MperpUp = getVariation_Mperp(hname+"_MperpUp", hist, cat, 0.20);
	TH1D* hist_MperpDn = getVariation_Mperp(hname+"_MperpDn", hist, cat, -0.20);

	// add systematic histograms to editor
	if(m_ProcHistSys.count(proc) == 0)
	  m_ProcHistSys[proc] = map<Systematic,map<Category,pair<TH1D*,TH1D*> > >();

	if(hist_RISRUp && hist_RISRDn){
	  if(m_ProcHistSys[proc].count(sys_RISR) == 0)
	    m_ProcHistSys[proc][sys_RISR] = map<Category,pair<TH1D*,TH1D*> >();
	  if(m_ProcHistSys[proc][sys_RISR].count(cat) == 0){
	    m_ProcHistSys[proc][sys_RISR][cat] = pair<TH1D*,TH1D*>(hist_RISRUp, hist_RISRDn);
	  }
	  if(!b_addProcSys_RISR){
	    if(m_ProcSys.count(proc) == 0)
	      m_ProcSys[proc] = Systematics();
	    m_ProcSys[proc] += sys_RISR;
	  }
	  b_addSys_RISR = true;
	  b_addProcSys_RISR = true;
	}

	if(hist_MperpUp && hist_MperpDn){
	  if(m_ProcHistSys[proc].count(sys_Mperp) == 0)
	    m_ProcHistSys[proc][sys_Mperp] = map<Category,pair<TH1D*,TH1D*> >();
	  if(m_ProcHistSys[proc][sys_Mperp].count(cat) == 0){
	    m_ProcHistSys[proc][sys_Mperp][cat] = pair<TH1D*,TH1D*>(hist_MperpUp, hist_MperpDn);
	  }
	  if(!b_addProcSys_Mperp){
	    if(m_ProcSys.count(proc) == 0)
	      m_ProcSys[proc] = Systematics();
	    m_ProcSys[proc] += sys_Mperp;
	  }
	  b_addSys_Mperp = true;
	  b_addProcSys_Mperp = true;
	} 
      
    }	
  }

  if(b_addSys_RISR){
    m_Sys += sys_RISR;
  }
  if(b_addSys_Mperp){
    m_Sys += sys_Mperp;
  }
  }
  } 
  CTs.clear();
}

TH1D* FitInputEditor::getVariation_RISR(string hname, TH1D* hist, const Category& cat, double varmax){
  if(!hist)
    return nullptr;
  if(varmax < -0.95)
    varmax = -0.95;
  
  const FitBin& fitBin = cat.GetFitBin();

  int NR = fitBin.NRBins();
  if(NR < 2)
    return nullptr;

  TH1D* hist_var = (TH1D*) hist->Clone(hname.c_str());
  double integral = hist->Integral();
  
  int ibin = 0;
  for(int r = 0; r < NR; r++){
    double var = 1. + 2.*varmax*(double(r)-double(NR-1)/2.)/double(NR-1);
 
    int NM = fitBin[r].NBins();
    for(int m = 0; m < NM; m++){
      hist_var->SetBinContent(ibin+1, hist->GetBinContent(ibin+1)*var);
      hist_var->SetBinError(ibin+1, hist->GetBinError(ibin+1)*var);
      ibin++;
    }
  }
  double integral_var = hist_var->Integral();
  if(integral_var > 0)
    hist_var->Scale(integral/integral_var);
  
  return hist_var;
}

TH1D* FitInputEditor::getVariation_Mperp(string hname, TH1D* hist, const Category& cat, double varmax){
  if(!hist)
    return nullptr;

  bool isvar = false;

  if(varmax < -0.95)
    varmax = -0.95;
  
  const FitBin& fitBin = cat.GetFitBin();

  int NR = fitBin.NRBins();

  TH1D* hist_var = (TH1D*) hist->Clone(hname.c_str());
  double integral = hist->Integral(); 
  int ibin = 0;
  for(int r = 0; r < NR; r++){
    int NM = fitBin[r].NBins();
    if(NM > 1)
      isvar = true;
    int im = 0;
    for(int m = 0; m < NM; m++){
      double var;
      if(NM < 2)
	var = 1.;
      else
	var = 1. + 2.*varmax*(double(im)-double(NM-1)/2.)/double(NM-1);
      hist_var->SetBinContent(ibin+1, hist->GetBinContent(ibin+1)*var);
      hist_var->SetBinError(ibin+1, hist->GetBinError(ibin+1)*var);
      ibin++;
      im++;
    }
  }
  double integral_var = hist_var->Integral();
  if(integral_var > 0) hist_var->Scale(integral/integral_var);
  if(!isvar)
    return nullptr;
  
  return hist_var;
}

void FitInputEditor::AddFakeData(){
  Process data_obs("data_obs", kData);

  int Nproc = m_Proc.GetN();
  ProcessList bkg_procs;
  for(int p = 0; p < Nproc; p++){
    if(m_Proc[p].Type() == kBkg)
      bkg_procs += m_Proc[p];
  }
  Nproc = bkg_procs.GetN();

  bool added = false;
  
  int Ncat = m_Cat.GetN();
  for(int c = 0; c < Ncat; c++){
    Category cat = m_Cat[c];
    TH1D* hist    = nullptr;
    TH2D* hist_2D = nullptr;
    for(int p = 0; p < Nproc; p++){
      Process proc = bkg_procs[p];
      if(IsFilled(cat, proc)){
	if(!hist)
	  hist = (TH1D*) GetHistogram(cat, proc)->Clone(("h_data_"+cat.GetLabel()).c_str());
	else
	  hist->Add(GetHistogram(cat, proc));
      }
      if(IsFilled2D(cat, proc)){
	if(!hist_2D)
	  hist_2D = (TH2D*) GetHistogram2D(cat, proc)->Clone(("h_data2D_"+cat.GetLabel()).c_str());
	else
	  hist_2D->Add(GetHistogram2D(cat, proc));
      }
    }

    if(hist){
      added = true;
      if(m_ProcHist.count(data_obs) == 0)
	m_ProcHist[data_obs] = map<Category,TH1D*>();
      if(m_ProcHist[data_obs].count(cat) == 0)
	m_ProcHist[data_obs][cat] = hist;
    }
    if(hist_2D){
      added = true;
      if(m_ProcHist_2D.count(data_obs) == 0)
	m_ProcHist_2D[data_obs] = map<Category,TH2D*>();
      if(m_ProcHist_2D[data_obs].count(cat) == 0)
	m_ProcHist_2D[data_obs][cat] = hist_2D;
    }
  }

  if(added)
    m_Proc += data_obs;
}

void FitInputEditor::AddEmptyData(){
  Process data_obs("data_obs", kData);
  bool added = false;
  int Nproc = m_Proc.GetN(); 
  int Ncat = m_Cat.GetN();
  for(int c = 0; c < Ncat; c++){
    Category cat = m_Cat[c];
    TH1D* hist    = nullptr;
    if(!IsThere(cat,data_obs)){
	for(int p = 0; p < Nproc; p++){
		Process proc = m_Proc[p];
		if(IsFilled(cat,proc)){ 
			hist = (TH1D*) GetHistogram(cat,proc)->Clone(("h_data_"+cat.GetLabel()).c_str());
		 	break;
		}
	}
     if(hist == nullptr) return;
	hist->SetBinContent(0,0.);
	hist->SetBinContent(hist->GetNbinsX()+1,0.);
	for(int i = 0; i < hist->GetNbinsX(); i++){ hist->SetBinContent(i+1,1e-8); hist->SetBinError(i+1,0.); }
	if(m_ProcHist.count(data_obs) == 0) 
		m_ProcHist[data_obs] = map<Category,TH1D*>();
	if(m_ProcHist[data_obs].count(cat) == 0 || !m_ProcHist[data_obs][cat]) 
		m_ProcHist[data_obs][cat] = hist;
	added = true;
    }

  }

  if(added)
    m_Proc += data_obs;
}
