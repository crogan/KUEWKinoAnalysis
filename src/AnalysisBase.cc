#include <TH1D.h>
#include <iostream>

#include "AnalysisBase.hh"
#include "ParticleList.hh"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "StopNtupleTree.hh"

using namespace std;

template <class Base>
AnalysisBase<Base>::AnalysisBase(TTree* tree)
  : Base(tree)
{
  m_Nsample = 0;
  m_SampleIndex = 0;
  m_DoSMS = false;
}

template <class Base>
AnalysisBase<Base>::~AnalysisBase() {}

template <class Base>
string AnalysisBase<Base>::GetEntry(int entry){
  if (!Base::fChain) return 0;
  
  Base::fChain->GetEntry(entry);
  m_SampleIndex = GetSampleIndex();

  return m_IndexToSample[m_SampleIndex];
}

template <class Base>
int AnalysisBase<Base>::GetSampleIndex(){
  if(m_Nsample == 0){
    m_IndexToSample[0] = "KUAnalysis";
    m_IndexToXsec[0] = 1.;
    m_IndexToNevent[0] = 1.;
    m_IndexToNweight[0] = 1.;
    m_Nsample++;
  }

  return 0;
}

template <class Base>
double AnalysisBase<Base>::GetXsec(){
  if(m_Nsample)
    return m_IndexToXsec[m_SampleIndex];
  else
    return 0.;
}
  
template <class Base>
void AnalysisBase<Base>::AddLabel(const string& label){
  m_Label = label;
}

template <class Base>
double AnalysisBase<Base>::DeltaPhiMin(const vector<TLorentzVector>& JETs, const TVector3& MET, int N){
  double dphimin = acos(-1);
  int Njet = JETs.size();
  for(int i = 0; i < Njet; i++){
    if(N > 0 && i >= N) break;
    if(fabs(JETs[i].Vect().DeltaPhi(MET)) < dphimin) dphimin = fabs(JETs[i].Vect().DeltaPhi(MET));
  }
  return dphimin;
}

template <class Base>
double AnalysisBase<Base>::DeltaPhiMin(const vector<pair<TLorentzVector, bool> >& JETs, const TVector3& MET, int N){
  double dphimin = acos(-1);
  int Njet = JETs.size();
  for(int i = 0; i < Njet; i++){
    if(N > 0 && i >= N) break;
    if(fabs(JETs[i].first.Vect().DeltaPhi(MET)) < dphimin) dphimin = fabs(JETs[i].first.Vect().DeltaPhi(MET));
  }
  return dphimin;
}

template <class Base>
double AnalysisBase<Base>::GetEventWeight(){
  return 0;
}

template <class Base>
TVector3 AnalysisBase<Base>::GetMET(){
  return TVector3(0.,0.,0.);
}

template <class Base>
ParticleList AnalysisBase<Base>::GetJets(){
  return ParticleList();
}

template <class Base>
ParticleList AnalysisBase<Base>::GetElectrons(){
  return ParticleList();
}

template <class Base>
ParticleList AnalysisBase<Base>::GetMuons(){
  return ParticleList();
}

template <class Base>
ParticleList GetGenElectrons(){
  return ParticleList();
}

template <class Base>
ParticleList GetGenMuons(){
  return ParticleList();
}

template <class Base>
ParticleList GetGenNeutrinos(){
  return ParticleList();
}

template <class Base>
ParticleList GetGenBosons(){
  return ParticleList();
}

template <class Base>
ParticleList GetGenSparticles(){
  return ParticleList();
}

template <class Base>
void AnalysisBase<Base>::MomTensorCalc(vector<TLorentzVector>& input, vector<double>& eigenvalues, double power, bool threeD){

  eigenvalues.clear();
  
  int N = input.size();

  if(threeD){
    if(N <= 0){
      for(int i = 0; i < 3; i++) eigenvalues.push_back(0.);
      return;
    }
    if(N == 1){
      eigenvalues.push_back(1.);
      for(int i = 0; i < 2; i++) eigenvalues.push_back(0.);
      return;
    }
    
    TMatrixDSym momTensor(3);
    momTensor.Zero();

    double norm = 0.;
    double P = 0.;
    double pnorm = 0.;
    for(int i = 0; i < N; i++){
      P = input[i].P();
      if( P > 0. ){
	norm += pow(P, power);
	pnorm = pow(P, power - 2.);
	momTensor(0,0) += pnorm*input[i].Px()*input[i].Px();
	momTensor(0,1) += pnorm*input[i].Px()*input[i].Py();
	momTensor(0,2) += pnorm*input[i].Px()*input[i].Pz();
	momTensor(1,0) += pnorm*input[i].Py()*input[i].Px();
	momTensor(1,1) += pnorm*input[i].Py()*input[i].Py();
	momTensor(1,2) += pnorm*input[i].Py()*input[i].Pz();
	momTensor(2,0) += pnorm*input[i].Pz()*input[i].Px();
	momTensor(2,1) += pnorm*input[i].Pz()*input[i].Py();
	momTensor(2,2) += pnorm*input[i].Pz()*input[i].Pz();
      }
    }
    if(norm > 0.){
      momTensor = (1./norm)*momTensor;
      TVectorD evalues(3);
      momTensor.EigenVectors(evalues);
      for(int i = 0; i < 3; i++) eigenvalues.push_back(evalues(i));
      return;
    } else {
      for(int i = 0; i < 3; i++) eigenvalues.push_back(0.);
      return;
    }

  } else { // transverse
    if(N <= 0){
      for(int i = 0; i < 2; i++) eigenvalues.push_back(0.);
      return;
    }
    if(N == 1){
      eigenvalues.push_back(1.);
      eigenvalues.push_back(0.);
      return;
    }

    TMatrixDSym momTensor(2);
    momTensor.Zero();

    double norm = 0.;
    double P = 0.;
    double pnorm = 0.;
    for(int i = 0; i < N; i++){
      P = input[i].Pt();
      if( P > 0. ){
	norm += pow(P, power);
	pnorm = pow(P, power - 2.);
	momTensor(0,0) += pnorm*input[i].Px()*input[i].Px();
	momTensor(0,1) += pnorm*input[i].Px()*input[i].Py();
	momTensor(1,0) += pnorm*input[i].Py()*input[i].Px();
	momTensor(1,1) += pnorm*input[i].Py()*input[i].Py();
      }
    }
    if(norm > 0.){
      momTensor = (1./norm)*momTensor;
      TVectorD evalues(2);
      momTensor.EigenVectors(evalues);
      for(int i = 0; i < 2; i++) eigenvalues.push_back(evalues(i));
      return;
    } else{
      for(int i = 0; i < 2; i++) eigenvalues.push_back(0.);
      return;
    }

  }
} 

/////////////////////////////////////////////////
// StopNtupleTree specific methods
/////////////////////////////////////////////////

template <>
int AnalysisBase<StopNtupleTree>::GetSampleIndex(){
  if(!m_DoSMS){
    if(m_Nsample == 0){
      m_IndexToSample[0]  = "KUAnalysis";
      m_IndexToXsec[0]    = m_XsecTool.GetXsec_BKG(m_Label);
      m_IndexToNevent[0]  = m_NeventTool.GetNevent_BKG(m_Label);
      m_IndexToNweight[0] = m_NeventTool.GetNweight_BKG(m_Label);
      m_Nsample++;
    }
    return 0;
  }
  
  int MP = 0;
  int MC = 0;
  int Ngen = genDecayPdgIdVec->size();
  int PDGID;
  for(int i = 0; i < Ngen; i++){
    PDGID = fabs(genDecayPdgIdVec->at(i));
    if(PDGID > 1000000 && PDGID < 3000000){
      int mass = int(genDecayLVec->at(i).M()+0.5);
      if(PDGID == 1000022)
	MC = mass;
      else
	if(mass > MP)
	  MP = mass;
    }
  }

  int hash = 100000*MP + MC;
  if(m_HashToIndex.count(hash) == 0){
    m_HashToIndex[hash] = m_Nsample;
    m_IndexToSample[m_Nsample]  = std::string(Form("%d_%d", MP, MC));
    m_IndexToXsec[m_Nsample]    = m_XsecTool.GetXsec_SMS(m_Label, MP);
    m_IndexToNevent[m_Nsample]  = m_NeventTool.GetNevent_SMS(m_Label, MP, MC);
    m_IndexToNweight[m_Nsample] = m_NeventTool.GetNweight_SMS(m_Label, MP, MC);
  
    m_Nsample++;
  }

  return m_HashToIndex[hash];
}


template <>
double AnalysisBase<StopNtupleTree>::GetEventWeight(){
  if(m_IndexToNweight[m_SampleIndex] > 0.)
    return (m_USEFLOAT ? evtWeight_f : evtWeight_d)*m_IndexToXsec[m_SampleIndex]/m_IndexToNweight[m_SampleIndex];
  else
    return 0.;
}

template <>
TVector3 AnalysisBase<StopNtupleTree>::GetMET(){
  TVector3 vmet;
  if(m_USEFLOAT)
    vmet.SetPtEtaPhi(met_f,0.0,metphi_f);
  else
    vmet.SetPtEtaPhi(met_d,0.0,metphi_d);
  return vmet;
}

template <>
TVector3 AnalysisBase<StopNtupleTree>::GetGenMET(){
  TVector3 vmet;
  vmet.SetPtEtaPhi(genmet,0.0,genmetphi);
  return vmet;
}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetJets(){
  ParticleList list;

  int Njet = jetsLVecLepCleaned->size();
  for(int i = 0; i < Njet; i++){
    TLorentzVector JET = (*jetsLVecLepCleaned)[i];
    Particle jet;
    float mass = JET.M();
    if(std::isnan(mass))
      mass = 0;
    if(std::isinf(mass))
      mass = 0;
    if(mass < 0.)
      mass = 0.;
    jet.SetPtEtaPhiM( JET.Pt(), JET.Eta(), JET.Phi(), mass );
    jet.SetBtag((*recoJetsBtag_0_LepCleaned)[i]);

    if(jet.Btag() > 0.9535)
      jet.SetParticleID(kTight);
    else if(jet.Btag() > 0.8484) 
      jet.SetParticleID(kMedium);
    else if(jet.Btag() > 0.5426)
      jet.SetParticleID(kLoose);
      
    list.push_back(jet);
  }

  return list;
}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetElectrons(){
  ParticleList list;

  int N = elesLVec->size();
  for(int i = 0; i < N; i++){
    Particle lep;
    lep.SetVectM((*elesLVec)[i].Vect(),std::max(0.,(*elesLVec)[i].M()));
    lep.SetPDGID( (elesCharge->at(i) < 0. ? 13 : -13) );
    lep.SetCharge( (elesCharge->at(i) < 0. ? -1 : 1) );
     
    if(tightElectronID->at(i))
      lep.SetParticleID(kTight);
    else if(mediumElectronID->at(i))
      lep.SetParticleID(kMedium);
    else if(looseElectronID->at(i))
      lep.SetParticleID(kLoose);
    else if(vetoElectronID->at(i))
      lep.SetParticleID(kVeryLoose);
     
    lep.SetRelIso(elesRelIso->at(i));
    lep.SetMiniIso(elesMiniIso->at(i));

    list.push_back(lep);
  }

  return list;

}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetMuons(){
  ParticleList list;

  int N = muonsLVec->size();
  for(int i = 0; i < N; i++){
    Particle lep;
    lep.SetVectM((*muonsLVec)[i].Vect(),std::max(0.,(*muonsLVec)[i].M()));
    lep.SetPDGID( (muonsCharge->at(i) < 0. ? 15 : -15) );
    lep.SetCharge( (muonsCharge->at(i) < 0. ? -1 : 1) );
     
    if(muonsFlagTight->at(i))
      lep.SetParticleID(kTight);
    else if(muonsFlagMedium->at(i))
      lep.SetParticleID(kMedium);
     
    lep.SetRelIso(muonsRelIso->at(i));
    lep.SetMiniIso(muonsMiniIso->at(i));

    list.push_back(lep);
  }

  return list;
}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetGenElectrons(){
  ParticleList list;
  
  int N = genDecayPdgIdVec->size();
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = genDecayPdgIdVec->at(i);
    if(abs(PDGID) == 13){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      lep.SetCharge( (PDGID > 0 ? -1 : 1) );
      lep.SetVectM((*genDecayLVec)[i].Vect(),max(0.,(*genDecayLVec)[i].M()));

      list.push_back(lep);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetGenMuons(){
  ParticleList list;
  
  int N = genDecayPdgIdVec->size();
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = genDecayPdgIdVec->at(i);
    if(abs(PDGID) == 15){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      lep.SetCharge( (PDGID > 0 ? -1 : 1) );
      lep.SetVectM((*genDecayLVec)[i].Vect(),std::max(0.,(*genDecayLVec)[i].M()));

      list.push_back(lep);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetGenNeutrinos(){
  ParticleList list;
  
  int N = genDecayPdgIdVec->size();
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = genDecayPdgIdVec->at(i);
    if(abs(PDGID) == 12 || abs(PDGID) == 14 || abs(PDGID) == 16){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      lep.SetVectM((*genDecayLVec)[i].Vect(),(*genDecayLVec)[i].M());

      list.push_back(lep);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetGenBosons(){
  ParticleList list;
  
  int N = genDecayPdgIdVec->size();
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = genDecayPdgIdVec->at(i);
    if(abs(PDGID) == 23 || abs(PDGID) == 24 || abs(PDGID) == 25){
      Particle p;
      
      p.SetPDGID(PDGID);
      p.SetVectM((*genDecayLVec)[i].Vect(),(*genDecayLVec)[i].M());

      list.push_back(p);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<StopNtupleTree>::GetGenSparticles(){
  ParticleList list;
  
  int N = genDecayPdgIdVec->size();
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = genDecayPdgIdVec->at(i);
    if(abs(PDGID) >= 1000000 && abs(PDGID) < 3000000){
      Particle p;
      
      p.SetPDGID(PDGID);
      p.SetVectM((*genDecayLVec)[i].Vect(),(*genDecayLVec)[i].M());

      list.push_back(p);
    }
  }

  return list;
}

template class AnalysisBase<StopNtupleTree>;

