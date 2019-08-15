//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul 18 21:46:49 2019 by ROOT version 6.14/04
// from TTree KUAnalysis/KUAnalysis
// found on file: /Users/christopherrogan/Dropbox/SAMPLES/EWKino/StopNtuple_new/All_Bkg_2017/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17.root
//////////////////////////////////////////////////////////

#ifndef ReducedBase_h
#define ReducedBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"

using std::vector;

class ReducedBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        weight;
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
   vector<int>     *ID_lep;
   vector<int>     *Index_lep;
   Int_t           Njet;
   Int_t           Nbjet;
   vector<double>  *PT_jet;
   vector<double>  *Eta_jet;
   vector<double>  *Phi_jet;
   vector<double>  *M_jet;
   vector<double>  *Btag_jet;
   vector<double>  *Flavor_jet;
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
   vector<vector<int> > *index_jet_ISR;
   vector<vector<int> > *index_jet_S;
   vector<vector<int> > *index_lep_ISR;
   vector<vector<int> > *index_lep_S;
   vector<int>     *Njet_a;
   vector<int>     *Njet_b;
   vector<int>     *Nbjet_a;
   vector<int>     *Nbjet_b;
   vector<int>     *Nlep_a;
   vector<int>     *Nlep_b;
   vector<vector<int> > *index_jet_a;
   vector<vector<int> > *index_jet_b;
   vector<vector<int> > *index_lep_a;
   vector<vector<int> > *index_lep_b;
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
   TBranch        *b_weight;   //!
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
   TBranch        *b_ID_lep;   //!
   TBranch        *b_Index_lep;   //!
   TBranch        *b_Njet;   //!
   TBranch        *b_Nbjet;   //!
   TBranch        *b_PT_jet;   //!
   TBranch        *b_Eta_jet;   //!
   TBranch        *b_Phi_jet;   //!
   TBranch        *b_M_jet;   //!
   TBranch        *b_Btag_jet;   //!
   TBranch        *b_Flavor_jet;   //!
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
   TBranch        *b_index_jet_ISR;   //!
   TBranch        *b_index_jet_S;   //!
   TBranch        *b_index_lep_ISR;   //!
   TBranch        *b_index_lep_S;   //!
   TBranch        *b_Njet_a;   //!
   TBranch        *b_Njet_b;   //!
   TBranch        *b_Nbjet_a;   //!
   TBranch        *b_Nbjet_b;   //!
   TBranch        *b_Nlep_a;   //!
   TBranch        *b_Nlep_b;   //!
   TBranch        *b_index_jet_a;   //!
   TBranch        *b_index_jet_b;   //!
   TBranch        *b_index_lep_a;   //!
   TBranch        *b_index_lep_b;   //!
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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/christopherrogan/Dropbox/SAMPLES/EWKino/StopNtuple_new/All_Bkg_2017/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/Users/christopherrogan/Dropbox/SAMPLES/EWKino/StopNtuple_new/All_Bkg_2017/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8_Fall17.root");
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
   ID_lep = 0;
   Index_lep = 0;
   PT_jet = 0;
   Eta_jet = 0;
   Phi_jet = 0;
   M_jet = 0;
   Btag_jet = 0;
   Flavor_jet = 0;
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
   index_jet_ISR = 0;
   index_jet_S = 0;
   index_lep_ISR = 0;
   index_lep_S = 0;
   Njet_a = 0;
   Njet_b = 0;
   Nbjet_a = 0;
   Nbjet_b = 0;
   Nlep_a = 0;
   Nlep_b = 0;
   index_jet_a = 0;
   index_jet_b = 0;
   index_lep_a = 0;
   index_lep_b = 0;
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

   fChain->SetBranchAddress("weight", &weight, &b_weight);
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
   fChain->SetBranchAddress("ID_lep", &ID_lep, &b_ID_lep);
   fChain->SetBranchAddress("Index_lep", &Index_lep, &b_Index_lep);
   fChain->SetBranchAddress("Njet", &Njet, &b_Njet);
   fChain->SetBranchAddress("Nbjet", &Nbjet, &b_Nbjet);
   fChain->SetBranchAddress("PT_jet", &PT_jet, &b_PT_jet);
   fChain->SetBranchAddress("Eta_jet", &Eta_jet, &b_Eta_jet);
   fChain->SetBranchAddress("Phi_jet", &Phi_jet, &b_Phi_jet);
   fChain->SetBranchAddress("M_jet", &M_jet, &b_M_jet);
   fChain->SetBranchAddress("Btag_jet", &Btag_jet, &b_Btag_jet);
   fChain->SetBranchAddress("Flavor_jet", &Flavor_jet, &b_Flavor_jet);
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
   fChain->SetBranchAddress("index_jet_ISR", &index_jet_ISR, &b_index_jet_ISR);
   fChain->SetBranchAddress("index_jet_S", &index_jet_S, &b_index_jet_S);
   fChain->SetBranchAddress("index_lep_ISR", &index_lep_ISR, &b_index_lep_ISR);
   fChain->SetBranchAddress("index_lep_S", &index_lep_S, &b_index_lep_S);
   fChain->SetBranchAddress("Njet_a", &Njet_a, &b_Njet_a);
   fChain->SetBranchAddress("Njet_b", &Njet_b, &b_Njet_b);
   fChain->SetBranchAddress("Nbjet_a", &Nbjet_a, &b_Nbjet_a);
   fChain->SetBranchAddress("Nbjet_b", &Nbjet_b, &b_Nbjet_b);
   fChain->SetBranchAddress("Nlep_a", &Nlep_a, &b_Nlep_a);
   fChain->SetBranchAddress("Nlep_b", &Nlep_b, &b_Nlep_b);
   fChain->SetBranchAddress("index_jet_a", &index_jet_a, &b_index_jet_a);
   fChain->SetBranchAddress("index_jet_b", &index_jet_b, &b_index_jet_b);
   fChain->SetBranchAddress("index_lep_a", &index_lep_a, &b_index_lep_a);
   fChain->SetBranchAddress("index_lep_b", &index_lep_b, &b_index_lep_b);
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
