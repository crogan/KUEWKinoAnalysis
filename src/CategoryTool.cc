#include "../include/CategoryTool.hh"

#include "Leptonic.hh"
#include "Hadronic.hh"

using std::cout;
using std::endl;

///////////////////////////////////////////
////////// CategoryTool class
///////////////////////////////////////////

CategoryTool::CategoryTool() {}

CategoryTool::~CategoryTool() {}

// Get all 0L,1L,2L,3L categories
CategoryList CategoryTool::GetCategories() const {
  CategoryList Categories;
  Categories += GetCategories_0L();
  Categories += GetCategories_1L();
  Categories += GetCategories_2L();
  Categories += GetCategories_3L();

  return Categories;

  // define leptons
  Lep elp(kElectron, kPos);
  Lep elm(kElectron, kNeg);
  Lep mup(kMuon, kPos);
  Lep mum(kMuon, kNeg);

  // define hadrons
  Had jt(kJet);
  Had bj(kBtag);
  Had sv(kSV);

  // define leptonic categories
  // 0 L
  Leptonic L_0(LepList(), LepList(), "0");

  // 1 L
  Leptonic L_elp_0(LepList().Add(elp), LepList(), "elp");
  Leptonic L_elm_0(LepList().Add(elm), LepList(), "elm");
  Leptonic L_mup_0(LepList().Add(mup), LepList(), "mup");
  Leptonic L_mum_0(LepList().Add(mum), LepList(), "mum");

  // 2 L, opposite hemisphere
  Leptonic    L_OS_el_el(LepList().Add(elp), LepList().Add(elm), "el^el");
  Leptonic    L_SS_el_el(LepList().Add(elp), LepList().Add(elp), "el^el");
  L_SS_el_el += Leptonic(LepList().Add(elm), LepList().Add(elm));
  Leptonic    L_OS_mu_mu(LepList().Add(mup), LepList().Add(mum), "mu^mu");
  Leptonic    L_SS_mu_mu(LepList().Add(mup), LepList().Add(mup), "mu^mu");
  L_SS_mu_mu += Leptonic(LepList().Add(mum), LepList().Add(mum));
  Leptonic    L_OS_el_mu(LepList().Add(elp), LepList().Add(mum), "el^mu");
  L_OS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mup));
  Leptonic    L_SS_el_mu(LepList().Add(elp), LepList().Add(mup), "el^mu");
  L_SS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mum));

  // 2 L, same hemisphere
  Leptonic    L_OS_elel_0(LepList().Add(elp).Add(elm), LepList(), "elel^0");
  Leptonic    L_SS_elel_0(LepList().Add(elp).Add(elp), LepList(), "elel^0");
  L_SS_elel_0 += Leptonic(LepList().Add(elm).Add(elm), LepList());
  Leptonic    L_OS_mumu_0(LepList().Add(mup).Add(mum), LepList(), "mumu^0");
  Leptonic    L_SS_mumu_0(LepList().Add(mup).Add(mup), LepList(), "mumu^0");
  L_SS_mumu_0 += Leptonic(LepList().Add(mum).Add(mum), LepList());
  Leptonic    L_OS_elmu_0(LepList().Add(elp).Add(mum), LepList(), "elmu^0");
  L_OS_elmu_0 += Leptonic(LepList().Add(elm).Add(mup), LepList());
  Leptonic    L_SS_elmu_0(LepList().Add(elp).Add(mup), LepList(), "elmu^0");
  L_SS_elmu_0 += Leptonic(LepList().Add(elm).Add(mum), LepList());


  
  // define hadronic categories
  // 0j 
  Hadronic              H_0( 0,  0,  0, "0j0sv");
  Hadronic        H_0jge1SV( 0,  0, -1, "0jge1sv");
  Hadronic        H_1SV( 0,  0,  1, "0j1sv");
  Hadronic      H_ge2SV( 0,  0, -2, "0jge2sv");
  
  Hadronic        H_1j0b0SV( 1,  0, 0, "1j0b0sv");
  Hadronic        H_1j1b0SV( 1,  1, 0, "1j1b0sv");
  Hadronic        H_1j0bge1SV( 1,  0, -1, "1j0bge1sv");
  Hadronic        H_1j1bge1SV( 1,  1, -1, "1j1bge1sv");

  Hadronic        H_2j0b( 2, 0, 0, "2j0b");
  H_2j0b += Hadronic(2, 0, -1);
  Hadronic        H_2j1b( 2, 1, 0, "2j1b");
  H_2j1b += Hadronic(2, 1, -1);
   Hadronic       H_2j2b( 2, 2, 0, "2j2b");
  H_2j2b += Hadronic(2, 2, -1);

  Hadronic        H_3j0b( 3, 0, 0, "3j0b");
  H_3j0b += Hadronic(3, 0, -1);
  Hadronic        H_3j1b( 3, 1, 0, "3j1b");
  H_3j1b += Hadronic(3, 1, -1);
  Hadronic        H_3j2b( 3, -2, 0, "3j2b");
  H_3j2b += Hadronic(3, -2, -1);

  Hadronic        H_4j0b( 4, 0, 0, "4j0b");
  H_4j0b += Hadronic(4, 0, -1);
  Hadronic        H_4j1b( 4, 1, 0, "4j1b");
  H_4j1b += Hadronic(4, 1, -1);
  Hadronic        H_4j2b( 4, -2, 0, "4j2b");
  H_4j2b += Hadronic(4, -2, -1);

  Hadronic        H_5j0b( 5, 0, 0, "5j0b");
  H_5j0b += Hadronic(5, 0, -1);
  Hadronic        H_5j1b( 5, 1, 0, "5j1b");
  H_5j1b += Hadronic(5, 1, -1);
  Hadronic        H_5j2b( 5, -2, 0, "5j2b");
  H_5j2b += Hadronic(5, -2, -1);

  Hadronic      H_0Bjet(-1,  0,  0, "0bjet");
  H_0Bjet +=   Hadronic(-1,  0, -1);
  Hadronic      H_1Bjet(-1,  1,  0, "1bjet");
  H_1Bjet +=   Hadronic(-1,  1, -1);
  Hadronic    H_ge1Bjet(-1, -1,  0, "ge1bjet");
  H_ge1Bjet += Hadronic(-1, -1, -1);
  Hadronic    H_ge2Bjet(-1, -2,  0, "ge2bjet");
  H_ge2Bjet += Hadronic(-1, -2, -1);

  vector<Hadronic> H_ISR;
  H_ISR.push_back(H_0Bjet);
  H_ISR.push_back(H_ge1Bjet);

  vector<Hadronic> H_0L;
  H_0L.push_back(H_0jge1SV);
  H_0L.push_back(H_1j0b0SV);
  H_0L.push_back(H_1j1b0SV);
  H_0L.push_back(H_1j0bge1SV);
  H_0L.push_back(H_1j1bge1SV);
  H_0L.push_back(H_2j0b);
  H_0L.push_back(H_2j1b);
  H_0L.push_back(H_2j2b);
  H_0L.push_back(H_3j0b);
  H_0L.push_back(H_3j1b);
  H_0L.push_back(H_3j2b);
  H_0L.push_back(H_4j0b);
  H_0L.push_back(H_4j1b);
  H_0L.push_back(H_4j2b);
  H_0L.push_back(H_5j0b);
  H_0L.push_back(H_5j1b);
  H_0L.push_back(H_5j2b);

  vector<Hadronic> H_1L;
  H_1L.push_back(H_0);
  H_1L.push_back(H_0jge1SV);
  H_1L.push_back(H_1j0b0SV);
  H_1L.push_back(H_1j1b0SV);
  H_1L.push_back(H_1j0bge1SV);
  H_1L.push_back(H_1j1bge1SV);
  H_1L.push_back(H_2j0b);
  H_1L.push_back(H_2j1b);
  H_1L.push_back(H_2j2b);
  H_1L.push_back(H_3j0b);
  H_1L.push_back(H_3j1b);
  H_1L.push_back(H_3j2b);
  H_1L.push_back(H_4j0b);
  H_1L.push_back(H_4j1b);
  H_1L.push_back(H_4j2b);

  vector<Hadronic> H_2LOS;
  H_2LOS.push_back(H_0);
  H_2LOS.push_back(H_1SV);
  H_2LOS.push_back(H_0Bjet);
  H_2LOS.push_back(H_1Bjet);
  H_2LOS.push_back(H_ge2Bjet);

  vector<Hadronic> H_2LSS;
  H_2LSS.push_back(H_0);
  H_2LSS.push_back(H_1SV);
  H_2LSS.push_back(H_0Bjet);
  H_2LSS.push_back(H_1Bjet);
  H_2LSS.push_back(H_ge2Bjet);

  // SV eta bins
  vector<double> SVeta_0L;
  SVeta_0L.push_back(0.);
  SVeta_0L.push_back(1.5);  
  
  vector<double> PTISR_0L;
  PTISR_0L.push_back(200.);
  PTISR_0L.push_back(400.);
  
  vector<double> PTISR_1L;
  PTISR_1L.push_back(200.);
  PTISR_1L.push_back(400.);

  vector<double> PTISR_2LOS;
  PTISR_2LOS.push_back(200.);

  vector<double> PTISR_2LSS;
  PTISR_2LSS.push_back(200.);

  cout << "Initializing 0L" << endl;
  CategoryList Cats_0L;
  Cats_0L += Category(L_0, "Ch0L");

  cout << "Initializing 0L" << endl;
  CategoryList Cats_1L;
  Cats_1L += Category(L_elp_0, "Ch1L"); 
  Cats_1L += Category(L_elm_0, "Ch1L");
  Cats_1L += Category(L_mup_0, "Ch1L");
  Cats_1L += Category(L_mum_0, "Ch1L");

  cout << "Initializing 1L" << endl;
  CategoryList Cats_2LOS;
  Cats_2LOS += Category(L_OS_el_el,  "Ch2LOS");
  Cats_2LOS += Category(L_OS_mu_mu,  "Ch2LOS");
  Cats_2LOS += Category(L_OS_el_mu,  "Ch2LOS");
  Cats_2LOS += Category(L_OS_elel_0, "Ch2LOS");
  Cats_2LOS += Category(L_OS_mumu_0, "Ch2LOS");
  Cats_2LOS += Category(L_OS_elmu_0, "Ch2LOS");

  cout << "Initializing 2L" << endl;
  CategoryList Cats_2LSS;
  Cats_2LSS += Category(L_SS_el_el,  "Ch2LSS");
  Cats_2LSS += Category(L_SS_mu_mu,  "Ch2LSS");
  Cats_2LSS += Category(L_SS_el_mu,  "Ch2LSS");
  Cats_2LSS += Category(L_SS_elel_0, "Ch2LSS");
  Cats_2LSS += Category(L_SS_mumu_0, "Ch2LSS");
  Cats_2LSS += Category(L_SS_elmu_0, "Ch2LSS");

  vector<double> BinsM_0L;
  BinsM_0L.push_back(0.);
  BinsM_0L.push_back(5.);
  BinsM_0L.push_back(10.);
  BinsM_0L.push_back(20.);
  BinsM_0L.push_back(50.);
  BinsM_0L.push_back(100.);
  BinsM_0L.push_back(200.);
  vector<double> BinsR_0L;
  BinsR_0L.push_back(0.5);
  BinsR_0L.push_back(0.7);
  BinsR_0L.push_back(0.8);
  BinsR_0L.push_back(0.85);
  BinsR_0L.push_back(0.9);
  BinsR_0L.push_back(0.95);
  BinsR_0L.push_back(0.975);
  BinsR_0L.push_back(1.);
  FitBin Bin_0L(BinsR_0L, BinsM_0L);


//do coarser binning to look at variations
  vector<double> BinsM_1L;
  BinsM_1L.push_back(0.);
  //BinsM_1L.push_back(5.);
  //BinsM_1L.push_back(10.);
  BinsM_1L.push_back(20.);
  BinsM_1L.push_back(50.);
  BinsM_1L.push_back(100.);
  //BinsM_1L.push_back(200.);
  //BinsM_1L.push_back(400.);
  BinsM_1L.push_back(600.);
  vector<double> BinsR_1L;
  BinsR_1L.push_back(0.8);
  //BinsR_1L.push_back(0.85);
  BinsR_1L.push_back(0.9);
  //BinsR_1L.push_back(0.925);
  BinsR_1L.push_back(0.95);
  //BinsR_1L.push_back(0.975);
  BinsR_1L.push_back(1.);
  //BinsR_1L.push_back(1.05);
  FitBin Bin_1L(BinsR_1L, BinsM_1L);

  vector<double> BinsM_2L;
  BinsM_2L.push_back(0.);
  BinsM_2L.push_back(5.);
  BinsM_2L.push_back(10.);
  BinsM_2L.push_back(20.);
  BinsM_2L.push_back(50.);
  BinsM_2L.push_back(100.);
  BinsM_2L.push_back(200.);
  BinsM_2L.push_back(400.);
  BinsM_2L.push_back(600.);
  vector<double> BinsR_2L;
  BinsR_2L.push_back(0.8);
  BinsR_2L.push_back(0.85);
  BinsR_2L.push_back(0.9);
  BinsR_2L.push_back(0.925);
  BinsR_2L.push_back(0.95);
  BinsR_2L.push_back(0.975);
  BinsR_2L.push_back(1.);
  BinsR_2L.push_back(1.05);
  //FitBin Bin_2L(BinsR_2L, BinsM_2L);

  vector<LepID> IDs;
  IDs.push_back(kGold);
  IDs.push_back(kSilver);
  IDs.push_back(kBronze);
  
  cout << "Building 0L" << endl;
  Cats_0L = Cats_0L.CreateFitBinRegions(Bin_0L); //
  Cats_0L = Cats_0L.CreateHadronicSRegions(H_0L);
  Cats_0L = Cats_0L.CreateHadronicISRRegions(H_ISR);
  Cats_0L = Cats_0L.CreateGenericRegions("PTISR", PTISR_2LOS);

  cout << "Building 1L" << endl;
  Cats_1L = Cats_1L.CreateFitBinRegions(Bin_0L);
  Cats_1L = Cats_1L.CreateLeptonIDRegions(IDs, 1);
  Cats_1L = Cats_1L.CreateHadronicSRegions(H_1L);
  Cats_1L = Cats_1L.CreateHadronicISRRegions(H_ISR);
  Cats_1L = Cats_1L.CreateGenericRegions("PTISR", PTISR_2LOS);

  cout << "Building 2L0L" << endl;
  Cats_2LOS = Cats_2LOS.CreateFitBinRegions(Bin_0L);
  Cats_2LOS = Cats_2LOS.CreateLeptonIDRegions(IDs, 1);
  Cats_2LOS = Cats_2LOS.CreateHadronicSRegions(H_2LOS);
  Cats_2LOS = Cats_2LOS.CreateHadronicISRRegions(H_ISR);
  Cats_2LOS = Cats_2LOS.CreateGenericRegions("PTISR", PTISR_2LOS);

  cout << "Building 2LSS" << endl;
  Cats_2LSS = Cats_2LSS.CreateFitBinRegions(Bin_0L);
  Cats_2LSS = Cats_2LSS.CreateLeptonIDRegions(IDs, 1);
  Cats_2LSS = Cats_2LSS.CreateHadronicSRegions(H_2LSS);
  Cats_2LSS = Cats_2LSS.CreateHadronicISRRegions(H_ISR);
  Cats_2LSS = Cats_2LSS.CreateGenericRegions("PTISR", PTISR_2LOS);
  
  
  //////////////////////////////////////////////////////

  // CategoryList Categories;
  // Categories += Cats_0L;
  // Categories += Cats_1L;
  // Categories += Cats_2LOS;
  // Categories += Cats_2LSS;

  return Categories;
}

CategoryList CategoryTool::GetCategories_RandD() const {
  // define leptons
  Lep elp(kElectron, kPos);
  Lep elm(kElectron, kNeg);
  Lep mup(kMuon, kPos);
  Lep mum(kMuon, kNeg);

  // define hadrons
  Had jt(kJet);
  Had bj(kBtag);
  Had sv(kSV);

  // 1 L
  Leptonic L_elp_0(LepList().Add(elp), LepList(), "elp");
  Leptonic L_elm_0(LepList().Add(elm), LepList(), "elm");
  Leptonic L_mup_0(LepList().Add(mup), LepList(), "mup");
  Leptonic L_mum_0(LepList().Add(mum), LepList(), "mum");


  // 2 L, opposite hemisphere
  Leptonic    L_OS_el_el(LepList().Add(elp), LepList().Add(elm), "el^el");
  Leptonic    L_SS_el_el(LepList().Add(elp), LepList().Add(elp), "el^el");
  L_SS_el_el += Leptonic(LepList().Add(elm), LepList().Add(elm));
  Leptonic    L_OS_mu_mu(LepList().Add(mup), LepList().Add(mum), "mu^mu");
  Leptonic    L_SS_mu_mu(LepList().Add(mup), LepList().Add(mup), "mu^mu");
  L_SS_mu_mu += Leptonic(LepList().Add(mum), LepList().Add(mum));
  Leptonic    L_OS_el_mu(LepList().Add(elp), LepList().Add(mum), "el^mu");
  L_OS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mup));
  Leptonic    L_SS_el_mu(LepList().Add(elp), LepList().Add(mup), "el^mu");
  L_SS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mum));

  Hadronic      H_0Bjet(-1,  0,  0, "0bjet");
  H_0Bjet +=   Hadronic(-1,  0, -1);
  Hadronic      H_1Bjet(-1,  1,  0, "1bjet");
  H_1Bjet +=   Hadronic(-1,  1, -1);
  Hadronic    H_ge1Bjet(-1, -1,  0, "ge1bjet");
  H_ge1Bjet += Hadronic(-1, -1, -1);
  Hadronic    H_ge2Bjet(-1, -2,  0, "ge2bjet");
  H_ge2Bjet += Hadronic(-1, -2, -1);

  Hadronic H_incl(-1,  -1,  -1, "INCL");
  vector<Hadronic> H_INCL;
  H_INCL.push_back(H_incl);

  vector<Hadronic> H_ISR;
  H_ISR.push_back(H_0Bjet);
  H_ISR.push_back(H_ge1Bjet); 
  
  vector<double> PTISR_INCL;
  PTISR_INCL.push_back(200.);

  cout << "Initializing 1L" << endl;
  CategoryList Cats_1L;
  Cats_1L += Category(L_elp_0, "Ch1L"); 
  Cats_1L += Category(L_elm_0, "Ch1L");
  Cats_1L += Category(L_mup_0, "Ch1L");
  Cats_1L += Category(L_mum_0, "Ch1L");

  cout << "Initializing 2LOS" << endl;
  CategoryList Cats_2LOS;
  Cats_2LOS += Category(L_OS_el_el,  "Ch2LOS");
  Cats_2LOS += Category(L_OS_mu_mu,  "Ch2LOS");
  Cats_2LOS += Category(L_OS_el_mu,  "Ch2LOS");

  FitBin Bin_INCL = GetFitBin(2);

  vector<LepID> IDs;
  IDs.push_back(kGold);
  IDs.push_back(kSilver);
  IDs.push_back(kBronze);
  
  cout << "Building 1L" << endl;
  Cats_1L = Cats_1L.CreateFitBinRegions(Bin_INCL);
  Cats_1L = Cats_1L.CreateLeptonIDRegions(IDs, 1);
  Cats_1L = Cats_1L.CreateHadronicSRegions(H_INCL);
  Cats_1L = Cats_1L.CreateHadronicISRRegions(H_ISR);
  Cats_1L = Cats_1L.CreateGenericRegions("PTISR", PTISR_INCL);

  cout << "Building 2L0L" << endl;
  Cats_2LOS = Cats_2LOS.CreateFitBinRegions(Bin_INCL);
  Cats_2LOS = Cats_2LOS.CreateLeptonIDRegions(IDs, 1);
  Cats_2LOS = Cats_2LOS.CreateHadronicSRegions(H_INCL);
  Cats_2LOS = Cats_2LOS.CreateHadronicISRRegions(H_ISR);
  Cats_2LOS = Cats_2LOS.CreateGenericRegions("PTISR", PTISR_INCL);
  
  //////////////////////////////////////////////////////

  CategoryList Categories;
  Categories += Cats_1L;
  Categories += Cats_2LOS;

  return Categories;
}

FitBin CategoryTool::GetFitBin(int index) const {
  vector<RBin> RBins;

  // R1
  if(index == 1){
     RBins.push_back(RBin(0.6,   0.7,   VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
     RBins.push_back(RBin(0.7,   0.75,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
     RBins.push_back(RBin(0.75,  0.8,   VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
     RBins.push_back(RBin(0.8,   0.85,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
     RBins.push_back(RBin(0.85,  0.9,   VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
     RBins.push_back(RBin(0.9,   0.925, VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
     RBins.push_back(RBin(0.925, 0.95,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
     RBins.push_back(RBin(0.95,  1.0,   VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(50.).a(100.).a(120.)));
  }

  // R2
  if(index == 2){
     RBins.push_back(RBin(0.6,   0.75,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(120.)));
     RBins.push_back(RBin(0.75,  0.85,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(120.)));
     RBins.push_back(RBin(0.85,  0.9,   VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(120.)));
     RBins.push_back(RBin(0.9,   0.925, VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(120.)));
     RBins.push_back(RBin(0.925, 0.95,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(120.)));
     RBins.push_back(RBin(0.95,  0.975, VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(120.)));
     RBins.push_back(RBin(0.975, 1.,    VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(120.)));
  }

  // R3
  if(index == 3){
     RBins.push_back(RBin(0.6,   0.7,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(150.)));
     RBins.push_back(RBin(0.7,   0.8,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(150.)));
     RBins.push_back(RBin(0.8,   0.85,  VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(150.)));
     RBins.push_back(RBin(0.85,  0.9,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(150.)));
     RBins.push_back(RBin(0.9,   0.95,  VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(150.)));
     RBins.push_back(RBin(0.95,  1.0,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(150.)));
  }

  // R4
  if(index == 4){
    RBins.push_back(RBin(0.6,  0.7,  VD().a(0.).a(20.).a(30.).a(40.).a(50.).a(100.).a(200.)));
    RBins.push_back(RBin(0.7,  0.75, VD().a(0.).a(20.).a(30.).a(40.).a(50.).a(100.).a(200.)));
    RBins.push_back(RBin(0.75, 0.8,  VD().a(0.).a(20.).a(30.).a(40.).a(50.).a(100.).a(200.)));
    RBins.push_back(RBin(0.8,  0.85, VD().a(0.).a(20.).a(30.).a(40.).a(50.).a(100.).a(200.)));
    RBins.push_back(RBin(0.85, 0.9,  VD().a(0.).a(20.).a(30.).a(40.).a(50.).a(100.).a(200.)));
    RBins.push_back(RBin(0.9,  0.95, VD().a(0.).a(20.).a(30.).a(40.).a(50.).a(100.).a(200.)));
    RBins.push_back(RBin(0.95, 1.0,  VD().a(0.).a(20.).a(30.).a(40.).a(50.).a(100.).a(200.)));
  }

 // // R5
 // if(index == 5){
 //   RBins.push_back(RBin(0.6,  0.65, VD().a(0.).a(20.).a(40.).a(60.).a(80.).a(100.).a(200.)));
 //   RBins.push_back(RBin(0.65, 0.7,  VD().a(0.).a(20.).a(40.).a(60.).a(80.).a(100.).a(200.)));
 //   RBins.push_back(RBin(0.7,  0.75, VD().a(0.).a(20.).a(40.).a(60.).a(80.).a(100.).a(200.)));
 //   RBins.push_back(RBin(0.75, 0.8,  VD().a(0.).a(20.).a(40.).a(60.).a(80.).a(100.).a(200.)));
 //   RBins.push_back(RBin(0.8,  0.85, VD().a(0.).a(20.).a(40.).a(60.).a(80.).a(100.).a(200.)));
 //   RBins.push_back(RBin(0.85, 0.9,  VD().a(0.).a(20.).a(40.).a(60.).a(80.).a(100.).a(200.)));
 //   RBins.push_back(RBin(0.9,  1.0,  VD().a(0.).a(20.).a(40.).a(60.).a(80.).a(100.).a(200.)));
 // }
  
// R5 - 1L 3j - based on T2tt_5000420
  if(index == 5){
    RBins.push_back(RBin(0.6, 0.65,  VD().a(0.).a(45).a(100.).a(160.).a(240.).a(300.)));
    RBins.push_back(RBin(0.65, 0.7,  VD().a(0.).a(45.).a(120.).a(155.).a(200.).a(300.)));
    RBins.push_back(RBin(0.7, 0.75,  VD().a(0.).a(40.).a(110.).a(150.).a(190.).a(300.)));
    RBins.push_back(RBin(0.75, 0.83, VD().a(0.).a(90.).a(160.).a(300.)));
    RBins.push_back(RBin(0.83, 0.9,  VD().a(0.).a(0).a(300.)));
    //RBins.push_back(RBin(0.8,  0.85, VD().a(0.).a(60.).a(80.).a(100.).a(200.)));
    //RBins.push_back(RBin(0.85, 0.9,  VD().a(0.).a(60.).a(80.).a(100.).a(200.)));
    //RBins.push_back(RBin(0.9,  1.0,  VD().a(0.).a(60.).a(80.).a(100.).a(200.)));

//original R5 M6
if(index == 6){
    RBins.push_back(RBin(0.6,  0.65, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.65,  0.7, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.7,  0.75, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.75,  0.8, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.8,  0.85, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.85,  0.9, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.9,  1.0,  VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
}



  // R5 M6 - 4j - based on T2tt5000420
//  if(index == 6){
//    RBins.push_back(RBin(0.6,  0.63, VD().a(0.).a(60.).a(140.).a(200.).a(300.)));
//    RBins.push_back(RBin(0.63, 0.7,  VD().a(0.).a(60.).a(100.).a(140.).a(170).a(300.)));
//    RBins.push_back(RBin(0.7,  0.8, VD().a(0.).a(60.).a(100.).a(140.).a(300.)));
//    RBins.push_back(RBin(0.8, 0.9,  VD().a(0.).a(300.)));
//   // RBins.push_back(RBin(0.8,  0.9,  VD().a(0.).a(100.).a(140.).a(200.)));
//    //RBins.push_back(RBin(0.85, 0.9,  VD().a(0.).a(80.).a(120.).a(200.)));
//   // RBins.push_back(RBin(0.9,  1.0,  VD().a(0.).a(100.).a(140.).a(200.)));
//  }

  }
  FitBin Bins(RBins);
  return Bins;

}

Hadronic CategoryTool::GetHadronicRegion(int Njet, int index) const {

  /////////
  // 0 jet
  /////////
  if(Njet == 0){
    // fully inclusive
    if(index == 0){
      Hadronic H(0, 0, 0, "incl");
      H += Hadronic(0, 0, -1);
      H += Hadronic(-1, 0, 0);
      H += Hadronic(-1, 0, -1);
      H += Hadronic(-1, -1, 0);
      H += Hadronic(-1, -1, -1);
      return H;
    }

    // C1
    if(index == 1){
      Hadronic H(0, 0, 0, "0j0sv");
      return H;
    }

    // C2
    if(index == 2){
      Hadronic H(0, 0, -1, "0jge1sv");
      return H;
    }

    // exactly 1 sv
    if(index == 3){
      Hadronic H(0, 0, 1, "0j1sv");
      return H;
    }

    // ge 2 sv
    if(index == 4){
      Hadronic H(0, 0, -2, "0jge2sv");
      return H;
    }
  }

  /////////
  // 1 jet
  /////////
  if(Njet == 1){
    // fully inclusive ge1 jet
    if(index == 0){
      Hadronic H(-1, 0, 0, "ge1j");
      H += Hadronic(-1, 0, -1);
      H += Hadronic(-1, -1, 0);
      H += Hadronic(-1, -1, -1);
      return H;
    }

    // inclusive exactly 1 jet
    if(index == 1){
      Hadronic H(1, 0, 0, "1j");
      H += Hadronic(1, 0, -1);
      H += Hadronic(1, -1, 0);
      H += Hadronic(1, -1, -1);
      return H;
    }

    // C3 exactly 1 jet 0b 0sv
    if(index == 2){
      Hadronic H(1, 0, 0, "1j0b0sv");
      return H;
    }

    // C4 exactly 1 jet 1b 0sv
    if(index == 3){
      Hadronic H(1, 1, 0, "1j1b0sv");
      return H;
    }

    // C5 exactly 1 jet 0b ge1sv
    if(index == 4){
      Hadronic H(1, 0, -1, "1j0bge1sv");
      return H;
    }

    // C6 exactly 1 jet 1b ge1sv
    if(index == 5){
      Hadronic H(1, 1, -1, "1j0bge1sv");
      return H;
    }

    // B1 ge1 jet 0 b
    if(index == 6){
      Hadronic H(-1, 0, 0, "ge1j0b");
      H += Hadronic(-1, 0, -1);
      return H;
    }

    // B2 ge1 jet ge1 b
    if(index == 7){
      Hadronic H(-1, -1, 0, "ge1jge1b");
      H += Hadronic(-1, -1, -1);
      return H;
    }
  }

  /////////
  // 2 jet
  /////////
  if(Njet == 2){
    // fully inclusive ge2 jet
    if(index == 0){
      Hadronic H(-2, 0, 0, "ge2j");
      H += Hadronic(-2, 0, -1);
      H += Hadronic(-2, -1, 0);
      H += Hadronic(-2, -1, -1);
      return H;
    }

    // fully inclusive exactly 2 jet
    if(index == 1){
      Hadronic H(2, 0, 0, "2j");
      H += Hadronic(2, 0, -1);
      H += Hadronic(2, -1, 0);
      H += Hadronic(2, -1, -1);
      return H;
    }

    // C7 2 jet 0b
    if(index == 2){
      Hadronic H(2, 0, 0, "2j0b");
      H += Hadronic(2, 0, -1);
      return H;
    }

    // C8 2 jet 1b
    if(index == 3){
      Hadronic H(2, 1, 0, "2j1b");
      H += Hadronic(2, 1, -1);
      return H;
    }

    // C9 2 jet 2b
    if(index == 4){
      Hadronic H(2, 2, 0, "2j2b");
      H += Hadronic(2, 2, -1);
      return H;
    }

     // ge2j 0b
    if(index == 5){
      Hadronic H(-2, 0, 0, "ge2j0b");
      H += Hadronic(-2, 0, -1);
      return H;
    }

    // ge2j ge1b
    if(index == 6){
      Hadronic H(-2, -1, 0, "ge2jge1b");
      H += Hadronic(-2, -1, -1);
      return H;
    }

    //  ge2j 1b
    if(index == 7){
      Hadronic H(-2, 1, 0, "ge2j1b");
      H += Hadronic(-2, 1, -1);
      return H;
    }

    //  ge2j ge2b
    if(index == 8){
      Hadronic H(-2, -2, 0, "ge2jge2b");
      H += Hadronic(-2, -2, -1);
      return H;
    }
  }

  /////////
  // 3 jet
  /////////
  if(Njet == 3){
    // fully inclusive ge3 jet
    if(index == 0){
      Hadronic H(-3, 0, 0, "ge3j");
      H += Hadronic(-3, 0, -1);
      H += Hadronic(-3, -1, 0);
      H += Hadronic(-3, -1, -1);
      return H;
    }
    
    // fully inclusive exactly 3 jet
    if(index == 1){
      Hadronic H(3, 0, 0, "3j");
      H += Hadronic(3, 0, -1);
      H += Hadronic(3, -1, 0);
      H += Hadronic(3, -1, -1);
      return H;
    }

    // C10 3j 0b
    if(index == 2){
      Hadronic H(3, 0, 0, "3j0b");
      H += Hadronic(3, 0, -1);
      return H;
    }

    // C11 3j ge1b
    if(index == 3){
      Hadronic H(3, -1, 0, "3jge1b");
      H += Hadronic(3, -1, -1);
      return H;
    }

    //  3j 1b
    if(index == 4){
      Hadronic H(3, 1, 0, "3j1b");
      H += Hadronic(3, 1, -1);
      return H;
    }

    //  3j ge2b
    if(index == 5){
      Hadronic H(3, -2, 0, "3jge2b");
      H += Hadronic(3, -2, -1);
      return H;
    }

     // ge3j 0b
    if(index == 6){
      Hadronic H(-3, 0, 0, "ge3j0b");
      H += Hadronic(-3, 0, -1);
      return H;
    }

    // ge3j ge1b
    if(index == 7){
      Hadronic H(-3, -1, 0, "ge3jge1b");
      H += Hadronic(-3, -1, -1);
      return H;
    }

    //  ge3j 1b
    if(index == 8){
      Hadronic H(-3, 1, 0, "ge3j1b");
      H += Hadronic(-3, 1, -1);
      return H;
    }

    //  ge3j ge2b
    if(index == 9){
      Hadronic H(-3, -2, 0, "ge3jge2b");
      H += Hadronic(-3, -2, -1);
      return H;
    } 
  }

  /////////
  // 4 jet
  /////////
  if(Njet == 4){
    // C12 fully inclusive ge4 jet
    if(index == 0){
      Hadronic H(-4, 0, 0, "ge4j");
      H += Hadronic(-4, 0, -1);
      H += Hadronic(-4, -1, 0);
      H += Hadronic(-4, -1, -1);
      return H;
    }
    
    // fully inclusive exactly 4 jet
    if(index == 1){
      Hadronic H(4, 0, 0, "4j");
      H += Hadronic(4, 0, -1);
      H += Hadronic(4, -1, 0);
      H += Hadronic(4, -1, -1);
      return H;
    }

    // 4j 0b
    if(index == 2){
      Hadronic H(4, 0, 0, "4j0b");
      H += Hadronic(4, 0, -1);
      return H;
    }

    // 4j ge1b
    if(index == 3){
      Hadronic H(4, -1, 0, "4jge1b");
      H += Hadronic(4, -1, -1);
      return H;
    }

    //  4j 1b
    if(index == 4){
      Hadronic H(4, 1, 0, "4j1b");
      H += Hadronic(4, 1, -1);
      return H;
    }

    //  4j ge2b
    if(index == 5){
      Hadronic H(4, -2, 0, "4jge2b");
      H += Hadronic(4, -2, -1);
      return H;
    }

    // ge4j 0b
    if(index == 6){
      Hadronic H(-4, 0, 0, "ge4j0b");
      H += Hadronic(-4, 0, -1);
      return H;
    }

    // ge4j ge1b
    if(index == 7){
      Hadronic H(-4, -1, 0, "ge4jge1b");
      H += Hadronic(-4, -1, -1);
      return H;
    }

    //  ge4j 1b
    if(index == 8){
      Hadronic H(-4, 1, 0, "ge4j1b");
      H += Hadronic(-4, 1, -1);
      return H;
    }

    //  ge4j ge2b
    if(index == 9){
      Hadronic H(-4, -2, 0, "ge4jge2b");
      H += Hadronic(-4, -2, -1);
      return H;
    }
  }

  /////////
  // 5 jet
  /////////
  if(Njet == 5){
    // fully inclusive ge5 jet
    if(index == 0){
      Hadronic H(-5, 0, 0, "ge5j");
      H += Hadronic(-5, 0, -1);
      H += Hadronic(-5, -1, 0);
      H += Hadronic(-5, -1, -1);
      return H;
    }
    
    // fully inclusive exactly 5 jet
    if(index == 1){
      Hadronic H(5, 0, 0, "5j");
      H += Hadronic(5, 0, -1);
      H += Hadronic(5, -1, 0);
      H += Hadronic(5, -1, -1);
      return H;
    }

    // 5j 0b
    if(index == 2){
      Hadronic H(5, 0, 0, "5j0b");
      H += Hadronic(5, 0, -1);
      return H;
    }

    // 5j ge1b
    if(index == 3){
      Hadronic H(5, -1, 0, "5jge1b");
      H += Hadronic(5, -1, -1);
      return H;
    }

    //  5j 1b
    if(index == 4){
      Hadronic H(5, 1, 0, "5j1b");
      H += Hadronic(5, 1, -1);
      return H;
    }

    //  5j ge2b
    if(index == 5){
      Hadronic H(5, -2, 0, "5jge2b");
      H += Hadronic(5, -2, -1);
      return H;
    }

    // ge5j 0b
    if(index == 6){
      Hadronic H(-5, 0, 0, "ge5j0b");
      H += Hadronic(-5, 0, -1);
      return H;
    }

    // ge5j ge1b
    if(index == 7){
      Hadronic H(-5, -1, 0, "ge5jge1b");
      H += Hadronic(-5, -1, -1);
      return H;
    }

    //  ge5j 1b
    if(index == 8){
      Hadronic H(-5, 1, 0, "ge5j1b");
      H += Hadronic(-5, 1, -1);
      return H;
    }

    //  ge5j ge2b
    if(index == 9){
      Hadronic H(-5, -2, 0, "ge5jge2b");
      H += Hadronic(-5, -2, -1);
      return H;
    }
    
  }

  // default inclusive
  Hadronic H(0, 0, 0, "incl");
  H += Hadronic(0, 0, -1);
  H += Hadronic(-1, 0, 0);
  H += Hadronic(-1, 0, -1);
  H += Hadronic(-1, -1, 0);
  H += Hadronic(-1, -1, -1);
  return H;

}

CategoryList CategoryTool::GetCategories_0L() const {
  CategoryList Cats_0L;

  return Cats_0L;
}

CategoryList CategoryTool::GetCategories_1L() const {
  CategoryList Cats_1L;
  cout << "Creating 1L Categories" << endl;

  // define leptons
  Lep elp(kElectron, kPos);
  Lep elm(kElectron, kNeg);
  Lep mup(kMuon, kPos);
  Lep mum(kMuon, kNeg);

  // separated by charge
  Leptonic L_elp_0(LepList().Add(elp), LepList(), "elp");
  Leptonic L_elm_0(LepList().Add(elm), LepList(), "elm");
  Leptonic L_mup_0(LepList().Add(mup), LepList(), "mup");
  Leptonic L_mum_0(LepList().Add(mum), LepList(), "mum");
  CategoryList Cats_1Lcharge;
  Cats_1Lcharge += Category(L_elp_0, "Ch1L"); 
  Cats_1Lcharge += Category(L_elm_0, "Ch1L");
  Cats_1Lcharge += Category(L_mup_0, "Ch1L");
  Cats_1Lcharge += Category(L_mum_0, "Ch1L");

  // charge inclusive
  Leptonic L_elpm_0(LepList().Add(elp), LepList(), "elpm");
  L_elpm_0 += Leptonic(LepList().Add(elm), LepList());
  Leptonic L_mupm_0(LepList().Add(mup), LepList(), "mupm");
  L_mupm_0 += Leptonic(LepList().Add(elm), LepList());
  CategoryList Cats_1Lincl;
  Cats_1Lincl += Category(L_elpm_0, "Ch1L"); 
  Cats_1Lincl += Category(L_mupm_0, "Ch1L");

  // Get hadronic ISR region lists - inclusive and b-separated
  vector<Hadronic> H_ISR_noB;
  vector<Hadronic> H_ISR_B;
  H_ISR_noB.push_back(GetHadronicRegion(1, 0)); // ge1j
  H_ISR_B.push_back(GetHadronicRegion(1, 6)); // ge1j0b
  H_ISR_B.push_back(GetHadronicRegion(1, 7)); // ge1jge1b
  
  // Get PTISR binning
  vector<double> PTISR_incl;
  PTISR_incl.push_back(200.);

  // Get SV binning
  vector<double> SV_eta;
  SV_eta.push_back(0.);
  SV_eta.push_back(1.5);

  // Get Lep ID vectors
  vector<LepID> IDs_all;
  IDs_all.push_back(kGold);
  IDs_all.push_back(kSilver);
  IDs_all.push_back(kBronze);
  vector<LepID> IDs_gold;
  IDs_gold.push_back(kGold);
  vector<LepID> IDs_silver;
  IDs_silver.push_back(kSilver);
  vector<LepID> IDs_bronze;
  IDs_bronze.push_back(kBronze);


  ///////////////////////////////////////////////
  
  cout << "Building 1L 0S object regions" << endl;
  CategoryList Cats_1L_0S_gold = Cats_1Lcharge;
  CategoryList Cats_1L_0S_silver = Cats_1Lincl;
  CategoryList Cats_1L_0S_bronze = Cats_1Lcharge;
  vector<Hadronic> H_1L_0S;
  H_1L_0S.push_back(GetHadronicRegion(0, 1)); // 0j0sv

  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateFitBinRegions(GetFitBin(1));
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateHadronicSRegions(H_1L_0S);
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_0S_silver = Cats_1L_0S_silver.CreateFitBinRegions(GetFitBin(1));
  Cats_1L_0S_silver = Cats_1L_0S_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_0S_silver = Cats_1L_0S_silver.CreateHadronicSRegions(H_1L_0S);
  Cats_1L_0S_silver = Cats_1L_0S_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_0S_silver = Cats_1L_0S_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_0S_bronze = Cats_1L_0S_bronze.CreateFitBinRegions(GetFitBin(1));
  Cats_1L_0S_bronze = Cats_1L_0S_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_0S_bronze = Cats_1L_0S_bronze.CreateHadronicSRegions(H_1L_0S);
  Cats_1L_0S_bronze = Cats_1L_0S_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_0S_bronze = Cats_1L_0S_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L += Cats_1L_0S_gold;
  Cats_1L += Cats_1L_0S_silver;
  Cats_1L += Cats_1L_0S_bronze;

  ///////////////////////////////////////////////

  cout << "Building 1L 0Sjet SV object regions" << endl;
  CategoryList Cats_1L_SV0S_gold = Cats_1Lcharge;
  CategoryList Cats_1L_SV0S_silver = Cats_1Lincl;
  CategoryList Cats_1L_SV0S_bronze = Cats_1Lcharge;
  vector<Hadronic> H_1L_SV0S;
  H_1L_SV0S.push_back(GetHadronicRegion(0, 2)); // 0jge1sv

  Cats_1L_SV0S_gold = Cats_1L_SV0S_gold.CreateFitBinRegions(GetFitBin(2));
  Cats_1L_SV0S_gold = Cats_1L_SV0S_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_SV0S_gold = Cats_1L_SV0S_gold.CreateHadronicSRegions(H_1L_SV0S);
  Cats_1L_SV0S_gold = Cats_1L_SV0S_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_SV0S_gold = Cats_1L_SV0S_gold.CreateGenericRegions("PTISR", PTISR_incl);
  Cats_1L_SV0S_gold = Cats_1L_SV0S_gold.CreateGenericRegions("SVeta", SV_eta);

  Cats_1L_SV0S_silver = Cats_1L_SV0S_silver.CreateFitBinRegions(GetFitBin(2));
  Cats_1L_SV0S_silver = Cats_1L_SV0S_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_SV0S_silver = Cats_1L_SV0S_silver.CreateHadronicSRegions(H_1L_SV0S);
  Cats_1L_SV0S_silver = Cats_1L_SV0S_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_SV0S_silver = Cats_1L_SV0S_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_SV0S_bronze = Cats_1L_SV0S_bronze.CreateFitBinRegions(GetFitBin(2));
  Cats_1L_SV0S_bronze = Cats_1L_SV0S_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_SV0S_bronze = Cats_1L_SV0S_bronze.CreateHadronicSRegions(H_1L_SV0S);
  Cats_1L_SV0S_bronze = Cats_1L_SV0S_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_SV0S_bronze = Cats_1L_SV0S_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L += Cats_1L_SV0S_gold;
  Cats_1L += Cats_1L_SV0S_silver;
  Cats_1L += Cats_1L_SV0S_bronze;

  ///////////////////////////////////////////////

  cout << "Building 1L 1j 0/1b 0sv object regions" << endl;
  CategoryList Cats_1L_1j0svS_gold = Cats_1Lcharge;
  CategoryList Cats_1L_1j0svS_silver = Cats_1Lincl;
  CategoryList Cats_1L_1j0svS_bronze = Cats_1Lincl;
  vector<Hadronic> H_1L_1j0svS;
  H_1L_1j0svS.push_back(GetHadronicRegion(1, 2)); // 1j0b0sv
  H_1L_1j0svS.push_back(GetHadronicRegion(1, 3)); // 1j1b0sv

  Cats_1L_1j0svS_gold = Cats_1L_1j0svS_gold.CreateFitBinRegions(GetFitBin(3));
  Cats_1L_1j0svS_gold = Cats_1L_1j0svS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_1j0svS_gold = Cats_1L_1j0svS_gold.CreateHadronicSRegions(H_1L_1j0svS);
  Cats_1L_1j0svS_gold = Cats_1L_1j0svS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_1j0svS_gold = Cats_1L_1j0svS_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_1j0svS_silver = Cats_1L_1j0svS_silver.CreateFitBinRegions(GetFitBin(3));
  Cats_1L_1j0svS_silver = Cats_1L_1j0svS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_1j0svS_silver = Cats_1L_1j0svS_silver.CreateHadronicSRegions(H_1L_1j0svS);
  Cats_1L_1j0svS_silver = Cats_1L_1j0svS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_1j0svS_silver = Cats_1L_1j0svS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_1j0svS_bronze = Cats_1L_1j0svS_bronze.CreateFitBinRegions(GetFitBin(3));
  Cats_1L_1j0svS_bronze = Cats_1L_1j0svS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_1j0svS_bronze = Cats_1L_1j0svS_bronze.CreateHadronicSRegions(H_1L_1j0svS);
  Cats_1L_1j0svS_bronze = Cats_1L_1j0svS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_1j0svS_bronze = Cats_1L_1j0svS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L += Cats_1L_1j0svS_gold;
  Cats_1L += Cats_1L_1j0svS_silver;
  Cats_1L += Cats_1L_1j0svS_bronze;

  ///////////////////////////////////////////////

  cout << "Building 1L 1j 0/1b ge1sv object regions" << endl;
  CategoryList Cats_1L_1jge1svS_gold = Cats_1Lcharge;
  CategoryList Cats_1L_1jge1svS_silver = Cats_1Lincl;
  CategoryList Cats_1L_1jge1svS_bronze = Cats_1Lincl;
  vector<Hadronic> H_1L_1jge1svS;
  H_1L_1jge1svS.push_back(GetHadronicRegion(1, 4)); // 1j0bge1sv
  H_1L_1jge1svS.push_back(GetHadronicRegion(1, 5)); // 1j1bge1sv

  Cats_1L_1jge1svS_gold = Cats_1L_1jge1svS_gold.CreateFitBinRegions(GetFitBin(3));
  Cats_1L_1jge1svS_gold = Cats_1L_1jge1svS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_1jge1svS_gold = Cats_1L_1jge1svS_gold.CreateHadronicSRegions(H_1L_1jge1svS);
  Cats_1L_1jge1svS_gold = Cats_1L_1jge1svS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_1jge1svS_gold = Cats_1L_1jge1svS_gold.CreateGenericRegions("PTISR", PTISR_incl);
  Cats_1L_1jge1svS_gold = Cats_1L_1jge1svS_gold.CreateGenericRegions("SVeta", SV_eta);

  Cats_1L_1jge1svS_silver = Cats_1L_1jge1svS_silver.CreateFitBinRegions(GetFitBin(3));
  Cats_1L_1jge1svS_silver = Cats_1L_1jge1svS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_1jge1svS_silver = Cats_1L_1jge1svS_silver.CreateHadronicSRegions(H_1L_1jge1svS);
  Cats_1L_1jge1svS_silver = Cats_1L_1jge1svS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_1jge1svS_silver = Cats_1L_1jge1svS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_1jge1svS_bronze = Cats_1L_1jge1svS_bronze.CreateFitBinRegions(GetFitBin(3));
  Cats_1L_1jge1svS_bronze = Cats_1L_1jge1svS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_1jge1svS_bronze = Cats_1L_1jge1svS_bronze.CreateHadronicSRegions(H_1L_1jge1svS);
  Cats_1L_1jge1svS_bronze = Cats_1L_1jge1svS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_1jge1svS_bronze = Cats_1L_1jge1svS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L += Cats_1L_1jge1svS_gold;
  Cats_1L += Cats_1L_1jge1svS_silver;
  Cats_1L += Cats_1L_1jge1svS_bronze;

  ///////////////////////////////////////////////

  cout << "Building 1L 2j 0/1/2b object regions" << endl;
  CategoryList Cats_1L_2jS_gold = Cats_1Lcharge;
  CategoryList Cats_1L_2jS_silver = Cats_1Lincl;
  CategoryList Cats_1L_2jS_bronze = Cats_1Lincl;
  vector<Hadronic> H_1L_2jS;
  H_1L_2jS.push_back(GetHadronicRegion(2, 2)); // 2j0b
  H_1L_2jS.push_back(GetHadronicRegion(2, 3)); // 2j1b
  H_1L_2jS.push_back(GetHadronicRegion(2, 4)); // 2j2b

  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateFitBinRegions(GetFitBin(4));
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateHadronicSRegions(H_1L_2jS);
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateFitBinRegions(GetFitBin(4));
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateHadronicSRegions(H_1L_2jS);
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateFitBinRegions(GetFitBin(4));
  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateHadronicSRegions(H_1L_2jS);
  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L += Cats_1L_2jS_gold;
  Cats_1L += Cats_1L_2jS_silver;
  Cats_1L += Cats_1L_2jS_bronze;

  ///////////////////////////////////////////////

  cout << "Building 1L 3j 0/1/2b object regions" << endl;
  CategoryList Cats_1L_3jS_gold = Cats_1Lcharge;
  CategoryList Cats_1L_3jS_silver = Cats_1Lincl;
  CategoryList Cats_1L_3jS_bronze = Cats_1Lincl;
  vector<Hadronic> H_1L_3jS;
  H_1L_3jS.push_back(GetHadronicRegion(3, 2)); // 3j0b
  H_1L_3jS.push_back(GetHadronicRegion(3, 4)); // 3j1b
  H_1L_3jS.push_back(GetHadronicRegion(3, 5)); // 3jge2b

  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateFitBinRegions(GetFitBin(5));
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateHadronicSRegions(H_1L_3jS);
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateFitBinRegions(GetFitBin(5));
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateHadronicSRegions(H_1L_3jS);
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateFitBinRegions(GetFitBin(5));
  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateHadronicSRegions(H_1L_3jS);
  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L += Cats_1L_3jS_gold;
  Cats_1L += Cats_1L_3jS_silver;
  Cats_1L += Cats_1L_3jS_bronze;

  ///////////////////////////////////////////////

  cout << "Building 1L 4j 0/1b object regions" << endl;
  CategoryList Cats_1L_4jS_gold = Cats_1Lcharge;
  CategoryList Cats_1L_4jS_silver = Cats_1Lincl;
  CategoryList Cats_1L_4jS_bronze = Cats_1Lincl;
  vector<Hadronic> H_1L_4jS;
  H_1L_4jS.push_back(GetHadronicRegion(4, 6)); // ge4j0b
  H_1L_4jS.push_back(GetHadronicRegion(4, 7)); // ge4jge1b

  Cats_1L_4jS_gold = Cats_1L_4jS_gold.CreateFitBinRegions(GetFitBin(6));
  Cats_1L_4jS_gold = Cats_1L_4jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_4jS_gold = Cats_1L_4jS_gold.CreateHadronicSRegions(H_1L_4jS);
  Cats_1L_4jS_gold = Cats_1L_4jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_4jS_gold = Cats_1L_4jS_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_4jS_silver = Cats_1L_4jS_silver.CreateFitBinRegions(GetFitBin(6));
  Cats_1L_4jS_silver = Cats_1L_4jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_4jS_silver = Cats_1L_4jS_silver.CreateHadronicSRegions(H_1L_4jS);
  Cats_1L_4jS_silver = Cats_1L_4jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_4jS_silver = Cats_1L_4jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_4jS_bronze = Cats_1L_4jS_bronze.CreateFitBinRegions(GetFitBin(6));
  Cats_1L_4jS_bronze = Cats_1L_4jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_4jS_bronze = Cats_1L_4jS_bronze.CreateHadronicSRegions(H_1L_4jS);
  Cats_1L_4jS_bronze = Cats_1L_4jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_4jS_bronze = Cats_1L_4jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L += Cats_1L_4jS_gold;
  Cats_1L += Cats_1L_4jS_silver;
  Cats_1L += Cats_1L_4jS_bronze;

  ///////////////////////////////////////////////

  return Cats_1L;
}

CategoryList CategoryTool::GetCategories_2L() const {
  CategoryList Cats_2L;

  return Cats_2L;
}

CategoryList CategoryTool::GetCategories_3L() const {
  CategoryList Cats_3L;

  return Cats_3L;
}
