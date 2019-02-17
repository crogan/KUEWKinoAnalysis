#ifndef ReducedNtuple_h
#define ReducedNtuple_h

#include "NtupleBase.hh"
#include "StopNtupleTree.hh"
#include "RestFrames/RestFrames.hh"

using namespace RestFrames;

class ReducedNtuple : public NtupleBase<StopNtupleTree> {

public:
  ReducedNtuple(TTree* tree = 0);
  virtual ~ReducedNtuple();

private:
  TTree* InitOutputTree(const string& sample);
  void FillOutputTree(TTree* tree);

  double m_MET;
  double m_MET_phi;

  double m_HT;

  int m_nEl;
  int m_nMu;
  int m_nBjet;
  
  double m_pT_1lep;
  int m_id_1lep;
  double m_pT_2lep;
  int m_id_2lep;
  double m_pT_3lep;
  int m_id_3lep;

  // which tree are we using?
  
  bool m_Is_2LNJ;
  bool m_Is_2L1L;
  
  double m_HN2S;
  double m_HN2SR;
  double m_H11S;
  double m_HN1Ca;
  double m_HN1Cb;
  double m_H11Ca;
  double m_H11Cb;
  double m_cosC;

  double m_MZ;
  double m_MJ;
  double m_cosZ;
  double m_cosJ;
  
  // common variables for output tree
  double m_weight;

  bool m_Is_SF;

  int m_Nj;
  int m_NjS;
  int m_NjISR;
  
  // compressed observables
  // common to all trees
  double m_PTISR_comb;
  double m_PTCM_comb;
  double m_RISR_comb;
  double m_cosCM_comb;
  double m_cosS_comb;
  double m_MISR_comb;
  double m_MS_comb;
  double m_dphiCMI_comb;
  double m_dphiSI_comb;
  double m_dphiISRI_comb;

  double m_PTISR_fix;
  double m_PTCM_fix;
  double m_RISR_fix;
  double m_cosCM_fix;
  double m_cosS_fix;
  double m_MISR_fix;
  double m_MS_fix;
  double m_dphiCMI_fix;
  double m_dphiSI_fix;
  double m_dphiISRI_fix;

  // double m_MZ;
  // double m_cosZ;


  // RestFrames frames and friends

  // combinatoric (transverse) tree
  // for jet assignment
  LabRecoFrame*        LAB_comb;
  DecayRecoFrame*      CM_comb;
  DecayRecoFrame*      S_comb;
  VisibleRecoFrame*    ISR_comb;
  VisibleRecoFrame*    J_comb;
  VisibleRecoFrame*    L_comb;
  InvisibleRecoFrame*  I_comb;
  InvisibleGroup*      INV_comb;
  SetMassInvJigsaw*    InvMass_comb;
  CombinatoricGroup*   JETS_comb;
  MinMassesCombJigsaw* SplitJETS_comb;

  // OS 2L tree w/ fixed jet assign.
  LabRecoFrame*        LAB_fix;
  DecayRecoFrame*      CM_fix;
  DecayRecoFrame*      S_fix;
  VisibleRecoFrame*    ISR_fix;

  DecayRecoFrame*      L_fix;  
  VisibleRecoFrame*    L1_fix;
  VisibleRecoFrame*    L2_fix;
 
  InvisibleRecoFrame*  I_fix;

  InvisibleGroup*       INV_fix;
  SetMassInvJigsaw*     InvMass_fix;
  SetRapidityInvJigsaw* InvRapidity_fix;

  // 2L+NJ tree (Z->ll + W/Z->qq)
  LabRecoFrame*        LAB_2LNJ;
 
  DecayRecoFrame*      S_2LNJ;
 

  DecayRecoFrame*      Ca_2LNJ;  
  DecayRecoFrame*      Z_2LNJ;
  VisibleRecoFrame*    L1_2LNJ;
  VisibleRecoFrame*    L2_2LNJ;

  DecayRecoFrame*          Cb_2LNJ;
  SelfAssemblingRecoFrame* JSA_2LNJ;
  VisibleRecoFrame*        J_2LNJ;
  
  InvisibleRecoFrame*  Ia_2LNJ;
  InvisibleRecoFrame*  Ib_2LNJ;

  InvisibleGroup*       INV_2LNJ;
  SetMassInvJigsaw*     InvMass_2LNJ;
  SetRapidityInvJigsaw* InvRapidity_2LNJ;
  ContraBoostInvJigsaw* SplitINV_2LNJ;
  CombinatoricGroup*    JETS_2LNJ;

  // 2L+1L tree (Z->ll + Z/W->l)
  LabRecoFrame*        LAB_2L1L;
 
  DecayRecoFrame*      S_2L1L;
 

  DecayRecoFrame*      Ca_2L1L;
  DecayRecoFrame*      Z_2L1L;  
  VisibleRecoFrame*    L1_2L1L;
  VisibleRecoFrame*    L2_2L1L;

  DecayRecoFrame*      Cb_2L1L;  
  VisibleRecoFrame*    Lb_2L1L;
  
  InvisibleRecoFrame*  Ia_2L1L;
  InvisibleRecoFrame*  Ib_2L1L;

  InvisibleGroup*       INV_2L1L;
  SetMassInvJigsaw*     InvMass_2L1L;
  SetRapidityInvJigsaw* InvRapidity_2L1L;
  ContraBoostInvJigsaw* SplitINV_2L1L;


};

#endif
