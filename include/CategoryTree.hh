#ifndef CategoryTree_HH
#define CategoryTree_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>
#include <cstdarg>

#include "Criteria.hh"

enum CatLabelType { kNorm, kL, kL_sup, kL_sub, kJ, kJ_sup, kJ_sub, kX_sup, kX_sub };

///////////////////////////////////////////
////////// CategoryTree class
///////////////////////////////////////////

class CategoryTree {
public:
  CategoryTree(const VS& match_string = VS(),
	       const string& bare_label = "",
	       const string& spec_label = "",
	       CatLabelType  spec_type = kNorm,
	       bool DoDraw = true);
  
  
  virtual ~CategoryTree();

  void SetMatchString(const VS& match_string);
  void SetBareLabel(const string& bare_label);
  void SetSpecLabel(const string& spec_label, CatLabelType spec_type = kNorm);

  const VS&     GetMatchString() const;
  const string& GetBareLabel() const;
  const string& GetSpecLabel() const;
  CatLabelType  GetSpecType() const;

  void AddSubCategory(CategoryTree& cat);

protected:
  void SetParent(CategoryTree& par);
  
private:
  VS           m_MatchString;
  string       m_BareLabel;
  string       m_SpecLabel;
  CatLabelType m_SpecType;
  
  CategoryTree* m_ParCat;
  vector<CategoryTree*> m_SubCats;

  bool m_DoDraw;
  
};

#endif

