#include <TH1D.h>
#include <iostream>

#include "AnalysisBase.hh"
#include "ParticleList.hh"
#include "TMatrixDSym.h"
#include "TVectorD.h"

#include "StopNtupleTree.hh"
#include "SUSYNANOBase.hh"

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
void AnalysisBase<Base>::AddLabels(const string& dataset, const string& filetag){
  m_DataSet = dataset;
  m_FileTag = filetag;
}

template <class Base>
void AnalysisBase<Base>::AddEventCountFile(const string& rootfile){
  m_NeventTool.BuildMap(rootfile);
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
      m_IndexToXsec[0]    = m_XsecTool.GetXsec_BKG(m_DataSet);
      m_IndexToNevent[0]  = m_NeventTool.GetNevent_BKG(m_DataSet, m_FileTag);
      m_IndexToNweight[0] = m_NeventTool.GetNweight_BKG(m_DataSet, m_FileTag);
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
    m_IndexToSample[m_Nsample]  = std::string(Form("SMS_%d_%d", MP, MC));
    m_IndexToXsec[m_Nsample]    = m_XsecTool.GetXsec_SMS(m_DataSet, MP);
    m_IndexToNevent[m_Nsample]  = m_NeventTool.GetNevent_SMS(m_DataSet, m_FileTag, MP, MC);
    m_IndexToNweight[m_Nsample] = m_NeventTool.GetNweight_SMS(m_DataSet, m_FileTag, MP, MC);
  
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

  int Njet = jetsLVec->size();
  for(int i = 0; i < Njet; i++){
    TLorentzVector JET = (*jetsLVec)[i];
    Particle jet;
    float mass = JET.M();
    if(std::isnan(mass))
      mass = 0;
    if(std::isinf(mass))
      mass = 0;
    if(mass < 0.)
      mass = 0.;
    jet.SetPtEtaPhiM( JET.Pt(), JET.Eta(), JET.Phi(), mass );
    jet.SetBtag((*recoJetsBtag_0)[i]);

    if(jet.Btag() > 0.9535)
      jet.SetParticleID(kTight);
    else if(jet.Btag() > 0.8484) 
      jet.SetParticleID(kMedium);
    else if(jet.Btag() > 0.5426)
      jet.SetParticleID(kLoose);

    jet.SetPDGID( (*recoJetsFlavor)[i] );
      
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
    lep.SetPDGID( (elesCharge->at(i) < 0. ? 11 : -11) );
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
    lep.SetPDGID( (muonsCharge->at(i) < 0. ? 13 : -13) );
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
    if(abs(PDGID) == 11){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      int mom = genDecayMomRefVec->at(i);
      if(mom >= 0 && mom < N)
	lep.SetMomPDGID(genDecayPdgIdVec->at(mom));
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
    if(abs(PDGID) == 13){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      int mom = genDecayMomRefVec->at(i);
      if(mom >= 0 && mom < N)
	lep.SetMomPDGID(genDecayPdgIdVec->at(mom));
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
      int mom = genDecayMomRefVec->at(i);
      if(mom >= 0 && mom < N)
	lep.SetMomPDGID(genDecayPdgIdVec->at(mom));
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
      int mom = genDecayMomRefVec->at(i);
      if(mom >= 0 && mom < N)
	p.SetMomPDGID(genDecayPdgIdVec->at(mom));
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
      int mom = genDecayMomRefVec->at(i);
      if(mom >= 0 && mom < N)
	p.SetMomPDGID(genDecayPdgIdVec->at(mom));
      p.SetVectM((*genDecayLVec)[i].Vect(),(*genDecayLVec)[i].M());

      list.push_back(p);
    }
  }

  return list;
}

/////////////////////////////////////////////////
// SUSYNANOBase specific methods
/////////////////////////////////////////////////

template <>
int AnalysisBase<SUSYNANOBase>::GetSampleIndex(){
  if(!m_DoSMS){
    if(m_Nsample == 0){
      m_IndexToSample[0]  = "KUAnalysis";
      m_IndexToXsec[0]    = m_XsecTool.GetXsec_BKG(m_DataSet);
      m_IndexToNevent[0]  = m_NeventTool.GetNevent_BKG(m_DataSet, m_FileTag);
      m_IndexToNweight[0] = m_NeventTool.GetNweight_BKG(m_DataSet, m_FileTag);
      m_Nsample++;
    }
    return 0;
  }
  
  int MP = 0;
  int MC = 0;
  int Ngen = nGenPart;
  int PDGID;
  for(int i = 0; i < nGenPart; i++){
    PDGID = fabs(GenPart_pdgId[i]);
    if(PDGID > 1000000 && PDGID < 3000000){
      int mass = int(GenPart_mass[i]+0.5);
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
    m_IndexToSample[m_Nsample]  = std::string(Form("SMS_%d_%d", MP, MC));
    m_IndexToXsec[m_Nsample]    = m_XsecTool.GetXsec_SMS(m_DataSet, MP);
    m_IndexToNevent[m_Nsample]  = m_NeventTool.GetNevent_SMS(m_DataSet, m_FileTag, MP, MC);
    m_IndexToNweight[m_Nsample] = m_NeventTool.GetNweight_SMS(m_DataSet, m_FileTag, MP, MC);
  
    m_Nsample++;
  }

  return m_HashToIndex[hash];
}


template <>
double AnalysisBase<SUSYNANOBase>::GetEventWeight(){
  if(m_IndexToNweight[m_SampleIndex] > 0.)
    return genWeight*m_IndexToXsec[m_SampleIndex]/m_IndexToNweight[m_SampleIndex];
  else
    return 0.;
}

template <>
TVector3 AnalysisBase<SUSYNANOBase>::GetMET(){
  TVector3 vmet;
  vmet.SetPtEtaPhi(MET_pt,0.0,MET_phi);
  return vmet;
}

template <>
TVector3 AnalysisBase<SUSYNANOBase>::GetGenMET(){
  TVector3 vmet;
  vmet.SetPtEtaPhi(GenMET_pt,0.0,GenMET_phi);
  return vmet;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetJets(){
  ParticleList list;

  int Njet = nJet;
  for(int i = 0; i < Njet; i++){
    TLorentzVector JET;

    JET.SetPtEtaPhiM(Jet_pt[i],Jet_eta[i],Jet_phi[i],Jet_mass[i]);
    
    Particle jet;
    float mass = JET.M();
    if(std::isnan(mass))
      mass = 0;
    if(std::isinf(mass))
      mass = 0;
    if(mass < 0.)
      mass = 0.;
    jet.SetPtEtaPhiM( JET.Pt(), JET.Eta(), JET.Phi(), mass );
    jet.SetBtag(Jet_btagCSVV2[i]);

    if(jet.Btag() > 0.9535)
      jet.SetParticleID(kTight);
    else if(jet.Btag() > 0.8484) 
      jet.SetParticleID(kMedium);
    else if(jet.Btag() > 0.5426)
      jet.SetParticleID(kLoose);

    jet.SetPDGID( Jet_partonFlavour[i] );
      
    list.push_back(jet);
  }

  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetElectrons(){
  ParticleList list;

  int N = nElectron;
  for(int i = 0; i < N; i++){
    Particle lep;
    lep.SetPtEtaPhiM(Electron_pt[i], Electron_eta[i],
		     Electron_phi[i], std::max(Electron_mass[i],float(0.)));
    lep.SetPDGID( (Electron_charge[i] < 0. ? 11 : -11) );
    lep.SetCharge( (Electron_charge[i] < 0. ? -1 : 1) );
     
    if(Electron_mvaFall17V2Iso_WP90[i])
      lep.SetParticleID(kTight);
    else if(Electron_mvaFall17V2Iso_WP80[i])
      lep.SetParticleID(kMedium);
    else if(Electron_mvaFall17V2Iso_WPL[i])
      lep.SetParticleID(kLoose);
    else if(Electron_mvaFall17V2noIso_WPL[i])
      lep.SetParticleID(kVeryLoose);
     
    lep.SetRelIso(Electron_pfRelIso03_all[i]);
    lep.SetMiniIso(Electron_miniPFRelIso_all[i]);

    list.push_back(lep);
  }

  return list;

}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetMuons(){
  ParticleList list;

  int N = nMuon;
  for(int i = 0; i < N; i++){
    Particle lep;
    lep.SetPtEtaPhiM(Muon_pt[i], Muon_eta[i],
		     Muon_phi[i], std::max(float(0.),Muon_mass[i]));
    lep.SetPDGID( (Muon_charge[i] < 0. ? 13 : -13) );
    lep.SetCharge( (Muon_charge[i] < 0. ? -1 : 1) );
     
    if(Muon_tightId[i])
      lep.SetParticleID(kTight);
    else if(Muon_mediumId[i])
      lep.SetParticleID(kMedium);
    else if(Muon_triggerIdLoose[i])
      lep.SetParticleID(kLoose);
     
    lep.SetRelIso(Muon_pfRelIso03_all[i]);
    lep.SetMiniIso(Muon_miniPFRelIso_all[i]);

    list.push_back(lep);
  }

  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetGenElectrons(){
  ParticleList list;
  
  int N = nGenPart;
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = GenPart_pdgId[i];
    if(abs(PDGID) == 11){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      int mom = GenPart_genPartIdxMother[i];
      if(mom >= 0 && mom < N)
	lep.SetMomPDGID(GenPart_pdgId[mom]);
      lep.SetCharge( (PDGID > 0 ? -1 : 1) );
      lep.SetPtEtaPhiM(GenPart_pt[i], GenPart_eta[i],
		       GenPart_phi[i], max(float(0.),GenPart_mass[i]));

      list.push_back(lep);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetGenMuons(){
  ParticleList list;
  
  int N = nGenPart;
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = GenPart_pdgId[i];
    if(abs(PDGID) == 13){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      int mom = GenPart_genPartIdxMother[i];
      if(mom >= 0 && mom < N)
	lep.SetMomPDGID(GenPart_pdgId[mom]);
      lep.SetCharge( (PDGID > 0 ? -1 : 1) );
      lep.SetPtEtaPhiM(GenPart_pt[i], GenPart_eta[i],
		       GenPart_phi[i], max(float(0.),GenPart_mass[i]));

      list.push_back(lep);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetGenNeutrinos(){
  ParticleList list;
  
  int N = nGenPart;
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = GenPart_pdgId[i];
    if(abs(PDGID) == 12 || abs(PDGID) == 14 || abs(PDGID) == 16){
      Particle lep;
      
      lep.SetPDGID(PDGID);
      int mom = GenPart_genPartIdxMother[i];
      if(mom >= 0 && mom < N)
	lep.SetMomPDGID(GenPart_pdgId[mom]);
      lep.SetPtEtaPhiM(GenPart_pt[i], GenPart_eta[i],
		       GenPart_phi[i], max(float(0.),GenPart_mass[i]));

      list.push_back(lep);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetGenBosons(){
  ParticleList list;

  int N = nGenPart;
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = GenPart_pdgId[i];
    if(abs(PDGID) == 23 || abs(PDGID) == 24 || abs(PDGID) == 25){
      Particle p;
      
      p.SetPDGID(PDGID);
      int mom = GenPart_genPartIdxMother[i];
      if(mom >= 0 && mom < N)
	p.SetMomPDGID(GenPart_pdgId[mom]);
      p.SetPtEtaPhiM(GenPart_pt[i], GenPart_eta[i],
		     GenPart_phi[i], max(float(0.),GenPart_mass[i]));

      list.push_back(p);
    }
  }

  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetGenSparticles(){
  ParticleList list;

  int N = nGenPart;
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = GenPart_pdgId[i];
    if(abs(PDGID) >= 1000000 && abs(PDGID) < 3000000){
      Particle p;
      
      p.SetPDGID(PDGID);
      int mom = GenPart_genPartIdxMother[i];
      if(mom >= 0 && mom < N)
	p.SetMomPDGID(GenPart_pdgId[mom]);
      p.SetPtEtaPhiM(GenPart_pt[i], GenPart_eta[i],
		     GenPart_phi[i], max(float(0.),GenPart_mass[i]));

      list.push_back(p);
    }
  }

  return list;
}

template class AnalysisBase<StopNtupleTree>;
template class AnalysisBase<SUSYNANOBase>;

