//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Apr 27 02:59:26 2020 by ROOT version 6.20/04
// from TTree KUAnalysis/KUAnalysis
// found on file: /Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_26_04_20/Fall17_10X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root
//////////////////////////////////////////////////////////

#ifndef ReducedBase_h
#define ReducedBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

using std::vector;

class ReducedBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Bool_t          event_skipped;
   Double_t        weight;
   Double_t        PUweight;
   Double_t        PUweight_up;
   Double_t        PUweight_down;
   Double_t        m_BtagSFweight;
   Double_t        m_BtagSFweight_up;
   Double_t        m_BtagSFweight_down;
   Int_t           runnum;
   Int_t           luminum;
   Long64_t        eventnum;
   Int_t           NPV;
   Int_t           NPU;
   Bool_t          METtrigger;
   Bool_t          METHTtrigger;
   Bool_t          METORtrigger;
   Double_t        MET;
   Double_t        MET_phi;
   Double_t        genMET;
   Double_t        genMET_phi;
   Double_t        HT;
   Int_t           Nele;
   Int_t           Nmu;
   Int_t           Nlep;
   vector<double>  *PT_lep;
   vector<double>  *Eta_lep;
   vector<double>  *Phi_lep;
   vector<double>  *M_lep;
   vector<int>     *Charge_lep;
   vector<int>     *PDGID_lep;
   vector<double>  *RelIso_lep;
   vector<double>  *MiniIso_lep;
   vector<double>  *Dxy_lep;
   vector<double>  *DxyErr_lep;
   vector<double>  *Dz_lep;
   vector<double>  *DzErr_lep;
   vector<double>  *IP3D_lep;
   vector<double>  *SIP3D_lep;
   vector<int>     *ID_lep;
   vector<int>     *Index_lep;
   Int_t           Njet;
   Int_t           Nbjet;
   vector<double>  *PT_jet;
   vector<double>  *Eta_jet;
   vector<double>  *Phi_jet;
   vector<double>  *M_jet;
   vector<double>  *Btag_jet;
   vector<int>     *BtagID_jet;
   vector<double>  *Flavor_jet;
   vector<vector<int> > *index_jet_a;
   vector<vector<int> > *index_jet_b;
   vector<vector<int> > *index_jet_ISR;
   vector<vector<int> > *index_jet_S;
   Int_t           NSV;
   vector<double>  *PT_SV;
   vector<double>  *Eta_SV;
   vector<double>  *Phi_SV;
   vector<double>  *M_SV;
   vector<double>  *ProbB_SV;
   vector<double>  *ProbC_SV;
   Int_t           genNele;
   Int_t           genNmu;
   Int_t           genNlep;
   vector<double>  *genPT_lep;
   vector<double>  *genEta_lep;
   vector<double>  *genPhi_lep;
   vector<double>  *genM_lep;
   vector<int>     *genCharge_lep;
   vector<int>     *genPDGID_lep;
   vector<int>     *genMomPDGID_lep;
   vector<int>     *genIndex_lep;
   Int_t           genNnu;
   vector<double>  *genPT_nu;
   vector<double>  *genEta_nu;
   vector<double>  *genPhi_nu;
   vector<int>     *genPDGID_nu;
   vector<int>     *genMomPDGID_nu;
   Int_t           genNboson;
   vector<double>  *genPT_boson;
   vector<double>  *genEta_boson;
   vector<double>  *genPhi_boson;
   vector<double>  *genM_boson;
   vector<int>     *genPDGID_boson;
   vector<int>     *genMomPDGID_boson;
   Int_t           genNsusy;
   vector<double>  *genPT_susy;
   vector<double>  *genEta_susy;
   vector<double>  *genPhi_susy;
   vector<double>  *genM_susy;
   vector<int>     *genPDGID_susy;
   vector<int>     *genMomPDGID_susy;
   vector<int>     *Njet_ISR;
   vector<int>     *Njet_S;
   vector<int>     *Nbjet_ISR;
   vector<int>     *Nbjet_S;
   vector<int>     *Nlep_ISR;
   vector<int>     *Nlep_S;
   vector<int>     *NSV_ISR;
   vector<int>     *NSV_S;
   vector<vector<int> > *index_SV_ISR;
   vector<vector<int> > *index_SV_S;
   vector<vector<int> > *index_lep_ISR;
   vector<vector<int> > *index_lep_S;
   vector<vector<double> > *dphi_lep_S;
   vector<vector<double> > *cos_lep_S;
   vector<int>     *Njet_a;
   vector<int>     *Njet_b;
   vector<int>     *Nbjet_a;
   vector<int>     *Nbjet_b;
   vector<int>     *Nlep_a;
   vector<int>     *Nlep_b;
   vector<int>     *NSV_a;
   vector<int>     *NSV_b;
   vector<vector<int> > *index_lep_a;
   vector<vector<int> > *index_lep_b;
   vector<vector<int> > *index_SV_a;
   vector<vector<int> > *index_SV_b;
   vector<double>  *PTCM;
   vector<double>  *cosCM;
   vector<double>  *dphiCM;
   vector<double>  *dphiCMI;
   vector<double>  *MS;
   vector<double>  *PS;
   vector<double>  *cosS;
   vector<double>  *dphiS;
   vector<double>  *dphiSI;
   vector<double>  *PTS;
   vector<double>  *PzS;
   vector<double>  *MX3a;
   vector<double>  *cosX3a;
   vector<double>  *MX3b;
   vector<double>  *cosX3b;
   vector<double>  *EVa;
   vector<double>  *EVb;
   vector<double>  *PVa;
   vector<double>  *PVb;
   vector<double>  *EJa;
   vector<double>  *EJb;
   vector<double>  *PJa;
   vector<double>  *PJb;
   vector<double>  *MX2a;
   vector<double>  *cosX2a;
   vector<double>  *MX2b;
   vector<double>  *cosX2b;
   vector<double>  *ELa;
   vector<double>  *ELb;
   vector<double>  *PLa;
   vector<double>  *PLb;
   vector<double>  *MV;
   vector<double>  *PV;
   vector<double>  *MVa;
   vector<double>  *MVb;
   vector<double>  *MJa;
   vector<double>  *MJb;
   vector<double>  *MLa;
   vector<double>  *MLb;
   vector<double>  *cosJa;
   vector<double>  *cosJb;
   vector<double>  *cosLa;
   vector<double>  *cosLb;
   vector<double>  *MJ;
   vector<double>  *ML;
   vector<double>  *EJ;
   vector<double>  *EL;
   vector<double>  *PJ;
   vector<double>  *PL;
   vector<double>  *PX3;
   vector<double>  *PX3_BoostT;
   vector<double>  *MX3a_BoostT;
   vector<double>  *MX3b_BoostT;
   vector<double>  *PV_BoostT;
   vector<double>  *EVa_BoostT;
   vector<double>  *EVb_BoostT;
   vector<double>  *PVa_BoostT;
   vector<double>  *PVb_BoostT;
   vector<double>  *EJ_BoostT;
   vector<double>  *EL_BoostT;
   vector<double>  *PJ_BoostT;
   vector<double>  *PL_BoostT;
   vector<double>  *H11S;
   vector<double>  *H21S;
   vector<double>  *HT21S;
   vector<double>  *H22S;
   vector<double>  *HT22S;
   vector<double>  *H42S;
   vector<double>  *HT42S;
   vector<double>  *H11X3a;
   vector<double>  *H11X3b;
   vector<double>  *H21X3a;
   vector<double>  *H21X3b;
   vector<double>  *PTISR;
   vector<double>  *RISR;
   vector<double>  *MISR;
   Bool_t          Is_1L;
   Bool_t          Is_2L;
   Bool_t          Is_3L;
   Bool_t          Is_4L;

   // List of branches
   TBranch        *b_event_skipped;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_PUweight;   //!
   TBranch        *b_PUweight_up;   //!
   TBranch        *b_PUweight_down;   //!
   TBranch        *b_m_BtagSFweight;   //!
   TBranch        *b_m_BtagSFweight_up;   //!
   TBranch        *b_m_BtagSFweight_down;   //!
   TBranch        *b_runnum;   //!
   TBranch        *b_luminum;   //!
   TBranch        *b_eventnum;   //!
   TBranch        *b_NPV;   //!
   TBranch        *b_NPU;   //!
   TBranch        *b_METtrigger;   //!
   TBranch        *b_METHTtrigger;   //!
   TBranch        *b_METORtrigger;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_MET_phi;   //!
   TBranch        *b_genMET;   //!
   TBranch        *b_genMET_phi;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_Nele;   //!
   TBranch        *b_Nmu;   //!
   TBranch        *b_Nlep;   //!
   TBranch        *b_PT_lep;   //!
   TBranch        *b_Eta_lep;   //!
   TBranch        *b_Phi_lep;   //!
   TBranch        *b_M_lep;   //!
   TBranch        *b_Charge_lep;   //!
   TBranch        *b_PDGID_lep;   //!
   TBranch        *b_RelIso_lep;   //!
   TBranch        *b_MiniIso_lep;   //!
   TBranch        *b_Dxy_lep;   //!
   TBranch        *b_DxyErr_lep;   //!
   TBranch        *b_Dz_lep;   //!
   TBranch        *b_DzErr_lep;   //!
   TBranch        *b_IP3D_lep;   //!
   TBranch        *b_SIP3D_lep;   //!
   TBranch        *b_ID_lep;   //!
   TBranch        *b_Index_lep;   //!
   TBranch        *b_Njet;   //!
   TBranch        *b_Nbjet;   //!
   TBranch        *b_PT_jet;   //!
   TBranch        *b_Eta_jet;   //!
   TBranch        *b_Phi_jet;   //!
   TBranch        *b_M_jet;   //!
   TBranch        *b_Btag_jet;   //!
   TBranch        *b_BtagID_jet;   //!
   TBranch        *b_Flavor_jet;   //!
   TBranch        *b_index_jet_a;   //!
   TBranch        *b_index_jet_b;   //!
   TBranch        *b_index_jet_ISR;   //!
   TBranch        *b_index_jet_S;   //!
   TBranch        *b_NSV;   //!
   TBranch        *b_PT_SV;   //!
   TBranch        *b_Eta_SV;   //!
   TBranch        *b_Phi_SV;   //!
   TBranch        *b_M_SV;   //!
   TBranch        *b_ProbB_SV;   //!
   TBranch        *b_ProbC_SV;   //!
   TBranch        *b_genNele;   //!
   TBranch        *b_genNmu;   //!
   TBranch        *b_genNlep;   //!
   TBranch        *b_genPT_lep;   //!
   TBranch        *b_genEta_lep;   //!
   TBranch        *b_genPhi_lep;   //!
   TBranch        *b_genM_lep;   //!
   TBranch        *b_genCharge_lep;   //!
   TBranch        *b_genPDGID_lep;   //!
   TBranch        *b_genMomPDGID_lep;   //!
   TBranch        *b_genIndex_lep;   //!
   TBranch        *b_genNnu;   //!
   TBranch        *b_genPT_nu;   //!
   TBranch        *b_genEta_nu;   //!
   TBranch        *b_genPhi_nu;   //!
   TBranch        *b_genPDGID_nu;   //!
   TBranch        *b_genMomPDGID_nu;   //!
   TBranch        *b_genNboson;   //!
   TBranch        *b_genPT_boson;   //!
   TBranch        *b_genEta_boson;   //!
   TBranch        *b_genPhi_boson;   //!
   TBranch        *b_genM_boson;   //!
   TBranch        *b_genPDGID_boson;   //!
   TBranch        *b_genMomPDGID_boson;   //!
   TBranch        *b_genNsusy;   //!
   TBranch        *b_genPT_susy;   //!
   TBranch        *b_genEta_susy;   //!
   TBranch        *b_genPhi_susy;   //!
   TBranch        *b_genM_susy;   //!
   TBranch        *b_genPDGID_susy;   //!
   TBranch        *b_genMomPDGID_susy;   //!
   TBranch        *b_Njet_ISR;   //!
   TBranch        *b_Njet_S;   //!
   TBranch        *b_Nbjet_ISR;   //!
   TBranch        *b_Nbjet_S;   //!
   TBranch        *b_Nlep_ISR;   //!
   TBranch        *b_Nlep_S;   //!
   TBranch        *b_NSV_ISR;   //!
   TBranch        *b_NSV_S;   //!
   TBranch        *b_index_SV_ISR;   //!
   TBranch        *b_index_SV_S;   //!
   TBranch        *b_index_lep_ISR;   //!
   TBranch        *b_index_lep_S;   //!
   TBranch        *b_dphi_lep_S;   //!
   TBranch        *b_cos_lep_S;   //!
   TBranch        *b_Njet_a;   //!
   TBranch        *b_Njet_b;   //!
   TBranch        *b_Nbjet_a;   //!
   TBranch        *b_Nbjet_b;   //!
   TBranch        *b_Nlep_a;   //!
   TBranch        *b_Nlep_b;   //!
   TBranch        *b_NSV_a;   //!
   TBranch        *b_NSV_b;   //!
   TBranch        *b_index_lep_a;   //!
   TBranch        *b_index_lep_b;   //!
   TBranch        *b_index_SV_a;   //!
   TBranch        *b_index_SV_b;   //!
   TBranch        *b_PTCM;   //!
   TBranch        *b_cosCM;   //!
   TBranch        *b_dphiCM;   //!
   TBranch        *b_dphiCMI;   //!
   TBranch        *b_MS;   //!
   TBranch        *b_PS;   //!
   TBranch        *b_cosS;   //!
   TBranch        *b_dphiS;   //!
   TBranch        *b_dphiSI;   //!
   TBranch        *b_PTS;   //!
   TBranch        *b_PzS;   //!
   TBranch        *b_MX3a;   //!
   TBranch        *b_cosX3a;   //!
   TBranch        *b_MX3b;   //!
   TBranch        *b_cosX3b;   //!
   TBranch        *b_EVa;   //!
   TBranch        *b_EVb;   //!
   TBranch        *b_PVa;   //!
   TBranch        *b_PVb;   //!
   TBranch        *b_EJa;   //!
   TBranch        *b_EJb;   //!
   TBranch        *b_PJa;   //!
   TBranch        *b_PJb;   //!
   TBranch        *b_MX2a;   //!
   TBranch        *b_cosX2a;   //!
   TBranch        *b_MX2b;   //!
   TBranch        *b_cosX2b;   //!
   TBranch        *b_ELa;   //!
   TBranch        *b_ELb;   //!
   TBranch        *b_PLa;   //!
   TBranch        *b_PLb;   //!
   TBranch        *b_MV;   //!
   TBranch        *b_PV;   //!
   TBranch        *b_MVa;   //!
   TBranch        *b_MVb;   //!
   TBranch        *b_MJa;   //!
   TBranch        *b_MJb;   //!
   TBranch        *b_MLa;   //!
   TBranch        *b_MLb;   //!
   TBranch        *b_cosJa;   //!
   TBranch        *b_cosJb;   //!
   TBranch        *b_cosLa;   //!
   TBranch        *b_cosLb;   //!
   TBranch        *b_MJ;   //!
   TBranch        *b_ML;   //!
   TBranch        *b_EJ;   //!
   TBranch        *b_EL;   //!
   TBranch        *b_PJ;   //!
   TBranch        *b_PL;   //!
   TBranch        *b_PX3;   //!
   TBranch        *b_PX3_BoostT;   //!
   TBranch        *b_MX3a_BoostT;   //!
   TBranch        *b_MX3b_BoostT;   //!
   TBranch        *b_PV_BoostT;   //!
   TBranch        *b_EVa_BoostT;   //!
   TBranch        *b_EVb_BoostT;   //!
   TBranch        *b_PVa_BoostT;   //!
   TBranch        *b_PVb_BoostT;   //!
   TBranch        *b_EJ_BoostT;   //!
   TBranch        *b_EL_BoostT;   //!
   TBranch        *b_PJ_BoostT;   //!
   TBranch        *b_PL_BoostT;   //!
   TBranch        *b_H11S;   //!
   TBranch        *b_H21S;   //!
   TBranch        *b_HT21S;   //!
   TBranch        *b_H22S;   //!
   TBranch        *b_HT22S;   //!
   TBranch        *b_H42S;   //!
   TBranch        *b_HT42S;   //!
   TBranch        *b_H11X3a;   //!
   TBranch        *b_H11X3b;   //!
   TBranch        *b_H21X3a;   //!
   TBranch        *b_H21X3b;   //!
   TBranch        *b_PTISR;   //!
   TBranch        *b_RISR;   //!
   TBranch        *b_MISR;   //!
   TBranch        *b_Is_1L;   //!
   TBranch        *b_Is_2L;   //!
   TBranch        *b_Is_3L;   //!
   TBranch        *b_Is_4L;   //!

   ReducedBase(TTree *tree=0);
   virtual ~ReducedBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

inline ReducedBase::ReducedBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_26_04_20/Fall17_10X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/Users/christopherrogan/Dropbox/SAMPLES/EWKino/NANO/NEW_26_04_20/Fall17_10X/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17_102X.root");
      }
      f->GetObject("KUAnalysis",tree);

   }
   Init(tree);
}

inline ReducedBase::~ReducedBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

inline Int_t ReducedBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
inline Long64_t ReducedBase::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

inline void ReducedBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   PT_lep = 0;
   Eta_lep = 0;
   Phi_lep = 0;
   M_lep = 0;
   Charge_lep = 0;
   PDGID_lep = 0;
   RelIso_lep = 0;
   MiniIso_lep = 0;
   Dxy_lep = 0;
   DxyErr_lep = 0;
   Dz_lep = 0;
   DzErr_lep = 0;
   IP3D_lep = 0;
   SIP3D_lep = 0;
   ID_lep = 0;
   Index_lep = 0;
   PT_jet = 0;
   Eta_jet = 0;
   Phi_jet = 0;
   M_jet = 0;
   Btag_jet = 0;
   BtagID_jet = 0;
   Flavor_jet = 0;
   index_jet_a = 0;
   index_jet_b = 0;
   index_jet_ISR = 0;
   index_jet_S = 0;
   PT_SV = 0;
   Eta_SV = 0;
   Phi_SV = 0;
   M_SV = 0;
   ProbB_SV = 0;
   ProbC_SV = 0;
   genPT_lep = 0;
   genEta_lep = 0;
   genPhi_lep = 0;
   genM_lep = 0;
   genCharge_lep = 0;
   genPDGID_lep = 0;
   genMomPDGID_lep = 0;
   genIndex_lep = 0;
   genPT_nu = 0;
   genEta_nu = 0;
   genPhi_nu = 0;
   genPDGID_nu = 0;
   genMomPDGID_nu = 0;
   genPT_boson = 0;
   genEta_boson = 0;
   genPhi_boson = 0;
   genM_boson = 0;
   genPDGID_boson = 0;
   genMomPDGID_boson = 0;
   genPT_susy = 0;
   genEta_susy = 0;
   genPhi_susy = 0;
   genM_susy = 0;
   genPDGID_susy = 0;
   genMomPDGID_susy = 0;
   Njet_ISR = 0;
   Njet_S = 0;
   Nbjet_ISR = 0;
   Nbjet_S = 0;
   Nlep_ISR = 0;
   Nlep_S = 0;
   NSV_ISR = 0;
   NSV_S = 0;
   index_SV_ISR = 0;
   index_SV_S = 0;
   index_lep_ISR = 0;
   index_lep_S = 0;
   dphi_lep_S = 0;
   cos_lep_S = 0;
   Njet_a = 0;
   Njet_b = 0;
   Nbjet_a = 0;
   Nbjet_b = 0;
   Nlep_a = 0;
   Nlep_b = 0;
   NSV_a = 0;
   NSV_b = 0;
   index_lep_a = 0;
   index_lep_b = 0;
   index_SV_a = 0;
   index_SV_b = 0;
   PTCM = 0;
   cosCM = 0;
   dphiCM = 0;
   dphiCMI = 0;
   MS = 0;
   PS = 0;
   cosS = 0;
   dphiS = 0;
   dphiSI = 0;
   PTS = 0;
   PzS = 0;
   MX3a = 0;
   cosX3a = 0;
   MX3b = 0;
   cosX3b = 0;
   EVa = 0;
   EVb = 0;
   PVa = 0;
   PVb = 0;
   EJa = 0;
   EJb = 0;
   PJa = 0;
   PJb = 0;
   MX2a = 0;
   cosX2a = 0;
   MX2b = 0;
   cosX2b = 0;
   ELa = 0;
   ELb = 0;
   PLa = 0;
   PLb = 0;
   MV = 0;
   PV = 0;
   MVa = 0;
   MVb = 0;
   MJa = 0;
   MJb = 0;
   MLa = 0;
   MLb = 0;
   cosJa = 0;
   cosJb = 0;
   cosLa = 0;
   cosLb = 0;
   MJ = 0;
   ML = 0;
   EJ = 0;
   EL = 0;
   PJ = 0;
   PL = 0;
   PX3 = 0;
   PX3_BoostT = 0;
   MX3a_BoostT = 0;
   MX3b_BoostT = 0;
   PV_BoostT = 0;
   EVa_BoostT = 0;
   EVb_BoostT = 0;
   PVa_BoostT = 0;
   PVb_BoostT = 0;
   EJ_BoostT = 0;
   EL_BoostT = 0;
   PJ_BoostT = 0;
   PL_BoostT = 0;
   H11S = 0;
   H21S = 0;
   HT21S = 0;
   H22S = 0;
   HT22S = 0;
   H42S = 0;
   HT42S = 0;
   H11X3a = 0;
   H11X3b = 0;
   H21X3a = 0;
   H21X3b = 0;
   PTISR = 0;
   RISR = 0;
   MISR = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event_skipped", &event_skipped, &b_event_skipped);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
   fChain->SetBranchAddress("PUweight_up", &PUweight_up, &b_PUweight_up);
   fChain->SetBranchAddress("PUweight_down", &PUweight_down, &b_PUweight_down);
   fChain->SetBranchAddress("m_BtagSFweight", &m_BtagSFweight, &b_m_BtagSFweight);
   fChain->SetBranchAddress("m_BtagSFweight_up", &m_BtagSFweight_up, &b_m_BtagSFweight_up);
   fChain->SetBranchAddress("m_BtagSFweight_down", &m_BtagSFweight_down, &b_m_BtagSFweight_down);
   fChain->SetBranchAddress("runnum", &runnum, &b_runnum);
   fChain->SetBranchAddress("luminum", &luminum, &b_luminum);
   fChain->SetBranchAddress("eventnum", &eventnum, &b_eventnum);
   fChain->SetBranchAddress("NPV", &NPV, &b_NPV);
   fChain->SetBranchAddress("NPU", &NPU, &b_NPU);
   fChain->SetBranchAddress("METtrigger", &METtrigger, &b_METtrigger);
   fChain->SetBranchAddress("METHTtrigger", &METHTtrigger, &b_METHTtrigger);
   fChain->SetBranchAddress("METORtrigger", &METORtrigger, &b_METORtrigger);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("MET_phi", &MET_phi, &b_MET_phi);
   fChain->SetBranchAddress("genMET", &genMET, &b_genMET);
   fChain->SetBranchAddress("genMET_phi", &genMET_phi, &b_genMET_phi);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("Nele", &Nele, &b_Nele);
   fChain->SetBranchAddress("Nmu", &Nmu, &b_Nmu);
   fChain->SetBranchAddress("Nlep", &Nlep, &b_Nlep);
   fChain->SetBranchAddress("PT_lep", &PT_lep, &b_PT_lep);
   fChain->SetBranchAddress("Eta_lep", &Eta_lep, &b_Eta_lep);
   fChain->SetBranchAddress("Phi_lep", &Phi_lep, &b_Phi_lep);
   fChain->SetBranchAddress("M_lep", &M_lep, &b_M_lep);
   fChain->SetBranchAddress("Charge_lep", &Charge_lep, &b_Charge_lep);
   fChain->SetBranchAddress("PDGID_lep", &PDGID_lep, &b_PDGID_lep);
   fChain->SetBranchAddress("RelIso_lep", &RelIso_lep, &b_RelIso_lep);
   fChain->SetBranchAddress("MiniIso_lep", &MiniIso_lep, &b_MiniIso_lep);
   fChain->SetBranchAddress("Dxy_lep", &Dxy_lep, &b_Dxy_lep);
   fChain->SetBranchAddress("DxyErr_lep", &DxyErr_lep, &b_DxyErr_lep);
   fChain->SetBranchAddress("Dz_lep", &Dz_lep, &b_Dz_lep);
   fChain->SetBranchAddress("DzErr_lep", &DzErr_lep, &b_DzErr_lep);
   fChain->SetBranchAddress("IP3D_lep", &IP3D_lep, &b_IP3D_lep);
   fChain->SetBranchAddress("SIP3D_lep", &SIP3D_lep, &b_SIP3D_lep);
   fChain->SetBranchAddress("ID_lep", &ID_lep, &b_ID_lep);
   fChain->SetBranchAddress("Index_lep", &Index_lep, &b_Index_lep);
   fChain->SetBranchAddress("Njet", &Njet, &b_Njet);
   fChain->SetBranchAddress("Nbjet", &Nbjet, &b_Nbjet);
   fChain->SetBranchAddress("PT_jet", &PT_jet, &b_PT_jet);
   fChain->SetBranchAddress("Eta_jet", &Eta_jet, &b_Eta_jet);
   fChain->SetBranchAddress("Phi_jet", &Phi_jet, &b_Phi_jet);
   fChain->SetBranchAddress("M_jet", &M_jet, &b_M_jet);
   fChain->SetBranchAddress("Btag_jet", &Btag_jet, &b_Btag_jet);
   fChain->SetBranchAddress("BtagID_jet", &BtagID_jet, &b_BtagID_jet);
   fChain->SetBranchAddress("Flavor_jet", &Flavor_jet, &b_Flavor_jet);
   fChain->SetBranchAddress("index_jet_a", &index_jet_a, &b_index_jet_a);
   fChain->SetBranchAddress("index_jet_b", &index_jet_b, &b_index_jet_b);
   fChain->SetBranchAddress("index_jet_ISR", &index_jet_ISR, &b_index_jet_ISR);
   fChain->SetBranchAddress("index_jet_S", &index_jet_S, &b_index_jet_S);
   fChain->SetBranchAddress("NSV", &NSV, &b_NSV);
   fChain->SetBranchAddress("PT_SV", &PT_SV, &b_PT_SV);
   fChain->SetBranchAddress("Eta_SV", &Eta_SV, &b_Eta_SV);
   fChain->SetBranchAddress("Phi_SV", &Phi_SV, &b_Phi_SV);
   fChain->SetBranchAddress("M_SV", &M_SV, &b_M_SV);
   fChain->SetBranchAddress("ProbB_SV", &ProbB_SV, &b_ProbB_SV);
   fChain->SetBranchAddress("ProbC_SV", &ProbC_SV, &b_ProbC_SV);
   fChain->SetBranchAddress("genNele", &genNele, &b_genNele);
   fChain->SetBranchAddress("genNmu", &genNmu, &b_genNmu);
   fChain->SetBranchAddress("genNlep", &genNlep, &b_genNlep);
   fChain->SetBranchAddress("genPT_lep", &genPT_lep, &b_genPT_lep);
   fChain->SetBranchAddress("genEta_lep", &genEta_lep, &b_genEta_lep);
   fChain->SetBranchAddress("genPhi_lep", &genPhi_lep, &b_genPhi_lep);
   fChain->SetBranchAddress("genM_lep", &genM_lep, &b_genM_lep);
   fChain->SetBranchAddress("genCharge_lep", &genCharge_lep, &b_genCharge_lep);
   fChain->SetBranchAddress("genPDGID_lep", &genPDGID_lep, &b_genPDGID_lep);
   fChain->SetBranchAddress("genMomPDGID_lep", &genMomPDGID_lep, &b_genMomPDGID_lep);
   fChain->SetBranchAddress("genIndex_lep", &genIndex_lep, &b_genIndex_lep);
   fChain->SetBranchAddress("genNnu", &genNnu, &b_genNnu);
   fChain->SetBranchAddress("genPT_nu", &genPT_nu, &b_genPT_nu);
   fChain->SetBranchAddress("genEta_nu", &genEta_nu, &b_genEta_nu);
   fChain->SetBranchAddress("genPhi_nu", &genPhi_nu, &b_genPhi_nu);
   fChain->SetBranchAddress("genPDGID_nu", &genPDGID_nu, &b_genPDGID_nu);
   fChain->SetBranchAddress("genMomPDGID_nu", &genMomPDGID_nu, &b_genMomPDGID_nu);
   fChain->SetBranchAddress("genNboson", &genNboson, &b_genNboson);
   fChain->SetBranchAddress("genPT_boson", &genPT_boson, &b_genPT_boson);
   fChain->SetBranchAddress("genEta_boson", &genEta_boson, &b_genEta_boson);
   fChain->SetBranchAddress("genPhi_boson", &genPhi_boson, &b_genPhi_boson);
   fChain->SetBranchAddress("genM_boson", &genM_boson, &b_genM_boson);
   fChain->SetBranchAddress("genPDGID_boson", &genPDGID_boson, &b_genPDGID_boson);
   fChain->SetBranchAddress("genMomPDGID_boson", &genMomPDGID_boson, &b_genMomPDGID_boson);
   fChain->SetBranchAddress("genNsusy", &genNsusy, &b_genNsusy);
   fChain->SetBranchAddress("genPT_susy", &genPT_susy, &b_genPT_susy);
   fChain->SetBranchAddress("genEta_susy", &genEta_susy, &b_genEta_susy);
   fChain->SetBranchAddress("genPhi_susy", &genPhi_susy, &b_genPhi_susy);
   fChain->SetBranchAddress("genM_susy", &genM_susy, &b_genM_susy);
   fChain->SetBranchAddress("genPDGID_susy", &genPDGID_susy, &b_genPDGID_susy);
   fChain->SetBranchAddress("genMomPDGID_susy", &genMomPDGID_susy, &b_genMomPDGID_susy);
   fChain->SetBranchAddress("Njet_ISR", &Njet_ISR, &b_Njet_ISR);
   fChain->SetBranchAddress("Njet_S", &Njet_S, &b_Njet_S);
   fChain->SetBranchAddress("Nbjet_ISR", &Nbjet_ISR, &b_Nbjet_ISR);
   fChain->SetBranchAddress("Nbjet_S", &Nbjet_S, &b_Nbjet_S);
   fChain->SetBranchAddress("Nlep_ISR", &Nlep_ISR, &b_Nlep_ISR);
   fChain->SetBranchAddress("Nlep_S", &Nlep_S, &b_Nlep_S);
   fChain->SetBranchAddress("NSV_ISR", &NSV_ISR, &b_NSV_ISR);
   fChain->SetBranchAddress("NSV_S", &NSV_S, &b_NSV_S);
   fChain->SetBranchAddress("index_SV_ISR", &index_SV_ISR, &b_index_SV_ISR);
   fChain->SetBranchAddress("index_SV_S", &index_SV_S, &b_index_SV_S);
   fChain->SetBranchAddress("index_lep_ISR", &index_lep_ISR, &b_index_lep_ISR);
   fChain->SetBranchAddress("index_lep_S", &index_lep_S, &b_index_lep_S);
   fChain->SetBranchAddress("dphi_lep_S", &dphi_lep_S, &b_dphi_lep_S);
   fChain->SetBranchAddress("cos_lep_S", &cos_lep_S, &b_cos_lep_S);
   fChain->SetBranchAddress("Njet_a", &Njet_a, &b_Njet_a);
   fChain->SetBranchAddress("Njet_b", &Njet_b, &b_Njet_b);
   fChain->SetBranchAddress("Nbjet_a", &Nbjet_a, &b_Nbjet_a);
   fChain->SetBranchAddress("Nbjet_b", &Nbjet_b, &b_Nbjet_b);
   fChain->SetBranchAddress("Nlep_a", &Nlep_a, &b_Nlep_a);
   fChain->SetBranchAddress("Nlep_b", &Nlep_b, &b_Nlep_b);
   fChain->SetBranchAddress("NSV_a", &NSV_a, &b_NSV_a);
   fChain->SetBranchAddress("NSV_b", &NSV_b, &b_NSV_b);
   fChain->SetBranchAddress("index_lep_a", &index_lep_a, &b_index_lep_a);
   fChain->SetBranchAddress("index_lep_b", &index_lep_b, &b_index_lep_b);
   fChain->SetBranchAddress("index_SV_a", &index_SV_a, &b_index_SV_a);
   fChain->SetBranchAddress("index_SV_b", &index_SV_b, &b_index_SV_b);
   fChain->SetBranchAddress("PTCM", &PTCM, &b_PTCM);
   fChain->SetBranchAddress("cosCM", &cosCM, &b_cosCM);
   fChain->SetBranchAddress("dphiCM", &dphiCM, &b_dphiCM);
   fChain->SetBranchAddress("dphiCMI", &dphiCMI, &b_dphiCMI);
   fChain->SetBranchAddress("MS", &MS, &b_MS);
   fChain->SetBranchAddress("PS", &PS, &b_PS);
   fChain->SetBranchAddress("cosS", &cosS, &b_cosS);
   fChain->SetBranchAddress("dphiS", &dphiS, &b_dphiS);
   fChain->SetBranchAddress("dphiSI", &dphiSI, &b_dphiSI);
   fChain->SetBranchAddress("PTS", &PTS, &b_PTS);
   fChain->SetBranchAddress("PzS", &PzS, &b_PzS);
   fChain->SetBranchAddress("MX3a", &MX3a, &b_MX3a);
   fChain->SetBranchAddress("cosX3a", &cosX3a, &b_cosX3a);
   fChain->SetBranchAddress("MX3b", &MX3b, &b_MX3b);
   fChain->SetBranchAddress("cosX3b", &cosX3b, &b_cosX3b);
   fChain->SetBranchAddress("EVa", &EVa, &b_EVa);
   fChain->SetBranchAddress("EVb", &EVb, &b_EVb);
   fChain->SetBranchAddress("PVa", &PVa, &b_PVa);
   fChain->SetBranchAddress("PVb", &PVb, &b_PVb);
   fChain->SetBranchAddress("EJa", &EJa, &b_EJa);
   fChain->SetBranchAddress("EJb", &EJb, &b_EJb);
   fChain->SetBranchAddress("PJa", &PJa, &b_PJa);
   fChain->SetBranchAddress("PJb", &PJb, &b_PJb);
   fChain->SetBranchAddress("MX2a", &MX2a, &b_MX2a);
   fChain->SetBranchAddress("cosX2a", &cosX2a, &b_cosX2a);
   fChain->SetBranchAddress("MX2b", &MX2b, &b_MX2b);
   fChain->SetBranchAddress("cosX2b", &cosX2b, &b_cosX2b);
   fChain->SetBranchAddress("ELa", &ELa, &b_ELa);
   fChain->SetBranchAddress("ELb", &ELb, &b_ELb);
   fChain->SetBranchAddress("PLa", &PLa, &b_PLa);
   fChain->SetBranchAddress("PLb", &PLb, &b_PLb);
   fChain->SetBranchAddress("MV", &MV, &b_MV);
   fChain->SetBranchAddress("PV", &PV, &b_PV);
   fChain->SetBranchAddress("MVa", &MVa, &b_MVa);
   fChain->SetBranchAddress("MVb", &MVb, &b_MVb);
   fChain->SetBranchAddress("MJa", &MJa, &b_MJa);
   fChain->SetBranchAddress("MJb", &MJb, &b_MJb);
   fChain->SetBranchAddress("MLa", &MLa, &b_MLa);
   fChain->SetBranchAddress("MLb", &MLb, &b_MLb);
   fChain->SetBranchAddress("cosJa", &cosJa, &b_cosJa);
   fChain->SetBranchAddress("cosJb", &cosJb, &b_cosJb);
   fChain->SetBranchAddress("cosLa", &cosLa, &b_cosLa);
   fChain->SetBranchAddress("cosLb", &cosLb, &b_cosLb);
   fChain->SetBranchAddress("MJ", &MJ, &b_MJ);
   fChain->SetBranchAddress("ML", &ML, &b_ML);
   fChain->SetBranchAddress("EJ", &EJ, &b_EJ);
   fChain->SetBranchAddress("EL", &EL, &b_EL);
   fChain->SetBranchAddress("PJ", &PJ, &b_PJ);
   fChain->SetBranchAddress("PL", &PL, &b_PL);
   fChain->SetBranchAddress("PX3", &PX3, &b_PX3);
   fChain->SetBranchAddress("PX3_BoostT", &PX3_BoostT, &b_PX3_BoostT);
   fChain->SetBranchAddress("MX3a_BoostT", &MX3a_BoostT, &b_MX3a_BoostT);
   fChain->SetBranchAddress("MX3b_BoostT", &MX3b_BoostT, &b_MX3b_BoostT);
   fChain->SetBranchAddress("PV_BoostT", &PV_BoostT, &b_PV_BoostT);
   fChain->SetBranchAddress("EVa_BoostT", &EVa_BoostT, &b_EVa_BoostT);
   fChain->SetBranchAddress("EVb_BoostT", &EVb_BoostT, &b_EVb_BoostT);
   fChain->SetBranchAddress("PVa_BoostT", &PVa_BoostT, &b_PVa_BoostT);
   fChain->SetBranchAddress("PVb_BoostT", &PVb_BoostT, &b_PVb_BoostT);
   fChain->SetBranchAddress("EJ_BoostT", &EJ_BoostT, &b_EJ_BoostT);
   fChain->SetBranchAddress("EL_BoostT", &EL_BoostT, &b_EL_BoostT);
   fChain->SetBranchAddress("PJ_BoostT", &PJ_BoostT, &b_PJ_BoostT);
   fChain->SetBranchAddress("PL_BoostT", &PL_BoostT, &b_PL_BoostT);
   fChain->SetBranchAddress("H11S", &H11S, &b_H11S);
   fChain->SetBranchAddress("H21S", &H21S, &b_H21S);
   fChain->SetBranchAddress("HT21S", &HT21S, &b_HT21S);
   fChain->SetBranchAddress("H22S", &H22S, &b_H22S);
   fChain->SetBranchAddress("HT22S", &HT22S, &b_HT22S);
   fChain->SetBranchAddress("H42S", &H42S, &b_H42S);
   fChain->SetBranchAddress("HT42S", &HT42S, &b_HT42S);
   fChain->SetBranchAddress("H11X3a", &H11X3a, &b_H11X3a);
   fChain->SetBranchAddress("H11X3b", &H11X3b, &b_H11X3b);
   fChain->SetBranchAddress("H21X3a", &H21X3a, &b_H21X3a);
   fChain->SetBranchAddress("H21X3b", &H21X3b, &b_H21X3b);
   fChain->SetBranchAddress("PTISR", &PTISR, &b_PTISR);
   fChain->SetBranchAddress("RISR", &RISR, &b_RISR);
   fChain->SetBranchAddress("MISR", &MISR, &b_MISR);
   fChain->SetBranchAddress("Is_1L", &Is_1L, &b_Is_1L);
   fChain->SetBranchAddress("Is_2L", &Is_2L, &b_Is_2L);
   fChain->SetBranchAddress("Is_3L", &Is_3L, &b_Is_3L);
   fChain->SetBranchAddress("Is_4L", &Is_4L, &b_Is_4L);
   Notify();

   fChain->SetBranchStatus("*",0);
   fChain->SetBranchStatus("*weight",1);
   fChain->SetBranchStatus("MET",1);
   fChain->SetBranchStatus("MET_phi",1);
   fChain->SetBranchStatus("RISR",1);
   fChain->SetBranchStatus("PTISR",1);
   fChain->SetBranchStatus("*lep",1);
   fChain->SetBranchStatus("Nlep*",1);
   fChain->SetBranchStatus("NSV*",1);
   fChain->SetBranchStatus("Nbjet*",1);
   fChain->SetBranchStatus("Njet*",1);
   //fChain->SetBranchStatus("MS",1);
   //fChain->SetBranchStatus("PVa",1);
   //fChain->SetBranchStatus("PVb",1);
   //fChain->SetBranchStatus("PzS",1);
   //fChain->SetBranchStatus("PTS",1);
   //fChain->SetBranchStatus("H11S",1);
   // fChain->SetBranchStatus("dphiSI",1);
   // fChain->SetBranchStatus("dphiCMI",1);
   // fChain->SetBranchStatus("MX3a",1);
   // fChain->SetBranchStatus("MX3b",1);
   // fChain->SetBranchStatus("MVa",1);
   // fChain->SetBranchStatus("MVb",1);
   // fChain->SetBranchStatus("EVa",1);
   // fChain->SetBranchStatus("EVb",1);
   fChain->SetBranchStatus("*_BoostT",1);
   // fChain->SetBranchStatus("ML",1);
   // fChain->SetBranchStatus("EL",1);
   fChain->SetBranchStatus("cosX*",1);
   fChain->SetBranchStatus("index*",1);
   // fChain->SetBranchStatus("*jet",1);
   // fChain->SetBranchStatus("Eta_SV",1);
   // fChain->SetBranchStatus("Phi_SV",1);
   // fChain->SetBranchStatus("index_SV_b",1);
   //fChain->SetBranchStatus("*jet",1);
   fChain->SetBranchStatus("genPDGID_lep", 1);
   fChain->SetBranchStatus("genMomPDGID_lep", 1);
}

inline Bool_t ReducedBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

inline void ReducedBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
inline Int_t ReducedBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
