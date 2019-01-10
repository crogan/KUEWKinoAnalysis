//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 28 20:18:09 2018 by ROOT version 6.10/08
// from TTree SOSAnalysis/SOSAnalysis
// found on file: /Users/crogan/Dropbox/SAMPLES/SOS/NTUPLES/SIG/TChiWZ_300_100.root
//////////////////////////////////////////////////////////

#ifndef ReducedBase_h
#define ReducedBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class ReducedBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        weight;
   Double_t        MET;
   Double_t        MET_phi;
   Double_t        HT;
   Bool_t          Is_SF;
   Int_t           nEl;
   Int_t           nMu;
   Int_t           nBjet;
   Double_t        pT_1lep;
   Int_t           id_1lep;
   Double_t        pT_2lep;
   Int_t           id_2lep;
   Double_t        pT_3lep;
   Int_t           id_3lep;
   Int_t           Nj;
   Int_t           NjS;
   Int_t           NjISR;
   Double_t        PTCM_comb;
   Double_t        PTISR_comb;
   Double_t        RISR_comb;
   Double_t        cosCM_comb;
   Double_t        cosS_comb;
   Double_t        MISR_comb;
   Double_t        MS_comb;
   Double_t        dphiCMI_comb;
   Double_t        dphiSI_comb;
   Double_t        dphiISRI_comb;
   Double_t        PTCM_fix;
   Double_t        PTISR_fix;
   Double_t        RISR_fix;
   Double_t        cosCM_fix;
   Double_t        cosS_fix;
   Double_t        MISR_fix;
   Double_t        MS_fix;
   Double_t        dphiCMI_fix;
   Double_t        dphiSI_fix;
   Double_t        dphiISRI_fix;
   Double_t        MZ;
   Double_t        cosZ;
   Bool_t          Is_2LNJ;
   Bool_t          Is_2L1L;
   Double_t        HN2S;
   Double_t        HN2SR;
   Double_t        H11S;
   Double_t        HN1Ca;
   Double_t        HN1Cb;
   Double_t        H11Ca;
   Double_t        H11Cb;
   Double_t        cosC;
  
   Double_t        MJ;
  
   Double_t        cosJ;

   // List of branches
   TBranch        *b_weight;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_MET_phi;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_Is_SF;   //!
   TBranch        *b_nEl;   //!
   TBranch        *b_nMu;   //!
   TBranch        *b_nBjet;   //!
   TBranch        *b_pT_1lep;   //!
   TBranch        *b_id_1lep;   //!
   TBranch        *b_pT_2lep;   //!
   TBranch        *b_id_2lep;   //!
   TBranch        *b_pT_3lep;   //!
   TBranch        *b_id_3lep;   //!
   TBranch        *b_Nj;   //!
   TBranch        *b_NjS;   //!
   TBranch        *b_NjISR;   //!
   TBranch        *b_PTCM_comb;   //!
   TBranch        *b_PTISR_comb;   //!
   TBranch        *b_RISR_comb;   //!
   TBranch        *b_cosCM_comb;   //!
   TBranch        *b_cosS_comb;   //!
   TBranch        *b_MISR_comb;   //!
   TBranch        *b_MS_comb;   //!
   TBranch        *b_dphiCMI_comb;   //!
   TBranch        *b_dphiSI_comb;   //!
   TBranch        *b_dphiISRI_comb;   //!
   TBranch        *b_PTCM_fix;   //!
   TBranch        *b_PTISR_fix;   //!
   TBranch        *b_RISR_fix;   //!
   TBranch        *b_cosCM_fix;   //!
   TBranch        *b_cosS_fix;   //!
   TBranch        *b_MISR_fix;   //!
   TBranch        *b_MS_fix;   //!
   TBranch        *b_dphiCMI_fix;   //!
   TBranch        *b_dphiSI_fix;   //!
   TBranch        *b_dphiISRI_fix;   //!
   TBranch        *b_MZ;   //!
   TBranch        *b_cosZ;   //!
   TBranch        *b_Is_2LNJ;   //!
   TBranch        *b_Is_2L1L;   //!
   TBranch        *b_HN2S;   //!
   TBranch        *b_HN2SR;   //!
   TBranch        *b_H11S;   //!
   TBranch        *b_HN1Ca;   //!
   TBranch        *b_HN1Cb;   //!
   TBranch        *b_H11Ca;   //!
   TBranch        *b_H11Cb;   //!
   TBranch        *b_cosC;   //!
   
   TBranch        *b_MJ;   //!
     TBranch        *b_cosJ;   //!

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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/crogan/Dropbox/SAMPLES/SOS/NTUPLES/SIG/TChiWZ_300_100.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/Users/crogan/Dropbox/SAMPLES/SOS/NTUPLES/SIG/TChiWZ_300_100.root");
      }
      f->GetObject("SOSAnalysis",tree);

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

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("MET_phi", &MET_phi, &b_MET_phi);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("Is_SF", &Is_SF, &b_Is_SF);
   fChain->SetBranchAddress("nEl", &nEl, &b_nEl);
   fChain->SetBranchAddress("nMu", &nMu, &b_nMu);
   fChain->SetBranchAddress("nBjet", &nBjet, &b_nBjet);
   fChain->SetBranchAddress("pT_1lep", &pT_1lep, &b_pT_1lep);
   fChain->SetBranchAddress("id_1lep", &id_1lep, &b_id_1lep);
   fChain->SetBranchAddress("pT_2lep", &pT_2lep, &b_pT_2lep);
   fChain->SetBranchAddress("id_2lep", &id_2lep, &b_id_2lep);
   fChain->SetBranchAddress("pT_3lep", &pT_3lep, &b_pT_3lep);
   fChain->SetBranchAddress("id_3lep", &id_3lep, &b_id_3lep);
   fChain->SetBranchAddress("Nj", &Nj, &b_Nj);
   fChain->SetBranchAddress("NjS", &NjS, &b_NjS);
   fChain->SetBranchAddress("NjISR", &NjISR, &b_NjISR);
   fChain->SetBranchAddress("PTCM_comb", &PTCM_comb, &b_PTCM_comb);
   fChain->SetBranchAddress("PTISR_comb", &PTISR_comb, &b_PTISR_comb);
   fChain->SetBranchAddress("RISR_comb", &RISR_comb, &b_RISR_comb);
   fChain->SetBranchAddress("cosCM_comb", &cosCM_comb, &b_cosCM_comb);
   fChain->SetBranchAddress("cosS_comb", &cosS_comb, &b_cosS_comb);
   fChain->SetBranchAddress("MISR_comb", &MISR_comb, &b_MISR_comb);
   fChain->SetBranchAddress("MS_comb", &MS_comb, &b_MS_comb);
   fChain->SetBranchAddress("dphiCMI_comb", &dphiCMI_comb, &b_dphiCMI_comb);
   fChain->SetBranchAddress("dphiSI_comb", &dphiSI_comb, &b_dphiSI_comb);
   fChain->SetBranchAddress("dphiISRI_comb", &dphiISRI_comb, &b_dphiISRI_comb);
   fChain->SetBranchAddress("PTCM_fix", &PTCM_fix, &b_PTCM_fix);
   fChain->SetBranchAddress("PTISR_fix", &PTISR_fix, &b_PTISR_fix);
   fChain->SetBranchAddress("RISR_fix", &RISR_fix, &b_RISR_fix);
   fChain->SetBranchAddress("cosCM_fix", &cosCM_fix, &b_cosCM_fix);
   fChain->SetBranchAddress("cosS_fix", &cosS_fix, &b_cosS_fix);
   fChain->SetBranchAddress("MISR_fix", &MISR_fix, &b_MISR_fix);
   fChain->SetBranchAddress("MS_fix", &MS_fix, &b_MS_fix);
   fChain->SetBranchAddress("dphiCMI_fix", &dphiCMI_fix, &b_dphiCMI_fix);
   fChain->SetBranchAddress("dphiSI_fix", &dphiSI_fix, &b_dphiSI_fix);
   fChain->SetBranchAddress("dphiISRI_fix", &dphiISRI_fix, &b_dphiISRI_fix);
   fChain->SetBranchAddress("MZ", &MZ, &b_MZ);
   fChain->SetBranchAddress("cosZ", &cosZ, &b_cosZ);
   fChain->SetBranchAddress("Is_2LNJ", &Is_2LNJ, &b_Is_2LNJ);
   fChain->SetBranchAddress("Is_2L1L", &Is_2L1L, &b_Is_2L1L);
   fChain->SetBranchAddress("HN2S", &HN2S, &b_HN2S);
   fChain->SetBranchAddress("HN2SR", &HN2SR, &b_HN2SR);
   fChain->SetBranchAddress("H11S", &H11S, &b_H11S);
   fChain->SetBranchAddress("HN1Ca", &HN1Ca, &b_HN1Ca);
   fChain->SetBranchAddress("HN1Cb", &HN1Cb, &b_HN1Cb);
   fChain->SetBranchAddress("H11Ca", &H11Ca, &b_H11Ca);
   fChain->SetBranchAddress("H11Cb", &H11Cb, &b_H11Cb);
   fChain->SetBranchAddress("cosC", &cosC, &b_cosC);
//    fChain->SetBranchAddress("MZ", &MZ, &b_MZ);
   fChain->SetBranchAddress("MJ", &MJ, &b_MJ);
//    fChain->SetBranchAddress("cosZ", &cosZ, &b_cosZ);
   fChain->SetBranchAddress("cosJ", &cosJ, &b_cosJ);
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

