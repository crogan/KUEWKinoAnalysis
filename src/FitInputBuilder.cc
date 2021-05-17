#include <iostream>
#include <map>

#include "../include/FitInputBuilder.hh"

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


double FitInputBuilder::AddEvent(double weight, double Mperp, double RISR,
			  const Category&   cat,
			  const Process&    proc,
			  const Systematic& sys){
  
  string scat  = cat.Label()+"-"+cat.GetLabel();
  string sproc = proc.Name();
  double rlow;
  if(m_Cat.count(scat) == 0)
    m_Cat[scat] = new Category(cat);
  
  if(m_Proc.count(sproc) == 0){
    m_Proc[sproc] = new Process(proc);
  }

  if(sys.IsDefault())
   rlow =  m_Proc[sproc]->AddEvent(weight, Mperp, RISR, cat, sys, m_BookHist);
  else
    rlow = m_Proc[sproc]->AddEvent(weight, Mperp, RISR, cat, sys);
return rlow;			  
}

void FitInputBuilder::AddFakeShapeSystematics(Process proc, Systematics systs){
	if(proc.Name().find("_Fakes_") == string::npos){cout << "Adding shape systematics for fake processes only." << endl;  return;}
	Systematics list;
	string procName = proc.Name();
	int idx = procName.find("_Fakes_");
	string procSubStr = procName.substr(0,idx);
	string fakeName = procName.substr(idx).substr(7);
	string lepFlav = fakeName.substr(0,2);
	string fakeSource = fakeName.substr(2);
	for(int s = 0; s < systs.GetN(); s++){
		//match process
		if(systs[s].Label().find(procSubStr) == string::npos) continue;
		//match lepton flavor
		if(systs[s].Label().find(lepFlav) == string::npos) continue;
		//match fake source
		if(systs[s].Label().find(fakeSource) == string::npos) continue;
		list += systs[s];
	}
	if(m_Proc.count(procName) == 0) m_Proc[procName] = new Process(procName,kBkg);	
	
	m_Proc[procName]->AddShapeSysts(list);	
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



void FitInputBuilder::WriteProc(){
  if(m_ProcTree)
    delete m_ProcTree;

  m_ProcTree = new TTree("Process", "Process");
  m_ProcBranch.InitFill(m_ProcTree);
  
  auto p = m_Proc.begin();
  while(p != m_Proc.end()){
    m_ProcBranch.FillProcess(*p->second, *m_OutFile);
    Process pp = *p->second;
//    cout << "FitInputBuilder::WriteProc process name: " << pp.Name() << endl;   
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



