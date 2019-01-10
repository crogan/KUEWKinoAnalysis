#include "ReducedNtuple.hh"

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

void ReducedNtuple::InitOutputTree(){
  if(m_Tree)
    delete m_Tree;

  string name = string(fChain->GetName());

  m_Tree = (TTree*) new TTree("KUAnalysis", "KUAnalysis");

  m_Tree->Branch("weight", &m_weight);
  
  m_Tree->Branch("MET", &m_MET);
  m_Tree->Branch("MET_phi", &m_MET_phi);
  m_Tree->Branch("HT", &m_HT);

  // SF?
  m_Tree->Branch("Is_SF", &m_Is_SF);

  // pre-computed lepton variables
  m_Tree->Branch("nEl", &m_nEl);
  m_Tree->Branch("nMu", &m_nMu);
  m_Tree->Branch("nBjet", &m_nBjet);
 
  m_Tree->Branch("pT_1lep", &m_pT_1lep);
  m_Tree->Branch("id_1lep", &m_id_1lep);
  m_Tree->Branch("pT_2lep", &m_pT_2lep);
  m_Tree->Branch("id_2lep", &m_id_2lep);
  m_Tree->Branch("pT_3lep", &m_pT_3lep);
  m_Tree->Branch("id_3lep", &m_id_3lep);

  m_Tree->Branch("Nj", &m_Nj);
  m_Tree->Branch("NjS", &m_NjS);
  m_Tree->Branch("NjISR", &m_NjISR);

  m_Tree->Branch("PTCM_comb", &m_PTCM_comb);
  m_Tree->Branch("PTISR_comb", &m_PTISR_comb);
  m_Tree->Branch("RISR_comb", &m_RISR_comb);
  m_Tree->Branch("cosCM_comb", &m_cosCM_comb);
  m_Tree->Branch("cosS_comb", &m_cosS_comb);
  m_Tree->Branch("MISR_comb", &m_MISR_comb);
  m_Tree->Branch("MS_comb", &m_MS_comb);
  m_Tree->Branch("dphiCMI_comb", &m_dphiCMI_comb);
  m_Tree->Branch("dphiSI_comb", &m_dphiSI_comb);
  m_Tree->Branch("dphiISRI_comb", &m_dphiISRI_comb);

  m_Tree->Branch("PTCM_fix", &m_PTCM_fix);
  m_Tree->Branch("PTISR_fix", &m_PTISR_fix);
  m_Tree->Branch("RISR_fix", &m_RISR_fix);
  m_Tree->Branch("cosCM_fix", &m_cosCM_fix);
  m_Tree->Branch("cosS_fix", &m_cosS_fix);
  m_Tree->Branch("MISR_fix", &m_MISR_fix);
  m_Tree->Branch("MS_fix", &m_MS_fix);
  m_Tree->Branch("dphiCMI_fix", &m_dphiCMI_fix);
  m_Tree->Branch("dphiSI_fix", &m_dphiSI_fix);
  m_Tree->Branch("dphiISRI_fix", &m_dphiISRI_fix);

  m_Tree->Branch("MZ", &m_MZ);
  m_Tree->Branch("cosZ", &m_cosZ);

  // which tree are we using for event?
 
  m_Tree->Branch("Is_2LNJ", &m_Is_2LNJ);
  m_Tree->Branch("Is_2L1L", &m_Is_2L1L);
  
  m_Tree->Branch("HN2S", &m_HN2S);
  m_Tree->Branch("HN2SR", &m_HN2SR);
  m_Tree->Branch("H11S", &m_H11S);
  m_Tree->Branch("HN1Ca", &m_HN1Ca);
  m_Tree->Branch("HN1Cb", &m_HN1Cb);
  m_Tree->Branch("H11Ca", &m_H11Ca);
  m_Tree->Branch("H11Cb", &m_H11Cb);
  m_Tree->Branch("cosC", &m_cosC);

  m_Tree->Branch("MZ", &m_MZ);
  m_Tree->Branch("MJ", &m_MJ);
  m_Tree->Branch("cosZ", &m_cosZ);
  m_Tree->Branch("cosJ", &m_cosJ);
}

void ReducedNtuple::FillOutputTree(){

  m_weight = GetEventWeight();

  vector<TLorentzVector> Jets; 
  GetJets(Jets, 20., 2.4); 
  
  // // need at least one jet to play
  // if(Jets.size() < 1) 
  //   return; 

  m_Nj = Jets.size();
  
  TVector3 ETMiss = GetMET();

  if(ETMiss.Mag() < 100.)
    return;
      
  vector<TLorentzVector> Leptons;
  vector<int> LepIDs;
  GetLeptons(Leptons, LepIDs, 3.5, 2.4);

  // figure out which tree to use
  
  m_Is_2LNJ = false;
  m_Is_2L1L = false;
  
  
  if(Leptons.size() < 2) // at least 2 muons for now
    return;

  if(Leptons.size() == 3){
    // at least 1 OS/SF pair
    if(LepIDs[0]+LepIDs[1] == 0 ||
       LepIDs[0]+LepIDs[2] == 0 ||
       LepIDs[1]+LepIDs[2] == 0){
      m_Is_2L1L = true;
    }
  }

  if(Leptons.size() == 2){
    // SS and/or OF leptons
    if(LepIDs[0]+LepIDs[1] == 0){
      if(Jets.size() >= 1){
	m_Is_2LNJ = true;
      }
    }
  }

  if(!m_Is_2LNJ && !m_Is_2L1L)
    return;

  if(Leptons[0].Pt() < 5. && Leptons[1].Pt() < 5.) // lead muon greater than 5 GeV in Pt
    return;

  // if(Leptons[0].Pt() > 30. || Leptons[1].Pt() > 30.) // muon pT's less than 30
  //   return;

  m_weight = GetEventWeight();
  
  m_MET     = ETMiss.Pt();
  m_MET_phi = ETMiss.Phi();

  //m_nBjet = nBJet30_MV2c10;

  
  m_pT_1lep = Leptons[0].Pt();
  m_id_1lep = LepIDs[0];
  
  m_pT_2lep = Leptons[1].Pt();
  m_id_2lep = LepIDs[1];

  if(Leptons.size() > 2){
    m_pT_3lep = Leptons[2].Pt();
    m_id_3lep = Leptons[2].Pt();
  } else {
    m_pT_3lep = 0.;
    m_id_3lep = 0;
  }

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
	  double diff = fabs((Jets[i]+Jets[j]).M() - 80.);
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
	if(LepIDs[i]+LepIDs[j] == 0){
	  if(mSFOS < 0. ||
	     (Leptons[i]+Leptons[j]).M() < mSFOS){
	    mSFOS = (Leptons[i]+Leptons[j]).M();
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

  if(m_Tree)
    m_Tree->Fill();
  
}
