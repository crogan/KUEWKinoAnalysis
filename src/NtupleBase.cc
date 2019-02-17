#include <TFile.h>

#include "NtupleBase.hh"
#include "StopNtupleTree.hh"

template <class Base>
NtupleBase<Base>::NtupleBase(TTree* tree)
  : AnalysisBase<Base>(tree)
{
  
}

template <class Base>
NtupleBase<Base>::~NtupleBase(){
  int Ntree = m_Trees.size();
  for(int i = 0; i < Ntree; i++)
    if(m_Trees[i])
      delete m_Trees[i];
}

template <class Base>
void NtupleBase<Base>::WriteNtuple(const string& filename){
  TFile* outfile = new TFile(filename.c_str(),"UPDATE");
  outfile->cd();

  string sample;
  
  Long64_t N = Base::fChain->GetEntries();
  for(Long64_t i = 0; i < N; i++){
    int mymod = N/10;
    if(mymod < 1)
      mymod = 1;
    if(i%mymod == 0)
      cout << " event = " << i << " : " << N << endl;

    sample = AnalysisBase<Base>::GetEntry(i);
        
    if(m_Label2Tree.count(sample) == 0){
      TTree* tree = InitOutputTree(sample);
      m_Trees.push_back(tree);
      m_Label2Tree[sample] = tree;
    }
   
    outfile->cd();
    FillOutputTree(m_Label2Tree[sample]);
  }

  int Ntree = m_Trees.size();
  cout << "Ntree " << Ntree << endl;
  for(int i = 0; i < Ntree; i++){
    outfile->cd();
    m_Trees[i]->Write("",TObject::kOverwrite);
    delete m_Trees[i];
    m_Trees[i] = nullptr;
  }
  
  outfile->Close();
  delete outfile;

  m_Trees.clear();
  m_Label2Tree.clear();
}

template class NtupleBase<StopNtupleTree>;


