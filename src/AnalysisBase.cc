#include <TH1D.h>
#include <iostream>

#include "AnalysisBase.hh"
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
int AnalysisBase<Base>::GetJets(vector<TLorentzVector>& JETs, double pt_cut, double eta_cut) {
  return 0.;
}

template <class Base>
void AnalysisBase<Base>::GetLeptons(vector<TLorentzVector>& LEPs, vector<int>& IDs,
				    double pt_cut, double eta_cut) {}

template <class Base>
int AnalysisBase<Base>::GetLargeRJets(vector<TLorentzVector>& JETs, double pt_cut, double eta_cut) {
  return 0.;
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
int AnalysisBase<StopNtupleTree>::GetJets(vector<TLorentzVector>& JETs, double pt_cut, double eta_cut){

  int Njet = jetsLVec->size();
  for(int i = 0; i < Njet; i++){
    TLorentzVector JET = (*jetsLVec)[i];
    if(JET.Pt() >= pt_cut && (fabs(JET.Eta()) < eta_cut || eta_cut < 0)){
      float mass = JET.M();
      if(std::isnan(mass))
	mass = 0;
      if(std::isinf(mass))
	mass = 0;
      if(mass < 0.)
	mass = 0.;
      JET.SetPtEtaPhiM( JET.Pt(), JET.Eta(), JET.Phi(), mass );
      JETs.push_back(JET);
    }
  }
  return 0.;

}

template <>
void AnalysisBase<StopNtupleTree>::GetLeptons(vector<TLorentzVector>& LEPs, vector<int>& IDs,
					      double pt_cut, double eta_cut) {
  LEPs.clear();
  IDs.clear();
  
  int Nmu = muonsLVec->size();
  int Nel = elesLVec->size();

  for(int i = 0; i < Nmu; i++){
    // muon ID medium
    if(!muonsFlagMedium->at(i))
      continue;
    
    TLorentzVector LEP = (*muonsLVec)[i];
    if((LEP.Pt() >= pt_cut) && (fabs(LEP.Eta()) < eta_cut || eta_cut < 0)){
      LEPs.push_back(LEP);
      if(muonsCharge->at(i) < 0.)
	IDs.push_back(15);
      else
	IDs.push_back(-15);
    }
  }

  for(int i = 0; i < Nel; i++){
    // electrons ID medium
    if(!mediumElectronID->at(i))
      continue;
    
    TLorentzVector LEP = (*elesLVec)[i];
    if((LEP.Pt() >= pt_cut) && (fabs(LEP.Eta()) < eta_cut || eta_cut < 0)){
      LEPs.push_back(LEP);
      if(elesCharge->at(i) < 0.)
	IDs.push_back(13);
      else
	IDs.push_back(-13);
    }
  }
}

// template<>
// int AnalysisBase<InputTreeBase>::GetLargeRJets(vector<TLorentzVector>& JETs, double pt_cut, double eta_cut){
  
//   int Njet = ptAK8->size();
//   for(int i = 0; i < Njet; i++){
//     if((ptAK8->at(i) >= pt_cut) && (fabs(etaAK8->at(i)) < eta_cut || eta_cut < 0)){
//       TLorentzVector JET;
//       float mass = MAK8->at(i);
//       if(std::isnan(mass))
// 	mass = 0;
//       if(std::isinf(mass))
// 	mass = 0;
//       if(mass < 0.)
// 	mass = 0.;
//       JET.SetPtEtaPhiM( ptAK8->at(i), etaAK8->at(i), phiAK8->at(i), mass );
//       JETs.push_back(JET);
//     }
//   }
//   return 0.;

// }

template class AnalysisBase<StopNtupleTree>;

