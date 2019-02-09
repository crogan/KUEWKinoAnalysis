#include <TH1D.h>
#include <iostream>

#include "AnalysisBase.hh"
#include "StopNtupleTree.hh"
#include "TMatrixDSym.h"
#include "TVectorD.h"

using namespace std;

template <class Base>
AnalysisBase<Base>::AnalysisBase(TTree* tree)
  : Base(tree)
{
  m_CurrentFile = -1;
  m_DSID = -1;
  m_Nevent = 1.;
  m_Label  = "";
  m_XSEC = 0.;
  InitMaps();
}

template <class Base>
AnalysisBase<Base>::~AnalysisBase() {}

template <class Base>
Int_t AnalysisBase<Base>::GetEntry(Long64_t entry){
  if (!Base::fChain) return 0;
  
  Int_t ret = Base::fChain->GetEntry(entry);
 
  // if(Base::fChain->GetTreeNumber() != m_CurrentFile)
  //   NewFile();

  return ret;
}

template <class Base>
void AnalysisBase<Base>::AddLabel(string& label){
  m_Label = label;
  m_XSEC = m_IDtoXSEC[m_Label];
  return;
}

template <class Base>
void AnalysisBase<Base>::AddNevent(double nevt){
  m_Nevent = nevt;
  return;
}

template <class Base>
void AnalysisBase<Base>::NewFile(){
  // m_CurrentFile = Base::fChain->GetTreeNumber();

  // TFile* F = ((TChain*)Base::fChain)->GetFile();
  // if(!F) return;

  // char *p, *q;
  // char fname[256];
  // sprintf(fname,"%s",F->GetName());
  // p = strtok(fname, "/");

  // q = p;
  // while(p){
  //   q = p;
  //   p = strtok(NULL,"/");
  // }
  // m_DSID = atoi(strtok(q,"."));

  // TH1D* h_counter = (TH1D*) F->Get("Counter_JobBookeeping_JobBookeeping");
  // int nevt_wgt = h_counter->GetBinContent(2);
  // m_IDtoNEVT[m_DSID] = nevt_wgt;

  // cout << "Initialized file " << F->GetName() << ": ";
  // cout << "   DSID   = " << m_DSID << endl;
  // cout << "   XSEC   = " << m_IDtoXSEC[m_DSID] << endl;
  // cout << "   Nevt^W = " << m_IDtoNEVT[m_DSID] << endl;
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
void AnalysisBase<Base>::InitMaps() {}

template <class Base>
void AnalysisBase<Base>::InitXSECmap() {}

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
double AnalysisBase<StopNtupleTree>::GetEventWeight(){
  return evtWeight;
}

template <>
TVector3 AnalysisBase<StopNtupleTree>::GetMET(){
  TVector3 vmet;
  vmet.SetPtEtaPhi(met,0.0,metphi);
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
    if(!elesFlagMedium->at(i))
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

template <>
void AnalysisBase<StopNtupleTree>::InitMaps(){

  m_Label2Nevent["DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 10607207;
  m_Label2Nweight["DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 10607207;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 10607207;

  m_Label2Nevent["DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 596079;
  m_Label2Nweight["DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 596079;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 596079;

  m_Label2Nevent["DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9555206;
  m_Label2Nweight["DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9555206;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9555206;

  m_Label2Nevent["DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 399492;
  m_Label2Nweight["DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 399492;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 399492;

  m_Label2Nevent["DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9307066;
  m_Label2Nweight["DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9307066;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9307066;

  m_Label2Nevent["DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 8292957;
  m_Label2Nweight["DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 8292957;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 8292957;

  m_Label2Nevent["DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9616188;
  m_Label2Nweight["DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9616188;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 9616188;

  m_Label2Nevent["DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 2668730;
  m_Label2Nweight["DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 2668730;
  m_Label2Nabsweight["DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 2668730;

  m_Label2Nevent["DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 538840;
  m_Label2Nweight["DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 538840;
  m_Label2Nabsweight["DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 538840;

  m_Label2Nevent["GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 28519627;
  m_Label2Nweight["GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 28519627;
  m_Label2Nabsweight["GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 28519627;

  m_Label2Nevent["GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 97114677;
  m_Label2Nweight["GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 97114677;
  m_Label2Nabsweight["GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 97114677;

  m_Label2Nevent["GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 11556013;
  m_Label2Nweight["GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 11556013;
  m_Label2Nabsweight["GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 11556013;

  m_Label2Nevent["GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 11412985;
  m_Label2Nweight["GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 11412985;
  m_Label2Nabsweight["GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 11412985;

  m_Label2Nevent["QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8"] = 35819814;
  m_Label2Nweight["QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8"] = 35819814;
  m_Label2Nabsweight["QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8"] = 35819814;

  m_Label2Nevent["QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8"] = 9981655;
  m_Label2Nweight["QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8"] = 9981655;
  m_Label2Nabsweight["QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8"] = 9981655;

  m_Label2Nevent["QCD_Pt_120to170_TuneCP5_13TeV_pythia8"] = 29854280;
  m_Label2Nweight["QCD_Pt_120to170_TuneCP5_13TeV_pythia8"] = 29854280;
  m_Label2Nabsweight["QCD_Pt_120to170_TuneCP5_13TeV_pythia8"] = 29854280;

  m_Label2Nevent["QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8"] = 12457308;
  m_Label2Nweight["QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8"] = 12457308;
  m_Label2Nabsweight["QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8"] = 12457308;

  m_Label2Nevent["QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8"] = 11353270;
  m_Label2Nweight["QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8"] = 11353270;
  m_Label2Nabsweight["QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8"] = 11353270;

  m_Label2Nevent["QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8"] = 2963737;
  m_Label2Nweight["QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8"] = 2963737;
  m_Label2Nabsweight["QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8"] = 2963737;

  m_Label2Nevent["QCD_Pt_15to30_TuneCP5_13TeV_pythia8"] = 39155193;
  m_Label2Nweight["QCD_Pt_15to30_TuneCP5_13TeV_pythia8"] = 39155193;
  m_Label2Nabsweight["QCD_Pt_15to30_TuneCP5_13TeV_pythia8"] = 39155193;

  m_Label2Nevent["QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8"] = 39981175;
  m_Label2Nweight["QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8"] = 39981175;
  m_Label2Nabsweight["QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8"] = 39981175;

  m_Label2Nevent["QCD_Pt_170to300_TuneCP5_13TeV_pythia8"] = 56298920;
  m_Label2Nweight["QCD_Pt_170to300_TuneCP5_13TeV_pythia8"] = 56298920;
  m_Label2Nabsweight["QCD_Pt_170to300_TuneCP5_13TeV_pythia8"] = 56298920;

  m_Label2Nevent["QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8"] = 14796774;
  m_Label2Nweight["QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8"] = 14796774;
  m_Label2Nabsweight["QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8"] = 14796774;

  m_Label2Nevent["QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8"] = 2923941;
  m_Label2Nweight["QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8"] = 2923941;
  m_Label2Nabsweight["QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8"] = 2923941;

  m_Label2Nevent["QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8"] = 1949724;
  m_Label2Nweight["QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8"] = 1949724;
  m_Label2Nabsweight["QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8"] = 1949724;

  m_Label2Nevent["QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8"] = 1910526;
  m_Label2Nweight["QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8"] = 1910526;
  m_Label2Nabsweight["QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8"] = 1910526;

  m_Label2Nevent["QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8"] = 399226;
  m_Label2Nweight["QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8"] = 399226;
  m_Label2Nabsweight["QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8"] = 399226;

  m_Label2Nevent["QCD_Pt_300to470_TuneCP5_13TeV_pythia8"] = 111229780;
  m_Label2Nweight["QCD_Pt_300to470_TuneCP5_13TeV_pythia8"] = 111229780;
  m_Label2Nabsweight["QCD_Pt_300to470_TuneCP5_13TeV_pythia8"] = 111229780;

  m_Label2Nevent["QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8"] = 22403620;
  m_Label2Nweight["QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8"] = 22403620;
  m_Label2Nabsweight["QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8"] = 22403620;

  m_Label2Nevent["QCD_Pt_30to50_TuneCP5_13TeV_pythia8"] = 35500760;
  m_Label2Nweight["QCD_Pt_30to50_TuneCP5_13TeV_pythia8"] = 35500760;
  m_Label2Nabsweight["QCD_Pt_30to50_TuneCP5_13TeV_pythia8"] = 35500760;

  m_Label2Nevent["QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8"] = 9980050;
  m_Label2Nweight["QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8"] = 9980050;
  m_Label2Nabsweight["QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8"] = 9980050;

  m_Label2Nevent["QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8"] = 757837;
  m_Label2Nweight["QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8"] = 757837;
  m_Label2Nabsweight["QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8"] = 757837;

  m_Label2Nevent["QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8"] = 391735;
  m_Label2Nweight["QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8"] = 391735;
  m_Label2Nabsweight["QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8"] = 391735;

  m_Label2Nevent["QCD_Pt_470to600_TuneCP5_13TeV_pythia8"] = 27881028;
  m_Label2Nweight["QCD_Pt_470to600_TuneCP5_13TeV_pythia8"] = 27881028;
  m_Label2Nabsweight["QCD_Pt_470to600_TuneCP5_13TeV_pythia8"] = 27881028;

  m_Label2Nevent["QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8"] = 3959986;
  m_Label2Nweight["QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8"] = 3959986;
  m_Label2Nabsweight["QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8"] = 3959986;

  m_Label2Nevent["QCD_Pt_50to80_TuneCP5_13TeV_pythia8"] = 38522403;
  m_Label2Nweight["QCD_Pt_50to80_TuneCP5_13TeV_pythia8"] = 38522403;
  m_Label2Nabsweight["QCD_Pt_50to80_TuneCP5_13TeV_pythia8"] = 38522403;

  m_Label2Nevent["QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8"] = 9954370;
  m_Label2Nweight["QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8"] = 9954370;
  m_Label2Nabsweight["QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8"] = 9954370;

  m_Label2Nevent["QCD_Pt_600to800_TuneCP5_13TeV_pythia8"] = 66134964;
  m_Label2Nweight["QCD_Pt_600to800_TuneCP5_13TeV_pythia8"] = 66134964;
  m_Label2Nabsweight["QCD_Pt_600to800_TuneCP5_13TeV_pythia8"] = 66134964;

  m_Label2Nevent["QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8"] = 13519308;
  m_Label2Nweight["QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8"] = 13519308;
  m_Label2Nabsweight["QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8"] = 13519308;

  m_Label2Nevent["QCD_Pt_800to1000_TuneCP5_13TeV_pythia8"] = 116423008;
  m_Label2Nweight["QCD_Pt_800to1000_TuneCP5_13TeV_pythia8"] = 116423008;
  m_Label2Nabsweight["QCD_Pt_800to1000_TuneCP5_13TeV_pythia8"] = 116423008;

  m_Label2Nevent["QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8"] = 19697092;
  m_Label2Nweight["QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8"] = 19697092;
  m_Label2Nabsweight["QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8"] = 19697092;

  m_Label2Nevent["QCD_Pt_80to120_TuneCP5_13TeV_pythia8"] = 56387936;
  m_Label2Nweight["QCD_Pt_80to120_TuneCP5_13TeV_pythia8"] = 56387936;
  m_Label2Nabsweight["QCD_Pt_80to120_TuneCP5_13TeV_pythia8"] = 56387936;

  m_Label2Nevent["QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8"] = 14645958;
  m_Label2Nweight["QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8"] = 14645958;
  m_Label2Nabsweight["QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8"] = 14645958;

  m_Label2Nevent["ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8"] = 9883805;
  m_Label2Nweight["ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8"] = 6167441;
  m_Label2Nabsweight["ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8"] = 9883805;

  m_Label2Nevent["ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 38811017;
  m_Label2Nweight["ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 38811017;
  m_Label2Nabsweight["ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 38811017;

  m_Label2Nevent["ST_t-channel_antitop_5f_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 3997530;
  m_Label2Nweight["ST_t-channel_antitop_5f_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 3975764;
  m_Label2Nabsweight["ST_t-channel_antitop_5f_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 3997530;

  m_Label2Nevent["ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 66815756;
  m_Label2Nweight["ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 66815756;
  m_Label2Nabsweight["ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 66815756;

  m_Label2Nevent["ST_t-channel_top_5f_TuneCP5_13TeV-powheg-pythia8"] = 5976002;
  m_Label2Nweight["ST_t-channel_top_5f_TuneCP5_13TeV-powheg-pythia8"] = 5941696;
  m_Label2Nabsweight["ST_t-channel_top_5f_TuneCP5_13TeV-powheg-pythia8"] = 5976002;

  m_Label2Nevent["ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 708261;
  m_Label2Nweight["ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 708261;
  m_Label2Nabsweight["ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 708261;

  m_Label2Nevent["ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 7977430;
  m_Label2Nweight["ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 7916640;
  m_Label2Nabsweight["ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 7977430;

  m_Label2Nevent["ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 992024;
  m_Label2Nweight["ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 992024;
  m_Label2Nabsweight["ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 992024;

  m_Label2Nevent["ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 7794186;
  m_Label2Nweight["ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 7734344;
  m_Label2Nabsweight["ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 7794186;

  m_Label2Nevent["TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8"] = 145309027;
  m_Label2Nweight["TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8"] = 54243217;
  m_Label2Nabsweight["TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8"] = 145309027;

  m_Label2Nevent["TTJets_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8026103;
  m_Label2Nweight["TTJets_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8016963;
  m_Label2Nabsweight["TTJets_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8026103;

  m_Label2Nevent["TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8"] = 88745871;
  m_Label2Nweight["TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8"] = 30712089;
  m_Label2Nabsweight["TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8"] = 88745871;

  m_Label2Nevent["TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 250000;
  m_Label2Nweight["TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 104640;
  m_Label2Nabsweight["TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 250000;

  m_Label2Nevent["TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"] = 8705576;
  m_Label2Nweight["TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"] = 8634992;
  m_Label2Nabsweight["TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"] = 8705576;

  m_Label2Nevent["TTToHadronic_TuneCP5_13TeV-powheg-pythia8"] = 42357944;
  m_Label2Nweight["TTToHadronic_TuneCP5_13TeV-powheg-pythia8"] = 42357944;
  m_Label2Nabsweight["TTToHadronic_TuneCP5_13TeV-powheg-pythia8"] = 42357944;

  m_Label2Nevent["TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8"] = 41221873;
  m_Label2Nweight["TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8"] = 40888765;
  m_Label2Nabsweight["TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8"] = 41221873;

  m_Label2Nevent["TTWH_TuneCP5_13TeV-madgraph-pythia8"] = 200000;
  m_Label2Nweight["TTWH_TuneCP5_13TeV-madgraph-pythia8"] = 198982;
  m_Label2Nabsweight["TTWH_TuneCP5_13TeV-madgraph-pythia8"] = 200000;

  m_Label2Nevent["TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8"] = 4919674;
  m_Label2Nweight["TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8"] = 2692366;
  m_Label2Nabsweight["TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8"] = 4919674;

  m_Label2Nevent["TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 5280565;
  m_Label2Nweight["TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 2716249;
  m_Label2Nabsweight["TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 5280565;

  m_Label2Nevent["TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8"] = 811306;
  m_Label2Nweight["TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8"] = 441560;
  m_Label2Nabsweight["TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8"] = 811306;

  m_Label2Nevent["TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 781201;
  m_Label2Nweight["TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 403219;
  m_Label2Nabsweight["TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 781201;

  m_Label2Nevent["TTWW_TuneCP5_13TeV-madgraph-pythia8"] = 200000;
  m_Label2Nweight["TTWW_TuneCP5_13TeV-madgraph-pythia8"] = 199010;
  m_Label2Nabsweight["TTWW_TuneCP5_13TeV-madgraph-pythia8"] = 200000;

  m_Label2Nevent["TTWZ_TuneCP5_13TeV-madgraph-pythia8"] = 200000;
  m_Label2Nweight["TTWZ_TuneCP5_13TeV-madgraph-pythia8"] = 198758;
  m_Label2Nabsweight["TTWZ_TuneCP5_13TeV-madgraph-pythia8"] = 200000;

  m_Label2Nevent["TTZH_TuneCP5_13TeV-madgraph-pythia8"] = 200000;
  m_Label2Nweight["TTZH_TuneCP5_13TeV-madgraph-pythia8"] = 199286;
  m_Label2Nabsweight["TTZH_TuneCP5_13TeV-madgraph-pythia8"] = 200000;

  m_Label2Nevent["TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 7974473;
  m_Label2Nweight["TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 3718439;
  m_Label2Nabsweight["TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 7974473;

  m_Label2Nevent["TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8"] = 750000;
  m_Label2Nweight["TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8"] = 356286;
  m_Label2Nabsweight["TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8"] = 750000;

  m_Label2Nevent["TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 749400;
  m_Label2Nweight["TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 351164;
  m_Label2Nabsweight["TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 749400;

  m_Label2Nevent["TTZZ_TuneCP5_13TeV-madgraph-pythia8"] = 200000;
  m_Label2Nweight["TTZZ_TuneCP5_13TeV-madgraph-pythia8"] = 199372;
  m_Label2Nabsweight["TTZZ_TuneCP5_13TeV-madgraph-pythia8"] = 200000;

  m_Label2Nevent["WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"] = 77803408;
  m_Label2Nweight["WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"] = 77734008;
  m_Label2Nabsweight["WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"] = 77803408;

  m_Label2Nevent["WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8081153;
  m_Label2Nweight["WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8028207;
  m_Label2Nabsweight["WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8081153;

  m_Label2Nevent["WJetsToQQ_HT400to600_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9738307;
  m_Label2Nweight["WJetsToQQ_HT400to600_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9708539;
  m_Label2Nabsweight["WJetsToQQ_HT400to600_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9738307;

  m_Label2Nevent["WJetsToQQ_HT600to800_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8798398;
  m_Label2Nweight["WJetsToQQ_HT600to800_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8761386;
  m_Label2Nabsweight["WJetsToQQ_HT600to800_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 8798398;

  m_Label2Nevent["WWG_TuneCP5_13TeV-amcatnlo-pythia8"] = 1000000;
  m_Label2Nweight["WWG_TuneCP5_13TeV-amcatnlo-pythia8"] = 824454;
  m_Label2Nabsweight["WWG_TuneCP5_13TeV-amcatnlo-pythia8"] = 1000000;

  m_Label2Nevent["WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 999400;
  m_Label2Nweight["WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 827630;
  m_Label2Nabsweight["WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 999400;

  m_Label2Nevent["WWTo2L2Nu_13TeV-powheg"] = 1999000;
  m_Label2Nweight["WWTo2L2Nu_13TeV-powheg"] = 1999000;
  m_Label2Nabsweight["WWTo2L2Nu_13TeV-powheg"] = 1999000;

  m_Label2Nevent["WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 2000000;
  m_Label2Nweight["WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1992522;
  m_Label2Nabsweight["WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 2000000;

  m_Label2Nevent["WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 2000000;
  m_Label2Nweight["WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 1992526;
  m_Label2Nabsweight["WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 2000000;

  m_Label2Nevent["WWTo4Q_13TeV-powheg"] = 1998400;
  m_Label2Nweight["WWTo4Q_13TeV-powheg"] = 1998400;
  m_Label2Nabsweight["WWTo4Q_13TeV-powheg"] = 1998400;

  m_Label2Nevent["WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1976644;
  m_Label2Nweight["WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1969110;
  m_Label2Nabsweight["WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1976644;

  m_Label2Nevent["WWToLNuQQ_13TeV-powheg"] = 8997800;
  m_Label2Nweight["WWToLNuQQ_13TeV-powheg"] = 8997800;
  m_Label2Nabsweight["WWToLNuQQ_13TeV-powheg"] = 8997800;

  m_Label2Nevent["WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 18674616;
  m_Label2Nweight["WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 18604316;
  m_Label2Nabsweight["WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 18674616;

  m_Label2Nevent["WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 232300;
  m_Label2Nweight["WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 203246;
  m_Label2Nabsweight["WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 232300;

  m_Label2Nevent["WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 240000;
  m_Label2Nweight["WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 210538;
  m_Label2Nabsweight["WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 240000;

  m_Label2Nevent["WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 250000;
  m_Label2Nweight["WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 219964;
  m_Label2Nabsweight["WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 250000;

  m_Label2Nevent["WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 250000;
  m_Label2Nweight["WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 221468;
  m_Label2Nabsweight["WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 250000;

  m_Label2Nevent["WW_TuneCP5_13TeV-pythia8"] = 7791498;
  m_Label2Nweight["WW_TuneCP5_13TeV-pythia8"] = 7791498;
  m_Label2Nabsweight["WW_TuneCP5_13TeV-pythia8"] = 7791498;

  m_Label2Nevent["WZ_TuneCP5_13TeV-pythia8"] = 3928630;
  m_Label2Nweight["WZ_TuneCP5_13TeV-pythia8"] = 3928630;
  m_Label2Nabsweight["WZ_TuneCP5_13TeV-pythia8"] = 3928630;

  m_Label2Nevent["WZ_TuneCUETP8M1_13TeV-pythia8"] = 1000000;
  m_Label2Nweight["WZ_TuneCUETP8M1_13TeV-pythia8"] = 1000000;
  m_Label2Nabsweight["WZ_TuneCUETP8M1_13TeV-pythia8"] = 1000000;

  m_Label2Nevent["ZJetsToNuNu_HT-100To200_13TeV-madgraph"] = 22737266;
  m_Label2Nweight["ZJetsToNuNu_HT-100To200_13TeV-madgraph"] = 22702468;
  m_Label2Nabsweight["ZJetsToNuNu_HT-100To200_13TeV-madgraph"] = 22737266;

  m_Label2Nevent["ZJetsToNuNu_HT-1200To2500_13TeV-madgraph"] = 338948;
  m_Label2Nweight["ZJetsToNuNu_HT-1200To2500_13TeV-madgraph"] = 334332;
  m_Label2Nabsweight["ZJetsToNuNu_HT-1200To2500_13TeV-madgraph"] = 338948;

  m_Label2Nevent["ZJetsToNuNu_HT-200To400_13TeV-madgraph"] = 21675916;
  m_Label2Nweight["ZJetsToNuNu_HT-200To400_13TeV-madgraph"] = 21618510;
  m_Label2Nabsweight["ZJetsToNuNu_HT-200To400_13TeV-madgraph"] = 21675916;

  m_Label2Nevent["ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph"] = 6734;
  m_Label2Nweight["ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph"] = 6446;
  m_Label2Nabsweight["ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph"] = 6734;

  m_Label2Nevent["ZJetsToNuNu_HT-400To600_13TeV-madgraph"] = 9134120;
  m_Label2Nweight["ZJetsToNuNu_HT-400To600_13TeV-madgraph"] = 9094890;
  m_Label2Nabsweight["ZJetsToNuNu_HT-400To600_13TeV-madgraph"] = 9134120;

  m_Label2Nevent["ZJetsToNuNu_HT-600To800_13TeV-madgraph"] = 5697594;
  m_Label2Nweight["ZJetsToNuNu_HT-600To800_13TeV-madgraph"] = 5664642;
  m_Label2Nabsweight["ZJetsToNuNu_HT-600To800_13TeV-madgraph"] = 5697594;

  m_Label2Nevent["ZJetsToNuNu_HT-800To1200_13TeV-madgraph"] = 2058077;
  m_Label2Nweight["ZJetsToNuNu_HT-800To1200_13TeV-madgraph"] = 2041779;
  m_Label2Nabsweight["ZJetsToNuNu_HT-800To1200_13TeV-madgraph"] = 2058077;

  m_Label2Nevent["ZJetsToQQ_HT-800toInf_TuneCP5_13TeV-madgraphMLM-pythia8"] = 26309061;
  m_Label2Nweight["ZJetsToQQ_HT-800toInf_TuneCP5_13TeV-madgraphMLM-pythia8"] = 26129381;
  m_Label2Nabsweight["ZJetsToQQ_HT-800toInf_TuneCP5_13TeV-madgraphMLM-pythia8"] = 26309061;

  m_Label2Nevent["ZJetsToQQ_HT400to600_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9502165;
  m_Label2Nweight["ZJetsToQQ_HT400to600_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9471393;
  m_Label2Nabsweight["ZJetsToQQ_HT400to600_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9502165;

  m_Label2Nevent["ZJetsToQQ_HT600to800_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9864685;
  m_Label2Nweight["ZJetsToQQ_HT600to800_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9826701;
  m_Label2Nabsweight["ZJetsToQQ_HT600to800_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 9864685;

  m_Label2Nevent["ZZTo2L2Nu_13TeV_powheg_pythia8"] = 17587243;
  m_Label2Nweight["ZZTo2L2Nu_13TeV_powheg_pythia8"] = 8842475;
  m_Label2Nabsweight["ZZTo2L2Nu_13TeV_powheg_pythia8"] = 8842475;

  m_Label2Nevent["ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 43186490;
  m_Label2Nweight["ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 19366831;
  m_Label2Nabsweight["ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 30511127;

  m_Label2Nevent["ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8"] = 30016308;
  m_Label2Nweight["ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8"] = 18430292;
  m_Label2Nabsweight["ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8"] = 30016308;

  m_Label2Nevent["ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8"] = 62172314;
  m_Label2Nweight["ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8"] = 39238220;
  m_Label2Nabsweight["ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8"] = 62172314;

  m_Label2Nevent["ZZTo4L_13TeV_powheg_pythia8"] = 13637841;
  m_Label2Nweight["ZZTo4L_13TeV_powheg_pythia8"] = 6669988;
  m_Label2Nabsweight["ZZTo4L_13TeV_powheg_pythia8"] = 6669988;

  m_Label2Nevent["ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 30413835;
  m_Label2Nweight["ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 18816739;
  m_Label2Nabsweight["ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 30413835;

  m_Label2Nevent["ZZZ_TuneCP5_13TeV-amcatnlo-pythia8"] = 250000;
  m_Label2Nweight["ZZZ_TuneCP5_13TeV-amcatnlo-pythia8"] = 214318;
  m_Label2Nabsweight["ZZZ_TuneCP5_13TeV-amcatnlo-pythia8"] = 250000;

  m_Label2Nevent["ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 249237;
  m_Label2Nweight["ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 213197;
  m_Label2Nabsweight["ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 249237;

  m_Label2Nevent["ZZ_TuneCP5_13TeV-pythia8"] = 1949768;
  m_Label2Nweight["ZZ_TuneCP5_13TeV-pythia8"] = 1949768;
  m_Label2Nabsweight["ZZ_TuneCP5_13TeV-pythia8"] = 1949768;

  m_Label2Nevent["ttH_M125_TuneCP5_13TeV-powheg-pythia8"] = 9783674;
  m_Label2Nweight["ttH_M125_TuneCP5_13TeV-powheg-pythia8"] = 9580578;
  m_Label2Nabsweight["ttH_M125_TuneCP5_13TeV-powheg-pythia8"] = 9783674;

  m_Label2Nevent["ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 6415920;
  m_Label2Nweight["ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 6388052;
  m_Label2Nabsweight["ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 6415920;

  m_Label2Nevent["ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 9698473;
  m_Label2Nweight["ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 9678691;
  m_Label2Nabsweight["ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 9698473;

  m_Label2Xsec["DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["GJets_DR-0p4_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["GJets_DR-0p4_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["GJets_DR-0p4_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_120to170_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_15to30_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_170to300_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_300to470_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_30to50_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_3200toInf_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_470to600_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_50to80_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_600to800_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_800to1000_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_80to120_TuneCP5_13TeV_pythia8"] = 1;

  m_Label2Xsec["QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8"] = 1;

  m_Label2Xsec["ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 1;

  m_Label2Xsec["ST_t-channel_antitop_5f_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1"] = 1;

  m_Label2Xsec["ST_t-channel_top_5f_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 1;

  m_Label2Xsec["ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4"] = 1;

  m_Label2Xsec["ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8"] = 1;

  m_Label2Xsec["TTJets_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8"] = 1;

  m_Label2Xsec["TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["TTToHadronic_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["TTWH_TuneCP5_13TeV-madgraph-pythia8"] = 1;

  m_Label2Xsec["TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;

  m_Label2Xsec["TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;

  m_Label2Xsec["TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;

  m_Label2Xsec["TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8"] = 1;

  m_Label2Xsec["TTWW_TuneCP5_13TeV-madgraph-pythia8"] = 1;

  m_Label2Xsec["TTWZ_TuneCP5_13TeV-madgraph-pythia8"] = 1;

  m_Label2Xsec["TTZH_TuneCP5_13TeV-madgraph-pythia8"] = 1;

  m_Label2Xsec["TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["TTZZ_TuneCP5_13TeV-madgraph-pythia8"] = 1;

  m_Label2Xsec["WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["WJetsToQQ_HT400to600_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["WJetsToQQ_HT600to800_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["WWG_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["WWTo2L2Nu_13TeV-powheg"] = 1;

  m_Label2Xsec["WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["WWTo4Q_13TeV-powheg"] = 1;

  m_Label2Xsec["WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["WWToLNuQQ_13TeV-powheg"] = 1;

  m_Label2Xsec["WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["WW_TuneCP5_13TeV-pythia8"] = 1;

  m_Label2Xsec["WZ_TuneCP5_13TeV-pythia8"] = 1;

  m_Label2Xsec["WZ_TuneCUETP8M1_13TeV-pythia8"] = 1;

  m_Label2Xsec["ZJetsToNuNu_HT-100To200_13TeV-madgraph"] = 1;

  m_Label2Xsec["ZJetsToNuNu_HT-1200To2500_13TeV-madgraph"] = 1;

  m_Label2Xsec["ZJetsToNuNu_HT-200To400_13TeV-madgraph"] = 1;

  m_Label2Xsec["ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph"] = 1;

  m_Label2Xsec["ZJetsToNuNu_HT-400To600_13TeV-madgraph"] = 1;

  m_Label2Xsec["ZJetsToNuNu_HT-600To800_13TeV-madgraph"] = 1;

  m_Label2Xsec["ZJetsToNuNu_HT-800To1200_13TeV-madgraph"] = 1;

  m_Label2Xsec["ZJetsToQQ_HT-800toInf_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["ZJetsToQQ_HT400to600_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["ZJetsToQQ_HT600to800_3j_TuneCP5_13TeV-madgraphMLM-pythia8"] = 1;

  m_Label2Xsec["ZZTo2L2Nu_13TeV_powheg_pythia8"] = 1;

  m_Label2Xsec["ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;

  m_Label2Xsec["ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;

  m_Label2Xsec["ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;

  m_Label2Xsec["ZZTo4L_13TeV_powheg_pythia8"] = 1;

  m_Label2Xsec["ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8"] = 1;

  m_Label2Xsec["ZZZ_TuneCP5_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8"] = 1;

  m_Label2Xsec["ZZ_TuneCP5_13TeV-pythia8"] = 1;

  m_Label2Xsec["ttH_M125_TuneCP5_13TeV-powheg-pythia8"] = 1;

  m_Label2Xsec["ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 1;

  m_Label2Xsec["ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8"] = 1;
}

template class AnalysisBase<StopNtupleTree>;

