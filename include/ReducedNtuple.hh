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
  TTree* InitOutputTree(const string& sample, bool do_slim = false);
  void FillOutputTree(TTree* tree, const Systematic& sys = Systematic::Default(), bool do_slim = false);

  void ClearVariables();

  bool m_event_skipped;
  
  // common variables for output tree
  double m_weight;
  double m_PUweight;
  double m_PUweight_up;
  double m_PUweight_down;
  double m_MuFweight;
  double m_MuFweight_up;
  double m_MuFweight_down;
  double m_MuRweight;
  double m_MuRweight_up;
  double m_MuRweight_down;
  double m_PDFweight;
  double m_PDFweight_up;
  double m_PDFweight_down;
  double m_BtagHFSFweight;
  double m_BtagHFSFweight_up;
  double m_BtagHFSFweight_down;
  double m_BtagLFSFweight;
  double m_BtagLFSFweight_up;
  double m_BtagLFSFweight_down;

  double m_elIDSFweight;
  double m_elIDSFweight_up;
  double m_elIDSFweight_down;
  double m_elISOSFweight;
  double m_elISOSFweight_up;
  double m_elISOSFweight_down;
  double m_elSIPSFweight;
  double m_elSIPSFweight_up;
  double m_elSIPSFweight_down;
  double m_elVLSFweight;
  double m_elVLSFweight_up;
  double m_elVLSFweight_down;
  double m_muIDSFweight;
  double m_muIDSFweight_up;
  double m_muIDSFweight_down;
  double m_muISOSFweight;
  double m_muISOSFweight_up;
  double m_muISOSFweight_down;
  double m_muSIPSFweight;
  double m_muSIPSFweight_up;
  double m_muSIPSFweight_down;
  double m_muVLSFweight;
  double m_muVLSFweight_up;
  double m_muVLSFweight_down;
  
  double m_MetTrigSFweight;
  double m_MetTrigSFweight_up;
  double m_MetTrigSFweight_down;
  int    m_MetTrigSFCurveIndex;

  int m_runnum;
  int m_luminum;
  Long64_t m_eventnum;

  int m_NPV;
  int m_NPU;
  
  double m_MET;
  double m_MET_phi;

  double m_altMET;
  double m_altMET_phi;

  double m_genMET;
  double m_genMET_phi;

  double m_LHE_HT;
  double m_LHE_HTIncoming;

  double m_HT_eta24;
  double m_HT_eta24_id;
  double m_HT_eta3;
  double m_HT_eta3_id;
  double m_HT_eta5;
  double m_HT_eta5_id;
  
  bool m_EventFilter;
  bool m_FastSimEventVeto;

  bool m_EventFlag_FailJetID;
  bool m_EventFlag_JetInHEM;
  bool m_EventFlag_JetInHEM_Pt20;
  bool m_EventFlag_JetInHEM_Pt20_JetID;
  bool m_HEM_Veto;

  double m_PrefireWeight;
  double m_PrefireWeight_up;
  double m_PrefireWeight_down;
  
  bool m_METtrigger;
  bool m_METHTtrigger;
  bool m_METORtrigger;

  bool m_SingleElectrontrigger;
  bool m_SingleMuontrigger;
  bool m_DoubleElectrontrigger;
  bool m_DoubleMuontrigger;
  bool m_EMutrigger;  
  
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
  vector<int>    m_SourceID_lep;
  vector<int>    m_LepQual_lep;
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

  int m_NGenjet;
  vector<double> m_PT_Genjet;
  vector<double> m_Eta_Genjet;
  vector<double> m_Phi_Genjet;
  vector<double> m_M_Genjet;
  vector<int>    m_Index_jet;

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
  vector<int>    m_genSourceID_lep;
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

  // Cascade
  int m_c_Njet_a;
  int m_c_Nbjet_a;
  int m_c_Njet_b;
  int m_c_Nbjet_b;
  int m_c_Nlep_1a;
  int m_c_Nlep_1b;
  int m_c_Nlep_2b;
  vector<int> m_c_index_jet_a;
  vector<int> m_c_index_jet_b;
  vector<int> m_c_index_lep_1a;
  vector<int> m_c_index_lep_1b;
  vector<int> m_c_index_lep_2b;
  
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
  double m_PzCM;
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

  double m_PV_lab;
  double m_dphiMET_V;

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
  double m_gammaT;

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
  double m_PISR;
  double m_PTISR;
  double m_MISR;
  double m_RISR;
  double m_RISRT;

  // Cascade variables
  double  m_c_MX3a_S; 
  double  m_c_MX3b_S;
  double  m_c_MX3a_Vis;
  double  m_c_MX3b_Vis;
  double  m_c_MX2a_S;
  double  m_c_MX2b_S;
  double  m_c_MX2a_Vis;
  double  m_c_MX2b_Vis;
 
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
  SelfAssemblingRecoFrame*   saL2b[2];
  VisibleRecoFrame*   ISR[2];
  VisibleRecoFrame*   Ja[2];
  VisibleRecoFrame*   Jb[2];
  VisibleRecoFrame*   La[2];
  VisibleRecoFrame*   Lb[2];
  InvisibleRecoFrame* X1a[2];
  InvisibleRecoFrame* X1b[2];
  VisibleRecoFrame*   L2a[2];
  VisibleRecoFrame*   L2b[2];

  InvisibleGroup*       INV[2];
  SetMassInvJigsaw*     InvM[2];
  SetRapidityInvJigsaw* InvEta[2];
  MinMassesSqInvJigsaw* InvSplit[2];
  
  CombinatoricGroup*   COMB_J[2];
  MinMassesCombJigsaw*   CombSplit_ISR[2];
  MinMassesSqCombJigsaw* CombSplit_J[2];

  CombinatoricGroup*   COMB_L[2];
  MinMassesSqCombJigsaw* CombSplit_L[2];

  MinMassesSqCombJigsaw* CombSplit_L2[2];
  MinMassesSqCombJigsaw* CombSplit_Lb[2];
 
};

#endif
