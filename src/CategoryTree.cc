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

