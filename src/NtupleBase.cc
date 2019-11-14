#include <TFile.h>

#include "NtupleBase.hh"
#include "StopNtupleTree.hh"
#include "SUSYNANOBase.hh"

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

  std::pair<int,int> masses(0,0);
  
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

    // event count bookkeeping
    if(AnalysisBase<Base>::IsSMS())
      masses = AnalysisBase<Base>::GetSUSYMasses();
    if(m_mapNevent.count(masses) == 0){
      m_masses.push_back(masses);
      m_mapNevent[masses] = 1.;
    } else {
      m_mapNevent[masses] += 1.;
    }
  }

  int Ntree = m_Trees.size();
  cout << "Ntree " << Ntree << endl;
  for(int i = 0; i < Ntree; i++){
    outfile->cd();
    m_Trees[i]->Write("",TObject::kOverwrite);
    delete m_Trees[i];
    m_Trees[i] = nullptr;
  }

  // event count tree
  outfile->cd();
  TTree* tout = (TTree*) new TTree("EventCount", "EventCount");
  
  string dataset = string(AnalysisBase<Base>::GetDataSet());
  string filetag = string(AnalysisBase<Base>::GetFileTag());
  double Nevent;
  double Nweight = 0.;
  int MP;
  int MC;
  tout->Branch("Nevent", &Nevent);
  tout->Branch("Nweight", &Nweight);
  tout->Branch("filetag", &filetag);
  tout->Branch("dataset", &dataset);
  tout->Branch("MP", &MP);
  tout->Branch("MC", &MC);
  int Nmass = m_masses.size();
  for(int i = 0; i < Nmass; i++){
    Nevent = m_mapNevent[m_masses[i]];
    MP = m_masses[i].first;
    MC = m_masses[i].second;
    tout->Fill();
  }
  tout->Write("",TObject::kOverwrite);
  delete tout;
  
  outfile->Close();
  delete outfile;

  m_Trees.clear();
  m_Label2Tree.clear();
}

template class NtupleBase<StopNtupleTree>;
template class NtupleBase<SUSYNANOBase>;


