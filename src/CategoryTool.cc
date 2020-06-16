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

CategoryList CategoryTool::GetCategories() const {

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
  Hadronic          H_0( 0,  0,  0, "0j0sv");
  Hadronic        H_1SV( 0,  0,  1, "0j1sv");
  Hadronic      H_ge2SV( 0,  0, -2, "0jge2sv");
  Hadronic      H_0Bjet(-1,  0,  0, "0bjet");
  H_0Bjet +=   Hadronic(-1,  0, -1);
  Hadronic      H_1Bjet(-1,  1,  0, "1bjet");
  H_1Bjet +=   Hadronic(-1,  1, -1);
  Hadronic    H_ge1Bjet(-1, -1,  0, "ge1bjet");
  H_ge1Bjet += Hadronic(-1, -1, -1);
  Hadronic    H_ge2Bjet(-1, -2,  0, "ge2bjet");
  H_ge2Bjet += Hadronic(-1, -2, -1);

  vector<const Hadronic*> H_ISR;
  H_ISR.push_back(&H_0Bjet);
  H_ISR.push_back(&H_ge1Bjet);

  vector<const Hadronic*> H_0L;
  H_0L.push_back(&H_1SV);
  H_0L.push_back(&H_ge2SV);
  H_0L.push_back(&H_0Bjet);
  H_0L.push_back(&H_1Bjet);
  H_0L.push_back(&H_ge2Bjet);
  
  vector<const Hadronic*> H_1L;
  H_1L.push_back(&H_0);
  H_1L.push_back(&H_1SV);
  H_1L.push_back(&H_ge2SV);
  H_1L.push_back(&H_0Bjet);
  H_1L.push_back(&H_1Bjet);
  H_1L.push_back(&H_ge2Bjet);

  vector<const Hadronic*> H_2LOS;
  H_2LOS.push_back(&H_0);
  H_2LOS.push_back(&H_1SV);
  H_2LOS.push_back(&H_0Bjet);
  H_2LOS.push_back(&H_1Bjet);
  H_2LOS.push_back(&H_ge2Bjet);

  vector<const Hadronic*> H_2LSS;
  H_2LSS.push_back(&H_0);
  H_2LSS.push_back(&H_1SV);
  H_2LSS.push_back(&H_0Bjet);
  H_2LSS.push_back(&H_1Bjet);
  H_2LSS.push_back(&H_ge2Bjet);

  vector<double> PTISR_0L;
  PTISR_0L.push_back(200.);
  PTISR_0L.push_back(500.);
  
  vector<double> PTISR_1L;
  PTISR_1L.push_back(200.);
  PTISR_1L.push_back(300.);

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
  BinsM_0L.push_back(400.);
  BinsM_0L.push_back(600.);
  vector<double> BinsR_0L;
  BinsR_0L.push_back(0.5);
  BinsR_0L.push_back(0.6);
  BinsR_0L.push_back(0.7);
  BinsR_0L.push_back(0.8);
  BinsR_0L.push_back(0.85);
  BinsR_0L.push_back(0.9);
  BinsR_0L.push_back(0.925);
  BinsR_0L.push_back(0.95);
  BinsR_0L.push_back(0.975);
  BinsR_0L.push_back(1.);
  BinsR_0L.push_back(1.05);
  FitBin Bin_0L(BinsR_0L, BinsM_0L);

  vector<double> BinsM_1L;
  BinsM_1L.push_back(0.);
  BinsM_1L.push_back(5.);
  BinsM_1L.push_back(10.);
  BinsM_1L.push_back(20.);
  BinsM_1L.push_back(50.);
  BinsM_1L.push_back(100.);
  BinsM_1L.push_back(200.);
  BinsM_1L.push_back(400.);
  BinsM_1L.push_back(600.);
  vector<double> BinsR_1L;
  BinsR_1L.push_back(0.8);
  BinsR_1L.push_back(0.85);
  BinsR_1L.push_back(0.9);
  BinsR_1L.push_back(0.925);
  BinsR_1L.push_back(0.95);
  BinsR_1L.push_back(0.975);
  BinsR_1L.push_back(1.);
  BinsR_1L.push_back(1.05);
  //FitBin Bin_1L(BinsR_1L, BinsM_1L);

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

  
  cout << "Building 0L" << endl;
  Cats_0L = Cats_0L.CreateFitBinRegions(Bin_0L); //
  Cats_0L = Cats_0L.CreateHadronicSRegions(H_0L);
  Cats_0L = Cats_0L.CreateHadronicISRRegions(H_ISR);
  Cats_0L = Cats_0L.CreateGenericRegions("PTISR", PTISR_2LOS);

  cout << "Building 1L" << endl;
  Cats_1L = Cats_1L.CreateFitBinRegions(Bin_0L);
  Cats_1L = Cats_1L.CreateLeptonIDRegions(3, 2);
  Cats_1L = Cats_1L.CreateHadronicSRegions(H_1L);
  Cats_1L = Cats_1L.CreateHadronicISRRegions(H_ISR);
  Cats_1L = Cats_1L.CreateGenericRegions("PTISR", PTISR_2LOS);

  cout << "Building 2L0L" << endl;
  Cats_2LOS = Cats_2LOS.CreateFitBinRegions(Bin_0L);
  Cats_2LOS = Cats_2LOS.CreateLeptonIDRegions(3, 2);
  Cats_2LOS = Cats_2LOS.CreateHadronicSRegions(H_2LOS);
  Cats_2LOS = Cats_2LOS.CreateHadronicISRRegions(H_ISR);
  Cats_2LOS = Cats_2LOS.CreateGenericRegions("PTISR", PTISR_2LOS);

  cout << "Building 2LSS" << endl;
  Cats_2LSS = Cats_2LSS.CreateFitBinRegions(Bin_0L);
  Cats_2LSS = Cats_2LSS.CreateLeptonIDRegions(3, 2);
  Cats_2LSS = Cats_2LSS.CreateHadronicSRegions(H_2LSS);
  Cats_2LSS = Cats_2LSS.CreateHadronicISRRegions(H_ISR);
  Cats_2LSS = Cats_2LSS.CreateGenericRegions("PTISR", PTISR_2LOS);
  
  //////////////////////////////////////////////////////

  CategoryList Categories;
  Categories += Cats_0L;
  Categories += Cats_1L;
  Categories += Cats_2LOS;
  Categories += Cats_2LSS;

  return Categories;
}
