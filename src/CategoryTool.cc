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

}

FitBin CategoryTool::GetFitBin(int index) const {
  vector<RBin> RBins;

  // for 0L SV regions
  if(index == 0){
    RBins.push_back(RBin(0.9,   0.95,  VD().a(0.).a(5.).a(10.).a(30.)));
    RBins.push_back(RBin(0.95,  1.0,   VD().a(0.).a(5.).a(10.).a(30.)));
  }
  
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
    RBins.push_back(RBin(0.6,   0.75,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(40.)));
    RBins.push_back(RBin(0.75,  0.85,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(40.)));
    RBins.push_back(RBin(0.85,  0.9,   VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(40.)));
    RBins.push_back(RBin(0.9,   0.925, VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(40.)));
    RBins.push_back(RBin(0.925, 0.95,  VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(40.)));
    RBins.push_back(RBin(0.95,  0.975, VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(40.)));
    RBins.push_back(RBin(0.975, 1.,    VD().a(0.).a(5.).a(10.).a(20.).a(30.).a(40.)));
  }

  // R3
  if(index == 3){
    RBins.push_back(RBin(0.6,   0.7,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(60.)));
    RBins.push_back(RBin(0.7,   0.8,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(60.)));
    RBins.push_back(RBin(0.8,   0.85,  VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(60.)));
    RBins.push_back(RBin(0.85,  0.9,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(60.)));
    RBins.push_back(RBin(0.9,   0.95,  VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(60.)));
    RBins.push_back(RBin(0.95,  1.0,   VD().a(0.).a(10.).a(20.).a(30.).a(50.).a(60.)));
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

////original R5 M6
if(index == 6){
    RBins.push_back(RBin(0.6,  0.65, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.65,  0.7, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.7,  0.75, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.75,  0.8, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.8,  0.85, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.85,  0.9, VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
    RBins.push_back(RBin(0.9,  1.0,  VD().a(0.).a(40.).a(60.).a(80.).a(100.).a(120.).a(200.)));
}


//  // R5 M6 - 4j - based on T2tt5000420
//  if(index == 6){
//    RBins.push_back(RBin(0.6,  0.63, VD().a(0.).a(50.).a(90.).a(150.).a(200.).a(250.).a(300.)));
//    RBins.push_back(RBin(0.63, 0.7,  VD().a(0.).a(60.).a(100.).a(140.).a(200).a(300.)));
//    RBins.push_back(RBin(0.7,  0.75, VD().a(0.).a(50.).a(130.).a(190.).a(300.)));
//    RBins.push_back(RBin(0.75,  0.8, VD().a(0.).a(50.).a(140.).a(300.)));
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
      Hadronic H(1, 1, -1, "1j1bge1sv");
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

    // exactly 1 jet 0b
    if(index == 8){
      Hadronic H(1, 0, 0, "1j0b");
      H += Hadronic(1, 0, -1);
      return H;
    }

    // exactly 1 jet 1b
    if(index == 9){
      Hadronic H(1, 1, 0, "1j1b");
      H += Hadronic(1, 1, -1);
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

    // C8 2 jet ge1b
    if(index == 3){
      Hadronic H(2, -1, 0, "2jge1b");
      H += Hadronic(2, -1, -1);
      return H;
    }
    
    // C8 2 jet 1b
    if(index == 4){
      Hadronic H(2, 1, 0, "2j1b");
      H += Hadronic(2, 1, -1);
      return H;
    }

    // C9 2 jet 2b
    if(index == 5){
      Hadronic H(2, 2, 0, "2j2b");
      H += Hadronic(2, 2, -1);
      return H;
    }

    // ge2j 0b
    if(index == 6){
      Hadronic H(-2, 0, 0, "ge2j0b");
      H += Hadronic(-2, 0, -1);
      return H;
    }

    // ge2j ge1b
    if(index == 7){
      Hadronic H(-2, -1, 0, "ge2jge1b");
      H += Hadronic(-2, -1, -1);
      return H;
    }

    //  ge2j 1b
    if(index == 8){
      Hadronic H(-2, 1, 0, "ge2j1b");
      H += Hadronic(-2, 1, -1);
      return H;
    }

    //  ge2j ge2b
    if(index == 9){
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
  cout << "Creating 0L Categories" << endl;

  // define leptonic categories
  // 0 L
  Leptonic L_0(LepList(), LepList(), "0");

  // Get hadronic ISR region lists - inclusive and b-separated
  vector<Hadronic> H_ISR_noB;
  vector<Hadronic> H_ISR_B;
  H_ISR_noB.push_back(GetHadronicRegion(1, 0)); // ge1j
  H_ISR_B.push_back(GetHadronicRegion(1, 6)); // ge1j0b
  H_ISR_B.push_back(GetHadronicRegion(1, 7)); // ge1jge1b
  
  // Get PTISR binning
  vector<double> PTISR_incl;
  PTISR_incl.push_back(300.);

  vector<double> PTISR_bin;
  PTISR_bin.push_back(300.);
  PTISR_bin.push_back(450.);

  // Get SV binning
  vector<double> SV_eta;
  SV_eta.push_back(0.);
  SV_eta.push_back(1.5);

  ///////////////////////////////////////////////

  cout << "Building 0L 0Sjet SV object regions" << endl;
  CategoryList Cats_0L_SV;
  Cats_0L_SV += Category(L_0, "Ch0L");
  vector<Hadronic> H_0L_SV;
  H_0L_SV.push_back(GetHadronicRegion(0, 3)); // 0j1sv
  H_0L_SV.push_back(GetHadronicRegion(0, 4)); // 0jge2sv

  Cats_0L_SV = Cats_0L_SV.CreateFitBinRegions(GetFitBin(0));
  Cats_0L_SV = Cats_0L_SV.CreateHadronicSRegions(H_0L_SV);
  Cats_0L_SV = Cats_0L_SV.CreateHadronicISRRegions(H_ISR_B);
  Cats_0L_SV = Cats_0L_SV.CreateGenericRegions("PTISR", PTISR_incl);
  Cats_0L_SV = Cats_0L_SV.CreateGenericRegions("SVeta", SV_eta);

  Cats_0L += Cats_0L_SV;

  ///////////////////////////////////////////////

  cout << "Building 0L 2j 0/1/2b object regions" << endl;
  CategoryList Cats_0L_2jS;
  Cats_0L_2jS += Category(L_0, "Ch0L");
  vector<Hadronic> H_0L_2jS;
  H_0L_2jS.push_back(GetHadronicRegion(2, 2)); // 2j0b
  H_0L_2jS.push_back(GetHadronicRegion(2, 4)); // 2j1b
  H_0L_2jS.push_back(GetHadronicRegion(2, 5)); // 2j2b

  Cats_0L_2jS = Cats_0L_2jS.CreateFitBinRegions(GetFitBin(4));
  Cats_0L_2jS = Cats_0L_2jS.CreateHadronicSRegions(H_0L_2jS);
  Cats_0L_2jS = Cats_0L_2jS.CreateHadronicISRRegions(H_ISR_B);
  Cats_0L_2jS = Cats_0L_2jS.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_0L += Cats_0L_2jS;

  ///////////////////////////////////////////////

  cout << "Building 0L 3j 0/1/2b object regions" << endl;
  CategoryList Cats_0L_3jS;
  Cats_0L_3jS += Category(L_0, "Ch0L");
  vector<Hadronic> H_0L_3jS;
  H_0L_3jS.push_back(GetHadronicRegion(3, 2)); // 3j0b
  H_0L_3jS.push_back(GetHadronicRegion(3, 4)); // 3j1b
  H_0L_3jS.push_back(GetHadronicRegion(3, 5)); // 3j2b

  Cats_0L_3jS = Cats_0L_3jS.CreateFitBinRegions(GetFitBin(4));
  Cats_0L_3jS = Cats_0L_3jS.CreateHadronicSRegions(H_0L_3jS);
  Cats_0L_3jS = Cats_0L_3jS.CreateHadronicISRRegions(H_ISR_B);
  Cats_0L_3jS = Cats_0L_3jS.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_0L += Cats_0L_3jS;

  ///////////////////////////////////////////////

  cout << "Building 0L 4j 0/1/2b object regions" << endl;
  CategoryList Cats_0L_4jS;
  Cats_0L_4jS += Category(L_0, "Ch0L");
  vector<Hadronic> H_0L_4jS;
  H_0L_4jS.push_back(GetHadronicRegion(4, 2)); // 4j0b
  H_0L_4jS.push_back(GetHadronicRegion(4, 4)); // 4j1b
  H_0L_4jS.push_back(GetHadronicRegion(4, 5)); // 4j2b

  Cats_0L_4jS = Cats_0L_4jS.CreateFitBinRegions(GetFitBin(5));
  Cats_0L_4jS = Cats_0L_4jS.CreateHadronicSRegions(H_0L_4jS);
  Cats_0L_4jS = Cats_0L_4jS.CreateHadronicISRRegions(H_ISR_B);
  Cats_0L_4jS = Cats_0L_4jS.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_0L += Cats_0L_4jS;

  ///////////////////////////////////////////////

  cout << "Building 0L 5j 0/ge1b object regions" << endl;
  CategoryList Cats_0L_5jS;
  Cats_0L_5jS += Category(L_0, "Ch0L");
  vector<Hadronic> H_0L_5jS;
  H_0L_5jS.push_back(GetHadronicRegion(5, 6)); // ge5j0b
  H_0L_5jS.push_back(GetHadronicRegion(5, 7)); // ge5jge1b

  Cats_0L_5jS = Cats_0L_5jS.CreateFitBinRegions(GetFitBin(6));
  Cats_0L_5jS = Cats_0L_5jS.CreateHadronicSRegions(H_0L_5jS);
  Cats_0L_5jS = Cats_0L_5jS.CreateHadronicISRRegions(H_ISR_B);
  Cats_0L_5jS = Cats_0L_5jS.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_0L += Cats_0L_5jS;

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

  vector<double> PTISR_bin;
  PTISR_bin.push_back(200.);
  PTISR_bin.push_back(300.);

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
  CategoryList Cats_1L_0S_bronze = Cats_1Lincl;
  vector<Hadronic> H_1L_0S;
  H_1L_0S.push_back(GetHadronicRegion(0, 1)); // 0j0sv

  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateFitBinRegions(GetFitBin(1)); // R1
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateHadronicSRegions(H_1L_0S);
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_0S_gold = Cats_1L_0S_gold.CreateGenericRegions("PTISR", PTISR_bin);

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
  CategoryList Cats_1L_SV0S_bronze = Cats_1Lincl;
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
  Cats_1L_1j0svS_gold = Cats_1L_1j0svS_gold.CreateGenericRegions("PTISR", PTISR_bin);

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
  H_1L_2jS.push_back(GetHadronicRegion(2, 4)); // 2j1b
  H_1L_2jS.push_back(GetHadronicRegion(2, 5)); // 2j2b

  vector<Hadronic> H_1L_2jS_bkg;
  H_1L_2jS_bkg.push_back(GetHadronicRegion(2, 2)); // 2j0b
  H_1L_2jS_bkg.push_back(GetHadronicRegion(2, 3)); // 2jge1b

  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateFitBinRegions(GetFitBin(4));
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateHadronicSRegions(H_1L_2jS);
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_2jS_gold = Cats_1L_2jS_gold.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateFitBinRegions(GetFitBin(4));
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateHadronicSRegions(H_1L_2jS_bkg);
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_2jS_silver = Cats_1L_2jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateFitBinRegions(GetFitBin(4));
  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_2jS_bronze = Cats_1L_2jS_bronze.CreateHadronicSRegions(H_1L_2jS_bkg);
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

  vector<Hadronic> H_1L_3jS_bkg;
  H_1L_3jS_bkg.push_back(GetHadronicRegion(3, 2)); // 3j0b
  H_1L_3jS_bkg.push_back(GetHadronicRegion(3, 3)); // 3jge1b

  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateFitBinRegions(GetFitBin(5));
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateHadronicSRegions(H_1L_3jS);
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_1L_3jS_gold = Cats_1L_3jS_gold.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateFitBinRegions(GetFitBin(5));
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateHadronicSRegions(H_1L_3jS_bkg);
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_1L_3jS_silver = Cats_1L_3jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateFitBinRegions(GetFitBin(5));
  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_1L_3jS_bronze = Cats_1L_3jS_bronze.CreateHadronicSRegions(H_1L_3jS_bkg);
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
  Cats_1L_4jS_gold = Cats_1L_4jS_gold.CreateGenericRegions("PTISR", PTISR_bin);

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
  cout << "Creating 2L Categories" << endl;

  // define leptons
  Lep elp(kElectron, kPos);
  Lep elm(kElectron, kNeg);
  Lep mup(kMuon, kPos);
  Lep mum(kMuon, kNeg);

  // 2 L, opposite hemisphere, charge separated
  Leptonic    L_OS_el_el(LepList().Add(elp), LepList().Add(elm), "OSel^el");
  Leptonic    L_SS_el_el(LepList().Add(elp), LepList().Add(elp), "SSel^el");
  L_SS_el_el += Leptonic(LepList().Add(elm), LepList().Add(elm));
  Leptonic    L_OS_mu_mu(LepList().Add(mup), LepList().Add(mum), "OSmu^mu");
  Leptonic    L_SS_mu_mu(LepList().Add(mup), LepList().Add(mup), "SSmu^mu");
  L_SS_mu_mu += Leptonic(LepList().Add(mum), LepList().Add(mum));
  Leptonic    L_OS_el_mu(LepList().Add(elp), LepList().Add(mum), "OSel^mu");
  L_OS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mup));
  Leptonic    L_SS_el_mu(LepList().Add(elp), LepList().Add(mup), "SSel^mu");
  L_SS_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mum));
  // 2 L, same hemisphere, charge separated
  Leptonic    L_OS_elel_0(LepList().Add(elp).Add(elm), LepList(), "OSelel^0");
  Leptonic    L_SS_elel_0(LepList().Add(elp).Add(elp), LepList(), "SSelel^0");
  L_SS_elel_0 += Leptonic(LepList().Add(elm).Add(elm), LepList());
  Leptonic    L_OS_mumu_0(LepList().Add(mup).Add(mum), LepList(), "OSmumu^0");
  Leptonic    L_SS_mumu_0(LepList().Add(mup).Add(mup), LepList(), "SSmumu^0");
  L_SS_mumu_0 += Leptonic(LepList().Add(mum).Add(mum), LepList());
  Leptonic    L_OS_elmu_0(LepList().Add(elp).Add(mum), LepList(), "OSelmu^0");
  L_OS_elmu_0 += Leptonic(LepList().Add(elm).Add(mup), LepList());
  Leptonic    L_SS_elmu_0(LepList().Add(elp).Add(mup), LepList(), "SSelmu^0");
  L_SS_elmu_0 += Leptonic(LepList().Add(elm).Add(mum), LepList());
  CategoryList Cats_2Lchargehem;
  Cats_2Lchargehem += Category(L_OS_el_el, "Ch2L");
  Cats_2Lchargehem += Category(L_SS_el_el, "Ch2L");
  Cats_2Lchargehem += Category(L_OS_mu_mu, "Ch2L");
  Cats_2Lchargehem += Category(L_SS_mu_mu, "Ch2L");
  Cats_2Lchargehem += Category(L_OS_el_mu, "Ch2L");
  Cats_2Lchargehem += Category(L_SS_el_mu, "Ch2L");
  Cats_2Lchargehem += Category(L_OS_elel_0, "Ch2L");
  Cats_2Lchargehem += Category(L_SS_elel_0, "Ch2L");
  Cats_2Lchargehem += Category(L_OS_mumu_0, "Ch2L");
  Cats_2Lchargehem += Category(L_SS_mumu_0, "Ch2L");
  Cats_2Lchargehem += Category(L_OS_elmu_0, "Ch2L");
  Cats_2Lchargehem += Category(L_SS_elmu_0, "Ch2L");

  // 2 L, hemipshere separated only
  Leptonic    L_el_el(LepList().Add(elp), LepList().Add(elm), "el^el");
  L_el_el += Leptonic(LepList().Add(elp), LepList().Add(elp));
  L_el_el += Leptonic(LepList().Add(elm), LepList().Add(elm));
  Leptonic    L_mu_mu(LepList().Add(mup), LepList().Add(mum), "mu^mu");
  L_mu_mu += Leptonic(LepList().Add(mup), LepList().Add(mup));
  L_mu_mu += Leptonic(LepList().Add(mum), LepList().Add(mum));
  Leptonic    L_el_mu(LepList().Add(elp), LepList().Add(mum), "el^mu");
  L_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mup));
  L_el_mu += Leptonic(LepList().Add(elp), LepList().Add(mup));
  L_el_mu += Leptonic(LepList().Add(elm), LepList().Add(mum));
  Leptonic    L_elel_0(LepList().Add(elp).Add(elm), LepList(), "elel^0");
  L_elel_0 += Leptonic(LepList().Add(elp).Add(elp), LepList());
  L_elel_0 += Leptonic(LepList().Add(elm).Add(elm), LepList());
  Leptonic    L_mumu_0(LepList().Add(mup).Add(mum), LepList(), "mumu^0");
  L_mumu_0 += Leptonic(LepList().Add(mup).Add(mup), LepList());
  L_mumu_0 += Leptonic(LepList().Add(mum).Add(mum), LepList());
  Leptonic    L_elmu_0(LepList().Add(elp).Add(mum), LepList(), "elmu^0");
  L_elmu_0 += Leptonic(LepList().Add(elm).Add(mup), LepList());
  L_elmu_0 += Leptonic(LepList().Add(elp).Add(mup), LepList());
  L_elmu_0 += Leptonic(LepList().Add(elm).Add(mum), LepList());
  CategoryList Cats_2Lhem;
  Cats_2Lhem += Category(L_el_el, "Ch2L");
  Cats_2Lhem += Category(L_mu_mu, "Ch2L");
  Cats_2Lhem += Category(L_el_mu, "Ch2L");
  Cats_2Lhem += Category(L_elel_0, "Ch2L");
  Cats_2Lhem += Category(L_mumu_0, "Ch2L");
  Cats_2Lhem += Category(L_elmu_0, "Ch2L");
  
  // 2 L, charge separated only
  Leptonic    L_OS_elel(LepList().Add(elp), LepList().Add(elm), "OSelel");
  L_OS_elel += Leptonic(LepList().Add(elp).Add(elm), LepList());
  Leptonic    L_SS_elel(LepList().Add(elp), LepList().Add(elp), "SSelel");
  L_SS_elel += Leptonic(LepList().Add(elm), LepList().Add(elm));
  L_SS_elel += Leptonic(LepList().Add(elp).Add(elp), LepList());
  L_SS_elel += Leptonic(LepList().Add(elm).Add(elm), LepList());
  Leptonic    L_OS_mumu(LepList().Add(mup), LepList().Add(mum), "OSmumu");
  L_OS_mumu += Leptonic(LepList().Add(mup).Add(mum), LepList());
  Leptonic    L_SS_mumu(LepList().Add(mup), LepList().Add(mup), "SSmumu");
  L_SS_mumu += Leptonic(LepList().Add(mum), LepList().Add(mum));
  L_SS_mumu += Leptonic(LepList().Add(mup).Add(mup), LepList());
  L_SS_mumu += Leptonic(LepList().Add(mum).Add(mum), LepList());
  Leptonic    L_OS_elmu(LepList().Add(elp), LepList().Add(mum), "OSelmu");
  L_OS_elmu += Leptonic(LepList().Add(elm), LepList().Add(mup));
  L_OS_elmu += Leptonic(LepList().Add(elp).Add(mum), LepList());
  L_OS_elmu += Leptonic(LepList().Add(elm).Add(mup), LepList());
  Leptonic    L_SS_elmu(LepList().Add(elp), LepList().Add(mup), "SSelmu");
  L_SS_elmu += Leptonic(LepList().Add(elm), LepList().Add(mum));
  L_SS_elmu += Leptonic(LepList().Add(elp).Add(mup), LepList());
  L_SS_elmu += Leptonic(LepList().Add(elm).Add(mum), LepList());
  CategoryList Cats_2Lcharge;
  Cats_2Lcharge += Category(L_OS_elel, "Ch2L");
  Cats_2Lcharge += Category(L_SS_elel, "Ch2L");
  Cats_2Lcharge += Category(L_OS_mumu, "Ch2L");
  Cats_2Lcharge += Category(L_SS_mumu, "Ch2L");
  Cats_2Lcharge += Category(L_OS_elmu, "Ch2L");
  Cats_2Lcharge += Category(L_SS_elmu, "Ch2L");

  // 2L, flavor separated only
  Leptonic    L_elel(LepList().Add(elp), LepList().Add(elm), "elel");
  L_elel += Leptonic(LepList().Add(elp).Add(elm), LepList());
  L_elel += Leptonic(LepList().Add(elp), LepList().Add(elp));
  L_elel += Leptonic(LepList().Add(elm), LepList().Add(elm));
  L_elel += Leptonic(LepList().Add(elp).Add(elp), LepList());
  L_elel += Leptonic(LepList().Add(elm).Add(elm), LepList());
  Leptonic    L_mumu(LepList().Add(mup), LepList().Add(mum), "mumu");
  L_mumu += Leptonic(LepList().Add(mup).Add(mum), LepList());
  L_mumu += Leptonic(LepList().Add(mup), LepList().Add(mup));
  L_mumu += Leptonic(LepList().Add(mum), LepList().Add(mum));
  L_mumu += Leptonic(LepList().Add(mup).Add(mup), LepList());
  L_mumu += Leptonic(LepList().Add(mum).Add(mum), LepList());
  Leptonic    L_elmu(LepList().Add(elp), LepList().Add(mum), "elmu");
  L_elmu += Leptonic(LepList().Add(elm), LepList().Add(mup));
  L_elmu += Leptonic(LepList().Add(elp).Add(mum), LepList());
  L_elmu += Leptonic(LepList().Add(elm).Add(mup), LepList());
  L_elmu += Leptonic(LepList().Add(elp), LepList().Add(mup), "elmu");
  L_elmu += Leptonic(LepList().Add(elm), LepList().Add(mum));
  L_elmu += Leptonic(LepList().Add(elp).Add(mup), LepList());
  L_elmu += Leptonic(LepList().Add(elm).Add(mum), LepList());
  CategoryList Cats_2Lflavor;
  Cats_2Lflavor += Category(L_elel, "Ch2L");
  Cats_2Lflavor += Category(L_mumu, "Ch2L");
  Cats_2Lflavor += Category(L_elmu, "Ch2L");

  // Get hadronic ISR region lists - inclusive and b-separated
  vector<Hadronic> H_ISR_noB;
  vector<Hadronic> H_ISR_B;
  H_ISR_noB.push_back(GetHadronicRegion(1, 0)); // ge1j
  H_ISR_B.push_back(GetHadronicRegion(1, 6)); // ge1j0b
  H_ISR_B.push_back(GetHadronicRegion(1, 7)); // ge1jge1b
  
  // Get PTISR binning
  vector<double> PTISR_incl;
  PTISR_incl.push_back(200.);

  vector<double> PTISR_bin;
  PTISR_bin.push_back(200.);
  PTISR_bin.push_back(300.);

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
  
  cout << "Building 2L 0S object regions" << endl;
  CategoryList Cats_2L_0S_gold = Cats_2Lcharge;
  CategoryList Cats_2L_0S_silver = Cats_2Lflavor;
  CategoryList Cats_2L_0S_bronze = Cats_2Lflavor;
  vector<Hadronic> H_2L_0S;
  H_2L_0S.push_back(GetHadronicRegion(0, 1)); // 0j0sv

  Cats_2L_0S_gold = Cats_2L_0S_gold.CreateFitBinRegions(GetFitBin(1)); // R1
  Cats_2L_0S_gold = Cats_2L_0S_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_2L_0S_gold = Cats_2L_0S_gold.CreateHadronicSRegions(H_2L_0S);
  Cats_2L_0S_gold = Cats_2L_0S_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_2L_0S_gold = Cats_2L_0S_gold.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_2L_0S_silver = Cats_2L_0S_silver.CreateFitBinRegions(GetFitBin(1));
  Cats_2L_0S_silver = Cats_2L_0S_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_2L_0S_silver = Cats_2L_0S_silver.CreateHadronicSRegions(H_2L_0S);
  Cats_2L_0S_silver = Cats_2L_0S_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_0S_silver = Cats_2L_0S_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L_0S_bronze = Cats_2L_0S_bronze.CreateFitBinRegions(GetFitBin(1));
  Cats_2L_0S_bronze = Cats_2L_0S_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_2L_0S_bronze = Cats_2L_0S_bronze.CreateHadronicSRegions(H_2L_0S);
  Cats_2L_0S_bronze = Cats_2L_0S_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_0S_bronze = Cats_2L_0S_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L += Cats_2L_0S_gold;
  Cats_2L += Cats_2L_0S_silver;
  Cats_2L += Cats_2L_0S_bronze;

  ///////////////////////////////////////////////

  cout << "Building 2L 0Sjet SV object regions" << endl;
  CategoryList Cats_2L_SV0S_gold = Cats_2Lcharge;
  CategoryList Cats_2L_SV0S_silver = Cats_2Lflavor;
  CategoryList Cats_2L_SV0S_bronze = Cats_2Lflavor;
  vector<Hadronic> H_2L_SV0S;
  H_2L_SV0S.push_back(GetHadronicRegion(0, 2)); // 0jge1sv

  Cats_2L_SV0S_gold = Cats_2L_SV0S_gold.CreateFitBinRegions(GetFitBin(2));
  Cats_2L_SV0S_gold = Cats_2L_SV0S_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_2L_SV0S_gold = Cats_2L_SV0S_gold.CreateHadronicSRegions(H_2L_SV0S);
  Cats_2L_SV0S_gold = Cats_2L_SV0S_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_2L_SV0S_gold = Cats_2L_SV0S_gold.CreateGenericRegions("PTISR", PTISR_incl);
  Cats_2L_SV0S_gold = Cats_2L_SV0S_gold.CreateGenericRegions("SVeta", SV_eta);

  Cats_2L_SV0S_silver = Cats_2L_SV0S_silver.CreateFitBinRegions(GetFitBin(2));
  Cats_2L_SV0S_silver = Cats_2L_SV0S_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_2L_SV0S_silver = Cats_2L_SV0S_silver.CreateHadronicSRegions(H_2L_SV0S);
  Cats_2L_SV0S_silver = Cats_2L_SV0S_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_SV0S_silver = Cats_2L_SV0S_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L_SV0S_bronze = Cats_2L_SV0S_bronze.CreateFitBinRegions(GetFitBin(2));
  Cats_2L_SV0S_bronze = Cats_2L_SV0S_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_2L_SV0S_bronze = Cats_2L_SV0S_bronze.CreateHadronicSRegions(H_2L_SV0S);
  Cats_2L_SV0S_bronze = Cats_2L_SV0S_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_SV0S_bronze = Cats_2L_SV0S_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L += Cats_2L_SV0S_gold;
  Cats_2L += Cats_2L_SV0S_silver;
  Cats_2L += Cats_2L_SV0S_bronze;

  ///////////////////////////////////////////////

  cout << "Building 2L 1j 0/1b object regions" << endl;
  CategoryList Cats_2L_1jS_gold = Cats_2Lhem;
  CategoryList Cats_2L_1jS_silver = Cats_2Lflavor;
  CategoryList Cats_2L_1jS_bronze = Cats_2Lflavor;
  vector<Hadronic> H_2L_1jS;
  H_2L_1jS.push_back(GetHadronicRegion(1, 8)); // 1j0b
  H_2L_1jS.push_back(GetHadronicRegion(1, 9)); // 1j1b

  Cats_2L_1jS_gold = Cats_2L_1jS_gold.CreateFitBinRegions(GetFitBin(3));
  Cats_2L_1jS_gold = Cats_2L_1jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_2L_1jS_gold = Cats_2L_1jS_gold.CreateHadronicSRegions(H_2L_1jS);
  Cats_2L_1jS_gold = Cats_2L_1jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_2L_1jS_gold = Cats_2L_1jS_gold.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_2L_1jS_silver = Cats_2L_1jS_silver.CreateFitBinRegions(GetFitBin(3));
  Cats_2L_1jS_silver = Cats_2L_1jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_2L_1jS_silver = Cats_2L_1jS_silver.CreateHadronicSRegions(H_2L_1jS);
  Cats_2L_1jS_silver = Cats_2L_1jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_1jS_silver = Cats_2L_1jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L_1jS_bronze = Cats_2L_1jS_bronze.CreateFitBinRegions(GetFitBin(3));
  Cats_2L_1jS_bronze = Cats_2L_1jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_2L_1jS_bronze = Cats_2L_1jS_bronze.CreateHadronicSRegions(H_2L_1jS);
  Cats_2L_1jS_bronze = Cats_2L_1jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_1jS_bronze = Cats_2L_1jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L += Cats_2L_1jS_gold;
  Cats_2L += Cats_2L_1jS_silver;
  Cats_2L += Cats_2L_1jS_bronze;

  ///////////////////////////////////////////////

  cout << "Building 2L 2j 0/1/2b object regions" << endl;
  CategoryList Cats_2L_2jS_gold = Cats_2Lchargehem;
  CategoryList Cats_2L_2jS_silver = Cats_2Lflavor;
  CategoryList Cats_2L_2jS_bronze = Cats_2Lflavor;
  vector<Hadronic> H_2L_2jS;
  H_2L_2jS.push_back(GetHadronicRegion(2, 2)); // 2j0b
  H_2L_2jS.push_back(GetHadronicRegion(2, 4)); // 2j1b
  H_2L_2jS.push_back(GetHadronicRegion(2, 5)); // 2j2b

  vector<Hadronic> H_2L_2jS_bkg;
  H_2L_2jS_bkg.push_back(GetHadronicRegion(2, 2)); // 2j0b
  H_2L_2jS_bkg.push_back(GetHadronicRegion(2, 3)); // 2jge1b

  Cats_2L_2jS_gold = Cats_2L_2jS_gold.CreateFitBinRegions(GetFitBin(4));
  Cats_2L_2jS_gold = Cats_2L_2jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_2L_2jS_gold = Cats_2L_2jS_gold.CreateHadronicSRegions(H_2L_2jS);
  Cats_2L_2jS_gold = Cats_2L_2jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_2L_2jS_gold = Cats_2L_2jS_gold.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_2L_2jS_silver = Cats_2L_2jS_silver.CreateFitBinRegions(GetFitBin(4));
  Cats_2L_2jS_silver = Cats_2L_2jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_2L_2jS_silver = Cats_2L_2jS_silver.CreateHadronicSRegions(H_2L_2jS_bkg);
  Cats_2L_2jS_silver = Cats_2L_2jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_2jS_silver = Cats_2L_2jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L_2jS_bronze = Cats_2L_2jS_bronze.CreateFitBinRegions(GetFitBin(4));
  Cats_2L_2jS_bronze = Cats_2L_2jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_2L_2jS_bronze = Cats_2L_2jS_bronze.CreateHadronicSRegions(H_2L_2jS_bkg);
  Cats_2L_2jS_bronze = Cats_2L_2jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_2jS_bronze = Cats_2L_2jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L += Cats_2L_2jS_gold;
  Cats_2L += Cats_2L_2jS_silver;
  Cats_2L += Cats_2L_2jS_bronze;

  ///////////////////////////////////////////////

  cout << "Building 2L ge3j 0/ge1b object regions" << endl;
  CategoryList Cats_2L_3jS_gold = Cats_2Lchargehem;
  CategoryList Cats_2L_3jS_silver = Cats_2Lflavor;
  CategoryList Cats_2L_3jS_bronze = Cats_2Lflavor;
  vector<Hadronic> H_2L_3jS;
  H_2L_3jS.push_back(GetHadronicRegion(3, 6)); // ge3j0b
  H_2L_3jS.push_back(GetHadronicRegion(3, 7)); // ge3jge1b

  Cats_2L_3jS_gold = Cats_2L_3jS_gold.CreateFitBinRegions(GetFitBin(5));
  Cats_2L_3jS_gold = Cats_2L_3jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_2L_3jS_gold = Cats_2L_3jS_gold.CreateHadronicSRegions(H_2L_3jS);
  Cats_2L_3jS_gold = Cats_2L_3jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_2L_3jS_gold = Cats_2L_3jS_gold.CreateGenericRegions("PTISR", PTISR_bin);

  Cats_2L_3jS_silver = Cats_2L_3jS_silver.CreateFitBinRegions(GetFitBin(5));
  Cats_2L_3jS_silver = Cats_2L_3jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_2L_3jS_silver = Cats_2L_3jS_silver.CreateHadronicSRegions(H_2L_3jS);
  Cats_2L_3jS_silver = Cats_2L_3jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_3jS_silver = Cats_2L_3jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L_3jS_bronze = Cats_2L_3jS_bronze.CreateFitBinRegions(GetFitBin(5));
  Cats_2L_3jS_bronze = Cats_2L_3jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_2L_3jS_bronze = Cats_2L_3jS_bronze.CreateHadronicSRegions(H_2L_3jS);
  Cats_2L_3jS_bronze = Cats_2L_3jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_2L_3jS_bronze = Cats_2L_3jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_2L += Cats_2L_3jS_gold;
  Cats_2L += Cats_2L_3jS_silver;
  Cats_2L += Cats_2L_3jS_bronze;

  ///////////////////////////////////////////////
 
  return Cats_2L;
}

CategoryList CategoryTool::GetCategories_3L() const {
  CategoryList Cats_3L;
  cout << "Creating 3L Categories" << endl;

  // define leptons
  Lep elp(kElectron, kPos);
  Lep elm(kElectron, kNeg);
  Lep mup(kMuon, kPos);
  Lep mum(kMuon, kNeg);

  // 3 L, elelel - OSSF hem separated
  Leptonic    L_OSSF_elel_el(LepList().Add(elp).Add(elm), LepList().Add(elp), "OSSFelel^el");
  L_OSSF_elel_el += Leptonic(LepList().Add(elp).Add(elm), LepList().Add(elm));
  L_OSSF_elel_el += Leptonic(LepList().Add(elp).Add(elm).Add(elp), LepList());
  L_OSSF_elel_el += Leptonic(LepList().Add(elp).Add(elm).Add(elm), LepList());
  Leptonic    L_nOSSF_elel_el(LepList().Add(elp).Add(elp), LepList().Add(elp), "nOSSFelel^el");
  L_nOSSF_elel_el += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(elm));
  L_nOSSF_elel_el += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(elm));
  L_nOSSF_elel_el += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(elp));
  L_nOSSF_elel_el += Leptonic(LepList().Add(elp).Add(elp).Add(elp), LepList());
  L_nOSSF_elel_el += Leptonic(LepList().Add(elm).Add(elm).Add(elm), LepList());
  // 3 L, elelel - OSSF incl
  Leptonic    L_OSSF_elelel(LepList().Add(elp).Add(elm), LepList().Add(elp), "OSSFelelel");
  L_OSSF_elelel += Leptonic(LepList().Add(elp).Add(elm), LepList().Add(elm));
  L_OSSF_elelel += Leptonic(LepList().Add(elp).Add(elm).Add(elp), LepList());
  L_OSSF_elelel += Leptonic(LepList().Add(elp).Add(elm).Add(elm), LepList());
  L_OSSF_elelel += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(elm));
  L_OSSF_elelel += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(elp));
  Leptonic    L_nOSSF_elelel(LepList().Add(elp).Add(elp), LepList().Add(elp), "nOSSFelelel");
  L_nOSSF_elelel += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(elm));
  L_nOSSF_elelel += Leptonic(LepList().Add(elp).Add(elp).Add(elp), LepList());
  L_nOSSF_elelel += Leptonic(LepList().Add(elm).Add(elm).Add(elm), LepList());
  // 3 L, elelel - incl
  Leptonic    L_elelel(LepList().Add(elp).Add(elm), LepList().Add(elp), "elelel");
  L_elelel += Leptonic(LepList().Add(elp).Add(elm), LepList().Add(elm));
  L_elelel += Leptonic(LepList().Add(elp).Add(elm).Add(elp), LepList());
  L_elelel += Leptonic(LepList().Add(elp).Add(elm).Add(elm), LepList());
  L_elelel += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(elm));
  L_elelel += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(elp));
  L_elelel += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(elp));
  L_elelel += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(elm));
  L_elelel += Leptonic(LepList().Add(elp).Add(elp).Add(elp), LepList());
  L_elelel += Leptonic(LepList().Add(elm).Add(elm).Add(elm), LepList());

  // 3 L, mumumu - OSSF hem separated
  Leptonic    L_OSSF_mumu_mu(LepList().Add(mup).Add(mum), LepList().Add(mup), "OSSFmumu^mu");
  L_OSSF_mumu_mu += Leptonic(LepList().Add(mup).Add(mum), LepList().Add(mum));
  L_OSSF_mumu_mu += Leptonic(LepList().Add(mup).Add(mum).Add(mup), LepList());
  L_OSSF_mumu_mu += Leptonic(LepList().Add(mup).Add(mum).Add(mum), LepList());
  Leptonic    L_nOSSF_mumu_mu(LepList().Add(mup).Add(mup), LepList().Add(mup), "nOSSFmumu^mu");
  L_nOSSF_mumu_mu += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(mum));
  L_nOSSF_mumu_mu += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(mum));
  L_nOSSF_mumu_mu += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(mup));
  L_nOSSF_mumu_mu += Leptonic(LepList().Add(mup).Add(mup).Add(mup), LepList());
  L_nOSSF_mumu_mu += Leptonic(LepList().Add(mum).Add(mum).Add(mum), LepList());
  // 3 L, mumumu - OSSF incl
  Leptonic    L_OSSF_mumumu(LepList().Add(mup).Add(mum), LepList().Add(mup), "OSSFmumumu");
  L_OSSF_mumumu += Leptonic(LepList().Add(mup).Add(mum), LepList().Add(mum));
  L_OSSF_mumumu += Leptonic(LepList().Add(mup).Add(mum).Add(mup), LepList());
  L_OSSF_mumumu += Leptonic(LepList().Add(mup).Add(mum).Add(mum), LepList());
  L_OSSF_mumumu += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(mum));
  L_OSSF_mumumu += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(mup));
  Leptonic    L_nOSSF_mumumu(LepList().Add(mup).Add(mup), LepList().Add(mup), "nOSSFmumumu");
  L_nOSSF_mumumu += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(mum));
  L_nOSSF_mumumu += Leptonic(LepList().Add(mup).Add(mup).Add(mup), LepList());
  L_nOSSF_mumumu += Leptonic(LepList().Add(mum).Add(mum).Add(mum), LepList());
  // 3 L, mumumu - incl
  Leptonic    L_mumumu(LepList().Add(mup).Add(mum), LepList().Add(mup), "mumumu");
  L_mumumu += Leptonic(LepList().Add(mup).Add(mum), LepList().Add(mum));
  L_mumumu += Leptonic(LepList().Add(mup).Add(mum).Add(mup), LepList());
  L_mumumu += Leptonic(LepList().Add(mup).Add(mum).Add(mum), LepList());
  L_mumumu += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(mum));
  L_mumumu += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(mup));
  L_mumumu += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(mup));
  L_mumumu += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(mum));
  L_mumumu += Leptonic(LepList().Add(mup).Add(mup).Add(mup), LepList());
  L_mumumu += Leptonic(LepList().Add(mum).Add(mum).Add(mum), LepList());

  // 3 L, elelmu - OSSF hem separated
  Leptonic    L_OSSF_elel_mu(LepList().Add(elp).Add(elm), LepList().Add(mup), "OSSFelel^mu");
  L_OSSF_elel_mu += Leptonic(LepList().Add(elp).Add(elm), LepList().Add(mum));
  L_OSSF_elel_mu += Leptonic(LepList().Add(elp).Add(elm).Add(mup), LepList());
  L_OSSF_elel_mu += Leptonic(LepList().Add(elp).Add(elm).Add(mum), LepList());
  Leptonic    L_nOSSF_elel_mu(LepList().Add(elp).Add(elp), LepList().Add(mup), "nOSSFelel^mu");
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(mum));
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(mum));
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(mup));
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elp).Add(elp).Add(mup), LepList());
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elm).Add(elm).Add(mum), LepList());
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elp).Add(elp).Add(mum), LepList());
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elm).Add(elm).Add(mup), LepList());
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elp).Add(mup), LepList().Add(elm));
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elp).Add(mum), LepList().Add(elm));
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elm).Add(mup), LepList().Add(elp));
  L_nOSSF_elel_mu += Leptonic(LepList().Add(elm).Add(mum), LepList().Add(elp));
  // 3 L, elelmu - OSSF incl
  Leptonic    L_OSSF_elelmu(LepList().Add(elp).Add(elm), LepList().Add(mup), "OSSFelelmu");
  L_OSSF_elelmu += Leptonic(LepList().Add(elp).Add(elm), LepList().Add(mum));
  L_OSSF_elelmu += Leptonic(LepList().Add(elp).Add(elm).Add(mup), LepList());
  L_OSSF_elelmu += Leptonic(LepList().Add(elp).Add(elm).Add(mum), LepList());
  L_OSSF_elelmu += Leptonic(LepList().Add(elp).Add(mup), LepList().Add(elm));
  L_OSSF_elelmu += Leptonic(LepList().Add(elp).Add(mum), LepList().Add(elm));
  L_OSSF_elelmu += Leptonic(LepList().Add(elm).Add(mup), LepList().Add(elp));
  L_OSSF_elelmu += Leptonic(LepList().Add(elm).Add(mum), LepList().Add(elp));
  Leptonic    L_nOSSF_elelmu(LepList().Add(elp).Add(elp), LepList().Add(mup), "nOSSFelelmu");
  L_nOSSF_elelmu += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(mum));
  L_nOSSF_elelmu += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(mum));
  L_nOSSF_elelmu += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(mup));
  L_nOSSF_elelmu += Leptonic(LepList().Add(elp).Add(elp).Add(mup), LepList());
  L_nOSSF_elelmu += Leptonic(LepList().Add(elm).Add(elm).Add(mum), LepList());
  L_nOSSF_elelmu += Leptonic(LepList().Add(elp).Add(elp).Add(mum), LepList());
  L_nOSSF_elelmu += Leptonic(LepList().Add(elm).Add(elm).Add(mup), LepList());
  // 3 L, elelmu - incl
  Leptonic    L_elelmu(LepList().Add(elp).Add(elm), LepList().Add(mup), "elelmu");
  L_elelmu += Leptonic(LepList().Add(elp).Add(elm), LepList().Add(mum));
  L_elelmu += Leptonic(LepList().Add(elp).Add(elm).Add(mup), LepList());
  L_elelmu += Leptonic(LepList().Add(elp).Add(elm).Add(mum), LepList());
  L_elelmu += Leptonic(LepList().Add(elp).Add(mup), LepList().Add(elm));
  L_elelmu += Leptonic(LepList().Add(elp).Add(mum), LepList().Add(elm));
  L_elelmu += Leptonic(LepList().Add(elm).Add(mup), LepList().Add(elp));
  L_elelmu += Leptonic(LepList().Add(elm).Add(mum), LepList().Add(elp));
  L_elelmu += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(mup));
  L_elelmu += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(mum));
  L_elelmu += Leptonic(LepList().Add(elp).Add(elp), LepList().Add(mum));
  L_elelmu += Leptonic(LepList().Add(elm).Add(elm), LepList().Add(mup));
  L_elelmu += Leptonic(LepList().Add(elp).Add(elp).Add(mup), LepList());
  L_elelmu += Leptonic(LepList().Add(elm).Add(elm).Add(mum), LepList());
  L_elelmu += Leptonic(LepList().Add(elp).Add(elp).Add(mum), LepList());
  L_elelmu += Leptonic(LepList().Add(elm).Add(elm).Add(mup), LepList());

    // 3 L, mumuel - OSSF hem separated
  Leptonic    L_OSSF_mumu_el(LepList().Add(mup).Add(mum), LepList().Add(elp), "OSSFmumu^el");
  L_OSSF_mumu_el += Leptonic(LepList().Add(mup).Add(mum), LepList().Add(elm));
  L_OSSF_mumu_el += Leptonic(LepList().Add(mup).Add(mum).Add(elp), LepList());
  L_OSSF_mumu_el += Leptonic(LepList().Add(mup).Add(mum).Add(elm), LepList());
  Leptonic    L_nOSSF_mumu_el(LepList().Add(mup).Add(mup), LepList().Add(elp), "nOSSFmumu^el");
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(elm));
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(elm));
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(elp));
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mup).Add(mup).Add(elp), LepList());
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mum).Add(mum).Add(elm), LepList());
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mup).Add(mup).Add(elm), LepList());
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mum).Add(mum).Add(elp), LepList());
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mup).Add(elp), LepList().Add(mum));
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mup).Add(elm), LepList().Add(mum));
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mum).Add(elp), LepList().Add(mup));
  L_nOSSF_mumu_el += Leptonic(LepList().Add(mum).Add(elm), LepList().Add(mup));
  // 3 L, mumuel - OSSF incl
  Leptonic    L_OSSF_mumuel(LepList().Add(mup).Add(mum), LepList().Add(elp), "OSSFmumuel");
  L_OSSF_mumuel += Leptonic(LepList().Add(mup).Add(mum), LepList().Add(elm));
  L_OSSF_mumuel += Leptonic(LepList().Add(mup).Add(mum).Add(elp), LepList());
  L_OSSF_mumuel += Leptonic(LepList().Add(mup).Add(mum).Add(elm), LepList());
  L_OSSF_mumuel += Leptonic(LepList().Add(mup).Add(elp), LepList().Add(mum));
  L_OSSF_mumuel += Leptonic(LepList().Add(mup).Add(elm), LepList().Add(mum));
  L_OSSF_mumuel += Leptonic(LepList().Add(mum).Add(elp), LepList().Add(mup));
  L_OSSF_mumuel += Leptonic(LepList().Add(mum).Add(elm), LepList().Add(mup));
  Leptonic    L_nOSSF_mumuel(LepList().Add(mup).Add(mup), LepList().Add(elp), "nOSSFmumuel");
  L_nOSSF_mumuel += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(elm));
  L_nOSSF_mumuel += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(elm));
  L_nOSSF_mumuel += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(elp));
  L_nOSSF_mumuel += Leptonic(LepList().Add(mup).Add(mup).Add(elp), LepList());
  L_nOSSF_mumuel += Leptonic(LepList().Add(mum).Add(mum).Add(elm), LepList());
  L_nOSSF_mumuel += Leptonic(LepList().Add(mup).Add(mup).Add(elm), LepList());
  L_nOSSF_mumuel += Leptonic(LepList().Add(mum).Add(mum).Add(elp), LepList());
  // 3 L, mumuel - incl
  Leptonic    L_mumuel(LepList().Add(mup).Add(mum), LepList().Add(elp), "mumuel");
  L_mumuel += Leptonic(LepList().Add(mup).Add(mum), LepList().Add(elm));
  L_mumuel += Leptonic(LepList().Add(mup).Add(mum).Add(elp), LepList());
  L_mumuel += Leptonic(LepList().Add(mup).Add(mum).Add(elm), LepList());
  L_mumuel += Leptonic(LepList().Add(mup).Add(elp), LepList().Add(mum));
  L_mumuel += Leptonic(LepList().Add(mup).Add(elm), LepList().Add(mum));
  L_mumuel += Leptonic(LepList().Add(mum).Add(elp), LepList().Add(mup));
  L_mumuel += Leptonic(LepList().Add(mum).Add(elm), LepList().Add(mup));
  L_mumuel += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(elp));
  L_mumuel += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(elm));
  L_mumuel += Leptonic(LepList().Add(mup).Add(mup), LepList().Add(elm));
  L_mumuel += Leptonic(LepList().Add(mum).Add(mum), LepList().Add(elp));
  L_mumuel += Leptonic(LepList().Add(mup).Add(mup).Add(elp), LepList());
  L_mumuel += Leptonic(LepList().Add(mum).Add(mum).Add(elm), LepList());
  L_mumuel += Leptonic(LepList().Add(mup).Add(mup).Add(elm), LepList());
  L_mumuel += Leptonic(LepList().Add(mum).Add(mum).Add(elp), LepList());
 
  CategoryList Cats_3Lchargehem;
  Cats_3Lchargehem += Category(L_OSSF_elel_el, "Ch3L");
  Cats_3Lchargehem += Category(L_nOSSF_elel_el, "Ch3L");
  Cats_3Lchargehem += Category(L_OSSF_mumu_mu, "Ch3L");
  Cats_3Lchargehem += Category(L_nOSSF_mumu_mu, "Ch3L");
  Cats_3Lchargehem += Category(L_OSSF_elel_mu, "Ch3L");
  Cats_3Lchargehem += Category(L_nOSSF_elel_mu, "Ch3L");
  Cats_3Lchargehem += Category(L_OSSF_mumu_el, "Ch3L");
  Cats_3Lchargehem += Category(L_nOSSF_mumu_el, "Ch3L");

  CategoryList Cats_3Lcharge;
  Cats_3Lcharge += Category(L_OSSF_elelel, "Ch3L");
  Cats_3Lcharge += Category(L_nOSSF_elelel, "Ch3L");
  Cats_3Lcharge += Category(L_OSSF_mumumu, "Ch3L");
  Cats_3Lcharge += Category(L_nOSSF_mumumu, "Ch3L");
  Cats_3Lcharge += Category(L_OSSF_elelmu, "Ch3L");
  Cats_3Lcharge += Category(L_nOSSF_elelmu, "Ch3L");
  Cats_3Lcharge += Category(L_OSSF_mumuel, "Ch3L");
  Cats_3Lcharge += Category(L_nOSSF_mumuel, "Ch3L");

  CategoryList Cats_3Lflavor;
  Cats_3Lflavor += Category(L_elelel, "Ch3L");
  Cats_3Lflavor += Category(L_mumumu, "Ch3L");
  Cats_3Lflavor += Category(L_elelmu, "Ch3L");
  Cats_3Lflavor += Category(L_mumuel, "Ch3L");

  // Get hadronic ISR region lists - inclusive and b-separated
  vector<Hadronic> H_ISR_noB;
  vector<Hadronic> H_ISR_B;
  H_ISR_noB.push_back(GetHadronicRegion(1, 0)); // ge1j
  H_ISR_B.push_back(GetHadronicRegion(1, 6)); // ge1j0b
  H_ISR_B.push_back(GetHadronicRegion(1, 7)); // ge1jge1b
  
  // Get PTISR binning
  vector<double> PTISR_incl;
  PTISR_incl.push_back(200.);

  vector<double> PTISR_bin;
  PTISR_bin.push_back(200.);
  PTISR_bin.push_back(300.);

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
  
  cout << "Building 3L 0S object regions" << endl;
  CategoryList Cats_3L_0S_gold = Cats_3Lchargehem; //Cats_3Lcharge
  CategoryList Cats_3L_0S_silver = Cats_3Lflavor;
  CategoryList Cats_3L_0S_bronze = Cats_3Lflavor;
  vector<Hadronic> H_3L_0S;
  H_3L_0S.push_back(GetHadronicRegion(0, 1)); // 0j0sv

  Cats_3L_0S_gold = Cats_3L_0S_gold.CreateFitBinRegions(GetFitBin(1)); // R1
  Cats_3L_0S_gold = Cats_3L_0S_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_3L_0S_gold = Cats_3L_0S_gold.CreateHadronicSRegions(H_3L_0S);
  Cats_3L_0S_gold = Cats_3L_0S_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_3L_0S_gold = Cats_3L_0S_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L_0S_silver = Cats_3L_0S_silver.CreateFitBinRegions(GetFitBin(1));
  Cats_3L_0S_silver = Cats_3L_0S_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_3L_0S_silver = Cats_3L_0S_silver.CreateHadronicSRegions(H_3L_0S);
  Cats_3L_0S_silver = Cats_3L_0S_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_3L_0S_silver = Cats_3L_0S_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L_0S_bronze = Cats_3L_0S_bronze.CreateFitBinRegions(GetFitBin(1));
  Cats_3L_0S_bronze = Cats_3L_0S_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_3L_0S_bronze = Cats_3L_0S_bronze.CreateHadronicSRegions(H_3L_0S);
  Cats_3L_0S_bronze = Cats_3L_0S_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_3L_0S_bronze = Cats_3L_0S_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L += Cats_3L_0S_gold;
  Cats_3L += Cats_3L_0S_silver;
  Cats_3L += Cats_3L_0S_bronze;

  ///////////////////////////////////////////////

  cout << "Building 3L 1j 0/1b object regions" << endl;
  CategoryList Cats_3L_1jS_gold = Cats_3Lchargehem; //Cats_3Lcharge
  CategoryList Cats_3L_1jS_silver = Cats_3Lflavor;
  CategoryList Cats_3L_1jS_bronze = Cats_3Lflavor;
  vector<Hadronic> H_3L_1jS;
  H_3L_1jS.push_back(GetHadronicRegion(1, 8)); // 1j0b
  H_3L_1jS.push_back(GetHadronicRegion(1, 9)); // 1j1b

  Cats_3L_1jS_gold = Cats_3L_1jS_gold.CreateFitBinRegions(GetFitBin(3));
  Cats_3L_1jS_gold = Cats_3L_1jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_3L_1jS_gold = Cats_3L_1jS_gold.CreateHadronicSRegions(H_3L_1jS);
  Cats_3L_1jS_gold = Cats_3L_1jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_3L_1jS_gold = Cats_3L_1jS_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L_1jS_silver = Cats_3L_1jS_silver.CreateFitBinRegions(GetFitBin(3));
  Cats_3L_1jS_silver = Cats_3L_1jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_3L_1jS_silver = Cats_3L_1jS_silver.CreateHadronicSRegions(H_3L_1jS);
  Cats_3L_1jS_silver = Cats_3L_1jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_3L_1jS_silver = Cats_3L_1jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L_1jS_bronze = Cats_3L_1jS_bronze.CreateFitBinRegions(GetFitBin(3));
  Cats_3L_1jS_bronze = Cats_3L_1jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_3L_1jS_bronze = Cats_3L_1jS_bronze.CreateHadronicSRegions(H_3L_1jS);
  Cats_3L_1jS_bronze = Cats_3L_1jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_3L_1jS_bronze = Cats_3L_1jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L += Cats_3L_1jS_gold;
  Cats_3L += Cats_3L_1jS_silver;
  Cats_3L += Cats_3L_1jS_bronze;

  ///////////////////////////////////////////////

  cout << "Building 3L ge2j 0/ge1b object regions" << endl;
  CategoryList Cats_3L_2jS_gold = Cats_3Lchargehem;
  CategoryList Cats_3L_2jS_silver = Cats_3Lflavor;
  CategoryList Cats_3L_2jS_bronze = Cats_3Lflavor;
  vector<Hadronic> H_3L_2jS;
  H_3L_2jS.push_back(GetHadronicRegion(2, 6)); // ge2j0b
  H_3L_2jS.push_back(GetHadronicRegion(2, 7)); // ge2jge1b

  Cats_3L_2jS_gold = Cats_3L_2jS_gold.CreateFitBinRegions(GetFitBin(4));
  Cats_3L_2jS_gold = Cats_3L_2jS_gold.CreateLeptonIDRegions(IDs_gold, 1);
  Cats_3L_2jS_gold = Cats_3L_2jS_gold.CreateHadronicSRegions(H_3L_2jS);
  Cats_3L_2jS_gold = Cats_3L_2jS_gold.CreateHadronicISRRegions(H_ISR_B);
  Cats_3L_2jS_gold = Cats_3L_2jS_gold.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L_2jS_silver = Cats_3L_2jS_silver.CreateFitBinRegions(GetFitBin(4));
  Cats_3L_2jS_silver = Cats_3L_2jS_silver.CreateLeptonIDRegions(IDs_silver, 1);
  Cats_3L_2jS_silver = Cats_3L_2jS_silver.CreateHadronicSRegions(H_3L_2jS);
  Cats_3L_2jS_silver = Cats_3L_2jS_silver.CreateHadronicISRRegions(H_ISR_noB);
  Cats_3L_2jS_silver = Cats_3L_2jS_silver.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L_2jS_bronze = Cats_3L_2jS_bronze.CreateFitBinRegions(GetFitBin(4));
  Cats_3L_2jS_bronze = Cats_3L_2jS_bronze.CreateLeptonIDRegions(IDs_bronze, 1);
  Cats_3L_2jS_bronze = Cats_3L_2jS_bronze.CreateHadronicSRegions(H_3L_2jS);
  Cats_3L_2jS_bronze = Cats_3L_2jS_bronze.CreateHadronicISRRegions(H_ISR_noB);
  Cats_3L_2jS_bronze = Cats_3L_2jS_bronze.CreateGenericRegions("PTISR", PTISR_incl);

  Cats_3L += Cats_3L_2jS_gold;
  Cats_3L += Cats_3L_2jS_silver;
  Cats_3L += Cats_3L_2jS_bronze;

  ///////////////////////////////////////////////
  
  return Cats_3L;
}
