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
  
}

CategoryTreeTool::~CategoryTreeTool(){
  
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





CategoryTree CategoryTreeTool::GetCategories_0L() const {
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
  
  CategoryTree CT_0L(VS().a("Ch0L"), "0L", "0L", kL, false);
  CT_0L.AddSubCategory(CT_0j);
  CT_0L.AddSubCategory(CT_1j);
  CT_0L.AddSubCategory(CT_2j);
  CT_0L.AddSubCategory(CT_3j);
  CT_0L.AddSubCategory(CT_4j);
  CT_0L.AddSubCategory(CT_ge5j);

  return CT_0L;
}

CategoryTree CategoryTreeTool::GetCategories_1L() const {
  CategoryTree CT_0b(VS().a("j0b"), "0b", "0b", kJ_sub, false);
  CategoryTree CT_1b(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, false);
  CategoryTree CT_2b(VS().a("2bS"), "2b", "2b", kJ_sub, false);
  
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  CategoryTree CT_inclbISR(VS().a("ge1jISR"), "", "", kX_sub, false);

  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  CT_2b.AddSubCategory(CT_inclbISR);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, false);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, false);
  
  CategoryTree CT_0sv_0(VS().a("0svS"), "0sv", "0sv", kJ_sup, false);
  CategoryTree CT_0sv_1(VS().a("0svS"), "0sv", "0sv", kJ_sup, false);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, false);
  
  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, false);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, false);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k1(VS().a("PTISR1_gamT1"), "", "", kX_sup, false);

  CategoryTree CT_k0_0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k1_0(VS().a("PTISR1_gamT1"), "", "", kX_sup, false);

  CategoryTree CT_k0_1(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k1_1(VS().a("PTISR1_gamT1"), "", "", kX_sup, false);

  CT_g0.AddSubCategory(CT_p0);
  CT_g0.AddSubCategory(CT_p1);
  CT_g1.AddSubCategory(CT_p0);
  CT_g1.AddSubCategory(CT_p1);

  // CT_k0.AddSubCategory(CT_0bISR);
  // CT_k0.AddSubCategory(CT_1bISR);
  // CT_k0.AddSubCategory(CT_2b);
  CT_k1_1.AddSubCategory(CT_0b);
  CT_k1_1.AddSubCategory(CT_1b);
  CT_k1.AddSubCategory(CT_0b);
  CT_k1.AddSubCategory(CT_1b);
  CT_k1.AddSubCategory(CT_2b);

  CT_0sv_0.AddSubCategory(CT_0bISR);
  CT_0sv_0.AddSubCategory(CT_1bISR);
  //CT_0sv_1.AddSubCategory(CT_k0_1);
  CT_0sv_1.AddSubCategory(CT_k1_1);
  
  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "4J", kJ, true);
  
  CT_0j.AddSubCategory(CT_0sv_0);
  CT_0j.AddSubCategory(CT_1sv);
  CT_1j.AddSubCategory(CT_0sv_1);
  CT_1j.AddSubCategory(CT_1sv);
  //CT_2j.AddSubCategory(CT_k0);
  CT_2j.AddSubCategory(CT_k1);
  //CT_3j.AddSubCategory(CT_k0);
  CT_3j.AddSubCategory(CT_k1);
  //CT_ge4j.AddSubCategory(CT_k0);
  CT_ge4j.AddSubCategory(CT_k1);
  
  CategoryTree CT_gold(VS().a("gold"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("slvr"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("bron"), "bronze", "B", kL_sub, false);
  
  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_2j);
  CT_gold.AddSubCategory(CT_3j);
  CT_gold.AddSubCategory(CT_ge4j);
  
  CT_silver.AddSubCategory(CT_0j);
  CT_silver.AddSubCategory(CT_1j);
  CT_silver.AddSubCategory(CT_2j);
  CT_silver.AddSubCategory(CT_3j);
  CT_silver.AddSubCategory(CT_ge4j);
  
  CT_bronze.AddSubCategory(CT_0j);
  CT_bronze.AddSubCategory(CT_1j);
  CT_bronze.AddSubCategory(CT_2j);
  CT_bronze.AddSubCategory(CT_3j);
  CT_bronze.AddSubCategory(CT_ge4j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  //CT_1L.AddSubCategory(CT_bronze);
  //CT_1L.AddSubCategory(CT_silver);
  CT_1L.AddSubCategory(CT_gold);

  //CT_1L.AddSubCategory(CT_0j);
  //CT_1L.AddSubCategory(CT_1j);
  // // CT_1L.AddSubCategory(CT_2j);
  // // CT_1L.AddSubCategory(CT_3j);
  // // CT_1L.AddSubCategory(CT_ge4j);

  return CT_1L;
}


CategoryTree CategoryTreeTool::GetCategories_2L() const {
  CategoryTree CT_0b(VS().a("j0b"), "0b", "0b", kJ_sub, false);
  CategoryTree CT_1b(VS().a("1bS").a("j1b"), "1b", "1b", kJ_sub, false);
 
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);

  CT_0b.AddSubCategory(CT_0bISR);
  CT_0b.AddSubCategory(CT_1bISR);
  CT_1b.AddSubCategory(CT_0bISR);
  CT_1b.AddSubCategory(CT_1bISR);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);
  
  CategoryTree CT_0sv(VS().a("0svS"), "0sv", "0sv", kJ_sup, false);
  CategoryTree CT_1sv(VS().a("1svS"), "1sv", "1sv", kJ_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, true);

  CategoryTree CT_g0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_g1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k1(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  CategoryTree CT_k0_0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  CategoryTree CT_k1_0(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  CT_g0.AddSubCategory(CT_p0);
  CT_g0.AddSubCategory(CT_p1);
  CT_g1.AddSubCategory(CT_p0);
  CT_g1.AddSubCategory(CT_p1);

  // CT_k0.AddSubCategory(CT_0bISR);
  // CT_k0.AddSubCategory(CT_1bISR);
  CT_k1_0.AddSubCategory(CT_0bISR);
  CT_k1_0.AddSubCategory(CT_1bISR);
  CT_k1.AddSubCategory(CT_0b);
  CT_k1.AddSubCategory(CT_1b);
  
  

  CategoryTree CT_Z(VS().a("OSelel").a("OSmumu").a("OSSHelel").a("OSSHmumu"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ(VS().a("OSelmu").a("OSOHelel").a("OSOHmumu"), "OSnoZ*", "OS", kL_sup, true);
  CategoryTree CT_SS(VS().a("SSelel").a("SSmumu"), "SS", "SS", kL_sup, true);

  CategoryTree CT_Z_0(VS().a("OSelel").a("OSmumu").a("OSSHelel").a("OSSHmumu"), "Z*", "Z*", kL_sup, true);
  CategoryTree CT_noZ_0(VS().a("OSelmu").a("OSOHelel").a("OSOHmumu"), "OSnoZ*", "OS", kL_sup, true);
  CategoryTree CT_SS_0(VS().a("SSelel").a("SSmumu"), "SS", "SS", kL_sup, true);

  CT_1sv.AddSubCategory(CT_etaC);
  CT_1sv.AddSubCategory(CT_etaF);
  
  //CT_Z_0.AddSubCategory(CT_k0_0);
  CT_Z_0.AddSubCategory(CT_k1_0);
  //CT_noZ_0.AddSubCategory(CT_k0_0);
  CT_noZ_0.AddSubCategory(CT_k1_0);

  //CT_Z.AddSubCategory(CT_k0);
  CT_Z.AddSubCategory(CT_k1);
  //CT_noZ.AddSubCategory(CT_k0);
  CT_noZ.AddSubCategory(CT_k1);

  CT_0sv.AddSubCategory(CT_Z_0);
  CT_0sv.AddSubCategory(CT_noZ_0);
  CT_0sv.AddSubCategory(CT_SS_0);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "#geq 2 j #in S", "2J", kJ, true);
  CategoryTree CT_ge3j(VS().a("_ge3j"), "#geq 2 j #in S", "3J", kJ, true);
  
  CT_0j.AddSubCategory(CT_0sv);
  CT_0j.AddSubCategory(CT_1sv);

  CT_1j.AddSubCategory(CT_Z);
  CT_1j.AddSubCategory(CT_noZ);
  CT_1j.AddSubCategory(CT_SS);

  CT_2j.AddSubCategory(CT_Z);
  CT_2j.AddSubCategory(CT_noZ);
  CT_2j.AddSubCategory(CT_SS);

  CT_ge3j.AddSubCategory(CT_Z);
  CT_ge3j.AddSubCategory(CT_noZ);
  CT_ge3j.AddSubCategory(CT_SS);
  
  CategoryTree CT_gold(VS().a("elGelG").a("elGmuG").a("muGmuG"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("elS").a("muS"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("elB").a("muB"), "bronze", "B", kL_sub, false);

  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_1j);
  CT_gold.AddSubCategory(CT_2j);
  CT_gold.AddSubCategory(CT_ge3j);
 
  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, false);
  CT_2L.AddSubCategory(CT_bronze);
  CT_2L.AddSubCategory(CT_silver);
  CT_2L.AddSubCategory(CT_gold);

  return CT_2L;
  
}



CategoryTree CategoryTreeTool::GetCategories_3L() const {
  CategoryTree CT_0b(VS().a("0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "geq 1 b #in S", "1b", kJ_sub, true);
 
  CategoryTree CT_0bISR(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, true);
  CategoryTree CT_1bISR(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, true);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_gam0(VS().a("gamT0"), "", "#gamma-", kX_sup, false);
  CategoryTree CT_gam1(VS().a("gamT1"), "", "#gamma+", kX_sup, false);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, true);

  CT_gam0.AddSubCategory(CT_p0);
  CT_gam0.AddSubCategory(CT_p1);
  CT_gam1.AddSubCategory(CT_p0);
  CT_gam1.AddSubCategory(CT_p1);

  CategoryTree CT_OSSFelelel(VS().a("OSSFelel^el"), "OSSF#it{ee e} ", "Z* #it{eee}", kL_sup, true);
  CategoryTree CT_OSSFelelmu(VS().a("OSSFelel^mu"), "OSSF#it{ee} #mu", "Z* #it{ee}#mu", kL_sup, true);
  CategoryTree CT_OSSFmumuel(VS().a("OSSFmumu^el"), "OSSF#mu#mu #it{e}", "Z* #mu#mu#it{e}", kL_sup, true);
  CategoryTree CT_OSSFmumumu(VS().a("OSSFmumu^mu"), "OSSF#mu#mu #mu", "Z* #mu#mu#mu", kL_sup, true);

  CategoryTree CT_nOSSFelelel(VS().a("nOSSFelel^el"), "nOSSF #it{eee}", "noZ* #it{eee}", kL_sup, true);
  CategoryTree CT_nOSSFelelmu(VS().a("nOSSFelel^mu"), "nOSSF #it{ee}#mu", "noZ* #it{ee}#mu", kL_sup, true);
  CategoryTree CT_nOSSFmumuel(VS().a("nOSSFmumu^el"), "nOSSF #it{e}#mu#mu", "noZ* #it{e}#mu#mu", kL_sup, true);
  CategoryTree CT_nOSSFmumumu(VS().a("nOSSFmumu^mu"), "nOSSF #mu#mu#mu", "noZ* #mu#mu#mu", kL_sup, true);

  CategoryTree CT_elelel(VS().a("_elelel"), "#it{eee}", "#it{eee}", kL_sup, true);
  CategoryTree CT_elelmu(VS().a("_elelmu"), "#it{ee}#mu", "#it{ee}#mu", kL_sup, true);
  CategoryTree CT_mumuel(VS().a("_mumuel"), "#it{e}#mu#mu", "#it{e}#mu#mu", kL_sup, true);
  CategoryTree CT_mumumu(VS().a("_mumumu"), "#mu#mu#mu", "#mu#mu#mu", kL_sup, true);

  CT_elelel.AddSubCategory(CT_0b);
  CT_elelel.AddSubCategory(CT_ge1b);
  CT_elelmu.AddSubCategory(CT_0b);
  CT_elelmu.AddSubCategory(CT_ge1b);
  CT_mumuel.AddSubCategory(CT_0b);
  CT_mumuel.AddSubCategory(CT_ge1b);
  CT_mumumu.AddSubCategory(CT_0b);
  CT_mumumu.AddSubCategory(CT_ge1b);

  // CT_OSSFelelel.AddSubCategory(CT_gam0);
  // CT_OSSFelelel.AddSubCategory(CT_gam1);
  // CT_OSSFelelmu.AddSubCategory(CT_gam0);
  // CT_OSSFelelmu.AddSubCategory(CT_gam1);
  // CT_OSSFmumuel.AddSubCategory(CT_gam0);
  // CT_OSSFmumuel.AddSubCategory(CT_gam1);
  // CT_OSSFmumumu.AddSubCategory(CT_gam0);
  // CT_OSSFmumumu.AddSubCategory(CT_gam1);

  // CT_OSSFelelel.AddSubCategory(CT_p0);
  // CT_OSSFelelel.AddSubCategory(CT_p1);
  // CT_OSSFelelmu.AddSubCategory(CT_p0);
  // CT_OSSFelelmu.AddSubCategory(CT_p1);
  // CT_OSSFmumuel.AddSubCategory(CT_p0);
  // CT_OSSFmumuel.AddSubCategory(CT_p1);
  // CT_OSSFmumumu.AddSubCategory(CT_p0);
  // CT_OSSFmumumu.AddSubCategory(CT_p1);

  CategoryTree CT_0j(VS().a("_0j"), "", "0J", kJ, true);
  CategoryTree CT_ge1j(VS().a("_ge1jS"), "", "1J", kJ, true);
  
  
  CT_0j.AddSubCategory(CT_OSSFelelel);
  CT_0j.AddSubCategory(CT_OSSFelelmu);
  CT_0j.AddSubCategory(CT_OSSFmumuel);
  CT_0j.AddSubCategory(CT_OSSFmumumu);
  CT_0j.AddSubCategory(CT_nOSSFelelel);
  CT_0j.AddSubCategory(CT_nOSSFelelmu);
  CT_0j.AddSubCategory(CT_nOSSFmumuel);
  CT_0j.AddSubCategory(CT_nOSSFmumumu);

  CT_ge1j.AddSubCategory(CT_OSSFelelel);
  CT_ge1j.AddSubCategory(CT_OSSFelelmu);
  CT_ge1j.AddSubCategory(CT_OSSFmumuel);
  CT_ge1j.AddSubCategory(CT_OSSFmumumu);
  CT_ge1j.AddSubCategory(CT_nOSSFelelel);
  CT_ge1j.AddSubCategory(CT_nOSSFelelmu);
  CT_ge1j.AddSubCategory(CT_nOSSFmumuel);
  CT_ge1j.AddSubCategory(CT_nOSSFmumumu);
  

  CategoryTree CT_gold(VS().a("elGelGelG").a("elGelGmuG").a("elGmuGmuG").a("muGmuGmuG"), "gold", "G", kL_sub, false);
  CategoryTree CT_silver(VS().a("elS").a("muS"), "silver", "S", kL_sub, false);
  CategoryTree CT_bronze(VS().a("elB").a("muB"), "bronze", "B", kL_sub, false);

  CT_gold.AddSubCategory(CT_0j);
  CT_gold.AddSubCategory(CT_ge1j);
  
  CategoryTree CT_3L(VS().a("Ch3L"), "3L", "3L", kL, false);
  //CT_3L.AddSubCategory(CT_bronze);
  //CT_3L.AddSubCategory(CT_silver);
  CT_3L.AddSubCategory(CT_gold);

  return CT_3L;
}


CategoryTree CategoryTreeTool::GetCategories_Fakes1L() const {
  
  CategoryTree CT_0b(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, true);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);

  CT_0bISR_0jsv.AddSubCategory(CT_etaF);
  CT_0bISR_0jsv.AddSubCategory(CT_etaC);
  CT_1bISR_0jsv.AddSubCategory(CT_etaF);
  CT_1bISR_0jsv.AddSubCategory(CT_etaC);
  
  CT_0bISR_0j.AddSubCategory(CT_p0);
  CT_0bISR_0j.AddSubCategory(CT_p1);
  CT_1bISR_0j.AddSubCategory(CT_p0);
  CT_1bISR_0j.AddSubCategory(CT_p1);

  CT_0bISR_1j.AddSubCategory(CT_0b);
  CT_0bISR_1j.AddSubCategory(CT_1b);
  CT_1bISR_1j.AddSubCategory(CT_0b);
  CT_1bISR_1j.AddSubCategory(CT_1b);
  
  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_gamT0);
  CT_0b.AddSubCategory(CT_gamT1);
  CT_1b.AddSubCategory(CT_gamT0);
  CT_1b.AddSubCategory(CT_gamT1);
  CT_ge1b.AddSubCategory(CT_gamT0);
  CT_ge1b.AddSubCategory(CT_gamT1);
  CT_2b.AddSubCategory(CT_gamT0);
  CT_2b.AddSubCategory(CT_gamT1);
 
  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  CT_0sv_1j.AddSubCategory(CT_0bISR_1j);
  CT_0sv_1j.AddSubCategory(CT_1bISR_1j);
  CT_ge1sv_0j.AddSubCategory(CT_0bISR_0jsv);
  CT_ge1sv_0j.AddSubCategory(CT_1bISR_0jsv);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);
  
  CategoryTree CT_gold_0j(VS().a("gold"), "gold mu", "1L0jmuG", kL_sub, true);
  CategoryTree CT_gold_1j(VS().a("gold"), "gold mu", "1L1jmuG", kL_sub, true);
  CategoryTree CT_gold_2j(VS().a("gold"), "gold mu", "1L2jmuG", kL_sub, true);
  CategoryTree CT_gold_3j(VS().a("gold"), "gold mu", "1L3jmuG", kL_sub, true);
  CategoryTree CT_gold_ge4j(VS().a("gold"), "gold mu", "1Lge4jmuG", kL_sub, true);
  CategoryTree CT_silver_0j(VS().a("slvr"), "silver mu", "1L0jmuS", kL_sub, true);
  CategoryTree CT_silver_1j(VS().a("slvr"), "silver mu", "1L1jmuS", kL_sub, true);
  CategoryTree CT_silver_2j(VS().a("slvr"), "silver mu", "1L2jmuS", kL_sub, true);
  CategoryTree CT_silver_3j(VS().a("slvr"), "silver mu", "1L3jmuS", kL_sub, true);
  CategoryTree CT_silver_ge4j(VS().a("slvr"), "silver mu", "1Lge4jmuS", kL_sub, true);
  CategoryTree CT_bronze_0j(VS().a("bron"), "bronze mu", "1L0jmuB", kL_sub, true);
  CategoryTree CT_bronze_1j(VS().a("bron"), "bronze mu", "1L1jmuB", kL_sub, true);
  CategoryTree CT_bronze_2j(VS().a("bron"), "bronze mu", "1L2jmuB", kL_sub, true);
  CategoryTree CT_bronze_3j(VS().a("bron"), "bronze mu", "1L3jmuB", kL_sub, true);
  CategoryTree CT_bronze_ge4j(VS().a("bron"), "bronze mu", "1Lge4jmuB", kL_sub, true);

  CT_gold_0j.AddSubCategory(CT_0sv_0j);
  CT_gold_0j.AddSubCategory(CT_ge1sv_0j);
  CT_gold_1j.AddSubCategory(CT_0sv_1j);
  CT_gold_1j.AddSubCategory(CT_ge1sv_1j);
  CT_gold_2j.AddSubCategory(CT_0b);
  CT_gold_2j.AddSubCategory(CT_1b);
  CT_gold_2j.AddSubCategory(CT_2b);
  CT_gold_3j.AddSubCategory(CT_0b);
  CT_gold_3j.AddSubCategory(CT_1b);
  CT_gold_3j.AddSubCategory(CT_2b);
  CT_gold_ge4j.AddSubCategory(CT_0b);
  CT_gold_ge4j.AddSubCategory(CT_ge1b);
  
  CT_silver_0j.AddSubCategory(CT_0sv_0j);
  CT_silver_0j.AddSubCategory(CT_ge1sv_0j);
  CT_silver_1j.AddSubCategory(CT_0sv_1j);
  CT_silver_1j.AddSubCategory(CT_ge1sv_1j);
  CT_silver_2j.AddSubCategory(CT_0b);
  CT_silver_2j.AddSubCategory(CT_1b);
  CT_silver_2j.AddSubCategory(CT_2b);
  CT_silver_3j.AddSubCategory(CT_0b);
  CT_silver_3j.AddSubCategory(CT_1b);
  CT_silver_3j.AddSubCategory(CT_2b);
  CT_silver_ge4j.AddSubCategory(CT_0b);
  CT_silver_ge4j.AddSubCategory(CT_ge1b);

  CT_bronze_0j.AddSubCategory(CT_0sv_0j);
  CT_bronze_0j.AddSubCategory(CT_ge1sv_0j);
  CT_bronze_1j.AddSubCategory(CT_0sv_1j);
  CT_bronze_1j.AddSubCategory(CT_ge1sv_1j);
  CT_bronze_2j.AddSubCategory(CT_0b);
  CT_bronze_2j.AddSubCategory(CT_1b);
  CT_bronze_2j.AddSubCategory(CT_2b);
  CT_bronze_3j.AddSubCategory(CT_0b);
  CT_bronze_3j.AddSubCategory(CT_1b);
  CT_bronze_3j.AddSubCategory(CT_2b);
  CT_bronze_ge4j.AddSubCategory(CT_0b);
  CT_bronze_ge4j.AddSubCategory(CT_ge1b);

  CategoryTree CT_0j(VS().a("_0j"), "", "1Lmu0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1Lmu1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "1Lmu2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "1Lmu3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "1Lmu4J", kJ, true);
  
  CT_0j.AddSubCategory(CT_gold_0j);
  CT_0j.AddSubCategory(CT_silver_0j);
  CT_0j.AddSubCategory(CT_bronze_0j);
  CT_1j.AddSubCategory(CT_gold_1j);
  CT_1j.AddSubCategory(CT_silver_1j);
  CT_1j.AddSubCategory(CT_bronze_1j);
  CT_2j.AddSubCategory(CT_gold_2j);
  CT_2j.AddSubCategory(CT_silver_2j);
  CT_2j.AddSubCategory(CT_bronze_2j);
  CT_3j.AddSubCategory(CT_gold_3j);
  CT_3j.AddSubCategory(CT_silver_3j);
  CT_3j.AddSubCategory(CT_bronze_3j);
  CT_ge4j.AddSubCategory(CT_gold_ge4j);
  CT_ge4j.AddSubCategory(CT_silver_ge4j);
  CT_ge4j.AddSubCategory(CT_bronze_ge4j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);


  return CT_1L;
}


CategoryTree CategoryTreeTool::GetCategories_Fakes2L() const {
  CategoryTree CT_0b(VS().a("j0b"), "0b", "0b #in S", kJ_sub, true);
  CategoryTree CT_1b(VS().a("1bS"), "1b", "1b #in S", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "ge1b", "ge1b #in S", kJ_sub, true);

  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, true);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);


  CT_0bISR_0jsv.AddSubCategory(CT_etaF);
  CT_0bISR_0jsv.AddSubCategory(CT_etaC);
  CT_1bISR_0jsv.AddSubCategory(CT_etaF);
  CT_1bISR_0jsv.AddSubCategory(CT_etaC);

  CT_0bISR_0j.AddSubCategory(CT_p0);
  CT_0bISR_0j.AddSubCategory(CT_p1);
  CT_1bISR_0j.AddSubCategory(CT_p0);
  CT_1bISR_0j.AddSubCategory(CT_p1);

  CT_0bISR_1j.AddSubCategory(CT_0b);
  CT_0bISR_1j.AddSubCategory(CT_1b);
  CT_1bISR_1j.AddSubCategory(CT_0b);
  CT_1bISR_1j.AddSubCategory(CT_1b);

  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_gamT0);
  CT_0b.AddSubCategory(CT_gamT1);
  CT_1b.AddSubCategory(CT_gamT0);
  CT_1b.AddSubCategory(CT_gamT1);
  CT_ge1b.AddSubCategory(CT_gamT0);
  CT_ge1b.AddSubCategory(CT_gamT1);

  
  CategoryTree CT_0sv_0j(VS().a("0svS"), "0sv", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "ge1sv", "ge1sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0sv", "0sv", kJ_sup, false);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "ge1sv", "ge1sv", kJ_sup, true);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  CT_0sv_1j.AddSubCategory(CT_0bISR_1j);
  CT_0sv_1j.AddSubCategory(CT_1bISR_1j);
  CT_ge1sv_0j.AddSubCategory(CT_0bISR_0jsv);
  CT_ge1sv_0j.AddSubCategory(CT_1bISR_0jsv);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);



  // CategoryTree CT_k0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  // CategoryTree CT_k1(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  // CategoryTree CT_k0_0(VS().a("PTISR0_gamT0").a("PTISR0_gamT1").a("PTISR1_gamT0"), "", "k-", kX_sup, false);
  // CategoryTree CT_k1_0(VS().a("PTISR1_gamT1"), "", "k+", kX_sup, false);

  // CT_g0.AddSubCategory(CT_p0);
  // CT_g0.AddSubCategory(CT_p1);
  // CT_g1.AddSubCategory(CT_p0);
  // CT_g1.AddSubCategory(CT_p1);

  // CT_k0.AddSubCategory(CT_0bISR);
  // CT_k0.AddSubCategory(CT_1bISR);
  // CT_k1_0.AddSubCategory(CT_0bISR);
  // CT_k1_0.AddSubCategory(CT_1bISR);
  // CT_k1.AddSubCategory(CT_0b);
  // CT_k1.AddSubCategory(CT_1b);

  //depth 4 - splits by bS multiplicity
  CategoryTree CT_gold_0j_el(VS().a("gold"), "gold ee", "2L0jelG", kL_sub, true);
  CategoryTree CT_silver_0j_el(VS().a("slvr"), "silver ee", "2L0jelS", kL_sub, true);
  CategoryTree CT_bronze_0j_el(VS().a("bron"), "bronze ee", "2L0jelB", kL_sub, true);

  CategoryTree CT_gold_1j_el(VS().a("gold"), "gold ee", "2L1jelG", kL_sub, true);
  CategoryTree CT_silver_1j_el(VS().a("slvr"), "silver ee", "2L1jelS", kL_sub, true);
  CategoryTree CT_bronze_1j_el(VS().a("bron"), "bronze ee", "2L1jelB", kL_sub, true);

  CategoryTree CT_gold_2j_el(VS().a("gold"), "gold ee", "2L2jelG", kL_sub, true);
  CategoryTree CT_silver_2j_el(VS().a("slvr"), "silver ee", "2L2jelS", kL_sub, true);
  CategoryTree CT_bronze_2j_el(VS().a("bron"), "bronze ee", "2L2jelB", kL_sub, true);

  CategoryTree CT_gold_ge3j_el(VS().a("gold"), "gold ee", "2Lge3jelG", kL_sub, true);
  CategoryTree CT_silver_ge3j_el(VS().a("slvr"), "silver ee", "2Lge3jelS", kL_sub, true);
  CategoryTree CT_bronze_ge3j_el(VS().a("bron"), "bronze ee", "2Lge3jelB", kL_sub, true);
  
  
  // CategoryTree CT_Z(VS().a("OSelel").a("OSmumu").a("OSSHelel").a("OSSHmumu"), "Z*", "Z*", kL_sup, true);
  // CategoryTree CT_noZ(VS().a("OSelmu").a("OSOHelel").a("OSOHmumu"), "OSnoZ*", "OS", kL_sup, true);
  // CategoryTree CT_SS(VS().a("SSelel").a("SSmumu"), "SS", "SS", kL_sup, true);

  // CategoryTree CT_Z_0(VS().a("OSelel").a("OSmumu").a("OSSHelel").a("OSSHmumu"), "Z*", "Z*", kL_sup, true);
  // CategoryTree CT_noZ_0(VS().a("OSelmu").a("OSOHelel").a("OSOHmumu"), "OSnoZ*", "OS", kL_sup, true);
  // CategoryTree CT_SS_0(VS().a("SSelel").a("SSmumu"), "SS", "SS", kL_sup, true);

  

  CT_gold_0j_el.AddSubCategory(CT_0sv_0j);
  CT_gold_0j_el.AddSubCategory(CT_ge1sv_0j);
  CT_gold_1j_el.AddSubCategory(CT_0b);
  CT_gold_1j_el.AddSubCategory(CT_1b);
  CT_gold_2j_el.AddSubCategory(CT_0b);
  CT_gold_2j_el.AddSubCategory(CT_ge1b);
  CT_gold_ge3j_el.AddSubCategory(CT_0b);
  CT_gold_ge3j_el.AddSubCategory(CT_ge1b);

  CT_silver_0j_el.AddSubCategory(CT_0sv_0j);
  CT_silver_0j_el.AddSubCategory(CT_ge1sv_0j);
  CT_silver_1j_el.AddSubCategory(CT_0b);
  CT_silver_1j_el.AddSubCategory(CT_1b);
  CT_silver_2j_el.AddSubCategory(CT_0b);
  CT_silver_2j_el.AddSubCategory(CT_ge1b);
  CT_silver_ge3j_el.AddSubCategory(CT_0b);
  CT_silver_ge3j_el.AddSubCategory(CT_ge1b);

  CT_bronze_0j_el.AddSubCategory(CT_0sv_0j);
  CT_bronze_0j_el.AddSubCategory(CT_ge1sv_0j);
  CT_bronze_1j_el.AddSubCategory(CT_0b);
  CT_bronze_1j_el.AddSubCategory(CT_1b);
  CT_bronze_2j_el.AddSubCategory(CT_0b);
  CT_bronze_2j_el.AddSubCategory(CT_ge1b);
  CT_bronze_ge3j_el.AddSubCategory(CT_0b);
  CT_bronze_ge3j_el.AddSubCategory(CT_ge1b);

  CategoryTree CT_0j_el(VS().a("_0j"), "", "2Lelel0J", kJ, true);
  CategoryTree CT_1j_el(VS().a("_1j"), "", "2Lelel1J", kJ, true);
  CategoryTree CT_2j_el(VS().a("_2j"), "2 j #in S", "2Lelel2J", kJ, true);
  CategoryTree CT_ge3j_el(VS().a("_ge3j"), "#geq 3 j #in S", "3Lelel2J", kJ, true);
  
  CT_0j_el.AddSubCategory(CT_gold_0j_el);
  CT_0j_el.AddSubCategory(CT_silver_0j_el);
  CT_0j_el.AddSubCategory(CT_bronze_0j_el);

  CT_1j_el.AddSubCategory(CT_gold_1j_el);
  CT_1j_el.AddSubCategory(CT_silver_1j_el);
  CT_1j_el.AddSubCategory(CT_bronze_1j_el);

  CT_2j_el.AddSubCategory(CT_gold_2j_el);
  CT_2j_el.AddSubCategory(CT_silver_2j_el);
  CT_2j_el.AddSubCategory(CT_bronze_2j_el);

  CT_ge3j_el.AddSubCategory(CT_gold_ge3j_el);
  CT_ge3j_el.AddSubCategory(CT_silver_ge3j_el);
  CT_ge3j_el.AddSubCategory(CT_bronze_ge3j_el);
  
  CategoryTree CT_2L(VS().a("Ch2L"), "2L", "2L", kL, true);

  CT_2L.AddSubCategory(CT_0j_el);
  CT_2L.AddSubCategory(CT_1j_el);
  CT_2L.AddSubCategory(CT_2j_el);
  CT_2L.AddSubCategory(CT_ge3j_el);
  
  return CT_2L;
  
}

CategoryTree CategoryTreeTool::GetCategories_Fakes3L() const {
  CategoryTree CT_gold_el_0j(VS().a("gold"), "3LelG0j", "3LelG0j", kL_sup, true);
  CategoryTree CT_silver_el_0j(VS().a("slvr"), "3LelS0j", "3LelS0j", kL_sup, true);
  CategoryTree CT_bronze_el_0j(VS().a("bron"), "3LelB0j", "3LelB0j", kL_sup, true);
  
  CategoryTree CT_gold_el_1j(VS().a("gold"), "3LelG1j", "3LelG1j", kL_sup, true);
  CategoryTree CT_silver_el_1j(VS().a("slvr"), "3LelS1j", "3LelS1j", kL_sup, true);
  CategoryTree CT_bronze_el_1j(VS().a("bron"), "3LelB1j", "3LelB1j", kL_sup, true);

  CategoryTree CT_0j_el(VS().a("_0jS"),"","3Lel0j",kL,true); 
  CategoryTree CT_1j_el(VS().a("_ge1jS"),"","3Lelge1j",kL,true); 

  CT_0j_el.AddSubCategory(CT_gold_el_0j);
  CT_0j_el.AddSubCategory(CT_silver_el_0j);
  CT_0j_el.AddSubCategory(CT_bronze_el_0j);

  CT_1j_el.AddSubCategory(CT_gold_el_1j);
  CT_1j_el.AddSubCategory(CT_silver_el_1j);
  CT_1j_el.AddSubCategory(CT_bronze_el_1j);
  
  CategoryTree CT_3L(VS().a("Ch3L"), "3L", "3L", kL, true);

  CT_3L.AddSubCategory(CT_0j_el);
  CT_3L.AddSubCategory(CT_1j_el);
  

  return CT_3L;
}

CategoryTree CategoryTreeTool::GetCategories_QCD0L() const {
  CategoryTree CT_0b(VS().a("j0bS"), "0 b #in S", "0b", kJ_sub, true);
  CategoryTree CT_1b(VS().a("j1bS"), "1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_ge1b(VS().a("ge1bS"), "geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, true);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);


  CT_0bISR_0jsv.AddSubCategory(CT_etaF);
  CT_0bISR_0jsv.AddSubCategory(CT_etaC);
  CT_1bISR_0jsv.AddSubCategory(CT_etaF);
  CT_1bISR_0jsv.AddSubCategory(CT_etaC);
  
  CT_0bISR_0j.AddSubCategory(CT_p0);
  CT_0bISR_0j.AddSubCategory(CT_p1);
  CT_1bISR_0j.AddSubCategory(CT_p0);
  CT_1bISR_0j.AddSubCategory(CT_p1);

  CT_0bISR_1j.AddSubCategory(CT_0b);
  CT_0bISR_1j.AddSubCategory(CT_1b);
  CT_1bISR_1j.AddSubCategory(CT_0b);
  CT_1bISR_1j.AddSubCategory(CT_1b);



 // CT_p0.AddSubCategory(CT_gamT0);
 // CT_p1.AddSubCategory(CT_gamT0);
 // CT_p0.AddSubCategory(CT_gamT1);
 // CT_p1.AddSubCategory(CT_gamT1);
  
  
  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_gamT0);
  CT_0b.AddSubCategory(CT_gamT1);
  CT_1b.AddSubCategory(CT_gamT0);
  CT_1b.AddSubCategory(CT_gamT1);
  CT_ge1b.AddSubCategory(CT_gamT0);
  CT_ge1b.AddSubCategory(CT_gamT1);
  CT_2b.AddSubCategory(CT_gamT0);
  CT_2b.AddSubCategory(CT_gamT1);
 

  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);

  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  CT_0sv_1j.AddSubCategory(CT_0bISR_1j);
  CT_0sv_1j.AddSubCategory(CT_1bISR_1j);
  CT_ge1sv_0j.AddSubCategory(CT_0bISR_0jsv);
  CT_ge1sv_0j.AddSubCategory(CT_1bISR_0jsv);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);


//depth = 4 - splits by bS multiplicity

  
  CategoryTree CT_0j(VS().a("_0j"), "", "0L0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "0L1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "0L2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "0L3J", kJ, true);
  CategoryTree CT_4j(VS().a("_4j"), "4 j #in S", "0L4J", kJ, true);
  CategoryTree CT_ge5j(VS().a("_ge5j"), "#geq 5 j #in S", "0L5J", kJ, true);

  
  CT_0j.AddSubCategory(CT_0sv_0j);
  CT_0j.AddSubCategory(CT_ge1sv_0j);
  CT_1j.AddSubCategory(CT_0sv_1j);
  CT_1j.AddSubCategory(CT_ge1sv_1j);
  CT_2j.AddSubCategory(CT_0b);
  CT_2j.AddSubCategory(CT_1b);
  CT_2j.AddSubCategory(CT_2b);
  CT_3j.AddSubCategory(CT_0b);
  CT_3j.AddSubCategory(CT_1b);
  CT_3j.AddSubCategory(CT_2b);
  CT_4j.AddSubCategory(CT_0b);
  CT_4j.AddSubCategory(CT_1b);
  CT_4j.AddSubCategory(CT_2b);
  CT_ge5j.AddSubCategory(CT_0b);
  CT_ge5j.AddSubCategory(CT_1b);
  CT_ge5j.AddSubCategory(CT_2b);
 
  
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
  CategoryTree CT_ge1b(VS().a("ge1bS"), "geq 1 b #in S", "1b", kJ_sub, true);
  CategoryTree CT_2b(VS().a("2bS"), "#geq 2 b #in S", "2b", kJ_sub, true);
 
  CategoryTree CT_0bISR_1j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_1j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);

  CategoryTree CT_0bISR_0j(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0j(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_0bISR_0jsv(VS().a("0bISR"), "0 b #in ISR", "0b", kX_sub, false);
  CategoryTree CT_1bISR_0jsv(VS().a("ge1bISR"), "#geq 1 b #in ISR", "1b", kX_sub, false);
  
  CategoryTree CT_etaC(VS().a("SVeta0"), "|#eta^{SV}| #leq 1.5", "svc", kX_sup, true);
  CategoryTree CT_etaF(VS().a("SVeta1"), "|#eta^{SV}| > 1.5", "svf", kX_sup, true);

  CategoryTree CT_p0(VS().a("PTISR0"), "", "p-", kX_sup, true);
  CategoryTree CT_p1(VS().a("PTISR1"), "", "p+", kX_sup, true);
  
  CategoryTree CT_gamT0(VS().a("gamT0"), "", "#gamma -", kX_sup, false);
  CategoryTree CT_gamT1(VS().a("gamT1"), "", "#gamma +", kX_sup, false);


  CT_0bISR_0jsv.AddSubCategory(CT_etaF);
  CT_0bISR_0jsv.AddSubCategory(CT_etaC);
  CT_1bISR_0jsv.AddSubCategory(CT_etaF);
  CT_1bISR_0jsv.AddSubCategory(CT_etaC);
  
  CT_0bISR_0j.AddSubCategory(CT_p0);
  CT_0bISR_0j.AddSubCategory(CT_p1);
  CT_1bISR_0j.AddSubCategory(CT_p0);
  CT_1bISR_0j.AddSubCategory(CT_p1);

  CT_0bISR_1j.AddSubCategory(CT_0b);
  CT_0bISR_1j.AddSubCategory(CT_1b);
  CT_1bISR_1j.AddSubCategory(CT_0b);
  CT_1bISR_1j.AddSubCategory(CT_1b);



 // CT_p0.AddSubCategory(CT_gamT0);
 // CT_p1.AddSubCategory(CT_gamT0);
 // CT_p0.AddSubCategory(CT_gamT1);
 // CT_p1.AddSubCategory(CT_gamT1);
  
  
  CT_gamT0.AddSubCategory(CT_p0);
  CT_gamT0.AddSubCategory(CT_p1);
  CT_gamT1.AddSubCategory(CT_p0);
  CT_gamT1.AddSubCategory(CT_p1);

  CT_0b.AddSubCategory(CT_gamT0);
  CT_0b.AddSubCategory(CT_gamT1);
  CT_1b.AddSubCategory(CT_gamT0);
  CT_1b.AddSubCategory(CT_gamT1);
  CT_ge1b.AddSubCategory(CT_gamT0);
  CT_ge1b.AddSubCategory(CT_gamT1);
  CT_2b.AddSubCategory(CT_gamT0);
  CT_2b.AddSubCategory(CT_gamT1);
 

  CategoryTree CT_0sv_0j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_0j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  CategoryTree CT_0sv_1j(VS().a("0svS"), "0 sv #in S", "0sv", kJ_sup, true);
  CategoryTree CT_ge1sv_1j(VS().a("ge1svS"), "#geq 1 sv #in S", "1sv", kJ_sup, true);
  
 
  CT_0sv_0j.AddSubCategory(CT_0bISR_0j);
  CT_0sv_0j.AddSubCategory(CT_1bISR_0j);
  CT_0sv_1j.AddSubCategory(CT_0bISR_1j);
  CT_0sv_1j.AddSubCategory(CT_1bISR_1j);
  CT_ge1sv_0j.AddSubCategory(CT_0bISR_0jsv);
  CT_ge1sv_0j.AddSubCategory(CT_1bISR_0jsv);
  CT_ge1sv_1j.AddSubCategory(CT_etaC);
  CT_ge1sv_1j.AddSubCategory(CT_etaF);


//depth = 4 - splits by bS multiplicity

  CategoryTree CT_gold_0j(VS().a("gold"), "gold #it{l}", "1L0jG", kL_sub, true);
  CategoryTree CT_gold_1j(VS().a("gold"), "gold #it{l}", "1L1jG", kL_sub, true);
  CategoryTree CT_gold_2j(VS().a("gold"), "gold #it{l}", "1L2jG", kL_sub, true);
  CategoryTree CT_gold_3j(VS().a("gold"), "gold #it{l}", "1L3jG", kL_sub, true);
  CategoryTree CT_gold_ge4j(VS().a("gold"), "gold #it{l}", "1Lge4jG", kL_sub, true);
  CategoryTree CT_silver_0j(VS().a("slvr"), "silver #it{l}", "1L0jS", kL_sub, true);
  CategoryTree CT_silver_1j(VS().a("slvr"), "silver #it{l}", "1L1jS", kL_sub, true);
  CategoryTree CT_silver_2j(VS().a("slvr"), "silver #it{l}", "1L2jS", kL_sub, true);
  CategoryTree CT_silver_3j(VS().a("slvr"), "silver #it{l}", "1L3jS", kL_sub, true);
  CategoryTree CT_silver_ge4j(VS().a("slvr"), "silver #it{l}", "1Lge4jS", kL_sub, true);
  CategoryTree CT_bronze_0j(VS().a("bron"), "bronze #it{l}", "1L0jB", kL_sub, true);
  CategoryTree CT_bronze_1j(VS().a("bron"), "bronze #it{l}", "1L1jB", kL_sub, true);
  CategoryTree CT_bronze_2j(VS().a("bron"), "bronze #it{l}", "1L2jB", kL_sub, true);
  CategoryTree CT_bronze_3j(VS().a("bron"), "bronze #it{l}", "1L3jB", kL_sub, true);
  CategoryTree CT_bronze_ge4j(VS().a("bron"), "bronze #it{l}", "1Lge4jB", kL_sub, true);
  

  CT_gold_0j.AddSubCategory(CT_0sv_0j);
  CT_gold_0j.AddSubCategory(CT_ge1sv_0j);
  CT_gold_1j.AddSubCategory(CT_0sv_1j);
  CT_gold_1j.AddSubCategory(CT_ge1sv_1j);
  CT_gold_2j.AddSubCategory(CT_0b);
  CT_gold_2j.AddSubCategory(CT_1b);
  CT_gold_2j.AddSubCategory(CT_2b);
  CT_gold_3j.AddSubCategory(CT_0b);
  CT_gold_3j.AddSubCategory(CT_1b);
  CT_gold_3j.AddSubCategory(CT_2b);
  CT_gold_ge4j.AddSubCategory(CT_0b);
  CT_gold_ge4j.AddSubCategory(CT_ge1b);

  CT_silver_0j.AddSubCategory(CT_0sv_0j);
  CT_silver_0j.AddSubCategory(CT_ge1sv_0j);
  CT_silver_1j.AddSubCategory(CT_0sv_1j);
  CT_silver_1j.AddSubCategory(CT_ge1sv_1j);
  CT_silver_2j.AddSubCategory(CT_0b);
  CT_silver_2j.AddSubCategory(CT_1b);
  CT_silver_2j.AddSubCategory(CT_2b);
  CT_silver_3j.AddSubCategory(CT_0b);
  CT_silver_3j.AddSubCategory(CT_1b);
  CT_silver_3j.AddSubCategory(CT_2b);
  CT_silver_ge4j.AddSubCategory(CT_0b);
  CT_silver_ge4j.AddSubCategory(CT_ge1b);
  

  CT_bronze_0j.AddSubCategory(CT_0sv_0j);
  CT_bronze_0j.AddSubCategory(CT_ge1sv_0j);
  CT_bronze_1j.AddSubCategory(CT_0sv_1j);
  CT_bronze_1j.AddSubCategory(CT_ge1sv_1j);
  CT_bronze_2j.AddSubCategory(CT_0b);
  CT_bronze_2j.AddSubCategory(CT_1b);
  CT_bronze_2j.AddSubCategory(CT_2b);
  CT_bronze_3j.AddSubCategory(CT_0b);
  CT_bronze_3j.AddSubCategory(CT_1b);
  CT_bronze_3j.AddSubCategory(CT_2b);
  CT_bronze_ge4j.AddSubCategory(CT_0b);
  CT_bronze_ge4j.AddSubCategory(CT_ge1b);


  CategoryTree CT_0j(VS().a("_0j"), "", "1L0J", kJ, true);
  CategoryTree CT_1j(VS().a("_1j"), "", "1L1J", kJ, true);
  CategoryTree CT_2j(VS().a("_2j"), "2 j #in S", "1L2J", kJ, true);
  CategoryTree CT_3j(VS().a("_3j"), "3 j #in S", "1L3J", kJ, true);
  CategoryTree CT_ge4j(VS().a("_ge4j"), "#geq 4 j #in S", "1L4J", kJ, true);

  
  CT_0j.AddSubCategory(CT_gold_0j);
  CT_0j.AddSubCategory(CT_silver_0j);
  CT_0j.AddSubCategory(CT_bronze_0j);
  CT_1j.AddSubCategory(CT_gold_1j);
  CT_1j.AddSubCategory(CT_silver_1j);
  CT_1j.AddSubCategory(CT_bronze_1j);
  CT_2j.AddSubCategory(CT_gold_2j);
  CT_2j.AddSubCategory(CT_silver_2j);
  CT_2j.AddSubCategory(CT_bronze_2j);
  CT_3j.AddSubCategory(CT_gold_3j);
  CT_3j.AddSubCategory(CT_silver_3j);
  CT_3j.AddSubCategory(CT_bronze_3j);
  CT_ge4j.AddSubCategory(CT_gold_ge4j);
  CT_ge4j.AddSubCategory(CT_silver_ge4j);
  CT_ge4j.AddSubCategory(CT_bronze_ge4j);
  
  CategoryTree CT_1L(VS().a("Ch1L"), "1L", "1L", kL, true);
 
  CT_1L.AddSubCategory(CT_0j);
  CT_1L.AddSubCategory(CT_1j);
  CT_1L.AddSubCategory(CT_2j);
  CT_1L.AddSubCategory(CT_3j);
  CT_1L.AddSubCategory(CT_ge4j);
  

  return CT_1L;
}
