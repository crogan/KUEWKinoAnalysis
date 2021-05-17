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

void FitInputBuilder::WriteFit(const string& outputroot,Systematics systs){
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

   std::cout << "writing shape systematics to ouput..." << std::endl;
   WriteShapeSysts(systs);
   std::cout << "...done" << std::endl;
  

  std::cout << "writing Categories to ouput" << std::endl;
  WriteCat();
  std::cout << "...done" << std::endl;

  m_OutFile->Close();
  delete m_OutFile;
  m_OutFile = nullptr;
}


void FitInputBuilder::WriteShapeSysts(Systematics systs){
 cout << "FitInputBuilder::WriteShapeSysts" << endl; 

 if(!m_ProcTree) return;
 //check m_Proc tree for sys in systs 
 //if exists, continue
 //if systs doesnt exist in m_Proc, add to tree
 int N = m_ProcTree->GetEntries();
 for(int i = 0; i < N; i++){
  m_ProcTree->GetEntry(i);
  Process p = m_ProcBranch.GetProcess();
  cout << "process: " << p.Name() << endl;
  //if(systs.Contains(p.Name())) continue;
  //else 
 } 

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
int N = m_ProcTree->GetEntries();
cout << "nentries in procTree: " << N << endl;
 for(int i = 0; i < N; i++){
cout << "entry #: " << i << endl;
  m_ProcTree->GetEntry(i);
  Process p = m_ProcBranch.GetProcess();
  cout << "process: " << p.Name() << endl;
  //if(systs.Contains(p.Name())) continue;
  //else 
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



void FitInputBuilder::WriteShapeVariations(ProcessList samples, const string& oFile){
  ProcessList fakeProcList;
  ProcessList fakeProcList_QCD;
  for(int i = 0; i < samples.GetN(); i++){
    cout << samples[i].Name() << endl;
    //skip QCD here - add to separate processList
     if(samples[i].Name().find("QCD") != string::npos){
           fakeProcList_QCD += samples[i];
           fakeProcList_QCD += samples[i].FakeProcess("Fakes_elf0");
           fakeProcList_QCD += samples[i].FakeProcess("Fakes_elf1");
           fakeProcList_QCD += samples[i].FakeProcess("Fakes_muf0");
           fakeProcList_QCD += samples[i].FakeProcess("Fakes_muf1");
           continue;
      }
    //non QCD backgrounds
     if(samples[i].Type() == kBkg && samples[i].Name().find("QCD") == string::npos){
     	   fakeProcList += samples[i].FakeProcess("Fakes_elf0");
    	   fakeProcList += samples[i].FakeProcess("Fakes_elf1");
           fakeProcList += samples[i].FakeProcess("Fakes_muf0");
           fakeProcList += samples[i].FakeProcess("Fakes_muf1");
      }
     }
//if there is at least 1 categoryTree for fakes
//if(CT_Fakes.size() > 0){
//cout << "# fake channels: " << CT_Fakes.size() << endl;
//        for(int f = 0; f < CT_Fakes.size(); f++){
//                        shapeTemplateTool STT(CT_Fakes[f],fakeProcList,OutFile);
//                                        STT.createTemplates();
//                                                        shapeVariationTool SVT(CT_Fakes[f],fakeProcList, OutFile);
//                                                                        SVT.doVariations();
//                                                                                }       
//                                                                                }
//                                                                                cout << "# QCD processes: " << fakeProcList_QCD.GetN() << endl;
//                                                                                cout << "# QCD channels: " << CT_QCD.size() << endl;
//                                                                                //if there is at least 1 categoryTree for QCD
//                                                                                if(CT_QCD.size() > 0){
//                                                                                        for(int f = 0; f < CT_QCD.size(); f++){
//                                                                                                        shapeTemplateTool STT(CT_QCD[f],fakeProcList_QCD,OutFile);
//                                                                                                                        STT.createTemplates();
//                                                                                                                                        shapeVariationTool SVT(CT_QCD[f],fakeProcList_QCD, OutFile);
//                                                                                                                                                        SVT.doVariations();
//                                                                                                                                                                }       
//                                                                                                                                                                }


}
