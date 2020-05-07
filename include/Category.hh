#ifndef Category_HH
#define Category_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <iostream>
#include <vector>

#include "Generic.hh"
#include "FitBin.hh"

class Leptonic;
class Hadronic;

///////////////////////////////////////////
////////// Category class
///////////////////////////////////////////

class CategoryList;

class Category : public Criteria {
public:
  // constructor for ~empty category for lists in Combine interface
  Category(const string& label,
	   const FitBin& bin,
	   const string& name = "none");

  // constructor for only lepton-specified criteria
  Category(const Leptonic& lep,
	   const string& name = "none");

  // nominal constructor for functioning Category
  Category(const Leptonic& lep,
	   const Hadronic& S_had,
	   const Hadronic& ISR_had,
	   const string& name = "none");
  
  Category(const Category& cat);

  virtual ~Category();

  std::string GetLabel() const;
 
  int GetN() const;
  const Criteria& operator [] (int i) const;
  
  Category& AddGenericBin(const GenericBin& bin);
  Category& AddGenericVal(const GenericVal& val);

  bool operator == (const Criteria& cat) const;

  bool operator < (const Category&) const;
  bool operator > (const Category&) const;

  CategoryList CreateLeptonIDRegions(int NID = 3, int Nfakemax = 2);
  CategoryList CreateGenericRegions(const string& label, const VD& bin_edges);
  CategoryList CreateHadronicSRegions(const vector<const Hadronic*>& had) const;
  CategoryList CreateHadronicISRRegions(const vector<const Hadronic*>& had) const;

  Category& SetFitBin(const FitBin& bin);
  const FitBin& GetFitBin() const;
  
  FitBin* GetNewFitBin(const string& process) const;

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

  CategoryList Filter(const string& label) const;
  CategoryList Remove(const string& label) const;
  CategoryList FilterOR(VS& labels) const;
  CategoryList FilterAND(VS& labels) const;
  CategoryList RemoveOR(VS& labels) const;
  CategoryList RemoveAND(VS& labels) const;

  void Print() const;

  CategoryList CreateFitBinRegions(const FitBin& bin) const;
  CategoryList CreateLeptonIDRegions(int NID = 3, int Nfakemax = 2) const;
  CategoryList CreateGenericRegions(const string& label, const VD& bin_edges) const;
  CategoryList CreateHadronicSRegions(const vector<const Hadronic*>& had) const;
  CategoryList CreateHadronicISRRegions(const vector<const Hadronic*>& had) const;

  VC GetCategories() const; 

private:
  int m_N;
  vector<Category*> m_Cat;
  map<string,bool> m_CatMap;

};

///////////////////////////////////////////
////////// CategoryBranch class
///////////////////////////////////////////

class CategoryBranch {
public:
  CategoryBranch();

  virtual ~CategoryBranch();

  void InitFill(TTree* tree);
  void FillCategory(Category& cat);
  
  void InitGet(TTree* tree);
  Category GetCategory();

private:
  TTree* m_Tree;
  
  string m_Cat;
  string m_Bin;

  string* m_CatPtr;
  string* m_BinPtr;
  TBranch* m_b_Cat;
  TBranch* m_b_Bin;

  FitBinBranch m_FitBinBranch;
  
};

#endif

