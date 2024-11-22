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


class SpectroscopicLabel {
public:
    SpectroscopicLabel();
    virtual ~SpectroscopicLabel();

    void AddLabel(const string& label, CatLabelType type);

    string GetSpectroscopicLabel() const;
    string GetPlainLabel() const;

private:
    string m_L;
    string m_J;

    string m_Norm;
    string m_Lsup;
    string m_Lsub;
    string m_Jsup;
    string m_Jsub;
    string m_Xsup;
    string m_Xsub;
};

///////////////////////////////////////////
////////// CategoryTree class
///////////////////////////////////////////

class CategoryTree {
public:
    //CategoryTree();
    CategoryTree(const VS& match_string = VS(),
                 const string& bare_label = "",
                 const string& spec_label = "",
                 CatLabelType  spec_type = kNorm,
                 bool DoDraw = true);
    CategoryTree(const CategoryTree& CT);

    virtual ~CategoryTree();

    CategoryTree& operator = (const CategoryTree& CT);

    void SetMatchString(const VS& match_string);
    void SetBareLabel(const string& bare_label);
    void SetSpecLabel(const string& spec_label, CatLabelType spec_type = kNorm);

    const VS&     GetMatchString() const;
    const string& GetBareLabel() const;
    const string& GetSpecLabel() const;
    CatLabelType  GetSpecType() const;
    bool          IsVisible() const;

    void AddSubCategory(const CategoryTree& cat);

    string GetSpectroscopicLabel() const;
    string GetPlainLabel(int depth) const;
    VS     GetMatchLabels() const;

    const CategoryTree* GetParent() const;
    const CategoryTree* GetVisibleParent() const;

    int GetDepth() const;

    int GetNVisible() const;

    void GetListVisible(vector<const CategoryTree*>& cats) const;
    void GetListDeepest(vector<const CategoryTree*>& cats) const;
    void GetListDepth(vector<const CategoryTree*>& cats, int depth = 0) const;

    void Print(int depth = 0) const;


protected:
    void SetParent(const CategoryTree& par);

    const vector<CategoryTree*>& GetSubCats() const;

    void AddSpectroscopicLabel(SpectroscopicLabel& slabel, int depth = -1) const;

private:
    VS           m_MatchString;
    string       m_BareLabel;
    string       m_SpecLabel;
    CatLabelType m_SpecType;

    const CategoryTree* m_ParCat;
    vector<CategoryTree*> m_SubCats;

    bool m_DoDraw;

    void Clear();

};

///////////////////////////////////////////
////////// CategoryTreeTool class
///////////////////////////////////////////

class CategoryTreeTool {
public:
  CategoryTreeTool();
  
  virtual ~CategoryTreeTool();
  
  CategoryTree GetCategories() const;
  CategoryTree GetCategories_0L() const;
  CategoryTree GetCategories_1L() const;
  CategoryTree GetCategories_2L() const;
  CategoryTree GetCategories_3L() const;
  
  CategoryTree GetCategoriesFakes() const;
  CategoryTree GetCategories_Fakes1L() const;
  CategoryTree GetCategories_Fakes2L() const;
  CategoryTree GetCategories_Fakes3L() const;
  CategoryTree GetCategories_QCD0L() const;
  CategoryTree GetCategories_QCD1L() const;
  CategoryTree GetCategories_Wjets2L() const;
  CategoryTree GetCategories_Wjets3L() const;
  CategoryTree GetCategories_0L_plotFormat() const;
  CategoryTree GetCategories_0L_Summary() const;
  CategoryTree GetCategories_SV_Summary() const;
  CategoryTree GetCategories_0L_fineSplit() const;
  CategoryTree GetCategories_0L_PTISRgamTSplit() const;
  CategoryTree GetCategories_1L_plotFormat() const;
  CategoryTree GetCategories_1L_Summary() const;
  CategoryTree GetCategories_1L_ChargeSep() const;
  CategoryTree GetCategories_1L_fineSplit() const;
  CategoryTree GetCategories_1L_PTISRgamTSplit() const;
  CategoryTree GetCategories_2L_plotFormat() const;
  CategoryTree GetCategories_2L_Summary() const;
  CategoryTree GetCategories_2L_fineSplit() const;
  CategoryTree GetCategories_2L_flavorSplit() const;
  CategoryTree GetCategories_2L_PTISRgamTSplit() const;
  CategoryTree GetCategories_3L_plotFormat() const;
  CategoryTree GetCategories_3L_Summary() const;
  CategoryTree GetCategories_Plot1L() const;
  CategoryTree GetCategories_0L_complete() const;
  CategoryTree GetCategories_1L_complete() const;
  CategoryTree GetCategories_2L_complete() const;
  CategoryTree GetCategories_3L_complete() const;
  CategoryTree GetCategories_All() const;
  CategoryTree GetCategories_2L_flavorSepAndSS() const;
  CategoryTree GetCategories_SVonly() const;
  CategoryTree GetCategories_SVConly() const;
  CategoryTree GetCategories_StopMultiJet() const;
  CategoryTree GetCategories_0LMultiJetBsplit() const;

private:
  CategoryTree* _CT_gamma0_etaInc;
  CategoryTree* _CT_etaC;
  CategoryTree* _CT_etaF;
  CategoryTree* _CT_gamma0;
  CategoryTree* _CT_gamma1;
  CategoryTree* _CT_ptISR0_gamma0_etaInc;
  CategoryTree* _CT_ptISR0;
  CategoryTree* _CT_ptISR1;
  CategoryTree* _CT_ptISR0_gamma0;
  CategoryTree* _CT_ptISR1_gamma0;
  CategoryTree* _CT_ptISR0_gammaInc;
  CategoryTree* _CT_ptISR1_gammaInc;
  CategoryTree* _CT_0bISR_ptISRInc;
  CategoryTree* _CT_0bISR_ptISRInc_gammaInc;
  CategoryTree* _CT_ge1bISR_ptISRInc;
  CategoryTree* _CT_ge1bISR_ptISRInc_gammaInc;
  CategoryTree* _CT_ge1jISR_ptISR0;
  CategoryTree* _CT_ge1jISR_ptISRInc;
  CategoryTree* _CT_ge1jISR_ptISRInc_gammaInc;
  CategoryTree* _CT_ge1jISR_ptISR0_gamma0_etaInc;
  CategoryTree* _CT_0sv_inclbISR_ptISRInc;
  CategoryTree* _CT_0sv_ge1jISR_ptISRInc;
  CategoryTree* _CT_0sv_inclbISR_ptISRInc_gammaInc;
  CategoryTree* _CT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc;
  CategoryTree* _CT_0bS_inclbISR_ptISRInc_gammaInc;
  CategoryTree* _CT_1bS_inclbISR_ptISRInc_gammaInc;
  CategoryTree* _CT_2bS_ge1jISR_ptISRInc_gammaInc;
  CategoryTree* _CT_ge2bS_ge1jISR_ptISRInc_gammaInc;
  
  void AssembleCTmodules();
  
  const CategoryTree& getCT_gamma0_etaInc() const;
  const CategoryTree& getCT_etaC() const;
  const CategoryTree& getCT_etaF() const;
  const CategoryTree& getCT_gamma0() const;
  const CategoryTree& getCT_gamma1() const;
  const CategoryTree& getCT_ptISR0_gamma0_etaInc() const;
  const CategoryTree& getCT_ptISR0() const;
  const CategoryTree& getCT_ptISR1() const;
  const CategoryTree& getCT_ptISR0_gamma0() const;
  const CategoryTree& getCT_ptISR1_gamma0() const;
  const CategoryTree& getCT_ptISR0_gammaInc() const;
  const CategoryTree& getCT_ptISR1_gammaInc() const;
  const CategoryTree& getCT_0bISR_ptISRInc() const;
  const CategoryTree& getCT_0bISR_ptISRInc_gammaInc() const;
  const CategoryTree& getCT_ge1bISR_ptISRInc() const;
  const CategoryTree& getCT_ge1bISR_ptISRInc_gammaInc() const;
  const CategoryTree& getCT_ge1jISR_ptISR0() const;
  const CategoryTree& getCT_ge1jISR_ptISRInc() const;
  const CategoryTree& getCT_ge1jISR_ptISRInc_gammaInc() const;
  const CategoryTree& getCT_ge1jISR_ptISR0_gamma0_etaInc() const;
  const CategoryTree& getCT_0sv_ge1jISR_ptISRInc() const;
  const CategoryTree& getCT_0sv_inclbISR_ptISRInc() const;
  const CategoryTree& getCT_0sv_inclbISR_ptISRInc_gammaInc() const;
  const CategoryTree& getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc() const;
  const CategoryTree& getCT_0bS_inclbISR_ptISRInc_gammaInc() const;
  const CategoryTree& getCT_1bS_inclbISR_ptISRInc_gammaInc() const;
  const CategoryTree& getCT_2bS_ge1jISR_ptISRInc_gammaInc() const;
  const CategoryTree& getCT_ge2bS_ge1jISR_ptISRInc_gammaInc() const;
};

#endif

