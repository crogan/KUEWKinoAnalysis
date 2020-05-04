#ifndef Hadronic_HH
#define Hadronic_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <iostream>
#include <vector>

#include "Criteria.hh"

using std::vector;
using std::string;

enum HadFlavor { kSV, kBtag, kJet };

///////////////////////////////////////////
////////// Had class
///////////////////////////////////////////

class Had {
public:
  Had(HadFlavor flavor = kJet);
  Had(const Had&);
  virtual ~Had();

  // for sorting hadrons in category
  bool operator < (const Had& had) const;

  // for matching hadron categories
  bool operator == (const Had& had) const;

  HadFlavor Flavor() const;

  Had& SetFlavor(HadFlavor flavor);
 
  std::string IDLabel() const;
    
private:
  HadFlavor m_Flavor;
};

///////////////////////////////////////////
////////// HadList class
///////////////////////////////////////////

class HadList {
public:
  HadList();
  HadList(const HadList& hads);
  virtual ~HadList();

  void Clear();
  
  HadList& operator = (const HadList& had);
  
  int GetN() const;
  const Had& operator [] (int i) const;

  HadList& Add(const Had& had);
  HadList& operator += (const Had& had);
  HadList& operator += (const HadList& hads);

  // for matching hemispheres
  bool operator == (const HadList& leps) const;

  // for sorting hemispheres
  bool operator < (const HadList& leps) const;

  std::string GetIDLabel() const;
  
private:
  int m_N;
  vector<Had*> m_Had;
};

///////////////////////////////////////////
////////// Hadronic class
///////////////////////////////////////////

class Hadronic : public Criteria {
public:
  Hadronic(const HadList& Hem_a, const HadList& Hem_b, const std::string& label = "none");
  Hadronic(int Njet = 0, int Nbjet = 0, int NSV = 0, const std::string& label = "none");
  Hadronic(const Hadronic&);

  virtual ~Hadronic();
  
  bool operator == (const Criteria& hadronic) const;

  int GetN() const;
  const Hadronic& operator [] (int i) const;
  Hadronic& operator += (const Hadronic& hadronic);

  int GetNjet() const;
  int GetNbjet() const;
  int GetNSV() const;

  const HadList& GetHemA() const;
  const HadList& GetHemB() const;

private:
  int m_Nhadronic;
  vector<Hadronic*> m_Hadronic;
  
  int m_Njet;
  int m_Nbjet;
  int m_NSV;

  HadList m_Had_a;
  HadList m_Had_b;
};

#endif

