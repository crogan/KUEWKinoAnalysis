#ifndef ReducedNtuple_h
#define ReducedNtuple_h

#include "NtupleBase.hh"
#include "RestFrames/RestFrames.hh"

template class std::vector<std::vector<int> >;

using namespace RestFrames;

template <class Base>
class ReducedNtuple : public NtupleBase<Base> {

public:
  ReducedNtuple(TTree* tree = 0);
  virtual ~ReducedNtuple();

private:
  TTree* InitOutputTree(const string& sample);
  void FillOutputTree(TTree* tree);

  void ClearVariables();

  bool m_event_skipped;
  
  // common variables for output tree
  double m_weight;
  
  double m_MET;
  double m_MET_phi;

  double m_genMET;
  double m_genMET_phi;

  double m_HT;

  int m_Nele;
  int m_Nmu;
  
  int m_Nlep;
  vector<double> m_PT_lep;
  vector<double> m_Eta_lep;
  vector<double> m_Phi_lep;
  vector<double> m_M_lep;
  vector<int>    m_Charge_lep;
  vector<int>    m_PDGID_lep;
  vector<double> m_RelIso_lep;
  vector<double> m_MiniIso_lep;
  vector<int>    m_ID_lep;
  vector<int>    m_Index_lep;

  int m_Njet;
  int m_Nbjet;
  vector<double> m_PT_jet;
  vector<double> m_Eta_jet;
  vector<double> m_Phi_jet;
  vector<double> m_M_jet;
  vector<double> m_Btag_jet;
  vector<double> m_Flavor_jet;

  int m_genNele;
  int m_genNmu;

  int m_genNlep;
  vector<double> m_genPT_lep;
  vector<double> m_genEta_lep;
  vector<double> m_genPhi_lep;
  vector<double> m_genM_lep;
  vector<int>    m_genCharge_lep;
  vector<int>    m_genPDGID_lep;
  vector<int>    m_genMomPDGID_lep;
  vector<int>    m_genIndex_lep;

  int m_genNnu;
  vector<double> m_genPT_nu;
  vector<double> m_genEta_nu;
  vector<double> m_genPhi_nu;
  vector<int>    m_genPDGID_nu;
  vector<int>    m_genMomPDGID_nu;
  
  int m_genNboson;
  vector<double> m_genPT_boson;
  vector<double> m_genEta_boson;
  vector<double> m_genPhi_boson;
  vector<double> m_genM_boson;
  vector<int>    m_genPDGID_boson;
  vector<int>    m_genMomPDGID_boson;
  
  int m_genNsusy;
  vector<double> m_genPT_susy;
  vector<double> m_genEta_susy;
  vector<double> m_genPhi_susy;
  vector<double> m_genM_susy;
  vector<int>    m_genPDGID_susy;
  vector<int>    m_genMomPDGID_susy;

  //////////////////////
  // derived observables
  //////////////////////

  // Object Counting Variables
  
  vector<int> m_Njet_ISR;
  vector<int> m_Njet_S;
  vector<int> m_Nbjet_ISR;
  vector<int> m_Nbjet_S;
  vector<int> m_Nlep_ISR;
  vector<int> m_Nlep_S;
  vector<vector<int> > m_index_jet_ISR;
  vector<vector<int> > m_index_jet_S;
  vector<vector<int> > m_index_lep_ISR;
  vector<vector<int> >    m_index_lep_S;
  vector<vector<double> > m_dphi_lep_S;
  vector<vector<double> > m_cos_lep_S;
  
  vector<int> m_Njet_a;
  vector<int> m_Njet_b;
  vector<int> m_Nbjet_a;
  vector<int> m_Nbjet_b;
  vector<int> m_Nlep_a;
  vector<int> m_Nlep_b;
 
  vector<vector<int> > m_index_jet_a;
  vector<vector<int> > m_index_jet_b;
  vector<vector<int> > m_index_lep_a;
  vector<vector<int> > m_index_lep_b;
  
  // Kinematic Variables

  vector<double> m_PTCM;
  vector<double> m_cosCM;
  vector<double> m_dphiCM;
  vector<double> m_dphiCMI;
  
  vector<double> m_MS;
  vector<double> m_PS;
  vector<double> m_cosS;
  vector<double> m_dphiS;
  vector<double> m_dphiSI;
  vector<double> m_PTS;
  vector<double> m_PzS;

  vector<double> m_MX3a;
  vector<double> m_cosX3a;
  vector<double> m_MX3b;
  vector<double> m_cosX3b;
  vector<double> m_EVa;
  vector<double> m_EVb;
  vector<double> m_PVa;
  vector<double> m_PVb;
  vector<double> m_EJa;
  vector<double> m_EJb;
  vector<double> m_PJa;
  vector<double> m_PJb;

  vector<double> m_MX2a;
  vector<double> m_cosX2a;
  vector<double> m_MX2b;
  vector<double> m_cosX2b;
  vector<double> m_ELa;
  vector<double> m_ELb;
  vector<double> m_PLa;
  vector<double> m_PLb;

  vector<double> m_MV;
  vector<double> m_PV;
  vector<double> m_MVa;
  vector<double> m_MVb;

  vector<double> m_MJa;
  vector<double> m_MJb;
  vector<double> m_MLa;
  vector<double> m_MLb;
  vector<double> m_cosJa;
  vector<double> m_cosJb;
  vector<double> m_cosLa;
  vector<double> m_cosLb;

  vector<double> m_H11S;
  vector<double> m_H21S;
  vector<double> m_HT21S;
  vector<double> m_H22S;
  vector<double> m_HT22S;
  vector<double> m_H42S;
  vector<double> m_HT42S;
  
  vector<double> m_H11X3a;
  vector<double> m_H11X3b;
  vector<double> m_H21X3a;
  vector<double> m_H21X3b;

  // ISR related variables
  vector<double> m_PTISR;
  vector<double> m_MISR;
  vector<double> m_RISR;

  // which tree are we using for PAIR?
  // vanilla - index 0
  bool m_Is_1L;
  bool m_Is_2L;
  bool m_Is_3L;
  bool m_Is_4L;
 
  // RestFrames frames and friends
  LabRecoFrame*     LAB[2];
  DecayRecoFrame*   CM[2];
  DecayRecoFrame*   S[2];
  DecayRecoFrame*   X3a[2];
  DecayRecoFrame*   X3b[2];
  DecayRecoFrame*   X2a[2];
  DecayRecoFrame*   X2b[2];
  SelfAssemblingRecoFrame*   saJa[2];
  SelfAssemblingRecoFrame*   saJb[2];
  SelfAssemblingRecoFrame*   saLa[2];
  SelfAssemblingRecoFrame*   saLb[2];
  VisibleRecoFrame*   ISR[2];
  VisibleRecoFrame*   Ja[2];
  VisibleRecoFrame*   Jb[2];
  VisibleRecoFrame*   La[2];
  VisibleRecoFrame*   Lb[2];
  InvisibleRecoFrame* X1a[2];
  InvisibleRecoFrame* X1b[2];

  InvisibleGroup*       INV[2];
  SetMassInvJigsaw*     InvM[2];
  SetRapidityInvJigsaw* InvEta[2];
  MinMassesSqInvJigsaw* InvSplit[2];
  
  CombinatoricGroup*   COMB_J[2];
  MinMassesCombJigsaw*   CombSplit_ISR[2];
  MinMassesSqCombJigsaw* CombSplit_J[2];

  CombinatoricGroup*   COMB_L[2];
  MinMassesSqCombJigsaw* CombSplit_L[2];
 
  
};

#endif
