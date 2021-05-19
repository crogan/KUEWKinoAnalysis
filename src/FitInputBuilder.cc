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

void FitInputBuilder::AddEvent(double weight, double Mperp, double RISR,
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
ProcessList FitInputBuilder::MakeFakeProcesses(Process proc){
	ProcessList fakeProcList;
	if(proc.Name().find("_Fakes_") == string::npos && proc.Name().find("QCD") == string::npos){
	fakeProcList += proc.FakeProcess("Fakes_elf0");
        fakeProcList += proc.FakeProcess("Fakes_elf1");
        fakeProcList += proc.FakeProcess("Fakes_muf0");
        fakeProcList += proc.FakeProcess("Fakes_muf1");			
	}
	else if(proc.Name().find("_Fakes_") == string::npos && proc.Name().find("QCD") != string::npos){
	fakeProcList += proc;
	fakeProcList += proc.FakeProcess("Fakes_elf0");
        fakeProcList += proc.FakeProcess("Fakes_elf1");
        fakeProcList += proc.FakeProcess("Fakes_muf0");
        fakeProcList += proc.FakeProcess("Fakes_muf1");			
	}
	for(int p = 0; p < fakeProcList.GetN(); p++){
		if(m_Proc.count(fakeProcList[p].Name()) == 0) m_Proc[fakeProcList[p].Name()] = new Process(fakeProcList[p].Name(),kBkg);	
	}
	return fakeProcList;
}
void FitInputBuilder::AddFakeShapeSystematics(Process proc, Systematics systs){
	ProcessList fakeProcs;
	if(proc.Name().find("_Fakes_") == string::npos || proc.Name().find("QCD") != string::npos){
		fakeProcs += MakeFakeProcesses(proc);
	}	
	else fakeProcs += proc;
	for(int p = 0; p < fakeProcs.GetN(); p++){
//		cout << "AddFakeShapeSystematics process: " << fakeProcs[p].Name() << endl;
		Process fakeProc = fakeProcs[p];
		string procName = fakeProc.Name();
		Systematics list;
		if(procName.find("QCD") != string::npos){
			for(int s = 0; s < systs.GetN(); s++){
				//match process
				if(systs[s].Label().find("QCD") == string::npos) continue;

				list += systs[s];
			}


		}
		else{
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
		}
//cout << "add fake systematics to m_proc, proc: " << procName << endl;
		m_Proc[procName]->AddShapeSysts(list);	
	}
	
}



void FitInputBuilder::WriteFakeShapeSysts(TFile* OutFile){
	vector<string> sprocs;
	ProcessList fakeProcList;
	string fname = OutFile->GetName();
	auto p = m_Proc.begin();
  	while(p != m_Proc.end()){
		string procName = p->first;
		if(procName.find("_Fakes_") != string::npos && procName.find("QCD") == string::npos){ //no QCD
 //   		cout << "FitInputBuilder::WriteFakeShapeSysts process name: " << p->first << endl;
			fakeProcList += *p->second;
			sprocs.push_back(p->first);
		}
		p++;
  	}
//	cout << "# fake channels: " << m_CT_Fakes.size() << endl;
//cout << "writefakeshapesyts # fake procs: " << fakeProcList.GetN() << endl;
	if(m_CT_Fakes.size() > 0 && fakeProcList.GetN() > 0){
	        for(int f = 0; f < m_CT_Fakes.size(); f++){
	                shapeTemplateTool STT(m_CT_Fakes[f],fakeProcList,OutFile);
	                STT.createTemplates();
	                //shapeVariationTool SVT(m_CT_Fakes[f],fakeProcList, fname);
	                shapeVariationTool SVT(m_CT_Fakes[f],fakeProcList, OutFile);
	                SVT.doVariations();
	        }       
	}
	else { cout << "Fakes CategoryTree empty." << endl; }

}


void FitInputBuilder::WriteQCDShapeSysts(TFile* OutFile){
	vector<string> sprocs;
	ProcessList fakeProcList;
	string fname = OutFile->GetName();
	auto p = m_Proc.begin();
  	while(p != m_Proc.end()){
		string procName = p->first;
		if(procName.find("QCD") != string::npos){
			fakeProcList += *p->second;
			sprocs.push_back(p->first);
		}
		p++;
  	}
	if(m_CT_QCD.size() > 0){
	        for(int f = 0; f < m_CT_QCD.size(); f++){
	                shapeTemplateTool STT(m_CT_QCD[f],fakeProcList,OutFile);
	                STT.createTemplates();
	                shapeVariationTool SVT(m_CT_QCD[f],fakeProcList, OutFile);
	                SVT.doVariations();
	        }       
	}
	else { cout << "QCD CategoryTree empty." << endl; }

}


void FitInputBuilder::SetFakeCategories(vector<CategoryTree> CTs){
	m_CT_Fakes = CTs;
}

void FitInputBuilder::SetQCDCategories(vector<CategoryTree> CTs){
	m_CT_QCD = CTs;
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



