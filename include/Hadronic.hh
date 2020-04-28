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
////////// Hadronic class
///////////////////////////////////////////

class Hadronic : public Criteria {
public:
  Hadronic(int Njet, int Nbjet, int NSV, const std::string& label = "none");
  Hadronic(const Hadronic&);

  virtual ~Hadronic();
  
  bool operator == (const Criteria& hadronic) const;

  int GetN() const;
  const Hadronic& operator [] (int i) const;
  Hadronic& operator += (const Hadronic& hadronic);

  int GetNjet() const;
  int GetNbjet() const;
  int GetNSV() const;

private:
  int m_Nhadronic;
  vector<Hadronic*> m_Hadronic;
  
  int m_Njet;
  int m_Nbjet;
  int m_NSV;
};

#endif

