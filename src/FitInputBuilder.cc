#include <iostream>
#include <map>

#include "FitInputBuilder.hh"

using std::cout;
using std::endl;

///////////////////////////////////////////
////////// FitInputBuilder class
///////////////////////////////////////////

FitInputBuilder::FitInputBuilder(bool extrahist){
  m_OutFile  = nullptr;
  m_ProcTree = nullptr;
  m_CatTree  = nullptr;
  m_BookHist = extrahist;
}

FitInputBuilder::~FitInputBuilder(){
  map<string,Process*>  m_Proc;
  map<string,Category*> m_Cat;  

  
  auto p = m_Proc.begin();
  while(p != m_Proc.end()){
    delete p->second;
    p++;
  }
  
  auto c = m_Cat.begin();
  while(c != m_Cat.end()){
    delete c->second;
    c++;
  }

  if(m_ProcTree)
    delete m_ProcTree;
  if(m_CatTree)
    delete m_CatTree;
}

bool FitInputBuilder::AddEvent(double weight, double Mperp, double RISR,
			       const Category&   cat,
			       const Process&    proc,
			       const Systematic& sys){
  
  string scat  = cat.Label()+"_"+cat.GetLabel();
  string sproc = proc.Name();
  
  if(m_Cat.count(scat) == 0)
    m_Cat[scat] = new Category(cat);
  
  if(m_Proc.count(sproc) == 0){
    m_Proc[sproc] = new Process(proc);
  }

  if(sys.IsDefault())
    return m_Proc[sproc]->AddEvent(weight, Mperp, RISR, cat, sys, m_BookHist);
  else
    return m_Proc[sproc]->AddEvent(weight, Mperp, RISR, cat, sys);
			  
}

const Process& FitInputBuilder::FakeProcess(const string& label){
  if(m_Proc.count(label) == 0){
    m_Proc[label] = new Process(label, kBkg);
  }

  return *m_Proc[label];
}

void FitInputBuilder::WriteFit(const string& outputroot){
  if(m_OutFile){
    if(m_OutFile->IsOpen())
      m_OutFile->Close();
    delete m_OutFile;
  }
  
  m_OutFile = new TFile(outputroot.c_str(), "RECREATE");
  if(!m_OutFile->IsOpen()){
    m_OutFile = nullptr;
    return;
  }

  std::cout << "writing Processes to output..." << std::endl;
  WriteProc();
  std::cout << "...done" << std::endl;

  std::cout << "writing Categories to output" << std::endl;
  WriteCat();
  std::cout << "...done" << std::endl;
  
  m_OutFile->Close();
  delete m_OutFile;
  m_OutFile = nullptr;
}

void FitInputBuilder::WriteProc(){
  if(m_ProcTree)
    delete m_ProcTree;

  m_ProcTree = new TTree("Process", "Process");
  m_ProcBranch.InitFill(m_ProcTree);
  
  auto p = m_Proc.begin();
  while(p != m_Proc.end()){
    m_ProcBranch.FillProcess(*p->second, *m_OutFile);
    
    p++;
  }

  if(m_OutFile){
    m_OutFile->cd();
    m_ProcTree->Write("", TObject::kOverwrite);
  }
 
  if(m_ProcTree)
    delete m_ProcTree;
  
  m_ProcTree = nullptr;
}

void FitInputBuilder::WriteCat(){
  if(m_CatTree)
    delete m_CatTree;

  m_CatTree = new TTree("Category", "Category");
  m_CatBranch.InitFill(m_CatTree);
  
  auto c = m_Cat.begin();
  while(c != m_Cat.end()){
    m_CatBranch.FillCategory(*c->second);
    
    c++;
  }

  if(m_OutFile){
    m_OutFile->cd();
    m_CatTree->Write("", TObject::kOverwrite);
  }

  if(m_CatTree)
    delete m_CatTree;

  m_CatTree = nullptr;
}


