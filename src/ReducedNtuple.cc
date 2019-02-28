#include "ReducedNtuple.hh"
#include "ParticleList.hh"

using namespace RestFrames;

ReducedNtuple::ReducedNtuple(TTree* tree)
  : NtupleBase<StopNtupleTree>(tree)
{
  // RestFrames stuff

  // combinatoric (transverse) tree
  // for jet assignment
  LAB_comb = new LabRecoFrame("LAB_comb","LAB");
  CM_comb  = new DecayRecoFrame("CM_comb","CM");
  S_comb   = new DecayRecoFrame("S_comb","S");
  ISR_comb = new VisibleRecoFrame("ISR_comb","ISR");
  J_comb   = new VisibleRecoFrame("J_comb","Jets");
  L_comb   = new VisibleRecoFrame("L_comb","#it{l}'s");
  I_comb   = new InvisibleRecoFrame("I_comb","Inv");
  
  LAB_comb->SetChildFrame(*CM_comb);
  CM_comb->AddChildFrame(*ISR_comb);
  CM_comb->AddChildFrame(*S_comb);
  S_comb->AddChildFrame(*L_comb);
  S_comb->AddChildFrame(*J_comb);
  S_comb->AddChildFrame(*I_comb);
  
  LAB_comb->InitializeTree();

  // OS 2L tree w/ fixed jet assign.
  LAB_fix = new LabRecoFrame("LAB_fix","LAB");
  CM_fix  = new DecayRecoFrame("CM_fix","CM");
  S_fix   = new DecayRecoFrame("S_fix","S");
  ISR_fix = new VisibleRecoFrame("ISR_fix","ISR");
  L_fix   = new DecayRecoFrame("L_fix","#it{ll}");
  L1_fix  = new VisibleRecoFrame("L1_fix","#it{l}_{1}");
  L2_fix  = new VisibleRecoFrame("L2_fix","#it{l}_{2}");
  I_fix   = new InvisibleRecoFrame("I_fix","I");

  LAB_fix->SetChildFrame(*CM_fix);
  CM_fix->AddChildFrame(*ISR_fix);
  CM_fix->AddChildFrame(*S_fix);
  S_fix->AddChildFrame(*L_fix);
  S_fix->AddChildFrame(*I_fix);
  L_fix->AddChildFrame(*L1_fix);
  L_fix->AddChildFrame(*L2_fix);
  
  LAB_fix->InitializeTree();

  // 2L+NJ tree (Z->ll + W/Z->qq)
  LAB_2LNJ = new LabRecoFrame("LAB_2LNJ","LAB");
  
  S_2LNJ   = new DecayRecoFrame("S_2LNJ","S");
  
  Ca_2LNJ  = new DecayRecoFrame("Ca_2LNJ","C_{a}");  
  Z_2LNJ   = new DecayRecoFrame("Z_2LNJ","Z");
  L1_2LNJ  = new VisibleRecoFrame("L1_2LNJ","#it{l}_{1}");
  L2_2LNJ  = new VisibleRecoFrame("L2_2LNJ","#it{l}_{2}");
  Cb_2LNJ  = new DecayRecoFrame("Cb_2LNJ","C_{b}");
  JSA_2LNJ = new SelfAssemblingRecoFrame("JSA_2L2J", "J");
  J_2LNJ   = new VisibleRecoFrame("J_2L2J","Jets");
  Ia_2LNJ  = new InvisibleRecoFrame("Ia_2LNJ","I_{a}");
  Ib_2LNJ  = new InvisibleRecoFrame("Ib_2LNJ","I_{b}");

  LAB_2LNJ->SetChildFrame(*S_2LNJ);
  S_2LNJ->AddChildFrame(*Ca_2LNJ);
  S_2LNJ->AddChildFrame(*Cb_2LNJ);
  Ca_2LNJ->AddChildFrame(*Z_2LNJ);
  Ca_2LNJ->AddChildFrame(*Ia_2LNJ);
  Cb_2LNJ->AddChildFrame(*JSA_2LNJ);
  Cb_2LNJ->AddChildFrame(*Ib_2LNJ);
  Z_2LNJ->AddChildFrame(*L1_2LNJ);
  Z_2LNJ->AddChildFrame(*L2_2LNJ);
  JSA_2LNJ->AddChildFrame(*J_2LNJ);

  LAB_2LNJ->InitializeTree();

  // 2L+1L tree (Z->ll + Z/W->l)
  LAB_2L1L = new LabRecoFrame("LAB_2L1L","LAB");
 
  S_2L1L   = new DecayRecoFrame("S_2L1L","S");
  
  Ca_2L1L  = new DecayRecoFrame("Ca_2L1L","C_{a}");
  Z_2L1L   = new DecayRecoFrame("Z_2L1L","Z");  
  L1_2L1L  = new VisibleRecoFrame("L1_2L1L","#it{l}_{1}");
  L2_2L1L  = new VisibleRecoFrame("L2_2L1L","#it{l}_{2}");
  Cb_2L1L  = new DecayRecoFrame("Cb_2L1L","C_{b}");  
  Lb_2L1L  = new VisibleRecoFrame("Lb_2L1L","#it{l}_{b}");
  Ia_2L1L  = new InvisibleRecoFrame("Ia_2L1L","I_{a}");
  Ib_2L1L  = new InvisibleRecoFrame("Ia_2L1L","I_{b}");

  LAB_2L1L->SetChildFrame(*S_2L1L);
 
  S_2L1L->AddChildFrame(*Ca_2L1L);
  S_2L1L->AddChildFrame(*Cb_2L1L);
  Ca_2L1L->AddChildFrame(*Z_2L1L);
  Ca_2L1L->AddChildFrame(*Ia_2L1L);
  Z_2L1L->AddChildFrame(*L1_2L1L);
  Z_2L1L->AddChildFrame(*L2_2L1L);
  Cb_2L1L->AddChildFrame(*Lb_2L1L);
  Cb_2L1L->AddChildFrame(*Ib_2L1L);

  LAB_2L1L->InitializeTree(); 

   ////////////// Jigsaw rules set-up /////////////////

  // combinatoric (transverse) tree
  // for jet assignment
  INV_comb = new InvisibleGroup("INV_comb","Invisible System");
  INV_comb->AddFrame(*I_comb);
  
  InvMass_comb = new SetMassInvJigsaw("InvMass_comb", "Invisible system mass Jigsaw");
  INV_comb->AddJigsaw(*InvMass_comb);
  
  JETS_comb = new CombinatoricGroup("JETS_comb","Jets System");
  JETS_comb->AddFrame(*ISR_comb);
  JETS_comb->SetNElementsForFrame(*ISR_comb, 1);
  JETS_comb->AddFrame(*J_comb);
  JETS_comb->SetNElementsForFrame(*J_comb, 0);
  
  SplitJETS_comb = new MinMassesCombJigsaw("SplitJETS_comb", "Minimize M_{ISR} and M_{S} Jigsaw");
  JETS_comb->AddJigsaw(*SplitJETS_comb);
  SplitJETS_comb->AddCombFrame(*ISR_comb, 0);
  SplitJETS_comb->AddCombFrame(*J_comb, 1);
  SplitJETS_comb->AddObjectFrame(*ISR_comb,0);
  SplitJETS_comb->AddObjectFrame(*S_comb,1);

  if(!LAB_comb->InitializeAnalysis()){
    cout << "Problem initializing \"comb\" analysis" << endl;
  }

  // OS 2L tree w/ fixed jet assign.
  INV_fix = new InvisibleGroup("INV_fix","Invisible System");
  INV_fix->AddFrame(*I_fix);
  
  InvMass_fix = new SetMassInvJigsaw("InvMass_fix", "Invisible system mass Jigsaw");
  INV_fix->AddJigsaw(*InvMass_fix);
  InvRapidity_fix = new SetRapidityInvJigsaw("InvRapidity_fix", "Set inv. system rapidity");
  INV_fix->AddJigsaw(*InvRapidity_fix);
  InvRapidity_fix->AddVisibleFrames(S_fix->GetListVisibleFrames());

  if(!LAB_fix->InitializeAnalysis()){
    cout << "Problem initializing \"fix\" analysis" << endl;
  }

  // 2L+NJ tree (Z->ll + W/Z->qq)
  INV_2LNJ = new InvisibleGroup("INV_2LNJ","Invisible System");
  INV_2LNJ->AddFrame(*Ia_2LNJ);
  INV_2LNJ->AddFrame(*Ib_2LNJ);
  
  InvMass_2LNJ = new SetMassInvJigsaw("InvMass_2LNJ", "Invisible system mass Jigsaw");
  INV_2LNJ->AddJigsaw(*InvMass_2LNJ);
  InvRapidity_2LNJ = new SetRapidityInvJigsaw("InvRapidity_2LNJ", "Set inv. system rapidity");
  INV_2LNJ->AddJigsaw(*InvRapidity_2LNJ);
  InvRapidity_2LNJ->AddVisibleFrames(S_2LNJ->GetListVisibleFrames());
  SplitINV_2LNJ = new ContraBoostInvJigsaw("SplitINV_2LNJ", "INV -> I_{a}+ I_{b} jigsaw");
  INV_2LNJ->AddJigsaw(*SplitINV_2LNJ);
  SplitINV_2LNJ->AddVisibleFrames(Ca_2LNJ->GetListVisibleFrames(), 0);
  SplitINV_2LNJ->AddVisibleFrames(Cb_2LNJ->GetListVisibleFrames(), 1);
  SplitINV_2LNJ->AddInvisibleFrame(*Ia_2LNJ, 0);
  SplitINV_2LNJ->AddInvisibleFrame(*Ib_2LNJ, 1);
  
  JETS_2LNJ = new CombinatoricGroup("JETS_comb","Jets System");
  JETS_2LNJ->AddFrame(*J_2LNJ);
  JETS_2LNJ->SetNElementsForFrame(*J_2LNJ, 0);

  if(!LAB_2LNJ->InitializeAnalysis()){
    cout << "Problem initializing \"2LNJ\" analysis" << endl;
  }

  // 2L+1L tree (Z->ll + Z/W->l)
  INV_2L1L = new InvisibleGroup("INV_2L1L","Invisible System");
  INV_2L1L->AddFrame(*Ia_2L1L);
  INV_2L1L->AddFrame(*Ib_2L1L);
  
  InvMass_2L1L = new SetMassInvJigsaw("InvMass_2L1L", "Invisible system mass Jigsaw");
  INV_2L1L->AddJigsaw(*InvMass_2L1L);
  InvRapidity_2L1L = new SetRapidityInvJigsaw("InvRapidity_2L1L", "Set inv. system rapidity");
  INV_2L1L->AddJigsaw(*InvRapidity_2L1L);
  InvRapidity_2L1L->AddVisibleFrames(S_2L1L->GetListVisibleFrames());
  SplitINV_2L1L = new ContraBoostInvJigsaw("SplitINV_2L1L", "INV -> I_{a}+ I_{b} jigsaw");
  INV_2L1L->AddJigsaw(*SplitINV_2L1L);
  SplitINV_2L1L->AddVisibleFrames(Ca_2L1L->GetListVisibleFrames(), 0);
  SplitINV_2L1L->AddVisibleFrames(Cb_2L1L->GetListVisibleFrames(), 1);
  SplitINV_2L1L->AddInvisibleFrame(*Ia_2L1L, 0);
  SplitINV_2L1L->AddInvisibleFrame(*Ib_2L1L, 1);

  if(!LAB_2L1L->InitializeAnalysis()){
    cout << "Problem initializing \"2L1L\" analysis" << endl;
  }
}

ReducedNtuple::~ReducedNtuple() {
  // combinatoric (transverse) tree
  // for jet assignment
  delete LAB_comb;
  delete CM_comb;
  delete S_comb;
  delete ISR_comb;
  delete J_comb;
  delete L_comb;
  delete I_comb;
  delete INV_comb;
  delete InvMass_comb;
  delete JETS_comb;
  delete SplitJETS_comb;

  // OS 2L tree w/ fixed jet assign.
  delete LAB_fix;
  delete CM_fix;
  delete S_fix;
  delete ISR_fix;

  delete L_fix;  
  delete L1_fix;
  delete L2_fix;
 
  delete I_fix;

  delete INV_fix;
  delete InvMass_fix;
  delete InvRapidity_fix;

  // 2L+NJ tree (Z->ll + W/Z->qq)
  delete LAB_2LNJ;
 
  delete S_2LNJ;
 
  delete Ca_2LNJ;  
  delete Z_2LNJ;
  delete L1_2LNJ;
  delete L2_2LNJ;
  delete Cb_2LNJ;
  delete JSA_2LNJ;
  delete J_2LNJ;
  delete Ia_2LNJ;
  delete Ib_2LNJ;
  delete INV_2LNJ;
  delete InvMass_2LNJ;
  delete InvRapidity_2LNJ;
  delete SplitINV_2LNJ;
  delete JETS_2LNJ;

  // 2L+1L tree (Z->ll + Z/W->l)
  delete LAB_2L1L;
  
  delete S_2L1L;
  
  delete Ca_2L1L;
  delete Z_2L1L;  
  delete L1_2L1L;
  delete L2_2L1L;
  delete Cb_2L1L;  
  delete Lb_2L1L;
  delete Ia_2L1L;
  delete Ib_2L1L;
  delete INV_2L1L;
  delete InvMass_2L1L;
  delete InvRapidity_2L1L;
  delete SplitINV_2L1L;
  
}

TTree* ReducedNtuple::InitOutputTree(const string& sample){

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

  tree->Branch("Nbjet", &m_Nbjet);
 
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
  tree->Branch("genPDGID_susy",   &m_genPDGID_susy);

  // Calculated Observables
  tree->Branch("Nj", &m_Nj);
  tree->Branch("NjS", &m_NjS);
  tree->Branch("NjISR", &m_NjISR);

  tree->Branch("PTCM_comb", &m_PTCM_comb);
  tree->Branch("PTISR_comb", &m_PTISR_comb);
  tree->Branch("RISR_comb", &m_RISR_comb);
  tree->Branch("cosCM_comb", &m_cosCM_comb);
  tree->Branch("cosS_comb", &m_cosS_comb);
  tree->Branch("MISR_comb", &m_MISR_comb);
  tree->Branch("MS_comb", &m_MS_comb);
  tree->Branch("dphiCMI_comb", &m_dphiCMI_comb);
  tree->Branch("dphiSI_comb", &m_dphiSI_comb);
  tree->Branch("dphiISRI_comb", &m_dphiISRI_comb);

  tree->Branch("PTCM_fix", &m_PTCM_fix);
  tree->Branch("PTISR_fix", &m_PTISR_fix);
  tree->Branch("RISR_fix", &m_RISR_fix);
  tree->Branch("cosCM_fix", &m_cosCM_fix);
  tree->Branch("cosS_fix", &m_cosS_fix);
  tree->Branch("MISR_fix", &m_MISR_fix);
  tree->Branch("MS_fix", &m_MS_fix);
  tree->Branch("dphiCMI_fix", &m_dphiCMI_fix);
  tree->Branch("dphiSI_fix", &m_dphiSI_fix);
  tree->Branch("dphiISRI_fix", &m_dphiISRI_fix);

  tree->Branch("MZ", &m_MZ);
  tree->Branch("cosZ", &m_cosZ);

  // which tree are we using for event?
 
  tree->Branch("Is_2LNJ", &m_Is_2LNJ);
  tree->Branch("Is_2L1L", &m_Is_2L1L);
  
  tree->Branch("HN2S", &m_HN2S);
  tree->Branch("HN2SR", &m_HN2SR);
  tree->Branch("H11S", &m_H11S);
  tree->Branch("HN1Ca", &m_HN1Ca);
  tree->Branch("HN1Cb", &m_HN1Cb);
  tree->Branch("H11Ca", &m_H11Ca);
  tree->Branch("H11Cb", &m_H11Cb);
  tree->Branch("cosC", &m_cosC);

  tree->Branch("MZ", &m_MZ);
  tree->Branch("MJ", &m_MJ);
  tree->Branch("cosZ", &m_cosZ);
  tree->Branch("cosJ", &m_cosJ);

  return tree;
}

void ReducedNtuple::FillOutputTree(TTree* tree){

  ParticleList Jets = GetJets();
  Jets = Jets.PtEtaCut(30., 3.);
  
  m_Nj = Jets.size();
  
  TVector3 ETMiss = GetMET();

  if(ETMiss.Mag() < 50.)
    return;

  ParticleList Muons = GetMuons();
  Muons = Muons.ParticleIDCut(kMedium);
  Muons = Muons.PtEtaCut(3.5);

  ParticleList Electrons = GetElectrons();
  Electrons = Electrons.ParticleIDCut(kMedium);
  Electrons = Electrons.PtEtaCut(3.5);
  
  ParticleList Leptons = Electrons+Muons;
  Leptons.SortByPt();

  // require at least one lepton for now
  if(Leptons.size() < 1)
    return;
  
  // figure out which tree to use
  
  m_Is_2LNJ = false;
  m_Is_2L1L = false;
  
  
  if(Leptons.size() < 2) // at least 2 leptons for now
    return;

  if(Leptons.size() == 3){
    // at least 1 OS/SF pair
    if(Leptons[0].Charge()*Leptons[0].PDGID()+Leptons[1].Charge()*Leptons[1].PDGID() == 0 ||
       Leptons[0].Charge()*Leptons[0].PDGID()+Leptons[2].Charge()*Leptons[2].PDGID() == 0 ||
       Leptons[2].Charge()*Leptons[2].PDGID()+Leptons[1].Charge()*Leptons[1].PDGID() == 0){
      m_Is_2L1L = true;
    }
  }

  if(Leptons.size() == 2){
    // SS and/or OF leptons
    if(Leptons[0].Charge()*Leptons[0].PDGID()+Leptons[1].Charge()*Leptons[1].PDGID() == 0){
      if(Jets.size() >= 1){
	m_Is_2LNJ = true;
      }
    }
  }

  if(!m_Is_2LNJ && !m_Is_2L1L)
    return;

  if(Leptons[0].Pt() < 5. && Leptons[1].Pt() < 5.) // lead leptons greater than 5 GeV in Pt
    return;

  // 2LNJ analysis
  if(m_Is_2LNJ){
    LAB_2LNJ->ClearEvent();

    // put jets in their place
    int NJ = Jets.size();
    if(NJ == 1)
      JETS_2LNJ->AddLabFrameFourVector(Jets[0]);
    else {
      int i1,i2;
      double mdiff = 100000.;
      for(int i = 0; i < NJ-1; i++){
	for(int j = i+1; j < NJ; j++){
	  double diff = fabs(TLorentzVector(Jets[i]+Jets[j]).M() - 80.);
	  if(diff < mdiff){
	    mdiff = diff;
	    i1 = i;
	    i2 = j;
	  }
	}
      }
      JETS_2LNJ->AddLabFrameFourVector(Jets[i1]);
      JETS_2LNJ->AddLabFrameFourVector(Jets[i2]);
      
    }
    
    // put leptons in their place
    L1_2LNJ->SetLabFrameFourVector(Leptons[0]);
    L2_2LNJ->SetLabFrameFourVector(Leptons[1]);

    INV_2LNJ->SetLabFrameThreeVector(ETMiss);

    if(!LAB_2LNJ->AnalyzeEvent())
      cout << "Something went wrong with \"2LNJ\" tree event analysis" << endl;
  }

  // 2L1L analysis
  if(m_Is_2L1L){
    LAB_2L1L->ClearEvent();
    
    // put leptons in their place
    // find min mass SF/OS pair
    pair<int,int> iSFOS;
    double        mSFOS = -1.;
    for(int i = 0; i < 2; i++){
      for(int j = i+1; j < 3; j++){
	if(Leptons[i].Charge()*Leptons[i].PDGID()+Leptons[j].Charge()*Leptons[j].PDGID() == 0){
	  if(mSFOS < 0. ||
	     TLorentzVector(Leptons[i]+Leptons[j]).M() < mSFOS){
	    mSFOS = TLorentzVector(Leptons).M();
	    iSFOS.first  = i;
	    iSFOS.second = j;
	  }
	}
      }
    }
    
    for(int i = 0; i < 3; i++){
      if(i == iSFOS.first)
	L1_2L1L->SetLabFrameFourVector(Leptons[i]);
      if(i == iSFOS.second)
	L2_2L1L->SetLabFrameFourVector(Leptons[i]);
      if(i != iSFOS.first && i != iSFOS.second)
	Lb_2L1L->SetLabFrameFourVector(Leptons[i]);
    }

    INV_2L1L->SetLabFrameThreeVector(ETMiss);
    
    if(!LAB_2L1L->AnalyzeEvent())
      cout << "Something went wrong with \"2L1L\" tree event analysis" << endl;
  }

  m_HN2S = 0.;
  m_HN2SR = 0.;
  m_H11S = 0.;
  m_HN1Ca = 0.;
  m_HN1Cb = 0.;
  m_H11Ca = 0.;
  m_H11Cb = 0.;
  m_cosC = 0.;

  m_MZ = 0.;
  m_MJ = 0.;
  m_cosZ = 0.;
  m_cosJ = 0.;

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

  // ISR analysis
  if(Jets.size() < 1)
    return;
  
  // first - analyze jet combinatoric tree
  // (regardless of later tree)
  LAB_comb->ClearEvent();

  TLorentzVector JetTOT(0.,0.,0.,0.);
  m_HT = 0.;
  
  vector<RFKey> jetID; 
  for(int i = 0; i < int(Jets.size()); i++){
    TLorentzVector jet = Jets[i];
    m_HT += jet.Pt();
    JetTOT += jet;
    
    // transverse view of jet 4-vectors
    jet.SetPtEtaPhiM(jet.Pt(),0.0,jet.Phi(),jet.M());
    jetID.push_back(JETS_comb->AddLabFrameFourVector(jet));
  }

  TLorentzVector lepSys(0.,0.,0.,0.);
  for(int i = 0; i < int(Leptons.size()); i++){
    TLorentzVector lep1;
    // transverse view of mu 4-vectors
    lep1.SetPtEtaPhiM(Leptons[i].Pt(),0.0,Leptons[i].Phi(),Leptons[i].M());
    lepSys = lepSys + lep1;
  }  
  L_comb->SetLabFrameFourVector(lepSys);

  INV_comb->SetLabFrameThreeVector(ETMiss);
  if(!LAB_comb->AnalyzeEvent())
    cout << "Something went wrong with \"comb\" tree event analysis" << endl;


  // Jet counting from comb tree
  m_NjS   = 0;
  m_NjISR = 0;
 
  for(int i = 0; i < int(Jets.size()); i++){
    if(JETS_comb->GetFrame(jetID[i]) == *J_comb){ // sparticle group
      m_NjS++;
    } else {
      m_NjISR++;
    }
  }


  // fixed tree analysis
  LAB_fix->ClearEvent();


  ISR_fix->SetLabFrameFourVector(JetTOT);
  // put leptons in their place
  L1_fix->SetLabFrameFourVector(Leptons[0]);
  L2_fix->SetLabFrameFourVector(Leptons[1]);

  INV_fix->SetLabFrameThreeVector(ETMiss);
    
  if(!LAB_fix->AnalyzeEvent())
    cout << "Something went wrong with \"fix\" tree event analysis" << endl;

   
  m_cosCM_comb = CM_comb->GetCosDecayAngle();
  m_cosS_comb  = S_comb->GetCosDecayAngle();
  m_MISR_comb = ISR_comb->GetMass();
  m_MS_comb   = S_comb->GetCosDecayAngle();
  m_dphiCMI_comb = acos(-1.)-fabs(CM_comb->GetDeltaPhiBoostVisible());
  m_dphiSI_comb  = acos(-1.)-fabs(S_comb->GetDeltaPhiBoostVisible());

  m_cosCM_fix = CM_fix->GetCosDecayAngle();
  m_cosS_fix  = S_fix->GetCosDecayAngle();
  m_MISR_fix = ISR_fix->GetMass();
  m_MS_fix   = S_fix->GetCosDecayAngle();
  m_dphiCMI_fix = acos(-1.)-fabs(CM_fix->GetDeltaPhiBoostVisible());
  m_dphiSI_fix  = acos(-1.)-fabs(S_fix->GetDeltaPhiBoostVisible());

  m_MZ = L_fix->GetMass();
  m_cosZ = L_fix->GetCosDecayAngle();
  for(;;){ //comb tree
    TLorentzVector vP_CM  = CM_comb->GetFourVector();
    TLorentzVector vP_ISR = ISR_comb->GetFourVector();
    TLorentzVector vP_I   = I_comb->GetFourVector();
    
    m_PTCM_comb = vP_CM.Pt();
    
    TVector3 boostZ = vP_CM.BoostVector();
    boostZ.SetX(0.);
    boostZ.SetY(0.);
    
    vP_CM.Boost(-boostZ);
    vP_ISR.Boost(-boostZ);
    vP_I.Boost(-boostZ);
    
    TVector3 boostT = vP_CM.BoostVector();
    vP_ISR.Boost(-boostT);
    vP_I.Boost(-boostT);
    
    TVector3 vPt_ISR = vP_ISR.Vect();
    TVector3 vPt_I   = vP_I.Vect();
    vPt_ISR -= vPt_ISR.Dot(boostZ.Unit())*boostZ.Unit();
    vPt_I   -= vPt_I.Dot(boostZ.Unit())*boostZ.Unit();
    
    m_PTISR_comb =  vPt_ISR.Mag();
    m_RISR_comb  = -vPt_I.Dot(vPt_ISR.Unit()) / m_PTISR_comb;
    m_dphiISRI_comb = fabs(vPt_ISR.Angle(vPt_I));

    break;
  }

  for(;;){ // fixed jet tree
    TLorentzVector vP_CM  = CM_fix->GetFourVector();
    TLorentzVector vP_ISR = ISR_fix->GetFourVector();
    TLorentzVector vP_I   = I_fix->GetFourVector();
    
    m_PTCM_fix = vP_CM.Pt();
    
    TVector3 boostZ = vP_CM.BoostVector();
    boostZ.SetX(0.);
    boostZ.SetY(0.);
    
    vP_CM.Boost(-boostZ);
    vP_ISR.Boost(-boostZ);
    vP_I.Boost(-boostZ);
    
    TVector3 boostT = vP_CM.BoostVector();
    vP_ISR.Boost(-boostT);
    vP_I.Boost(-boostT);
    
    TVector3 vPt_ISR = vP_ISR.Vect();
    TVector3 vPt_I   = vP_I.Vect();
    vPt_ISR -= vPt_ISR.Dot(boostZ.Unit())*boostZ.Unit();
    vPt_I   -= vPt_I.Dot(boostZ.Unit())*boostZ.Unit();
    
    m_PTISR_fix =  vPt_ISR.Mag();
    m_RISR_fix  = -vPt_I.Dot(vPt_ISR.Unit()) / m_PTISR_fix;
    m_dphiISRI_fix = fabs(vPt_ISR.Angle(vPt_I));

    break;
  }

  m_weight = GetEventWeight();
  
  m_MET     = ETMiss.Pt();
  m_MET_phi = ETMiss.Phi();

  TVector3 genETMiss = GetGenMET();
  m_genMET     = genETMiss.Pt();
  m_genMET_phi = genETMiss.Phi();

  m_Nele = Electrons.size();
  m_Nmu  = Muons.size();
  m_Nlep = Leptons.size();

  ParticleList GenMuons = GetGenMuons();
  ParticleList GenElectrons = GetGenElectrons();
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
  ParticleList GenNus = GetGenNeutrinos();
  m_genNnu = GenNus.size();
  m_PT_nu.clear();
  m_Eta_nu.clear();
  m_Phi_nu.clear();
  m_PDGID_nu.clear();
  for(int i  0; i < m_genNnu; i++){
    m_PT_nu.push_back(GenNus[i].Pt());
    m_Eta_nu.push_back(GenNus[i].Eta());
    m_Phi_nu.push_back(GenNus[i].Phi());
    m_PDGID_nu.push_back(GenNus[i].PDGID());
  }
  
  int m_genNboson;
  vector<double> m_genPT_boson;
  vector<double> m_genEta_boson;
  vector<double> m_genPhi_boson;
  vector<double> m_genM_boson;
  vector<int>    m_genPDGID_boson;
  
  int m_genNsusy;
  vector<double> m_genPT_susy;
  vector<double> m_genEta_susy;
  vector<double> m_genPhi_susy;
  vector<double> m_genM_susy;
  vector<int>    m_genPDGID_susy;


  int m_Nbjet;

  
  if(tree)
    tree->Fill();
  
}
