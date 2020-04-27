#ifndef Category_HH
#define Category_HH

#include <TH1D.h>
#include <TH2D.h>
#include <iostream>
#include <vector>

using std::vector;

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
////////// Criteria class
///////////////////////////////////////////

class Criteria {
public:
  Criteria(const std::string& label = "none");
  Criteria(const Criteria&);

  virtual ~Criteria();

  const string& Label() const;
  Criteria& SetLabel(const string& label);

  virtual bool operator == (const Criteria& criteria) const = 0;

private:
  std::string m_Label;
};

class CriteriaList {
public:
  CriteriaList();
  CriteriaList(const CriteriaList& list);
  virtual ~CriteriaList();

  void Clear();
  
  CriteriaList& operator = (const CriteriaList& list);
  
  int GetN() const;
  const Criteria& operator [] (int i) const;

  CriteriaList& Add(const Criteria& criteria);
  CriteriaList& operator += (const Criteria& criteria);
  CriteriaList& operator += (const CriteriaList& criteria);
  
private:
  int m_N;
  vector<Criteria*> m_Criteria;
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

///////////////////////////////////////////
////////// GenericBin class
///////////////////////////////////////////

class GenericBin : public Criteria {
public:
  GenericBin(double bin_min, double bin_max, const std::string& label = "none");
  GenericBin(const GenericBin&);

  virtual ~GenericBin();

  double GetMin() const;
  double GetMax() const;
  
  bool operator == (const Criteria& bin) const;

private:
  double m_Min;
  double m_Max;
};

class GenericVal : public Criteria {
public:
  GenericVal(double val, const std::string& label = "none");
  GenericVal(const GenericVal&);

  virtual ~GenericVal();

  double GetVal() const;
  
  bool operator == (const Criteria& bin) const;

private:
  double m_Val;
 
};

///////////////////////////////////////////
////////// FitBin class
///////////////////////////////////////////

class FitBin {
public:
  // default is to include overflow in last bin for x, but not y
  FitBin(const vector<double>& bin_edges_x = vector<double>(),
	 const vector<double>& bin_edges_y = vector<double>());
  FitBin(const FitBin&);
  
  virtual ~FitBin();

  FitBin& operator = (const FitBin& bin);
  
  FitBin& InitializeHistogram(const std::string& label);
  void Fill(double weight = 1., double X = 0., double Y = 0.);
  
  const vector<double>& GetBinEdgesX() const;
  const vector<double>& GetBinEdgesY() const;

  void WriteHistogram(const std::string& name,
		      const std::string& fold,
		      TFile& file) const;

private:
  int m_NX;
  int m_NY;

  vector<double> m_Edges_X;
  vector<double> m_Edges_Y;
  
  TH1D* m_hist1D;
  TH2D* m_hist2D;

  double m_LastBinCenterX;
};

///////////////////////////////////////////
////////// Category class
///////////////////////////////////////////

class CategoryList;

class Category {
public:
  Category(const Leptonic& lep,
	   const Hadronic& S_had,
	   const Hadronic& ISR_had);
  Category(const Category& cat);

  virtual ~Category();

  std::string GetLabel() const;

  int GetN() const;
  const Criteria& operator [] (int i) const;
  
  Category& AddGenericBin(const GenericBin& bin);
  Category& AddGenericVal(const GenericVal& val);

  bool operator == (const Category& cat) const;

  CategoryList CreateLeptonIDRegions(int NID = 3, int Nfakemax = 2);
  CategoryList CreateGenericRegions(const string& label, const vector<double>& bin_edges);
  CategoryList CreateHadronicSRegions(const vector<const Hadronic*>& had) const;
  CategoryList CreateHadronicISRRegions(const vector<const Hadronic*>& had) const;

  Category& SetFitBin(const FitBin& bin);
  const FitBin& GetFitBin() const;
  
  FitBin* GetNewFitBin(const std::string& process) const;

private:
  CriteriaList m_Criteria;

  FitBin m_Template;

};

///////////////////////////////////////////
////////// CategoryList class
///////////////////////////////////////////

class CategoryList {
public:
  CategoryList();
  CategoryList(const CategoryList& list);
  virtual ~CategoryList();

  void Clear();

  int Find(const Category& cat) const;
  
  CategoryList& operator = (const CategoryList& list);
  
  int GetN() const;
  const Category& operator [] (int i) const;

  CategoryList& Add(const Category& cat);
  CategoryList& operator += (const Category& cat);
  CategoryList& operator += (const CategoryList& cat);

  void Print() const;

  CategoryList CreateFitBinRegions(const FitBin& bin) const;
  CategoryList CreateLeptonIDRegions(int NID = 3, int Nfakemax = 2) const;
  CategoryList CreateGenericRegions(const string& label, const vector<double>& bin_edges) const;
  CategoryList CreateHadronicSRegions(const vector<const Hadronic*>& had) const;
  CategoryList CreateHadronicISRRegions(const vector<const Hadronic*>& had) const;

private:
  int m_N;
  vector<Category*> m_Cat;

};

///////////////////////////////////////////
////////// SL (string list) class
///////////////////////////////////////////

class SL : public std::vector<std::string> {
public:
  SL(){ }
  virtual ~SL(){ }

  SL& a(const std::string& s){
    (*this).push_back(s);
    return *this;
  }
};

#endif

