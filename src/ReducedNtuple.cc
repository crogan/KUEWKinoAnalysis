#include "ReducedNtuple.hh"
#include "ParticleList.hh"
#include "TInterpreter.h"

#include "StopNtupleTree.hh"
#include "SUSYNANOBase.hh"

using namespace RestFrames;

template <class Base>
ReducedNtuple<Base>::ReducedNtuple(TTree* tree)
  : NtupleBase<Base>(tree)
{
  m_library_generated = false;

  
  // RestFrame Decay Trees
  for(int t = 0; t < 2; t++){
    LAB[t] = new LabRecoFrame(Form("LAB_%d",t),"LAB");
    CM[t]  = new DecayRecoFrame(Form("CM_%d",t),"CM");
    S[t]   = new DecayRecoFrame(Form("S_%d",t),"#tilde{S}");
    X3a[t] = new DecayRecoFrame(Form("X3a_%d",t),"#tilde{#chi}_{3a}");
    X3b[t] = new DecayRecoFrame(Form("X3b_%d",t),"#tilde{#chi}_{3b}");
    X2a[t] = new DecayRecoFrame(Form("X2a_%d",t),"#tilde{#chi}_{2a}");
    X2b[t] = new DecayRecoFrame(Form("X2b_%d",t),"#tilde{#chi}_{2b}");
    saJa[t]  = new SelfAssemblingRecoFrame(Form("saJa_%d",t),"jets_{a}");
    saJb[t]  = new SelfAssemblingRecoFrame(Form("saJb_%d",t),"jets_{b}");
    saLa[t]  = new SelfAssemblingRecoFrame(Form("saLa_%d",t),"leps_{a}");
    saLb[t]  = new SelfAssemblingRecoFrame(Form("saLb_%d",t),"leps_{b}");

    ISR[t] = new VisibleRecoFrame(Form("ISR_%d",t),"ISR");
    Ja[t] = new VisibleRecoFrame(Form("Ja_%d",t),"jets_{a}");
    Jb[t] = new VisibleRecoFrame(Form("Jb_%d",t),"jets_{b}");
    La[t] = new VisibleRecoFrame(Form("La_%d",t),"leps_{a}");
    Lb[t] = new VisibleRecoFrame(Form("Lb_%d",t),"leps_{b}");
    X1a[t] = new InvisibleRecoFrame(Form("X1a_%d",t),"#tilde{#chi}_{1a}");
    X1b[t] = new InvisibleRecoFrame(Form("X1b_%d",t),"#tilde{#chi}_{1b}");

    LAB[t]->SetChildFrame(*CM[t]);
    CM[t]->AddChildFrame(*S[t]);
    CM[t]->AddChildFrame(*ISR[t]);
    S[t]->AddChildFrame(*X3a[t]);
    S[t]->AddChildFrame(*X3b[t]);
    X3a[t]->AddChildFrame(*X2a[t]);
    X3b[t]->AddChildFrame(*X2b[t]);
    X3a[t]->AddChildFrame(*saJa[t]);
    X3b[t]->AddChildFrame(*saJb[t]);
    //X3a[t]->AddChildFrame(*Ja[t]);
    //X3b[t]->AddChildFrame(*Jb[t]);
    X2a[t]->AddChildFrame(*X1a[t]);
    X2b[t]->AddChildFrame(*X1b[t]);
    X2a[t]->AddChildFrame(*saLa[t]);
    X2b[t]->AddChildFrame(*saLb[t]);
    //X2a[t]->AddChildFrame(*La[t]);
    //X2b[t]->AddChildFrame(*Lb[t]);
    saJa[t]->AddChildFrame(*Ja[t]);
    saJb[t]->AddChildFrame(*Jb[t]);
    saLa[t]->AddChildFrame(*La[t]);
    saLb[t]->AddChildFrame(*Lb[t]);
    
    if(!LAB[t]->InitializeTree()){
      cout << "Problem initializing tree #" << t << endl;
    }
    
    INV[t] = new InvisibleGroup(Form("INV_%d",t),"Invisible System");
    INV[t]->AddFrame(*X1a[t]);
    INV[t]->AddFrame(*X1b[t]);
    
    InvM[t] = new SetMassInvJigsaw(Form("InvM_%d",t), "Set inv. system mass");
    INV[t]->AddJigsaw(*InvM[t]);

    InvEta[t] = new SetRapidityInvJigsaw(Form("InvEta_%d",t), "Set inv. system rapidity");
    INV[t]->AddJigsaw(*InvEta[t]);
    InvEta[t]->AddVisibleFrames(S[t]->GetListVisibleFrames());
    
    InvSplit[t] = new MinMassesSqInvJigsaw(Form("InvSplit%d",t), "INV -> #tilde{#chi_{1a}}+ #tilde{#chi_{1b}}", 2);
    INV[t]->AddJigsaw(*InvSplit[t]);
    InvSplit[t]->AddVisibleFrame(*Ja[t], 0);
    InvSplit[t]->AddVisibleFrame(*Jb[t], 1);
    InvSplit[t]->AddVisibleFrame(*La[t], 0);
    InvSplit[t]->AddVisibleFrame(*Lb[t], 1);
    // InvSplit[t]->AddVisibleFrames(X3a[t]->GetListVisibleFrames(), 0);
    // InvSplit[t]->AddVisibleFrames(X3b[t]->GetListVisibleFrames(), 1);
    InvSplit[t]->AddInvisibleFrame(*X1a[t], 0);
    InvSplit[t]->AddInvisibleFrame(*X1b[t], 1);

    COMB_J[t] =  new CombinatoricGroup(Form("COMB_J_%d",t), "Combinatoric System of Jets");
    CombSplit_ISR[t] = new MinMassesCombJigsaw(Form("CombSplit_ISR_%d",t), "Minimize M_{T}^{ISR} and M_{T}^{S}");
    CombSplit_J[t] = new MinMassesSqCombJigsaw(Form("CombSplit_J_%d",t), "Minimize M_{Va}^{2} + M_{Vb}^{2} ",2,2);
   
    COMB_L[t] =  new CombinatoricGroup(Form("COMB_L_%d",t), "Combinatoric System of Leps");
    CombSplit_L[t] = new MinMassesSqCombJigsaw(Form("CombSplit_L_%d",t), "Minimize M_{Va}^{2} + M_{Vb}^{2}",2,2);
   
    if(t > 0){
      COMB_J[t]->AddFrame(*ISR[t]);
      COMB_J[t]->SetNElementsForFrame(*ISR[t], 1);
      COMB_J[t]->AddFrame(*Ja[t]);
      COMB_J[t]->SetNElementsForFrame(*Ja[t], 0);
      COMB_J[t]->AddFrame(*Jb[t]);
      COMB_J[t]->SetNElementsForFrame(*Jb[t], 0);

      COMB_J[t]->AddJigsaw(*CombSplit_ISR[t]);
      CombSplit_ISR[t]->SetTransverse();
      CombSplit_ISR[t]->AddCombFrame(*ISR[t], 0);
      CombSplit_ISR[t]->AddCombFrame(*Ja[t], 1);
      CombSplit_ISR[t]->AddCombFrame(*Jb[t], 1);
      CombSplit_ISR[t]->AddObjectFrame(*ISR[t], 0);
      CombSplit_ISR[t]->AddObjectFrame(*S[t], 1);

      COMB_J[t]->AddJigsaw(*CombSplit_J[t]);
      CombSplit_J[t]->AddCombFrame(*Ja[t], 0);
      CombSplit_J[t]->AddCombFrame(*Jb[t], 1);
      CombSplit_J[t]->AddObjectFrames(X3a[t]->GetListVisibleFrames(), 0);
      CombSplit_J[t]->AddObjectFrames(X3b[t]->GetListVisibleFrames(), 1);

      COMB_L[t]->AddFrame(*La[t]);
      COMB_L[t]->SetNElementsForFrame(*La[t], 1);
      COMB_L[t]->AddFrame(*Lb[t]);
      COMB_L[t]->SetNElementsForFrame(*Lb[t], 0);

      COMB_L[t]->AddJigsaw(*CombSplit_L[t]);
      CombSplit_L[t]->AddCombFrame(*La[t], 0);
      CombSplit_L[t]->AddCombFrame(*Lb[t], 1);
      CombSplit_L[t]->AddObjectFrames(X3a[t]->GetListVisibleFrames(), 0);
      CombSplit_L[t]->AddObjectFrames(X3b[t]->GetListVisibleFrames(), 1);
    }

    if(!LAB[t]->InitializeAnalysis())
	cout << "Problem initializing analysis tree #" << t << endl;
  }

  /*
  TreePlot tree_plot("TreePlot","TreePlot");

  for(int t = 0; t < 2; t++){
    tree_plot.SetTree(*LAB[t]);
    tree_plot.Draw(Form("ANA_tree_%d",t), Form("Reconstruction Tree %d",t));

    tree_plot.SetTree(*COMB_J[t]);
    tree_plot.Draw(Form("ANA_comb_J_%d",t), Form("Combinatoric Jigsaws for jets %d",t),1);

    tree_plot.SetTree(*COMB_L[t]);
    tree_plot.Draw(Form("ANA_comb_L_%d",t), Form("Combinatoric Jigsaws for leps %d",t),1);

    tree_plot.SetTree(*INV[t]);
    tree_plot.Draw(Form("ANA_inv_%d",t), Form("Invisible Jigsaws %d",t),1);
 
  }
  tree_plot.WriteOutput("trees.root");
  */

   // Calculated Observables
  for(int i = 0; i < 2; i++){
    m_Njet_ISR.push_back(0);
    m_Njet_S.push_back(0);
    m_Nbjet_ISR.push_back(0);
    m_Nbjet_S.push_back(0);
    m_Nlep_ISR.push_back(0);
    m_Nlep_S.push_back(0);
    m_NSV_ISR.push_back(0);
    m_NSV_S.push_back(0);

    m_Njet_a.push_back(0);
    m_Njet_b.push_back(0);
    m_Nbjet_a.push_back(0);
    m_Nbjet_b.push_back(0);
    m_Nlep_a.push_back(0);
    m_Nlep_b.push_back(0);
    m_NSV_a.push_back(0);
    m_NSV_b.push_back(0);

    m_index_jet_ISR.push_back(vector<int>());
    m_index_jet_S.push_back(vector<int>());
    m_index_SV_ISR.push_back(vector<int>());
    m_index_SV_S.push_back(vector<int>());
    m_index_lep_ISR.push_back(vector<int>());
    m_index_lep_S.push_back(vector<int>());
    m_dphi_lep_S.push_back(vector<double>());
    m_cos_lep_S.push_back(vector<double>());
    
    m_index_jet_a.push_back(vector<int>());
    m_index_jet_b.push_back(vector<int>());
    m_index_lep_a.push_back(vector<int>());
    m_index_lep_b.push_back(vector<int>());
    m_index_SV_a.push_back(vector<int>());
    m_index_SV_b.push_back(vector<int>());

    m_PTCM.push_back(0);
    m_dphiCM.push_back(0);
    m_cosCM.push_back(0);
    m_dphiCMI.push_back(0);
    
    m_MS.push_back(0);
    m_PS.push_back(0);
    m_cosS.push_back(0);
    m_dphiS.push_back(0);
    m_dphiSI.push_back(0);
    m_PTS.push_back(0);
    m_PzS.push_back(0);
    
    m_MX3a.push_back(0);
    m_cosX3a.push_back(0);
    m_MX3b.push_back(0);
    m_cosX3b.push_back(0);
    m_EVa.push_back(0);
    m_EVb.push_back(0);
    m_PVa.push_back(0);
    m_PVb.push_back(0);
    m_EJa.push_back(0);
    m_EJb.push_back(0);
    m_PJa.push_back(0);
    m_PJb.push_back(0);
    
    m_MX2a.push_back(0);
    m_cosX2a.push_back(0);
    m_MX2b.push_back(0);
    m_cosX2b.push_back(0);
    m_ELa.push_back(0);
    m_ELb.push_back(0);
    m_PLa.push_back(0);
    m_PLb.push_back(0);
    
    m_MV.push_back(0);
    m_PV.push_back(0);
    m_MVa.push_back(0);
    m_MVb.push_back(0);

    m_MJa.push_back(0);
    m_MJb.push_back(0);
    m_MLa.push_back(0);
    m_MLb.push_back(0);
    m_cosJa.push_back(0);
    m_cosJb.push_back(0);
    m_cosLa.push_back(0);
    m_cosLb.push_back(0);
    
    m_H11S.push_back(0);
    m_H21S.push_back(0);
    m_HT21S.push_back(0);
    m_H22S.push_back(0);
    m_HT22S.push_back(0);
    m_H42S.push_back(0);
    m_HT42S.push_back(0);
    m_H11X3a.push_back(0);
    m_H11X3b.push_back(0);
    m_H21X3a.push_back(0);
    m_H21X3b.push_back(0);

    m_PTISR.push_back(0);
    m_RISR.push_back(0);
    m_MISR.push_back(0);
  }
  
}

template <class Base>
ReducedNtuple<Base>::~ReducedNtuple() {
  for(int i = 0; i < 2; i++){
    delete LAB[i];
    delete CM[i];
    delete S[i];
    delete X3a[i];
    delete X3b[i];
    delete X2a[i];
    delete X2b[i];
    delete saJa[i];
    delete saJb[i];
    delete saLa[i];
    delete saLb[i];
    delete ISR[i];
    delete Ja[i];
    delete Jb[i];
    delete La[i];
    delete Lb[i];
    delete X1a[i];
    delete X1b[i];
    
    delete INV[i];
    delete InvM[i];
    delete InvEta[i];
    delete InvSplit[i];
    
    delete COMB_J[i];
    delete CombSplit_ISR[i];
    delete CombSplit_J[i];
    
    delete COMB_L[i];
    delete CombSplit_L[i];
  }
  
}

template <class Base>
TTree* ReducedNtuple<Base>::InitOutputTree(const string& sample){

  // gInterpreter->GenerateDictionary("vectorr<int>", "vector");
  // gInterpreter->GenerateDictionary("vector<double>", "vector");
  if(!m_library_generated){
    gInterpreter->GenerateDictionary("std::vector<std::vector<int> >", "vector");
    gInterpreter->GenerateDictionary("std::vector<std::vector<double> >", "vector");
    m_library_generated = true;
  }

  TTree* tree = (TTree*) new TTree(sample.c_str(), sample.c_str());

  tree->Branch("event_skipped", &m_event_skipped);
  
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
  
  tree->Branch("Dxy_lep", &m_Dxy_lep);
  tree->Branch("DxyErr_lep", &m_DxyErr_lep);
  tree->Branch("Dz_lep", &m_Dz_lep);
  tree->Branch("DzErr_lep", &m_DzErr_lep);
  tree->Branch("IP3D_lep", &m_IP3D_lep);
  tree->Branch("SIP3D_lep", &m_SIP3D_lep);
  tree->Branch("ID_lep",      &m_ID_lep);
  tree->Branch("Index_lep",   &m_Index_lep);
  
  tree->Branch("Njet", &m_Njet);
  tree->Branch("Nbjet", &m_Nbjet);
  tree->Branch("PT_jet",  &m_PT_jet);
  tree->Branch("Eta_jet", &m_Eta_jet);
  tree->Branch("Phi_jet", &m_Phi_jet);
  tree->Branch("M_jet",   &m_M_jet);
  tree->Branch("Btag_jet",   &m_Btag_jet);
  tree->Branch("BtagID_jet",   &m_BtagID_jet);
  tree->Branch("Flavor_jet",   &m_Flavor_jet);

  tree->Branch("NSV", &m_NSV);
  tree->Branch("PT_SV",  &m_PT_SV);
  tree->Branch("Eta_SV", &m_Eta_SV);
  tree->Branch("Phi_SV", &m_Phi_SV);
  tree->Branch("M_SV",   &m_M_SV);
 
  tree->Branch("genNele", &m_genNele);
  tree->Branch("genNmu", &m_genNmu);

  tree->Branch("genNlep", &m_genNlep);
  tree->Branch("genPT_lep",  &m_genPT_lep);
  tree->Branch("genEta_lep", &m_genEta_lep);
  tree->Branch("genPhi_lep", &m_genPhi_lep);
  tree->Branch("genM_lep",   &m_genM_lep);
  tree->Branch("genCharge_lep",  &m_genCharge_lep);
  tree->Branch("genPDGID_lep",   &m_genPDGID_lep);
  tree->Branch("genMomPDGID_lep",   &m_genMomPDGID_lep);
  tree->Branch("genIndex_lep",   &m_genIndex_lep);

  tree->Branch("genNnu", &m_genNnu);
  tree->Branch("genPT_nu",  &m_genPT_nu);
  tree->Branch("genEta_nu", &m_genEta_nu);
  tree->Branch("genPhi_nu", &m_genPhi_nu);
  tree->Branch("genPDGID_nu",   &m_genPDGID_nu);
  tree->Branch("genMomPDGID_nu",   &m_genMomPDGID_nu);

  tree->Branch("genNboson", &m_genNboson);
  tree->Branch("genPT_boson",  &m_genPT_boson);
  tree->Branch("genEta_boson", &m_genEta_boson);
  tree->Branch("genPhi_boson", &m_genPhi_boson);
  tree->Branch("genM_boson",   &m_genM_boson);
  tree->Branch("genPDGID_boson",   &m_genPDGID_boson);
  tree->Branch("genMomPDGID_boson",   &m_genMomPDGID_boson);

  tree->Branch("genNsusy", &m_genNsusy);
  tree->Branch("genPT_susy",  &m_genPT_susy);
  tree->Branch("genEta_susy", &m_genEta_susy);
  tree->Branch("genPhi_susy", &m_genPhi_susy);
  tree->Branch("genM_susy",   &m_genM_susy);
  tree->Branch("genPDGID_susy", &m_genPDGID_susy);
  tree->Branch("genMomPDGID_susy", &m_genMomPDGID_susy);

  tree->Branch("Njet_ISR", &m_Njet_ISR);
  tree->Branch("Njet_S", &m_Njet_S);
  tree->Branch("Nbjet_ISR", &m_Nbjet_ISR);
  tree->Branch("Nbjet_S", &m_Nbjet_S);
  tree->Branch("Nlep_ISR", &m_Nlep_ISR);
  tree->Branch("Nlep_S", &m_Nlep_S);
  tree->Branch("NSV_ISR", &m_NSV_ISR);
  tree->Branch("NSV_S", &m_NSV_S);
  tree->Branch("index_jet_ISR", &m_index_jet_ISR);
  tree->Branch("index_jet_S", &m_index_jet_S);
  tree->Branch("index_SV_ISR", &m_index_SV_ISR);
  tree->Branch("index_SV_S", &m_index_SV_S);
  tree->Branch("index_lep_ISR", &m_index_lep_ISR);
  tree->Branch("index_lep_S", &m_index_lep_S);
  tree->Branch("dphi_lep_S", &m_dphi_lep_S);
  tree->Branch("cos_lep_S", &m_cos_lep_S);
  
  tree->Branch("Njet_a", &m_Njet_a);
  tree->Branch("Njet_b", &m_Njet_b);
  tree->Branch("Nbjet_a", &m_Nbjet_a);
  tree->Branch("Nbjet_b", &m_Nbjet_b);
  tree->Branch("Nlep_a", &m_Nlep_a);
  tree->Branch("Nlep_b", &m_Nlep_b);
  tree->Branch("NSV_a", &m_NSV_a);
  tree->Branch("NSV_b", &m_NSV_b);
  
  tree->Branch("index_jet_a", &m_index_jet_a);
  tree->Branch("index_jet_b", &m_index_jet_b);
  tree->Branch("index_lep_a", &m_index_lep_a);
  tree->Branch("index_lep_b", &m_index_lep_b);
  tree->Branch("index_SV_a", &m_index_SV_a);
  tree->Branch("index_SV_b", &m_index_SV_b);

  tree->Branch("PTCM", &m_PTCM);
  tree->Branch("cosCM", &m_cosCM);
  tree->Branch("dphiCM", &m_dphiCM);
  tree->Branch("dphiCMI", &m_dphiCMI);
  
  tree->Branch("MS", &m_MS);
  tree->Branch("PS", &m_PS);
  tree->Branch("cosS", &m_cosS);
  tree->Branch("dphiS", &m_dphiS);
  tree->Branch("dphiSI", &m_dphiSI);
  tree->Branch("PTS", &m_PTS);
  tree->Branch("PzS", &m_PzS);

  tree->Branch("MX3a", &m_MX3a);
  tree->Branch("cosX3a", &m_cosX3a);
  tree->Branch("MX3b", &m_MX3b);
  tree->Branch("cosX3b", &m_cosX3b);
  tree->Branch("EVa", &m_EVa);
  tree->Branch("EVb", &m_EVb);
  tree->Branch("PVa", &m_PVa);
  tree->Branch("PVb", &m_PVb);
  tree->Branch("EJa", &m_EJa);
  tree->Branch("EJb", &m_EJb);
  tree->Branch("PJa", &m_PJa);
  tree->Branch("PJb", &m_PJb);

  tree->Branch("MX2a", &m_MX2a);
  tree->Branch("cosX2a", &m_cosX2a);
  tree->Branch("MX2b", &m_MX2b);
  tree->Branch("cosX2b", &m_cosX2b);
  tree->Branch("ELa", &m_ELa);
  tree->Branch("ELb", &m_ELb);
  tree->Branch("PLa", &m_PLa);
  tree->Branch("PLb", &m_PLb);

  tree->Branch("MV", &m_MV);
  tree->Branch("PV", &m_PV);
  tree->Branch("MVa", &m_MVa);
  tree->Branch("MVb", &m_MVb);

  tree->Branch("MJa", &m_MJa);
  tree->Branch("MJb", &m_MJb);
  tree->Branch("MLa", &m_MLa);
  tree->Branch("MLb", &m_MLb);
  tree->Branch("cosJa", &m_cosJa);
  tree->Branch("cosJb", &m_cosJb);
  tree->Branch("cosLa", &m_cosLa);
  tree->Branch("cosLb", &m_cosLb);
  
  tree->Branch("H11S", &m_H11S);
  tree->Branch("H21S", &m_H21S);
  tree->Branch("HT21S", &m_HT21S);
  tree->Branch("H22S", &m_H22S);
  tree->Branch("HT22S", &m_HT22S);
  tree->Branch("H42S", &m_H42S);
  tree->Branch("HT42S", &m_HT42S);
  tree->Branch("H11X3a", &m_H11X3a);
  tree->Branch("H11X3b", &m_H11X3b);
  tree->Branch("H21X3a", &m_H21X3a);
  tree->Branch("H21X3b", &m_H21X3b);

  tree->Branch("PTISR", &m_PTISR);
  tree->Branch("RISR", &m_RISR);
  tree->Branch("MISR", &m_MISR);
  
  tree->Branch("Is_1L", &m_Is_1L);
  tree->Branch("Is_2L", &m_Is_2L);
  tree->Branch("Is_3L", &m_Is_3L);
  tree->Branch("Is_4L", &m_Is_4L);
  
  
  return tree;
}

template <class Base>
void ReducedNtuple<Base>::ClearVariables(){

  m_event_skipped = false;
  
  for(int i = 0; i < 2; i++){
    m_Njet_ISR[i] = 0;
    m_Njet_S[i] = 0;
    m_Nbjet_ISR[i] = 0;
    m_Nbjet_S[i] = 0;
    m_Nlep_ISR[i] = 0;
    m_Nlep_S[i] = 0;
    m_NSV_ISR[i] = 0;
    m_NSV_S[i] = 0;
    m_index_jet_ISR[i].clear();
    m_index_jet_S[i].clear();
    m_index_SV_ISR[i].clear();
    m_index_SV_S[i].clear();
    m_index_lep_ISR[i].clear();
    m_index_lep_S[i].clear();
    m_dphi_lep_S[i].clear();
    m_cos_lep_S[i].clear();
    
    m_Njet_a[i] = 0;
    m_Njet_b[i] = 0;
    m_Nbjet_a[i] = 0;
    m_Nbjet_b[i] = 0;
    m_Nlep_a[i] = 0;
    m_Nlep_b[i] = 0;
    m_NSV_a[i] = 0;
    m_NSV_b[i] = 0;
   
    m_index_jet_a[i].clear();
    m_index_jet_b[i].clear();
    m_index_lep_a[i].clear();
    m_index_lep_b[i].clear();
    m_index_SV_a[i].clear();
    m_index_SV_b[i].clear();

    m_PTCM[i] = 0.;
    m_cosCM[i] = 0.;
    m_dphiCM[i] = 0.;
    m_dphiCMI[i] = 0.;
  
    m_MS[i] = 0.;
    m_PS[i] = 0.;
    m_cosS[i] = 0.;
    m_dphiS[i] = 0.;
    m_dphiSI[i] = 0.;
    m_PTS[i] = 0.;
    m_PzS[i] = 0.;

    m_MX3a[i] = 0.;
    m_cosX3a[i] = 0.;
    m_MX3b[i] = 0.;
    m_cosX3b[i] = 0.;
    m_EVa[i] = 0.;
    m_EVb[i] = 0.;
    m_PVa[i] = 0.;
    m_PVb[i] = 0.;
    m_EJa[i] = 0.;
    m_EJb[i] = 0.;
    m_PJa[i] = 0.;
    m_PJb[i] = 0.;

    m_MX2a[i] = 0.;
    m_cosX2a[i] = 0.;
    m_MX2b[i] = 0.;
    m_cosX2b[i] = 0.;
    m_ELa[i] = 0.;
    m_ELb[i] = 0.;
    m_PLa[i] = 0.;
    m_PLb[i] = 0.;

    m_MV[i] = 0.;
    m_PV[i] = 0.;
    m_MVa[i] = 0.;
    m_MVb[i] = 0.;

    m_MJa[i] = 0.;
    m_MJb[i] = 0.;
    m_MLa[i] = 0.;
    m_MLb[i] = 0.;
    m_cosJa[i] = 0.;
    m_cosJb[i] = 0.;
    m_cosLa[i] = 0.;
    m_cosLb[i] = 0.;

    m_H11S[i] = 0.;
    m_H21S[i] = 0.;
    m_HT21S[i] = 0.;
    m_H22S[i] = 0.;
    m_HT22S[i] = 0.;
    m_H42S[i] = 0.;
    m_HT42S[i] = 0.;
  
    m_H11X3a[i] = 0.;
    m_H11X3b[i] = 0.;
    m_H21X3a[i] = 0.;
    m_H21X3b[i] = 0.;

    // ISR related variables
    m_PTISR[i] = 0.;
    m_MISR[i] = 0.;
    m_RISR[i] = 0.;
  }

  m_Is_1L = false;
  m_Is_2L = false;
  m_Is_3L = false;
  m_Is_4L = false;
 
}

template <class Base>
void ReducedNtuple<Base>::FillOutputTree(TTree* tree){

  bool good_PV;
  TVector3 PV = AnalysisBase<Base>::GetPV(good_PV);

  if(!good_PV)
    return;
  
  TVector3 ETMiss = AnalysisBase<Base>::GetMET();

  if(ETMiss.Mag() < 100.)
    return;

  ClearVariables();

  ParticleList Muons = AnalysisBase<Base>::GetMuons();
  Muons = Muons.ParticleIDCut(kVeryLoose);
  Muons = Muons.PtEtaCut(3.5);

  ParticleList Electrons = AnalysisBase<Base>::GetElectrons();
  Electrons = Electrons.ParticleIDCut(kVeryLoose);
  Electrons = Electrons.PtEtaCut(5.0);
  
  ParticleList Leptons = Electrons+Muons;
  Leptons.SortByPt();
  
  ParticleList Jets = AnalysisBase<Base>::GetJets();
  Jets = Jets.PtEtaCut(20., 2.4);

  ParticleList SVs = AnalysisBase<Base>::GetSVs(PV);
  SVs = SVs.RemoveOverlap(Leptons, 0.4);
  SVs = SVs.RemoveOverlap(Jets, 0.4);
  
  Jets = Jets.RemoveOverlap(Leptons, 0.2);

  // skip event reconstruction for now if too many jets
  // if(Jets.size() >= 18){
  //   m_event_skipped = true;
  //   if(tree)
  //     tree->Fill();
  //   return;
  // }

  m_Njet = Jets.size();
  
  ParticleList BJets;
  vector<int> BJets_index;
  for(int i = 0; i < m_Njet; i++){
    if(Jets[i].BtagID() >= kMedium){
      BJets += Jets[i];
      BJets_index.push_back(i);
    }
  }
  m_Nbjet = BJets.size();

  m_Nele = Electrons.size();
  m_Nmu  = Muons.size();
  m_Nlep = Leptons.size();

  m_NSV = SVs.size();
  
  // require at least one lepton for now
  if(m_Nlep < 1)
    return;
  
  // not enough stuff
  if(m_Nlep + m_Njet < 2)
    return;
  
  bool is_filled[2];
  for(int i = 0; i < 2; i++)
    is_filled[i] = true;
  
  // Sparticle pair-production trees analysis
  for(int t = 0; t < 2; t++){
    
    LAB[t]->ClearEvent();

    INV[t]->SetLabFrameThreeVector(ETMiss);
    
    if(t == 0){ // fixed content hemispheres
      is_filled[t] = false;

      TLorentzVector vISR;
      for(int i = 0; i < m_Njet; i++){
	vISR += Jets[i]; 
	m_Njet_ISR[t]++;
	if(Jets[i].BtagID() >= kMedium)
	  m_Nbjet_ISR[t]++;
	m_index_jet_ISR[t].push_back(i);
      }
      for(int i = 0; i < m_NSV; i++){
	vISR += SVs[i]; 
	m_NSV_ISR[t]++;
	m_index_SV_ISR[t].push_back(i);
      }
      
      ISR[t]->SetLabFrameFourVector(vISR);

      for(int l = 0; l < m_Nlep; l++){
	m_Nlep_S[t]++;
	m_index_lep_S[t].push_back(l);
      }
      
      // 1 leptons
      if(Leptons.size() == 1){
	m_Is_1L = true;
	is_filled[t] = true;

	m_Nlep_a[t]++;
	
	La[t]->SetLabFrameFourVector(Leptons[0]);
	
	m_index_lep_a[t].push_back(0);
      
	// 2 leptons
      } else if(Leptons.size() == 2){
	m_Is_2L = true;
	is_filled[t] = true;

	m_Nlep_a[t]++;
	m_Nlep_b[t]++;
	
	La[t]->SetLabFrameFourVector(Leptons[0]);
	Lb[t]->SetLabFrameFourVector(Leptons[1]);
	
	m_index_lep_a[t].push_back(0);
	m_index_lep_b[t].push_back(1);
	
	// 3 leptons
      } else if(Leptons.size() == 3){
	m_Is_3L = true;
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

	TLorentzVector vLa, vLb;
	// no OSSF pair
	if(M_OSSF < 0.){
	  for(int i = 0; i < m_Nlep; i++){
	    if(i == minM.first){
	      vLa += Leptons[i];
	      m_index_lep_a[t].push_back(i);
	    } else if(i == minM.second){
	      vLa += Leptons[i];
	      m_index_lep_a[t].push_back(i);
	    } else {
	      vLb += Leptons[i];
	      m_index_lep_b[t].push_back(i);
	    }
	  }
	  // OSSF pair
	} else {
	  for(int i = 0; i < m_Nlep; i++){
	    if(i == OSSF.first){
	      vLa += Leptons[i];
	      m_index_lep_a[t].push_back(i);
	    } else if(i == OSSF.second){
	      vLa += Leptons[i];
	      m_index_lep_a[t].push_back(i);
	    } else {
	      vLb += Leptons[i];
	      m_index_lep_b[t].push_back(i);
	    }
	  }
	}

	La[t]->SetLabFrameFourVector(vLa);
	Lb[t]->SetLabFrameFourVector(vLb);

	m_Nlep_a[t] = 2;
	m_Nlep_b[t]++;
     
	// 4 leptons
      } else if(Leptons.size() == 4){
	m_Is_4L = true;
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

	TLorentzVector vLa, vLb;
	for(int i = 0; i < m_Nlep; i++){
	  if(i == OSSF[0]){
	    vLa += Leptons[i];
	    m_index_lep_a[t].push_back(i);
	  } else if(i == OSSF[1]){
	    vLa += Leptons[i];
	    m_index_lep_a[t].push_back(i);
	  } else if(m_index_lep_b[t].size() == 0){
	    vLb += Leptons[i];
	    m_index_lep_b[t].push_back(i);
	  } else {
	    vLb += Leptons[i];
	    m_index_lep_b[t].push_back(i);
	  }
	}

	La[t]->SetLabFrameFourVector(vLa);
	Lb[t]->SetLabFrameFourVector(vLb);
	
	m_Nlep_a[t] = 2;
	m_Nlep_b[t] = 2;
      }
    
      if(!LAB[t]->AnalyzeEvent())
	  cout << "Something went wrong with tree event analysis #" << t << endl;
    }
	
    if(t == 1){ // with combinatorics
      if(m_Njet == 0){
	is_filled[t] = false;
	continue;
      }
      is_filled[t] = true;

      std::vector<RFKey> jetID;
      for(int i = 0; i < m_Njet; i++){
	jetID.push_back(COMB_J[t]->AddLabFrameFourVector(Jets[i]));
      }

      std::vector<RFKey> SVID;
      for(int i = 0; i < m_NSV; i++){
	SVID.push_back(COMB_J[t]->AddLabFrameFourVector(SVs[i]));
      }

      std::vector<RFKey> lepID;
      for(int i = 0; i < m_Nlep; i++){
	lepID.push_back(COMB_L[t]->AddLabFrameFourVector(Leptons[i]));
      }
    
      if(!LAB[t]->AnalyzeEvent())
	cout << "Something went wrong with tree event analysis #" << t << endl;

      // jet counting in ISR/S, hemispheres
      for(int i = 0; i < m_Njet; i++){
	if(COMB_J[t]->GetFrame(jetID[i]) == *ISR[t]){
	  m_Njet_ISR[t]++;
	  if(Jets[i].BtagID() >= kMedium)
	    m_Nbjet_ISR[t]++;
	  m_index_jet_ISR[t].push_back(i);
	}
	if(COMB_J[t]->GetFrame(jetID[i]) == *Ja[t]){
	  m_Njet_S[t]++;
	  m_Njet_a[t]++;
	  if(Jets[i].BtagID() >= kMedium){
	    m_Nbjet_S[t]++;
	    m_Nbjet_a[t]++;
	  }
	  m_index_jet_S[t].push_back(i);
	  m_index_jet_a[t].push_back(i);
	}
	if(COMB_J[t]->GetFrame(jetID[i]) == *Jb[t]){
	  m_Njet_S[t]++;
	  m_Njet_b[t]++;
	  if(Jets[i].BtagID() >= kMedium){
	    m_Nbjet_S[t]++;
	    m_Nbjet_b[t]++;
	  }
	  m_index_jet_S[t].push_back(i);
	  m_index_jet_b[t].push_back(i);
	}
      }
      
      // SV counting in ISR/S, hemispheres
      for(int i = 0; i < m_NSV; i++){
	if(COMB_J[t]->GetFrame(SVID[i]) == *ISR[t]){
	  m_NSV_ISR[t]++;
	  m_index_SV_ISR[t].push_back(i);
	}
	if(COMB_J[t]->GetFrame(SVID[i]) == *Ja[t]){
	  m_NSV_S[t]++;
	  m_NSV_a[t]++;
	  m_index_SV_S[t].push_back(i);
	  m_index_SV_a[t].push_back(i);
	}
	if(COMB_J[t]->GetFrame(SVID[i]) == *Jb[t]){
	  m_NSV_S[t]++;
	  m_NSV_b[t]++;
	  m_index_SV_S[t].push_back(i);
	  m_index_SV_b[t].push_back(i);
	}
      }

      // lepton counting in ISR/S, hemispheres
      for(int i = 0; i < m_Nlep; i++){
	m_Nlep_S[t]++;
	m_index_lep_S[t].push_back(i);
	if(COMB_L[t]->GetFrame(lepID[i]) == *La[t]){
	  m_Nlep_a[t]++;
	  m_index_lep_S[t].push_back(i);
	  m_index_lep_a[t].push_back(i);
	}
	if(COMB_L[t]->GetFrame(lepID[i]) == *Lb[t]){
	  m_Nlep_b[t]++;
	  m_index_lep_S[t].push_back(i);
	  m_index_lep_b[t].push_back(i);
	}
      }
    }
    
    if(!is_filled[t])
      continue;
    
    // Fill Observable Branches

    m_PTCM[t] = CM[t]->GetFourVector().Pt();
    m_cosCM[t] = CM[t]->GetCosDecayAngle();
    m_dphiCM[t] = CM[t]->GetDeltaPhiDecayAngle();
    m_dphiCMI[t] = CM[t]->GetDeltaPhiBoostVisible();
  
    m_MS[t] = S[t]->GetMass();
    m_PS[t] = S[t]->GetMomentum(*CM[t]);
    m_cosS[t]  = S[t]->GetCosDecayAngle();
    m_dphiS[t] = S[t]->GetDeltaPhiDecayAngle();
    m_dphiSI[t]  = S[t]->GetDeltaPhiBoostVisible();
    m_PTS[t] = S[t]->GetFourVector().Pt();
    m_PzS[t] = S[t]->GetFourVector().Pz();

    m_MX3a[t] = X3a[t]->GetMass();
    m_cosX3a[t] = X3a[t]->GetCosDecayAngle();
    m_MX3b[t] = X3b[t]->GetMass();
    m_cosX3b[t] = X3b[t]->GetCosDecayAngle();
    m_EVa[t] = X3a[t]->GetListVisibleFrames().GetFourVector(*X3a[t]).E();
    m_EVb[t] = X3b[t]->GetListVisibleFrames().GetFourVector(*X3b[t]).E();
    m_PVa[t] = X3a[t]->GetListVisibleFrames().GetFourVector(*X3a[t]).P();
    m_PVb[t] = X3b[t]->GetListVisibleFrames().GetFourVector(*X3b[t]).P();
    m_EJa[t] = saJa[t]->GetFourVector(*X3a[t]).E();
    m_EJb[t] = saJb[t]->GetFourVector(*X3b[t]).E();;
    m_PJa[t] = saJa[t]->GetFourVector(*X3a[t]).P();
    m_PJb[t] = saJb[t]->GetFourVector(*X3b[t]).P();

    m_MX2a[t] = X2a[t]->GetMass();
    m_cosX2a[t] = X2a[t]->GetCosDecayAngle();
    m_MX2b[t] = X2b[t]->GetMass();
    m_cosX2b[t] = X2b[t]->GetCosDecayAngle();
    m_ELa[t] = saLa[t]->GetFourVector(*X2a[t]).E();
    m_ELb[t] = saLb[t]->GetFourVector(*X2b[t]).E();;
    m_PLa[t] = saLa[t]->GetFourVector(*X2a[t]).P();
    m_PLb[t] = saLb[t]->GetFourVector(*X2b[t]).P();

    m_MV[t] = S[t]->GetListVisibleFrames().GetMass();
    m_PV[t] = S[t]->GetListVisibleFrames().GetFourVector().P();
    m_MVa[t] = X3a[t]->GetListVisibleFrames().GetMass();
    m_MVb[t] = X3b[t]->GetListVisibleFrames().GetMass();

    m_MJa[t] = saJa[t]->GetMass();
    m_MJb[t] = saJb[t]->GetMass();
    m_MLa[t] = saLa[t]->GetMass();
    m_MLb[t] = saLb[t]->GetMass();
    m_cosJa[t] = saJa[t]->GetCosDecayAngle();
    m_cosJb[t] = saJb[t]->GetCosDecayAngle();
    m_cosLa[t] = saLa[t]->GetCosDecayAngle();
    m_cosLb[t] = saLb[t]->GetCosDecayAngle();

    TLorentzVector vP_Ja_S  = saJa[t]->GetFourVector(*S[t]);
    TLorentzVector vP_Jb_S  = saJb[t]->GetFourVector(*S[t]);
    TLorentzVector vP_La_S  = saLa[t]->GetFourVector(*S[t]);
    TLorentzVector vP_Lb_S  = saLb[t]->GetFourVector(*S[t]);
    TLorentzVector vP_Ia_S  = X1a[t]->GetFourVector(*S[t]);
    TLorentzVector vP_Ib_S  = X1b[t]->GetFourVector(*S[t]);
    
    m_H11S[t] = 2.*(vP_Ia_S+vP_Ib_S).P();
    m_H21S[t] = (vP_Ja_S+vP_La_S).P() +
      (vP_Jb_S+vP_Lb_S).P() + m_H11S[t]/2.;
    m_H22S[t] = (vP_Ja_S+vP_La_S).P() +
      (vP_Jb_S+vP_Lb_S).P() + vP_Ia_S.P() + vP_Ia_S.P();
    m_HT21S[t] = m_H11S[t]/2.
      + S[t]->GetTransverseMomentum(X3a[t]->GetListVisibleFrames().GetFourVector())
      + S[t]->GetTransverseMomentum(X3b[t]->GetListVisibleFrames().GetFourVector());
    m_HT22S[t] = X1a[t]->GetTransverseMomentum(*S[t]) + X1b[t]->GetTransverseMomentum(*S[t])
      + m_HT21S[t] - m_H11S[t]/2.;
    m_H42S[t] = vP_Ja_S.P() + vP_Jb_S.P() + vP_La_S.P() + vP_Lb_S.P() + vP_Ia_S.P() + vP_Ia_S.P();
    m_HT42S[t] = X1a[t]->GetTransverseMomentum(*S[t]) + X1b[t]->GetTransverseMomentum(*S[t])
      + Ja[t]->GetTransverseMomentum(*S[t]) + Jb[t]->GetTransverseMomentum(*S[t])
      + La[t]->GetTransverseMomentum(*S[t]) + Lb[t]->GetTransverseMomentum(*S[t]);
    
    TLorentzVector vP_Ja_X3a  = saJa[t]->GetFourVector(*X3a[t]);
    TLorentzVector vP_Jb_X3b  = saJb[t]->GetFourVector(*X3b[t]);
    TLorentzVector vP_La_X3a  = saLa[t]->GetFourVector(*X3a[t]);
    TLorentzVector vP_Lb_X3b  = saLb[t]->GetFourVector(*X3b[t]);
    TLorentzVector vP_Ia_X3a  = X1a[t]->GetFourVector(*X3a[t]);
    TLorentzVector vP_Ib_X3b  = X1b[t]->GetFourVector(*X3b[t]);
    
    m_H11X3a[t] = 2.*vP_Ia_X3a.P();
    m_H11X3b[t] = 2.*vP_Ib_X3b.P();
    m_H21X3a[t] = vP_Ja_X3a.P() + vP_La_X3a.P() + vP_Ia_X3a.P();
    m_H21X3b[t] = vP_Jb_X3b.P() + vP_Lb_X3b.P() + vP_Ib_X3b.P();
    
    // ISR related variables
    if(m_Njet_ISR[t] > 0){
      TVector3 vPTISR = S[t]->GetTransverseFourVector(*CM[t]).Vect();
      TVector3 vPTINV = (X1a[t]->GetTransverseFourVector(*CM[t])+X1b[t]->GetTransverseFourVector(*CM[t])).Vect();

      m_PTISR[t] = vPTISR.Mag();
      m_MISR[t] = ISR[t]->GetMass();
      m_RISR[t] = fabs(vPTINV.Dot(vPTISR.Unit())) / vPTISR.Mag();

      TVector3 isr_t = ISR[t]->GetTransverseFourVector(*S[t]).Vect();
      TVector3 isr   = ISR[t]->GetFourVector(*S[t]).Vect();
      for(int l = 0; l < m_Nlep_S[t]; l++){
	TVector3 lep   = S[t]->GetFourVector(Leptons[m_index_lep_S[t][l]]).Vect();
	TVector3 lep_t = S[t]->GetTransverseFourVector(Leptons[m_index_lep_S[t][l]]).Vect();
	m_dphi_lep_S[t].push_back( lep_t.Angle(isr_t) );
	m_cos_lep_S[t].push_back( lep_t.Unit().Dot(isr_t.Unit()) );
      }
    }
  }
  

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
  m_BtagID_jet.clear();
  m_Flavor_jet.clear();
  for(int i = 0; i < m_Njet; i++){
    m_PT_jet.push_back(Jets[i].Pt());
    m_Eta_jet.push_back(Jets[i].Eta());
    m_Phi_jet.push_back(Jets[i].Phi());
    m_M_jet.push_back(Jets[i].M());
    m_Btag_jet.push_back(Jets[i].Btag());
    m_BtagID_jet.push_back(Jets[i].BtagID());
    m_Flavor_jet.push_back(Jets[i].PDGID());
  }

  // Fill SVs
  m_PT_SV.clear();
  m_Eta_SV.clear();
  m_Phi_SV.clear();
  m_M_SV.clear();
  for(int i = 0; i < m_NSV; i++){
    m_PT_SV.push_back(SVs[i].Pt());
    m_Eta_SV.push_back(SVs[i].Eta());
    m_Phi_SV.push_back(SVs[i].Phi());
    m_M_SV.push_back(SVs[i].M());
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
  m_Dxy_lep.clear();
  m_DxyErr_lep.clear();
  m_Dz_lep.clear();
  m_DzErr_lep.clear();
  m_IP3D_lep.clear();
  m_SIP3D_lep.clear();
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
    m_Dxy_lep.push_back(Leptons[r].Dxy());
    m_DxyErr_lep.push_back(Leptons[r].DxyErr());
    m_Dz_lep.push_back(Leptons[r].Dz());
    m_DzErr_lep.push_back(Leptons[r].DzErr());
    m_IP3D_lep.push_back(Leptons[r].IP3D());
    m_SIP3D_lep.push_back(Leptons[r].SIP3D());
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
  m_genMomPDGID_lep.clear();
  m_genIndex_lep.clear();
  for(int g = 0; g < m_genNlep; g++){
    m_genPT_lep.push_back(GenLeptons[g].Pt());
    m_genEta_lep.push_back(GenLeptons[g].Eta());
    m_genPhi_lep.push_back(GenLeptons[g].Phi());
    m_genM_lep.push_back(GenLeptons[g].M());
    m_genCharge_lep.push_back(GenLeptons[g].Charge());
    m_genPDGID_lep.push_back(GenLeptons[g].PDGID());
    m_genMomPDGID_lep.push_back(GenLeptons[g].MomPDGID());
    m_genIndex_lep.push_back(genmatch[g]);
  }
  
  // Fill gen neutrino branches
  ParticleList GenNus = AnalysisBase<Base>::GetGenNeutrinos();
  m_genNnu = GenNus.size();
  m_genPT_nu.clear();
  m_genEta_nu.clear();
  m_genPhi_nu.clear();
  m_genPDGID_nu.clear();
  m_genMomPDGID_nu.clear();
  for(int i = 0; i < m_genNnu; i++){
    m_genPT_nu.push_back(GenNus[i].Pt());
    m_genEta_nu.push_back(GenNus[i].Eta());
    m_genPhi_nu.push_back(GenNus[i].Phi());
    m_genPDGID_nu.push_back(GenNus[i].PDGID());
    m_genMomPDGID_nu.push_back(GenNus[i].MomPDGID());
  }
  
  // Fill gen boson branches
  ParticleList GenBosons = AnalysisBase<Base>::GetGenBosons();
  m_genNboson = GenBosons.size();
  m_genPT_boson.clear();
  m_genEta_boson.clear();
  m_genPhi_boson.clear();
  m_genM_boson.clear();
  m_genPDGID_boson.clear();
  m_genMomPDGID_boson.clear();
  for(int i = 0; i < m_genNboson; i++){
    m_genPT_boson.push_back(GenBosons[i].Pt());
    m_genEta_boson.push_back(GenBosons[i].Eta());
    m_genPhi_boson.push_back(GenBosons[i].Phi());
    m_genM_boson.push_back(GenBosons[i].Phi());
    m_genPDGID_boson.push_back(GenBosons[i].PDGID());
    m_genMomPDGID_boson.push_back(GenBosons[i].MomPDGID());
  }

  // Fill gen sparticle branches
  ParticleList GenSparticles = AnalysisBase<Base>::GetGenSparticles();
  m_genNsusy = GenSparticles.size();
  m_genPT_susy.clear();
  m_genEta_susy.clear();
  m_genPhi_susy.clear();
  m_genM_susy.clear();
  m_genPDGID_susy.clear();
  m_genMomPDGID_susy.clear();
  for(int i = 0; i < m_genNsusy; i++){
    m_genPT_susy.push_back(GenSparticles[i].Pt());
    m_genEta_susy.push_back(GenSparticles[i].Eta());
    m_genPhi_susy.push_back(GenSparticles[i].Phi());
    m_genM_susy.push_back(GenSparticles[i].Phi());
    m_genPDGID_susy.push_back(GenSparticles[i].PDGID());
    m_genMomPDGID_susy.push_back(GenSparticles[i].MomPDGID());
  }
  
  // Fill output tree
  if(tree)
    tree->Fill();
}

template class ReducedNtuple<StopNtupleTree>;
template class ReducedNtuple<SUSYNANOBase>;
