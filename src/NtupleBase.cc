#include <TFile.h>

#include "NtupleBase.hh"
#include "StopNtupleTree.hh"

template <class Base>
NtupleBase<Base>::NtupleBase(TTree* tree)
  : AnalysisBase<Base>(tree)
{
  m_Tree = nullptr;
  m_EvtTree = nullptr;
}

template <class Base>
NtupleBase<Base>::~NtupleBase(){
  if(m_Tree)
    delete m_Tree;
  if(m_EvtTree)
    delete m_EvtTree;
}

template <class Base>
void NtupleBase<Base>::WriteNtuple(const string& filename){
  TFile* outfile = new TFile(filename.c_str(),"UPDATE");
  outfile->cd();
  InitOutputTree();

  m_EvtTree = (TTree*) new TTree("EventCount", "EventCount");
  m_EvtTree->Branch("EvtTotal", &m_EvtTotal);
  m_EvtTree->Branch("EvtSample", &m_EvtSample);
  m_EvtTree->Branch("EvtPreselection", &m_EvtPreselection);
  m_EvtTree->Branch("EvtSelected", &m_EvtSelected);

  m_EvtTotal = AnalysisBase<Base>::GetXSEC()*1000.; // xsec in fb
  m_EvtSample = 0.;
  m_EvtPreselection = 0.;
  m_EvtSelected = 0.; 

  Long64_t N = Base::fChain->GetEntries();
  for(Long64_t i = 0; i < N; i++){
    int mymod = N/10;
    if(mymod < 1)
      mymod = 1;
    if(i%mymod == 0)
      cout << " event = " << i << " : " << N << endl;
    
    AnalysisBase<Base>::GetEntry(i);
    m_EvtSample += AnalysisBase<Base>::GetEventWeight();
   
    outfile->cd();
    FillOutputTree();
  }

  m_EvtTree->Fill();
  m_EvtTree->Write("",TObject::kOverwrite);
  delete m_EvtTree;
  m_EvtTree = nullptr;

  m_Tree->Write("",TObject::kOverwrite);
  delete m_Tree;
  m_Tree = nullptr;
  outfile->Close();
  delete outfile;
}

template class NtupleBase<StopNtupleTree>;


