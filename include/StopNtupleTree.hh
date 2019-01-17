//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jan 17 11:45:25 2019 by ROOT version 6.10/08
// from TTree AUX/AUX
// found on file: stopFlatNtuples_7.root
//////////////////////////////////////////////////////////

#ifndef StopNtupleTree_h
#define StopNtupleTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"

using std::vector;
using std::string;

class StopNtupleTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          event;
   Double_t        genHT;
   Double_t        genmet;
   Double_t        genmetphi;
   Double_t        x1;
   Double_t        x2;
   Double_t        q;
   Double_t        mht;
   Double_t        mhtphi;
   Double_t        mt2;
   Double_t        ht;
   Double_t        met;
   Double_t        metphi;
   Double_t        calomet;
   Double_t        calometphi;
   Double_t        dPhi0_CUT;
   Double_t        dPhi1_CUT;
   Double_t        dPhi2_CUT;
   Double_t        tru_npv;
   Double_t        avg_npv;
   Double_t        stored_weight;
   Double_t        evtWeight;
   Int_t           globalTightHalo2016Filter;
   Int_t           goodVerticesFilter;
   Int_t           eeBadScFilter;
   Int_t           EcalDeadCellTriggerPrimitiveFilter;
   Int_t           noBadMuonsFilter;
   Int_t           badMuonsFilter;
   Int_t           duplicateMuonsFilter;
   Int_t           nMuons_CUT;
   Int_t           nMuons;
   Int_t           nElectrons_CUT;
   Int_t           nElectrons;
   Int_t           nJets;
   Int_t           NJetsISR;
   Int_t           id1;
   Int_t           id2;
   Int_t           loose_nIsoTrks;
   Int_t           nIsoTrks_CUT;
   Int_t           nJets_CUT;
   Int_t           vtxSize;
   Int_t           npv;
   Int_t           nm1;
   Int_t           n0;
   Int_t           np1;
   UInt_t          looseJetID;
   UInt_t          tightJetID;
   UInt_t          tightlepvetoJetID;
   UInt_t          looseJetID_NoLep;
   UInt_t          tightJetID_NoLep;
   UInt_t          tightlepvetoJetID_NoLep;
   UInt_t          BadChargedCandidateFilter;
   UInt_t          BadPFMuonFilter;
   UInt_t          HBHENoiseFilter;
   UInt_t          HBHEIsoNoiseFilter;
   vector<double>  *svPT;
   vector<double>  *svETA;
   vector<double>  *svPhi;
   vector<double>  *svMass;
   vector<double>  *svNTracks;
   vector<double>  *svChi2;
   vector<double>  *svNDF;
   vector<double>  *svDXY;
   vector<double>  *svDXYerr;
   vector<double>  *svD3D;
   vector<double>  *svD3Derr;
   vector<double>  *svCosThetaSVPS;
   vector<double>  *muonsCharge;
   vector<double>  *muonsMtw;
   vector<double>  *muonsRelIso;
   vector<double>  *muonsMiniIso;
   vector<double>  *muonspfActivity;
   vector<double>  *specialFixMuonsCharge;
   vector<double>  *pfGammaIso;
   vector<double>  *isEB;
   vector<double>  *genMatched;
   vector<double>  *hadTowOverEM;
   vector<double>  *sigmaIetaIeta;
   vector<double>  *pfChargedIso;
   vector<double>  *pfNeutralIso;
   vector<double>  *pfChargedIsoRhoCorr;
   vector<double>  *pfNeutralIsoRhoCorr;
   vector<double>  *pfGammaIsoRhoCorr;
   vector<double>  *hasPixelSeed;
   vector<double>  *passElectronVeto;
   vector<double>  *photonPt;
   vector<double>  *photonEta;
   vector<double>  *photonPhi;
   vector<double>  *elesCharge;
   vector<double>  *elesMtw;
   vector<double>  *elesRelIso;
   vector<double>  *elesMiniIso;
   vector<double>  *elespfActivity;
   vector<double>  *recoJetsJecUnc;
   vector<double>  *recoJetsJecScaleRawToFull;
   vector<double>  *qgLikelihood;
   vector<double>  *qgPtD;
   vector<double>  *qgAxis2;
   vector<double>  *recoJetschargedHadronEnergyFraction;
   vector<double>  *recoJetschargedEmEnergyFraction;
   vector<double>  *recoJetsneutralEmEnergyFraction;
   vector<double>  *recoJetsmuonEnergyFraction;
   vector<double>  *recoJetsBtag_0;
   vector<double>  *recoJetsCharge_0;
   vector<double>  *tau1;
   vector<double>  *tau2;
   vector<double>  *tau3;
   vector<double>  *softDropMass;
   vector<double>  *ak8SubJetsBdisc;
   vector<double>  *puppitau1;
   vector<double>  *puppitau2;
   vector<double>  *puppitau3;
   vector<double>  *puppisoftDropMass;
   vector<double>  *puppiSubJetsBdisc;
   vector<double>  *recoJetsJecUncLepCleaned;
   vector<double>  *prodJetsNoLep_qgLikelihood;
   vector<double>  *prodJetsNoLep_qgPtD;
   vector<double>  *prodJetsNoLep_qgAxis2;
   vector<double>  *recoJetschargedHadronEnergyFractionLepCleaned;
   vector<double>  *recoJetsneutralEmEnergyFractionLepCleaned;
   vector<double>  *recoJetschargedEmEnergyFractionLepCleaned;
   vector<double>  *recoJetsmuonEnergyFractionLepCleaned;
   vector<double>  *recoJetsBtag_0_LepCleaned;
   vector<double>  *recoJetsCharge_0_LepCleaned;
   vector<double>  *recoJetsJecScaleRawToFull_LepCleaned;
   vector<double>  *prodJetsNoLep_tau1;
   vector<double>  *prodJetsNoLep_tau2;
   vector<double>  *prodJetsNoLep_tau3;
   vector<double>  *prodJetsNoLep_puppisoftDropMass;
   vector<double>  *prodJetsNoLep_puppitau1;
   vector<double>  *prodJetsNoLep_puppitau2;
   vector<double>  *prodJetsNoLep_puppitau3;
   vector<double>  *prodJetsNoLep_puppiSubJetsBdisc;
   vector<double>  *W_emu_pfActivityVec;
   vector<double>  *W_tau_emu_pfActivityVec;
   vector<double>  *W_tau_prongs_pfActivityVec;
   vector<double>  *ScaleWeightsMiniAOD;
   vector<double>  *trksForIsoVeto_charge;
   vector<double>  *trksForIsoVeto_dz;
   vector<double>  *trksForIsoVeto_iso;
   vector<double>  *trksForIsoVeto_pfActivity;
   vector<double>  *loose_isoTrks_charge;
   vector<double>  *loose_isoTrks_dz;
   vector<double>  *loose_isoTrks_iso;
   vector<double>  *loose_isoTrks_mtw;
   vector<double>  *loose_isoTrks_pfActivity;
   vector<double>  *metMagUp;
   vector<double>  *metMagDown;
   vector<double>  *metPhiUp;
   vector<double>  *metPhiDown;
   vector<int>     *PassTrigger;
   vector<int>     *TriggerPrescales;
   vector<int>     *muonsFlagMedium;
   vector<int>     *muonsFlagTight;
   vector<int>     *specialFixtype;
   vector<int>     *elesFlagMedium;
   vector<int>     *elesFlagVeto;
   vector<int>     *recoJetsFlavor;
   vector<int>     *qgMult;
   vector<int>     *muMatchedJetIdx;
   vector<int>     *eleMatchedJetIdx;
   vector<int>     *looseisoTrksMatchedJetIdx;
   vector<int>     *trksForIsoVetoMatchedJetIdx;
   vector<int>     *prodJetsNoLep_qgMult;
   vector<int>     *genDecayIdxVec;
   vector<int>     *genDecayPdgIdVec;
   vector<int>     *genDecayMomIdxVec;
   vector<int>     *genDecayMomRefVec;
   vector<int>     *W_emuVec;
   vector<int>     *W_tauVec;
   vector<int>     *W_tau_emuVec;
   vector<int>     *W_tau_prongsVec;
   vector<int>     *W_tau_nuVec;
   vector<int>     *selPDGid;
   vector<int>     *trksForIsoVeto_pdgId;
   vector<int>     *trksForIsoVeto_idx;
   vector<int>     *loose_isoTrks_pdgId;
   vector<int>     *loose_isoTrks_idx;
   vector<int>     *forVetoIsoTrksidx;
   vector<unsigned int> *loosePhotonID;
   vector<unsigned int> *mediumPhotonID;
   vector<unsigned int> *tightPhotonID;
   vector<unsigned int> *nonPrompt;
   vector<unsigned int> *elesisEB;
   vector<string>  *ntpVersion;
   vector<string>  *TriggerNames;
   vector<string>  *genDecayStrVec;
   vector<TLorentzVector> *svSoftLVec;
   vector<TLorentzVector> *svLVec;
   vector<TLorentzVector> *muonsLVec;
   vector<TLorentzVector> *specialFixMuonsLVec;
   vector<TLorentzVector> *gammaLVec;
   vector<TLorentzVector> *gammaLVecGen;
   vector<TLorentzVector> *elesLVec;
   vector<TLorentzVector> *jetsLVec;
   vector<TLorentzVector> *puppiJetsLVec;
   vector<TLorentzVector> *puppiSubJetsLVec;
   vector<TLorentzVector> *ak8JetsLVec;
   vector<TLorentzVector> *ak8SubJetsLVec;
   vector<TLorentzVector> *jetsLVecLepCleaned;
   vector<TLorentzVector> *prodJetsNoLep_puppiJetsLVec;
   vector<TLorentzVector> *prodJetsNoLep_puppiSubJetsLVec;
   vector<TLorentzVector> *genDecayLVec;
   vector<TLorentzVector> *selGenParticle;
   vector<TLorentzVector> *genjetsLVec;
   vector<TLorentzVector> *trksForIsoVetoLVec;
   vector<TLorentzVector> *loose_isoTrksLVec;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!
   TBranch        *b_genHT;   //!
   TBranch        *b_genmet;   //!
   TBranch        *b_genmetphi;   //!
   TBranch        *b_x1;   //!
   TBranch        *b_x2;   //!
   TBranch        *b_q;   //!
   TBranch        *b_mht;   //!
   TBranch        *b_mhtphi;   //!
   TBranch        *b_mt2;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metphi;   //!
   TBranch        *b_calomet;   //!
   TBranch        *b_calometphi;   //!
   TBranch        *b_dPhi0_CUT;   //!
   TBranch        *b_dPhi1_CUT;   //!
   TBranch        *b_dPhi2_CUT;   //!
   TBranch        *b_tru_npv;   //!
   TBranch        *b_avg_npv;   //!
   TBranch        *b_stored_weight;   //!
   TBranch        *b_evtWeight;   //!
   TBranch        *b_globalTightHalo2016Filter;   //!
   TBranch        *b_goodVerticesFilter;   //!
   TBranch        *b_eeBadScFilter;   //!
   TBranch        *b_EcalDeadCellTriggerPrimitiveFilter;   //!
   TBranch        *b_noBadMuonsFilter;   //!
   TBranch        *b_badMuonsFilter;   //!
   TBranch        *b_duplicateMuonsFilter;   //!
   TBranch        *b_nMuons_CUT;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_nElectrons_CUT;   //!
   TBranch        *b_nElectrons;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_NJetsISR;   //!
   TBranch        *b_id1;   //!
   TBranch        *b_id2;   //!
   TBranch        *b_loose_nIsoTrks;   //!
   TBranch        *b_nIsoTrks_CUT;   //!
   TBranch        *b_nJets_CUT;   //!
   TBranch        *b_vtxSize;   //!
   TBranch        *b_npv;   //!
   TBranch        *b_nm1;   //!
   TBranch        *b_n0;   //!
   TBranch        *b_np1;   //!
   TBranch        *b_looseJetID;   //!
   TBranch        *b_tightJetID;   //!
   TBranch        *b_tightlepvetoJetID;   //!
   TBranch        *b_looseJetID_NoLep;   //!
   TBranch        *b_tightJetID_NoLep;   //!
   TBranch        *b_tightlepvetoJetID_NoLep;   //!
   TBranch        *b_BadChargedCandidateFilter;   //!
   TBranch        *b_BadPFMuonFilter;   //!
   TBranch        *b_HBHENoiseFilter;   //!
   TBranch        *b_HBHEIsoNoiseFilter;   //!
   TBranch        *b_svPT;   //!
   TBranch        *b_svETA;   //!
   TBranch        *b_svPhi;   //!
   TBranch        *b_svMass;   //!
   TBranch        *b_svNTracks;   //!
   TBranch        *b_svChi2;   //!
   TBranch        *b_svNDF;   //!
   TBranch        *b_svDXY;   //!
   TBranch        *b_svDXYerr;   //!
   TBranch        *b_svD3D;   //!
   TBranch        *b_svD3Derr;   //!
   TBranch        *b_svCosThetaSVPS;   //!
   TBranch        *b_muonsCharge;   //!
   TBranch        *b_muonsMtw;   //!
   TBranch        *b_muonsRelIso;   //!
   TBranch        *b_muonsMiniIso;   //!
   TBranch        *b_muonspfActivity;   //!
   TBranch        *b_specialFixMuonsCharge;   //!
   TBranch        *b_pfGammaIso;   //!
   TBranch        *b_isEB;   //!
   TBranch        *b_genMatched;   //!
   TBranch        *b_hadTowOverEM;   //!
   TBranch        *b_sigmaIetaIeta;   //!
   TBranch        *b_pfChargedIso;   //!
   TBranch        *b_pfNeutralIso;   //!
   TBranch        *b_pfChargedIsoRhoCorr;   //!
   TBranch        *b_pfNeutralIsoRhoCorr;   //!
   TBranch        *b_pfGammaIsoRhoCorr;   //!
   TBranch        *b_hasPixelSeed;   //!
   TBranch        *b_passElectronVeto;   //!
   TBranch        *b_photonPt;   //!
   TBranch        *b_photonEta;   //!
   TBranch        *b_photonPhi;   //!
   TBranch        *b_elesCharge;   //!
   TBranch        *b_elesMtw;   //!
   TBranch        *b_elesRelIso;   //!
   TBranch        *b_elesMiniIso;   //!
   TBranch        *b_elespfActivity;   //!
   TBranch        *b_recoJetsJecUnc;   //!
   TBranch        *b_recoJetsJecScaleRawToFull;   //!
   TBranch        *b_qgLikelihood;   //!
   TBranch        *b_qgPtD;   //!
   TBranch        *b_qgAxis2;   //!
   TBranch        *b_recoJetschargedHadronEnergyFraction;   //!
   TBranch        *b_recoJetschargedEmEnergyFraction;   //!
   TBranch        *b_recoJetsneutralEmEnergyFraction;   //!
   TBranch        *b_recoJetsmuonEnergyFraction;   //!
   TBranch        *b_recoJetsBtag_0;   //!
   TBranch        *b_recoJetsCharge_0;   //!
   TBranch        *b_tau1;   //!
   TBranch        *b_tau2;   //!
   TBranch        *b_tau3;   //!
   TBranch        *b_softDropMass;   //!
   TBranch        *b_ak8SubJetsBdisc;   //!
   TBranch        *b_puppitau1;   //!
   TBranch        *b_puppitau2;   //!
   TBranch        *b_puppitau3;   //!
   TBranch        *b_puppisoftDropMass;   //!
   TBranch        *b_puppiSubJetsBdisc;   //!
   TBranch        *b_recoJetsJecUncLepCleaned;   //!
   TBranch        *b_prodJetsNoLep_qgLikelihood;   //!
   TBranch        *b_prodJetsNoLep_qgPtD;   //!
   TBranch        *b_prodJetsNoLep_qgAxis2;   //!
   TBranch        *b_recoJetschargedHadronEnergyFractionLepCleaned;   //!
   TBranch        *b_recoJetsneutralEmEnergyFractionLepCleaned;   //!
   TBranch        *b_recoJetschargedEmEnergyFractionLepCleaned;   //!
   TBranch        *b_recoJetsmuonEnergyFractionLepCleaned;   //!
   TBranch        *b_recoJetsBtag_0_LepCleaned;   //!
   TBranch        *b_recoJetsCharge_0_LepCleaned;   //!
   TBranch        *b_recoJetsJecScaleRawToFull_LepCleaned;   //!
   TBranch        *b_prodJetsNoLep_tau1;   //!
   TBranch        *b_prodJetsNoLep_tau2;   //!
   TBranch        *b_prodJetsNoLep_tau3;   //!
   TBranch        *b_prodJetsNoLep_puppisoftDropMass;   //!
   TBranch        *b_prodJetsNoLep_puppitau1;   //!
   TBranch        *b_prodJetsNoLep_puppitau2;   //!
   TBranch        *b_prodJetsNoLep_puppitau3;   //!
   TBranch        *b_prodJetsNoLep_puppiSubJetsBdisc;   //!
   TBranch        *b_W_emu_pfActivityVec;   //!
   TBranch        *b_W_tau_emu_pfActivityVec;   //!
   TBranch        *b_W_tau_prongs_pfActivityVec;   //!
   TBranch        *b_ScaleWeightsMiniAOD;   //!
   TBranch        *b_trksForIsoVeto_charge;   //!
   TBranch        *b_trksForIsoVeto_dz;   //!
   TBranch        *b_trksForIsoVeto_iso;   //!
   TBranch        *b_trksForIsoVeto_pfActivity;   //!
   TBranch        *b_loose_isoTrks_charge;   //!
   TBranch        *b_loose_isoTrks_dz;   //!
   TBranch        *b_loose_isoTrks_iso;   //!
   TBranch        *b_loose_isoTrks_mtw;   //!
   TBranch        *b_loose_isoTrks_pfActivity;   //!
   TBranch        *b_metMagUp;   //!
   TBranch        *b_metMagDown;   //!
   TBranch        *b_metPhiUp;   //!
   TBranch        *b_metPhiDown;   //!
   TBranch        *b_PassTrigger;   //!
   TBranch        *b_TriggerPrescales;   //!
   TBranch        *b_muonsFlagMedium;   //!
   TBranch        *b_muonsFlagTight;   //!
   TBranch        *b_specialFixtype;   //!
   TBranch        *b_elesFlagMedium;   //!
   TBranch        *b_elesFlagVeto;   //!
   TBranch        *b_recoJetsFlavor;   //!
   TBranch        *b_qgMult;   //!
   TBranch        *b_muMatchedJetIdx;   //!
   TBranch        *b_eleMatchedJetIdx;   //!
   TBranch        *b_looseisoTrksMatchedJetIdx;   //!
   TBranch        *b_trksForIsoVetoMatchedJetIdx;   //!
   TBranch        *b_prodJetsNoLep_qgMult;   //!
   TBranch        *b_genDecayIdxVec;   //!
   TBranch        *b_genDecayPdgIdVec;   //!
   TBranch        *b_genDecayMomIdxVec;   //!
   TBranch        *b_genDecayMomRefVec;   //!
   TBranch        *b_W_emuVec;   //!
   TBranch        *b_W_tauVec;   //!
   TBranch        *b_W_tau_emuVec;   //!
   TBranch        *b_W_tau_prongsVec;   //!
   TBranch        *b_W_tau_nuVec;   //!
   TBranch        *b_selPDGid;   //!
   TBranch        *b_trksForIsoVeto_pdgId;   //!
   TBranch        *b_trksForIsoVeto_idx;   //!
   TBranch        *b_loose_isoTrks_pdgId;   //!
   TBranch        *b_loose_isoTrks_idx;   //!
   TBranch        *b_forVetoIsoTrksidx;   //!
   TBranch        *b_loosePhotonID;   //!
   TBranch        *b_mediumPhotonID;   //!
   TBranch        *b_tightPhotonID;   //!
   TBranch        *b_nonPrompt;   //!
   TBranch        *b_elesisEB;   //!
   TBranch        *b_ntpVersion;   //!
   TBranch        *b_TriggerNames;   //!
   TBranch        *b_genDecayStrVec;   //!
   TBranch        *b_svSoftLVec;   //!
   TBranch        *b_svLVec;   //!
   TBranch        *b_muonsLVec;   //!
   TBranch        *b_specialFixMuonsLVec;   //!
   TBranch        *b_gammaLVec;   //!
   TBranch        *b_gammaLVecGen;   //!
   TBranch        *b_elesLVec;   //!
   TBranch        *b_jetsLVec;   //!
   TBranch        *b_puppiJetsLVec;   //!
   TBranch        *b_puppiSubJetsLVec;   //!
   TBranch        *b_ak8JetsLVec;   //!
   TBranch        *b_ak8SubJetsLVec;   //!
   TBranch        *b_jetsLVecLepCleaned;   //!
   TBranch        *b_prodJetsNoLep_puppiJetsLVec;   //!
   TBranch        *b_prodJetsNoLep_puppiSubJetsLVec;   //!
   TBranch        *b_genDecayLVec;   //!
   TBranch        *b_selGenParticle;   //!
   TBranch        *b_genjetsLVec;   //!
   TBranch        *b_trksForIsoVetoLVec;   //!
   TBranch        *b_loose_isoTrksLVec;   //!

   StopNtupleTree(TTree *tree=0);
   virtual ~StopNtupleTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif


inline StopNtupleTree::StopNtupleTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("stopFlatNtuples_7.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("stopFlatNtuples_7.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("stopFlatNtuples_7.root:/stopTreeMaker");
      dir->GetObject("AUX",tree);

   }
   Init(tree);
}

inline StopNtupleTree::~StopNtupleTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

inline Int_t StopNtupleTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
inline Long64_t StopNtupleTree::LoadTree(Long64_t entry)
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

inline void StopNtupleTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   svPT = 0;
   svETA = 0;
   svPhi = 0;
   svMass = 0;
   svNTracks = 0;
   svChi2 = 0;
   svNDF = 0;
   svDXY = 0;
   svDXYerr = 0;
   svD3D = 0;
   svD3Derr = 0;
   svCosThetaSVPS = 0;
   muonsCharge = 0;
   muonsMtw = 0;
   muonsRelIso = 0;
   muonsMiniIso = 0;
   muonspfActivity = 0;
   specialFixMuonsCharge = 0;
   pfGammaIso = 0;
   isEB = 0;
   genMatched = 0;
   hadTowOverEM = 0;
   sigmaIetaIeta = 0;
   pfChargedIso = 0;
   pfNeutralIso = 0;
   pfChargedIsoRhoCorr = 0;
   pfNeutralIsoRhoCorr = 0;
   pfGammaIsoRhoCorr = 0;
   hasPixelSeed = 0;
   passElectronVeto = 0;
   photonPt = 0;
   photonEta = 0;
   photonPhi = 0;
   elesCharge = 0;
   elesMtw = 0;
   elesRelIso = 0;
   elesMiniIso = 0;
   elespfActivity = 0;
   recoJetsJecUnc = 0;
   recoJetsJecScaleRawToFull = 0;
   qgLikelihood = 0;
   qgPtD = 0;
   qgAxis2 = 0;
   recoJetschargedHadronEnergyFraction = 0;
   recoJetschargedEmEnergyFraction = 0;
   recoJetsneutralEmEnergyFraction = 0;
   recoJetsmuonEnergyFraction = 0;
   recoJetsBtag_0 = 0;
   recoJetsCharge_0 = 0;
   tau1 = 0;
   tau2 = 0;
   tau3 = 0;
   softDropMass = 0;
   ak8SubJetsBdisc = 0;
   puppitau1 = 0;
   puppitau2 = 0;
   puppitau3 = 0;
   puppisoftDropMass = 0;
   puppiSubJetsBdisc = 0;
   recoJetsJecUncLepCleaned = 0;
   prodJetsNoLep_qgLikelihood = 0;
   prodJetsNoLep_qgPtD = 0;
   prodJetsNoLep_qgAxis2 = 0;
   recoJetschargedHadronEnergyFractionLepCleaned = 0;
   recoJetsneutralEmEnergyFractionLepCleaned = 0;
   recoJetschargedEmEnergyFractionLepCleaned = 0;
   recoJetsmuonEnergyFractionLepCleaned = 0;
   recoJetsBtag_0_LepCleaned = 0;
   recoJetsCharge_0_LepCleaned = 0;
   recoJetsJecScaleRawToFull_LepCleaned = 0;
   prodJetsNoLep_tau1 = 0;
   prodJetsNoLep_tau2 = 0;
   prodJetsNoLep_tau3 = 0;
   prodJetsNoLep_puppisoftDropMass = 0;
   prodJetsNoLep_puppitau1 = 0;
   prodJetsNoLep_puppitau2 = 0;
   prodJetsNoLep_puppitau3 = 0;
   prodJetsNoLep_puppiSubJetsBdisc = 0;
   W_emu_pfActivityVec = 0;
   W_tau_emu_pfActivityVec = 0;
   W_tau_prongs_pfActivityVec = 0;
   ScaleWeightsMiniAOD = 0;
   trksForIsoVeto_charge = 0;
   trksForIsoVeto_dz = 0;
   trksForIsoVeto_iso = 0;
   trksForIsoVeto_pfActivity = 0;
   loose_isoTrks_charge = 0;
   loose_isoTrks_dz = 0;
   loose_isoTrks_iso = 0;
   loose_isoTrks_mtw = 0;
   loose_isoTrks_pfActivity = 0;
   metMagUp = 0;
   metMagDown = 0;
   metPhiUp = 0;
   metPhiDown = 0;
   PassTrigger = 0;
   TriggerPrescales = 0;
   muonsFlagMedium = 0;
   muonsFlagTight = 0;
   specialFixtype = 0;
   elesFlagMedium = 0;
   elesFlagVeto = 0;
   recoJetsFlavor = 0;
   qgMult = 0;
   muMatchedJetIdx = 0;
   eleMatchedJetIdx = 0;
   looseisoTrksMatchedJetIdx = 0;
   trksForIsoVetoMatchedJetIdx = 0;
   prodJetsNoLep_qgMult = 0;
   genDecayIdxVec = 0;
   genDecayPdgIdVec = 0;
   genDecayMomIdxVec = 0;
   genDecayMomRefVec = 0;
   W_emuVec = 0;
   W_tauVec = 0;
   W_tau_emuVec = 0;
   W_tau_prongsVec = 0;
   W_tau_nuVec = 0;
   selPDGid = 0;
   trksForIsoVeto_pdgId = 0;
   trksForIsoVeto_idx = 0;
   loose_isoTrks_pdgId = 0;
   loose_isoTrks_idx = 0;
   forVetoIsoTrksidx = 0;
   loosePhotonID = 0;
   mediumPhotonID = 0;
   tightPhotonID = 0;
   nonPrompt = 0;
   elesisEB = 0;
   ntpVersion = 0;
   TriggerNames = 0;
   genDecayStrVec = 0;
   svSoftLVec = 0;
   svLVec = 0;
   muonsLVec = 0;
   specialFixMuonsLVec = 0;
   gammaLVec = 0;
   gammaLVecGen = 0;
   elesLVec = 0;
   jetsLVec = 0;
   puppiJetsLVec = 0;
   puppiSubJetsLVec = 0;
   ak8JetsLVec = 0;
   ak8SubJetsLVec = 0;
   jetsLVecLepCleaned = 0;
   prodJetsNoLep_puppiJetsLVec = 0;
   prodJetsNoLep_puppiSubJetsLVec = 0;
   genDecayLVec = 0;
   selGenParticle = 0;
   genjetsLVec = 0;
   trksForIsoVetoLVec = 0;
   loose_isoTrksLVec = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("genHT", &genHT, &b_genHT);
   fChain->SetBranchAddress("genmet", &genmet, &b_genmet);
   fChain->SetBranchAddress("genmetphi", &genmetphi, &b_genmetphi);
   fChain->SetBranchAddress("x1", &x1, &b_x1);
   fChain->SetBranchAddress("x2", &x2, &b_x2);
   fChain->SetBranchAddress("q", &q, &b_q);
   fChain->SetBranchAddress("mht", &mht, &b_mht);
   fChain->SetBranchAddress("mhtphi", &mhtphi, &b_mhtphi);
   fChain->SetBranchAddress("mt2", &mt2, &b_mt2);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metphi", &metphi, &b_metphi);
   fChain->SetBranchAddress("calomet", &calomet, &b_calomet);
   fChain->SetBranchAddress("calometphi", &calometphi, &b_calometphi);
   fChain->SetBranchAddress("dPhi0_CUT", &dPhi0_CUT, &b_dPhi0_CUT);
   fChain->SetBranchAddress("dPhi1_CUT", &dPhi1_CUT, &b_dPhi1_CUT);
   fChain->SetBranchAddress("dPhi2_CUT", &dPhi2_CUT, &b_dPhi2_CUT);
   fChain->SetBranchAddress("tru_npv", &tru_npv, &b_tru_npv);
   fChain->SetBranchAddress("avg_npv", &avg_npv, &b_avg_npv);
   fChain->SetBranchAddress("stored_weight", &stored_weight, &b_stored_weight);
   fChain->SetBranchAddress("evtWeight", &evtWeight, &b_evtWeight);
   fChain->SetBranchAddress("globalTightHalo2016Filter", &globalTightHalo2016Filter, &b_globalTightHalo2016Filter);
   fChain->SetBranchAddress("goodVerticesFilter", &goodVerticesFilter, &b_goodVerticesFilter);
   fChain->SetBranchAddress("eeBadScFilter", &eeBadScFilter, &b_eeBadScFilter);
   fChain->SetBranchAddress("EcalDeadCellTriggerPrimitiveFilter", &EcalDeadCellTriggerPrimitiveFilter, &b_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("noBadMuonsFilter", &noBadMuonsFilter, &b_noBadMuonsFilter);
   fChain->SetBranchAddress("badMuonsFilter", &badMuonsFilter, &b_badMuonsFilter);
   fChain->SetBranchAddress("duplicateMuonsFilter", &duplicateMuonsFilter, &b_duplicateMuonsFilter);
   fChain->SetBranchAddress("nMuons_CUT", &nMuons_CUT, &b_nMuons_CUT);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("nElectrons_CUT", &nElectrons_CUT, &b_nElectrons_CUT);
   fChain->SetBranchAddress("nElectrons", &nElectrons, &b_nElectrons);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("NJetsISR", &NJetsISR, &b_NJetsISR);
   fChain->SetBranchAddress("id1", &id1, &b_id1);
   fChain->SetBranchAddress("id2", &id2, &b_id2);
   fChain->SetBranchAddress("loose_nIsoTrks", &loose_nIsoTrks, &b_loose_nIsoTrks);
   fChain->SetBranchAddress("nIsoTrks_CUT", &nIsoTrks_CUT, &b_nIsoTrks_CUT);
   fChain->SetBranchAddress("nJets_CUT", &nJets_CUT, &b_nJets_CUT);
   fChain->SetBranchAddress("vtxSize", &vtxSize, &b_vtxSize);
   fChain->SetBranchAddress("npv", &npv, &b_npv);
   fChain->SetBranchAddress("nm1", &nm1, &b_nm1);
   fChain->SetBranchAddress("n0", &n0, &b_n0);
   fChain->SetBranchAddress("np1", &np1, &b_np1);
   fChain->SetBranchAddress("looseJetID", &looseJetID, &b_looseJetID);
   fChain->SetBranchAddress("tightJetID", &tightJetID, &b_tightJetID);
   fChain->SetBranchAddress("tightlepvetoJetID", &tightlepvetoJetID, &b_tightlepvetoJetID);
   fChain->SetBranchAddress("looseJetID_NoLep", &looseJetID_NoLep, &b_looseJetID_NoLep);
   fChain->SetBranchAddress("tightJetID_NoLep", &tightJetID_NoLep, &b_tightJetID_NoLep);
   fChain->SetBranchAddress("tightlepvetoJetID_NoLep", &tightlepvetoJetID_NoLep, &b_tightlepvetoJetID_NoLep);
   fChain->SetBranchAddress("BadChargedCandidateFilter", &BadChargedCandidateFilter, &b_BadChargedCandidateFilter);
   fChain->SetBranchAddress("BadPFMuonFilter", &BadPFMuonFilter, &b_BadPFMuonFilter);
   fChain->SetBranchAddress("HBHENoiseFilter", &HBHENoiseFilter, &b_HBHENoiseFilter);
   fChain->SetBranchAddress("HBHEIsoNoiseFilter", &HBHEIsoNoiseFilter, &b_HBHEIsoNoiseFilter);
   fChain->SetBranchAddress("svPT", &svPT, &b_svPT);
   fChain->SetBranchAddress("svETA", &svETA, &b_svETA);
   fChain->SetBranchAddress("svPhi", &svPhi, &b_svPhi);
   fChain->SetBranchAddress("svMass", &svMass, &b_svMass);
   fChain->SetBranchAddress("svNTracks", &svNTracks, &b_svNTracks);
   fChain->SetBranchAddress("svChi2", &svChi2, &b_svChi2);
   fChain->SetBranchAddress("svNDF", &svNDF, &b_svNDF);
   fChain->SetBranchAddress("svDXY", &svDXY, &b_svDXY);
   fChain->SetBranchAddress("svDXYerr", &svDXYerr, &b_svDXYerr);
   fChain->SetBranchAddress("svD3D", &svD3D, &b_svD3D);
   fChain->SetBranchAddress("svD3Derr", &svD3Derr, &b_svD3Derr);
   fChain->SetBranchAddress("svCosThetaSVPS", &svCosThetaSVPS, &b_svCosThetaSVPS);
   fChain->SetBranchAddress("muonsCharge", &muonsCharge, &b_muonsCharge);
   fChain->SetBranchAddress("muonsMtw", &muonsMtw, &b_muonsMtw);
   fChain->SetBranchAddress("muonsRelIso", &muonsRelIso, &b_muonsRelIso);
   fChain->SetBranchAddress("muonsMiniIso", &muonsMiniIso, &b_muonsMiniIso);
   fChain->SetBranchAddress("muonspfActivity", &muonspfActivity, &b_muonspfActivity);
   fChain->SetBranchAddress("specialFixMuonsCharge", &specialFixMuonsCharge, &b_specialFixMuonsCharge);
   fChain->SetBranchAddress("pfGammaIso", &pfGammaIso, &b_pfGammaIso);
   fChain->SetBranchAddress("isEB", &isEB, &b_isEB);
   fChain->SetBranchAddress("genMatched", &genMatched, &b_genMatched);
   fChain->SetBranchAddress("hadTowOverEM", &hadTowOverEM, &b_hadTowOverEM);
   fChain->SetBranchAddress("sigmaIetaIeta", &sigmaIetaIeta, &b_sigmaIetaIeta);
   fChain->SetBranchAddress("pfChargedIso", &pfChargedIso, &b_pfChargedIso);
   fChain->SetBranchAddress("pfNeutralIso", &pfNeutralIso, &b_pfNeutralIso);
   fChain->SetBranchAddress("pfChargedIsoRhoCorr", &pfChargedIsoRhoCorr, &b_pfChargedIsoRhoCorr);
   fChain->SetBranchAddress("pfNeutralIsoRhoCorr", &pfNeutralIsoRhoCorr, &b_pfNeutralIsoRhoCorr);
   fChain->SetBranchAddress("pfGammaIsoRhoCorr", &pfGammaIsoRhoCorr, &b_pfGammaIsoRhoCorr);
   fChain->SetBranchAddress("hasPixelSeed", &hasPixelSeed, &b_hasPixelSeed);
   fChain->SetBranchAddress("passElectronVeto", &passElectronVeto, &b_passElectronVeto);
   fChain->SetBranchAddress("photonPt", &photonPt, &b_photonPt);
   fChain->SetBranchAddress("photonEta", &photonEta, &b_photonEta);
   fChain->SetBranchAddress("photonPhi", &photonPhi, &b_photonPhi);
   fChain->SetBranchAddress("elesCharge", &elesCharge, &b_elesCharge);
   fChain->SetBranchAddress("elesMtw", &elesMtw, &b_elesMtw);
   fChain->SetBranchAddress("elesRelIso", &elesRelIso, &b_elesRelIso);
   fChain->SetBranchAddress("elesMiniIso", &elesMiniIso, &b_elesMiniIso);
   fChain->SetBranchAddress("elespfActivity", &elespfActivity, &b_elespfActivity);
   fChain->SetBranchAddress("recoJetsJecUnc", &recoJetsJecUnc, &b_recoJetsJecUnc);
   fChain->SetBranchAddress("recoJetsJecScaleRawToFull", &recoJetsJecScaleRawToFull, &b_recoJetsJecScaleRawToFull);
   fChain->SetBranchAddress("qgLikelihood", &qgLikelihood, &b_qgLikelihood);
   fChain->SetBranchAddress("qgPtD", &qgPtD, &b_qgPtD);
   fChain->SetBranchAddress("qgAxis2", &qgAxis2, &b_qgAxis2);
   fChain->SetBranchAddress("recoJetschargedHadronEnergyFraction", &recoJetschargedHadronEnergyFraction, &b_recoJetschargedHadronEnergyFraction);
   fChain->SetBranchAddress("recoJetschargedEmEnergyFraction", &recoJetschargedEmEnergyFraction, &b_recoJetschargedEmEnergyFraction);
   fChain->SetBranchAddress("recoJetsneutralEmEnergyFraction", &recoJetsneutralEmEnergyFraction, &b_recoJetsneutralEmEnergyFraction);
   fChain->SetBranchAddress("recoJetsmuonEnergyFraction", &recoJetsmuonEnergyFraction, &b_recoJetsmuonEnergyFraction);
   fChain->SetBranchAddress("recoJetsBtag_0", &recoJetsBtag_0, &b_recoJetsBtag_0);
   fChain->SetBranchAddress("recoJetsCharge_0", &recoJetsCharge_0, &b_recoJetsCharge_0);
   fChain->SetBranchAddress("tau1", &tau1, &b_tau1);
   fChain->SetBranchAddress("tau2", &tau2, &b_tau2);
   fChain->SetBranchAddress("tau3", &tau3, &b_tau3);
   fChain->SetBranchAddress("softDropMass", &softDropMass, &b_softDropMass);
   fChain->SetBranchAddress("ak8SubJetsBdisc", &ak8SubJetsBdisc, &b_ak8SubJetsBdisc);
   fChain->SetBranchAddress("puppitau1", &puppitau1, &b_puppitau1);
   fChain->SetBranchAddress("puppitau2", &puppitau2, &b_puppitau2);
   fChain->SetBranchAddress("puppitau3", &puppitau3, &b_puppitau3);
   fChain->SetBranchAddress("puppisoftDropMass", &puppisoftDropMass, &b_puppisoftDropMass);
   fChain->SetBranchAddress("puppiSubJetsBdisc", &puppiSubJetsBdisc, &b_puppiSubJetsBdisc);
   fChain->SetBranchAddress("recoJetsJecUncLepCleaned", &recoJetsJecUncLepCleaned, &b_recoJetsJecUncLepCleaned);
   fChain->SetBranchAddress("prodJetsNoLep_qgLikelihood", &prodJetsNoLep_qgLikelihood, &b_prodJetsNoLep_qgLikelihood);
   fChain->SetBranchAddress("prodJetsNoLep_qgPtD", &prodJetsNoLep_qgPtD, &b_prodJetsNoLep_qgPtD);
   fChain->SetBranchAddress("prodJetsNoLep_qgAxis2", &prodJetsNoLep_qgAxis2, &b_prodJetsNoLep_qgAxis2);
   fChain->SetBranchAddress("recoJetschargedHadronEnergyFractionLepCleaned", &recoJetschargedHadronEnergyFractionLepCleaned, &b_recoJetschargedHadronEnergyFractionLepCleaned);
   fChain->SetBranchAddress("recoJetsneutralEmEnergyFractionLepCleaned", &recoJetsneutralEmEnergyFractionLepCleaned, &b_recoJetsneutralEmEnergyFractionLepCleaned);
   fChain->SetBranchAddress("recoJetschargedEmEnergyFractionLepCleaned", &recoJetschargedEmEnergyFractionLepCleaned, &b_recoJetschargedEmEnergyFractionLepCleaned);
   fChain->SetBranchAddress("recoJetsmuonEnergyFractionLepCleaned", &recoJetsmuonEnergyFractionLepCleaned, &b_recoJetsmuonEnergyFractionLepCleaned);
   fChain->SetBranchAddress("recoJetsBtag_0_LepCleaned", &recoJetsBtag_0_LepCleaned, &b_recoJetsBtag_0_LepCleaned);
   fChain->SetBranchAddress("recoJetsCharge_0_LepCleaned", &recoJetsCharge_0_LepCleaned, &b_recoJetsCharge_0_LepCleaned);
   fChain->SetBranchAddress("recoJetsJecScaleRawToFull_LepCleaned", &recoJetsJecScaleRawToFull_LepCleaned, &b_recoJetsJecScaleRawToFull_LepCleaned);
   fChain->SetBranchAddress("prodJetsNoLep_tau1", &prodJetsNoLep_tau1, &b_prodJetsNoLep_tau1);
   fChain->SetBranchAddress("prodJetsNoLep_tau2", &prodJetsNoLep_tau2, &b_prodJetsNoLep_tau2);
   fChain->SetBranchAddress("prodJetsNoLep_tau3", &prodJetsNoLep_tau3, &b_prodJetsNoLep_tau3);
   fChain->SetBranchAddress("prodJetsNoLep_puppisoftDropMass", &prodJetsNoLep_puppisoftDropMass, &b_prodJetsNoLep_puppisoftDropMass);
   fChain->SetBranchAddress("prodJetsNoLep_puppitau1", &prodJetsNoLep_puppitau1, &b_prodJetsNoLep_puppitau1);
   fChain->SetBranchAddress("prodJetsNoLep_puppitau2", &prodJetsNoLep_puppitau2, &b_prodJetsNoLep_puppitau2);
   fChain->SetBranchAddress("prodJetsNoLep_puppitau3", &prodJetsNoLep_puppitau3, &b_prodJetsNoLep_puppitau3);
   fChain->SetBranchAddress("prodJetsNoLep_puppiSubJetsBdisc", &prodJetsNoLep_puppiSubJetsBdisc, &b_prodJetsNoLep_puppiSubJetsBdisc);
   fChain->SetBranchAddress("W_emu_pfActivityVec", &W_emu_pfActivityVec, &b_W_emu_pfActivityVec);
   fChain->SetBranchAddress("W_tau_emu_pfActivityVec", &W_tau_emu_pfActivityVec, &b_W_tau_emu_pfActivityVec);
   fChain->SetBranchAddress("W_tau_prongs_pfActivityVec", &W_tau_prongs_pfActivityVec, &b_W_tau_prongs_pfActivityVec);
   fChain->SetBranchAddress("ScaleWeightsMiniAOD", &ScaleWeightsMiniAOD, &b_ScaleWeightsMiniAOD);
   fChain->SetBranchAddress("trksForIsoVeto_charge", &trksForIsoVeto_charge, &b_trksForIsoVeto_charge);
   fChain->SetBranchAddress("trksForIsoVeto_dz", &trksForIsoVeto_dz, &b_trksForIsoVeto_dz);
   fChain->SetBranchAddress("trksForIsoVeto_iso", &trksForIsoVeto_iso, &b_trksForIsoVeto_iso);
   fChain->SetBranchAddress("trksForIsoVeto_pfActivity", &trksForIsoVeto_pfActivity, &b_trksForIsoVeto_pfActivity);
   fChain->SetBranchAddress("loose_isoTrks_charge", &loose_isoTrks_charge, &b_loose_isoTrks_charge);
   fChain->SetBranchAddress("loose_isoTrks_dz", &loose_isoTrks_dz, &b_loose_isoTrks_dz);
   fChain->SetBranchAddress("loose_isoTrks_iso", &loose_isoTrks_iso, &b_loose_isoTrks_iso);
   fChain->SetBranchAddress("loose_isoTrks_mtw", &loose_isoTrks_mtw, &b_loose_isoTrks_mtw);
   fChain->SetBranchAddress("loose_isoTrks_pfActivity", &loose_isoTrks_pfActivity, &b_loose_isoTrks_pfActivity);
   fChain->SetBranchAddress("metMagUp", &metMagUp, &b_metMagUp);
   fChain->SetBranchAddress("metMagDown", &metMagDown, &b_metMagDown);
   fChain->SetBranchAddress("metPhiUp", &metPhiUp, &b_metPhiUp);
   fChain->SetBranchAddress("metPhiDown", &metPhiDown, &b_metPhiDown);
   fChain->SetBranchAddress("PassTrigger", &PassTrigger, &b_PassTrigger);
   fChain->SetBranchAddress("TriggerPrescales", &TriggerPrescales, &b_TriggerPrescales);
   fChain->SetBranchAddress("muonsFlagMedium", &muonsFlagMedium, &b_muonsFlagMedium);
   fChain->SetBranchAddress("muonsFlagTight", &muonsFlagTight, &b_muonsFlagTight);
   fChain->SetBranchAddress("specialFixtype", &specialFixtype, &b_specialFixtype);
   fChain->SetBranchAddress("elesFlagMedium", &elesFlagMedium, &b_elesFlagMedium);
   fChain->SetBranchAddress("elesFlagVeto", &elesFlagVeto, &b_elesFlagVeto);
   fChain->SetBranchAddress("recoJetsFlavor", &recoJetsFlavor, &b_recoJetsFlavor);
   fChain->SetBranchAddress("qgMult", &qgMult, &b_qgMult);
   fChain->SetBranchAddress("muMatchedJetIdx", &muMatchedJetIdx, &b_muMatchedJetIdx);
   fChain->SetBranchAddress("eleMatchedJetIdx", &eleMatchedJetIdx, &b_eleMatchedJetIdx);
   fChain->SetBranchAddress("looseisoTrksMatchedJetIdx", &looseisoTrksMatchedJetIdx, &b_looseisoTrksMatchedJetIdx);
   fChain->SetBranchAddress("trksForIsoVetoMatchedJetIdx", &trksForIsoVetoMatchedJetIdx, &b_trksForIsoVetoMatchedJetIdx);
   fChain->SetBranchAddress("prodJetsNoLep_qgMult", &prodJetsNoLep_qgMult, &b_prodJetsNoLep_qgMult);
   fChain->SetBranchAddress("genDecayIdxVec", &genDecayIdxVec, &b_genDecayIdxVec);
   fChain->SetBranchAddress("genDecayPdgIdVec", &genDecayPdgIdVec, &b_genDecayPdgIdVec);
   fChain->SetBranchAddress("genDecayMomIdxVec", &genDecayMomIdxVec, &b_genDecayMomIdxVec);
   fChain->SetBranchAddress("genDecayMomRefVec", &genDecayMomRefVec, &b_genDecayMomRefVec);
   fChain->SetBranchAddress("W_emuVec", &W_emuVec, &b_W_emuVec);
   fChain->SetBranchAddress("W_tauVec", &W_tauVec, &b_W_tauVec);
   fChain->SetBranchAddress("W_tau_emuVec", &W_tau_emuVec, &b_W_tau_emuVec);
   fChain->SetBranchAddress("W_tau_prongsVec", &W_tau_prongsVec, &b_W_tau_prongsVec);
   fChain->SetBranchAddress("W_tau_nuVec", &W_tau_nuVec, &b_W_tau_nuVec);
   fChain->SetBranchAddress("selPDGid", &selPDGid, &b_selPDGid);
   fChain->SetBranchAddress("trksForIsoVeto_pdgId", &trksForIsoVeto_pdgId, &b_trksForIsoVeto_pdgId);
   fChain->SetBranchAddress("trksForIsoVeto_idx", &trksForIsoVeto_idx, &b_trksForIsoVeto_idx);
   fChain->SetBranchAddress("loose_isoTrks_pdgId", &loose_isoTrks_pdgId, &b_loose_isoTrks_pdgId);
   fChain->SetBranchAddress("loose_isoTrks_idx", &loose_isoTrks_idx, &b_loose_isoTrks_idx);
   fChain->SetBranchAddress("forVetoIsoTrksidx", &forVetoIsoTrksidx, &b_forVetoIsoTrksidx);
   fChain->SetBranchAddress("loosePhotonID", &loosePhotonID, &b_loosePhotonID);
   fChain->SetBranchAddress("mediumPhotonID", &mediumPhotonID, &b_mediumPhotonID);
   fChain->SetBranchAddress("tightPhotonID", &tightPhotonID, &b_tightPhotonID);
   fChain->SetBranchAddress("nonPrompt", &nonPrompt, &b_nonPrompt);
   fChain->SetBranchAddress("elesisEB", &elesisEB, &b_elesisEB);
   fChain->SetBranchAddress("ntpVersion", &ntpVersion, &b_ntpVersion);
   fChain->SetBranchAddress("TriggerNames", &TriggerNames, &b_TriggerNames);
   fChain->SetBranchAddress("genDecayStrVec", &genDecayStrVec, &b_genDecayStrVec);
   fChain->SetBranchAddress("svSoftLVec", &svSoftLVec, &b_svSoftLVec);
   fChain->SetBranchAddress("svLVec", &svLVec, &b_svLVec);
   fChain->SetBranchAddress("muonsLVec", &muonsLVec, &b_muonsLVec);
   fChain->SetBranchAddress("specialFixMuonsLVec", &specialFixMuonsLVec, &b_specialFixMuonsLVec);
   fChain->SetBranchAddress("gammaLVec", &gammaLVec, &b_gammaLVec);
   fChain->SetBranchAddress("gammaLVecGen", &gammaLVecGen, &b_gammaLVecGen);
   fChain->SetBranchAddress("elesLVec", &elesLVec, &b_elesLVec);
   fChain->SetBranchAddress("jetsLVec", &jetsLVec, &b_jetsLVec);
   fChain->SetBranchAddress("puppiJetsLVec", &puppiJetsLVec, &b_puppiJetsLVec);
   fChain->SetBranchAddress("puppiSubJetsLVec", &puppiSubJetsLVec, &b_puppiSubJetsLVec);
   fChain->SetBranchAddress("ak8JetsLVec", &ak8JetsLVec, &b_ak8JetsLVec);
   fChain->SetBranchAddress("ak8SubJetsLVec", &ak8SubJetsLVec, &b_ak8SubJetsLVec);
   fChain->SetBranchAddress("jetsLVecLepCleaned", &jetsLVecLepCleaned, &b_jetsLVecLepCleaned);
   fChain->SetBranchAddress("prodJetsNoLep_puppiJetsLVec", &prodJetsNoLep_puppiJetsLVec, &b_prodJetsNoLep_puppiJetsLVec);
   fChain->SetBranchAddress("prodJetsNoLep_puppiSubJetsLVec", &prodJetsNoLep_puppiSubJetsLVec, &b_prodJetsNoLep_puppiSubJetsLVec);
   fChain->SetBranchAddress("genDecayLVec", &genDecayLVec, &b_genDecayLVec);
   fChain->SetBranchAddress("selGenParticle", &selGenParticle, &b_selGenParticle);
   fChain->SetBranchAddress("genjetsLVec", &genjetsLVec, &b_genjetsLVec);
   fChain->SetBranchAddress("trksForIsoVetoLVec", &trksForIsoVetoLVec, &b_trksForIsoVetoLVec);
   fChain->SetBranchAddress("loose_isoTrksLVec", &loose_isoTrksLVec, &b_loose_isoTrksLVec);
   Notify();
}

inline Bool_t StopNtupleTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

inline void StopNtupleTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
inline Int_t StopNtupleTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
