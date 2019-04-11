#include "ReducedNtuple.hh"
#include "ParticleList.hh"
#include "TInterpreter.h"

#include "StopNtupleTree.hh"

using namespace RestFrames;

template <class Base>
ReducedNtuple<Base>::ReducedNtuple(TTree* tree)
  : NtupleBase<Base>(tree)
{
  // RestFrames stuff setup
  
  // ISR-style trees
  for(int t = 0; t < 3; t++){
    LAB_ISR[t] = new LabRecoFrame(Form("LAB_ISR%d",t),"LAB");
    CM_ISR[t]  = new DecayRecoFrame(Form("CM_ISR%d",t),"CM");
    S_ISR[t]   = new DecayRecoFrame(Form("S_ISR%d",t),"#tilde{S}");
    ISR_ISR[t] = new VisibleRecoFrame(Form("ISR_ISR%d",t),"ISR");
    V_ISR[t]   = new VisibleRecoFrame(Form("V_ISR%d",t),"Vis");
    L_ISR[t]   = new VisibleRecoFrame(Form("L_ISR%d",t),"Lep");
    I_ISR[t]   = new InvisibleRecoFrame(Form("I_ISR%d",t),"Inv");

    LAB_ISR[t]->SetChildFrame(*CM_ISR[t]);
    CM_ISR[t]->AddChildFrame(*S_ISR[t]);
    CM_ISR[t]->AddChildFrame(*ISR_ISR[t]);
    if(t == 0){ // jets in ISR, leptons in sparticle
      S_ISR[t]->AddChildFrame(*L_ISR[t]);
      S_ISR[t]->AddChildFrame(*I_ISR[t]);
    }
    if(t == 1){ // jets in ISR+V, leptons in sparticle
      S_ISR[t]->AddChildFrame(*L_ISR[t]);
      S_ISR[t]->AddChildFrame(*V_ISR[t]);
      S_ISR[t]->AddChildFrame(*I_ISR[t]);
    }
    if(t == 2){ // everything combinatorically assigned
      S_ISR[t]->AddChildFrame(*V_ISR[t]);
      S_ISR[t]->AddChildFrame(*I_ISR[t]);
    }

    if(!LAB_ISR[t]->InitializeTree()){
      cout << "Problem initializing ISR tree " << t << endl;
    }
    
    INV_ISR[t] = new InvisibleGroup(Form("INV_ISR%d",t),"Invisible System");
    INV_ISR[t]->AddFrame(*I_ISR[t]);
    InvM_ISR[t] = new SetMassInvJigsaw(Form("InvM_ISR%d",t), "Invisible mass Jigsaw");
    INV_ISR[t]->AddJigsaw(*InvM_ISR[t]);

    COMB_ISR[t] = new CombinatoricGroup(Form("COMB_ISR%d",t), "Combinatoric System");
    CombSplit_ISR[t] = new MinMassesCombJigsaw(Form("CombSplit_ISR%d",t), "Minimize M_{ISR} and M_{S} Jigsaw");
   
    if(t == 0){ // jets in ISR, leptons in sparticle
      // No combinatoric jigsaw necessary
    }
    if(t == 1){ // jets in ISR+V, leptons in sparticle
      COMB_ISR[t]->AddFrame(*ISR_ISR[t]);
      COMB_ISR[t]->SetNElementsForFrame(*ISR_ISR[t], 1);
      COMB_ISR[t]->AddFrame(*V_ISR[t]);
      COMB_ISR[t]->SetNElementsForFrame(*V_ISR[t], 0);
      COMB_ISR[t]->AddJigsaw(*CombSplit_ISR[t]);
      CombSplit_ISR[t]->AddCombFrame(*ISR_ISR[t], 0);
      CombSplit_ISR[t]->AddCombFrame(*V_ISR[t], 1);
      CombSplit_ISR[t]->AddObjectFrame(*ISR_ISR[t], 0);
      CombSplit_ISR[t]->AddObjectFrame(*S_ISR[t], 1);
    }
    if(t == 2){ // everything combinatorically assigned
      COMB_ISR[t]->AddFrame(*ISR_ISR[t]);
      COMB_ISR[t]->SetNElementsForFrame(*ISR_ISR[t], 1);
      COMB_ISR[t]->AddFrame(*V_ISR[t]);
      COMB_ISR[t]->SetNElementsForFrame(*V_ISR[t], 0);
      COMB_ISR[t]->AddJigsaw(*CombSplit_ISR[t]);
      CombSplit_ISR[t]->AddCombFrame(*ISR_ISR[t], 0);
      CombSplit_ISR[t]->AddCombFrame(*V_ISR[t], 1);
      CombSplit_ISR[t]->AddObjectFrame(*ISR_ISR[t], 0);
      CombSplit_ISR[t]->AddObjectFrame(*S_ISR[t], 1);
    }

    if(!LAB_ISR[t]->InitializeAnalysis()){
      cout << "Problem initializing ISR analysis " << t << endl;
    }
  }

  // Sparticle pair-production trees
  for(int t = 0; t < 5; t++){
    LAB_PAIR[t]  = new LabRecoFrame(Form("LAB_PAIR%d",t),"LAB");
    S_PAIR[t]    = new DecayRecoFrame(Form("S_PAIR%d",t),"#tilde{C}a #tilde{C}b");
    Ca_PAIR[t]   = new DecayRecoFrame(Form("Ca_PAIR%d",t),"#tilde{C}a");
    Cb_PAIR[t]   = new DecayRecoFrame(Form("Cb_PAIR%d",t),"#tilde{C}b");
    GCa_PAIR[t]   = new DecayRecoFrame(Form("GCa_PAIR%d",t),"#tilde{GC}a");
    GCb_PAIR[t]   = new DecayRecoFrame(Form("GCb_PAIR%d",t),"#tilde{GC}b");
    VSAa_PAIR[t] = new SelfAssemblingRecoFrame(Form("VSAa_PAIR%d",t),"Va");
    VSAb_PAIR[t] = new SelfAssemblingRecoFrame(Form("VSAb_PAIR%d",t),"Vb"); 
    Va_PAIR[t]   = new VisibleRecoFrame(Form("Va_PAIR%d",t),"Va");
    Vb_PAIR[t]   = new VisibleRecoFrame(Form("Vb_PAIR%d",t),"Vb");
    GVSAa_PAIR[t] = new SelfAssemblingRecoFrame(Form("GVSAa_PAIR%d",t),"GVa");
    GVSAb_PAIR[t] = new SelfAssemblingRecoFrame(Form("GVSAb_PAIR%d",t),"GVb"); 
    GVa_PAIR[t]   = new VisibleRecoFrame(Form("GVa_PAIR%d",t),"GVa");
    GVb_PAIR[t]   = new VisibleRecoFrame(Form("GVb_PAIR%d",t),"GVb");
    Ia_PAIR[t]   = new InvisibleRecoFrame(Form("Ia_PAIR%d",t),"Ia");
    Ib_PAIR[t]   = new InvisibleRecoFrame(Form("Ib_PAIR%d",t),"Ib");

    LAB_PAIR[t]->SetChildFrame(*S_PAIR[t]);
    S_PAIR[t]->AddChildFrame(*Ca_PAIR[t]);
    S_PAIR[t]->AddChildFrame(*Cb_PAIR[t]);

    Ca_PAIR[t]->AddChildFrame(*VSAa_PAIR[t]);
    Ca_PAIR[t]->AddChildFrame(*GCa_PAIR[t]);
    Cb_PAIR[t]->AddChildFrame(*VSAb_PAIR[t]);
    Cb_PAIR[t]->AddChildFrame(*GCb_PAIR[t]);

    GCa_PAIR[t]->AddChildFrame(*GVSAa_PAIR[t]);
    GCa_PAIR[t]->AddChildFrame(*Ia_PAIR[t]);
    GCb_PAIR[t]->AddChildFrame(*GVSAb_PAIR[t]);
    GCb_PAIR[t]->AddChildFrame(*Ib_PAIR[t]);
    
    VSAa_PAIR[t]->AddChildFrame(*Va_PAIR[t]);
    VSAb_PAIR[t]->AddChildFrame(*Vb_PAIR[t]);
    GVSAa_PAIR[t]->AddChildFrame(*GVa_PAIR[t]);
    GVSAb_PAIR[t]->AddChildFrame(*GVb_PAIR[t]);

    if(!LAB_PAIR[t]->InitializeTree()){
      cout << "Problem initializing PAIR tree " << t << endl;
    }
    
    INV_PAIR[t] = new InvisibleGroup(Form("INV_PAIR%d",t),"Invisible System");
    INV_PAIR[t]->AddFrame(*Ia_PAIR[t]);
    INV_PAIR[t]->AddFrame(*Ib_PAIR[t]);
    InvM_PAIR[t] = new SetMassInvJigsaw(Form("InvM_PAIR%d",t), "Invisible mass Jigsaw");
    INV_PAIR[t]->AddJigsaw(*InvM_PAIR[t]);
    InvEta_PAIR[t] = new SetRapidityInvJigsaw(Form("InvEta_PAIR%d",t), "Set inv. system rapidity");
    INV_PAIR[t]->AddJigsaw(*InvEta_PAIR[t]);
    InvEta_PAIR[t]->AddVisibleFrames(S_PAIR[t]->GetListVisibleFrames());
    InvSplit_PAIR[t] = new MinMassesSqInvJigsaw(Form("InvSplit_PAIR%d",t), "INV -> I_{a}+ I_{b} jigsaw", 2);
    INV_PAIR[t]->AddJigsaw(*InvSplit_PAIR[t]);
    InvSplit_PAIR[t]->AddVisibleFrames(Ca_PAIR[t]->GetListVisibleFrames(), 0);
    InvSplit_PAIR[t]->AddVisibleFrames(Cb_PAIR[t]->GetListVisibleFrames(), 1);
    InvSplit_PAIR[t]->AddInvisibleFrame(*Ia_PAIR[t], 0);
    InvSplit_PAIR[t]->AddInvisibleFrame(*Ib_PAIR[t], 1);

    COMB_PAIR[t] = new CombinatoricGroup(Form("COMB_PAIR%d",t), "Combinatoric System");
    COMBa_PAIR[t] = new CombinatoricGroup(Form("COMBa_PAIR%d",t), "Combinatoric System a");
    COMBb_PAIR[t] = new CombinatoricGroup(Form("COMBb_PAIR%d",t), "Combinatoric System b");
    CombSplit_PAIR[t] = new MinMassesCombJigsaw(Form("CombSplit_PAIR%d",t), "Minimize M_{Va} and M_{Vb} Jigsaw");
    CombSplita_PAIR[t] = new MinMassesCombJigsaw(Form("CombSplita_PAIR%d",t), "Minimize M_{Ca} Comb. Jigsaw");
    CombSplitb_PAIR[t] = new MinMassesCombJigsaw(Form("CombSplitb_PAIR%d",t), "Minimize M_{Cb} Comb. Jigsaw");
    
    if(t == 0 || t == 1){ // fixed content hemispheres
      COMBa_PAIR[t]->AddFrame(*Va_PAIR[t]);
      COMBb_PAIR[t]->AddFrame(*Vb_PAIR[t]);
      COMBa_PAIR[t]->SetNElementsForFrame(*Va_PAIR[t], 1);
      COMBb_PAIR[t]->SetNElementsForFrame(*Vb_PAIR[t], 1);
      COMBa_PAIR[t]->AddFrame(*GVa_PAIR[t]);
      COMBb_PAIR[t]->AddFrame(*GVb_PAIR[t]);
      COMBa_PAIR[t]->SetNElementsForFrame(*GVa_PAIR[t], 0);
      COMBb_PAIR[t]->SetNElementsForFrame(*GVb_PAIR[t], 0);

      COMBa_PAIR[t]->AddJigsaw(*CombSplita_PAIR[t]);
      COMBb_PAIR[t]->AddJigsaw(*CombSplitb_PAIR[t]);

      CombSplita_PAIR[t]->AddCombFrame(*Va_PAIR[t], 0);
      CombSplita_PAIR[t]->AddCombFrame(*GVa_PAIR[t], 1);
      CombSplita_PAIR[t]->AddObjectFrame(*Va_PAIR[t], 0);
      CombSplita_PAIR[t]->AddObjectFrame(*GVa_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddObjectFrame(*Ia_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddCombFrame(*Vb_PAIR[t], 0);
      CombSplitb_PAIR[t]->AddCombFrame(*GVb_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddObjectFrame(*Vb_PAIR[t], 0);
      CombSplitb_PAIR[t]->AddObjectFrame(*GVb_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddObjectFrame(*Ib_PAIR[t], 1);
      
    }
    if(t >= 2){ // select objects combinatorically assigned
      COMB_PAIR[t]->AddFrame(*Va_PAIR[t]);
      COMB_PAIR[t]->AddFrame(*Vb_PAIR[t]);
      COMB_PAIR[t]->SetNElementsForFrame(*Va_PAIR[t], 1);
      COMB_PAIR[t]->SetNElementsForFrame(*Vb_PAIR[t], 1);
      COMB_PAIR[t]->AddFrame(*GVa_PAIR[t]);
      COMB_PAIR[t]->AddFrame(*GVb_PAIR[t]);
      COMB_PAIR[t]->SetNElementsForFrame(*GVa_PAIR[t], 0);
      COMB_PAIR[t]->SetNElementsForFrame(*GVb_PAIR[t], 0);
      COMB_PAIR[t]->AddJigsaw(*CombSplit_PAIR[t]);
      CombSplit_PAIR[t]->AddCombFrame(*Va_PAIR[t], 0);
      CombSplit_PAIR[t]->AddCombFrame(*GVa_PAIR[t], 0);
      CombSplit_PAIR[t]->AddCombFrame(*Vb_PAIR[t], 1);
      CombSplit_PAIR[t]->AddCombFrame(*GVb_PAIR[t], 1);
      CombSplit_PAIR[t]->AddObjectFrame(*Va_PAIR[t], 0);
      CombSplit_PAIR[t]->AddObjectFrame(*GVa_PAIR[t], 0);
      CombSplit_PAIR[t]->AddObjectFrame(*Vb_PAIR[t], 1);
      CombSplit_PAIR[t]->AddObjectFrame(*GVb_PAIR[t], 1);

      COMB_PAIR[t]->AddJigsaw(*CombSplita_PAIR[t]);
      COMB_PAIR[t]->AddJigsaw(*CombSplitb_PAIR[t]);
      
      CombSplita_PAIR[t]->AddCombFrame(*Va_PAIR[t], 0);
      CombSplita_PAIR[t]->AddCombFrame(*GVa_PAIR[t], 1);
      CombSplita_PAIR[t]->AddObjectFrame(*Va_PAIR[t], 0);
      CombSplita_PAIR[t]->AddObjectFrame(*GVa_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddObjectFrame(*Ia_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddCombFrame(*Vb_PAIR[t], 0);
      CombSplitb_PAIR[t]->AddCombFrame(*GVb_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddObjectFrame(*Vb_PAIR[t], 0);
      CombSplitb_PAIR[t]->AddObjectFrame(*GVb_PAIR[t], 1);
      CombSplitb_PAIR[t]->AddObjectFrame(*Ib_PAIR[t], 1);
    }

    if(!LAB_PAIR[t]->InitializeAnalysis()){
      cout << "Problem initializing PAIR analysis " << t << endl;
    }
  }

  /*
  TreePlot tree_plot("TreePlot","TreePlot");

  for(int t = 0; t < 3; t++){
    tree_plot.SetTree(*LAB_ISR[t]);
    tree_plot.Draw(Form("ISR_tree_%d",t), Form("ISR Reconstruction Tree %d",t));

    tree_plot.SetTree(*COMB_ISR[t]);
    tree_plot.Draw(Form("ISR_comb_%d",t), Form("ISR Combinatoric Jigsaws %d",t),1);
  }

  for(int t = 0; t < 5; t++){
    tree_plot.SetTree(*LAB_PAIR[t]);
    tree_plot.Draw(Form("PAIR_tree_%d",t), Form("PAIR Reconstruction Tree %d",t));

    tree_plot.SetTree(*COMB_PAIR[t]);
    tree_plot.Draw(Form("PAIR_comb_%d",t), Form("PAIR Combinatoric Jigsaws %d",t),1);

    tree_plot.SetTree(*INV_PAIR[t]);
    tree_plot.Draw(Form("PAIR_inv_%d",t), Form("PAIR Invisible Jigsaws %d",t),1);
  }

  tree_plot.WriteOutput("trees.root");
  */

}

template <class Base>
ReducedNtuple<Base>::~ReducedNtuple() {
  for(int i = 0; i < 3; i++){
    // ISR-style trees
    delete LAB_ISR[i];
    delete CM_ISR[i];
    delete S_ISR[i];
    delete ISR_ISR[i];
    delete V_ISR[i];
    delete L_ISR[i];
    delete I_ISR[i];

    delete INV_ISR[i];
    delete InvM_ISR[i];
    delete COMB_ISR[i];
    delete CombSplit_ISR[i];
  }
  for(int i = 0; i < 5; i++){
    // Sparticle pair-production trees
    delete LAB_PAIR[i];
    delete S_PAIR[i];
    delete Ca_PAIR[i];
    delete Cb_PAIR[i];
    delete GCa_PAIR[i];
    delete GCb_PAIR[i];
    delete VSAa_PAIR[i];
    delete VSAb_PAIR[i];
    delete Va_PAIR[i];
    delete Vb_PAIR[i];
    delete GVSAa_PAIR[i];
    delete GVSAb_PAIR[i];
    delete GVa_PAIR[i];
    delete GVb_PAIR[i];
    delete Ia_PAIR[i];
    delete Ib_PAIR[i];

    delete INV_PAIR[i];
    delete InvM_PAIR[i];
    delete InvEta_PAIR[i];
    delete InvSplit_PAIR[i];
    delete COMB_PAIR[i];
    delete CombSplit_PAIR[i];
    delete COMBa_PAIR[i];
    delete COMBb_PAIR[i];
    delete CombSplita_PAIR[i];
    delete CombSplitb_PAIR[i];
  }
  
}

template <class Base>
TTree* ReducedNtuple<Base>::InitOutputTree(const string& sample){

  gInterpreter->GenerateDictionary("vector<vector<int>>", "vector");
  
  TTree* tree = (TTree*) new TTree(sample.c_str(), sample.c_str());

  tree->Branch("weight", &m_weight);
  
  tree->Branch("MET", &m_MET);
  tree->Branch("MET_phi", &m_MET_phi);
  tree->Branch("genMET", &m_genMET);
  tree->Branch("genMET_phi", &m_genMET_phi);

  tree->Branch("HT", &m_HT);

  tree->Branch("Nele", &m_Nele);
  tree->Branch("Nmu", &m_Nmu);
  
  tree->Branch("Nlep", &m_Nlep);
  tree->Branch("PT_lep",  &m_PT_lep);
  tree->Branch("Eta_lep", &m_Eta_lep);
  tree->Branch("Phi_lep", &m_Phi_lep);
  tree->Branch("M_lep",   &m_M_lep);
  tree->Branch("Charge_lep",  &m_Charge_lep);
  tree->Branch("PDGID_lep",   &m_PDGID_lep);
  tree->Branch("RelIso_lep",  &m_RelIso_lep);
  tree->Branch("MiniIso_lep", &m_MiniIso_lep);
  tree->Branch("ID_lep",      &m_ID_lep);
  tree->Branch("Index_lep",   &m_Index_lep);

  tree->Branch("Njet", &m_Njet);
  tree->Branch("Nbjet", &m_Nbjet);
  tree->Branch("PT_jet",  &m_PT_jet);
  tree->Branch("Eta_jet", &m_Eta_jet);
  tree->Branch("Phi_jet", &m_Phi_jet);
  tree->Branch("M_jet",   &m_M_jet);
  tree->Branch("Btag_jet",   &m_Btag_jet);
 
  tree->Branch("genNele", &m_genNele);
  tree->Branch("genNmu", &m_genNmu);

  tree->Branch("genNlep", &m_genNlep);
  tree->Branch("genPT_lep",  &m_genPT_lep);
  tree->Branch("genEta_lep", &m_genEta_lep);
  tree->Branch("genPhi_lep", &m_genPhi_lep);
  tree->Branch("genM_lep",   &m_genM_lep);
  tree->Branch("genCharge_lep",  &m_genCharge_lep);
  tree->Branch("genPDGID_lep",   &m_genPDGID_lep);
  tree->Branch("genIndex_lep",   &m_genIndex_lep);

  tree->Branch("genNnu", &m_genNnu);
  tree->Branch("genPT_nu",  &m_genPT_nu);
  tree->Branch("genEta_nu", &m_genEta_nu);
  tree->Branch("genPhi_nu", &m_genPhi_nu);
  tree->Branch("genPDGID_nu",   &m_genPDGID_nu);

  tree->Branch("genNboson", &m_genNboson);
  tree->Branch("genPT_boson",  &m_genPT_boson);
  tree->Branch("genEta_boson", &m_genEta_boson);
  tree->Branch("genPhi_boson", &m_genPhi_boson);
  tree->Branch("genM_boson",   &m_genM_boson);
  tree->Branch("genPDGID_boson",   &m_genPDGID_boson);

  tree->Branch("genNsusy", &m_genNsusy);
  tree->Branch("genPT_susy",  &m_genPT_susy);
  tree->Branch("genEta_susy", &m_genEta_susy);
  tree->Branch("genPhi_susy", &m_genPhi_susy);
  tree->Branch("genM_susy",   &m_genM_susy);
  tree->Branch("genPDGID_susy", &m_genPDGID_susy);
  
  // Calculated Observables
  for(int i = 0; i < 5; i++){
    m_Njet_a.push_back(0);
    m_Njet_b.push_back(0);
    m_Nbjet_a.push_back(0);
    m_Nbjet_b.push_back(0);
    m_Nlep_a.push_back(0);
    m_Nlep_b.push_back(0);
    m_Njet_ga.push_back(0);
    m_Njet_gb.push_back(0);
    m_Nbjet_ga.push_back(0);
    m_Nbjet_gb.push_back(0);
    m_Nlep_ga.push_back(0);
    m_Nlep_gb.push_back(0);
    m_index_jet_a.push_back(vector<int>());
    m_index_jet_b.push_back(vector<int>());
    m_index_lep_a.push_back(vector<int>());
    m_index_lep_b.push_back(vector<int>());
    m_index_jet_ga.push_back(vector<int>());
    m_index_jet_gb.push_back(vector<int>());
    m_index_lep_ga.push_back(vector<int>());
    m_index_lep_gb.push_back(vector<int>());
    m_MSS.push_back(0);
    m_PSS.push_back(0);
    m_cosSS.push_back(0);
    m_dphiSS.push_back(0);
    m_PTSS.push_back(0);
    m_PzSS.push_back(0);
    m_MCa.push_back(0);
    m_cosCa.push_back(0);
    m_MCb.push_back(0);
    m_cosCb.push_back(0);
    m_MGCa.push_back(0);
    m_cosGCa.push_back(0);
    m_MGCb.push_back(0);
    m_cosGCb.push_back(0);
    m_H11SS.push_back(0);
    m_H21SS.push_back(0);
    m_HT21SS.push_back(0);
    m_H22SS.push_back(0);
    m_HT22SS.push_back(0);
    m_H42SS.push_back(0);
    m_HT42SS.push_back(0);
    m_H11Ca.push_back(0);
    m_H11Cb.push_back(0);
    m_H21Ca.push_back(0);
    m_H21Cb.push_back(0);

    m_MVa.push_back(0);
    m_PVa.push_back(0);
    m_cosVa.push_back(0);
    m_MVb.push_back(0);
    m_PVb.push_back(0);
    m_cosVb.push_back(0);
  }
  tree->Branch("Njet_a", &m_Njet_a);
  tree->Branch("Njet_b", &m_Njet_b);
  tree->Branch("Nbjet_a", &m_Nbjet_a);
  tree->Branch("Nbjet_b", &m_Nbjet_b);
  tree->Branch("Nlep_a", &m_Nlep_a);
  tree->Branch("Nlep_b", &m_Nlep_b);
  tree->Branch("Njet_ga", &m_Njet_ga);
  tree->Branch("Njet_gb", &m_Njet_gb);
  tree->Branch("Nbjet_ga", &m_Nbjet_ga);
  tree->Branch("Nbjet_gb", &m_Nbjet_gb);
  tree->Branch("Nlep_ga", &m_Nlep_ga);
  tree->Branch("Nlep_gb", &m_Nlep_gb);
  tree->Branch("index_jet_a", &m_index_jet_a);
  tree->Branch("index_jet_b", &m_index_jet_b);
  tree->Branch("index_lep_a", &m_index_lep_a);
  tree->Branch("index_lep_b", &m_index_lep_b);
  tree->Branch("index_jet_ga", &m_index_jet_ga);
  tree->Branch("index_jet_gb", &m_index_jet_gb);
  tree->Branch("index_lep_ga", &m_index_lep_ga);
  tree->Branch("index_lep_gb", &m_index_lep_gb);
  tree->Branch("MSS", &m_MSS);
  tree->Branch("PSS", &m_PSS);
  tree->Branch("cosSS", &m_cosSS);
  tree->Branch("dphiSS", &m_dphiSS);
  tree->Branch("PTSS", &m_PTSS);
  tree->Branch("PzSS", &m_PzSS);
  tree->Branch("MCa", &m_MCa);
  tree->Branch("cosCa", &m_cosCa);
  tree->Branch("MCb", &m_MCb);
  tree->Branch("cosCb", &m_cosCb);
  tree->Branch("MGCa", &m_MGCa);
  tree->Branch("cosGCa", &m_cosGCa);
  tree->Branch("MGCb", &m_MGCb);
  tree->Branch("cosGCb", &m_cosGCb);
  tree->Branch("H11SS", &m_H11SS);
  tree->Branch("H21SS", &m_H21SS);
  tree->Branch("HT21SS", &m_HT21SS);
  tree->Branch("H22SS", &m_H22SS);
  tree->Branch("HT22SS", &m_HT22SS);
  tree->Branch("H42SS", &m_H42SS);
  tree->Branch("HT42SS", &m_HT42SS);
  tree->Branch("H11Ca", &m_H11Ca);
  tree->Branch("H11Cb", &m_H11Cb);
  tree->Branch("H21Ca", &m_H21Ca);
  tree->Branch("H21Cb", &m_H21Cb);
  tree->Branch("MVa", &m_MVa);
  tree->Branch("PVa", &m_PVa);
  tree->Branch("cosVa", &m_cosVa);
  tree->Branch("MVb", &m_MVb);
  tree->Branch("PVb", &m_PVb);
  tree->Branch("cosVb", &m_cosVb);
  // which tree are we using for PAIR?
  // vanilla - index 0
  tree->Branch("Is_1L_2J", &m_Is_1L_2J);
  tree->Branch("Is_2L_2J", &m_Is_2L_2J);
  tree->Branch("Is_1L_1L", &m_Is_1L_1L);
  tree->Branch("Is_2L_1L", &m_Is_2L_1L);
  tree->Branch("Is_2L_2L", &m_Is_2L_2L);
  // b-aware - index 0
  tree->Branch("Is_1L_B", &m_Is_1L_B);
  tree->Branch("Is_2L_B", &m_Is_2L_B);
  tree->Branch("Is_1LB_1LB", &m_Is_1LB_1LB);
  tree->Branch("Is_3L_B", &m_Is_3L_B);
 

  for(int i = 0; i < 3; i++){
    m_Njet_ISR.push_back(0);
    m_Njet_S.push_back(0);
    m_Nbjet_ISR.push_back(0);
    m_Nbjet_S.push_back(0);
    m_Nlep_ISR.push_back(0);
    m_Nlep_S.push_back(0);
    m_index_jet_ISR.push_back(vector<int>());
    m_index_jet_S.push_back(vector<int>());
    m_index_lep_ISR.push_back(vector<int>());
    m_index_lep_S.push_back(vector<int>());
    m_PTISR.push_back(0);
    m_PTCM.push_back(0);
    m_RISR.push_back(0);
    m_cosCM.push_back(0);
    m_cosS.push_back(0);
    m_MISR.push_back(0);
    m_MS.push_back(0);
    m_MV.push_back(0);
    m_ML.push_back(0);
    m_dphiCMI.push_back(0);
    m_dphiSI.push_back(0);
    m_dphiISRI.push_back(0);
  }
  tree->Branch("Njet_ISR", &m_Njet_ISR);
  tree->Branch("Njet_S", &m_Njet_S);
  tree->Branch("Nbjet_ISR", &m_Nbjet_ISR);
  tree->Branch("Nbjet_S", &m_Nbjet_S);
  tree->Branch("Nlep_ISR", &m_Nlep_ISR);
  tree->Branch("Nlep_S", &m_Nlep_S);
  tree->Branch("index_jet_ISR", &m_index_jet_ISR);
  tree->Branch("index_jet_S", &m_index_jet_S);
  tree->Branch("index_lep_ISR", &m_index_lep_ISR);
  tree->Branch("index_lep_S", &m_index_lep_S);
  tree->Branch("PTISR", &m_PTISR);
  tree->Branch("PTCM", &m_PTCM);
  tree->Branch("RISR", &m_RISR);
  tree->Branch("cosCM", &m_cosCM);
  tree->Branch("cosS", &m_cosS);
  tree->Branch("MISR", &m_MISR);
  tree->Branch("MS", &m_MS);
  tree->Branch("MV", &m_MV);
  tree->Branch("ML", &m_ML);
  tree->Branch("dphiCMI", &m_dphiCMI);
  tree->Branch("dphiSI", &m_dphiSI);
  tree->Branch("dphiISRI", &m_dphiISRI);

  return tree;
}

template <class Base>
void ReducedNtuple<Base>::ClearVariables(){
  for(int i = 0; i < 5; i++){
    m_Njet_a[i] = 0;
    m_Njet_b[i] = 0;
    m_Nbjet_a[i] = 0;
    m_Nbjet_b[i] = 0;
    m_Nlep_a[i] = 0;
    m_Nlep_b[i] = 0;
    m_Njet_ga[i] = 0;
    m_Njet_gb[i] = 0;
    m_Nbjet_ga[i] = 0;
    m_Nbjet_gb[i] = 0;
    m_Nlep_ga[i] = 0;
    m_Nlep_gb[i] = 0;
    m_index_jet_a[i].clear();
    m_index_jet_b[i].clear();
    m_index_lep_a[i].clear();
    m_index_lep_b[i].clear();
    m_index_jet_ga[i].clear();
    m_index_jet_gb[i].clear();
    m_index_lep_ga[i].clear();
    m_index_lep_gb[i].clear();
    m_MSS[i] = 0;
    m_PSS[i] = 0;
    m_cosSS[i] = 0;
    m_dphiSS[i] = 0;
    m_PTSS[i] = 0;
    m_PzSS[i] = 0;
    m_MCa[i] = 0;
    m_cosCa[i] = 0;
    m_MCb[i] = 0;
    m_cosCb[i] = 0;
    m_MGCa[i] = 0;
    m_cosGCa[i] = 0;
    m_MGCb[i] = 0;
    m_cosCb[i] = 0;
    m_H11SS[i] = 0;
    m_H21SS[i] = 0;
    m_HT21SS[i] = 0;
    m_H22SS[i] = 0;
    m_HT22SS[i] = 0;
    m_H42SS[i] = 0;
    m_HT42SS[i] = 0;
    m_H11Ca[i] = 0;
    m_H11Cb[i] = 0;
    m_H21Ca[i] = 0;
    m_H21Cb[i] = 0;
   
    m_MVa[i] = 0;
    m_PVa[i] = 0;
    m_cosVa[i] = 0;
    m_MVb[i] = 0;
    m_PVb[i] = 0;
    m_cosVb[i] = 0;
  }

  m_Is_1L_2J = false;
  m_Is_2L_2J = false;
  m_Is_1L_1L = false;
  m_Is_2L_1L = false;
  m_Is_2L_2L = false;
  
  m_Is_1L_B = false;
  m_Is_2L_B = false;
  m_Is_1LB_1LB = false;
  m_Is_3L_B = false;
  
  for(int i = 0; i < 3; i++){
    m_Njet_ISR[i] = 0;
    m_Njet_S[i] = 0;
    m_Nbjet_ISR[i] = 0;
    m_Nbjet_S[i] = 0;
    m_Nlep_ISR[i] = 0;
    m_Nlep_S[i] = 0;
    m_index_jet_ISR[i].clear();
    m_index_jet_S[i].clear();
    m_index_lep_ISR[i].clear();
    m_index_lep_S[i].clear();
    m_PTISR[i] = 0;
    m_PTCM[i] = 0;
    m_RISR[i] = 0;
    m_cosCM[i] = 0;
    m_cosS[i] = 0;
    m_MISR[i] = 0;
    m_MS[i] = 0;
    m_MV[i] = 0;
    m_ML[i] = 0;
    m_dphiCMI[i] = 0;
    m_dphiSI[i] = 0;
    m_dphiISRI[i] = 0;
  }
}

template <class Base>
void ReducedNtuple<Base>::FillOutputTree(TTree* tree){
  
  ParticleList Jets = AnalysisBase<Base>::GetJets();
  Jets = Jets.PtEtaCut(20., 2.4);
  m_Njet = Jets.size();
  
  ParticleList BJets;
  vector<int> BJets_index;
  for(int i = 0; i < m_Njet; i++){
    if(Jets[i].ParticleID() >= kMedium){
      BJets += Jets[i];
      BJets_index.push_back(i);
    }
  }
  m_Nbjet = BJets.size();
  
  TVector3 ETMiss = AnalysisBase<Base>::GetMET();

  if(ETMiss.Mag() < 100.)
    return;

  ClearVariables();
  
  ParticleList Muons = AnalysisBase<Base>::GetMuons();
  Muons = Muons.ParticleIDCut(kLoose);
  Muons = Muons.PtEtaCut(3.5);

  ParticleList Electrons = AnalysisBase<Base>::GetElectrons();
  Electrons = Electrons.ParticleIDCut(kLoose);
  Electrons = Electrons.PtEtaCut(3.5);
  
  ParticleList Leptons = Electrons+Muons;
  Leptons.SortByPt();

  m_Nele = Electrons.size();
  m_Nmu  = Muons.size();
  m_Nlep = Leptons.size();

  // require at least one lepton for now
  if(m_Nlep < 1)
    return;

  // not enough stuff
  if(m_Nlep + m_Njet < 2)
    return;
  
  bool is_filled[3];
  for(int i = 0; i < 5; i++)
    is_filled[i] = true;
  

  // Sparticle pair-production trees analysis
  for(int t = 0; t < 5; t++){
    LAB_PAIR[t]->ClearEvent();

    INV_PAIR[t]->SetLabFrameThreeVector(ETMiss);
    
    if(t == 0){ // fixed content hemispheres
      is_filled[t] = false;
      
      // 1 leptons
      if(Leptons.size() == 1){
	m_Is_1L_2J = true;
	is_filled[t] = true;
	
	COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[0]);
	m_Nlep_a[t]++;
	m_index_lep_a[t].push_back(0);

	double Mjet = -1.;
	int Mjet_index[2];
	Mjet_index[0] = 0;
	Mjet_index[1] = -1;
	
	for(int i = 0; i < m_Njet; i++){
	  if(fabs(Jets[i].M()-81.) < Mjet || Mjet < 0.){
	    Mjet = fabs(Jets[i].M()-81.);
	    Mjet_index[0] = i;
	    Mjet_index[1] = -1;
	  }
	  for(int j = i+1; j < m_Njet; j++){
	    double Mjets = TLorentzVector(Jets[i]+Jets[j]).M();
	    if(fabs(Mjets - 81.) < Mjet){
	      Mjet = fabs(Mjets - 81.);
	      Mjet_index[0] = i;
	      Mjet_index[1] = j;
	    }
	  }
	}
	
	std::vector<RFKey> jetID;
	jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Jets[Mjet_index[0]]));
	if(Mjet_index[1] >= 0)
	  jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Jets[Mjet_index[1]]));
	
	if(!LAB_PAIR[t]->AnalyzeEvent())
	  cout << "Something went wrong with PAIR tree event analysis " << t << endl;

	if(COMBb_PAIR[t]->GetFrame(jetID[0]) == *Vb_PAIR[t]){
	  m_Njet_b[t]++;
	  if(Jets[Mjet_index[0]].ParticleID() >= kMedium)
	    m_Nbjet_b[t]++;
	  m_index_jet_b[t].push_back(Mjet_index[0]);
	}
	if(COMBb_PAIR[t]->GetFrame(jetID[0]) == *GVb_PAIR[t]){
	  m_Njet_gb[t]++;
	  if(Jets[Mjet_index[0]].ParticleID() >= kMedium)
	    m_Nbjet_gb[t]++;
	  m_index_jet_gb[t].push_back(Mjet_index[0]);
	}
	if(Mjet_index[1] >= 0){
	  if(COMBb_PAIR[t]->GetFrame(jetID[1]) == *Vb_PAIR[t]){
	    m_Njet_b[t]++;
	    if(Jets[Mjet_index[1]].ParticleID() >= kMedium)
	      m_Nbjet_b[t]++;
	    m_index_jet_b[t].push_back(Mjet_index[1]);
	  }
	  if(COMBb_PAIR[t]->GetFrame(jetID[1]) == *GVb_PAIR[t]){
	    m_Njet_gb[t]++;
	    if(Jets[Mjet_index[1]].ParticleID() >= kMedium)
	      m_Nbjet_gb[t]++;
	    m_index_jet_gb[t].push_back(Mjet_index[1]);
	  }
	}
      
	// 2 leptons
      } else if(Leptons.size() == 2){
	if(Leptons[0].PDGID()+Leptons[1].PDGID() == 0){
	  m_Is_2L_2J = true;
	  is_filled[t] = true;

	  std::vector<RFKey> lepID;
	  for(int i = 0; i < m_Nlep; i++) 
	    lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));

	  double Mjet = -1.;
	  int Mjet_index[2];
	  Mjet_index[0] = 0;
	  Mjet_index[1] = -1;
	  
	  for(int i = 0; i < m_Njet; i++){
	    if(fabs(Jets[i].M()-81.) < Mjet || Mjet < 0.){
	      Mjet_index[0] = i;
	      Mjet_index[1] = -1;
	    }
	    for(int j = i+1; j < m_Njet; j++){
	      double Mjets = TLorentzVector(Jets[i]+Jets[j]).M();
	      if(fabs(Mjets - 81.) < Mjet){
		Mjet_index[0] = i;
		Mjet_index[1] = j;
	      }
	    }
	  }
	
	  std::vector<RFKey> jetID;
	  jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Jets[Mjet_index[0]]));
	  if(Mjet_index[1] >= 0)
	    jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Jets[Mjet_index[1]]));
	
	  if(!LAB_PAIR[t]->AnalyzeEvent())
	    cout << "Something went wrong with PAIR tree event analysis " << t << endl;

	  if(COMBb_PAIR[t]->GetFrame(jetID[0]) == *Vb_PAIR[t]){
	    m_Njet_b[t]++;
	    if(Jets[Mjet_index[0]].ParticleID() >= kMedium)
	      m_Nbjet_b[t]++;
	    m_index_jet_b[t].push_back(Mjet_index[0]);
	  }
	  if(COMBb_PAIR[t]->GetFrame(jetID[0]) == *GVb_PAIR[t]){
	    m_Njet_gb[t]++;
	    if(Jets[Mjet_index[0]].ParticleID() >= kMedium)
	      m_Nbjet_gb[t]++;
	    m_index_jet_gb[t].push_back(Mjet_index[0]);
	  }
	  if(Mjet_index[1] >= 0){
	    if(COMBb_PAIR[t]->GetFrame(jetID[1]) == *Vb_PAIR[t]){
	      m_Njet_b[t]++;
	      if(Jets[Mjet_index[1]].ParticleID() >= kMedium)
		m_Nbjet_b[t]++;
	      m_index_jet_b[t].push_back(Mjet_index[1]);
	    }
	    if(COMBb_PAIR[t]->GetFrame(jetID[1]) == *GVb_PAIR[t]){
	      m_Njet_gb[t]++;
	      if(Jets[Mjet_index[1]].ParticleID() >= kMedium)
		m_Nbjet_gb[t]++;
	      m_index_jet_gb[t].push_back(Mjet_index[1]);
	    }
	  }

	  for(int i = 0; i < m_Nlep; i++){
	    if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	      m_Nlep_a[t]++;
	      m_index_lep_a[t].push_back(i);
	    }
	    if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	      m_Njet_ga[t]++;
	      m_index_lep_ga[t].push_back(i);
	    }
	  }

	} else {
	  m_Is_1L_1L = true;
	  is_filled[t] = true;

	  std::vector<RFKey> lepID; 
	  lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[0]));
	  lepID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Leptons[1]));
	
	  if(!LAB_PAIR[t]->AnalyzeEvent())
	    cout << "Something went wrong with PAIR tree event analysis " << t << endl;

	  m_Nlep_a[t]++;
	  m_index_lep_a[t].push_back(0);
	  m_Nlep_b[t]++;
	  m_index_lep_b[t].push_back(0);
	}
	// 3 leptons
      } else if(Leptons.size() == 3){
	m_Is_2L_1L = true;
	is_filled[t] = true;

	std::pair<int,int> OSSF;
	double M_OSSF = -1.;

	std::pair<int,int> minM;
	double M_minM = -1.;
	
	for(int i = 0; i < 2; i++){
	  for(int j = i+1; j < 3; j++){
	    double M = TLorentzVector(Leptons[i]+Leptons[j]).M();
	    if(M < M_minM || M_minM < -1.){
	      M_minM = M;
	      minM.first = i;
	      minM.second = j;
	    }
	    if(Leptons[i].PDGID() + Leptons[j].PDGID() == 0){
	      if(M < M_OSSF || M_OSSF < -1.){
		M_OSSF = M;
		OSSF.first = i;
		OSSF.second = j;
	      }
	    }
	  }
	}

	std::vector<RFKey> lepID;
	
	// no OSSF pair
	if(M_OSSF < 0.){
	  for(int i = 0; i < m_Nlep; i++){
	    if(i == minM.first)
	      lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	    else if(i == minM.second)
	      lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	    else 
	      lepID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	  }
	  // OSSF pair
	} else {
	  for(int i = 0; i < m_Nlep; i++){
	    if(i == OSSF.first)
	      lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	    else if(i == OSSF.second)
	      lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	    else 
	      lepID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	  }
	}

	if(!LAB_PAIR[t]->AnalyzeEvent())
	  cout << "Something went wrong with PAIR tree event analysis " << t << endl;
	
	for(int i = 0; i < m_Nlep; i++){
	  if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	    m_Nlep_a[t]++;
	    m_index_lep_a[t].push_back(i);
	  }
	  if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	    m_Nlep_ga[t]++;
	    m_index_lep_ga[t].push_back(i);
	  }
	  if(COMBb_PAIR[t]->GetFrame(lepID[i]) == *Vb_PAIR[t]){
	    m_Nlep_b[t]++;
	    m_index_lep_b[t].push_back(i);
	  }
	}
     
	// 4 leptons
      } else if(Leptons.size() == 4){
	m_Is_2L_2L = true;
	is_filled[t] = true;

	
	int OSSF[2];
	double M_OSSF = -1.;
	int n_OSSF = 0;

	int minM[2];
	double M_minM = -1.;

	TLorentzVector PTOT;
	int PDGTOT;
	for(int i = 0; i < 4; i++){
	  PTOT += Leptons[i];
	  PDGTOT += Leptons[i].PDGID();
	}
	
	for(int i = 0; i < 3; i++){
	  for(int j = i+1; j < 4; j++){
	    TLorentzVector P1 = Leptons[i]+Leptons[j];
	    TLorentzVector P2 = PTOT - P1;
	    
	    double M = P1.M2() + P2.M2();
	    
	    int n = 0;
	    if(Leptons[i].PDGID() + Leptons[j].PDGID() == 0)
	      n++;
	    if(PDGTOT - Leptons[i].PDGID() - Leptons[j].PDGID() == 0)
	      n++;
	    
	    if(n >= n_OSSF){
	      if(((M_OSSF < 0) || (M < M_OSSF)) || (n > n_OSSF)){
		M_OSSF = M;
		n_OSSF = n;
		OSSF[0] = i;
		OSSF[1] = j;
	      }
	    } 
	  }
	}
	
	std::vector<RFKey> lepID;
	for(int i = 0; i < m_Nlep; i++){
	  if(i == OSSF[0] || i == OSSF[1]){
	    lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	  } else {
	    lepID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Leptons[i]));
	  }
	}
	  
	if(!LAB_PAIR[t]->AnalyzeEvent())
	  cout << "Something went wrong with PAIR tree event analysis " << t << " 4L" << endl;
	
	for(int i = 0; i < m_Nlep; i++){
	  if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	    m_Nlep_a[t]++;
	    m_index_lep_a[t].push_back(i);
	  }
	  if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	    m_Nlep_ga[t]++;
	    m_index_lep_ga[t].push_back(i);
	  }
	  if(COMBb_PAIR[t]->GetFrame(lepID[i]) == *Vb_PAIR[t]){
	    m_Nlep_b[t]++;
	    m_index_lep_b[t].push_back(i);
	  }
	  if(COMBb_PAIR[t]->GetFrame(lepID[i]) == *GVb_PAIR[t]){
	    m_Nlep_gb[t]++;
	    m_index_lep_gb[t].push_back(i);
	  }
	}	
      } 
    }

    if(t == 1){ // fixed content hemispheres
      is_filled[t] = false;

      if(m_Nbjet == 0)
	continue;
      
      // 1 leptons
      if(Leptons.size() == 1){
	m_Is_1L_B = true;
	is_filled[t] = true;
	
	COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[0]);
	m_Nlep_a[t]++;
	m_index_lep_a[t].push_back(0);
	
	std::vector<RFKey> jetID;
	for(int i = 0; i < m_Nbjet; i++)
	  jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(BJets[i]));
	
	if(!LAB_PAIR[t]->AnalyzeEvent())
	  cout << "Something went wrong with PAIR tree event analysis " << t << endl;

	for(int i = 0; i < m_Nbjet; i++){
	  if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *Vb_PAIR[t]){
	    m_Njet_b[t]++;
	    m_Nbjet_b[t]++;
	    m_index_jet_b[t].push_back(BJets_index[i]);
	  }
	  if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *GVb_PAIR[t]){
	    m_Njet_gb[t]++;
	    m_Nbjet_gb[t]++;
	    m_index_jet_gb[t].push_back(BJets_index[i]);
	  }
	}
      
      
	// 2 leptons
      } else if(Leptons.size() == 2){
	if(Leptons[0].PDGID()+Leptons[1].PDGID() == 0){
	  m_Is_2L_B = true;
	  is_filled[t] = true;

	  std::vector<RFKey> lepID;
	  for(int i = 0; i < m_Nlep; i++) 
	    lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));

	  std::vector<RFKey> jetID;
	  for(int i = 0; i < m_Nbjet; i++)
	    jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(BJets[i]));
	
	  if(!LAB_PAIR[t]->AnalyzeEvent())
	    cout << "Something went wrong with PAIR tree event analysis " << t << endl;

	  for(int i = 0; i < m_Nbjet; i++){
	    if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *Vb_PAIR[t]){
	      m_Njet_b[t]++;
	      m_Nbjet_b[t]++;
	      m_index_jet_b[t].push_back(BJets_index[i]);
	    }
	    if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *GVb_PAIR[t]){
	      m_Njet_gb[t]++;
	      m_Nbjet_gb[t]++;
	      m_index_jet_gb[t].push_back(BJets_index[i]);
	    }
	  }

	  for(int i = 0; i < m_Nlep; i++){
	    if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	      m_Nlep_a[t]++;
	      m_index_lep_a[t].push_back(i);
	    }
	    if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	      m_Njet_ga[t]++;
	      m_index_lep_ga[t].push_back(i);
	    }
	  }

	} else {
	  if(m_Nbjet != 2){
	    continue;
	  }
	  
	  m_Is_1LB_1LB = true;
	  is_filled[t] = true;

	  std::vector<RFKey> lepID; 
	  lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[0]));
	  lepID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(Leptons[1]));
	  
	  std::vector<RFKey> jetID;
	  if(TLorentzVector(Leptons[0]+BJets[0]).M2()+
	     TLorentzVector(Leptons[1]+BJets[1]).M2() <
	     TLorentzVector(Leptons[0]+BJets[1]).M2()+
	     TLorentzVector(Leptons[1]+BJets[0]).M2()){
	    jetID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(BJets[0]));
	    jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(BJets[1]));
	  } else {
	    jetID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(BJets[1]));
	    jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(BJets[0]));
	  }

	  
	  if(!LAB_PAIR[t]->AnalyzeEvent())
	    cout << "Something went wrong with PAIR tree event analysis " << t << endl;

	  
	  for(int i = 0; i < m_Nbjet; i++){
	    if(COMBa_PAIR[t]->GetFrame(jetID[i]) == *Va_PAIR[t]){
	      m_Njet_a[t]++;
	      m_Nbjet_a[t]++;
	      m_index_jet_a[t].push_back(BJets_index[i]);
	    }
	    if(COMBa_PAIR[t]->GetFrame(jetID[i]) == *GVa_PAIR[t]){
	      m_Njet_ga[t]++;
	      m_Nbjet_ga[t]++;
	      m_index_jet_ga[t].push_back(BJets_index[i]);
	    }
	    if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *Vb_PAIR[t]){
	      m_Njet_b[t]++;
	      m_Nbjet_b[t]++;
	      m_index_jet_b[t].push_back(BJets_index[i]);
	    }
	    if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *GVb_PAIR[t]){
	      m_Njet_gb[t]++;
	      m_Nbjet_gb[t]++;
	      m_index_jet_gb[t].push_back(BJets_index[i]);
	    }
	  }
	  
	  for(int i = 0; i < m_Nlep; i++){
	    if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	      m_Nlep_a[t]++;
	      m_index_lep_a[t].push_back(i);
	    }
	    if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	      m_Njet_ga[t]++;
	      m_index_lep_ga[t].push_back(i);
	    }
	    if(COMBb_PAIR[t]->GetFrame(lepID[i]) == *Vb_PAIR[t]){
	      m_Nlep_b[t]++;
	      m_index_lep_b[t].push_back(i);
	    }
	    if(COMBb_PAIR[t]->GetFrame(lepID[i]) == *GVb_PAIR[t]){
	      m_Njet_gb[t]++;
	      m_index_lep_gb[t].push_back(i);
	    }
	  }
	}
	// 3 leptons
      } else if(Leptons.size() == 3){
	m_Is_3L_B = true;
	is_filled[t] = true;

	std::vector<RFKey> lepID;
	for(int i = 0; i < m_Nlep; i++)
	  lepID.push_back(COMBa_PAIR[t]->AddLabFrameFourVector(Leptons[i]));

	std::vector<RFKey> jetID;
	for(int i = 0; i < m_Nbjet; i++)
	  jetID.push_back(COMBb_PAIR[t]->AddLabFrameFourVector(BJets[i]));

	if(!LAB_PAIR[t]->AnalyzeEvent())
	  cout << "Something went wrong with PAIR tree event analysis " << t << endl;
	
	for(int i = 0; i < m_Nlep; i++){
	  if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	    m_Nlep_a[t]++;
	    m_index_lep_a[t].push_back(i);
	  }
	  if(COMBa_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	    m_Nlep_ga[t]++;
	    m_index_lep_ga[t].push_back(i);
	  }
	}

	for(int i = 0; i < m_Nbjet; i++){
	  if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *Vb_PAIR[t]){
	    m_Njet_b[t]++;
	    m_Nbjet_b[t]++;
	    m_index_jet_b[t].push_back(BJets_index[i]);
	  }
	  if(COMBb_PAIR[t]->GetFrame(jetID[i]) == *GVb_PAIR[t]){
	    m_Njet_gb[t]++;
	    m_Nbjet_gb[t]++;
	    m_index_jet_gb[t].push_back(BJets_index[i]);
	  }
	}
      }
    }

    if(t == 2){ // leptons combinatorically assigned
      
      std::vector<RFKey> lepID;
      for(int i = 0; i < m_Nlep; i++) 
	lepID.push_back(COMB_PAIR[t]->AddLabFrameFourVector(Leptons[i]));

      if(m_Nlep < 2){
	is_filled[t] = false;
	continue;
      }	
      
      if(!LAB_PAIR[t]->AnalyzeEvent())
	cout << "Something went wrong with PAIR tree event analysis " << t << endl;

      for(int i = 0; i < m_Nlep; i++){
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	  m_Nlep_a[t]++;
	  m_index_lep_a[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	  m_Nlep_ga[t]++;
	  m_index_lep_ga[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *Vb_PAIR[t]){
	  m_Nlep_b[t]++;
	  m_index_lep_b[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *GVb_PAIR[t]){
	  m_Nlep_gb[t]++;
	  m_index_lep_gb[t].push_back(i);
	}
      }
    }
    
    if(t == 3){ // leptons and b's combinatorically assigned
      std::vector<RFKey> jetID;
      int Nb = 0;
      for(int i = 0; i < m_Njet; i++){
	if(Jets[i].ParticleID() >= kMedium){
	  jetID.push_back(COMB_PAIR[t]->AddLabFrameFourVector(Jets[i]));
	  Nb++;
	} else
	  jetID.push_back(RFKey(-1));
      }
      std::vector<RFKey> lepID;
      for(int i = 0; i < m_Nlep; i++) 
	lepID.push_back(COMB_PAIR[t]->AddLabFrameFourVector(Leptons[i]));

      if(m_Nlep + Nb < 2){
	is_filled[t] = false;
	continue;
      }	
      
      if(!LAB_PAIR[t]->AnalyzeEvent())
	cout << "Something went wrong with PAIR tree event analysis " << t << endl;

      for(int i = 0; i < m_Njet; i++){
	if(jetID[i] == -1)
	  continue;
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *Va_PAIR[t]){
	  m_Njet_a[t]++;
	  m_Nbjet_a[t]++;
	  m_index_jet_a[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *GVa_PAIR[t]){
	  m_Njet_ga[t]++;
	  m_Nbjet_ga[t]++;
	  m_index_jet_ga[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *Vb_PAIR[t]){
	  m_Njet_b[t]++;
	  m_Nbjet_b[t]++;
	  m_index_jet_b[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *GVb_PAIR[t]){
	  m_Njet_gb[t]++;
	  m_Nbjet_gb[t]++;
	  m_index_jet_gb[t].push_back(i);
	} 
      }
      for(int i = 0; i < m_Nlep; i++){
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	  m_Nlep_a[t]++;
	  m_index_lep_a[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	  m_Nlep_ga[t]++;
	  m_index_lep_ga[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *Vb_PAIR[t]){
	  m_Nlep_b[t]++;
	  m_index_lep_b[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *GVb_PAIR[t]){
	  m_Nlep_gb[t]++;
	  m_index_lep_gb[t].push_back(i);
	}
      }
    }
    if(t == 4){ // everything combinatorically assigned
      std::vector<RFKey> jetID;
      for(int i = 0; i < m_Njet; i++) 
	jetID.push_back(COMB_PAIR[t]->AddLabFrameFourVector(Jets[i]));
      std::vector<RFKey> lepID;
      for(int i = 0; i < m_Nlep; i++) 
	lepID.push_back(COMB_PAIR[t]->AddLabFrameFourVector(Leptons[i]));

      if(!LAB_PAIR[t]->AnalyzeEvent())
	cout << "Something went wrong with PAIR tree event analysis " << t << endl;

      for(int i = 0; i < m_Njet; i++){
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *Va_PAIR[t]){
	  m_Njet_a[t]++;
	  if(Jets[i].ParticleID() >= kMedium)
	    m_Nbjet_a[t]++;
	  m_index_jet_a[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *GVa_PAIR[t]){
	  m_Njet_ga[t]++;
	  if(Jets[i].ParticleID() >= kMedium)
	    m_Nbjet_ga[t]++;
	  m_index_jet_ga[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *Vb_PAIR[t]){
	  m_Njet_b[t]++;
	  if(Jets[i].ParticleID() >= kMedium)
	    m_Nbjet_b[t]++;
	  m_index_jet_b[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(jetID[i]) == *GVb_PAIR[t]){
	  m_Njet_gb[t]++;
	  if(Jets[i].ParticleID() >= kMedium)
	    m_Nbjet_gb[t]++;
	  m_index_jet_gb[t].push_back(i);
	} 
      }
      for(int i = 0; i < m_Nlep; i++){
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *Va_PAIR[t]){
	  m_Nlep_a[t]++;
	  m_index_lep_a[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *GVa_PAIR[t]){
	  m_Nlep_ga[t]++;
	  m_index_lep_ga[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *Vb_PAIR[t]){
	  m_Nlep_b[t]++;
	  m_index_lep_b[t].push_back(i);
	}
	if(COMB_PAIR[t]->GetFrame(lepID[i]) == *GVb_PAIR[t]){
	  m_Nlep_gb[t]++;
	  m_index_lep_gb[t].push_back(i);
	}
      }
    }   

    if(!is_filled[t])
      continue;
    
    // Fill Observable Branches
    
    m_MSS[t] = S_PAIR[t]->GetMass();
    m_PSS[t] = Ca_PAIR[t]->GetMomentum(*S_PAIR[t]);
    m_cosSS[t] = S_PAIR[t]->GetCosDecayAngle();
    m_dphiSS[t] = S_PAIR[t]->GetDeltaPhiDecayAngle();
    TLorentzVector vPSS = S_PAIR[t]->GetFourVector();
    m_PTSS[t] = vPSS.Pt();
    m_PzSS[t] = vPSS.Pz();

    m_MCa[t] = Ca_PAIR[t]->GetMass();
    m_cosCa[t] = Ca_PAIR[t]->GetCosDecayAngle();
    m_MCb[t] = Cb_PAIR[t]->GetMass();
    m_cosCb[t] = Cb_PAIR[t]->GetCosDecayAngle();

    m_MGCa[t] = GCa_PAIR[t]->GetMass();
    m_cosGCa[t] = GCa_PAIR[t]->GetCosDecayAngle();
    m_MGCb[t] = GCb_PAIR[t]->GetMass();
    m_cosGCb[t] = GCb_PAIR[t]->GetCosDecayAngle();

    TLorentzVector vP_Va_SS  = Va_PAIR[t]->GetFourVector(*S_PAIR[t]);
    TLorentzVector vP_Vb_SS  = Vb_PAIR[t]->GetFourVector(*S_PAIR[t]);
    TLorentzVector vP_GVa_SS = GVa_PAIR[t]->GetFourVector(*S_PAIR[t]);
    TLorentzVector vP_GVb_SS = GVb_PAIR[t]->GetFourVector(*S_PAIR[t]);
    TLorentzVector vP_Ia_SS  = Ia_PAIR[t]->GetFourVector(*S_PAIR[t]);
    TLorentzVector vP_Ib_SS  = Ib_PAIR[t]->GetFourVector(*S_PAIR[t]);
    
    m_H11SS[t] = 2.*(vP_Ia_SS+vP_Ib_SS).P();
    m_H21SS[t] = (vP_Va_SS+vP_GVa_SS).P() + (vP_Vb_SS+vP_GVb_SS).P() + m_H11SS[t]/2.;
    m_H22SS[t] = (vP_Va_SS+vP_GVa_SS).P() + (vP_Vb_SS+vP_GVb_SS).P() + vP_Ia_SS.P() + vP_Ia_SS.P();
    m_HT21SS[t] = m_H11SS[t]/2.
      + S_PAIR[t]->GetTransverseMomentum(Va_PAIR[t]->GetFourVector()+GVa_PAIR[t]->GetFourVector())
      + S_PAIR[t]->GetTransverseMomentum(Vb_PAIR[t]->GetFourVector()+GVb_PAIR[t]->GetFourVector());
    m_HT22SS[t] = Ia_PAIR[t]->GetTransverseMomentum(*S_PAIR[t]) + Ib_PAIR[t]->GetTransverseMomentum(*S_PAIR[t])
      + S_PAIR[t]->GetTransverseMomentum(Va_PAIR[t]->GetFourVector()+GVa_PAIR[t]->GetFourVector())
      + S_PAIR[t]->GetTransverseMomentum(Vb_PAIR[t]->GetFourVector()+GVb_PAIR[t]->GetFourVector());
    m_H42SS[t] = vP_Va_SS.P() + vP_GVa_SS.P() + vP_Vb_SS.P() + vP_GVb_SS.P() + vP_Ia_SS.P() + vP_Ia_SS.P();;
    m_HT42SS[t] = Ia_PAIR[t]->GetTransverseMomentum(*S_PAIR[t]) + Ib_PAIR[t]->GetTransverseMomentum(*S_PAIR[t])
      + Va_PAIR[t]->GetTransverseMomentum(*S_PAIR[t]) + Vb_PAIR[t]->GetTransverseMomentum(*S_PAIR[t])
      + GVa_PAIR[t]->GetTransverseMomentum(*S_PAIR[t]) + GVb_PAIR[t]->GetTransverseMomentum(*S_PAIR[t]);

    TLorentzVector vP_Va_Ca  = Va_PAIR[t]->GetFourVector(*Ca_PAIR[t]);
    TLorentzVector vP_Vb_Cb  = Vb_PAIR[t]->GetFourVector(*Cb_PAIR[t]);
    TLorentzVector vP_GVa_Ca = GVa_PAIR[t]->GetFourVector(*Ca_PAIR[t]);
    TLorentzVector vP_GVb_Cb = GVb_PAIR[t]->GetFourVector(*Cb_PAIR[t]);
    TLorentzVector vP_Ia_Ca  = Ia_PAIR[t]->GetFourVector(*Ca_PAIR[t]);
    TLorentzVector vP_Ib_Cb  = Ib_PAIR[t]->GetFourVector(*Cb_PAIR[t]);

    m_MVa[t] = (vP_Va_Ca+vP_GVa_Ca).M();
    m_PVa[t] = (vP_Va_Ca+vP_GVa_Ca).P();
    m_cosVa[t] = Ca_PAIR[t]->GetCosDecayAngle(*Ia_PAIR[t]);
    m_MVb[t] = (vP_Vb_Cb+vP_GVb_Cb).M();
    m_PVb[t] = (vP_Vb_Cb+vP_GVb_Cb).P();
    m_cosVb[t] = Cb_PAIR[t]->GetCosDecayAngle(*Ib_PAIR[t]);
    
    m_H11Ca[t] = 2.*vP_Ia_Ca.P();
    m_H11Cb[t] = 2.*vP_Ib_Cb.P();
    m_H21Ca[t] = vP_Va_Ca.P() + vP_GVa_Ca.P() + vP_Ia_Ca.P();
    m_H21Cb[t] = vP_Vb_Cb.P() + vP_GVb_Cb.P() + vP_Ib_Cb.P();
  }

  // ISR-style analysis
  if(m_Njet > 0){
    ParticleList JetsT = Jets;
    ParticleList LeptonsT = Leptons;
    
    for(int i = 0; i < m_Njet; i++)
      JetsT[i].SetPtEtaPhiM(JetsT[i].Pt(),0.,JetsT[i].Phi(),JetsT[i].M());
    
    for(int i = 0; i < m_Nlep; i++)
      LeptonsT[i].SetPtEtaPhiM(LeptonsT[i].Pt(),0.,LeptonsT[i].Phi(),LeptonsT[i].M());
    
    // loop through different trees
    for(int t = 0; t < 3; t++){
      LAB_ISR[t]->ClearEvent();
      
      INV_ISR[t]->SetLabFrameThreeVector(ETMiss);
      
      if(t == 0){ // jets in ISR, leptons in sparticle
	ISR_ISR[t]->SetLabFrameFourVector(JetsT);
	L_ISR[t]->SetLabFrameFourVector(LeptonsT);
	
	if(!LAB_ISR[t]->AnalyzeEvent())
	  cout << "Something went wrong with ISR tree event analysis " << t << endl;

	for(int i = 0; i < m_Njet; i++){
	  m_Njet_ISR[t]++;
	  // NOTE
	  if(Jets[i].ParticleID() >= kMedium)
	    m_Nbjet_ISR[t]++;
	  m_index_jet_ISR[t].push_back(i);
	}
	for(int i = 0; i < m_Nlep; i++){
	  m_Nlep_S[t]++;
	  m_index_lep_S[t].push_back(i);
	}
      }
      
      if(t == 1){ // jets in ISR+V, leptons in sparticle
	std::vector<RFKey> jetID;
	for(int i = 0; i < m_Njet; i++) 
	  jetID.push_back(COMB_ISR[t]->AddLabFrameFourVector(JetsT[i]));

	L_ISR[t]->SetLabFrameFourVector(LeptonsT);

	if(!LAB_ISR[t]->AnalyzeEvent())
	  cout << "Something went wrong with ISR tree event analysis " << t << endl;

	for(int i = 0; i < m_Njet; i++){
	  if(COMB_ISR[t]->GetFrame(jetID[i]) == *ISR_ISR[t]){
	    m_Njet_ISR[t]++;
	    if(Jets[i].ParticleID() >= kMedium)
	      m_Nbjet_ISR[t]++;
	    m_index_jet_ISR[t].push_back(i);
	  } else {
	    m_Njet_S[t]++;
	    if(Jets[i].ParticleID() >= kMedium)
	      m_Nbjet_S[t]++;
	    m_index_jet_S[t].push_back(i);
	  }
	}
	for(int i = 0; i < m_Nlep; i++){
	  m_Nlep_S[t]++;
	  m_index_lep_S[t].push_back(i);
	}
      }
      
      if(t == 2){ // everything combinatorically assigned
	std::vector<RFKey> jetID;
	for(int i = 0; i < m_Njet; i++) 
	  jetID.push_back(COMB_ISR[t]->AddLabFrameFourVector(JetsT[i]));
	std::vector<RFKey> lepID;
	for(int i = 0; i < m_Nlep; i++) 
	  lepID.push_back(COMB_ISR[t]->AddLabFrameFourVector(LeptonsT[i]));

	if(!LAB_ISR[t]->AnalyzeEvent())
	  cout << "Something went wrong with ISR tree event analysis " << t << endl;

	for(int i = 0; i < m_Njet; i++){
	  if(COMB_ISR[t]->GetFrame(jetID[i]) == *ISR_ISR[t]){
	    m_Njet_ISR[t]++;
	    if(Jets[i].ParticleID() >= kMedium)
	      m_Nbjet_ISR[t]++;
	    m_index_jet_ISR[t].push_back(i);
	  } else {
	    m_Njet_S[t]++;
	    if(Jets[i].ParticleID() >= kMedium)
	      m_Nbjet_S[t]++;
	    m_index_jet_S[t].push_back(i);
	  }
	}
	for(int i = 0; i < m_Nlep; i++){
	  if(COMB_ISR[t]->GetFrame(lepID[i]) == *ISR_ISR[t]){
	    m_Nlep_ISR[t]++;
	    m_index_lep_ISR[t].push_back(i);
	  } else {
	    m_Nlep_S[t]++;
	    m_index_lep_S[t].push_back(i);
	  }
	}
      }

      // Fill Observable Branches
      m_cosCM[t] = CM_ISR[t]->GetCosDecayAngle();
      m_cosS[t]  = S_ISR[t]->GetCosDecayAngle();
      m_MISR[t]  = ISR_ISR[t]->GetMass();
      m_MS[t]    = S_ISR[t]->GetMass();
      if(t != 0)
	m_MV[t] = V_ISR[t]->GetMass();
      if(t != 2)
	m_ML[t] = L_ISR[t]->GetMass();
      m_dphiCMI[t] = acos(-1.)-fabs(CM_ISR[t]->GetDeltaPhiBoostVisible());
      m_dphiSI[t]  = acos(-1.)-fabs(S_ISR[t]->GetDeltaPhiBoostVisible());
    
      m_PTCM[t]  = CM_ISR[t]->GetFourVector().Pt();
   
      TVector3 vPt_ISR = ISR_ISR[t]->GetFourVector(*CM_ISR[t]).Vect();
      TVector3 vPt_I   = I_ISR[t]->GetFourVector(*CM_ISR[t]).Vect();

      m_PTISR[t]    = vPt_ISR.Mag();
      m_RISR[t]     = m_Nlep_S[t]+m_Njet_S[t] > 0. ? -vPt_I.Dot(vPt_ISR.Unit())/ m_PTISR[t] : 0;
      m_dphiISRI[t] = fabs(vPt_ISR.Angle(vPt_I));
    }
  }
  
  
  /*
 

  if(m_Is_2LNJ){

  m_HN2S = Z_2LNJ->GetFourVector(*S_2LNJ).E() +
  J_2LNJ->GetFourVector(*S_2LNJ).E() +
  Ia_2LNJ->GetFourVector(*S_2LNJ).P() +
  Ib_2LNJ->GetFourVector(*S_2LNJ).P();
  m_H11S = 2.*(*Ia_2LNJ+*Ib_2LNJ).GetFourVector(*S_2LNJ).P();
  m_HN1Ca = Z_2LNJ->GetFourVector(*Ca_2LNJ).E()+
  Ia_2LNJ->GetFourVector(*Ca_2LNJ).P();
  m_HN1Cb = J_2LNJ->GetFourVector(*Cb_2LNJ).E()+
  Ib_2LNJ->GetFourVector(*Cb_2LNJ).P();
  m_H11Ca = 2.*Ia_2LNJ->GetFourVector(*Ca_2LNJ).P();
  m_H11Cb = 2.*Ib_2LNJ->GetFourVector(*Cb_2LNJ).P();
  m_cosC  = Ca_2LNJ->GetCosDecayAngle();
    
  m_MZ = Z_2LNJ->GetMass();
  m_MJ = J_2LNJ->GetMass();
  m_cosZ = Z_2LNJ->GetCosDecayAngle();
  if(Jets.size() > 1)
  m_cosJ = JSA_2LNJ->GetCosDecayAngle();
  }

  if(m_Is_2L1L){
    
  m_HN2S = Z_2L1L->GetFourVector(*S_2L1L).E() +
  Lb_2L1L->GetFourVector(*S_2L1L).E() +
  Ia_2L1L->GetFourVector(*S_2L1L).P() +
  Ib_2L1L->GetFourVector(*S_2L1L).P();
  m_H11S = 2.*(*Ia_2L1L+*Ib_2L1L).GetFourVector(*S_2L1L).P();
  m_HN1Ca = Z_2L1L->GetFourVector(*Ca_2L1L).E()+
  Ia_2L1L->GetFourVector(*Ca_2L1L).P();
  m_HN1Cb = Lb_2L1L->GetFourVector(*Cb_2L1L).E()+
  Ib_2L1L->GetFourVector(*Cb_2L1L).P();
  m_H11Ca = 2.*Ia_2L1L->GetFourVector(*Ca_2L1L).P();
  m_H11Cb = 2.*Ib_2L1L->GetFourVector(*Cb_2L1L).P();
  m_cosC  = Ca_2L1L->GetCosDecayAngle();
    
  m_MZ = Z_2L1L->GetMass();
  m_cosZ = Z_2L1L->GetCosDecayAngle();
  }

  */
  m_weight = AnalysisBase<Base>::GetEventWeight();
  
  m_MET     = ETMiss.Pt();
  m_MET_phi = ETMiss.Phi();

  TVector3 genETMiss = AnalysisBase<Base>::GetGenMET();
  m_genMET     = genETMiss.Pt();
  m_genMET_phi = genETMiss.Phi();

  // Fill Jets
  m_PT_jet.clear();
  m_Eta_jet.clear();
  m_Phi_jet.clear();
  m_M_jet.clear();
  m_Btag_jet.clear();
  for(int i = 0; i < m_Njet; i++){
    m_PT_jet.push_back(Jets[i].Pt());
    m_Eta_jet.push_back(Jets[i].Eta());
    m_Phi_jet.push_back(Jets[i].Phi());
    m_M_jet.push_back(Jets[i].M());
    m_Btag_jet.push_back(Jets[i].Btag());
  }

  ParticleList GenMuons = AnalysisBase<Base>::GetGenMuons();
  ParticleList GenElectrons = AnalysisBase<Base>::GetGenElectrons();
  ParticleList GenLeptons = GenElectrons+GenMuons;
  GenLeptons.SortByPt();

  m_genNele = GenElectrons.size();
  m_genNmu  = GenMuons.size();
  m_genNlep = GenLeptons.size();
  
  // Fill reconstructed lepton branches
  m_PT_lep.clear();
  m_Eta_lep.clear();
  m_Phi_lep.clear();
  m_M_lep.clear();
  m_Charge_lep.clear();
  m_PDGID_lep.clear();
  m_RelIso_lep.clear();
  m_MiniIso_lep.clear();
  m_ID_lep.clear();
  m_Index_lep.clear();
  vector<int> genmatch;
  for(int i = 0; i < m_genNlep; i++)
    genmatch.push_back(-1);
  for(int r = 0; r < m_Nlep; r++){
    m_PT_lep.push_back(Leptons[r].Pt());
    m_Eta_lep.push_back(Leptons[r].Eta());
    m_Phi_lep.push_back(Leptons[r].Phi());
    m_M_lep.push_back(Leptons[r].M());
    m_Charge_lep.push_back(Leptons[r].Charge());
    m_PDGID_lep.push_back(Leptons[r].PDGID());
    m_RelIso_lep.push_back(Leptons[r].RelIso());
    m_MiniIso_lep.push_back(Leptons[r].MiniIso());
    m_ID_lep.push_back(Leptons[r].ParticleID());
    int index = -1;
    for(int g = 0; g < m_genNlep; g++)
      if(Leptons[r].DeltaR(GenLeptons[g]) < 0.02){
	index = g;
	genmatch[g] = r;
	break;
      }
    m_Index_lep.push_back(index);
  }
  
  // Fill gen lepton branches
  m_genPT_lep.clear();
  m_genEta_lep.clear();
  m_genPhi_lep.clear();
  m_genM_lep.clear();
  m_genCharge_lep.clear();
  m_genPDGID_lep.clear();
  m_genIndex_lep.clear();
  for(int g = 0; g < m_genNlep; g++){
    m_genPT_lep.push_back(GenLeptons[g].Pt());
    m_genEta_lep.push_back(GenLeptons[g].Eta());
    m_genPhi_lep.push_back(GenLeptons[g].Phi());
    m_genM_lep.push_back(GenLeptons[g].M());
    m_genCharge_lep.push_back(GenLeptons[g].Charge());
    m_genPDGID_lep.push_back(GenLeptons[g].PDGID());
    m_genIndex_lep.push_back(genmatch[g]);
  }
  
  // Fill gen neutrino branches
  ParticleList GenNus = AnalysisBase<Base>::GetGenNeutrinos();
  m_genNnu = GenNus.size();
  m_genPT_nu.clear();
  m_genEta_nu.clear();
  m_genPhi_nu.clear();
  m_genPDGID_nu.clear();
  for(int i = 0; i < m_genNnu; i++){
    m_genPT_nu.push_back(GenNus[i].Pt());
    m_genEta_nu.push_back(GenNus[i].Eta());
    m_genPhi_nu.push_back(GenNus[i].Phi());
    m_genPDGID_nu.push_back(GenNus[i].PDGID());
  }
  
  // Fill gen boson branches
  ParticleList GenBosons = AnalysisBase<Base>::GetGenBosons();
  m_genNboson = GenBosons.size();
  m_genPT_boson.clear();
  m_genEta_boson.clear();
  m_genPhi_boson.clear();
  m_genM_boson.clear();
  m_genPDGID_boson.clear();
  for(int i = 0; i < m_genNboson; i++){
    m_genPT_boson.push_back(GenBosons[i].Pt());
    m_genEta_boson.push_back(GenBosons[i].Eta());
    m_genPhi_boson.push_back(GenBosons[i].Phi());
    m_genM_boson.push_back(GenBosons[i].Phi());
    m_genPDGID_boson.push_back(GenBosons[i].PDGID());
  }

  // Fill gen sparticle branches
  ParticleList GenSparticles = AnalysisBase<Base>::GetGenSparticles();
  m_genNsusy = GenSparticles.size();
  m_genPT_susy.clear();
  m_genEta_susy.clear();
  m_genPhi_susy.clear();
  m_genM_susy.clear();
  m_genPDGID_susy.clear();
  for(int i = 0; i < m_genNsusy; i++){
    m_genPT_susy.push_back(GenSparticles[i].Pt());
    m_genEta_susy.push_back(GenSparticles[i].Eta());
    m_genPhi_susy.push_back(GenSparticles[i].Phi());
    m_genM_susy.push_back(GenSparticles[i].Phi());
    m_genPDGID_susy.push_back(GenSparticles[i].PDGID());
  }

  // Fill output tree
  if(tree)
    tree->Fill();
  
}

template class ReducedNtuple<StopNtupleTree>;
