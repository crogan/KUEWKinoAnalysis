#include "ReducedNtuple.hh"
#include "ParticleList.hh"
#include "TInterpreter.h"

#include "StopNtupleTree.hh"
#include "SUSYNANOBase.hh"
#include "Leptonic.hh"

using namespace RestFrames;

template <class Base>
ReducedNtuple<Base>::ReducedNtuple(TTree* tree)
  : NtupleBase<Base>(tree)
{
  m_library_generated = false;

  LAB = new LabRecoFrame("LAB","LAB");
  CM  = new DecayRecoFrame("CM","CM");
  S   = new DecayRecoFrame("S","#tilde{S}");
  X3a = new DecayRecoFrame("X3a","#tilde{#chi}_{3a}");
  X3b = new DecayRecoFrame("X3b","#tilde{#chi}_{3b}");
  X2a = new DecayRecoFrame("X2a","#tilde{#chi}_{2a}");
  X2b = new DecayRecoFrame("X2b","#tilde{#chi}_{2b}");
  saJa  = new SelfAssemblingRecoFrame("saJa","jets_{a}");
  saJb  = new SelfAssemblingRecoFrame("saJb","jets_{b}");
  saLa  = new SelfAssemblingRecoFrame("saLa","leps_{a}");
  saLb  = new SelfAssemblingRecoFrame("saLb","leps_{b}");
  
  ISR = new VisibleRecoFrame("ISR","ISR");
  Ja = new VisibleRecoFrame("Ja","jets_{a}");
  Jb = new VisibleRecoFrame("Jb","jets_{b}");
  La = new VisibleRecoFrame("La","leps_{a}");
  Lb = new VisibleRecoFrame("Lb","leps_{b}");
  X1a = new InvisibleRecoFrame("X1a","#tilde{#chi}_{1a}");
  X1b = new InvisibleRecoFrame("X1b","#tilde{#chi}_{1b}");
  
  LAB->SetChildFrame(*CM);
  CM->AddChildFrame(*S);
  CM->AddChildFrame(*ISR);
  S->AddChildFrame(*X3a);
  S->AddChildFrame(*X3b);
  X3a->AddChildFrame(*X2a);
  X3b->AddChildFrame(*X2b);
  X3a->AddChildFrame(*saJa);
  X3b->AddChildFrame(*saJb);
  //X3a->AddChildFrame(*Ja);
  //X3b->AddChildFrame(*Jb);
  X2a->AddChildFrame(*X1a);
  X2b->AddChildFrame(*X1b);
  X2a->AddChildFrame(*saLa);
  X2b->AddChildFrame(*saLb);
  //X2a->AddChildFrame(*La);
  //X2b->AddChildFrame(*Lb);
  saJa->AddChildFrame(*Ja);
  saJb->AddChildFrame(*Jb);
  saLa->AddChildFrame(*La);
  saLb->AddChildFrame(*Lb);
  
  if(!LAB->InitializeTree()){
    cout << "Problem initializing tree" << endl;
  }
  
  INV = new InvisibleGroup("INV","Invisible System");
  INV->AddFrame(*X1a);
  INV->AddFrame(*X1b);
  
  InvM = new SetMassInvJigsaw("InvM", "Set inv. system mass");
  INV->AddJigsaw(*InvM);
  
  InvEta = new SetRapidityInvJigsaw("InvEta", "Set inv. system rapidity");
  INV->AddJigsaw(*InvEta);
  InvEta->AddVisibleFrames(S->GetListVisibleFrames());
  
  InvSplit = new MinMassesSqInvJigsaw("InvSplit", "INV -> #tilde{#chi_{1a}}+ #tilde{#chi_{1b}}", 2);
  INV->AddJigsaw(*InvSplit);
  InvSplit->AddVisibleFrame(*Ja, 0);
  InvSplit->AddVisibleFrame(*Jb, 1);
  InvSplit->AddVisibleFrame(*La, 0);
  InvSplit->AddVisibleFrame(*Lb, 1);
  // InvSplit->AddVisibleFrames(X3a->GetListVisibleFrames(), 0);
  // InvSplit->AddVisibleFrames(X3b->GetListVisibleFrames(), 1);
  InvSplit->AddInvisibleFrame(*X1a, 0);
  InvSplit->AddInvisibleFrame(*X1b, 1);
  
  COMB_J =  new CombinatoricGroup("COMB_J", "Combinatoric System of Jets");
  CombSplit_ISR = new MinMassesCombJigsaw("CombSplit_ISR", "Minimize M_{T}^{ISR} and M_{T}^{S}");
  CombSplit_J = new MinMassesSqCombJigsaw("CombSplit_J", "Minimize M_{Va}^{2} + M_{Vb}^{2} ",2,2);
  
  COMB_L =  new CombinatoricGroup("COMB_L", "Combinatoric System of Leps");
  CombSplit_L = new MinMassesSqCombJigsaw("CombSplit_L", "Minimize M_{Va}^{2} + M_{Vb}^{2}",2,2);
  
  COMB_J->AddFrame(*ISR);
  COMB_J->SetNElementsForFrame(*ISR, 1);
  COMB_J->AddFrame(*Ja);
  COMB_J->SetNElementsForFrame(*Ja, 0);
  COMB_J->AddFrame(*Jb);
  COMB_J->SetNElementsForFrame(*Jb, 0);
  
  COMB_J->AddJigsaw(*CombSplit_ISR);
  CombSplit_ISR->SetTransverse();
  CombSplit_ISR->AddCombFrame(*ISR, 0);
  CombSplit_ISR->AddCombFrame(*Ja, 1);
  CombSplit_ISR->AddCombFrame(*Jb, 1);
  CombSplit_ISR->AddObjectFrame(*ISR, 0);
  CombSplit_ISR->AddObjectFrame(*S, 1);
    
  COMB_J->AddJigsaw(*CombSplit_J);
  CombSplit_J->AddCombFrame(*Ja, 0);
  CombSplit_J->AddCombFrame(*Jb, 1);
  CombSplit_J->AddObjectFrames(X3a->GetListVisibleFrames(), 0);
  CombSplit_J->AddObjectFrames(X3b->GetListVisibleFrames(), 1);
    
  COMB_L->AddFrame(*La);
  COMB_L->SetNElementsForFrame(*La, 0);
  COMB_L->AddFrame(*Lb);
  COMB_L->SetNElementsForFrame(*Lb, 0);
    
  COMB_L->AddJigsaw(*CombSplit_L);
  CombSplit_L->AddCombFrame(*La, 0);
  CombSplit_L->AddCombFrame(*Lb, 1);
  CombSplit_L->AddObjectFrames(X3a->GetListVisibleFrames(), 0);
  CombSplit_L->AddObjectFrames(X3b->GetListVisibleFrames(), 1);
 

  if(!LAB->InitializeAnalysis())
    cout << "Problem initializing analysis tree" << endl;

  /*
    TreePlot tree_plot("TreePlot","TreePlot");

    for(int t = 0; t < 2; t++){
    tree_plot.SetTree(*LAB);
    tree_plot.Draw("ANA_tree", "Reconstruction Tree");

    tree_plot.SetTree(*COMB_J);
    tree_plot.Draw("ANA_comb", "Combinatoric Jigsaws for jets");

    tree_plot.SetTree(*COMB_L);
    tree_plot.Draw("ANA_comb_L", "Combinatoric Jigsaws for leps");

    tree_plot.SetTree(*INV);
    tree_plot.Draw("ANA_inv", "Invisible Jigsaws");
 
    }
    tree_plot.WriteOutput("trees.root");
  */
  
}

template <class Base>
ReducedNtuple<Base>::~ReducedNtuple() {
 
  delete LAB;
  delete CM;
  delete S;
  delete X3a;
  delete X3b;
  delete X2a;
  delete X2b;
  delete saJa;
  delete saJb;
  delete saLa;
  delete saLb;
  delete ISR;
  delete Ja;
  delete Jb;
  delete La;
  delete Lb;
  delete X1a;
  delete X1b;
    
  delete INV;
  delete InvM;
  delete InvEta;
  delete InvSplit;
    
  delete COMB_J;
  delete CombSplit_ISR;
  delete CombSplit_J;
    
  delete COMB_L;
  delete CombSplit_L;
  
}

template <class Base>
TTree* ReducedNtuple<Base>::InitOutputTree(const string& sample, bool do_slim){

  // gInterpreter->GenerateDictionary("vectorr<int>", "vector");
  // gInterpreter->GenerateDictionary("vector<double>", "vector");
  // if(!m_library_generated){
  //   gInterpreter->GenerateDictionary("std::vector<std::vector<int> >", "vector");
  //   gInterpreter->GenerateDictionary("std::vector<std::vector<double> >", "vector");
  //   m_library_generated = true;
  // }
  
  TTree* tree = (TTree*) new TTree(sample.c_str(), sample.c_str());

  tree->Branch("event_skipped", &m_event_skipped);
  
  tree->Branch("weight", &m_weight);

  tree->Branch("PUweight", &m_PUweight);
  tree->Branch("PUweight_up", &m_PUweight_up);
  tree->Branch("PUweight_down", &m_PUweight_down);

  tree->Branch("MuFweight", &m_MuFweight);
  tree->Branch("MuFweight_up", &m_MuFweight_up);
  tree->Branch("MuFweight_down", &m_MuFweight_down);

  tree->Branch("MuRweight", &m_MuRweight);
  tree->Branch("MuRweight_up", &m_MuRweight_up);
  tree->Branch("MuRweight_down", &m_MuRweight_down);

  tree->Branch("PDFweight", &m_PDFweight);
  tree->Branch("PDFweight_up", &m_PDFweight_up);
  tree->Branch("PDFweight_down", &m_PDFweight_down);

  tree->Branch("BtagHFSFweight", &m_BtagHFSFweight);
  tree->Branch("BtagHFSFweight_up", &m_BtagHFSFweight_up);
  tree->Branch("BtagHFSFweight_down", &m_BtagHFSFweight_down);
  tree->Branch("BtagLFSFweight", &m_BtagLFSFweight);
  tree->Branch("BtagLFSFweight_up", &m_BtagLFSFweight_up);
  tree->Branch("BtagLFSFweight_down", &m_BtagLFSFweight_down);

  tree->Branch("elIDSFweight", &m_elIDSFweight);
  tree->Branch("elIDSFweight_up", &m_elIDSFweight_up);
  tree->Branch("elIDSFweight_down", &m_elIDSFweight_down);
  tree->Branch("elISOSFweight", &m_elISOSFweight);
  tree->Branch("elISOSFweight_up", &m_elISOSFweight_up);
  tree->Branch("elISOSFweight_down", &m_elISOSFweight_down);
  tree->Branch("elSIPSFweight", &m_elSIPSFweight);
  tree->Branch("elSIPSFweight_up", &m_elSIPSFweight_up);
  tree->Branch("elSIPSFweight_down", &m_elSIPSFweight_down);
  tree->Branch("elVLSFweight", &m_elVLSFweight);
  tree->Branch("elVLSFweight_up", &m_elVLSFweight_up);
  tree->Branch("elVLSFweight_down", &m_elVLSFweight_down);
  tree->Branch("muIDSFweight", &m_muIDSFweight);
  tree->Branch("muIDSFweight_up", &m_muIDSFweight_up);
  tree->Branch("muIDSFweight_down", &m_muIDSFweight_down);
  tree->Branch("muISOSFweight", &m_muISOSFweight);
  tree->Branch("muISOSFweight_up", &m_muISOSFweight_up);
  tree->Branch("muISOSFweight_down", &m_muISOSFweight_down);
  tree->Branch("muSIPSFweight", &m_muSIPSFweight);
  tree->Branch("muSIPSFweight_up", &m_muSIPSFweight_up);
  tree->Branch("muSIPSFweight_down", &m_muSIPSFweight_down);
  tree->Branch("muVLSFweight", &m_muVLSFweight);
  tree->Branch("muVLSFweight_up", &m_muVLSFweight_up);
  tree->Branch("muVLSFweight_down", &m_muVLSFweight_down);
   
  tree->Branch("MetTrigSFweight", &m_MetTrigSFweight);
  tree->Branch("MetTrigSFweight_up", &m_MetTrigSFweight_up);
  tree->Branch("MetTrigSFweight_down", &m_MetTrigSFweight_down);
  tree->Branch("MetTrigSFCurveIndex", &m_MetTrigSFCurveIndex);

  tree->Branch("runnum", &m_runnum);
  tree->Branch("luminum", &m_luminum);
  tree->Branch("eventnum", &m_eventnum);

  tree->Branch("NPV", &m_NPV);

  tree->Branch("EventFilter", &m_EventFilter);
  tree->Branch("FastSimEventVeto", &m_FastSimEventVeto);

  tree->Branch("PrefireWeight", &m_PrefireWeight);
  tree->Branch("PrefireWeight_up", &m_PrefireWeight_up);
  tree->Branch("PrefireWeight_down", &m_PrefireWeight_down);

  tree->Branch("METtrigger", &m_METtrigger);
  tree->Branch("METHTtrigger", &m_METHTtrigger);
  tree->Branch("METORtrigger", &m_METORtrigger);

  if(!do_slim){
    tree->Branch("DoubleElectrontrigger", &m_DoubleElectrontrigger);
    tree->Branch("DoubleMuontrigger", &m_DoubleMuontrigger);
  }

  tree->Branch("EventFlag_FailJetID", &m_EventFlag_FailJetID);
  tree->Branch("EventFlag_JetInHEM", &m_EventFlag_JetInHEM);
  tree->Branch("EventFlag_JetInHEM_Pt20", &m_EventFlag_JetInHEM_Pt20);
  tree->Branch("EventFlag_JetInHEM_Pt20_JetID", &m_EventFlag_JetInHEM_Pt20_JetID);
  tree->Branch("HEM_Veto", &m_HEM_Veto);

  tree->Branch("SingleElectrontrigger", &m_SingleElectrontrigger);
  tree->Branch("SingleMuontrigger", &m_SingleMuontrigger);
  tree->Branch("EMutrigger", &m_EMutrigger);
  
  tree->Branch("MET", &m_MET);
  tree->Branch("MET_phi", &m_MET_phi);

  tree->Branch("altMET", &m_altMET);
  tree->Branch("altMET_phi", &m_altMET_phi);

  tree->Branch("LHE_HT", &m_LHE_HT);
  tree->Branch("LHE_HTIncoming", &m_LHE_HTIncoming);

  tree->Branch("HT_eta24", &m_HT_eta24);
  tree->Branch("HT_eta24_id", &m_HT_eta24_id);
  tree->Branch("HT_eta3", &m_HT_eta3);
  tree->Branch("HT_eta3_id", &m_HT_eta3_id);
  tree->Branch("HT_eta5", &m_HT_eta5);
  tree->Branch("HT_eta5_id", &m_HT_eta5_id);

  tree->Branch("Nele", &m_Nele);
  tree->Branch("Nmu", &m_Nmu);

  tree->Branch("Nlep", &m_Nlep);
  tree->Branch("PT_lep",  &m_PT_lep);
  
  tree->Branch("Eta_lep", &m_Eta_lep);
  tree->Branch("Phi_lep", &m_Phi_lep);
  tree->Branch("M_lep",   &m_M_lep);
  
  tree->Branch("Charge_lep",  &m_Charge_lep);
  tree->Branch("PDGID_lep",   &m_PDGID_lep);
  tree->Branch("ID_lep", &m_ID_lep);
  tree->Branch("SourceID_lep", &m_SourceID_lep);
  tree->Branch("LepQual_lep", &m_LepQual_lep);
  tree->Branch("Index_lep", &m_Index_lep);

 // if(!do_slim){
    tree->Branch("RelIso_lep",  &m_RelIso_lep);
    tree->Branch("MiniIso_lep", &m_MiniIso_lep);
    tree->Branch("Dxy_lep", &m_Dxy_lep);
    tree->Branch("DxyErr_lep", &m_DxyErr_lep);
    tree->Branch("Dz_lep", &m_Dz_lep);
    tree->Branch("DzErr_lep", &m_DzErr_lep);
    tree->Branch("IP3D_lep", &m_IP3D_lep);
    tree->Branch("SIP3D_lep", &m_SIP3D_lep);
  //}
  
  tree->Branch("Njet", &m_Njet);
  tree->Branch("Nbjet", &m_Nbjet);

  if(!do_slim){
    tree->Branch("PT_jet",  &m_PT_jet);
    tree->Branch("Eta_jet", &m_Eta_jet);
    tree->Branch("Phi_jet", &m_Phi_jet);
    tree->Branch("M_jet",   &m_M_jet);
    tree->Branch("Btag_jet",   &m_Btag_jet);
    tree->Branch("BtagID_jet",   &m_BtagID_jet);
    tree->Branch("Flavor_jet",   &m_Flavor_jet);
    tree->Branch("index_jet_a", &m_index_jet_a);
    tree->Branch("index_jet_b", &m_index_jet_b);
    tree->Branch("index_jet_ISR", &m_index_jet_ISR);
    tree->Branch("index_jet_S", &m_index_jet_S);
  }

  tree->Branch("NSV", &m_NSV);
  tree->Branch("PT_SV",  &m_PT_SV);
  tree->Branch("Eta_SV", &m_Eta_SV);
  tree->Branch("Phi_SV", &m_Phi_SV);
  tree->Branch("M_SV",   &m_M_SV);
  tree->Branch("ProbB_SV",   &m_ProbB_SV);
  tree->Branch("ProbC_SV",   &m_ProbC_SV);

  if(!do_slim){
    tree->Branch("PT_Genjet",  &m_PT_Genjet);
    tree->Branch("Eta_Genjet", &m_Eta_Genjet);
    tree->Branch("Phi_Genjet", &m_Phi_Genjet);
    tree->Branch("M_Genjet",   &m_M_Genjet);
    tree->Branch("Index_jet",   &m_Index_jet);
  }
  
  tree->Branch("Njet_ISR", &m_Njet_ISR);
  tree->Branch("Njet_S", &m_Njet_S);
  tree->Branch("Nbjet_ISR", &m_Nbjet_ISR);
  tree->Branch("Nbjet_S", &m_Nbjet_S);
  tree->Branch("Nlep_ISR", &m_Nlep_ISR);
  tree->Branch("Nlep_S", &m_Nlep_S);
  tree->Branch("NSV_ISR", &m_NSV_ISR);
  tree->Branch("NSV_S", &m_NSV_S);
  tree->Branch("index_SV_ISR", &m_index_SV_ISR);
  tree->Branch("index_SV_S", &m_index_SV_S);
  tree->Branch("index_lep_ISR", &m_index_lep_ISR);
  tree->Branch("index_lep_S", &m_index_lep_S);

  if(!do_slim){
    tree->Branch("dphi_lep_S", &m_dphi_lep_S);
    tree->Branch("cos_lep_S", &m_cos_lep_S);
    tree->Branch("dphi_jet_S", &m_dphi_jet_S);
    tree->Branch("cos_jet_S", &m_cos_jet_S);
    tree->Branch("dphi_SV_S", &m_dphi_SV_S);
    tree->Branch("cos_SV_S", &m_cos_SV_S);

    tree->Branch("dphiMET_lep_S", &m_dphiMET_lep_S);
    tree->Branch("dphiMET_jet_S", &m_dphiMET_jet_S);
    tree->Branch("dphiMET_SV_S", &m_dphiMET_SV_S);
  }
  
  tree->Branch("Njet_a", &m_Njet_a);
  tree->Branch("Njet_b", &m_Njet_b);
  tree->Branch("Nbjet_a", &m_Nbjet_a);
  tree->Branch("Nbjet_b", &m_Nbjet_b);
  tree->Branch("Nlep_a", &m_Nlep_a);
  tree->Branch("Nlep_b", &m_Nlep_b);
  tree->Branch("NSV_a", &m_NSV_a);
  tree->Branch("NSV_b", &m_NSV_b);
  
  tree->Branch("index_lep_a", &m_index_lep_a);
  tree->Branch("index_lep_b", &m_index_lep_b);
  tree->Branch("index_SV_a", &m_index_SV_a);
  tree->Branch("index_SV_b", &m_index_SV_b);

  tree->Branch("PTCM", &m_PTCM);
  tree->Branch("PzCM", &m_PzCM);
  tree->Branch("cosCM", &m_cosCM);
  tree->Branch("dphiCM", &m_dphiCM);
  tree->Branch("dphiCMI", &m_dphiCMI);
  tree->Branch("dphiMET_V", &m_dphiMET_V);

  tree->Branch("Mperp", &m_Mperp);
  tree->Branch("gammaT", &m_gammaT);
  tree->Branch("EJ_BoostT", &m_EJ_BoostT);
  tree->Branch("EL_BoostT", &m_EL_BoostT);
  tree->Branch("PTISR", &m_PTISR);
  tree->Branch("RISR", &m_RISR);
  
  if(!do_slim){
    tree->Branch("MS", &m_MS);
    tree->Branch("PS", &m_PS);
    tree->Branch("cosS", &m_cosS);
    tree->Branch("dphiS", &m_dphiS);
    tree->Branch("dphiSI", &m_dphiSI);
    tree->Branch("PTS", &m_PTS);
    tree->Branch("PzS", &m_PzS);
    
    tree->Branch("MX3a", &m_MX3a);
    tree->Branch("MX3b", &m_MX3b);
    tree->Branch("EVa", &m_EVa);
    tree->Branch("EVb", &m_EVb);
    tree->Branch("PVa", &m_PVa);
    tree->Branch("PVb", &m_PVb);
    tree->Branch("EJa", &m_EJa);
    tree->Branch("EJb", &m_EJb);
    tree->Branch("PJa", &m_PJa);
    tree->Branch("PJb", &m_PJb);

    tree->Branch("MX2a", &m_MX2a);
    tree->Branch("MX2b", &m_MX2b);
    tree->Branch("ELa", &m_ELa);
    tree->Branch("ELb", &m_ELb);
    tree->Branch("PLa", &m_PLa);
    tree->Branch("PLb", &m_PLb);

    tree->Branch("MV", &m_MV);
    tree->Branch("PV", &m_PV);
    tree->Branch("MVa", &m_MVa);
    tree->Branch("MVb", &m_MVb);
    tree->Branch("PV_lab", &m_PV_lab);

    tree->Branch("MJa", &m_MJa);
    tree->Branch("MJb", &m_MJb);
    tree->Branch("MLa", &m_MLa);
    tree->Branch("MLb", &m_MLb);
    tree->Branch("cosJa", &m_cosJa);
    tree->Branch("cosJb", &m_cosJb);
    tree->Branch("cosLa", &m_cosLa);
    tree->Branch("cosLb", &m_cosLb);
    
    tree->Branch("MJ", &m_MJ);
    tree->Branch("ML", &m_ML);
    tree->Branch("EJ", &m_EJ);
    tree->Branch("EL", &m_EL);
    tree->Branch("PJ", &m_PJ);
    tree->Branch("PL", &m_PL);
    
    tree->Branch("PX3", &m_PX3);
    tree->Branch("PX3_BoostT", &m_PX3_BoostT);
    tree->Branch("MX3a_BoostT", &m_MX3a_BoostT);
    tree->Branch("MX3b_BoostT", &m_MX3b_BoostT);

    tree->Branch("PV_BoostT", &m_PV_BoostT);
    
    tree->Branch("EVa_BoostT", &m_EVa_BoostT);
    tree->Branch("EVb_BoostT", &m_EVb_BoostT);
    tree->Branch("PVa_BoostT", &m_PVa_BoostT);
    tree->Branch("PVb_BoostT", &m_PVb_BoostT);
    
    tree->Branch("PJ_BoostT", &m_PJ_BoostT);
    tree->Branch("PL_BoostT", &m_PL_BoostT);
  
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
    
    tree->Branch("PISR", &m_PISR);
    tree->Branch("RISRT", &m_RISRT);
    tree->Branch("MISR", &m_MISR);
  }

  if(!AnalysisBase<Base>::IsData()){
    tree->Branch("NPU", &m_NPU);
    tree->Branch("genMET", &m_genMET);
    tree->Branch("genMET_phi", &m_genMET_phi);
    
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
    tree->Branch("genSourceID_lep",   &m_genSourceID_lep);
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
  }
    
  return tree;
}

template <class Base>
void ReducedNtuple<Base>::ClearVariables(){

  m_event_skipped = false;
  
  m_Njet_ISR = 0;
  m_Njet_S = 0;
  m_Nbjet_ISR = 0;
  m_Nbjet_S = 0;
  m_Nlep_ISR = 0;
  m_Nlep_S = 0;
  m_NSV_ISR = 0;
  m_NSV_S = 0;
  m_index_jet_ISR.clear();
  m_index_jet_S.clear();
  m_index_SV_ISR.clear();
  m_index_SV_S.clear();
  m_index_lep_ISR.clear();
  m_index_lep_S.clear();

  m_dphi_lep_S.clear();
  m_cos_lep_S.clear();
  m_dphi_jet_S.clear();
  m_cos_jet_S.clear();
  m_dphi_SV_S.clear();
  m_cos_SV_S.clear();

  m_dphiMET_lep_S.clear();
  m_dphiMET_jet_S.clear();
  m_dphiMET_SV_S.clear();
 
  m_Njet_a = 0;
  m_Njet_b = 0;
  m_Nbjet_a = 0;
  m_Nbjet_b = 0;
  m_Nlep_a = 0;
  m_Nlep_b = 0;
  m_NSV_a = 0;
  m_NSV_b = 0;
   
  m_index_jet_a.clear();
  m_index_jet_b.clear();
  m_index_lep_a.clear();
  m_index_lep_b.clear();
  m_index_SV_a.clear();
  m_index_SV_b.clear();

  m_PTCM = 0.;
  m_PzCM = 0.;
  m_cosCM = 0.;
  m_dphiCM = 0.;
  m_dphiCMI = 0.;
  
  m_MS = 0.;
  m_PS = 0.;
  m_cosS = 0.;
  m_dphiS = 0.;
  m_dphiSI = 0.;
  m_PTS = 0.;
  m_PzS = 0.;

  m_MX3a = 0.;
  m_MX3b = 0.;
  m_EVa = 0.;
  m_EVb = 0.;
  m_PVa = 0.;
  m_PVb = 0.;
  m_EJa = 0.;
  m_EJb = 0.;
  m_PJa = 0.;
  m_PJb = 0.;

  m_MX2a = 0.;
  m_MX2b = 0.;
  m_ELa = 0.;
  m_ELb = 0.;
  m_PLa = 0.;
  m_PLb = 0.;

  m_MV = 0.;
  m_PV = 0.;
  m_MVa = 0.;
  m_MVb = 0.;

  m_PV_lab = 0.;
  m_dphiMET_V = 0.;
  
  m_MJa = 0.;
  m_MJb = 0.;
  m_MLa = 0.;
  m_MLb = 0.;
  m_cosJa = 0.;
  m_cosJb = 0.;
  m_cosLa = 0.;
  m_cosLb = 0.;

  m_MJ = 0.;
  m_ML = 0.;
  m_EJ = 0.;
  m_EL = 0.;
  m_PJ = 0.;
  m_PL = 0.;
  
  m_PX3 = 0.;
  m_PX3_BoostT = 0.;
  m_MX3a_BoostT = 0.;
  m_MX3b_BoostT = 0.;

  m_PV_BoostT = 0.;
  
  m_EVa_BoostT = 0.;
  m_EVb_BoostT = 0.;
  m_PVa_BoostT = 0.;
  m_PVb_BoostT = 0.;

  m_EJ_BoostT = 0.;
  m_EL_BoostT = 0.;
  m_PJ_BoostT = 0.;
  m_PL_BoostT = 0.;

  m_H11S = 0.;
  m_H21S = 0.;
  m_HT21S = 0.;
  m_H22S = 0.;
  m_HT22S = 0.;
  m_H42S = 0.;
  m_HT42S = 0.;
  
  m_H11X3a = 0.;
  m_H11X3b = 0.;
  m_H21X3a = 0.;
  m_H21X3b = 0.;

  // ISR related variables
  m_PISR = 0.;
  m_PTISR = 0.;
  m_MISR = 0.;
  m_RISR = 0.;
  m_RISRT = 0.;
 
}

template <class Base>
void ReducedNtuple<Base>::FillOutputTree(TTree* tree, const Systematic& sys, bool do_slim){
  
  AnalysisBase<Base>::SetSystematic(sys);

  m_EventFilter = AnalysisBase<Base>::PassEventFilter();

  if(AnalysisBase<Base>::IsData())
    if(!AnalysisBase<Base>::IsGoodEvent())
      return;

  //cout << "Event Weight " << tree->GetName() << " " << AnalysisBase<Base>::GetEventWeight() << endl;
  
  bool good_PV;
  TVector3 PV = AnalysisBase<Base>::GetPV(good_PV);

  if(!good_PV)
    return;

  TVector3 ETMiss;
  ParticleList Jets_noID = AnalysisBase<Base>::GetJetsMET(ETMiss);
  ParticleList Jets      = AnalysisBase<Base>::GetJetsMET(ETMiss, 3); // jet ID 3
  ParticleList GenJets   = AnalysisBase<Base>::GetGenJets();

  m_FastSimEventVeto = AnalysisBase<Base>::FastSimEventVeto(GenJets);
  
  m_PrefireWeight = AnalysisBase<Base>::GetPrefireWeight(0);
  m_PrefireWeight_up = AnalysisBase<Base>::GetPrefireWeight(1);
  m_PrefireWeight_down = AnalysisBase<Base>::GetPrefireWeight(-1);
  
  if(ETMiss.Mag() < 150.)
    return;
  
  ClearVariables();

  if(Jets_noID.size() != Jets.size())
    m_EventFlag_FailJetID = true;
  else
    m_EventFlag_FailJetID = false;

  int Njet      = Jets.size();
  int Njet_noID = Jets_noID.size();

  // any jets in HEM region?
  m_EventFlag_JetInHEM = false;
  for(int j = 0; j < Njet_noID; j++)
    if(AnalysisBase<Base>::IsHEM(Jets_noID[j]))
      m_EventFlag_JetInHEM = true;

  Jets      = Jets.PtEtaCut(20., 5.);
  Jets_noID = Jets_noID.PtEtaCut(20., 5.);
  GenJets   = GenJets.PtEtaCut(20., 5.);
  Njet      = Jets.size();
  Njet_noID = Jets_noID.size();

  m_EventFlag_JetInHEM_Pt20 = false;
  m_HT_eta5 = 0.;
  for(int j = 0; j < Njet_noID; j++){
    if(AnalysisBase<Base>::IsHEM(Jets_noID[j]))
      m_EventFlag_JetInHEM_Pt20 = true;
    m_HT_eta5 += Jets_noID[j].Pt();
  }

  m_EventFlag_JetInHEM_Pt20_JetID = false;
  m_HT_eta5_id = 0.;
  for(int j = 0; j < Njet; j++){
    if(AnalysisBase<Base>::IsHEM(Jets[j]))
      m_EventFlag_JetInHEM_Pt20_JetID = true;
    m_HT_eta5_id += Jets[j].Pt();
  }

  Jets      = Jets.PtEtaCut(20., 3.);
  Jets_noID = Jets_noID.PtEtaCut(20., 3.);
  Njet      = Jets.size();
  Njet_noID = Jets_noID.size();

  m_HT_eta3 = 0.;
  for(int j = 0; j < Njet_noID; j++){
    m_HT_eta3 += Jets_noID[j].Pt();
  }

  m_HT_eta3_id = 0.;
  for(int j = 0; j < Njet; j++){
    m_HT_eta3_id += Jets[j].Pt();
  }

  Jets      = Jets.PtEtaCut(20., 2.4);
  Jets_noID = Jets_noID.PtEtaCut(20., 2.4);
  Njet      = Jets.size();
  Njet_noID = Jets_noID.size();

  m_HT_eta24 = 0.;
  for(int j = 0; j < Njet_noID; j++){
    m_HT_eta24 += Jets_noID[j].Pt();
  }

  m_HT_eta24_id = 0.;
  for(int j = 0; j < Njet; j++){
    m_HT_eta24_id += Jets[j].Pt();
  }

  m_LHE_HT = AnalysisBase<Base>::Get_LHE_HT();
  m_LHE_HTIncoming = AnalysisBase<Base>::Get_LHE_HTIncoming();
  
  ParticleList Muons = AnalysisBase<Base>::GetMuons();
  Muons = Muons.ParticleIDCut(kVeryLoose);
  Muons = Muons.PtEtaCut(3.0);

  ParticleList Electrons = AnalysisBase<Base>::GetElectrons();
  Electrons = Electrons.ParticleIDCut(kVeryLoose);
  Electrons = Electrons.PtEtaCut(5.0);
  
  ParticleList Leptons = Electrons+Muons;
  Leptons.SortByPt();

  ParticleList SVs = AnalysisBase<Base>::GetSVs(PV);
  SVs = SVs.RemoveOverlap(Leptons, 0.2);
  SVs = SVs.RemoveOverlap(Jets, 0.4);
  
  Jets = Jets.RemoveOverlap(Leptons, 0.2);

  ParticleList GenMuons = AnalysisBase<Base>::GetGenMuons();
  ParticleList GenElectrons = AnalysisBase<Base>::GetGenElectrons();
  ParticleList GenLeptons = GenElectrons+GenMuons;
  GenLeptons.SortByPt();

  m_genNele = GenElectrons.size();
  m_genNmu  = GenMuons.size();
  m_genNlep = GenLeptons.size();

  GenJets = GenJets.RemoveOverlap(GenLeptons, 0.2);

  // merge jets until total number is combinatorically manageable
  Jets = Jets.BinaryMerge(13-SVs.size()-Leptons.size());
  Jets.SortByPt();
  GenJets.SortByPt();
  
  // skip event reconstruction for now if too many jets
  // if((Jets.size()+SVs.size()) >= 16){
  //   m_event_skipped = true;
  //   if(tree)
  //     tree->Fill();
  //   return;
  // }
  
  m_Njet = Jets.size();
  m_NGenjet = GenJets.size();  

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
  
  // cout << "Njet=" << m_Njet << " NSV=" << m_NSV << " m_Nele=" << m_Nele << " Nmu=" << m_Nmu << endl;

  // require at least one lepton for now
  // if(m_Nlep < 1)
  //   return;
  
  // not enough stuff
  if(m_Nlep + m_Njet + m_NSV < 2 || m_Njet < 1)
    return;

  m_HEM_Veto = m_EventFlag_JetInHEM_Pt20;
  for(int l = 0; l < m_Nlep; l++)
    if(AnalysisBase<Base>::IsHEM(Leptons[l]))
      m_HEM_Veto = true;

  TVector3 altETMiss = AnalysisBase<Base>::GetAltMET();
  m_altMET     = altETMiss.Pt();
  m_altMET_phi = altETMiss.Phi();
  
  // Sparticle pair-production trees analysis
    
  LAB->ClearEvent(); //////////////////////////////////////////

  INV->SetLabFrameThreeVector(ETMiss);
    
  std::vector<RFKey> jetID;
  for(int i = 0; i < m_Njet; i++){
    jetID.push_back(COMB_J->AddLabFrameFourVector(Jets[i]));
  }

  std::vector<RFKey> SVID;
  for(int i = 0; i < m_NSV; i++){
    SVID.push_back(COMB_J->AddLabFrameFourVector(SVs[i]));
  }

  std::vector<RFKey> lepID;
  for(int i = 0; i < m_Nlep; i++){
    lepID.push_back(COMB_L->AddLabFrameFourVector(Leptons[i]));
  }

  if(!LAB->AnalyzeEvent())
    cout << "Something went wrong with tree event analysis" << endl;
      
  // jet counting in ISR/S, hemispheres
  for(int i = 0; i < m_Njet; i++){
    if(COMB_J->GetFrame(jetID[i]) == *ISR){
      m_Njet_ISR++;
      if(Jets[i].BtagID() >= kMedium)
	m_Nbjet_ISR++;
      m_index_jet_ISR.push_back(i);
    }
    if(COMB_J->GetFrame(jetID[i]) == *Ja){
      m_Njet_S++;
      m_Njet_a++;
      if(Jets[i].BtagID() >= kMedium){
	m_Nbjet_S++;
	m_Nbjet_a++;
      }
      m_index_jet_S.push_back(i);
      m_index_jet_a.push_back(i);
    }
    if(COMB_J->GetFrame(jetID[i]) == *Jb){
      m_Njet_S++;
      m_Njet_b++;
      if(Jets[i].BtagID() >= kMedium){
	m_Nbjet_S++;
	m_Nbjet_b++;
      }
      m_index_jet_S.push_back(i);
      m_index_jet_b.push_back(i);
    }
  }
      
  // SV counting in ISR/S, hemispheres
  for(int i = 0; i < m_NSV; i++){
    if(COMB_J->GetFrame(SVID[i]) == *ISR){
      m_NSV_ISR++;
      m_index_SV_ISR.push_back(i);
    }
    if(COMB_J->GetFrame(SVID[i]) == *Ja){
      m_NSV_S++;
      m_NSV_a++;
      m_index_SV_S.push_back(i);
      m_index_SV_a.push_back(i);
    }
    if(COMB_J->GetFrame(SVID[i]) == *Jb){
      m_NSV_S++;
      m_NSV_b++;
      m_index_SV_S.push_back(i);
      m_index_SV_b.push_back(i);
    }
  }

  // lepton counting in ISR/S, hemispheres
  for(int i = 0; i < m_Nlep; i++){
    m_Nlep_S++;
    if(COMB_L->GetFrame(lepID[i]) == *La){
      m_Nlep_a++;
      m_index_lep_S.push_back(i);
      m_index_lep_a.push_back(i);
    }
    if(COMB_L->GetFrame(lepID[i]) == *Lb){
      m_Nlep_b++;
      m_index_lep_S.push_back(i);
      m_index_lep_b.push_back(i);
    }
  }

  if(m_Nlep + m_NSV_S + m_Njet_S < 1)
    return;
  
  // Fill Observable Branches
  
  m_PTCM = CM->GetFourVector().Pt();
  m_PzCM = CM->GetFourVector().Pz();
  m_cosCM = CM->GetCosDecayAngle();
  m_dphiCM = CM->GetDeltaPhiDecayAngle();
  m_dphiCMI = CM->GetDeltaPhiBoostVisible();
  
  m_MS = S->GetMass();
  m_PS = S->GetMomentum(*CM);
  m_cosS  = S->GetCosDecayAngle();
  m_dphiS = S->GetDeltaPhiDecayAngle();
  m_dphiSI  = S->GetDeltaPhiBoostVisible();
  m_PTS = S->GetFourVector().Pt();
  m_PzS = S->GetFourVector().Pz();
  
  m_MX3a = X3a->GetMass();
  //m_cosX3a = X3a->GetCosDecayAngle();
  m_MX3b = X3b->GetMass();
  //m_cosX3b = X3b->GetCosDecayAngle();
  m_EVa = X3a->GetListVisibleFrames().GetFourVector(*X3a).E();
  m_EVb = X3b->GetListVisibleFrames().GetFourVector(*X3b).E();
  m_PVa = X3a->GetListVisibleFrames().GetFourVector(*X3a).P();
  m_PVb = X3b->GetListVisibleFrames().GetFourVector(*X3b).P();
  m_EJa = saJa->GetFourVector(*X3a).E();
  m_EJb = saJb->GetFourVector(*X3b).E();;
  m_PJa = saJa->GetFourVector(*X3a).P();
  m_PJb = saJb->GetFourVector(*X3b).P();
  
  m_MX2a = X2a->GetMass();
  //m_cosX2a = X2a->GetCosDecayAngle();
  m_MX2b = X2b->GetMass();
  //m_cosX2b = X2b->GetCosDecayAngle();
  m_ELa = saLa->GetFourVector(*X2a).E();
  m_ELb = saLb->GetFourVector(*X2b).E();;
  m_PLa = saLa->GetFourVector(*X2a).P();
  m_PLb = saLb->GetFourVector(*X2b).P();
  
  m_MV = S->GetListVisibleFrames().GetMass();
  m_PV = S->GetListVisibleFrames().GetFourVector(*S).P();
  m_MVa = X3a->GetListVisibleFrames().GetMass();
  m_MVb = X3b->GetListVisibleFrames().GetMass();

  m_PV_lab    = S->GetListVisibleFrames().GetFourVector().P();
  m_dphiMET_V = S->GetListVisibleFrames().GetFourVector().Vect().DeltaPhi(ETMiss);
  
  m_MJa = saJa->GetMass();
  m_MJb = saJb->GetMass();
  m_MLa = saLa->GetMass();
  m_MLb = saLb->GetMass();
  m_cosJa = saJa->GetCosDecayAngle();
  m_cosJb = saJb->GetCosDecayAngle();
  m_cosLa = saLa->GetCosDecayAngle();
  m_cosLb = saLb->GetCosDecayAngle();
  
  TLorentzVector vP_S_CM  = S->GetFourVector(*CM);
  TLorentzVector vP_Ja_S  = saJa->GetFourVector(*S);
  TLorentzVector vP_Jb_S  = saJb->GetFourVector(*S);
  TLorentzVector vP_La_S  = saLa->GetFourVector(*S);
  TLorentzVector vP_Lb_S  = saLb->GetFourVector(*S);
  TLorentzVector vP_Ia_S  = X1a->GetFourVector(*S);
  TLorentzVector vP_Ib_S  = X1b->GetFourVector(*S);
  
  m_MJ = (vP_Ja_S+vP_Jb_S).M();
  m_ML = (vP_La_S+vP_Lb_S).M();
  m_EJ = (vP_Ja_S+vP_Jb_S).E();
  m_EL = (vP_La_S+vP_Lb_S).E();
  m_PJ = (vP_Ja_S+vP_Jb_S).P();
  m_PL = (vP_La_S+vP_Lb_S).P();
  
  m_PX3 = (vP_Ja_S+vP_La_S+vP_Ia_S).P();
    
  m_H11S = 2.*(vP_Ia_S+vP_Ib_S).P();
  m_H21S = (vP_Ja_S+vP_La_S).P() +
    (vP_Jb_S+vP_Lb_S).P() + m_H11S/2.;
  m_H22S = (vP_Ja_S+vP_La_S).P() +
    (vP_Jb_S+vP_Lb_S).P() + vP_Ia_S.P() + vP_Ia_S.P();
  m_HT21S = m_H11S/2.
    + S->GetTransverseMomentum(X3a->GetListVisibleFrames().GetFourVector())
    + S->GetTransverseMomentum(X3b->GetListVisibleFrames().GetFourVector());
  m_HT22S = X1a->GetTransverseMomentum(*S) + X1b->GetTransverseMomentum(*S)
    + m_HT21S - m_H11S/2.;
  m_H42S = vP_Ja_S.P() + vP_Jb_S.P() + vP_La_S.P() + vP_Lb_S.P() + vP_Ia_S.P() + vP_Ia_S.P();
  m_HT42S = X1a->GetTransverseMomentum(*S) + X1b->GetTransverseMomentum(*S)
    + Ja->GetTransverseMomentum(*S) + Jb->GetTransverseMomentum(*S)
    + La->GetTransverseMomentum(*S) + Lb->GetTransverseMomentum(*S);
    
  TLorentzVector vP_Ja_X3a  = saJa->GetFourVector(*X3a);
  TLorentzVector vP_Jb_X3b  = saJb->GetFourVector(*X3b);
  TLorentzVector vP_La_X3a  = saLa->GetFourVector(*X3a);
  TLorentzVector vP_Lb_X3b  = saLb->GetFourVector(*X3b);
  TLorentzVector vP_Ia_X3a  = X1a->GetFourVector(*X3a);
  TLorentzVector vP_Ib_X3b  = X1b->GetFourVector(*X3b);
  
  m_H11X3a = 2.*vP_Ia_X3a.P();
  m_H11X3b = 2.*vP_Ib_X3b.P();
  m_H21X3a = vP_Ja_X3a.P() + vP_La_X3a.P() + vP_Ia_X3a.P();
  m_H21X3b = vP_Jb_X3b.P() + vP_Lb_X3b.P() + vP_Ib_X3b.P();

  // removing momentum components parallel to CM->S boost
  TVector3 boostVis = (vP_Ja_S+vP_La_S+vP_Jb_S+vP_Lb_S).BoostVector();
  TVector3 boostInv = (vP_Ia_S+vP_Ib_S).BoostVector();
  TVector3 daBoost = vP_S_CM.Vect().Unit();
  
  if((std::isnan(boostInv.Mag()) || std::isnan(boostVis.Mag())))
    cout << "boost NAN " << boostInv.Mag() << " " << boostVis.Mag() << " Ja=" << vP_Ja_S.P() << " Jb=" << vP_Jb_S.P() << " La=" << vP_La_S.P() << " Lb=" << vP_Lb_S.P() << " Inva=" << vP_Ia_S.P() << " Invb=" << vP_Ib_S.P() << " MET=" << ETMiss.Mag() << endl;
  
  boostVis = (boostVis.Dot(daBoost))*daBoost;
  boostInv = (boostInv.Dot(daBoost))*daBoost;

  if((!std::isnan(boostVis.Mag())) &&
     (boostVis.Mag() < 1)){
    vP_Ja_S.Boost(-boostVis);
    vP_Jb_S.Boost(-boostVis);
    vP_La_S.Boost(-boostVis);
    vP_Lb_S.Boost(-boostVis);
  } else {
    vP_Ja_S.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ja_S.M()));
    vP_Jb_S.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Jb_S.M()));
    vP_La_S.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_La_S.M()));
    vP_Lb_S.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Lb_S.M()));
  }
  if((!std::isnan(boostInv.Mag())) &&
     (boostInv.Mag() < 1)){
    vP_Ia_S.Boost(-boostInv);
    vP_Ib_S.Boost(-boostInv);
  } else {
    vP_Ia_S.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ia_S.M()));
    vP_Ib_S.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ib_S.M()));
  }
    
  m_PX3_BoostT = (vP_Ja_S+vP_La_S+vP_Ia_S).P();
  m_MX3a_BoostT = (vP_Ja_S+vP_La_S+vP_Ia_S).M();
  m_MX3b_BoostT = (vP_Jb_S+vP_Lb_S+vP_Ib_S).M();
  m_Mperp = sqrt(m_MX3a_BoostT*m_MX3a_BoostT+m_MX3b_BoostT*m_MX3b_BoostT)/sqrt(2.);
  m_gammaT = 2*m_Mperp/(sqrt(m_MX3a_BoostT*m_MX3a_BoostT+m_PX3_BoostT*m_PX3_BoostT) +
			sqrt(m_MX3b_BoostT*m_MX3b_BoostT+m_PX3_BoostT*m_PX3_BoostT));

  m_PV_BoostT = (vP_Ja_S+vP_La_S+vP_Jb_S+vP_Lb_S).P();
  
  m_EVa_BoostT = (vP_Ja_S+vP_La_S).E();
  m_EVb_BoostT = (vP_Jb_S+vP_Lb_S).E();
  m_PVa_BoostT = (vP_Ja_S+vP_La_S).P();
  m_PVb_BoostT = (vP_Jb_S+vP_Lb_S).P();

  m_EJ_BoostT = (vP_Ja_S+vP_Jb_S).E();
  m_EL_BoostT = (vP_La_S+vP_Lb_S).E();
  m_PJ_BoostT = (vP_Ja_S+vP_Jb_S).P();
  m_PL_BoostT = (vP_La_S+vP_Lb_S).P();

  // ISR related variables
  if(m_Njet_ISR > 0){
    TVector3 vPISR = S->GetFourVector(*CM).Vect();
    TVector3 vPINV = (X1a->GetFourVector(*CM)+X1b->GetFourVector(*CM)).Vect();

    m_PISR = vPISR.Mag();
    m_MISR = ISR->GetMass();
    m_RISR = fabs(vPINV.Dot(vPISR.Unit())) / vPISR.Mag();

    // transverse
    TVector3 vPTISR = S->GetTransverseFourVector(*CM).Vect();
    TVector3 vPTINV = (X1a->GetTransverseFourVector(*CM)+X1b->GetTransverseFourVector(*CM)).Vect();

    m_PTISR  = vPTISR.Mag();
    m_RISRT  = fabs(vPTINV.Dot(vPTISR.Unit())) / vPTISR.Mag();
    
    if((std::isnan(m_RISR) || std::isnan(m_Mperp))){
      cout << "VAR NAN " << vPTISR.Mag() << " " << vPTINV.Mag() << " NjetS=" << m_Njet_S << " Njeta=" << m_Njet_a << " Njetb=" << m_Njet_b << " Nlep=" << m_Nlep << " Nlepa=" << m_Nlep_a << " Nlepb=" << m_Nlep_b << " " << m_Mperp << endl;
      cout << m_RISR << endl;
    }

    TVector3 isr_t = ISR->GetTransverseFourVector(*S).Vect();
    TVector3 isr   = ISR->GetFourVector(*S).Vect();
    
    for(int l = 0; l < m_Nlep_S; l++){
      TVector3 lep   = S->GetFourVector(Leptons[m_index_lep_S[l]]).Vect();
      TVector3 lep_t = S->GetTransverseFourVector(Leptons[m_index_lep_S[l]]).Vect();
      m_dphi_lep_S.push_back( lep_t.Angle(isr_t) );
      m_cos_lep_S.push_back( lep.Unit().Dot(isr.Unit()) );
      m_dphiMET_lep_S.push_back( Leptons[m_index_lep_S[l]].Vect().DeltaPhi(ETMiss) );
    }
    for(int l = 0; l < m_Njet_S; l++){
      TVector3 jet   = S->GetFourVector(Jets[m_index_jet_S[l]]).Vect();
      TVector3 jet_t = S->GetTransverseFourVector(Jets[m_index_jet_S[l]]).Vect();
      m_dphi_jet_S.push_back( jet_t.Angle(isr_t) );
      m_cos_jet_S.push_back( jet.Unit().Dot(isr.Unit()) );
      m_dphiMET_jet_S.push_back( Jets[m_index_jet_S[l]].Vect().DeltaPhi(ETMiss) );
    }
    for(int l = 0; l < m_NSV_S; l++){
      TVector3 sv   = S->GetFourVector(SVs[m_index_SV_S[l]]).Vect();
      TVector3 sv_t = S->GetTransverseFourVector(SVs[m_index_SV_S[l]]).Vect();
      m_dphi_SV_S.push_back( sv_t.Angle(isr_t) );
      m_cos_SV_S.push_back( sv.Unit().Dot(isr.Unit()) );
      m_dphiMET_SV_S.push_back( SVs[m_index_SV_S[l]].Vect().DeltaPhi(ETMiss) );
    }
  }

  if(!AnalysisBase<Base>::IsData()){
    m_weight = AnalysisBase<Base>::GetEventWeight();
    
    m_PUweight = AnalysisBase<Base>::GetPUWeight(0);
    m_PUweight_up = AnalysisBase<Base>::GetPUWeight(1);
    m_PUweight_down = AnalysisBase<Base>::GetPUWeight(-1);

    m_MuFweight = AnalysisBase<Base>::GetMuFWeight(0);
    m_MuFweight_up = AnalysisBase<Base>::GetMuFWeight(1);
    m_MuFweight_down = AnalysisBase<Base>::GetMuFWeight(-1);

    m_MuRweight = AnalysisBase<Base>::GetMuRWeight(0);
    m_MuRweight_up = AnalysisBase<Base>::GetMuRWeight(1);
    m_MuRweight_down = AnalysisBase<Base>::GetMuRWeight(-1);

    m_PDFweight = AnalysisBase<Base>::GetPDFWeight(0);
    m_PDFweight_up = AnalysisBase<Base>::GetPDFWeight(1);
    m_PDFweight_down = AnalysisBase<Base>::GetPDFWeight(-1);
    
    m_BtagHFSFweight = AnalysisBase<Base>::GetBtagSFWeight(Jets, true, 0, kMedium);
    m_BtagHFSFweight_up = AnalysisBase<Base>::GetBtagSFWeight(Jets, true, 1, kMedium);
    m_BtagHFSFweight_down = AnalysisBase<Base>::GetBtagSFWeight(Jets, true, -1, kMedium);
    m_BtagLFSFweight = AnalysisBase<Base>::GetBtagSFWeight(Jets, false, 0, kMedium);
    m_BtagLFSFweight_up = AnalysisBase<Base>::GetBtagSFWeight(Jets, false, 1, kMedium);
    m_BtagLFSFweight_down = AnalysisBase<Base>::GetBtagSFWeight(Jets, false, -1, kMedium);

    m_elIDSFweight = AnalysisBase<Base>::GetElIDSFWeight(Electrons, 0);
    m_elIDSFweight_up = AnalysisBase<Base>::GetElIDSFWeight(Electrons, 1);
    m_elIDSFweight_down = AnalysisBase<Base>::GetElIDSFWeight(Electrons, -1);
    m_elISOSFweight = AnalysisBase<Base>::GetElISOSFWeight(Electrons, 0);
    m_elISOSFweight_up = AnalysisBase<Base>::GetElISOSFWeight(Electrons, 1);
    m_elISOSFweight_down = AnalysisBase<Base>::GetElISOSFWeight(Electrons, -1);
    m_elSIPSFweight = AnalysisBase<Base>::GetElSIPSFWeight(Electrons, 0);
    m_elSIPSFweight_up = AnalysisBase<Base>::GetElSIPSFWeight(Electrons, 1);
    m_elSIPSFweight_down = AnalysisBase<Base>::GetElSIPSFWeight(Electrons, -1);
    m_elVLSFweight = AnalysisBase<Base>::GetElVLIDSFWeight(Electrons, 0);
    m_elVLSFweight_up = AnalysisBase<Base>::GetElVLIDSFWeight(Electrons, 1);
    m_elVLSFweight_down = AnalysisBase<Base>::GetElVLIDSFWeight(Electrons, -1);
    m_muIDSFweight = AnalysisBase<Base>::GetMuIDSFWeight(Muons, 0);
    m_muIDSFweight_up = AnalysisBase<Base>::GetMuIDSFWeight(Muons, 1);
    m_muIDSFweight_down = AnalysisBase<Base>::GetMuIDSFWeight(Muons, -1);
    m_muISOSFweight = AnalysisBase<Base>::GetMuISOSFWeight(Muons, 0);
    m_muISOSFweight_up = AnalysisBase<Base>::GetMuISOSFWeight(Muons, 1);
    m_muISOSFweight_down = AnalysisBase<Base>::GetMuISOSFWeight(Muons, -1);
    m_muSIPSFweight = AnalysisBase<Base>::GetMuSIPSFWeight(Muons, 0);
    m_muSIPSFweight_up = AnalysisBase<Base>::GetMuSIPSFWeight(Muons, 1);
    m_muSIPSFweight_down = AnalysisBase<Base>::GetMuSIPSFWeight(Muons, -1);
    m_muVLSFweight = AnalysisBase<Base>::GetMuVLIDSFWeight(Muons, 0);
    m_muVLSFweight_up = AnalysisBase<Base>::GetMuVLIDSFWeight(Muons, 1);
    m_muVLSFweight_down = AnalysisBase<Base>::GetMuVLIDSFWeight(Muons, -1);

    m_MetTrigSFweight = AnalysisBase<Base>::GetMETTriggerSFWeight(m_MET, m_HT_eta5, m_Nele, m_Nmu, 0);
    m_MetTrigSFweight_up = AnalysisBase<Base>::GetMETTriggerSFWeight(m_MET, m_HT_eta5, m_Nele, m_Nmu, 1);
    m_MetTrigSFweight_down = AnalysisBase<Base>::GetMETTriggerSFWeight(m_MET, m_HT_eta5, m_Nele, m_Nmu, -1);
    m_MetTrigSFCurveIndex = AnalysisBase<Base>::GetMETTriggerSFCurve(m_HT_eta5, m_Nele, m_Nmu);
   
    m_NPU = AnalysisBase<Base>::GetNPUtrue();

    TVector3 genETMiss = AnalysisBase<Base>::GetGenMET();
    m_genMET     = genETMiss.Pt();
    m_genMET_phi = genETMiss.Phi();
  } else {
    m_weight = 1;
    m_PUweight = 1;
    m_PUweight_up = 1;
    m_PUweight_down = 1;
    m_MuFweight = 1;
    m_MuFweight_up = 1;
    m_MuFweight_down = 1;
    m_MuRweight = 1;
    m_MuRweight_up = 1;
    m_MuRweight_down = 1;
    m_PDFweight = 1;
    m_PDFweight_up = 1;
    m_PDFweight_down = 1;
    m_BtagHFSFweight = 1;
    m_BtagHFSFweight_up = 1;
    m_BtagHFSFweight_down = 1;
    m_BtagLFSFweight = 1;
    m_BtagLFSFweight_up = 1;
    m_BtagLFSFweight_down = 1;
    m_elIDSFweight = 1;
  m_elIDSFweight_up = 1;
  m_elIDSFweight_down = 1;
  m_elISOSFweight = 1;
  m_elISOSFweight_up = 1;
  m_elISOSFweight_down = 1;
  m_elSIPSFweight = 1;
  m_elSIPSFweight_up = 1;
  m_elSIPSFweight_down = 1;
  m_elVLSFweight = 1;
  m_elVLSFweight_up = 1;
  m_elVLSFweight_down = 1;
  m_muIDSFweight = 1;
  m_muIDSFweight_up = 1;
  m_muIDSFweight_down = 1;
  m_muISOSFweight = 1;
  m_muISOSFweight_up = 1;
  m_muISOSFweight_down = 1;
  m_muSIPSFweight = 1;
  m_muSIPSFweight_up = 1;
  m_muSIPSFweight_down = 1;
  m_muVLSFweight = 1;
  m_muVLSFweight_up = 1;
  m_muVLSFweight_down = 1;
    m_MetTrigSFweight = 1.;
    m_MetTrigSFweight_up = 1.;
    m_MetTrigSFweight_down = 1.;
    m_MetTrigSFCurveIndex = 0;

    m_NPU = 0.;
  }
  
  m_runnum   = AnalysisBase<Base>::GetRunNum();
  m_luminum  = AnalysisBase<Base>::GetLumiNum();
  m_eventnum = AnalysisBase<Base>::GetEventNum();

  m_NPV = AnalysisBase<Base>::GetNPV();

  m_METtrigger   = AnalysisBase<Base>::GetMETtrigger();
  m_METHTtrigger = AnalysisBase<Base>::GetMETHTtrigger();
  m_METORtrigger = AnalysisBase<Base>::GetMETORtrigger();

  m_SingleElectrontrigger = AnalysisBase<Base>::GetSingleElectrontrigger();
  m_SingleMuontrigger = AnalysisBase<Base>::GetSingleMuontrigger();
  m_DoubleElectrontrigger = AnalysisBase<Base>::GetDoubleElectrontrigger();
  m_DoubleMuontrigger = AnalysisBase<Base>::GetDoubleMuontrigger();
  m_EMutrigger = AnalysisBase<Base>::GetEMutrigger(); 
  
  m_MET     = ETMiss.Pt();
  m_MET_phi = ETMiss.Phi();

  // Fill Jets
  if(!do_slim){
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

    // Fill GenJets
    vector<int> genmatch_jet;
    for(int i = 0; i < m_NGenjet; i++)
      genmatch_jet.push_back(-1);
    m_PT_Genjet.clear();
    m_Eta_Genjet.clear();
    m_Phi_Genjet.clear();
    m_M_Genjet.clear();
    m_Index_jet.clear();
    for(int i = 0; i < m_NGenjet; i++){
      m_PT_Genjet.push_back(GenJets[i].Pt());
      m_Eta_Genjet.push_back(GenJets[i].Eta());
      m_Phi_Genjet.push_back(GenJets[i].Phi());
      m_M_Genjet.push_back(GenJets[i].M());
      int index = -1;
      double minDR = 0.1;
      for(int g = 0; g < m_NGenjet; g++)
	if(Jets[i].DeltaR(GenJets[g]) < minDR){
	  minDR = Jets[i].DeltaR(GenJets[g]);
	  index = g;
	  genmatch_jet[g] = i;
	}
      m_Index_jet.push_back(index);
    }
  }

  // Fill SVs
  m_PT_SV.clear();
  m_Eta_SV.clear();
  m_Phi_SV.clear();
  m_M_SV.clear();
  m_ProbB_SV.clear();
  m_ProbC_SV.clear();
  for(int i = 0; i < m_NSV; i++){
    m_PT_SV.push_back(SVs[i].Pt());
    m_Eta_SV.push_back(SVs[i].Eta());
    m_Phi_SV.push_back(SVs[i].Phi());
    m_M_SV.push_back(SVs[i].M());
    m_ProbB_SV.push_back(SVs[i].ProbB());
    m_ProbC_SV.push_back(SVs[i].ProbC());
  }
  
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
  m_SourceID_lep.clear();
  m_LepQual_lep.clear();
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
    m_LepQual_lep.push_back(Leptons[r].LepQual());
    int index = -1;
    double minDR = 0.1;
    for(int g = 0; g < m_genNlep; g++)
      if(Leptons[r].DeltaR(GenLeptons[g]) < minDR){
	minDR = Leptons[r].DeltaR(GenLeptons[g]);
	index = g;
	genmatch[g] = r;
      }
 
    m_Index_lep.push_back(index);
    if(index >= 0)
      Leptons[r].SetSourceID(GenLeptons[index].SourceID());
    else
      Leptons[r].SetSourceID(kFake);
    
    // m_ID_lep.push_back(Leptons[r].SourceID());
    m_SourceID_lep.push_back(Leptons[r].SourceID());
  }

  if(!AnalysisBase<Base>::IsData()){
    // Fill gen lepton branches
    m_genPT_lep.clear();
    m_genEta_lep.clear();
    m_genPhi_lep.clear();
    m_genM_lep.clear();
    m_genCharge_lep.clear();
    m_genPDGID_lep.clear();
    m_genMomPDGID_lep.clear();
    m_genSourceID_lep.clear();
    m_genIndex_lep.clear();
    for(int g = 0; g < m_genNlep; g++){
      m_genPT_lep.push_back(GenLeptons[g].Pt());
      m_genEta_lep.push_back(GenLeptons[g].Eta());
      m_genPhi_lep.push_back(GenLeptons[g].Phi());
      m_genM_lep.push_back(GenLeptons[g].M());
      m_genCharge_lep.push_back(GenLeptons[g].Charge());
      m_genPDGID_lep.push_back(GenLeptons[g].PDGID());
      m_genMomPDGID_lep.push_back(GenLeptons[g].MomPDGID());
      m_genSourceID_lep.push_back(GenLeptons[g].SourceID());
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
      m_genM_boson.push_back(GenBosons[i].M());
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
      m_genM_susy.push_back(GenSparticles[i].M());
      m_genPDGID_susy.push_back(GenSparticles[i].PDGID());
      m_genMomPDGID_susy.push_back(GenSparticles[i].MomPDGID());
    }
  }
  
  // Fill output tree
  if(tree)
    tree->Fill();
}

template class ReducedNtuple<StopNtupleTree>;
template class ReducedNtuple<SUSYNANOBase>;
