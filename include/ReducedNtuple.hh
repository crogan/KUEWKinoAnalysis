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
  bool m_library_generated;
  TTree* InitOutputTree(const string& sample);
  void FillOutputTree(TTree* tree, const Systematic& sys = Systematic::Default());

  void ClearVariables();

  bool m_event_skipped;
  
  // common variables for output tree
  double m_weight;
  double m_PUweight;
  double m_PUweight_up;
  double m_PUweight_down;
  double m_BtagSFweight;
  double m_BtagSFweight_up;
  double m_BtagSFweight_down;

  int m_runnum;
  int m_luminum;
  Long64_t m_eventnum;

  int m_NPV;
  int m_NPU;
  
  double m_MET;
  double m_MET_phi;

  double m_genMET;
  double m_genMET_phi;

  double m_HT;

  bool m_EventFilter;
  
  bool m_METtrigger;
  bool m_METHTtrigger;
  bool m_METORtrigger;
  
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
  vector<double> m_Dxy_lep;
  vector<double> m_DxyErr_lep;
  vector<double> m_Dz_lep;
  vector<double> m_DzErr_lep;
  vector<double> m_IP3D_lep;
  vector<double> m_SIP3D_lep;
  vector<int>    m_ID_lep;
  vector<int>    m_Index_lep;

  int m_Njet;
  int m_Nbjet;
  vector<double> m_PT_jet;
  vector<double> m_Eta_jet;
  vector<double> m_Phi_jet;
  vector<double> m_M_jet;
  vector<double> m_Btag_jet;
  vector<int>    m_BtagID_jet;
  vector<double> m_Flavor_jet;
  vector<double> m_ProbB_SV;
  vector<double> m_ProbC_SV;

  int m_NSV;
  vector<double> m_PT_SV;
  vector<double> m_Eta_SV;
  vector<double> m_Phi_SV;
  vector<double> m_M_SV;

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
  
  int m_Njet_ISR;
  int m_Njet_S;
  int m_Nbjet_ISR;
  int m_Nbjet_S;
  int m_Nlep_ISR;
  int m_Nlep_S;
  int m_NSV_ISR;
  int m_NSV_S;
  vector<int> m_index_jet_ISR;
  vector<int> m_index_jet_S;
  vector<int> m_index_SV_ISR;
  vector<int> m_index_SV_S;
  vector<int> m_index_lep_ISR;
  vector<int> m_index_lep_S;
  
  vector<double> m_dphi_lep_S;
  vector<double> m_cos_lep_S;
  vector<double> m_dphi_SV_S;
  vector<double> m_cos_SV_S;
  vector<double> m_dphi_jet_S;
  vector<double> m_cos_jet_S;
  
  vector<double> m_dphiMET_lep_S;
  vector<double> m_dphiMET_SV_S; 
  vector<double> m_dphiMET_jet_S; 
  
  int m_Njet_a;
  int m_Njet_b;
  int m_Nbjet_a;
  int m_Nbjet_b;
  int m_Nlep_a;
  int m_Nlep_b;
  int m_NSV_a;
  int m_NSV_b;
 
  vector<int> m_index_jet_a;
  vector<int> m_index_jet_b;
  vector<int> m_index_lep_a;
  vector<int> m_index_lep_b;
  vector<int> m_index_SV_a;
  vector<int> m_index_SV_b;
  
  // Kinematic Variables

  double m_PTCM;
  double m_cosCM;
  double m_dphiCM;
  double m_dphiCMI;
  
  double m_MS;
  double m_PS;
  double m_cosS;
  double m_dphiS;
  double m_dphiSI;
  double m_PTS;
  double m_PzS;

  double m_MX3a;
  double m_MX3b;
  double m_EVa;
  double m_EVb;
  double m_PVa;
  double m_PVb;
  double m_EJa;
  double m_EJb;
  double m_PJa;
  double m_PJb;

  double m_MX2a;
  double m_MX2b;
  double m_ELa;
  double m_ELb;
  double m_PLa;
  double m_PLb;

  double m_MV;
  double m_PV;
  double m_MVa;
  double m_MVb;

  double m_MJ;
  double m_ML;
  double m_EJ;
  double m_EL;
  double m_PJ;
  double m_PL;
  
  double m_PX3;
  double m_PX3_BoostT;
  double m_MX3a_BoostT;
  double m_MX3b_BoostT;
  double m_Mperp; 

  double m_PV_BoostT;
  
  double m_EVa_BoostT;
  double m_EVb_BoostT;
  double m_PVa_BoostT;
  double m_PVb_BoostT;

  double m_EJ_BoostT;
  double m_EL_BoostT;
  double m_PJ_BoostT;
  double m_PL_BoostT;
  
  double m_MJa;
  double m_MJb;
  double m_MLa;
  double m_MLb;
  double m_cosJa;
  double m_cosJb;
  double m_cosLa;
  double m_cosLb;

  double m_H11S;
  double m_H21S;
  double m_HT21S;
  double m_H22S;
  double m_HT22S;
  double m_H42S;
  double m_HT42S;
  
  double m_H11X3a;
  double m_H11X3b;
  double m_H21X3a;
  double m_H21X3b;

  // ISR related variables
  double m_PTISR;
  double m_MISR;
  double m_RISR;
 
  // RestFrames frames and friends
  LabRecoFrame*     LAB;
  DecayRecoFrame*   CM;
  DecayRecoFrame*   S;
  DecayRecoFrame*   X3a;
  DecayRecoFrame*   X3b;
  DecayRecoFrame*   X2a;
  DecayRecoFrame*   X2b;
  SelfAssemblingRecoFrame*   saJa;
  SelfAssemblingRecoFrame*   saJb;
  SelfAssemblingRecoFrame*   saLa;
  SelfAssemblingRecoFrame*   saLb;
  VisibleRecoFrame*   ISR;
  VisibleRecoFrame*   Ja;
  VisibleRecoFrame*   Jb;
  VisibleRecoFrame*   La;
  VisibleRecoFrame*   Lb;
  InvisibleRecoFrame* X1a;
  InvisibleRecoFrame* X1b;

  InvisibleGroup*       INV;
  SetMassInvJigsaw*     InvM;
  SetRapidityInvJigsaw* InvEta;
  MinMassesSqInvJigsaw* InvSplit;
  
  CombinatoricGroup*   COMB_J;
  MinMassesCombJigsaw*   CombSplit_ISR;
  MinMassesSqCombJigsaw* CombSplit_J;

  CombinatoricGroup*   COMB_L;
  MinMassesSqCombJigsaw* CombSplit_L;
 
};

#endif
