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
  : Base(tree), m_Systematics(true)
{
  m_Nsample = 0;
  m_SampleIndex = 0;
  m_DoSMS = false;
  m_IsData = false;
  m_IsFastSim = false;

  m_CurSys = &Systematic::Default();
}

template <class Base>
AnalysisBase<Base>::~AnalysisBase() {}

template <class Base>
void AnalysisBase<Base>::SetSystematic(const Systematic& sys){
  m_CurSys = &sys;
}

template <class Base>
const Systematic& AnalysisBase<Base>::CurrentSystematic() const {
  return *m_CurSys;
}

template <class Base>
void AnalysisBase<Base>::AddSystematics(){
  m_Systematics.Add(m_SysTool.GetTreeSystematics());
}

template <class Base>
void AnalysisBase<Base>::AddJESSystematics(){
  m_Systematics.Add(m_SysTool.JESSystematics());
}

template <class Base>
void AnalysisBase<Base>::AddMETSystematics(){
  m_Systematics.Add(m_SysTool.METSystematics());
}

template <class Base>
void AnalysisBase<Base>::AddEESSystematics(){
  m_Systematics.Add(m_SysTool.EESSystematics());
}

template <class Base>
void AnalysisBase<Base>::AddMMSSystematics(){
  m_Systematics.Add(m_SysTool.MMSSystematics());
}

template <class Base>
void AnalysisBase<Base>::DoSMS(){
  m_DoSMS = true;
  m_IsData = false;
}

template <class Base>
void AnalysisBase<Base>::DoData(){
  m_IsData = true;
  m_IsFastSim = false;
  m_DoSMS = false;
}

template <class Base>
void AnalysisBase<Base>::DoFastSim(){
  m_IsFastSim = true;
  m_IsData = false;
}

template <class Base>
string AnalysisBase<Base>::GetEntry(int entry){
  if (!Base::fChain) return 0;

  Base::ClearEvent();
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
void AnalysisBase<Base>::AddFilterEffFile(const string& rootfile){
  m_NeventTool.BuildFilterEffMap(rootfile);
}

template <class Base>
void AnalysisBase<Base>::AddJSONFile(const string& jsonfile){
  m_JSONTool.BuildMap(jsonfile);
}

template <class Base>
void AnalysisBase<Base>::AddPUFolder(const string& pufold){
  m_PUTool.BuildMap(pufold);
}

template <class Base>
void AnalysisBase<Base>::AddBtagFolder(const string& btagfold){
  m_BtagSFTool.BuildMap(btagfold);
}

template <class Base>
void AnalysisBase<Base>::AddJMEFolder(const string& jmefold){
  m_JMETool.BuildMap(jmefold);
}

template <class Base>
void AnalysisBase<Base>::AddSVDiscrFile(const string& svfile){
  m_SVDiscrTool.CreateNN(svfile);
}

template <class Base>
void AnalysisBase<Base>::InitializeHistograms(vector<TH1D*>& histos){}

template <class Base>
void AnalysisBase<Base>::BookHistograms(vector<TH1D*>& histos){}

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
int AnalysisBase<Base>::GetRunNum(){
  return 0;
}

template <class Base>
int AnalysisBase<Base>::GetLumiNum(){
  return 0;
}

template <class Base>
long AnalysisBase<Base>::GetEventNum(){
  return 0;
}

template <class Base>
bool AnalysisBase<Base>::PassEventFilter(){
  return true;
}

template <class Base>
double AnalysisBase<Base>::GetEventWeight(){
  return 0;
}

template <class Base>
double AnalysisBase<Base>::GetPUWeight(int updown){
  return 0;
}

template <class Base>
double AnalysisBase<Base>::GetBtagSFWeight(const ParticleList& jets, int updown, ParticleIDType tag){
  return 0;
}

template <class Base>
int AnalysisBase<Base>::GetNPV(){
  return 0;
}

template <class Base>
int AnalysisBase<Base>::GetNPUtrue(){
  return 0;
}

template <class Base>
bool AnalysisBase<Base>::IsGoodEvent(){
  return false;
}

template <class Base>
TVector3 AnalysisBase<Base>::GetMET(){
  return TVector3(0.,0.,0.);
}

template <class Base>
TVector3 AnalysisBase<Base>::GetPV(bool& good){
  good = false;
  return TVector3();
}

template <class Base>
bool AnalysisBase<Base>::GetMETtrigger(){
  return false;
}

template <class Base>
bool AnalysisBase<Base>::GetMETHTtrigger(){
  return false;
}

template <class Base>
bool AnalysisBase<Base>::GetMETORtrigger(){
  return false;
}

template <class Base>
ParticleList AnalysisBase<Base>::GetSVs(const TVector3& PV){
  return ParticleList();
}

template <class Base>
ParticleList AnalysisBase<Base>::GetJetsMET(TVector3& MET){
  return ParticleList();
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
ParticleList AnalysisBase<Base>::GetGenElectrons(){
  return ParticleList();
}

template <class Base>
ParticleList AnalysisBase<Base>::GetGenMuons(){
  return ParticleList();
}

template <class Base>
ParticleList AnalysisBase<Base>::GetGenNeutrinos(){
  return ParticleList();
}

template <class Base>
ParticleList AnalysisBase<Base>::GetGenBosons(){
  return ParticleList();
}

template <class Base>
ParticleList AnalysisBase<Base>::GetGenSparticles(){
  return ParticleList();
}

template <class Base>
std::pair<int,int> AnalysisBase<Base>::GetSUSYMasses(){

  return std::pair<int,int>(0,0);
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
int AnalysisBase<SUSYNANOBase>::GetRunNum(){
  return run;
}

template <>
int AnalysisBase<SUSYNANOBase>::GetLumiNum(){
  return luminosityBlock;
}

template <>
long AnalysisBase<SUSYNANOBase>::GetEventNum(){
  return event;
}

template <>
bool AnalysisBase<SUSYNANOBase>::PassEventFilter(){
  int year = 2016;
  if(m_FileTag.find("17") != std::string::npos)
    year = 2017;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  if(year == 2016){
    return Flag_goodVertices &&
      (IsFastSim() ? true : Flag_globalSuperTightHalo2016Filter) &&
      Flag_HBHENoiseFilter &&
      Flag_HBHENoiseIsoFilter &&
      Flag_EcalDeadCellTriggerPrimitiveFilter &&
      Flag_BadPFMuonFilter;
  }
  if(year == 2017){
    return Flag_goodVertices &&
      (IsFastSim() ? true : Flag_globalSuperTightHalo2016Filter) &&
      Flag_HBHENoiseFilter &&
      Flag_HBHENoiseIsoFilter &&
      Flag_EcalDeadCellTriggerPrimitiveFilter &&
      Flag_BadPFMuonFilter;
  }
  if(year == 2018){
    return Flag_goodVertices &&
      (IsFastSim() ? true : Flag_globalSuperTightHalo2016Filter) &&
      Flag_HBHENoiseFilter &&
      Flag_HBHENoiseIsoFilter &&
      Flag_EcalDeadCellTriggerPrimitiveFilter &&
      Flag_BadPFMuonFilter;
  }
  
  return true;
}

template <>
int AnalysisBase<SUSYNANOBase>::GetNPV(){
  return nOtherPV+1;
}

template <>
int AnalysisBase<SUSYNANOBase>::GetNPUtrue(){
  if(!IsData())
    return Pileup_nPU;
  
  return 0;
}

template <>
bool AnalysisBase<SUSYNANOBase>::GetMETtrigger(){
  int year = 2016;
  if(m_FileTag.find("17") != std::string::npos)
    year = 2017;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  if(year == 2016)
    return (HLT_PFMET120_PFMHT120_IDTight ||
	    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight);
  if(year == 2017 ||
     year == 2018)
    return (HLT_PFMET120_PFMHT120_IDTight ||
	    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight ||
	    HLT_PFMET120_PFMHT120_IDTight_PFHT60 ||
	    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60);

  return 0;
}
  
template <>
bool AnalysisBase<SUSYNANOBase>::GetMETHTtrigger(){
  int year = 2016;
  if(m_FileTag.find("17") != std::string::npos)
    year = 2017;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  if(year == 2016)
    return (HLT_PFHT300_PFMET100 ||
	    HLT_PFHT300_PFMET110);
  if(year == 2017 ||
     year == 2018)
    return (HLT_PFHT500_PFMET100_PFMHT100_IDTight ||
	    HLT_PFHT500_PFMET110_PFMHT110_IDTight ||
	    HLT_PFHT700_PFMET85_PFMHT85_IDTight ||
	    HLT_PFHT700_PFMET95_PFMHT95_IDTight ||
	    HLT_PFHT800_PFMET75_PFMHT75_IDTight ||
	    HLT_PFHT800_PFMET85_PFMHT85_IDTight);

  return 0;
}

template <>
bool AnalysisBase<SUSYNANOBase>::GetMETORtrigger(){
  int year = 2016;
  if(m_FileTag.find("17") != std::string::npos)
    year = 2017;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  if(year == 2016)
    return (HLT_PFMETNoMu90_PFMHTNoMu90_IDTight ||
	    HLT_PFMETNoMu100_PFMHTNoMu100_IDTight ||
	    HLT_PFMETNoMu110_PFMHTNoMu110_IDTight ||
	    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight ||
	    HLT_PFMET90_PFMHT90_IDTight ||
	    HLT_PFMET100_PFMHT100_IDTight ||
	    HLT_PFMET100_PFMHT100_IDTight_BeamHaloCleaned ||
	    HLT_PFMET110_PFMHT110_IDTight ||
	    HLT_PFMET120_PFMHT120_IDTight);
  if(year == 2017 ||
     year == 2018)
    return (HLT_PFMET110_PFMHT110_IDTight ||
	    HLT_PFMET120_PFMHT120_IDTight ||
	    HLT_PFMET130_PFMHT130_IDTight ||
	    HLT_PFMET140_PFMHT140_IDTight ||
	    HLT_PFMETNoMu110_PFMHTNoMu110_IDTight ||
	    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight ||
	    HLT_PFMETNoMu130_PFMHTNoMu130_IDTight ||
	    HLT_PFMETNoMu140_PFMHTNoMu140_IDTight ||
	    HLT_PFMET120_PFMHT120_IDTight_PFHT60 ||
	    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60);

  return 0;
}

template <>
std::pair<int,int> AnalysisBase<SUSYNANOBase>::GetSUSYMasses(){
  if(!IsData()){
    int MP = 0;
    int MC = 0;
    int Ngen = nGenPart;
    for(int i = 0; i < Ngen; i++){
      int PDGID = abs(GenPart_pdgId[i]);
      if(PDGID > 1000000 && PDGID < 3000000){
	int mass = int(GenPart_mass[i]+0.5);
	if(PDGID == 1000022)
	  MC = mass;
	else
	  if(mass > MP)
	    MP = mass;
      }
    }
  
    return std::pair<int,int>(MP,MC);
  } else {
    return std::pair<int,int>(0,0);
  }
}

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
  if(IsData())
    return 1.;

  if(m_IndexToNweight[m_SampleIndex] > 0.){
    if(!m_DoSMS)
      return genWeight*m_IndexToXsec[m_SampleIndex]/m_IndexToNweight[m_SampleIndex];

    return genWeight*m_IndexToXsec[m_SampleIndex]/m_IndexToNweight[m_SampleIndex]*m_NeventTool.GetFilterEff(m_DataSet,m_FileTag,luminosityBlock);
  } else
    return 0.;
}

template <>
double AnalysisBase<SUSYNANOBase>::GetPUWeight(int updown){
  if(IsData())
    return 1.;

  int year = 2016;
  if(m_FileTag.find("17") != std::string::npos)
    year = 2017;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  return m_PUTool.GetWeight(Pileup_nPU, year, updown);
}

template <>
double AnalysisBase<SUSYNANOBase>::GetBtagSFWeight(const ParticleList& jets, int updown, ParticleIDType tag){
  if(IsData())
    return 1.;

  int year = 2016;
  if(m_FileTag.find("17") != std::string::npos)
    year = 2017;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  int Njet = jets.size();
  int iflavor;
  double EFF, SF;

  double probMC   = 1.;
  double probDATA = 1.;
  
  for(int i = 0; i < Njet; i++){
    if(abs(jets[i].PDGID()) == 5)
      iflavor = 0;
    else if(abs(jets[i].PDGID()) == 4)
      iflavor = 1;
    else
      iflavor = 2;

    EFF = m_BtagSFTool.EFF(jets[i].Pt(), year, iflavor);
    SF  = m_BtagSFTool.SF(jets[i].Pt(), year, iflavor, updown);

    if(jets[i].BtagID() >= tag){
      probMC   *= EFF;
      probDATA *= SF*EFF;
    } else {
      probMC   *= (1.-EFF);
      probDATA *= (1.-SF*EFF);
    }
  }

  if(probMC <= 0. || probDATA <= 0.)
    return 1.;

  return probDATA/probMC;
}

template <>
bool AnalysisBase<SUSYNANOBase>::IsGoodEvent(){
  return m_JSONTool.IsGood(run, luminosityBlock);
}

template <>
TVector3 AnalysisBase<SUSYNANOBase>::GetGenMET(){
  if(IsData())
    return TVector3();
  
  TVector3 vmet;
  vmet.SetPtEtaPhi(GenMET_pt,0.0,GenMET_phi);
  return vmet;
}

template <>
TVector3 AnalysisBase<SUSYNANOBase>::GetPV(bool& good){
  good = false;
  TVector3 PV(0.,0.,0.);
  
  if(PV_chi2 < 0.)
    return PV;
  
  if(PV_ndof < 5)
    return PV;
  
  if(fabs(PV_z) > 24.)
    return PV;
  
  if(PV_x*PV_x + PV_y*PV_y > 4.)
    return PV;
  
  good = true;
  PV.SetXYZ(PV_x,PV_y,PV_z);
  
  return PV;
}

template <>
void AnalysisBase<SUSYNANOBase>::InitializeHistograms(vector<TH1D*>& histos){
  // nPU
  TH1D* h_nPU = new TH1D("hist_NPU", "hist_NPU", 75, 0., 75.);
  histos.push_back(h_nPU);

  // Btag efficiencies
  vector<double> bin_edges;
  bin_edges.push_back(20.);
  bin_edges.push_back(30.);
  bin_edges.push_back(40.);
  bin_edges.push_back(50.);
  bin_edges.push_back(60.);
  bin_edges.push_back(70.);
  bin_edges.push_back(85.);
  bin_edges.push_back(100.);
  bin_edges.push_back(120.);
  bin_edges.push_back(140.);
  bin_edges.push_back(170.);
  bin_edges.push_back(200.);
  bin_edges.push_back(250.);
  bin_edges.push_back(300.);
  bin_edges.push_back(400.);
  bin_edges.push_back(600.);
  bin_edges.push_back(800.);
  bin_edges.push_back(1000.);

  TH1D* h_btag[3][2]; // [flavor][den/num]
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
      h_btag[i][j] = (TH1D*) new TH1D(Form("hist_btag_flavor%d_%s", i, (j == 0 ? "den" : "num")),
				      Form("hist_btag_flavor%d_%s", i, (j == 0 ? "den" : "num")),
				      17, &bin_edges[0]);
      histos.push_back(h_btag[i][j]);
    }
  }
}

template <>
void AnalysisBase<SUSYNANOBase>::BookHistograms(vector<TH1D*>& histos){
  int year = 2017;
  if(m_FileTag.find("16") != std::string::npos)
    year = 2016;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  int ihist = 0;

  // nPU
  histos[ihist]->Fill(GetNPUtrue());

  ihist++;

  // Btag efficiencies
  int Njet = nJet;
  for(int i = 0; i < Njet; i++){
    if(Jet_pt[i] < 20. || fabs(Jet_eta[i] > 2.4))
      continue;

    bool btag = false;
    if(year == 2016)
      if(Jet_btagDeepFlavB[i] > 0.3093)
	btag = true;
    if(year == 2017)
      if(Jet_btagDeepFlavB[i] > 0.3033)
	btag = true;
    if(year == 2018)
      if(Jet_btagDeepFlavB[i] > 0.2770)
	btag = true;

    int flavor;
    if(abs(Jet_partonFlavour[i]) == 5)
      flavor = 0;
    else if(abs(Jet_partonFlavour[i]) == 4)
      flavor = 1;
    else
      flavor = 2;

    histos[ihist+2*flavor]->Fill(Jet_pt[i]);
    if(btag) histos[ihist+2*flavor+1]->Fill(Jet_pt[i]);
  }
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetJetsMET(TVector3& MET){
  int year = 2017;
  if(m_FileTag.find("16") != std::string::npos)
    year = 2016;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;
  
  ParticleList list;
  bool passID = true;
  int Njet = nJet;

  double delta  = (CurrentSystematic().IsUp() ? 1. : -1.);
  TVector3 deltaMET(0.,0.,0.);
  bool DO_JES = false;
  if(m_SysTool.JESSystematics() == CurrentSystematic())
    DO_JES = true;
  
  for(int i = 0; i < Njet; i++){
    bool failID = false;
    if(Jet_pt[i] < 15. || fabs(Jet_eta[i]) > 5.)
      continue;
    if(Jet_jetId[i] < 3)
      continue;
    
    Particle jet;
    float mass = Jet_mass[i];
    if(std::isnan(mass))
      mass = 0;
    if(std::isinf(mass))
      mass = 0;
    if(mass < 0.)
      mass = 0.;
    jet.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i],
		     Jet_phi[i], mass);
    
    if(DO_JES){
      double uncer = m_JMETool.GetFactor(year, CurrentSystematic().Label(),
					 Jet_pt[i], Jet_eta[i]);
      
      deltaMET -= delta*uncer*jet.Vect();
      jet.SetPtEtaPhiM((1.+delta*uncer)*jet.Pt(),
		       jet.Eta(), jet.Phi(),
		       (1.+delta*uncer)*jet.M());
    }
    
    // recalibrate jets
    // double raw = 1. - Jet_rawFactor[i];
    // double L1 = m_JMETool.GetFactor(year, "L1FastJet",
    // 				     raw*Jet_pt[i], Jet_eta[i],
    // 				     Jet_area[i], fixedGridRhoFastjetAll);
    // double L2 = m_JMETool.GetFactor(year, "L2Relative",
    // 				     raw*L1*Jet_pt[i], Jet_eta[i],
    // 				     Jet_area[i], fixedGridRhoFastjetAll);

    // cout << raw << " " << L1 << " " << L2 << " " << raw*L1*L2 << endl;

    if(Jet_jetId[i] >= 3)
      jet.SetParticleID(kTight);
    else if(Jet_jetId[i] >= 2) 
      jet.SetParticleID(kMedium);
    else if(Jet_jetId[i] >= 1)
      jet.SetParticleID(kLoose);
    
    // DeepCSV tagger
    // jet.SetBtag(Jet_btagDeepB[i]);

    // DeepFlavour tagger
    jet.SetBtag(Jet_btagDeepFlavB[i]);

    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
    if(year == 2016){
      // Deep CSV
      // if(jet.Btag() > 0.8953)
      // 	jet.SetBtagID(kTight);
      // else if(jet.Btag() > 0.6321) 
      // 	jet.SetBtagID(kMedium);
      // else if(jet.Btag() > 0.2217)
      // 	jet.SetBtagID(kLoose);

      // Deep Flavor
      if(jet.Btag() > 0.7221)
	jet.SetBtagID(kTight);
      else if(jet.Btag() > 0.3093) 
	jet.SetBtagID(kMedium);
      else if(jet.Btag() > 0.0614)
	jet.SetBtagID(kLoose);
    }

    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
    if(year == 2017){
      // DeepCSV
      // if(jet.Btag() > 0.8001)
      // 	jet.SetBtagID(kTight);
      // else if(jet.Btag() > 0.4941) 
      // 	jet.SetBtagID(kMedium);
      // else if(jet.Btag() > 0.1522)
      // 	jet.SetBtagID(kLoose);

      // Deep Flavor
      if(jet.Btag() > 0.7489)
	jet.SetBtagID(kTight);
      else if(jet.Btag() > 0.3033) 
	jet.SetBtagID(kMedium);
      else if(jet.Btag() > 0.0521)
	jet.SetBtagID(kLoose);
    }

    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
    if(year == 2018){
      // DeepCSV
      // if(jet.Btag() > 0.7527)
      // 	jet.SetBtagID(kTight);
      // else if(jet.Btag() > 0.4184) 
      // 	jet.SetBtagID(kMedium);
      // else if(jet.Btag() > 0.1241)
      // 	jet.SetBtagID(kLoose);

      // DeepFlavor
      if(jet.Btag() > 0.7264)
	jet.SetBtagID(kTight);
      else if(jet.Btag() > 0.2770) 
	jet.SetBtagID(kMedium);
      else if(jet.Btag() > 0.0494)
	jet.SetBtagID(kLoose);
    }

    jet.SetPDGID( Jet_partonFlavour[i] );
      
    list.push_back(jet);
  }

  // If one jet fails jet ID, 
  if(!passID)
    return ParticleList();

    year = 2016;
  
    if(year == 2017)
      MET.SetPtEtaPhi(METFixEE2017_pt,0.0,METFixEE2017_phi);
    else
      MET.SetPtEtaPhi(MET_pt,0.0,MET_phi);

    deltaMET.SetZ(0.);
    MET += deltaMET;

    if(CurrentSystematic() == Systematic("METUncer_UnClust")){
      if(year == 2017)
	deltaMET.SetXYZ(delta*METFixEE2017_MetUnclustEnUpDeltaX,
			delta*METFixEE2017_MetUnclustEnUpDeltaY, 0.);
      else
	deltaMET.SetXYZ(delta*MET_MetUnclustEnUpDeltaX,
			delta*MET_MetUnclustEnUpDeltaY, 0.);
      MET += deltaMET;
    }
  
    return list;
}

  template <>
  TVector3 AnalysisBase<SUSYNANOBase>::GetMET(){
    TVector3 MET;
    GetJetsMET(MET);

    return MET;
  }

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetJets(){
  TVector3 dum;
  return GetJetsMET(dum);
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetElectrons(){
  int year = 2017;
  if(m_FileTag.find("16") != std::string::npos)
    year = 2016;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;

  ParticleList list;

  int N = nElectron;
  for(int i = 0; i < N; i++){
    // baseline lepton definition
    if(Electron_pt[i] < 5. || fabs(Electron_eta[i]) > 2.5)
      continue;
    // if(fabs(Electron_dxy[i]) >= 0.05 || fabs(Electron_dz[i]) >= 0.1 ||
    //    Electron_ip3d[i] >= 0.0175 || Electron_sip3d[i] >= 2.5)
    if(fabs(Electron_dxy[i]) >= 0.05 || fabs(Electron_dz[i]) >= 0.1 ||
       Electron_sip3d[i] >= 8)
      continue;
    if(Electron_pfRelIso03_all[i]*Electron_pt[i] >= 20. + 300./Electron_pt[i])
      continue;

    Particle lep;
    lep.SetPtEtaPhiM(Electron_pt[i], Electron_eta[i],
		     Electron_phi[i], std::max(Electron_mass[i],float(0.)));
    lep.SetPDGID( (Electron_charge[i] < 0. ? 11 : -11) );
    lep.SetCharge( (Electron_charge[i] < 0. ? -1 : 1) );

    lep.SetDxy(Electron_dxy[i]);
    lep.SetDxyErr(Electron_dxyErr[i]);
    lep.SetDz(Electron_dz[i]);
    lep.SetDzErr(Electron_dzErr[i]);
    lep.SetIP3D(Electron_ip3d[i]);
    lep.SetSIP3D(Electron_sip3d[i]);

    lep.SetRelIso(Electron_pfRelIso03_all[i]);
    lep.SetMiniIso(Electron_miniPFRelIso_all[i]);

    // https://twiki.cern.ch/twiki/pub/CMS/SUSLeptonSF/Run2_SUSYwp_EleCB_MVA_8Jan19.pdf
    
    // FO baseline criteria
    if(Electron_lostHits[i] == 0 && Electron_convVeto[i]){

      double mva = Electron_mvaFall17V1noIso[i];
      if(year == 2016 || year == 2018)
	mva = Electron_mvaFall17V2noIso[i];


      // convert to raw MVA output
      if(mva == -1.)
	mva = -999.;
      else if(mva == 1.)
	mva = 999.;
      else
	mva = -0.5*log((1.-mva)/(1.+mva));
      
      // FO VLoose
      if(year == 2016){ // Summer16_94X legacy
	if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	  if(lep.Pt() < 10.){
	    if(mva > -0.259)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -0.388 + 0.109*(lep.Pt() - 25.))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -0.388)
	      lep.SetParticleID(kVeryLoose);
	  }
	} else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	  if(lep.Pt() < 10.){
	    if(mva > -0.256)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -0.696 + 0.106*(lep.Pt() - 25.))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -0.696)
	      lep.SetParticleID(kVeryLoose);
	  }
	} else { // eta < 2.5
	  if(lep.Pt() < 10.){
	    if(mva > -1.630)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -1.219 + 0.148*(lep.Pt() - 25.))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -1.219)
	      lep.SetParticleID(kVeryLoose);
	  }
	}
      }

      if(year == 2017){ // Fall17_94X
	if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	  if(lep.Pt() < 10.){
	    if(mva > -0.135)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > (-0.93 + (0.043/15.)*(lep.Pt()-10.)))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -0.887)
	      lep.SetParticleID(kVeryLoose);
	  }
	} else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	  if(lep.Pt() < 10.){
	    if(mva > -0.417)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > (-0.93 + (0.04/15.)*(lep.Pt()-10.)))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -0.89)
	      lep.SetParticleID(kVeryLoose);
	  }
	} else { // eta < 2.5
	  if(lep.Pt() < 10.){
	    if(mva > -0.470)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > (-0.942 + (0.032/15.)*(lep.Pt()-10.)))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -0.91)
	      lep.SetParticleID(kVeryLoose);
	  }
	}
      }

      if(year == 2018){ // Autumn18_102X
	if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	  if(lep.Pt() < 10.){
	    if(mva > 0.053)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -0.106 + 0.062*(lep.Pt() - 25.))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -0.106)
	      lep.SetParticleID(kVeryLoose);
	  }
	} else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	  if(lep.Pt() < 10.){
	    if(mva > -0.434)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -0.769 + 0.038*(lep.Pt() - 25.))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -0.769)
	      lep.SetParticleID(kVeryLoose);
	  }
	} else { // eta < 2.5
	  if(lep.Pt() < 10.){
	    if(mva > -0.956)
	      lep.SetParticleID(kVeryLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -1.461 + 0.042*(lep.Pt() - 25.))
	      lep.SetParticleID(kVeryLoose);
	  } else {
	    if(mva > -1.461)
	      lep.SetParticleID(kVeryLoose);
	  }
	}
      }

      // VLoose electron
      if(year == 2016){ // Summer16_94X legacy
	if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	  if(lep.Pt() < 10.){
	    if(mva > 1.309)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > 0.887 + 0.088*(lep.Pt() - 25.))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > 0.887)
	      lep.SetParticleID(kLoose);
	  }
	} else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	  if(lep.Pt() < 10.){
	    if(mva > 0.373)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > 0.112 + 0.099*(lep.Pt() - 25.))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > 0.112)
	      lep.SetParticleID(kLoose);
	  }
	} else { // eta < 2.5
	  if(lep.Pt() < 10.){
	    if(mva > 0.071)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -0.017 + 0.137*(lep.Pt() - 25.))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > -0.017)
	      lep.SetParticleID(kLoose);
	  }
	}
      }

      if(year == 2017){ // Fall17_94X
	if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	  if(lep.Pt() < 10.){
	    if(mva > 0.488)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > (-0.788 + (0.148/15.)*(lep.Pt()-10.)) )
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > -0.64)
	      lep.SetParticleID(kLoose);
	  }
	} else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	  if(lep.Pt() < 10.){
	    if(mva > -0.045)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > (-0.85 + (0.075/15.)*(lep.Pt()-10.)))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > -0.775)
	      lep.SetParticleID(kLoose);
	  }
	} else { // eta < 2.5
	  if(lep.Pt() < 10.){
	    if(mva > 0.176)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > (-0.81 + (0.077/15.)*(lep.Pt()-10.)))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > -0.733)
	      lep.SetParticleID(kLoose);
	  }
	}
      }

      if(year == 2018){ // Autumn18_102X
	if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	  if(lep.Pt() < 10.){
	    if(mva > 1.320)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > 1.204 + 0.066*(lep.Pt() - 25.))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > 1.204)
	      lep.SetParticleID(kLoose);
	  }
	} else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	  if(lep.Pt() < 10.){
	    if(mva > 0.192)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > 0.084 + 0.033*(lep.Pt() - 25.))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > 0.084)
	      lep.SetParticleID(kLoose);
	  }
	} else { // eta < 2.5
	  if(lep.Pt() < 10.){
	    if(mva > 0.362)
	      lep.SetParticleID(kLoose);
	  } else if(lep.Pt() < 25.) {
	    if(mva > -0.123 + 0.053*(lep.Pt() - 25.))
	      lep.SetParticleID(kLoose);
	  } else {
	    if(mva > -0.123)
	      lep.SetParticleID(kLoose);
	  }
	}
      }
	    
      // signal lepton IDs (only Tight for now) baseline criteria
      // if(lep.IP3D() < 0.01 && lep.SIP3D() < 2.){
      if(true){
	// Tight electron
	if(year == 2016){ // Summer16_94X legacy
	  if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 1.309)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 40.) {
	      if(mva > 3.447 + 0.063*(lep.Pt()- 25.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 4.392)
		lep.SetParticleID(kTight);
	    }
	  } else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 0.373)
		lep.SetParticleID(kTight); // just changed me
	    } else if(lep.Pt() < 40.) {
	      if(mva > 2.522 + 0.058*(lep.Pt() - 25.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 3.392)
		lep.SetParticleID(kTight);
	    }
	  } else { // eta < 2.5
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 0.071)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 40.) {
	      if(mva > 1.555 + 0.075*(lep.Pt() - 25.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 2.680)
		lep.SetParticleID(kTight);
	    }
	  }
	}

	if(year == 2017){ // Fall17_94X
	  if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 0.488)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 25.) {
	      if(mva > 0.2+0.032*(lep.Pt() - 10.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 0.68)
		lep.SetParticleID(kTight);
	    }
	  } else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > -0.045)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 25.) {
	      if(mva > 0.1+0.025*(lep.Pt() - 10.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 0.475)
		lep.SetParticleID(kTight);
	    }
	  } else { // eta < 2.5
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 0.176)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 25.) {
	      if(mva > -0.1+0.028*(lep.Pt() - 10.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 0.32)
		lep.SetParticleID(kTight);
	    }
	  }
	}

	if(year == 2018){ // Autumn18_102X
	  if(fabs(lep.Eta()) < 0.8){ // eta < 0.8
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 1.320)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 25.) {
	      if(mva > 4.277 + 0.112*(lep.Pt() - 25.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 4.277)
		lep.SetParticleID(kTight);
	    }
	  } else if(fabs(lep.Eta()) < 1.479){ // eta < 1.479
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 0.192)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 25.) {
	      if(mva > 3.152 + 0.060*(lep.Pt() - 25.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 3.152)
		lep.SetParticleID(kTight);
	    }
	  } else { // eta < 2.5
	    if(lep.Pt() < 10.){ // using VLoose ID for low pT
	      if(mva > 0.362)
		lep.SetParticleID(kTight);
	    } else if(lep.Pt() < 25.) {
	      if(mva > 2.359 + 0.087*(lep.Pt() - 25.))
		lep.SetParticleID(kTight);
	    } else {
	      if(mva > 2.359)
		lep.SetParticleID(kTight);
	    }
	  }
	}
	
      }
    } // end lepton id
    
 

    list.push_back(lep);
  }

  return list;

}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetMuons(){
  int year = 2017;
  if(m_FileTag.find("16") != std::string::npos)
    year = 2016;
  if(m_FileTag.find("18") != std::string::npos)
    year = 2018;
  
  ParticleList list;

  int N = nMuon;
  for(int i = 0; i < N; i++){
    // baseline lepton definition
    if(Muon_pt[i] < 3. || fabs(Muon_eta[i]) > 2.4)
      continue;
    // if(fabs(Muon_dxy[i]) >= 0.05 || fabs(Muon_dz[i]) >= 0.1 ||
    //    Muon_ip3d[i] >= 0.0175 || Muon_sip3d[i] >= 2.5)
    if(fabs(Muon_dxy[i]) >= 0.05 || fabs(Muon_dz[i]) >= 0.1 || Muon_sip3d[i] >= 8.)
      continue;
    if(Muon_pfRelIso03_all[i]*Muon_pt[i] >= 20. + 300./Muon_pt[i])
      continue;
    
    Particle lep;
    lep.SetPtEtaPhiM(Muon_pt[i], Muon_eta[i],
		     Muon_phi[i], std::max(float(0.),Muon_mass[i]));
    lep.SetPDGID( (Muon_charge[i] < 0. ? 13 : -13) );
    lep.SetCharge( (Muon_charge[i] < 0. ? -1 : 1) );

    lep.SetDxy(Muon_dxy[i]);
    lep.SetDxyErr(Muon_dxyErr[i]);
    lep.SetDz(Muon_dz[i]);
    lep.SetDzErr(Muon_dzErr[i]);
    lep.SetIP3D(Muon_ip3d[i]);
    lep.SetSIP3D(Muon_sip3d[i]);

    lep.SetRelIso(Muon_pfRelIso03_all[i]);
    lep.SetMiniIso(Muon_miniPFRelIso_all[i]);

    // FO baseline criteria
    if(true){
      lep.SetParticleID(kLoose);

      // signal lep criteria
      //if(lep.IP3D() < 0.01 && lep.SIP3D() < 2.){
      if(true){
	if(Muon_tightId[i])
	  lep.SetParticleID(kTight);
	else if(lep.Pt() < 0.){
	  if(Muon_softId[i])
	    lep.SetParticleID(kMedium);
	} else {
	  if(Muon_mediumId[i])
	    lep.SetParticleID(kMedium);
	}
      }
    }

    list.push_back(lep);
  }

  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetSVs(const TVector3& PV){
  ParticleList list;
  
  int N = nSV;
  for(int i = 0; i < N; i++){
    if(SV_chi2[i] < 0.)
      continue;
    if(SV_pt[i] >= 20.)
      continue;
    if(fabs(SV_eta[i]) >= 2.4)
      continue;

    TVector3 xSV;
    xSV.SetXYZ(SV_x[i],SV_y[i],SV_z[i]);

    Particle SV;
    SV.SetPtEtaPhiM(SV_pt[i],SV_eta[i],SV_phi[i],SV_mass[i]);

    SV.SetDxy(fabs((xSV-PV).Pt()));
    SV.SetD3d(SV_dlen[i]);
    SV.SetD3dSig(fabs(SV_dlenSig[i]));
    SV.SetCosTheta((xSV-PV).Unit().Dot(SV.Vect().Unit()));
    SV.SetNdof(SV_ndof[i]);

    std::map<std::string, double> probs = m_SVDiscrTool.PROB(SV);

    SV.SetProbB(probs["prob_isB"]); 
    SV.SetProbC(probs["prob_isC"]); 

    // if((xSV-PV).Unit().Dot(SV.Vect().Unit()) <= 0.98)
    //   continue;
    
    // if(SB_pt[i] < 20. && SB_dlenSig[i] > 4. &&
    //    SB_dxy[i] < 3. && SB_DdotP[i] > 0.98 &&
    //    SB_ntracks[i] >= 3){

    // dxy cut
    // if(fabs((xSV-PV).Pt()) >= 3.)
    //   continue;

    // if(SV_dlenSig[i] <= 4.)
    //   continue;
    // if(SV_ndof[i] < 1.8) // replacement for ntracks cut...
    //   continue;

    if(probs["prob_isB"] > 0.35)
      list.push_back(SV);
  }
  
  return list;
}

template <>
ParticleList AnalysisBase<SUSYNANOBase>::GetGenElectrons(){
  ParticleList list;

  if(IsData())
    return list;
  
  int N = nGenPart;
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = GenPart_pdgId[i];
    if(abs(PDGID) == 11 && GenPart_pt[i] > 3. && GenPart_status[i] == 1){
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

  if(IsData())
    return list;
  
  int N = nGenPart;
  int PDGID;
  for(int i = 0; i < N; i++){
    PDGID = GenPart_pdgId[i];
    if(abs(PDGID) == 13 && GenPart_pt[i] > 2. && GenPart_status[i] == 1){
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

  if(IsData())
    return list;
  
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

  if(IsData())
    return list;
  
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

  if(IsData())
    return list;
  
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

