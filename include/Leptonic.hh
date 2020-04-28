#ifndef Leptonic_HH
#define Leptonic_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <iostream>
#include <vector>

#include "Criteria.hh"

using std::vector;
using std::string;

enum LepFlavor { kElectron, kMuon };
enum LepCharge { kPos, kNeg };
enum LepID { kGold, kSilver, kBronze };
enum LepSource { kSignal, kTau, kQFlip, kHF, kLF };

///////////////////////////////////////////
////////// Lep class
///////////////////////////////////////////

class Lep {
public:
  Lep(LepFlavor flavor, LepCharge charge,
      LepID id = kGold, LepSource source = kSignal);
  Lep(const Lep&);
  virtual ~Lep();

  // for sorting leptons in category
  bool operator < (const Lep& lep) const;

  // for matching lepton categories
  bool operator == (const Lep& lep) const;

  LepFlavor Flavor() const;
  LepCharge Charge() const;
  LepID     ID() const;
  LepSource Source() const;

  Lep& SetFlavor(LepFlavor flavor);
  Lep& SetCharge(LepCharge charge);
  Lep& SetID(LepID id);
  Lep& SetSource(LepSource source);

  std::string IDLabel() const;
    
private:
  LepFlavor m_Flavor;
  LepCharge m_Charge;
  LepID     m_ID;
  LepSource m_Source;
};

LepSource GetLepSource(int PDGID, int genPDGID = 0, int momPDGID = 0);

///////////////////////////////////////////
////////// LepList class
///////////////////////////////////////////

class LepList {
public:
  LepList();
  LepList(const LepList& leps);
  virtual ~LepList();

  void Clear();
  
  LepList& operator = (const LepList& lep);
  
  int GetN() const;
  const Lep& operator [] (int i) const;

  LepList& Add(const Lep& lep);
  LepList& operator += (const Lep& lep);
  LepList& operator += (const LepList& leps);

  // for matching hemispheres
  bool operator == (const LepList& leps) const;

  // for sorting hemispheres
  bool operator < (const LepList& leps) const;

  std::string GetIDLabel() const;
  vector<std::string> GetFakeLabels(LepSource fake) const;

  LepList GetFakes(LepSource fake) const;
  
private:
  int m_N;
  vector<Lep*> m_Lep;
};

///////////////////////////////////////////
////////// Leptonic class
///////////////////////////////////////////

class Leptonic : public Criteria {
public:
  Leptonic(const LepList& Hem_a, const LepList& Hem_b, const std::string& label = "none");
  Leptonic(const Leptonic&);
  
  virtual ~Leptonic();
  
  bool operator == (const Criteria& leptonic) const;

  int GetN() const;
  const Leptonic& operator [] (int i) const;
  Leptonic& operator += (const Leptonic& leptonic);

  const LepList& GetHemA() const;
  const LepList& GetHemB() const;
    
private:
  int m_Nleptonic;
  vector<Leptonic*> m_Leptonic;

  LepList m_Lep_a;
  LepList m_Lep_b;

};

#endif

