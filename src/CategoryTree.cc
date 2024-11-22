#include <iostream>
#include <map>

#include "CategoryTree.hh"

SpectroscopicLabel::SpectroscopicLabel(){
  m_L = " L";
  m_J = " J";
  m_Lsup = "  ";
  m_Lsub = "  ";
  m_Jsup = "  ";
  m_Jsub = "  ";
  m_Xsup = "  ";
  m_Xsub = "  ";
  m_Norm = "";
}

SpectroscopicLabel::~SpectroscopicLabel(){
  
}

void SpectroscopicLabel::AddLabel(const string& label, CatLabelType type){
  if(type == kL)
    m_L = label;
  if(type == kJ)
    m_J = label;

  if(label == "")
    return;
  
  if(type == kNorm){
    if(m_Norm != "")
      m_Norm += " ";
    m_Norm += label;
  }
  if(type == kL_sup){
    if(m_Lsup == "  ")
      m_Lsup = label;
    else
      m_Lsup += " " + label;
  }
  if(type == kL_sub){
   if(m_Lsub == "  ")
      m_Lsub = label;
    else
      m_Lsub += " " + label;
  }
  if(type == kJ_sup){
    if(m_Jsup == "  ")
      m_Jsup = label;
    else
      m_Jsup += " " + label;
  }
  if(type == kJ_sub){
   if(m_Jsub == "  ")
      m_Jsub = label;
    else
      m_Jsub += " " + label;
  }
   if(type == kX_sup){
    if(m_Xsup == "  ")
      m_Xsup = label;
    else
      m_Xsup += " " + label;
  }
  if(type == kX_sub){
   if(m_Xsub == "  ")
     m_Xsub = label;
   else
     m_Xsub += " " + label;
  }
}

string SpectroscopicLabel::GetSpectroscopicLabel() const {
  string label = m_Norm + " " + m_L;
  if(m_Lsup != "")
    label += "^{#scale[0.8]{" + m_Lsup + "}}";
  if(m_Lsub != "")
    label += "_{#scale[0.8]{" + m_Lsub + "}}";
  label += " " + m_J;
  if(m_Jsup != "")
    label += "^{#scale[0.8]{" + m_Jsup + "}}";
  if(m_Jsub != "")
    label += "_{#scale[0.8]{" + m_Jsub + "}}";
  label += " X";
  if(m_Xsup != "")
    label += "^{#scale[0.8]{" + m_Xsup + "}}";
  if(m_Xsub != "")
    label += "_{#scale[0.8]{" + m_Xsub + "}}";
//  if(m_Lsup != "")
//    label += "^{{" + m_Lsup + "}}";
//  if(m_Lsub != "")
//    label += "_{{" + m_Lsub + "}}";
//  label += " " + m_J;
//  if(m_Jsup != "")
//    label += "^{{" + m_Jsup + "}}";
//  if(m_Jsub != "")
//    label += "_{{" + m_Jsub + "}}";
//  label += " X";
//  if(m_Xsup != "")
//    label += "^{{" + m_Xsup + "}}";
//  if(m_Xsub != "")
//    label += "_{{" + m_Xsub + "}}";

  return label;
}

string SpectroscopicLabel::GetPlainLabel() const {
  string label;
  if(m_Norm != "")
    label += m_Norm + " ";
  if(m_L != " L")
    label += m_L + " ";
  if(m_Lsup != "  ")
    label += m_Lsup + " ";
  if(m_Lsub != "  ")
    label += m_Lsub + " ";
  if(m_J != " J")
    label += m_J + " ";
  if(m_Jsup != "  ")
    label += m_Jsup + " ";
  if(m_Jsub != "  ")
    label += m_Jsub + "S ";
  // if((m_Jsup != "  ") || (m_Jsup != "  "))
  //   label += "#in S ";
  if(m_Xsup != "  ")
    label += m_Xsup + " ";
  if(m_Xsub != "  ")
    label += m_Xsub + "I ";
  return label;
}

///////////////////////////////////////////
////////// CategoryTree class
///////////////////////////////////////////

CategoryTree::CategoryTree(const VS& match_string,
			   const string& bare_label,
			   const string& spec_label,
			   CatLabelType  spec_type,
			   bool DoDraw){
  m_MatchString = match_string;
  m_BareLabel = bare_label;
  m_SpecLabel = spec_label;
  m_SpecType = spec_type;
  m_DoDraw = DoDraw;

  m_ParCat = nullptr;
}

CategoryTree::CategoryTree(const CategoryTree& CT){
  m_MatchString = CT.GetMatchString();
  m_BareLabel = CT.GetBareLabel();
  m_SpecLabel = CT.GetSpecLabel();
  m_SpecType = CT.GetSpecType();
  m_DoDraw = CT.IsVisible();

  m_ParCat = nullptr;

  const vector<CategoryTree*>& subCT = CT.GetSubCats();
  int N = subCT.size();
  for(int i = 0; i < N; i++)
    if(subCT[i])
      AddSubCategory(*subCT[i]);
}

CategoryTree::~CategoryTree(){
  Clear();
}

CategoryTree& CategoryTree::operator = (const CategoryTree& CT){
  Clear();

  m_MatchString = CT.GetMatchString();
  m_BareLabel = CT.GetBareLabel();
  m_SpecLabel = CT.GetSpecLabel();
  m_SpecType = CT.GetSpecType();
  m_DoDraw = CT.IsVisible();

  m_ParCat = nullptr;

  const vector<CategoryTree*>& subCT = CT.GetSubCats();
  int N = subCT.size();
  for(int i = 0; i < N; i++)
    if(subCT[i])
      AddSubCategory(*subCT[i]);

  return *this;
}

void CategoryTree::Clear(){
  m_ParCat = nullptr;
  int Nc = m_SubCats.size();

  for(int i = 0; i < Nc; i++)
    if(m_SubCats[i])
      delete m_SubCats[i];
}

void CategoryTree::SetMatchString(const VS& match_string){
  m_MatchString = match_string;
}

void CategoryTree::SetBareLabel(const string& bare_label){
  m_BareLabel = bare_label;
}

void CategoryTree::SetSpecLabel(const string& spec_label, CatLabelType spec_type){
  m_SpecLabel = spec_label;
  m_SpecType  = spec_type;
}

const VS& CategoryTree::GetMatchString() const {
  return m_MatchString;
}

const string& CategoryTree::GetBareLabel() const {
  return m_BareLabel;
}

const string& CategoryTree::GetSpecLabel() const {
  return m_SpecLabel;
}

CatLabelType CategoryTree::GetSpecType() const {
  return m_SpecType;
}

bool CategoryTree::IsVisible() const{
  return m_DoDraw;
}

void CategoryTree::SetParent(const CategoryTree& par){
  m_ParCat = &par;
}

const CategoryTree* CategoryTree::GetParent() const {
  return m_ParCat;
}

const CategoryTree* CategoryTree::GetVisibleParent() const {

  if(!m_ParCat)
    return nullptr;

  const CategoryTree* ParParCat = m_ParCat->GetParent();
  if(!ParParCat)
    return m_ParCat;
  
  if(m_ParCat->GetNVisible() >= ParParCat->GetNVisible() || !m_ParCat->IsVisible())
    return m_ParCat->GetVisibleParent();

  return m_ParCat;
}

void CategoryTree::AddSubCategory(const CategoryTree& cat){
  CategoryTree* cat_ptr = new CategoryTree(cat);
  m_SubCats.push_back(cat_ptr);
  cat_ptr->SetParent(*this);

}

const vector<CategoryTree*>& CategoryTree::GetSubCats() const {
  return m_SubCats;
}

string CategoryTree::GetSpectroscopicLabel() const {
  SpectroscopicLabel slabel;
  AddSpectroscopicLabel(slabel);

  return slabel.GetSpectroscopicLabel();
}

string CategoryTree::GetPlainLabel(int depth) const {
  SpectroscopicLabel slabel;
  AddSpectroscopicLabel(slabel, depth);
  
  return slabel.GetPlainLabel();
}

void CategoryTree::AddSpectroscopicLabel(SpectroscopicLabel& slabel, int depth) const {
if(IsVisible())
    depth--;
  if(depth == 0)
    return;
  if(m_ParCat != nullptr)
    m_ParCat->AddSpectroscopicLabel(slabel, depth);
 slabel.AddLabel(m_SpecLabel, m_SpecType);
}

int CategoryTree::GetDepth() const {
  int N = m_SubCats.size();

  int depth = 0;
  for(int i = 0; i < N; i++){
    int d = m_SubCats[i]->GetDepth();
    if(d > depth)
      depth = d;
  }
  if(m_DoDraw)
    depth += 1;

  return depth;
}

int CategoryTree::GetNVisible() const {
  int N = m_SubCats.size();

  int Nv = (m_DoDraw ? 1 : 0);
  for(int i = 0; i < N; i++)
    Nv += m_SubCats[i]->GetNVisible();

  return Nv;
}

void CategoryTree::Print(int depth) const {
  if(depth < 0)
    depth = 0;

  if(m_DoDraw){
    for(int i = 0; i < 4*depth; i++)
      cout << " ";
    cout << GetSpectroscopicLabel() << endl;
    depth += 1;
  }

  int N = m_SubCats.size();
  for(int i = 0; i < N; i++)
    m_SubCats[i]->Print(depth);
}

void CategoryTree::GetListVisible(vector<const CategoryTree*>& cats) const {
  // if(m_DoDraw)
  //   cats.push_back(this);

  // int N = m_SubCats.size();
  // for(int i = 0; i < N; i++)
  //   m_SubCats[i]->GetListVisible(cats);

  int Depth = GetDepth();
  for(int i = 0; i < Depth; i++)
    GetListDepth(cats, i);
}

void CategoryTree::GetListDeepest(vector<const CategoryTree*>& cats) const {
  if(m_SubCats.size() <= 0)
    cats.push_back(this);
  else {
    int N = m_SubCats.size();
    for(int i = 0; i < N; i++)
      m_SubCats[i]->GetListDeepest(cats);
  }
}

void CategoryTree::GetListDepth(vector<const CategoryTree*>& cats, int depth) const {
  if(depth < 0)
    return;
  
  if(depth == 0 && m_DoDraw){
    cats.push_back(this);
    return;
  }

  if(m_DoDraw)
    depth -= 1;

  int N = m_SubCats.size();
  for(int i = 0; i < N; i++)
    m_SubCats[i]->GetListDepth(cats, depth);
}

///////////////////////////////////////////
////////// CategoryTreeTool class
///////////////////////////////////////////

CategoryTreeTool::CategoryTreeTool(){
  AssembleCTmodules();
}

CategoryTreeTool::~CategoryTreeTool(){
  delete _CT_gamma0_etaInc;
  delete _CT_etaC;
  delete _CT_etaF;
  delete _CT_gamma0;
  delete _CT_gamma1;
  delete _CT_ptISR0_gamma0_etaInc;
  delete _CT_ptISR0;
  delete _CT_ptISR1;
  delete _CT_ptISR0_gamma0;
  delete _CT_ptISR1_gamma0;
  delete _CT_ptISR0_gammaInc;
  delete _CT_ptISR1_gammaInc;
  delete _CT_0bISR_ptISRInc;
  delete _CT_0bISR_ptISRInc_gammaInc;
  delete _CT_ge1bISR_ptISRInc;
  delete _CT_ge1bISR_ptISRInc_gammaInc;
  delete _CT_ge1jISR_ptISR0;
  delete _CT_ge1jISR_ptISRInc;
  delete _CT_ge1jISR_ptISRInc_gammaInc;
  delete _CT_ge1jISR_ptISR0_gamma0_etaInc;
  delete _CT_0sv_inclbISR_ptISRInc;
  delete _CT_0sv_ge1jISR_ptISRInc;
  delete _CT_0sv_inclbISR_ptISRInc_gammaInc;
  delete _CT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc;
  delete _CT_0bS_inclbISR_ptISRInc_gammaInc;
  delete _CT_1bS_inclbISR_ptISRInc_gammaInc;
  delete _CT_2bS_ge1jISR_ptISRInc_gammaInc;
  delete _CT_ge2bS_ge1jISR_ptISRInc_gammaInc;
}

CategoryTree CategoryTreeTool::GetCategories() const {

  CategoryTree CT_0L = GetCategories_0L();
  CategoryTree CT_1L = GetCategories_1L();
  CategoryTree CT_2L = GetCategories_2L();
  CategoryTree CT_3L = GetCategories_3L();

  CategoryTree CT_all(VS().a("Ch"), "", "", kNorm, false);
  CT_all.AddSubCategory(CT_0L);
  CT_all.AddSubCategory(CT_1L);
  CT_all.AddSubCategory(CT_2L);
  CT_all.AddSubCategory(CT_3L);

  return CT_all;
}

CategoryTree CategoryTreeTool::GetCategoriesFakes() const {

  CategoryTree CT_Fakes1L = GetCategories_Fakes1L();
  CategoryTree CT_Fakes2L = GetCategories_Fakes2L();
  CategoryTree CT_Fakes3L = GetCategories_Fakes3L();

  CategoryTree CT_all(VS().a("Ch"), "", "", kNorm, false);
  CT_all.AddSubCategory(CT_Fakes1L);
  CT_all.AddSubCategory(CT_Fakes2L);
  CT_all.AddSubCategory(CT_Fakes3L);

  return CT_all;
}

CategoryTree CategoryTreeTool::GetCategories_0L_fineSplit() const {
  CategoryTree CT_0b(VS().a("j0b0").a("0bS"), "0b", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, true);
  CategoryTree CT_0b_1j(VS().a("j0b0").a("0bS"), "0b", "0b", kJ_sub, true);
  CategoryTree CT_1b_1j(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, true);
  CategoryTree CT_0b_5j(VS().a("j0b0").a("0bS"), "0b", "0b", kJ_sub, true);
  CategoryTree CT_1b_5j(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, true);
  CategoryTree CT_2b_5j(VS().a("2bS"), "2b", "2b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "2b", "2b", kJ_sub, true);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  CategoryTree CT_0bISR_5j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_5j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, false);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, false);
  
  CategoryTree CT_0sv(VS().a("0svS"), "0sv", "0sv", kJ_sup, false);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, false);
  CategoryTree CT_2sv(VS().a("ge2svS"), "#geq 2 sv #in S", "2sv", kJ_sup, false);
  
  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_2sv.AddSubCategory(CT_etaC);
  CT_2sv.AddSubCategory(CT_etaF);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);
  CategoryTree CT_p0_1j(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1_1j(VS().a("PTISR1"), "", "p+", kX_sup, false);
  CategoryTree CT_p0_5j(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1_5j(VS().a("PTISR1"), "", "p+", kX_sup, false);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CT_p0.AddSubCategory(CT_g0);
  CT_p0.AddSubCategory(CT_g1);
  CT_p1.AddSubCategory(CT_g0);
  CT_p1.AddSubCategory(CT_g1);

 
  CT_0bISR.AddSubCategory(CT_p0);
  CT_0bISR.AddSubCategory(CT_p1);
  CT_1bISR.AddSubCategory(CT_p0);
  CT_1bISR.AddSubCategory(CT_p1);
 
  CT_0bISR_1j.AddSubCategory(CT_p0_1j);
  CT_1bISR_1j.AddSubCategory(CT_p1_1j);
 
  
  CT_0bISR_5j.AddSubCategory(CT_p0_5j);
  CT_1bISR_5j.AddSubCategory(CT_p1_5j);

  CT_inclbISR.AddSubCategory(CT_p1);
  CT_inclbISR.AddSubCategory(CT_p0);
  
  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  
  
  CT_0b_1j.AddSubCategory(CT_0bISR_1j);
  CT_0b_1j.AddSubCategory(CT_1bISR_1j);
  CT_1b_1j.AddSubCategory(CT_0bISR_1j);
  CT_1b_1j.AddSubCategory(CT_1bISR_1j);
  CT_0b_5j.AddSubCategory(CT_0bISR_5j);
  CT_0b_5j.AddSubCategory(CT_1bISR_5j);
  CT_1b_5j.AddSubCategory(CT_0bISR_5j);
  CT_1b_5j.AddSubCategory(CT_1bISR_5j);
  CT_2b.AddSubCategory(CT_p0);
  CT_2b.AddSubCategory(CT_p1);
  CT_2b_5j.AddSubCategory(CT_p0_5j);
  CT_2b_5j.AddSubCategory(CT_p1_5j);
  

  CT_0sv.AddSubCategory(CT_0b_1j);
  CT_0sv.AddSubCategory(CT_1b_1j);
  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4 j #in S", "4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "#geq 5 j #in S", "5J", kJ, true);
  
  CT_0j.AddSubCategory(CT_1sv);
  CT_0j.AddSubCategory(CT_2sv);

  CT_1j.AddSubCategory(CT_0sv);
  CT_1j.AddSubCategory(CT_1sv);
  
  CT_1j.AddSubCategory(CT_0b_1j);
  CT_1j.AddSubCategory(CT_1b_1j);

  CT_2j.AddSubCategory(CT_0b);
  CT_2j.AddSubCategory(CT_1b);
  CT_2j.AddSubCategory(CT_2b);
  CT_3j.AddSubCategory(CT_0b);
  CT_3j.AddSubCategory(CT_1b);
  CT_3j.AddSubCategory(CT_2b);
  CT_4j.AddSubCategory(CT_0b);
  CT_4j.AddSubCategory(CT_1b);
  CT_4j.AddSubCategory(CT_2b);
  CT_ge5j.AddSubCategory(CT_0b_5j);
  CT_ge5j.AddSubCategory(CT_1b_5j);
  CT_ge5j.AddSubCategory(CT_2b_5j);
  
  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, false);
  CT_0L.AddSubCategory(CT_0j);
  CT_0L.AddSubCategory(CT_1j);
  CT_0L.AddSubCategory(CT_2j);
  CT_0L.AddSubCategory(CT_3j);
  CT_0L.AddSubCategory(CT_4j);
  CT_0L.AddSubCategory(CT_ge5j);

  return CT_0L;
}

CategoryTree CategoryTreeTool::GetCategories_0L_PTISRgamTSplit() const {
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, false);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, false);
  
  CategoryTree CT_0sv(VS().a("0svS"), "0sv", "0sv", kJ_sup, false);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, false);
  CategoryTree CT_2sv(VS().a("ge2svS"), "#geq 2 sv #in S", "2sv", kJ_sup, false);
  
  CategoryTree CT_p0(VS().a("PTISR0"), "", "p_{T}^{ISR}-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p_{T}^{ISR}+", kX_sup, false);
  CategoryTree CT_p0_1j(VS().a("PTISR0"), "", "p_{T}^{ISR}-", kX_sup, false);
  CategoryTree CT_p1_1j(VS().a("PTISR1"), "", "p_{T}^{ISR}+", kX_sup, false);
  CategoryTree CT_p0_5j(VS().a("PTISR0"), "", "p_{T}^{ISR}-", kX_sup, false);
  CategoryTree CT_p1_5j(VS().a("PTISR1"), "", "p_{T}^{ISR}+", kX_sup, false);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma_{}_{#perp}-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma_{}_{#perp}+", kX_sup, false);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4 j #in S", "4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "#geq 5 j #in S", "5J", kJ, true);
  
  CT_0j.AddSubCategory(CT_1sv);
  CT_0j.AddSubCategory(CT_2sv);
  CT_1j.AddSubCategory(CT_p0);
  CT_1j.AddSubCategory(CT_p1);
  CT_2j.AddSubCategory(CT_p0);
  CT_2j.AddSubCategory(CT_p1);
  CT_2j.AddSubCategory(CT_g0);
  CT_2j.AddSubCategory(CT_g1);
  CT_3j.AddSubCategory(CT_p0);
  CT_3j.AddSubCategory(CT_p1);
  CT_3j.AddSubCategory(CT_g0);
  CT_3j.AddSubCategory(CT_g1);
  CT_4j.AddSubCategory(CT_p0);
  CT_4j.AddSubCategory(CT_p1);
  CT_4j.AddSubCategory(CT_g0);
  CT_4j.AddSubCategory(CT_g1);
  CT_ge5j.AddSubCategory(CT_p0);
  CT_ge5j.AddSubCategory(CT_p1);
  
  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, false);
  CT_0L.AddSubCategory(CT_0j);
  CT_0L.AddSubCategory(CT_1j);
  CT_0L.AddSubCategory(CT_2j);
  CT_0L.AddSubCategory(CT_3j);
  CT_0L.AddSubCategory(CT_4j);
  CT_0L.AddSubCategory(CT_ge5j);

  return CT_0L;
}


CategoryTree CategoryTreeTool::GetCategories_0L() const {
  CategoryTree CT_0b(VS().a("j0b0").a("0bS"), "0b", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "2b", "2b", kJ_sub, true);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CT_0bISR.AddSubCategory(CT_0b);
  CT_0bISR.AddSubCategory(CT_1b);
  CT_1bISR.AddSubCategory(CT_0b);
  CT_1bISR.AddSubCategory(CT_1b);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);
  
  CategoryTree CT_0sv(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, true);
  CategoryTree CT_2sv(VS().a("ge2svS"), "#geq 2 sv #in S", "2sv", kJ_sup, true);
  
  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_2sv.AddSubCategory(CT_etaC);
  CT_2sv.AddSubCategory(CT_etaF);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k1(VS().a("PTISR1_gamT1"), "", "", kX_sup, false);
  CategoryTree CT_k1_5j(VS().a("PTISR1_gamT0"), "", "", kX_sup, false);

  //CT_g0.AddSubCategory(CT_p0);
  //CT_g0.AddSubCategory(CT_p1);
  //CT_g1.AddSubCategory(CT_p0);
  //CT_g1.AddSubCategory(CT_p1);

  // CT_k0.AddSubCategory(CT_0bISR);
  // CT_k0.AddSubCategory(CT_1bISR);
  // CT_k0.AddSubCategory(CT_2b);
  //CT_k1.AddSubCategory(CT_0bISR);
  //CT_k1.AddSubCategory(CT_1bISR);
  //CT_k1.AddSubCategory(CT_2b);
  //CT_k1_5j.AddSubCategory(CT_0bISR);
  //CT_k1_5j.AddSubCategory(CT_1bISR);
  //CT_k1_5j.AddSubCategory(CT_2b);
  
 // CT_0bISR.AddSubCategory(CT_p0);
 // CT_0bISR.AddSubCategory(CT_p1);
 // CT_1bISR.AddSubCategory(CT_p0);
 // CT_1bISR.AddSubCategory(CT_p1);
 // CT_2b.AddSubCategory(CT_p0);
 // CT_2b.AddSubCategory(CT_p1);
  CT_0sv.AddSubCategory(CT_0bISR);
  CT_0sv.AddSubCategory(CT_1bISR);

  // CT_0b.AddSubCategory(CT_g0);
  // CT_0b.AddSubCategory(CT_g1);
  // CT_1b.AddSubCategory(CT_g0);
  // CT_1b.AddSubCategory(CT_g1);
  // CT_ge1b.AddSubCategory(CT_g0);
  // CT_ge1b.AddSubCategory(CT_g1);
  // CT_2b.AddSubCategory(CT_g0);
  // CT_2b.AddSubCategory(CT_g1);
  


  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4 j #in S", "4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "#geq 5 j #in S", "5J", kJ, true);
  
  CT_0j.AddSubCategory(CT_1sv);
  CT_0j.AddSubCategory(CT_2sv);
  CT_1j.AddSubCategory(CT_0sv);
  CT_1j.AddSubCategory(CT_1sv);
  //CT_2j.AddSubCategory(CT_k1);
  CT_2j.AddSubCategory(CT_0bISR);
  CT_2j.AddSubCategory(CT_1bISR);
  CT_2j.AddSubCategory(CT_2b);
  //CT_3j.AddSubCategory(CT_k1);
  CT_3j.AddSubCategory(CT_0bISR);
  CT_3j.AddSubCategory(CT_1bISR);
  CT_3j.AddSubCategory(CT_2b);
  //CT_4j.AddSubCategory(CT_k1);
  CT_4j.AddSubCategory(CT_0bISR);
  CT_4j.AddSubCategory(CT_1bISR);
  CT_4j.AddSubCategory(CT_2b);
  //CT_ge5j.AddSubCategory(CT_k1_5j);
  CT_ge5j.AddSubCategory(CT_0bISR);
  CT_ge5j.AddSubCategory(CT_1bISR);
  CT_ge5j.AddSubCategory(CT_2b);
  
  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, false);
  CT_0L.AddSubCategory(CT_0j);
  CT_0L.AddSubCategory(CT_1j);
  CT_0L.AddSubCategory(CT_2j);
  CT_0L.AddSubCategory(CT_3j);
  CT_0L.AddSubCategory(CT_4j);
  CT_0L.AddSubCategory(CT_ge5j);

  return CT_0L;
}

CategoryTree CategoryTreeTool::GetCategories_0L_Summary() const {

  CategoryTree CT_highJ(VS().a("Ch0L"), "", "", kJ, true);

  CategoryTree CT_0j(VS().a("_0j"), "0 j #in S", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "1 j #in S", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4 j #in S", "4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "#geq 5 j #in S", "5J", kJ, true);

  CT_highJ.AddSubCategory(CT_0j);
  CT_highJ.AddSubCategory(CT_1j);
  CT_highJ.AddSubCategory(CT_2j);
  CT_highJ.AddSubCategory(CT_3j);
  CT_highJ.AddSubCategory(CT_4j);
  CT_highJ.AddSubCategory(CT_ge5j);

  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, false);
  CT_0L.AddSubCategory(CT_highJ);

  return CT_0L;
}

CategoryTree CategoryTreeTool::GetCategories_SV_Summary() const {

/*
Ch0L_0_0j1svS
Ch0L_0_0jge2svS
 //prefer ge1 (2L ll)
Ch0L_0_1j1b0svS or Ch0L_0_1jge1svS
Ch1L_mupm_slvr_0j0svS or Ch1L_lp_gold_0jge1svS
Ch1L_mupm_bron_1j0svS or Ch1L_mupm_slvr_1jge1svS
Ch2L_mumu_bron_0j0svS or Ch2L_ll_slvr_0jge1svS
*/

  CategoryTree CT(VS().a(""), "", "", kNorm, true);

  //CategoryTree CT_0L_1j_0sv(VS().a("0L").a("_1j").a("0svS"), "0L, 1J, 0SV", "0L, 1J, 0SV", kJ_sup, true);
  //CategoryTree CT_0L_1j_1sv(VS().a("0L").a("_1j").a("ge1svS"), "0L, 1J, #geq 1SV", "0L, 1J, #geq 1SV", kJ_sup, true);
  //CategoryTree CT_0L_0j_1sv(VS().a("0L").a("_0j").a("1svS"), "0L, 0J, 1SV", "0L, 0J, 1SV", kJ_sup, true);
  //CategoryTree CT_0L_0j_2sv(VS().a("0L").a("_0j").a("ge2svS"), "0L, 0J, #geq 2SV", "0L, 0J, #geq 2SV", kJ_sup, true);
  //CategoryTree CT_1L_0j_0sv(VS().a("1L").a("_0j").a("0svS"), "1L, 0J, 0SV", "1L, 0J, 0SV", kJ_sup, true);
  //CategoryTree CT_1L_0j_1sv(VS().a("1L").a("_0j").a("ge1svS"), "1L, 0J, #geq 1SV", "1L, 0J, #geq 1SV", kJ_sup, true);
  //CategoryTree CT_1L_1j_0sv(VS().a("1L").a("_1j").a("0svS"), "1L, 1J, 0SV", "1L, 1J, 0SV", kJ_sup, true);
  //CategoryTree CT_1L_1j_1sv(VS().a("1L").a("_1j").a("ge1svS"), "1L, 1J, #geq 1SV", "1L, 1J, #geq 1SV", kJ_sup, true);
  //CategoryTree CT_2L_0j_0sv(VS().a("2L").a("_0j").a("0svS"), "2L, 0J, 0SV", "2L, 0J, 0SV", kJ_sup, true);
  //CategoryTree CT_2L_0j_1sv(VS().a("2L").a("_0j").a("ge1svS"), "2L, 0J, #geq 1SV", "2L, 0J, #geq 1SV", kJ_sup, true);

  CategoryTree CT_0L_0j_1sv(VS().a("1svS"), "1SV", "1SV", kJ_sup, true);
  CategoryTree CT_0L_0j_2sv(VS().a("ge2svS"), "#geq 2SV", "#geq 2SV", kJ_sup, true);
  CategoryTree CT_0L_0j_ge1sv(VS().a("1svS").a("ge2svS"), "#geq 1SV", "#geq 1SV", kJ_sup, true);
  CategoryTree CT_0L_1j_0sv(VS().a("0svS"), "0SV", "0SV", kJ_sup, true);
  CategoryTree CT_0L_1j_1sv(VS().a("ge1svS"), "#geq 1SV", "#geq 1SV", kJ_sup, true);
  CategoryTree CT_1L_0j_0sv(VS().a("0svS"), "0SV", "0SV", kJ_sup, true);
  CategoryTree CT_1L_0j_1sv(VS().a("ge1svS"), "#geq 1SV", "#geq 1SV", kJ_sup, true);
  CategoryTree CT_1L_1j_0sv(VS().a("0svS"), "0SV", "0SV", kJ_sup, true);
  CategoryTree CT_1L_1j_1sv(VS().a("ge1svS"), "#geq 1SV", "#geq 1SV", kJ_sup, true);
  CategoryTree CT_2L_0j_0sv(VS().a("0svS"), "0SV", "0SV", kJ_sup, true);
  CategoryTree CT_2L_0j_1sv(VS().a("ge1svS"), "#geq 1SV", "#geq 1SV", kJ_sup, true);

  CategoryTree CT_0L_0j(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_0L_1j(VS().a("_1j"), "1J", "1J", kJ, true);
  CategoryTree CT_1L_0j(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_1L_1j(VS().a("_1j"), "1J", "1J", kJ, true);
  CategoryTree CT_2L_0j(VS().a("_0j"), "0J", "0J", kJ, true);

  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, true);
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, true);

  //CT_0L_0j.AddSubCategory(CT_0L_0j_1sv);
  //CT_0L_0j.AddSubCategory(CT_0L_0j_2sv);
   CT_0L_0j.AddSubCategory(CT_0L_0j_ge1sv);
  //CT_0L_1j.AddSubCategory(CT_0L_1j_0sv);
  CT_0L_1j.AddSubCategory(CT_0L_1j_1sv);

  //CT_1L_0j.AddSubCategory(CT_1L_0j_0sv);
  CT_1L_0j.AddSubCategory(CT_1L_0j_1sv);
  //CT_1L_1j.AddSubCategory(CT_1L_1j_0sv);
  CT_1L_1j.AddSubCategory(CT_1L_1j_1sv);

  //CT_2L_0j.AddSubCategory(CT_2L_0j_0sv);
  CT_2L_0j.AddSubCategory(CT_2L_0j_1sv);

  CT_0L.AddSubCategory(CT_0L_0j);
  CT_0L.AddSubCategory(CT_0L_1j);

  CT_1L.AddSubCategory(CT_1L_0j);
  CT_1L.AddSubCategory(CT_1L_1j);

  CT_2L.AddSubCategory(CT_2L_0j);

  CT.AddSubCategory(CT_0L);
  CT.AddSubCategory(CT_1L);
  CT.AddSubCategory(CT_2L);

  return CT;
}

CategoryTree CategoryTreeTool::GetCategories_0L_plotFormat() const {
  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4 j #in S", "4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "#geq 5 j #in S", "5J", kJ, true);
  
  
  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, true);
  CT_0L.AddSubCategory(CT_0j);
  CT_0L.AddSubCategory(CT_1j);
  CT_0L.AddSubCategory(CT_2j);
  CT_0L.AddSubCategory(CT_3j);
  CT_0L.AddSubCategory(CT_4j);
  CT_0L.AddSubCategory(CT_ge5j);

  return CT_0L;
}

CategoryTree CategoryTreeTool::GetCategories_1L_ChargeSep() const {

  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_0j(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_elp(VS().a("elp"), "elp", "e^{+}", kL_sub, true);
  CategoryTree CT_elm(VS().a("elm"), "elm", "e^{-}", kL_sub, true);
  CategoryTree CT_mup(VS().a("mup"), "mup", "#mu^{+}", kL_sub, true);
  CategoryTree CT_mum(VS().a("mum"), "mum", "#mu^{-}", kL_sub, true);
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, false);

  CT_0j.AddSubCategory(CT_elp);
  CT_0j.AddSubCategory(CT_elm);
  CT_0j.AddSubCategory(CT_mup);
  CT_0j.AddSubCategory(CT_mum);
  CT_gold.AddSubCategory(CT_0j);

  CT_1L.AddSubCategory(CT_gold);

  return CT_1L;
}

CategoryTree CategoryTreeTool::GetCategories_1L_PTISRgamTSplit() const {

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "4J", kJ, true);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);


  CT_0j.AddSubCategory(CT_p0);
  CT_0j.AddSubCategory(CT_p1);

  CT_1j.AddSubCategory(CT_g0);
  CT_1j.AddSubCategory(CT_g1);
  CT_1j.AddSubCategory(CT_p0);
  CT_1j.AddSubCategory(CT_p1);

  CT_2j.AddSubCategory(CT_g0);
  CT_2j.AddSubCategory(CT_g1);
  CT_2j.AddSubCategory(CT_p0);
  CT_2j.AddSubCategory(CT_p1);

  CT_3j.AddSubCategory(CT_g0);
  CT_3j.AddSubCategory(CT_g1);
  CT_3j.AddSubCategory(CT_p0);
  CT_3j.AddSubCategory(CT_p1);

  CT_ge4j.AddSubCategory(CT_g0);
  CT_ge4j.AddSubCategory(CT_g1);
  CT_ge4j.AddSubCategory(CT_p0);
  CT_ge4j.AddSubCategory(CT_p1);
  

  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, false);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);

  return CT_1L;
}


CategoryTree CategoryTreeTool::GetCategories_1L_fineSplit() const {
  
  CategoryTree CT_0b(VS().a("j0b").a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("jge1bS").a("j1b"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_0b_notGold(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b_notGold(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "#geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "#geq 1 j #in ISR", "", kX_sub, true);
  
  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);

  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);
  CategoryTree CT_p0_0j(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1_0j(VS().a("PTISR1"), "", "p+", kX_sup, false);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);

  CT_p0.AddSubCategory(CT_gamT0);
  CT_p0.AddSubCategory(CT_gamT1);
  CT_p1.AddSubCategory(CT_gamT0);
  CT_p1.AddSubCategory(CT_gamT1);

  CT_0bISR_0j.AddSubCategory(CT_p0_0j);
  CT_0bISR_0j.AddSubCategory(CT_p1_0j);
  CT_0bISR_1j.AddSubCategory(CT_p0);
  CT_0bISR_1j.AddSubCategory(CT_p1);
  CT_1bISR_0j.AddSubCategory(CT_p0_0j);
  CT_1bISR_0j.AddSubCategory(CT_p1_0j);
  CT_1bISR_1j.AddSubCategory(CT_p0);
  CT_1bISR_1j.AddSubCategory(CT_p1);

  CT_0bISR.AddSubCategory(CT_p0);
  CT_0bISR.AddSubCategory(CT_p1);
  CT_1bISR.AddSubCategory(CT_p0);
  CT_1bISR.AddSubCategory(CT_p1);
  
  CT_0b.AddSubCategory(CT_1bISR);
  CT_0b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  CT_2b.AddSubCategory(CT_p0);
  CT_2b.AddSubCategory(CT_p1);
 
  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_ge1sv_notGold(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
 
  CT_0sv_1j.AddSubCategory(CT_0b);
  CT_0sv_1j.AddSubCategory(CT_1b);
 
  CT_0sv_notGold.AddSubCategory(CT_p0_0j);
  CT_0sv_notGold.AddSubCategory(CT_p1_0j);
  
  CT_ge1sv_0j.AddSubCategory(CT_etaC);
  CT_ge1sv_0j.AddSubCategory(CT_etaF);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);

  CT_ge1sv_notGold.AddSubCategory(CT_etaC);
  CT_ge1sv_notGold.AddSubCategory(CT_etaF);
  
  CategoryTree CT_gold_0j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_1j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_ge2j(VS().a("gold"), "gold", "G", kL_sub, true);
 
  CategoryTree CT_silver_01j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_silver_ge2j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze_01j(VS().a("bron"), "bronze", "B", kL_sub, true);
  CategoryTree CT_bronze_ge2j(VS().a("bron"), "bronze", "B", kL_sub, true);

  CT_gold_0j.AddSubCategory(CT_0sv_0j);
  CT_gold_0j.AddSubCategory(CT_ge1sv_0j);
 
  CT_gold_1j.AddSubCategory(CT_0sv_1j);
  CT_gold_1j.AddSubCategory(CT_ge1sv_1j);
 
  CT_gold_ge2j.AddSubCategory(CT_0b);
  CT_gold_ge2j.AddSubCategory(CT_1b);
  CT_gold_ge2j.AddSubCategory(CT_2b);
  
  CT_silver_01j.AddSubCategory(CT_0sv_notGold);
  CT_silver_01j.AddSubCategory(CT_ge1sv_notGold);
  CT_silver_ge2j.AddSubCategory(CT_p0);
  CT_silver_ge2j.AddSubCategory(CT_p1);
  
  CT_bronze_01j.AddSubCategory(CT_0sv_notGold);
  CT_bronze_01j.AddSubCategory(CT_ge1sv_notGold);
  CT_bronze_ge2j.AddSubCategory(CT_p0);
  CT_bronze_ge2j.AddSubCategory(CT_p1);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "4J", kJ, true);

  CategoryTree CT_elp(VS().a("elp"), "elp", "e^{+}", kL_sub, true);
  CategoryTree CT_elm(VS().a("elm"), "elm", "e^{-}", kL_sub, true);
  CategoryTree CT_mup(VS().a("mup"), "mup", "#mu^{+}", kL_sub, true);
  CategoryTree CT_mum(VS().a("mum"), "mum", "#mu^{-}", kL_sub, true);
  
  CT_0j.AddSubCategory(CT_gold_0j);
  CT_0j.AddSubCategory(CT_silver_01j);
  CT_0j.AddSubCategory(CT_bronze_01j);
  
  CT_1j.AddSubCategory(CT_gold_1j);
  CT_1j.AddSubCategory(CT_silver_01j);
  CT_1j.AddSubCategory(CT_bronze_01j);
 
  CT_2j.AddSubCategory(CT_gold_ge2j);
  CT_2j.AddSubCategory(CT_silver_ge2j);
  CT_2j.AddSubCategory(CT_bronze_ge2j);
  
  CT_3j.AddSubCategory(CT_gold_ge2j);
  CT_3j.AddSubCategory(CT_silver_ge2j);
  CT_3j.AddSubCategory(CT_bronze_ge2j);
  
  CT_ge4j.AddSubCategory(CT_gold_ge2j);
  CT_ge4j.AddSubCategory(CT_silver_ge2j);
  CT_ge4j.AddSubCategory(CT_bronze_ge2j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, false);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);

  return CT_1L;
}

CategoryTree CategoryTreeTool::GetCategories_1L_Summary() const {

  CategoryTree CT_gsbSplit(VS().a("Ch1L"), "", "1L", kL, true);
  CategoryTree CT_jSplit(VS().a("Ch1L"), "", "1L", kL, true);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "4J", kJ, true);

  CT_jSplit.AddSubCategory(CT_0j);
  CT_jSplit.AddSubCategory(CT_1j);
  CT_jSplit.AddSubCategory(CT_2j);
  CT_jSplit.AddSubCategory(CT_3j);
  CT_jSplit.AddSubCategory(CT_ge4j);

  CategoryTree CT_gold(VS().a("gold"), "gold", "gold", kL_sub, true); 
  CategoryTree CT_slvr(VS().a("slvr"), "silver", "silver", kL_sub, true);
  CategoryTree CT_bron(VS().a("bron"), "bronze", "bronze", kL_sub, true);

  CT_gsbSplit.AddSubCategory(CT_gold);
  CT_gsbSplit.AddSubCategory(CT_slvr);
  CT_gsbSplit.AddSubCategory(CT_bron);

  CategoryTree CT_1L(VS().a("Ch1L"), "", "1L", kL, false);
  CT_1L.AddSubCategory(CT_gsbSplit);
  CT_1L.AddSubCategory(CT_jSplit);

  return CT_1L;
}


CategoryTree CategoryTreeTool::GetCategories_1L() const {
  
  CategoryTree CT_0b(VS().a("j0b").a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("jge1bS").a("j1b"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_0b_notGold(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b_notGold(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "#geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "#geq 1 j #in ISR", "", kX_sub, true);
  
  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);

  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);

 // CT_0bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_0bISR_0jsv.AddSubCategory(CT_etaC);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaC);
  
  //CT_0bISR_0j.AddSubCategory(CT_p0);
  //CT_0bISR_0j.AddSubCategory(CT_p1);
  //CT_1bISR_0j.AddSubCategory(CT_p0);
  //CT_1bISR_0j.AddSubCategory(CT_p1);

  //CT_0bISR_1j.AddSubCategory(CT_p0);
  //CT_0bISR_1j.AddSubCategory(CT_p1);
  //CT_1bISR_1j.AddSubCategory(CT_p0);
  //CT_1bISR_1j.AddSubCategory(CT_p1);

  //CT_0bISR.AddSubCategory(CT_0b);
  //CT_0bISR.AddSubCategory(CT_1b);
  //CT_1bISR.AddSubCategory(CT_0b);
  //CT_1bISR.AddSubCategory(CT_1b);
  
  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  //CT_ge1b.AddSubCategory(CT_gamT0);
  //CT_ge1b.AddSubCategory(CT_gamT1);
  //CT_2b.AddSubCategory(CT_inclbISR);
  //CT_2b.AddSubCategory(CT_gamT1);
 
  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_ge1sv_notGold(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  // CT_0sv_1j.AddSubCategory(CT_0bISR);
  // CT_0sv_1j.AddSubCategory(CT_1bISR);
  CT_0sv_1j.AddSubCategory(CT_0bISR_1j);
  CT_0sv_1j.AddSubCategory(CT_1bISR_1j);
  CT_ge1sv_0j.AddSubCategory(CT_etaC);
  CT_ge1sv_0j.AddSubCategory(CT_etaF);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);

  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_ge1sv_notGold.AddSubCategory(CT_etaC);
  CT_ge1sv_notGold.AddSubCategory(CT_etaF);
  
  CategoryTree CT_gold_0j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_1j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_ge2j(VS().a("gold"), "gold", "G", kL_sub, true);
 
  CategoryTree CT_silver_01j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_silver_ge2j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze_01j(VS().a("bron"), "bronze", "B", kL_sub, true);
  CategoryTree CT_bronze_ge2j(VS().a("bron"), "bronze", "B", kL_sub, true);

  CT_gold_0j.AddSubCategory(CT_0sv_0j);
  CT_gold_0j.AddSubCategory(CT_ge1sv_0j);
  CT_gold_1j.AddSubCategory(CT_0sv_1j);
  CT_gold_1j.AddSubCategory(CT_ge1sv_1j);
  CT_gold_ge2j.AddSubCategory(CT_0b);
  CT_gold_ge2j.AddSubCategory(CT_1b);
  CT_gold_ge2j.AddSubCategory(CT_2b);
  
  CT_silver_01j.AddSubCategory(CT_0sv_notGold);
  CT_silver_01j.AddSubCategory(CT_ge1sv_notGold);
  //CT_silver_ge2j.AddSubCategory(CT_inclbISR);
  
  CT_bronze_01j.AddSubCategory(CT_0sv_notGold);
  CT_bronze_01j.AddSubCategory(CT_ge1sv_notGold);
  //CT_bronze_ge2j.AddSubCategory(CT_inclbISR);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "4J", kJ, true);
  
  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, false);
  
  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_2j);
  CT_gold.AddSubCategory(CT_3j);
  CT_gold.AddSubCategory(CT_ge4j);

  CT_0j.AddSubCategory(CT_gold_0j);
  CT_0j.AddSubCategory(CT_silver_01j);
  CT_0j.AddSubCategory(CT_bronze_01j);
  CT_1j.AddSubCategory(CT_gold_1j);
  CT_1j.AddSubCategory(CT_silver_01j);
  CT_1j.AddSubCategory(CT_bronze_01j);
  CT_2j.AddSubCategory(CT_gold_ge2j);
  CT_2j.AddSubCategory(CT_silver_ge2j);
  CT_2j.AddSubCategory(CT_bronze_ge2j);
  CT_3j.AddSubCategory(CT_gold_ge2j);
  CT_3j.AddSubCategory(CT_silver_ge2j);
  CT_3j.AddSubCategory(CT_bronze_ge2j);
  CT_ge4j.AddSubCategory(CT_gold_ge2j);
  CT_ge4j.AddSubCategory(CT_silver_ge2j);
  CT_ge4j.AddSubCategory(CT_bronze_ge2j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, false);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);


  return CT_1L;
}

CategoryTree CategoryTreeTool::GetCategories_1L_plotFormat() const {
  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "4J", kJ, true);

  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);

  return CT_1L;
  
}

CategoryTree CategoryTreeTool::GetCategories_2L_PTISRgamTSplit() const {

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);
  
  CT_0j.AddSubCategory(CT_g1);
  CT_0j.AddSubCategory(CT_g0);

  CT_1j.AddSubCategory(CT_g1);
  CT_1j.AddSubCategory(CT_g0);

  CT_ge2j.AddSubCategory(CT_g1);
  CT_ge2j.AddSubCategory(CT_g0);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, false);
  CT_2L.AddSubCategory(CT_0j);
  CT_2L.AddSubCategory(CT_1j);
  CT_2L.AddSubCategory(CT_ge2j);

  return CT_2L;
  
}


CategoryTree CategoryTreeTool::GetCategories_2L_fineSplit() const {

  CategoryTree CT_0b(VS().a("j0bS"), "0b", "0b", kJ_sub, false);
  CategoryTree CT_1b(VS().a("j1bS").a("jge1bS"), "1b", "1b", kJ_sub, false);
 
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "incl. b #in ISR", "", kX_sub, true);
  
  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CT_0bISR.AddSubCategory(CT_g0);
  CT_0bISR.AddSubCategory(CT_g1);

  CT_1bISR.AddSubCategory(CT_g0);
  CT_1bISR.AddSubCategory(CT_g1);
  
  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);
  
  
  CategoryTree CT_0sv_OS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_SS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, true);


  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_0sv_OS.AddSubCategory(CT_0bISR);
  CT_0sv_OS.AddSubCategory(CT_1bISR);
  CT_0sv_SS.AddSubCategory(CT_inclbISR);
  
//gold only
  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS_0(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_OS(VS().a("OS"),"OS","OS",kL_sup,true);


//silver, bronze only
 // CategoryTree CT_elel_0j(VS().a("elel"), "elel", "elel", kL_sup, true);
 // CategoryTree CT_elmu_0j(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
 // CategoryTree CT_mumu_0j(VS().a("mumu"),"mumu","mumu",kL_sup, true);
 //
 // CategoryTree CT_elel(VS().a("elel"), "elel", "elel", kL_sup, true);
 // CategoryTree CT_elmu(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
 // CategoryTree CT_mumu(VS().a("mumu"),"mumu","mumu",kL_sup, true);
//gold, silver, and bronze
  CategoryTree CT_ll(VS().a("ll"),"ll","ll",kL_sup,true);


 //gold 
  CT_Z.AddSubCategory(CT_0b);
  CT_Z.AddSubCategory(CT_1b);
  CT_noZ.AddSubCategory(CT_0b);
  CT_noZ.AddSubCategory(CT_1b);


//change here to add flavor separation in OS gold
  CT_OS.AddSubCategory(CT_0sv_OS);
  //CT_OSelel.AddSubCategory(CT_0sv_OS);
  //CT_OSelmu.AddSubCategory(CT_0sv_OS);
  //CT_OSmumu.AddSubCategory(CT_0sv_OS);
  //CT_OS.AddSubCategory(CT_OSelel);
  //CT_OS.AddSubCategory(CT_OSelmu);
  //CT_OS.AddSubCategory(CT_OSmumu);
  
  CT_ll.AddSubCategory(CT_1sv);
  
//SS regions are only for 0sv for 0j
  CT_SS.AddSubCategory(CT_inclbISR);
  CT_SS_0.AddSubCategory(CT_0sv_SS);
  
//s/b 0j
//these regions are only for 0sv for 0j
  //CT_elel_0j.AddSubCategory(CT_0sv_notGold);
  //CT_elmu_0j.AddSubCategory(CT_0sv_notGold);
  //CT_mumu_0j.AddSubCategory(CT_0sv_notGold);


  
  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);
  CategoryTree CT_0j_notGold(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j_notGold(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j_notGold(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);
  
  CT_0j.AddSubCategory(CT_ll);
  CT_0j.AddSubCategory(CT_SS_0);
  CT_0j.AddSubCategory(CT_OS);
  CT_0j_notGold.AddSubCategory(CT_1sv);
  CT_0j_notGold.AddSubCategory(CT_0sv_notGold);
 // CT_0j_notGold.AddSubCategory(CT_elel_0j);
 // CT_0j_notGold.AddSubCategory(CT_elmu_0j);
 // CT_0j_notGold.AddSubCategory(CT_mumu_0j);

  CT_1j.AddSubCategory(CT_Z);
  CT_1j.AddSubCategory(CT_noZ);
  CT_1j.AddSubCategory(CT_SS);
 // CT_1j_notGold.AddSubCategory(CT_elel);
 // CT_1j_notGold.AddSubCategory(CT_elmu);
 // CT_1j_notGold.AddSubCategory(CT_mumu);

  CT_ge2j.AddSubCategory(CT_Z);
  CT_ge2j.AddSubCategory(CT_noZ);
  CT_ge2j.AddSubCategory(CT_SS);
 // CT_ge2j_notGold.AddSubCategory(CT_elel);
 // CT_ge2j_notGold.AddSubCategory(CT_elmu);
 // CT_ge2j_notGold.AddSubCategory(CT_mumu);

  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, false);

  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_ge2j);
 
  CT_silver.AddSubCategory(CT_0j_notGold);
  CT_silver.AddSubCategory(CT_1j_notGold);
  CT_silver.AddSubCategory(CT_ge2j_notGold);

  CT_bronze.AddSubCategory(CT_0j_notGold);
  CT_bronze.AddSubCategory(CT_1j_notGold);
  CT_bronze.AddSubCategory(CT_ge2j_notGold);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, false);
  CT_2L.AddSubCategory(CT_bronze);
  CT_2L.AddSubCategory(CT_silver);
  CT_2L.AddSubCategory(CT_gold);

  return CT_2L;
  
}

CategoryTree CategoryTreeTool::GetCategories_2L_flavorSplit() const {
  CategoryTree CT_0b(VS().a("j0bS"), "0b", "0b", kJ_sub, false);
  CategoryTree CT_1b(VS().a("j1bS").a("jge1bS"), "1b", "1b", kJ_sub, false);

  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "incl. b #in ISR", "", kX_sub, true);
  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);


  CT_0bISR.AddSubCategory(CT_g0);
  CT_0bISR.AddSubCategory(CT_g1);

  CT_1bISR.AddSubCategory(CT_g0);
  CT_1bISR.AddSubCategory(CT_g1);

  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);

  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);


  CategoryTree CT_0sv_OS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_SS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);

  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_0sv_OS.AddSubCategory(CT_0bISR);
  CT_0sv_OS.AddSubCategory(CT_1bISR);
  CT_0sv_SS.AddSubCategory(CT_inclbISR);
  /*
  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_0sv_OS.AddSubCategory(CT_0bISR);
  CT_0sv_OS.AddSubCategory(CT_1bISR);
  CT_0sv_SS.AddSubCategory(CT_inclbISR);
  */
  //gold only
  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS_0(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_OS(VS().a("OS"),"OS","OS",kL_sup,true);

  CategoryTree CT_OSmumu(VS().a("_OSmumu"),"mumu","mumu",kL_sup,false);
  CategoryTree CT_OSelmu(VS().a("_OSelmu"),"elmu","elmu",kL_sup,false);
  CategoryTree CT_OSelel(VS().a("_OSelel"),"elel","elel",kL_sup,false);

  //silver, bronze only
  CategoryTree CT_elel_0j(VS().a("elel"), "elel", "elel", kL_sup, true);
  CategoryTree CT_elmu_0j(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
  CategoryTree CT_mumu_0j(VS().a("mumu"),"mumu","mumu",kL_sup, true);

  CategoryTree CT_elel(VS().a("elel"), "elel", "elel", kL_sup, true);
  CategoryTree CT_elmu(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
  CategoryTree CT_mumu(VS().a("mumu"),"mumu","mumu",kL_sup, true);
  //gold, silver, and bronze
  CategoryTree CT_ll(VS().a("ll"),"ll","ll",kL_sup,true);

  //gold
  CT_Z.AddSubCategory(CT_0b);
  CT_Z.AddSubCategory(CT_1b);
  CT_noZ.AddSubCategory(CT_0b);
  CT_noZ.AddSubCategory(CT_1b);
//change here to add flavor separation in OS gold
//  //CT_OS.AddSubCategory(CT_0sv_OS);
  CT_OSelel.AddSubCategory(CT_0sv_OS);
  CT_OSelmu.AddSubCategory(CT_0sv_OS);
  CT_OSmumu.AddSubCategory(CT_0sv_OS);

  CT_OS.AddSubCategory(CT_OSelel);
  CT_OS.AddSubCategory(CT_OSelmu);
  CT_OS.AddSubCategory(CT_OSmumu);

  CT_ll.AddSubCategory(CT_1sv);

//SS regions are only for 0sv for 0j
  CT_SS.AddSubCategory(CT_inclbISR);
  CT_SS_0.AddSubCategory(CT_0sv_SS);

//s/b 0j
////these regions are only for 0sv for 0j
  CT_elel_0j.AddSubCategory(CT_0sv_notGold);
  CT_elmu_0j.AddSubCategory(CT_0sv_notGold);
  CT_mumu_0j.AddSubCategory(CT_0sv_notGold);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);
  CategoryTree CT_0j_notGold(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j_notGold(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j_notGold(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);

  CT_0j.AddSubCategory(CT_ll);
  CT_0j.AddSubCategory(CT_SS_0);
  CT_0j.AddSubCategory(CT_OS);
  //CT_0j_notGold.AddSubCategory(CT_1sv);
  //  //CT_0j_notGold.AddSubCategory(CT_0sv_notGold);
    CT_0j_notGold.AddSubCategory(CT_elel_0j);
  CT_0j_notGold.AddSubCategory(CT_elmu_0j);
  CT_0j_notGold.AddSubCategory(CT_mumu_0j);
  CT_0j_notGold.AddSubCategory(CT_ll);

CT_1j.AddSubCategory(CT_Z);
  CT_1j.AddSubCategory(CT_noZ);
  CT_1j.AddSubCategory(CT_SS);
  CT_1j_notGold.AddSubCategory(CT_elel);
  CT_1j_notGold.AddSubCategory(CT_elmu);
  CT_1j_notGold.AddSubCategory(CT_mumu);

  CT_ge2j.AddSubCategory(CT_Z);
  CT_ge2j.AddSubCategory(CT_noZ);
  CT_ge2j.AddSubCategory(CT_SS);
  CT_ge2j_notGold.AddSubCategory(CT_elel);
  CT_ge2j_notGold.AddSubCategory(CT_elmu);
  CT_ge2j_notGold.AddSubCategory(CT_mumu);

  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, false);

  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_ge2j);

  CT_silver.AddSubCategory(CT_0j_notGold);
  CT_silver.AddSubCategory(CT_1j_notGold);
  CT_silver.AddSubCategory(CT_ge2j_notGold);

  CT_bronze.AddSubCategory(CT_0j_notGold);
  CT_bronze.AddSubCategory(CT_1j_notGold);
  CT_bronze.AddSubCategory(CT_ge2j_notGold);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, false);
  CT_2L.AddSubCategory(CT_bronze);
  CT_2L.AddSubCategory(CT_silver);
  CT_2L.AddSubCategory(CT_gold);

  return CT_2L;

}

CategoryTree CategoryTreeTool::GetCategories_2L_Summary() const {

  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_OS(VS().a("OS"),"OS","OS",kL_sup,true);

  CategoryTree CT_elel(VS().a("elel"), "elel", "e^{#pm} e^{#mp}", kL_sup, true);
  CategoryTree CT_elmu(VS().a("elmu"), "elmu", "e^{#pm} #mu^{#mp}", kL_sup, true);
  CategoryTree CT_mumu(VS().a("mumu"), "mumu", "#mu^{#pm} #mu^{#mp}",kL_sup, true);

  CategoryTree CT_ll(VS().a("ll"),"ge1sv","#geq 1sv",kL_sup,true);

  CategoryTree CT_cat1(VS().a("Ch2L"), "", "2L", kL, true);
  CategoryTree CT_cat2(VS().a("Ch2L"), "", "2L", kL, true);

  CT_cat1.AddSubCategory(CT_elel);
  CT_cat1.AddSubCategory(CT_elmu);
  CT_cat1.AddSubCategory(CT_mumu);
  //CT_cat2.AddSubCategory(CT_ll);
  CT_cat2.AddSubCategory(CT_Z);
  CT_cat2.AddSubCategory(CT_noZ);
  CT_cat2.AddSubCategory(CT_OS);
  CT_cat2.AddSubCategory(CT_SS);

  CategoryTree CT_gold(VS().a("gold"), "gold", "gold", kL, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "silver", kL, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "bronze", kL, false);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);

  CategoryTree CT_gsbJ(VS().a("Ch2L"), "", "2L", kL, true);

  CT_gsbJ.AddSubCategory(CT_0j);
  CT_gsbJ.AddSubCategory(CT_1j);
  CT_gsbJ.AddSubCategory(CT_ge2j);
  CT_gsbJ.AddSubCategory(CT_gold);
  CT_gsbJ.AddSubCategory(CT_silver);
  CT_gsbJ.AddSubCategory(CT_bronze);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, false);
  CT_2L.AddSubCategory(CT_gsbJ);
  CT_2L.AddSubCategory(CT_cat1);
  CT_2L.AddSubCategory(CT_cat2);

  return CT_2L;

}


CategoryTree CategoryTreeTool::GetCategories_2L() const {
  CategoryTree CT_0b(VS().a("j0bS"), "0b", "0b", kJ_sub, false);
  CategoryTree CT_1b(VS().a("j1bS").a("jge1bS"), "1b", "1b", kJ_sub, false);
 
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "incl. b #in ISR", "", kX_sub, true);

  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);
  
  
  CategoryTree CT_0sv_OS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_SS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);

//  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
//  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

 // CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
//  CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1"), "", "k-", kX_sup, false);
//  CategoryTree CT_k0_Z(VS().a("PTISR0_gamT0").a("PTISR0_gamT1"), "", "k-", kX_sup, false);
  //CategoryTree CT_k1(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  //CategoryTree CT_k0_0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
//  CategoryTree CT_k0_0(VS().a("PTISR0_gamT0"), "", "k-", kX_sup, false);
  //CategoryTree CT_k1_0(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  //CT_g0.AddSubCategory(CT_p0);
  //CT_g0.AddSubCategory(CT_p1);
  //CT_g1.AddSubCategory(CT_p0);
  //CT_g1.AddSubCategory(CT_p1);
 
  
  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_0sv_OS.AddSubCategory(CT_0bISR);
  CT_0sv_OS.AddSubCategory(CT_1bISR);
  CT_0sv_SS.AddSubCategory(CT_inclbISR);
  
//gold only
  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS_0(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_OS(VS().a("OS"),"OS","OS",kL_sup,true);

  CategoryTree CT_OSmumu(VS().a("_OSmumu"),"mumu","mumu",kL_sup,false);
  CategoryTree CT_OSelmu(VS().a("_OSelmu"),"elmu","elmu",kL_sup,false);
  CategoryTree CT_OSelel(VS().a("_OSelel"),"elel","elel",kL_sup,false);
  

//silver, bronze only
 // CategoryTree CT_elel_0j(VS().a("elel"), "elel", "elel", kL_sup, true);
 // CategoryTree CT_elmu_0j(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
 // CategoryTree CT_mumu_0j(VS().a("mumu"),"mumu","mumu",kL_sup, true);
 //
 // CategoryTree CT_elel(VS().a("elel"), "elel", "elel", kL_sup, true);
 // CategoryTree CT_elmu(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
 // CategoryTree CT_mumu(VS().a("mumu"),"mumu","mumu",kL_sup, true);
//gold, silver, and bronze
  CategoryTree CT_ll(VS().a("ll"),"ll","ll",kL_sup,true);


 //gold 
  CT_Z.AddSubCategory(CT_0b);
  CT_Z.AddSubCategory(CT_1b);
  CT_noZ.AddSubCategory(CT_0b);
  CT_noZ.AddSubCategory(CT_1b);


//change here to add flavor separation in OS gold
  CT_OS.AddSubCategory(CT_0sv_OS);
  //CT_OSelel.AddSubCategory(CT_0sv_OS);
  //CT_OSelmu.AddSubCategory(CT_0sv_OS);
  //CT_OSmumu.AddSubCategory(CT_0sv_OS);
  //CT_OS.AddSubCategory(CT_OSelel);
  //CT_OS.AddSubCategory(CT_OSelmu);
  //CT_OS.AddSubCategory(CT_OSmumu);
  
  CT_ll.AddSubCategory(CT_1sv);
  
//SS regions are only for 0sv for 0j
  CT_SS.AddSubCategory(CT_inclbISR);
  CT_SS_0.AddSubCategory(CT_0sv_SS);
  
//s/b 0j
//these regions are only for 0sv for 0j
  //CT_elel_0j.AddSubCategory(CT_0sv_notGold);
  //CT_elmu_0j.AddSubCategory(CT_0sv_notGold);
  //CT_mumu_0j.AddSubCategory(CT_0sv_notGold);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);
  CategoryTree CT_0j_notGold(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j_notGold(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_ge2j_notGold(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);
  
  CT_0j.AddSubCategory(CT_ll);
  CT_0j.AddSubCategory(CT_SS_0);
  CT_0j.AddSubCategory(CT_OS);
  CT_0j_notGold.AddSubCategory(CT_1sv);
  CT_0j_notGold.AddSubCategory(CT_0sv_notGold);
 // CT_0j_notGold.AddSubCategory(CT_elel_0j);
 // CT_0j_notGold.AddSubCategory(CT_elmu_0j);
 // CT_0j_notGold.AddSubCategory(CT_mumu_0j);

  CT_1j.AddSubCategory(CT_Z);
  CT_1j.AddSubCategory(CT_noZ);
  CT_1j.AddSubCategory(CT_SS);
 // CT_1j_notGold.AddSubCategory(CT_elel);
 // CT_1j_notGold.AddSubCategory(CT_elmu);
 // CT_1j_notGold.AddSubCategory(CT_mumu);

  CT_ge2j.AddSubCategory(CT_Z);
  CT_ge2j.AddSubCategory(CT_noZ);
  CT_ge2j.AddSubCategory(CT_SS);
 // CT_ge2j_notGold.AddSubCategory(CT_elel);
 // CT_ge2j_notGold.AddSubCategory(CT_elmu);
 // CT_ge2j_notGold.AddSubCategory(CT_mumu);

  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, false);

  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_ge2j);
 
  CT_silver.AddSubCategory(CT_0j_notGold);
  CT_silver.AddSubCategory(CT_1j_notGold);
  CT_silver.AddSubCategory(CT_ge2j_notGold);

  CT_bronze.AddSubCategory(CT_0j_notGold);
  CT_bronze.AddSubCategory(CT_1j_notGold);
  CT_bronze.AddSubCategory(CT_ge2j_notGold);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, false);
  CT_2L.AddSubCategory(CT_bronze);
  CT_2L.AddSubCategory(CT_silver);
  CT_2L.AddSubCategory(CT_gold);

  return CT_2L;
  
}

/*
CategoryTree CategoryTreeTool::GetCategories_2L_plotFormat() const {
  CategoryTree CT_0j(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "1J", "1J", kJ, true);
  CategoryTree CT_ge2j(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, true);
  CT_2L.AddSubCategory(CT_0j);
  CT_2L.AddSubCategory(CT_1j);
  CT_2L.AddSubCategory(CT_ge2j);

  return CT_2L;
  
}
*/

CategoryTree CategoryTreeTool::GetCategories_2L_plotFormat() const {
  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_OS(VS().a("OS"),"OS","OS",kL_sup,true);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, true);
  CT_2L.AddSubCategory(CT_Z);
  CT_2L.AddSubCategory(CT_noZ);
  CT_2L.AddSubCategory(CT_OS);
  CT_2L.AddSubCategory(CT_SS);

  return CT_2L;
}


CategoryTree CategoryTreeTool::GetCategories_3L() const {
  

  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);

  CategoryTree CT_0j(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_ge1j(VS().a("_ge1jS"), "#geq 1J", "1J", kJ, true);
  CategoryTree CT_inclJs(VS().a("_inclS"), "inclJS", "inclJ", kJ, true);
 
  CT_0j.AddSubCategory(CT_Z);
  CT_0j.AddSubCategory(CT_noZ); 

  CT_ge1j.AddSubCategory(CT_Z);
  CT_ge1j.AddSubCategory(CT_noZ); 

  //CT_inclJs.AddSubCategory(CT_SS);
  

  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, false);

  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_ge1j);
  CT_gold.AddSubCategory(CT_inclJs);
  
  CT_silver.AddSubCategory(CT_0j);
  CT_silver.AddSubCategory(CT_ge1j);
  CT_silver.AddSubCategory(CT_inclJs);
  
  CT_bronze.AddSubCategory(CT_0j);
  CT_bronze.AddSubCategory(CT_ge1j);
  CT_bronze.AddSubCategory(CT_inclJs);
  
  CategoryTree CT_3L(VS().a("Ch3L"), "3L", "3L", kL, false);
  CT_3L.AddSubCategory(CT_bronze);
  CT_3L.AddSubCategory(CT_silver);
  CT_3L.AddSubCategory(CT_gold);

  return CT_3L;
}

CategoryTree CategoryTreeTool::GetCategories_3L_plotFormat() const {
  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_ge1j"), "", "#geq 1J", kJ, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);

  CategoryTree CT_3L(VS().a("Ch3L"), "3L", "3L", kL, true);
  CT_3L.AddSubCategory(CT_0j);
  CT_3L.AddSubCategory(CT_1j);
  CT_3L.AddSubCategory(CT_Z);
  CT_3L.AddSubCategory(CT_noZ);
  CT_3L.AddSubCategory(CT_SS);

  return CT_3L;
  
}

CategoryTree CategoryTreeTool::GetCategories_Fakes1L() const {
  
  CategoryTree CT_0b(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_0b_notGold(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b_notGold(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "#geq 1 j #in ISR", "ge1j", kX_sub, false);
  
  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);

 // CT_0bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_0bISR_0jsv.AddSubCategory(CT_etaC);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaC);
  
  CT_0bISR_0j.AddSubCategory(CT_p0);
  CT_0bISR_0j.AddSubCategory(CT_p1);
  CT_1bISR_0j.AddSubCategory(CT_p0);
  CT_1bISR_0j.AddSubCategory(CT_p1);

  CT_0bISR.AddSubCategory(CT_0b);
  CT_0bISR.AddSubCategory(CT_1b);
  CT_1bISR.AddSubCategory(CT_0b);
  CT_1bISR.AddSubCategory(CT_1b);
  
  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_1bISR);
  CT_0b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  CT_ge1b.AddSubCategory(CT_gamT0);
  CT_ge1b.AddSubCategory(CT_gamT1);
  CT_2b.AddSubCategory(CT_inclbISR);
  //CT_2b.AddSubCategory(CT_gamT1);
 
  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_ge1sv_notGold(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  CT_0sv_1j.AddSubCategory(CT_0bISR);
  CT_0sv_1j.AddSubCategory(CT_1bISR);
  CT_ge1sv_0j.AddSubCategory(CT_etaC);
  CT_ge1sv_0j.AddSubCategory(CT_etaF);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);

  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_ge1sv_notGold.AddSubCategory(CT_etaC);
  CT_ge1sv_notGold.AddSubCategory(CT_etaF);
  
  CategoryTree CT_gold_0j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_1j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_ge2j(VS().a("gold"), "gold", "G", kL_sub, true);
 
  CategoryTree CT_silver_01j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_silver_ge2j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze_01j(VS().a("bron"), "bronze", "B", kL_sub, true);
  CategoryTree CT_bronze_ge2j(VS().a("bron"), "bronze", "B", kL_sub, true);

  CT_gold_0j.AddSubCategory(CT_0sv_0j);
  CT_gold_0j.AddSubCategory(CT_ge1sv_0j);
  CT_gold_1j.AddSubCategory(CT_0sv_1j);
  CT_gold_1j.AddSubCategory(CT_ge1sv_1j);
  CT_gold_ge2j.AddSubCategory(CT_0bISR);
  CT_gold_ge2j.AddSubCategory(CT_1bISR);
  CT_gold_ge2j.AddSubCategory(CT_2b);
  
  CT_silver_01j.AddSubCategory(CT_0sv_notGold);
  CT_silver_01j.AddSubCategory(CT_ge1sv_notGold);
  CT_silver_ge2j.AddSubCategory(CT_inclbISR);
  
  CT_bronze_01j.AddSubCategory(CT_0sv_notGold);
  CT_bronze_01j.AddSubCategory(CT_ge1sv_notGold);
  CT_bronze_ge2j.AddSubCategory(CT_inclbISR);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "ge4J", kJ, true);
  
  CT_0j.AddSubCategory(CT_gold_0j);
  CT_0j.AddSubCategory(CT_silver_01j);
  CT_0j.AddSubCategory(CT_bronze_01j);
  CT_1j.AddSubCategory(CT_gold_1j);
  CT_1j.AddSubCategory(CT_silver_01j);
  CT_1j.AddSubCategory(CT_bronze_01j);
  CT_2j.AddSubCategory(CT_gold_ge2j);
  CT_2j.AddSubCategory(CT_silver_ge2j);
  CT_2j.AddSubCategory(CT_bronze_ge2j);
  CT_3j.AddSubCategory(CT_gold_ge2j);
  CT_3j.AddSubCategory(CT_silver_ge2j);
  CT_3j.AddSubCategory(CT_bronze_ge2j);
  CT_ge4j.AddSubCategory(CT_gold_ge2j);
  CT_ge4j.AddSubCategory(CT_silver_ge2j);
  CT_ge4j.AddSubCategory(CT_bronze_ge2j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);


  return CT_1L;
}


CategoryTree CategoryTreeTool::GetCategories_Fakes2L() const {
  CategoryTree CT_0b(VS().a("j0b"), "0b", "0b", kJ_sub, false);
  CategoryTree CT_1b(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, false);
 
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "incl. b #in ISR", "ge1j", kX_sub, false);

  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);
  
  
  CategoryTree CT_0sv_OS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_SS(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, true);

  
  CategoryTree CT_p0(VS().a("PTISR0"), "p-", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

 // CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1"), "", "k-", kX_sup, false);
  CategoryTree CT_k0_Z(VS().a("PTISR0_gamT0").a("PTISR0_gamT1"), "", "k-", kX_sup, false);
  //CategoryTree CT_k1(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  //CategoryTree CT_k0_0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k0_0(VS().a("PTISR0_gamT0"), "", "k-", kX_sup, false);
  //CategoryTree CT_k1_0(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  //CT_g0.AddSubCategory(CT_p0);
  //CT_g0.AddSubCategory(CT_p1);
  //CT_g1.AddSubCategory(CT_p0);
  //CT_g1.AddSubCategory(CT_p1);
 
  
  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_0sv_notGold.AddSubCategory(CT_p0);
  CT_0sv_OS.AddSubCategory(CT_0bISR);
  CT_0sv_OS.AddSubCategory(CT_1bISR);
  CT_0sv_SS.AddSubCategory(CT_p0);
  
//gold only
  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS_0(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_OS(VS().a("OS"),"OS","OS",kL_sup,true);

  CategoryTree CT_OSmumu(VS().a("_OSmumu"),"mumu","mumu",kL_sup,true);
  CategoryTree CT_OSelmu(VS().a("_OSelmu"),"elmu","elmu",kL_sup,true);
  CategoryTree CT_OSelel(VS().a("_OSelel"),"elel","elel",kL_sup,true);
  

//silver, bronze only
  CategoryTree CT_elel_0j(VS().a("elel"), "elel", "elel", kL_sup, true);
  CategoryTree CT_elmu_0j(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
  CategoryTree CT_mumu_0j(VS().a("mumu"),"mumu","mumu",kL_sup, true);
 
  CategoryTree CT_elel(VS().a("elel"), "elel", "elel", kL_sup, true);
  CategoryTree CT_elmu(VS().a("elmu"), "elmu", "elmu", kL_sup, true);
  CategoryTree CT_mumu(VS().a("mumu"),"mumu","mumu",kL_sup, true);
//gold, silver, and bronze
  CategoryTree CT_ll(VS().a("ll"),"ll","ll",kL_sup,true);


  
  CT_Z.AddSubCategory(CT_0b);
  CT_Z.AddSubCategory(CT_1b);
  CT_noZ.AddSubCategory(CT_0b);
  CT_noZ.AddSubCategory(CT_1b);

  CT_OSelel.AddSubCategory(CT_0sv_OS);
  CT_OSelmu.AddSubCategory(CT_0sv_OS);
  CT_OSmumu.AddSubCategory(CT_0sv_OS);
  CT_SS.AddSubCategory(CT_inclbISR);
  CT_ll.AddSubCategory(CT_1sv);
  
//s/b 0j
  CT_elel_0j.AddSubCategory(CT_0sv_notGold);
  CT_elmu_0j.AddSubCategory(CT_0sv_notGold);
  CT_mumu_0j.AddSubCategory(CT_0sv_notGold);

  CT_SS_0.AddSubCategory(CT_0sv_SS);

  CT_OS.AddSubCategory(CT_OSelel);
  CT_OS.AddSubCategory(CT_OSelmu);
  CT_OS.AddSubCategory(CT_OSmumu);
  
  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, true);

  //CT_gold.AddSubCategory(CT_0j);
  //CT_gold.AddSubCategory(CT_1j);
  //CT_gold.AddSubCategory(CT_ge2j);
 
  //CT_silver.AddSubCategory(CT_0j_notGold);
  //CT_silver.AddSubCategory(CT_1j_notGold);
  //CT_silver.AddSubCategory(CT_ge2j_notGold);

  //CT_bronze.AddSubCategory(CT_0j_notGold);
  //CT_bronze.AddSubCategory(CT_1j_notGold);
  //CT_bronze.AddSubCategory(CT_ge2j_notGold);
 
//0j
  CT_gold.AddSubCategory(CT_ll);
  CT_gold.AddSubCategory(CT_SS_0);
  CT_gold.AddSubCategory(CT_OS);
//1j/ge2j
  CT_gold.AddSubCategory(CT_Z);
  CT_gold.AddSubCategory(CT_noZ);
  CT_gold.AddSubCategory(CT_SS);
//0j
  CT_bronze.AddSubCategory(CT_ll);
  CT_bronze.AddSubCategory(CT_elel_0j);
  CT_bronze.AddSubCategory(CT_elmu_0j);
  CT_bronze.AddSubCategory(CT_mumu_0j);
//1j/ge2j
  CT_bronze.AddSubCategory(CT_elel);
  CT_bronze.AddSubCategory(CT_elmu);
  CT_bronze.AddSubCategory(CT_mumu);
//0j
  CT_silver.AddSubCategory(CT_ll);
  CT_silver.AddSubCategory(CT_elel_0j);
  CT_silver.AddSubCategory(CT_elmu_0j);
  CT_silver.AddSubCategory(CT_mumu_0j);
//1j/ge2j
  CT_silver.AddSubCategory(CT_elel);
  CT_silver.AddSubCategory(CT_elmu);
  CT_silver.AddSubCategory(CT_mumu);
 
 
  CategoryTree CT_0j(VS().a("_0j"), "1J", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "0J", "1J", kJ, true);
  CategoryTree CT_ge2j(VS().a("_ge2j"), "ge2j", "2J", kJ, true);
  //CategoryTree CT_0j_notGold(VS().a("_0j"), "", "0J", kJ, true);
  //CategoryTree CT_1j_notGold(VS().a("_1j"), "", "1J", kJ, true);
  //CategoryTree CT_ge2j_notGold(VS().a("_ge2j"), "#geq 2 j #in S", "2J", kJ, true);
  
  CT_0j.AddSubCategory(CT_gold);
  CT_0j.AddSubCategory(CT_silver);
  CT_0j.AddSubCategory(CT_bronze);
  //CT_0j.AddSubCategory(CT_SS_0);
  //CT_0j.AddSubCategory(CT_OS);
  //CT_0j_notGold.AddSubCategory(CT_ll);
  //CT_0j_notGold.AddSubCategory(CT_elel_0j);
  //CT_0j_notGold.AddSubCategory(CT_elmu_0j);
  //CT_0j_notGold.AddSubCategory(CT_mumu_0j);

  CT_1j.AddSubCategory(CT_gold);
  CT_1j.AddSubCategory(CT_silver);
  CT_1j.AddSubCategory(CT_bronze);
  //CT_1j.AddSubCategory(CT_Z);
  //CT_1j.AddSubCategory(CT_noZ);
  //CT_1j.AddSubCategory(CT_SS);
  //CT_1j_notGold.AddSubCategory(CT_elel);
  //CT_1j_notGold.AddSubCategory(CT_elmu);
  //CT_1j_notGold.AddSubCategory(CT_mumu);

  CT_ge2j.AddSubCategory(CT_gold);
  CT_ge2j.AddSubCategory(CT_silver);
  CT_ge2j.AddSubCategory(CT_bronze);
  //CT_ge2j.AddSubCategory(CT_Z);
  //CT_ge2j.AddSubCategory(CT_noZ);
  //CT_ge2j.AddSubCategory(CT_SS);
  //CT_ge2j_notGold.AddSubCategory(CT_elel);
  //CT_ge2j_notGold.AddSubCategory(CT_elmu);
  //CT_ge2j_notGold.AddSubCategory(CT_mumu);


  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, true);
  CT_2L.AddSubCategory(CT_0j);
  CT_2L.AddSubCategory(CT_1j);
  CT_2L.AddSubCategory(CT_ge2j);
  //CT_2L.AddSubCategory(CT_bronze);
  //CT_2L.AddSubCategory(CT_silver);
  //CT_2L.AddSubCategory(CT_gold);

  return CT_2L;
  
}

CategoryTree CategoryTreeTool::GetCategories_Fakes3L() const {
  

  CategoryTree CT_Z(VS().a("Zstar"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("noZ"), "noZ*", "noZ*", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);


  CategoryTree CT_gold_SS(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_silver_SS(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze_SS(VS().a("bron"), "bronze", "B", kL_sub, true);
  
  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, true);
  
  CT_gold.AddSubCategory(CT_Z);
  CT_gold.AddSubCategory(CT_noZ);
  CT_gold_SS.AddSubCategory(CT_SS);
  
  CT_silver.AddSubCategory(CT_Z);
  CT_silver.AddSubCategory(CT_noZ);
  CT_silver_SS.AddSubCategory(CT_SS);
  
  CT_bronze.AddSubCategory(CT_Z);
  CT_bronze.AddSubCategory(CT_noZ);
  CT_bronze_SS.AddSubCategory(CT_SS);

  CategoryTree CT_0j(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_ge1j(VS().a("_ge1jS"), "ge1J", "ge1J", kJ, true);
  CategoryTree CT_inclJs(VS().a("_inclS"), "inclJS", "inclJS", kJ, true);
  
  CT_0j.AddSubCategory(CT_gold);
  CT_0j.AddSubCategory(CT_silver); 
  CT_0j.AddSubCategory(CT_bronze); 

  CT_ge1j.AddSubCategory(CT_gold);
  CT_ge1j.AddSubCategory(CT_silver); 
  CT_ge1j.AddSubCategory(CT_bronze); 

  CT_inclJs.AddSubCategory(CT_gold_SS);
  CT_inclJs.AddSubCategory(CT_silver_SS);
  CT_inclJs.AddSubCategory(CT_bronze_SS);
  
  CategoryTree CT_3L(VS().a("Ch3L"), "3L", "3L", kL, true);
  CT_3L.AddSubCategory(CT_0j);
  CT_3L.AddSubCategory(CT_ge1j);
  CT_3L.AddSubCategory(CT_inclJs);

  return CT_3L;
}

CategoryTree CategoryTreeTool::GetCategories_QCD0L() const {
  CategoryTree CT_0b(VS().a("j0b"), "0b", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "2b", "2b", kJ_sub, true);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CT_0bISR.AddSubCategory(CT_0b);
  CT_0bISR.AddSubCategory(CT_1b);
  CT_1bISR.AddSubCategory(CT_0b);
  CT_1bISR.AddSubCategory(CT_1b);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);
  
  CategoryTree CT_0sv(VS().a("0svS"), "0sv", "0sv", kJ_sup, false);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, true);
  CategoryTree CT_2sv(VS().a("ge2svS"), "#geq 2 sv #in S", "2sv", kJ_sup, true);
  
  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  CT_2sv.AddSubCategory(CT_etaC);
  CT_2sv.AddSubCategory(CT_etaF);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, true);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k1(VS().a("PTISR1_gamT1"), "", "", kX_sup, false);
  CategoryTree CT_k1_5j(VS().a("PTISR1_gamT0"), "", "", kX_sup, false);

  CT_g0.AddSubCategory(CT_p0);
  CT_g0.AddSubCategory(CT_p1);
  CT_g1.AddSubCategory(CT_p0);
  CT_g1.AddSubCategory(CT_p1);

  // CT_k0.AddSubCategory(CT_0bISR);
  // CT_k0.AddSubCategory(CT_1bISR);
  // CT_k0.AddSubCategory(CT_2b);
  CT_k1.AddSubCategory(CT_0bISR);
  CT_k1.AddSubCategory(CT_1bISR);
  CT_k1.AddSubCategory(CT_2b);
  CT_k1_5j.AddSubCategory(CT_0bISR);
  CT_k1_5j.AddSubCategory(CT_1bISR);
  CT_k1_5j.AddSubCategory(CT_2b);
  
  CT_0sv.AddSubCategory(CT_0bISR);
  CT_0sv.AddSubCategory(CT_1bISR);

  // CT_0b.AddSubCategory(CT_g0);
  // CT_0b.AddSubCategory(CT_g1);
  // CT_1b.AddSubCategory(CT_g0);
  // CT_1b.AddSubCategory(CT_g1);
  // CT_ge1b.AddSubCategory(CT_g0);
  // CT_ge1b.AddSubCategory(CT_g1);
  // CT_2b.AddSubCategory(CT_g0);
  // CT_2b.AddSubCategory(CT_g1);

  // CT_0b.AddSubCategory(CT_O0);
  // CT_0b.AddSubCategory(CT_O1);
  // CT_1b.AddSubCategory(CT_O0);
  // CT_1b.AddSubCategory(CT_O1);
  // CT_ge1b.AddSubCategory(CT_O0);
  // CT_ge1b.AddSubCategory(CT_O1);
  // CT_2b.AddSubCategory(CT_O0);
  // CT_2b.AddSubCategory(CT_O1);
  
  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4 j #in S", "4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "#geq 5 j #in S", "5J", kJ, true);
  
  CT_0j.AddSubCategory(CT_1sv);
  CT_0j.AddSubCategory(CT_2sv);
  CT_1j.AddSubCategory(CT_0sv);
  CT_1j.AddSubCategory(CT_1sv);
  //CT_2j.AddSubCategory(CT_k0);
  CT_2j.AddSubCategory(CT_k1);
  //CT_3j.AddSubCategory(CT_k0);
  CT_3j.AddSubCategory(CT_k1);
  //CT_4j.AddSubCategory(CT_k0);
  CT_4j.AddSubCategory(CT_k1);
  //CT_ge5j.AddSubCategory(CT_k0);
  CT_ge5j.AddSubCategory(CT_k1_5j);
  
  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, true);
  CT_0L.AddSubCategory(CT_0j);
  CT_0L.AddSubCategory(CT_1j);
  CT_0L.AddSubCategory(CT_2j);
  CT_0L.AddSubCategory(CT_3j);
  CT_0L.AddSubCategory(CT_4j);
  CT_0L.AddSubCategory(CT_ge5j);

  return CT_0L;
}

CategoryTree CategoryTreeTool::GetCategories_QCD1L() const {
  
  CategoryTree CT_0b(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_0b_notGold(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b_notGold(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "#geq 1 j #in ISR", "ge1j", kX_sub, false);
  
  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);

 // CT_0bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_0bISR_0jsv.AddSubCategory(CT_etaC);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaC);
  
  CT_0bISR_0j.AddSubCategory(CT_p0);
  CT_0bISR_0j.AddSubCategory(CT_p1);
  CT_1bISR_0j.AddSubCategory(CT_p0);
  CT_1bISR_0j.AddSubCategory(CT_p1);

  CT_0bISR.AddSubCategory(CT_0b);
  CT_0bISR.AddSubCategory(CT_1b);
  CT_1bISR.AddSubCategory(CT_0b);
  CT_1bISR.AddSubCategory(CT_1b);
  
  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_1bISR);
  CT_0b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  CT_ge1b.AddSubCategory(CT_gamT0);
  CT_ge1b.AddSubCategory(CT_gamT1);
  CT_2b.AddSubCategory(CT_inclbISR);
  //CT_2b.AddSubCategory(CT_gamT1);
 
  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_ge1sv_notGold(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  CT_0sv_1j.AddSubCategory(CT_0bISR);
  CT_0sv_1j.AddSubCategory(CT_1bISR);
  CT_ge1sv_0j.AddSubCategory(CT_etaC);
  CT_ge1sv_0j.AddSubCategory(CT_etaF);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);

  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_ge1sv_notGold.AddSubCategory(CT_etaC);
  CT_ge1sv_notGold.AddSubCategory(CT_etaF);
  
  CategoryTree CT_gold_0j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_1j(VS().a("gold"), "gold", "G", kL_sub, true);
  CategoryTree CT_gold_ge2j(VS().a("gold"), "gold", "G", kL_sub, true);
 
  CategoryTree CT_silver_01j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_silver_ge2j(VS().a("slvr"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze_01j(VS().a("bron"), "bronze", "B", kL_sub, true);
  CategoryTree CT_bronze_ge2j(VS().a("bron"), "bronze", "B", kL_sub, true);

  CT_gold_0j.AddSubCategory(CT_0sv_0j);
  CT_gold_0j.AddSubCategory(CT_ge1sv_0j);
  CT_gold_1j.AddSubCategory(CT_0sv_1j);
  CT_gold_1j.AddSubCategory(CT_ge1sv_1j);
  CT_gold_ge2j.AddSubCategory(CT_0bISR);
  CT_gold_ge2j.AddSubCategory(CT_1bISR);
  CT_gold_ge2j.AddSubCategory(CT_2b);
  
  CT_silver_01j.AddSubCategory(CT_0sv_notGold);
  CT_silver_01j.AddSubCategory(CT_ge1sv_notGold);
  CT_silver_ge2j.AddSubCategory(CT_inclbISR);
  
  CT_bronze_01j.AddSubCategory(CT_0sv_notGold);
  CT_bronze_01j.AddSubCategory(CT_ge1sv_notGold);
  CT_bronze_ge2j.AddSubCategory(CT_inclbISR);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_4j"), "#geq 4 j #in S", "ge4J", kJ, true);
  
  CT_0j.AddSubCategory(CT_gold_0j);
  CT_0j.AddSubCategory(CT_silver_01j);
  CT_0j.AddSubCategory(CT_bronze_01j);
  CT_1j.AddSubCategory(CT_gold_1j);
  CT_1j.AddSubCategory(CT_silver_01j);
  CT_1j.AddSubCategory(CT_bronze_01j);
  CT_2j.AddSubCategory(CT_gold_ge2j);
  CT_2j.AddSubCategory(CT_silver_ge2j);
  CT_2j.AddSubCategory(CT_bronze_ge2j);
  CT_3j.AddSubCategory(CT_gold_ge2j);
  CT_3j.AddSubCategory(CT_silver_ge2j);
  CT_3j.AddSubCategory(CT_bronze_ge2j);
  CT_ge4j.AddSubCategory(CT_gold_ge2j);
  CT_ge4j.AddSubCategory(CT_silver_ge2j);
  CT_ge4j.AddSubCategory(CT_bronze_ge2j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);


  return CT_1L;
}

CategoryTree CategoryTreeTool::GetCategories_Plot1L() const {
  
  CategoryTree CT_0b(VS().a("j0b").a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("jge1bS").a("j1b"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_0b_notGold(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b_notGold(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "#geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "#geq 1 j #in ISR", "", kX_sub, true);
  
  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);

  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);

 // CT_0bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_0bISR_0jsv.AddSubCategory(CT_etaC);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaF);
 // CT_1bISR_0jsv.AddSubCategory(CT_etaC);
  
  //CT_0bISR_0j.AddSubCategory(CT_p0);
  //CT_0bISR_0j.AddSubCategory(CT_p1);
  //CT_1bISR_0j.AddSubCategory(CT_p0);
  //CT_1bISR_0j.AddSubCategory(CT_p1);

  //CT_0bISR_1j.AddSubCategory(CT_p0);
  //CT_0bISR_1j.AddSubCategory(CT_p1);
  //CT_1bISR_1j.AddSubCategory(CT_p0);
  //CT_1bISR_1j.AddSubCategory(CT_p1);

  //CT_0bISR.AddSubCategory(CT_0b);
  //CT_0bISR.AddSubCategory(CT_1b);
  //CT_1bISR.AddSubCategory(CT_0b);
  //CT_1bISR.AddSubCategory(CT_1b);
  
  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_1bISR);
  CT_0b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  //CT_ge1b.AddSubCategory(CT_gamT0);
  //CT_ge1b.AddSubCategory(CT_gamT1);
  //CT_2b.AddSubCategory(CT_inclbISR);
  //CT_2b.AddSubCategory(CT_gamT1);
 
  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, false);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, false);
  CategoryTree CT_0sv_notGold(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, false);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, false);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, false);
  CategoryTree CT_ge1sv_notGold(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, false);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  // CT_0sv_1j.AddSubCategory(CT_0bISR);
  // CT_0sv_1j.AddSubCategory(CT_1bISR);
  CT_0sv_1j.AddSubCategory(CT_0bISR_1j);
  CT_0sv_1j.AddSubCategory(CT_1bISR_1j);
  CT_ge1sv_0j.AddSubCategory(CT_etaC);
  CT_ge1sv_0j.AddSubCategory(CT_etaF);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);

  CT_0sv_notGold.AddSubCategory(CT_inclbISR);
  CT_ge1sv_notGold.AddSubCategory(CT_etaC);
  CT_ge1sv_notGold.AddSubCategory(CT_etaF);
  
  CategoryTree CT_gold_0j(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_gold_1j(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_gold_ge2j(VS().a("gold"), "gold", "G", kL_sub, false);
 
  CategoryTree CT_silver_01j(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_silver_ge2j(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze_01j(VS().a("bron"), "bronze", "B", kL_sub, false);
  CategoryTree CT_bronze_ge2j(VS().a("bron"), "bronze", "B", kL_sub, false);

  CT_gold_0j.AddSubCategory(CT_0sv_0j);
  CT_gold_0j.AddSubCategory(CT_ge1sv_0j);
  CT_gold_1j.AddSubCategory(CT_0sv_1j);
  CT_gold_1j.AddSubCategory(CT_ge1sv_1j);
  CT_gold_ge2j.AddSubCategory(CT_0b);
  CT_gold_ge2j.AddSubCategory(CT_1b);
  CT_gold_ge2j.AddSubCategory(CT_2b);
  
  CT_silver_01j.AddSubCategory(CT_0sv_notGold);
  CT_silver_01j.AddSubCategory(CT_ge1sv_notGold);
  //CT_silver_ge2j.AddSubCategory(CT_inclbISR);
  
  CT_bronze_01j.AddSubCategory(CT_0sv_notGold);
  CT_bronze_01j.AddSubCategory(CT_ge1sv_notGold);
  //CT_bronze_ge2j.AddSubCategory(CT_inclbISR);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "4J", kJ, true);
  
  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, false);
  
  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_2j);
  CT_gold.AddSubCategory(CT_3j);
  CT_gold.AddSubCategory(CT_ge4j);

  CT_0j.AddSubCategory(CT_gold_0j);
  CT_0j.AddSubCategory(CT_silver_01j);
  CT_0j.AddSubCategory(CT_bronze_01j);
  CT_1j.AddSubCategory(CT_gold_1j);
  CT_1j.AddSubCategory(CT_silver_01j);
  CT_1j.AddSubCategory(CT_bronze_01j);
  CT_2j.AddSubCategory(CT_gold_ge2j);
  CT_2j.AddSubCategory(CT_silver_ge2j);
  CT_2j.AddSubCategory(CT_bronze_ge2j);
  CT_3j.AddSubCategory(CT_gold_ge2j);
  CT_3j.AddSubCategory(CT_silver_ge2j);
  CT_3j.AddSubCategory(CT_bronze_ge2j);
  CT_ge4j.AddSubCategory(CT_gold_ge2j);
  CT_ge4j.AddSubCategory(CT_silver_ge2j);
  CT_ge4j.AddSubCategory(CT_bronze_ge2j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, false);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);

  return CT_1L;
}

CategoryTree CategoryTreeTool::GetCategories_0L_complete() const {
  
  //level 5
  //0J
  CategoryTree CT_1sv(VS().a("1svS"), "1svS", "1sv", kJ_sup, true);
  CT_1sv.AddSubCategory(getCT_ge1jISR_ptISR0_gamma0_etaInc());

  CategoryTree CT_ge2sv(VS().a("ge2svS"), "ge2svS", "ge2sv", kJ_sup, true);
  CT_ge2sv.AddSubCategory(getCT_ge1jISR_ptISR0_gamma0_etaInc());

  //1J
  CategoryTree CT_0b_0sv(VS().a("j0b0").a("0bS"), "0bS", "0b", kJ_sub, true);
  CT_0b_0sv.AddSubCategory(getCT_0sv_inclbISR_ptISRInc());

  CategoryTree CT_1b_0sv(VS().a("1bS").a("j1b"), "1bS", "1b", kJ_sub, true);
  CT_1b_0sv.AddSubCategory(getCT_0sv_inclbISR_ptISRInc());

  //2J, 3J and 4J
  CategoryTree CT_0b(VS().a("j0b0").a("0bS"), "0bS", "0b", kJ_sub, true);
  CT_0b.AddSubCategory(getCT_0bISR_ptISRInc_gammaInc());
  CT_0b.AddSubCategory(getCT_ge1bISR_ptISRInc_gammaInc());

  CategoryTree CT_1b(VS().a("1bS").a("j1b"), "1bS", "1b", kJ_sub, true);
  CT_1b.AddSubCategory(getCT_0bISR_ptISRInc_gammaInc());
  CT_1b.AddSubCategory(getCT_ge1bISR_ptISRInc_gammaInc());

  CategoryTree CT_2b(VS().a("2bS"), "2bS", "2b", kJ_sub, true);
  CT_2b.AddSubCategory(getCT_ge1jISR_ptISRInc_gammaInc());

  CategoryTree CT_ge2b(VS().a("ge2bS"), "ge2bS", "ge2b", kJ_sub, true);
  CT_ge2b.AddSubCategory(getCT_ge1jISR_ptISRInc_gammaInc());

  //ge5j
  CategoryTree CT_0bISR_pInc_gam0(VS().a("0bISR"), "0bISR", "0b", kX_sub, true);
  CT_0bISR_pInc_gam0.AddSubCategory(getCT_ptISR0_gamma0());
  CT_0bISR_pInc_gam0.AddSubCategory(getCT_ptISR1_gamma0());

  CategoryTree CT_ge1bISR_pInc_gam0(VS().a("ge1bISR"), "ge1bISR", "ge1b", kX_sub, true);
  CT_ge1bISR_pInc_gam0.AddSubCategory(getCT_ptISR0_gamma0());
  CT_ge1bISR_pInc_gam0.AddSubCategory(getCT_ptISR1_gamma0());

  CategoryTree CT_ge1jISR_pInc_gam0(VS().a("ge1jISR"), "ge1jISR", "ge1J", kX_sub, true);
  CT_ge1jISR_pInc_gam0.AddSubCategory(getCT_ptISR0_gamma0());
  CT_ge1jISR_pInc_gam0.AddSubCategory(getCT_ptISR1_gamma0());
  
  CategoryTree CT_0b_5j(VS().a("j0b0").a("0bS"), "0bS", "0b", kJ_sub, true);
  CT_0b_5j.AddSubCategory(CT_0bISR_pInc_gam0);
  CT_0b_5j.AddSubCategory(CT_ge1bISR_pInc_gam0);

  CategoryTree CT_1b_5j(VS().a("1bS").a("j1b"), "1bS", "1b", kJ_sub, true);
  CT_1b_5j.AddSubCategory(CT_0bISR_pInc_gam0);
  CT_1b_5j.AddSubCategory(CT_ge1bISR_pInc_gam0);

  CategoryTree CT_ge2b_5j(VS().a("ge2bS"), "ge2bS", "ge2b", kJ_sub, true);
  CT_ge2b_5j.AddSubCategory(CT_ge1jISR_pInc_gam0);

  //level 6
  CategoryTree CT_0j(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "1J", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2J", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3J", "3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4J", "4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "ge5J", "ge5J", kJ, true);

  CT_0j.AddSubCategory(CT_1sv);
  CT_0j.AddSubCategory(CT_ge2sv);
  CT_1j.AddSubCategory(CT_0b_0sv);
  CT_1j.AddSubCategory(CT_1b_0sv);
  CT_1j.AddSubCategory(getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc());
  CT_2j.AddSubCategory(CT_0b);
  CT_2j.AddSubCategory(CT_1b);
  CT_2j.AddSubCategory(CT_2b);
  CT_3j.AddSubCategory(CT_0b);
  CT_3j.AddSubCategory(CT_1b);
  CT_3j.AddSubCategory(CT_ge2b);
  CT_4j.AddSubCategory(CT_0b);
  CT_4j.AddSubCategory(CT_1b);
  CT_4j.AddSubCategory(CT_2b);
  CT_ge5j.AddSubCategory(CT_0b_5j);
  CT_ge5j.AddSubCategory(CT_1b_5j);
  CT_ge5j.AddSubCategory(CT_ge2b_5j);

  //level 7
  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, true);

  CT_0L.AddSubCategory(CT_0j);
  CT_0L.AddSubCategory(CT_1j);
  CT_0L.AddSubCategory(CT_2j);
  CT_0L.AddSubCategory(CT_3j);
  CT_0L.AddSubCategory(CT_4j);
  CT_0L.AddSubCategory(CT_ge5j);

  return CT_0L;
}

CategoryTree CategoryTreeTool::GetCategories_1L_complete() const {

  //charge and flavor separated categories
  CategoryTree CT_0j_0svS(VS().a("_0j"), "0J", "0J", kJ, true);
  CT_0j_0svS.AddSubCategory(getCT_0sv_inclbISR_ptISRInc());
  
  CategoryTree CT_gold_CFsep(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_CFsep.AddSubCategory(CT_0j_0svS);
  
  CategoryTree CT_elp(VS().a("elp"), "elp", "e^{+}", kL_sub, true);
  CT_elp.AddSubCategory(CT_gold_CFsep);

  CategoryTree CT_elm(VS().a("elm"), "elm", "e^{-}", kL_sub, true);
  CT_elm.AddSubCategory(CT_gold_CFsep);

  CategoryTree CT_mup(VS().a("mup"), "mup", "#mu^{+}", kL_sub, true);
  CT_mup.AddSubCategory(CT_gold_CFsep);

  CategoryTree CT_mum(VS().a("mum"), "mum", "#mu^{-}", kL_sub, true);
  CT_mum.AddSubCategory(CT_gold_CFsep);

  //flavor separated categories
  CategoryTree CT_0j_Fsep(VS().a("_0j"), "0J", "0J", kJ, true);
  CT_0j_Fsep.AddSubCategory(getCT_0sv_ge1jISR_ptISRInc());
  CT_0j_Fsep.AddSubCategory(getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc());
  
  CategoryTree CT_1j_Fsep(VS().a("_1j"), "1J", "1J", kJ, true);
  CT_1j_Fsep.AddSubCategory(getCT_0sv_ge1jISR_ptISRInc());
  CT_1j_Fsep.AddSubCategory(getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc());
  
  CategoryTree CT_2j_Fsep(VS().a("_2j"), "2J", "2J", kJ, true);
  CT_2j_Fsep.AddSubCategory(getCT_ge1jISR_ptISRInc_gammaInc());
  
  CategoryTree CT_3j_Fsep(VS().a("_3j"), "3J", "3J", kJ, true);
  CT_3j_Fsep.AddSubCategory(getCT_ge1jISR_ptISRInc_gammaInc());
  
  CategoryTree CT_ge4j_Fsep(VS().a("_ge4j"), "4J", "ge4J", kJ, true);
  CT_ge4j_Fsep.AddSubCategory(getCT_ge1jISR_ptISRInc_gammaInc());
  
  CategoryTree CT_bronze_Fsep(VS().a("bron"), "bronze", "B", kL_sub, true);
  CT_bronze_Fsep.AddSubCategory(CT_0j_Fsep);
  CT_bronze_Fsep.AddSubCategory(CT_1j_Fsep);
  CT_bronze_Fsep.AddSubCategory(CT_2j_Fsep);
  CT_bronze_Fsep.AddSubCategory(CT_3j_Fsep);
  CT_bronze_Fsep.AddSubCategory(CT_ge4j_Fsep);
  
  CategoryTree CT_silver_Fsep(VS().a("slvr"), "silver", "S", kL_sub, true);
  CT_silver_Fsep.AddSubCategory(CT_0j_Fsep);
  CT_silver_Fsep.AddSubCategory(CT_1j_Fsep);
  CT_silver_Fsep.AddSubCategory(CT_2j_Fsep);
  CT_silver_Fsep.AddSubCategory(CT_3j_Fsep);
  CT_silver_Fsep.AddSubCategory(CT_ge4j_Fsep);

  CategoryTree CT_elpm(VS().a("elpm"), "elpm", "e^{+/-}", kL_sub, true);
  CT_elpm.AddSubCategory(CT_bronze_Fsep);
  CT_elpm.AddSubCategory(CT_silver_Fsep);

  CategoryTree CT_mupm(VS().a("mupm"), "mupm", "#mu^{+/-}", kL_sub, true);
  CT_mupm.AddSubCategory(CT_bronze_Fsep);
  CT_mupm.AddSubCategory(CT_silver_Fsep);

  //charge separated categories
  CategoryTree CT_0j_Csep(VS().a("_0j"), "0J", "0J", kJ, true);
  CT_0j_Csep.AddSubCategory(getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc());

  CategoryTree CT_gold_Csep(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_Csep.AddSubCategory(CT_0j_Csep);

  CategoryTree CT_lp(VS().a("_lp_"), "lp", "l^{+}", kL_sub, true);
  CT_lp.AddSubCategory(CT_gold_Csep);

  CategoryTree CT_lm(VS().a("_lm_"), "lm", "l^{-}", kL_sub, true);
  CT_lm.AddSubCategory(CT_gold_Csep);

  //charge and flavor inclusive
  CategoryTree CT_0b_0sv(VS().a("j0b0").a("0bS"), "0bS", "0b", kJ_sub, true);
  CT_0b_0sv.AddSubCategory(getCT_0sv_inclbISR_ptISRInc_gammaInc());

  CategoryTree CT_1b_0sv(VS().a("1bS").a("j1b"), "1bS", "1b", kJ_sub, true);
  CT_1b_0sv.AddSubCategory(getCT_0sv_inclbISR_ptISRInc_gammaInc());

  CategoryTree CT_1j_lInc(VS().a("_1j"), "1J", "1J", kJ, true);
  CT_1j_lInc.AddSubCategory(CT_0b_0sv);
  CT_1j_lInc.AddSubCategory(CT_1b_0sv);
  CT_1j_lInc.AddSubCategory(getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc());

  CategoryTree CT_2j_lInc(VS().a("_2j"), "2J", "2J", kJ, true);
  CT_2j_lInc.AddSubCategory(getCT_0bS_inclbISR_ptISRInc_gammaInc());
  CT_2j_lInc.AddSubCategory(getCT_1bS_inclbISR_ptISRInc_gammaInc());
  CT_2j_lInc.AddSubCategory(getCT_2bS_ge1jISR_ptISRInc_gammaInc());
  
  CategoryTree CT_3j_lInc(VS().a("_3j"), "3J", "3J", kJ, true);
  CT_3j_lInc.AddSubCategory(getCT_0bS_inclbISR_ptISRInc_gammaInc());
  CT_3j_lInc.AddSubCategory(getCT_1bS_inclbISR_ptISRInc_gammaInc());
  CT_3j_lInc.AddSubCategory(getCT_ge2bS_ge1jISR_ptISRInc_gammaInc());
  
  CategoryTree CT_ge4j_lInc(VS().a("_ge4j"), "ge4J", "ge4J", kJ, true);
  CT_ge4j_lInc.AddSubCategory(getCT_0bS_inclbISR_ptISRInc_gammaInc());
  CT_ge4j_lInc.AddSubCategory(getCT_1bS_inclbISR_ptISRInc_gammaInc());
  CT_ge4j_lInc.AddSubCategory(getCT_ge2bS_ge1jISR_ptISRInc_gammaInc());

  CategoryTree CT_gold_lInc(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_lInc.AddSubCategory(CT_1j_lInc);
  CT_gold_lInc.AddSubCategory(CT_2j_lInc);
  CT_gold_lInc.AddSubCategory(CT_3j_lInc);
  CT_gold_lInc.AddSubCategory(CT_ge4j_lInc);

  CategoryTree CT_lpm(VS().a("_lpm_"), "lpm", "l", kL_sub, true);
  CT_lpm.AddSubCategory(CT_gold_lInc);
  
  //put it all together
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  CT_1L.AddSubCategory(CT_elp);
  CT_1L.AddSubCategory(CT_elm);
  CT_1L.AddSubCategory(CT_mup);
  CT_1L.AddSubCategory(CT_mum);
  CT_1L.AddSubCategory(CT_elpm);
  CT_1L.AddSubCategory(CT_mupm);
  CT_1L.AddSubCategory(CT_lp);
  CT_1L.AddSubCategory(CT_lm);
  CT_1L.AddSubCategory(CT_lpm);
 
  return CT_1L;
}

CategoryTree CategoryTreeTool::GetCategories_2L_complete() const {

  //OS categories
  CategoryTree CT_0j_OS(VS().a("_0j"), "0J", "0J", kJ, true);
  CT_0j_OS.AddSubCategory(getCT_0sv_inclbISR_ptISRInc_gammaInc());

  CategoryTree CT_gold_OS(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_OS.AddSubCategory(CT_0j_OS);

  CategoryTree CT_OSelel(VS().a("_OSelel"), "OSelel", "e^{+}e^{-}", kL_sup, true);
  CT_OSelel.AddSubCategory(CT_gold_OS);
  
  CategoryTree CT_OSmumu(VS().a("_OSmumu"), "OSmumu", "#mu^{+}#mu^{-}", kL_sup, true);
  CT_OSmumu.AddSubCategory(CT_gold_OS);
  
  CategoryTree CT_OSelmu(VS().a("_OSelmu"), "OSelmu", "e#mu", kL_sup, true);
  CT_OSelmu.AddSubCategory(CT_gold_OS);

  //SS categories
  CategoryTree CT_ge1jISR_ptISR0(VS().a("ge1jISR"), "ge1jISR", "ge1J", kX_sub, true);
  CT_ge1jISR_ptISR0.AddSubCategory(getCT_ptISR0());

  CategoryTree CT_0sv(VS().a("0svS"), "0svS", "0sv", kJ_sup, true);
  CT_0sv.AddSubCategory(CT_ge1jISR_ptISR0);

  CategoryTree CT_0j0sv(VS().a("_0j"), "0J", "0J", kJ, true);
  CT_0j0sv.AddSubCategory(CT_0sv);
  
  CategoryTree CT_1jS(VS().a("_1jS"), "1jS", "1J", kJ, true);
  CT_1jS.AddSubCategory(CT_ge1jISR_ptISR0);

  CategoryTree CT_ge2jS(VS().a("_ge2jS"), "ge2js", "ge2J", kJ, true);
  CT_ge2jS.AddSubCategory(CT_ge1jISR_ptISR0);

  CategoryTree CT_gold_SS(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_SS.AddSubCategory(CT_0j0sv);
  CT_gold_SS.AddSubCategory(CT_1jS);
  CT_gold_SS.AddSubCategory(CT_ge2jS);

  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CT_SS.AddSubCategory(CT_gold_SS);

  //charge inclusive
  CategoryTree CT_bronze_Cinc(VS().a("bron"), "bronze", "B", kL_sub, true);
  CT_bronze_Cinc.AddSubCategory(CT_0j0sv);
  CT_bronze_Cinc.AddSubCategory(CT_1jS);
  CT_bronze_Cinc.AddSubCategory(CT_ge2jS);

  CategoryTree CT_silver_Cinc(VS().a("slvr"), "silver", "S", kL_sub, true);
  CT_silver_Cinc.AddSubCategory(CT_0j0sv);
  CT_silver_Cinc.AddSubCategory(CT_1jS);
  CT_silver_Cinc.AddSubCategory(CT_ge2jS);

  CategoryTree CT_elel(VS().a("elel"), "elel", "ee", kL_sup, true);
  CT_elel.AddSubCategory(CT_bronze_Cinc);
  CT_elel.AddSubCategory(CT_silver_Cinc);

  CategoryTree CT_mumu(VS().a("mumu"),"mumu","#mu#mu",kL_sup, true);
  CT_mumu.AddSubCategory(CT_bronze_Cinc);
  CT_mumu.AddSubCategory(CT_silver_Cinc);
  
  CategoryTree CT_elmu(VS().a("elmu"), "elmu", "e#mu", kL_sup, true);
  CT_elmu.AddSubCategory(CT_bronze_Cinc);
  CT_elmu.AddSubCategory(CT_silver_Cinc);

  //charge/flavor inclusive
  CategoryTree CT_0j_ll(VS().a("_0j"), "0J", "0J", kJ, true);
  CT_0j_ll.AddSubCategory(getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc());

  CategoryTree CT_bronze_ll(VS().a("bron"), "bronze", "B", kL_sub, true);
  CT_bronze_ll.AddSubCategory(CT_0j_ll);

  CategoryTree CT_silver_ll(VS().a("slvr"), "silver", "S", kL_sub, true);
  CT_silver_ll.AddSubCategory(CT_0j_ll);
  
  CategoryTree CT_gold_ll(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_ll.AddSubCategory(CT_0j_ll);
  
  CategoryTree CT_ll(VS().a("ll"), "ll", "ll", kL_sup, true);
  CT_ll.AddSubCategory(CT_bronze_ll);
  CT_ll.AddSubCategory(CT_silver_ll);
  CT_ll.AddSubCategory(CT_gold_ll);
  
  //Z and no Z
  CategoryTree CT_1jS_Z(VS().a("_1j"), "1J", "1J", kJ, true);
  CT_1jS_Z.AddSubCategory(getCT_0bS_inclbISR_ptISRInc_gammaInc());
  CT_1jS_Z.AddSubCategory(getCT_1bS_inclbISR_ptISRInc_gammaInc());

  CategoryTree CT_ge2jS_Z(VS().a("_ge2j"), "ge2j", "ge2J", kJ, true);
  CT_ge2jS_Z.AddSubCategory(getCT_0bS_inclbISR_ptISRInc_gammaInc());
  CT_ge2jS_Z.AddSubCategory(getCT_1bS_inclbISR_ptISRInc_gammaInc());

  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold.AddSubCategory(CT_1jS_Z);
  CT_gold.AddSubCategory(CT_ge2jS_Z);
  
  CategoryTree CT_Z(VS().a("Zstar"), "Z", "Z*", kL_sup, true);
  CT_Z.AddSubCategory(CT_gold);
  
  CategoryTree CT_noZ(VS().a("noZ"), "noZ", "no Z*", kL_sup, true);
  CT_noZ.AddSubCategory(CT_gold);

  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, true);
  CT_2L.AddSubCategory(CT_OSelel);
  CT_2L.AddSubCategory(CT_OSmumu);
  CT_2L.AddSubCategory(CT_OSelmu);
  CT_2L.AddSubCategory(CT_SS);
  CT_2L.AddSubCategory(CT_elel);
  CT_2L.AddSubCategory(CT_mumu);
  CT_2L.AddSubCategory(CT_elmu);
  CT_2L.AddSubCategory(CT_ll);
  CT_2L.AddSubCategory(CT_Z);
  CT_2L.AddSubCategory(CT_noZ);

  return CT_2L;
}

CategoryTree CategoryTreeTool::GetCategories_3L_complete() const{
  //same sign
  CategoryTree CT_inclJs(VS().a("_inclS"), "inclS", "inclS", kJ, true);
  CT_inclJs.AddSubCategory(getCT_ge1jISR_ptISR0());

  CategoryTree CT_bronze_SS(VS().a("bron"), "bronze", "B", kL_sub, true);
  CT_bronze_SS.AddSubCategory(CT_inclJs);

  CategoryTree CT_silver_SS(VS().a("slvr"), "silver", "S", kL_sub, true);
  CT_silver_SS.AddSubCategory(CT_inclJs);

  CategoryTree CT_gold_SS(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_SS.AddSubCategory(CT_inclJs);

  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CT_SS.AddSubCategory(CT_bronze_SS);
  CT_SS.AddSubCategory(CT_silver_SS);
  CT_SS.AddSubCategory(CT_gold_SS);

  //Z and noZ
  CategoryTree CT_0jS(VS().a("_0jS"), "0J", "0J", kJ, true);
  CT_0jS.AddSubCategory(getCT_ge1jISR_ptISR0());

  CategoryTree CT_ge1jS(VS().a("_ge1jS"), "ge1jS", "ge1J", kJ, true);
  CT_ge1jS.AddSubCategory(getCT_ge1jISR_ptISR0());

  CategoryTree CT_bronze_Z(VS().a("bron"), "bronze", "B", kL_sub, true);
  CT_bronze_Z.AddSubCategory(CT_0jS);
  CT_bronze_Z.AddSubCategory(CT_ge1jS);

  CategoryTree CT_silver_Z(VS().a("slvr"), "silver", "S", kL_sub, true);
  CT_silver_Z.AddSubCategory(CT_0jS);
  CT_silver_Z.AddSubCategory(CT_ge1jS);

  CategoryTree CT_gold_Z(VS().a("gold"), "gold", "G", kL_sub, true);
  CT_gold_Z.AddSubCategory(CT_0jS);
  CT_gold_Z.AddSubCategory(CT_ge1jS);

  CategoryTree CT_Z(VS().a("Zstar"), "Z", "Z*", kL_sup, true);
  CT_Z.AddSubCategory(CT_bronze_Z);
  CT_Z.AddSubCategory(CT_silver_Z);
  CT_Z.AddSubCategory(CT_gold_Z);

  CategoryTree CT_noZ(VS().a("noZ"), "noZ", "no Z*", kL_sup, true);
  CT_noZ.AddSubCategory(CT_bronze_Z);
  CT_noZ.AddSubCategory(CT_silver_Z);
  CT_noZ.AddSubCategory(CT_gold_Z);

  CategoryTree CT_3L(VS().a("Ch3L"), "3L", "3L", kL, true);
  CT_3L.AddSubCategory(CT_SS);
  CT_3L.AddSubCategory(CT_Z);
  CT_3L.AddSubCategory(CT_noZ);

  return CT_3L;
}
  
CategoryTree CategoryTreeTool::GetCategories_All() const {
  CategoryTree CT_0L = GetCategories_0L_complete();
  CategoryTree CT_1L = GetCategories_1L_complete();
  CategoryTree CT_2L = GetCategories_2L_complete();
  CategoryTree CT_3L = GetCategories_3L_complete();

  CategoryTree CT_all(VS().a("Ch"), "", "", kNorm, false);

  CT_all.AddSubCategory(CT_0L);
  CT_all.AddSubCategory(CT_1L);
  CT_all.AddSubCategory(CT_2L);
  CT_all.AddSubCategory(CT_3L);
  
  return CT_all;
}

CategoryTree CategoryTreeTool::GetCategories_2L_flavorSepAndSS() const {

  CategoryTree CT_0j_OS(VS().a("_0j"), "0J", "0J", kJ, true);
  CategoryTree CT_OSelel(VS().a("_OSelel"), "OSelel", "e^{+}e^{-}", kL_sup, true);
  CategoryTree CT_OSmumu(VS().a("_OSmumu"), "OSmumu", "#mu^{+}#mu^{-}", kL_sup, true);
  CategoryTree CT_OSelmu(VS().a("_OSelmu"), "OSelmu", "e^{+/-}#mu^{-/+}", kL_sup, true);
  CategoryTree CT_SS(VS().a("SS"), "SS", "SS", kL_sup, true);
  CategoryTree CT_gold_OS(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, false);

  CT_0j_OS.AddSubCategory(CT_OSelel);
  CT_0j_OS.AddSubCategory(CT_OSmumu);
  CT_0j_OS.AddSubCategory(CT_OSelmu);
  CT_0j_OS.AddSubCategory(CT_SS);
  CT_gold_OS.AddSubCategory(CT_0j_OS);
  CT_2L.AddSubCategory(CT_gold_OS);

  return CT_2L;
}

CategoryTree CategoryTreeTool::GetCategories_SVonly() const{
  //0L
  //0J
  CategoryTree CT_0L_0J(VS().a("_0j"), "", "0L 0J X_{#geq 2sv}", kJ, true);
  CategoryTree CT_0L(VS().a("Ch0L"), "", "", kL, false);
  CategoryTree CT_ge2sv(VS().a("ge2svS"), "", "", kJ_sup, false);

  CT_0L.AddSubCategory(CT_ge2sv);
  CT_0L_0J.AddSubCategory(CT_0L);

  //1J
  CategoryTree CT_0L_1J(VS().a("_1j"), "", "0L 1J X_{#geq 1sv}", kJ, true);
  CategoryTree CT_0L2(VS().a("Ch0L"), "", "", kL, false);
  CategoryTree CT_ge1sv(VS().a("ge1svS"), "", "", kJ_sup, false);
  CT_0L2.AddSubCategory(CT_ge1sv);
  CT_0L_1J.AddSubCategory(CT_0L2);
  
  //1L Gold
  //0J
  CategoryTree CT_1L_0J(VS().a("_0j"), "", "1L_{G} 0J X_{#geq 1sv}", kJ, true);
  CategoryTree CT_1L(VS().a("Ch1L"), "", "", kL, false);
  CategoryTree CT_gold(VS().a("gold"), "", "", kL_sub, false);
  CT_gold.AddSubCategory(CT_1L);
  CT_1L_0J.AddSubCategory(CT_gold);

  //1J 
  CategoryTree CT_1L_1J(VS().a("_1j"), "", "1L_{G} 1J X_{#geq 1sv}", kJ, true);
  CT_1L_1J.AddSubCategory(CT_gold);

  //2L Gold
  CategoryTree CT_2L_0J(VS().a("_0j"), "", "2L_{G} 0J X_{#geq 1sv}", kJ, true);
  CT_2L_0J.AddSubCategory(CT_gold);

  CategoryTree CT_SVonly(VS().a("Ch"), "SVonly", "", kNorm, true);
  CT_SVonly.AddSubCategory(CT_0L_0J);
  CT_SVonly.AddSubCategory(CT_0L_1J);
  CT_SVonly.AddSubCategory(CT_1L_0J);
  CT_SVonly.AddSubCategory(CT_1L_1J);
  CT_SVonly.AddSubCategory(CT_2L_0J);

  return CT_SVonly;
}

CategoryTree CategoryTreeTool::GetCategories_SVConly() const{

  //0L
  //0J
  CategoryTree CT_0L_0J(VS().a("_0j"), "", "0L 0J X_{#geq 2svc}", kJ, true);
  CategoryTree CT_0L(VS().a("Ch0L"), "", "", kL, false);
  CategoryTree CT_ge2sv(VS().a("ge2svS"), "", "", kJ_sup, false);
  CategoryTree CT_svEta0(VS().a("SVeta0"), "", "", kX_sup, false);

  CT_ge2sv.AddSubCategory(CT_svEta0);
  CT_0L.AddSubCategory(CT_ge2sv);
  CT_0L_0J.AddSubCategory(CT_0L);

  //1J
  CategoryTree CT_0L_1J(VS().a("_1j"), "", "0L 1J X_{#geq 1svc}", kJ, true);
  CategoryTree CT_0L2(VS().a("Ch0L"), "", "", kL, false);
  CategoryTree CT_ge1sv(VS().a("ge1svS"), "", "", kJ_sup, false);
  CT_ge1sv.AddSubCategory(CT_svEta0);
  CT_0L2.AddSubCategory(CT_ge1sv);
  CT_0L_1J.AddSubCategory(CT_0L2);

  //1L Gold
  //0J
  CategoryTree CT_1L_0J(VS().a("_0j"), "", "1L_{G} 0J X_{#geq 1svc}", kJ, true);
  CategoryTree CT_1L(VS().a("Ch1L"), "", "", kL, false);
  CategoryTree CT_gold(VS().a("gold"), "", "", kL_sub, false);
  CT_1L.AddSubCategory(CT_ge1sv);
  CT_gold.AddSubCategory(CT_1L);
  CT_1L_0J.AddSubCategory(CT_gold);

  //1J
  CategoryTree CT_1L_1J(VS().a("_1j"), "", "1L_{G} 1J X_{#geq 1svc}", kJ, true);
  CT_1L_1J.AddSubCategory(CT_gold);

  //2L Gold
  CategoryTree CT_2L_0J(VS().a("_0j"), "", "2L_{G} 0J X_{#geq 1svc}", kJ, true);
  CT_2L_0J.AddSubCategory(CT_gold);

  CategoryTree CT_SVonly(VS().a("Ch"), "SVonly", "", kNorm, true);
  CT_SVonly.AddSubCategory(CT_0L_0J);
  CT_SVonly.AddSubCategory(CT_0L_1J);
  CT_SVonly.AddSubCategory(CT_1L_0J);
  CT_SVonly.AddSubCategory(CT_1L_1J);
  CT_SVonly.AddSubCategory(CT_2L_0J);

  return CT_SVonly;
}

CategoryTree CategoryTreeTool::GetCategories_StopMultiJet() const{
  //0L
  //3J
  CategoryTree CT_0L_3J(VS().a("_3j"), "", "0L 3J", kJ, true);
  CategoryTree CT_0L(VS().a("Ch0L"), "", "", kL, false);
  CT_0L_3J.AddSubCategory(CT_0L);
  //4J
  CategoryTree CT_0L_4J(VS().a("_4j"), "", "0L 4J", kJ, true);
  CT_0L_4J.AddSubCategory(CT_0L);

  //5J
  CategoryTree CT_0L_ge5J(VS().a("_ge5j"), "", "0L #geq 5J", kJ, true);
  CT_0L_ge5J.AddSubCategory(CT_0L);
  
  //1L Gold
  //2J
  CategoryTree CT_1L_2J(VS().a("_2j"), "", "1L_{G} 2J", kJ, true);
  CategoryTree CT_1L(VS().a("Ch1L"), "", "", kL, false);
  CategoryTree CT_gold(VS().a("gold"), "", "", kL_sub, false);
  CT_gold.AddSubCategory(CT_1L);
  CT_1L_2J.AddSubCategory(CT_gold);

  //3J
  CategoryTree CT_1L_3J(VS().a("_3j"), "", "1L_{G} 3J", kJ, true);
  CT_1L_3J.AddSubCategory(CT_gold);

  //ge4J
  CategoryTree CT_1L_ge4J(VS().a("_ge4j"), "", "1L_{G} #geq 4J", kJ, true);
  CT_1L_ge4J.AddSubCategory(CT_gold);

  CategoryTree CT_StopMultiJet(VS().a("Ch"), "", "", kNorm, true);
  CT_StopMultiJet.AddSubCategory(CT_0L_3J);
  CT_StopMultiJet.AddSubCategory(CT_0L_4J);
  CT_StopMultiJet.AddSubCategory(CT_0L_ge5J);
  CT_StopMultiJet.AddSubCategory(CT_1L_2J);
  CT_StopMultiJet.AddSubCategory(CT_1L_3J);
  CT_StopMultiJet.AddSubCategory(CT_1L_ge4J);
  return CT_StopMultiJet;
}

CategoryTree CategoryTreeTool::GetCategories_0LMultiJetBsplit() const{
  CategoryTree CT_0bISR(VS().a("0bISR"), "0bISR", "0b", kX_sub);
  CategoryTree CT_ge1bISR(VS().a("ge1bISR"), "ge1bISR", "ge1b", kX_sub);
  CategoryTree CT_0b_5j(VS().a("j0b0").a("0bS"), "0bS", "0b", kJ_sub, false);
  CT_0b_5j.AddSubCategory(CT_0bISR);
  CT_0b_5j.AddSubCategory(CT_ge1bISR);
  CategoryTree CT_1b_5j(VS().a("1bS").a("j1b"), "1bS", "1b", kJ_sub, false);
  CT_1b_5j.AddSubCategory(CT_0bISR);
  CT_1b_5j.AddSubCategory(CT_ge1bISR);
  CategoryTree CT_ge2b_5j(VS().a("ge2bS"), "ge2bS", "ge2b", kJ_sub, true);
  CategoryTree CT_0L(VS().a("Ch0L"), "", "0L", kL, true);
  CategoryTree CT_0L_ge5J(VS().a("_ge5j"), "", "#geq 5J", kJ, true);
  CT_0L_ge5J.AddSubCategory(CT_0b_5j);
  CT_0L_ge5J.AddSubCategory(CT_1b_5j);
  CT_0L_ge5J.AddSubCategory(CT_ge2b_5j);
  CT_0L.AddSubCategory(CT_0L_ge5J);

  return CT_0L;
}

//CategoryTreeTool private
void CategoryTreeTool::AssembleCTmodules(){

  _CT_gamma0_etaInc = new CategoryTree(VS().a("gamT0"), "gamT0", "#gamma-", kX_sup);
  _CT_etaC          = new CategoryTree(VS().a("SVeta0"), "svc", "svc", kX_sup);
  _CT_etaF          = new CategoryTree(VS().a("SVeta1"), "svf", "svf", kX_sup);

  _CT_gamma0_etaInc->AddSubCategory(*_CT_etaC);
  _CT_gamma0_etaInc->AddSubCategory(*_CT_etaF);

  _CT_gamma0 = new CategoryTree(VS().a("gamT0"), "gamT0", "#gamma-", kX_sup);
  _CT_gamma1 = new CategoryTree(VS().a("gamT1"), "gamT1", "#gamma+", kX_sup);

  _CT_ptISR0_gamma0_etaInc = new CategoryTree(VS().a("PTISR0"), "PTISR0", "p-", kX_sup);
  _CT_ptISR0_gamma0_etaInc->AddSubCategory(*_CT_gamma0_etaInc);

  _CT_ptISR0 = new CategoryTree(VS().a("PTISR0"), "PTISR0", "p-", kX_sup);
  _CT_ptISR1 = new CategoryTree(VS().a("PTISR1"), "PTISR1", "p+", kX_sup);

  _CT_ptISR0_gamma0   = new CategoryTree(VS().a("PTISR0"), "PTISR0", "p-", kX_sup);
  _CT_ptISR1_gamma0   = new CategoryTree(VS().a("PTISR1"), "PTISR1", "p+", kX_sup);
  _CT_ptISR0_gammaInc = new CategoryTree(VS().a("PTISR0"), "PTISR0", "p-", kX_sup);
  _CT_ptISR1_gammaInc = new CategoryTree(VS().a("PTISR1"), "PTISR0", "p+", kX_sup);

  _CT_ptISR0_gamma0->AddSubCategory(*_CT_gamma0);
  _CT_ptISR1_gamma0->AddSubCategory(*_CT_gamma0);
  _CT_ptISR0_gammaInc->AddSubCategory(*_CT_gamma0);
  _CT_ptISR0_gammaInc->AddSubCategory(*_CT_gamma1);
  _CT_ptISR1_gammaInc->AddSubCategory(*_CT_gamma0);
  _CT_ptISR1_gammaInc->AddSubCategory(*_CT_gamma1);

  _CT_0bISR_ptISRInc = new CategoryTree(VS().a("0bISR"), "0bISR", "0b", kX_sub);
  _CT_0bISR_ptISRInc->AddSubCategory(*_CT_ptISR0);
  _CT_0bISR_ptISRInc->AddSubCategory(*_CT_ptISR1);

  _CT_0bISR_ptISRInc_gammaInc = new CategoryTree(VS().a("0bISR"), "0bISR", "0b", kX_sub);
  _CT_0bISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ptISR0_gammaInc);
  _CT_0bISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ptISR1_gammaInc);

  _CT_ge1bISR_ptISRInc = new CategoryTree(VS().a("ge1bISR"), "ge1bISR", "ge1b", kX_sub);
  _CT_ge1bISR_ptISRInc->AddSubCategory(*_CT_ptISR0);
  _CT_ge1bISR_ptISRInc->AddSubCategory(*_CT_ptISR1);

  _CT_ge1jISR_ptISR0 = new CategoryTree(VS().a("ge1jISR"), "ge1jISR", "ge1J", kX_sub);
  _CT_ge1jISR_ptISR0->AddSubCategory(*_CT_ptISR0);
  
  _CT_ge1jISR_ptISRInc = new CategoryTree(VS().a("ge1jISR"), "ge1jISR", "1J", kX_sub);
  _CT_ge1jISR_ptISRInc->AddSubCategory(*_CT_ptISR0);
  _CT_ge1jISR_ptISRInc->AddSubCategory(*_CT_ptISR1);

  _CT_ge1bISR_ptISRInc_gammaInc = new CategoryTree(VS().a("ge1bISR"), "ge1bISR", "ge1b", kX_sub);
  _CT_ge1bISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ptISR0_gammaInc);
  _CT_ge1bISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ptISR1_gammaInc);
  
  _CT_ge1jISR_ptISRInc_gammaInc = new CategoryTree(VS().a("ge1jISR"), "ge1jISR", "ge1J", kX_sub);
  _CT_ge1jISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ptISR0_gammaInc);
  _CT_ge1jISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ptISR1_gammaInc);

  _CT_ge1jISR_ptISR0_gamma0_etaInc = new CategoryTree(VS().a("ge1jISR"), "ge1jISR", "ge1J", kX_sub);
  _CT_ge1jISR_ptISR0_gamma0_etaInc->AddSubCategory(*_CT_ptISR0_gamma0_etaInc);
  
  _CT_0sv_ge1jISR_ptISRInc = new CategoryTree(VS().a("0svS"), "0svS", "0sv", kJ_sup);
  _CT_0sv_ge1jISR_ptISRInc->AddSubCategory(*_CT_0bISR_ptISRInc);
  _CT_0sv_ge1jISR_ptISRInc->AddSubCategory(*_CT_ge1bISR_ptISRInc);
  
  _CT_0sv_inclbISR_ptISRInc = new CategoryTree(VS().a("0svS"), "0svS", "0sv", kJ_sup);
  _CT_0sv_inclbISR_ptISRInc->AddSubCategory(*_CT_0bISR_ptISRInc);
  _CT_0sv_inclbISR_ptISRInc->AddSubCategory(*_CT_ge1bISR_ptISRInc);
  
  _CT_0sv_ge1jISR_ptISRInc = new CategoryTree(VS().a("0svS"), "0sv", "0sv", kJ_sup);
  _CT_0sv_ge1jISR_ptISRInc->AddSubCategory(*_CT_ge1jISR_ptISRInc);

  _CT_0sv_inclbISR_ptISRInc_gammaInc = new CategoryTree(VS().a("0svS"), "0svS", "0sv", kJ_sup);
  _CT_0sv_inclbISR_ptISRInc_gammaInc->AddSubCategory(*_CT_0bISR_ptISRInc_gammaInc);
  _CT_0sv_inclbISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ge1bISR_ptISRInc_gammaInc);
  
  _CT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc = new CategoryTree(VS().a("ge1svS"), "ge1SV", "ge1sv", kJ_sup);
  _CT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc->AddSubCategory(*_CT_ge1jISR_ptISR0_gamma0_etaInc);

  _CT_0bS_inclbISR_ptISRInc_gammaInc = new CategoryTree(VS().a("j0b0").a("0bS"), "0bS", "0b", kJ_sub);
  _CT_0bS_inclbISR_ptISRInc_gammaInc->AddSubCategory(*_CT_0bISR_ptISRInc_gammaInc);
  _CT_0bS_inclbISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ge1bISR_ptISRInc_gammaInc);
  
  _CT_1bS_inclbISR_ptISRInc_gammaInc = new CategoryTree(VS().a("1bS").a("j1b"), "1bS", "1b", kJ_sub);
  _CT_1bS_inclbISR_ptISRInc_gammaInc->AddSubCategory(*_CT_0bISR_ptISRInc_gammaInc);
  _CT_1bS_inclbISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ge1bISR_ptISRInc_gammaInc);
  
  _CT_2bS_ge1jISR_ptISRInc_gammaInc = new CategoryTree(VS().a("2bS"), "2bS", "2b", kJ_sub);
  _CT_2bS_ge1jISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ge1jISR_ptISRInc_gammaInc);
  
  _CT_ge2bS_ge1jISR_ptISRInc_gammaInc = new CategoryTree(VS().a("ge2bS"), "ge2bS", "ge2b", kJ_sub);
  _CT_ge2bS_ge1jISR_ptISRInc_gammaInc->AddSubCategory(*_CT_ge1jISR_ptISRInc_gammaInc);
}

const CategoryTree& CategoryTreeTool::getCT_gamma0_etaInc() const{
  return *_CT_gamma0_etaInc;
}

const CategoryTree& CategoryTreeTool::getCT_etaC() const{
  return *_CT_etaC;
}

const CategoryTree& CategoryTreeTool::getCT_etaF() const{
  return *_CT_etaF;
}

const CategoryTree& CategoryTreeTool::getCT_gamma0() const{
  return *_CT_gamma0;
}

const CategoryTree& CategoryTreeTool::getCT_gamma1() const{
  return *_CT_gamma1;
}

const CategoryTree& CategoryTreeTool::getCT_ptISR0_gamma0_etaInc() const{
  return *_CT_ptISR0_gamma0_etaInc;
}

const CategoryTree& CategoryTreeTool::getCT_ptISR0() const{
  return *_CT_ptISR0;
}

const CategoryTree& CategoryTreeTool::getCT_ptISR1() const{
  return *_CT_ptISR1;
}

const CategoryTree& CategoryTreeTool::getCT_ptISR0_gamma0() const{
  return *_CT_ptISR0_gamma0;
}

const CategoryTree& CategoryTreeTool::getCT_ptISR1_gamma0() const{
  return *_CT_ptISR1_gamma0;
}

const CategoryTree& CategoryTreeTool::getCT_ptISR0_gammaInc() const{
  return *_CT_ptISR0_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_ptISR1_gammaInc() const{
  return *_CT_ptISR1_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_0bISR_ptISRInc() const{
  return *_CT_0bISR_ptISRInc;
}

const CategoryTree& CategoryTreeTool::getCT_0bISR_ptISRInc_gammaInc() const{
  return *_CT_0bISR_ptISRInc_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_ge1bISR_ptISRInc() const{
  return *_CT_ge1bISR_ptISRInc;
}

const CategoryTree& CategoryTreeTool::getCT_ge1bISR_ptISRInc_gammaInc() const{
  return *_CT_ge1bISR_ptISRInc_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_ge1jISR_ptISR0() const{
  return *_CT_ge1jISR_ptISR0;
}

const CategoryTree& CategoryTreeTool::getCT_ge1jISR_ptISRInc() const{
  return *_CT_ge1jISR_ptISRInc;
}

const CategoryTree& CategoryTreeTool::getCT_ge1jISR_ptISRInc_gammaInc() const{
  return *_CT_ge1jISR_ptISRInc_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_ge1jISR_ptISR0_gamma0_etaInc() const{
  return *_CT_ge1jISR_ptISR0_gamma0_etaInc;
}

const CategoryTree& CategoryTreeTool::getCT_0sv_inclbISR_ptISRInc() const{
  return *_CT_0sv_inclbISR_ptISRInc;
}

const CategoryTree& CategoryTreeTool::getCT_0sv_ge1jISR_ptISRInc() const{
  return *_CT_0sv_ge1jISR_ptISRInc;
}

const CategoryTree& CategoryTreeTool::getCT_0sv_inclbISR_ptISRInc_gammaInc()const{
  return *_CT_0sv_inclbISR_ptISRInc_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc() const{
  return *_CT_ge1sv_ge1jISR_ptISR0_gamma0_etaInc;
}

const CategoryTree& CategoryTreeTool::getCT_0bS_inclbISR_ptISRInc_gammaInc() const{
  return *_CT_0bS_inclbISR_ptISRInc_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_1bS_inclbISR_ptISRInc_gammaInc() const{
  return *_CT_1bS_inclbISR_ptISRInc_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_2bS_ge1jISR_ptISRInc_gammaInc() const{
  return *_CT_2bS_ge1jISR_ptISRInc_gammaInc;
}

const CategoryTree& CategoryTreeTool::getCT_ge2bS_ge1jISR_ptISRInc_gammaInc() const{
  return *_CT_ge2bS_ge1jISR_ptISRInc_gammaInc;
}
