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

using std::vector;
using std::string;

///////////////////////////////////////////
////////// Category class
///////////////////////////////////////////

class CategoryList;

class Category : public Criteria {
public:
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
////////// CategoryBranch class
///////////////////////////////////////////

class CategoryBranch {
public:
  CategoryBranch();

  virtual ~CategoryBranch();

  void Init(TTree* tree);
  
  void FillCategory(Category& cat);

private:
  TTree* m_Tree;
  
  string m_Cat;
  string m_Bin;

  FitBinBranch m_FitBinBranch;
  
};

#endif

