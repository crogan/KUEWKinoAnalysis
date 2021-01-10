#include <iostream>
#include <map>

#include "CategoryTree.hh"

SpectroscopicLabel::SpectroscopicLabel(){
  m_L = "?L";
  m_J = "?J";
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
    if(m_Lsup != "")
      m_Lsup += " ";
    m_Lsup += label;
  }
  if(type == kL_sub){
    if(m_Lsub != "")
      m_Lsub += " ";
    m_Lsub += label;
  }
  if(type == kJ_sup){
    if(m_Jsup != "")
      m_Jsup += " ";
    m_Jsup += label;
  }
  if(type == kJ_sub){
    if(m_Jsub != "")
      m_Jsub += " ";
    m_Jsub += label;
  }
   if(type == kX_sup){
    if(m_Xsup != "")
      m_Xsup += " ";
    m_Xsup += label;
  }
  if(type == kX_sub){
    if(m_Xsub != "")
      m_Xsub += " ";
    m_Xsub += label;
  }
}

string SpectroscopicLabel::GetSpectroscopicLabel() const {
  string label = m_Norm + " " + m_L;
  if(m_Lsup != "")
    label += "^{" + m_Lsup + "}";
  if(m_Lsub != "")
    label += "_{" + m_Lsub + "}";
  label += " " + m_J;
  if(m_Jsup != "")
    label += "^{" + m_Jsup + "}";
  if(m_Jsub != "")
    label += "_{" + m_Jsub + "}";
  label += " X";
  if(m_Xsup != "")
    label += "^{" + m_Xsup + "}";
  if(m_Xsub != "")
    label += "_{" + m_Xsub + "}";

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

void CategoryTree::AddSpectroscopicLabel(SpectroscopicLabel& slabel) const {
  if(m_ParCat != nullptr)
    m_ParCat->AddSpectroscopicLabel(slabel);

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
  if(m_DoDraw)
    cats.push_back(this);

  int N = m_SubCats.size();
  for(int i = 0; i < N; i++)
    m_SubCats[i]->GetListVisible(cats);
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
  
}

CategoryTreeTool::~CategoryTreeTool(){
  
}

CategoryTree CategoryTreeTool::GetCategories() const {

  // CategoryTree CT_0L = GetCategories_0L();
  CategoryTree CT_1L = GetCategories_1L();
  // CategoryTree CT_2L = GetCategories_2L();
  // CategoryTree CT_3L = GetCategories_3L();

  CategoryTree CT_all(VS().a("Ch"), "", "", kNorm, false);
  //CT_all.AddSubCategory(CT_0L);
  CT_all.AddSubCategory(CT_1L);
  //CT_all.AddSubCategory(CT_2L);
  //CT_all.AddSubCategory(CT_3L);

  return CT_all;
}

CategoryTree CategoryTreeTool::GetCategories_0L() const {
  
}

CategoryTree CategoryTreeTool::GetCategories_1L() const {
  CategoryTree CT_0b(VS().a("0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);
  
  CategoryTree CT_0sv(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  
  CT_0sv.AddSubCategory(CT_0bISR);
  CT_0sv.AddSubCategory(CT_1bISR);
  CT_ge1sv.AddSubCategory(CT_etaC);
  CT_ge1sv.AddSubCategory(CT_etaF);
  
  CategoryTree CT_0j(VS().a("-0j"), "", "0J", kJ, false);
  CategoryTree CT_1j(VS().a("-1j"), "", "1J", kJ, false);
  CategoryTree CT_2j(VS().a("-2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("-3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("-ge4j"), "#geq 4 j #in S", "4J", kJ, true);
  
  CT_0j.AddSubCategory(CT_0sv);
  CT_0j.AddSubCategory(CT_ge1sv);
  CT_1j.AddSubCategory(CT_0sv);
  CT_1j.AddSubCategory(CT_ge1sv);
  CT_2j.AddSubCategory(CT_0b);
  CT_2j.AddSubCategory(CT_1b);
  CT_2j.AddSubCategory(CT_2b);
  CT_3j.AddSubCategory(CT_0b);
  CT_3j.AddSubCategory(CT_1b);
  CT_3j.AddSubCategory(CT_2b);
  CT_ge4j.AddSubCategory(CT_0b);
  CT_ge4j.AddSubCategory(CT_ge1b);
  
  CategoryTree CT_gold(VS().a("elG").a("muG"), "gold", "G", kL_sub, true);
  CategoryTree CT_silver(VS().a("elS").a("muS"), "silver", "S", kL_sub, true);
  CategoryTree CT_bronze(VS().a("elB").a("muB"), "bronze", "B", kL_sub, true);
  
  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_2j);
  CT_gold.AddSubCategory(CT_3j);
  CT_gold.AddSubCategory(CT_ge4j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  CT_1L.AddSubCategory(CT_bronze);
  CT_1L.AddSubCategory(CT_silver);
  CT_1L.AddSubCategory(CT_gold);

  return CT_1L;
}

CategoryTree CategoryTreeTool::GetCategories_2L() const {
  
}

CategoryTree CategoryTreeTool::GetCategories_3L() const {
  
}
