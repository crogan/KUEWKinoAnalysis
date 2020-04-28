#include <iostream>
#include <map>

#include "../include/FitBuilder.hh"

///////////////////////////////////////////
////////// FitBuilder class
///////////////////////////////////////////

FitBuilder::FitBuilder(){
  m_OutFile  = nullptr;
  m_ProcTree = nullptr;
  m_CatTree  = nullptr;
}

FitBuilder::~FitBuilder(){
  for(int i = 0; i < 2; i++){
    map<string,map<string,FitBin*> >::iterator p = m_Proc[i].begin();
    while(p != m_Proc[i].end()){
      map<string,FitBin*>::iterator b = p->second.begin();
      while(b != p->second.end()){
	delete b->second;
	b++;
      }
      p++;
    }
  }
  map<string,Category*>::iterator c = m_Cat.begin();
  while(c != m_Cat.end()){
    delete c->second;
    c++;
  }

  if(m_ProcTree)
    delete m_ProcTree;
  if(m_CatTree)
    delete m_CatTree;
}

void FitBuilder::AddEvent(double weight, double Mperp, double RISR,
			  const Category& cat, const string& process, bool is_signal){
  string label = cat.Label()+"_"+cat.GetLabel();

  if(m_Proc[is_signal].count(process) == 0){
    m_Proc[is_signal][process] = map<string,FitBin*>();
  }

  if(m_Cat.count(label) == 0)
    m_Cat[label] = new Category(cat);

  if(m_Proc[is_signal][process].count(label) == 0)
    m_Proc[is_signal][process][label] = cat.GetNewFitBin(process+"_"+label);

  m_Proc[is_signal][process][label]->Fill(weight, Mperp, RISR);
}

void FitBuilder::WriteFit(const string& outputroot){
  
  m_OutFile = new TFile(outputroot.c_str(), "RECREATE");
  m_OutFile->cd();
  m_OutFile->mkdir("hist2D");

  if(m_ProcTree)
    delete m_ProcTree;
  if(m_CatTree)
    delete m_CatTree;

  m_ProcTree = new TTree("Process", "Process");
  m_ProcTree->Branch("process", &m_sProc);
  m_ProcTree->Branch("cat", &m_ProcCat);
  m_ProcTree->Branch("sig", &m_ProcIsSig);

  for(int i = 0; i < 2; i++){
    // Process loop
    map<string,map<string,FitBin*> >::iterator p = m_Proc[i].begin();
    while(p != m_Proc[i].end()){
      FillProc(p->first, i);
      p++;
    }
  }

  m_OutFile->cd();
  m_ProcTree->Write("", TObject::kOverwrite);

  delete m_ProcTree;
  m_ProcTree = nullptr;
  
  m_CatTree = new TTree("Category", "Category");
  m_CatTree->Branch("cat", &m_sCat);
  m_CatTree->Branch("bin", &m_sBin);
  m_CatTree->Branch("bin_edge_x", &m_BinX);
  m_CatTree->Branch("bin_edge_y", &m_BinY);
  
  // cat loop
  map<string,Category*>::iterator c = m_Cat.begin();
  while(c != m_Cat.end()){
    FillCat(*c->second);
    c++;
  }

  m_OutFile->cd();
  m_CatTree->Write("", TObject::kOverwrite);

  delete m_CatTree;
  m_CatTree = nullptr;

  m_OutFile->Close();
  delete m_OutFile;
  m_OutFile = nullptr;
}

void FitBuilder::FillProc(const string& proc, bool is_signal){
  m_sProc = proc;
  m_ProcIsSig = is_signal;
  m_OutFile->cd();
  m_OutFile->mkdir(proc.c_str());
  m_OutFile->mkdir(("hist2D/"+proc).c_str());
  
  map<string,FitBin*>::iterator b = m_Proc[is_signal][proc].begin();
  m_ProcCat.clear();
  while(b != m_Proc[is_signal][proc].end()){
    m_ProcCat.push_back(b->first);
    b->second->WriteHistogram(proc+"_"+b->first, proc, *m_OutFile);
    b++;
  }
  m_ProcTree->Fill();
}

void FitBuilder::FillCat(const Category& cat){
  m_sCat = cat.Label();
  m_sBin = cat.GetLabel();
  m_BinX = cat.GetFitBin().GetBinEdgesX();
  m_BinY = cat.GetFitBin().GetBinEdgesY();

  m_CatTree->Fill();
}

