#include <iostream>
#include <map>

#include "Category.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"

using std::cout;
using std::endl;

///////////////////////////////////////////
////////// Category class
///////////////////////////////////////////

Category::Category(const Leptonic& lep,
		   const string& name)
  : Criteria(name){
  m_Criteria += lep;
  m_Criteria += Hadronic();
  m_Criteria += Hadronic();
}

Category::Category(const Leptonic& lep,
		   const Hadronic& S_had,
		   const Hadronic& ISR_had,
		   const string& name)
  : Criteria(name){
  
  m_Criteria += lep;
  m_Criteria += S_had;
  m_Criteria += ISR_had;
 
}

Category::Category(const Category& cat)
  : Criteria(cat){
  int N = cat.GetN();

  for(int i = 0; i < N; i++){
    m_Criteria += cat[i];
  }
  m_Template = cat.GetFitBin();
}


Category::Category(const string& label,
		   const FitBin& bin,
		   const string& name)
  : Criteria(name){
  
  m_Criteria += GenericVal(0., label);
  m_Template = bin;
}

Category::~Category(){ }

std::string Category::GetLabel() const {
  int N = m_Criteria.GetN();

  string label = "";
  for(int i = 0; i < N; i++){
    label += m_Criteria[i].Label();
    if(i == 1)
      label += "S";
    if(i == 2)
      label += "ISR";
    if(i < N-1 && N > 1)
      label += "-";
  }

  return label;
}

int Category::GetN() const {
  return m_Criteria.GetN();
}

const Criteria& Category::operator [] (int i) const {
  return m_Criteria[i];
}

Category& Category::AddGenericBin(const GenericBin& bin){
  m_Criteria += bin;

  return *this;
}

Category& Category::AddGenericVal(const GenericVal& val){
  m_Criteria += val;

  return *this;
}

bool Category::operator == (const Criteria& cat) const {
  const Category* pcat = dynamic_cast<const Category*>(&cat);
  if(!pcat)
    return false;
  
  int N = m_Criteria.GetN();
  if(pcat->GetN() < N)
    return false;

  for(int i = 0; i < N; i++){
    if(!(m_Criteria[i] == (*pcat)[i])){
      return false;
    }
  }

  return true;
}

bool Category:: operator < (const Category& cat) const {
  return ((Label()+"_"+GetLabel()) < (cat.Label()+"_"+cat.GetLabel()));
}

bool Category::operator > (const Category& cat) const {
  return ((Label()+"_"+GetLabel()) > (cat.Label()+"_"+cat.GetLabel()));
}

Category& Category::SetFitBin(const FitBin& bin){
  m_Template = bin;
  
  return *this;
}

const FitBin& Category::GetFitBin() const {
  return m_Template;
}

FitBin* Category::GetNewFitBin(const std::string& process) const {
  FitBin* bin = new FitBin(m_Template);
 
  bin->InitializeHistogram(process+"_"+Label()+"_"+GetLabel());

  return bin;
}

CategoryList Category::CreateLeptonIDRegions(int NID, int Nfakemax){
  CategoryList list;

  if(m_Criteria.GetN() < 3)
    return list;
  
  if(NID <= 1 || Nfakemax <= 0){
    list += *this;
    return list;
  }

  std::map<std::string, Leptonic*> label_to_Leptonic;

  const Leptonic& leptonic = dynamic_cast<const Leptonic&>(m_Criteria[0]);
  int Nleptonic = leptonic.GetN();
  string label  = leptonic.Label();
  for(int l = 0; l < Nleptonic; l++){
    LepList list_a = leptonic[l].GetHemA();
    LepList list_b = leptonic[l].GetHemB();
    int Na = list_a.GetN();
    int Nb = list_b.GetN();
    int N  = Na + Nb;
    string llabel;
    if(N < 1){
      //llabel = list_a.GetIDLabel();
      llabel = "0^0";
      if(label_to_Leptonic.count(llabel) == 0)
	label_to_Leptonic[llabel] = new Leptonic(list_a, list_b);
      else
	(*label_to_Leptonic[llabel]) += Leptonic(list_a, list_b);
    }
    LepList nlist_a;
    LepList nlist_b;
    LepList nlist;
    for(int f = 0; f < N; f++){
      for(int i = 0; i < NID; i++){
	nlist_a.Clear();
	nlist_b.Clear();
	nlist.Clear();
	Lep* lep = nullptr; 
	if(f < Na)
	  lep = new Lep(list_a[f]);
	else
	  lep = new Lep(list_b[f-Na]);
	
	lep->SetID(LepID(i));
	
	for(int j = 0; j < N; j++){
	  if(j < Na)
	    if(j != f)
	      nlist_a += list_a[j];
	    else
	      nlist_a += *lep;
	  else
	    if(j != f)
	      nlist_b += list_b[j-Na];
	    else
	      nlist_b += *lep;
	}
	nlist += nlist_a;
	nlist += nlist_b;
	
	llabel = nlist.GetIDLabel();
	if(label_to_Leptonic.count(llabel) == 0)
	  label_to_Leptonic[llabel] = new Leptonic(nlist_a, nlist_b);
	else
	  (*label_to_Leptonic[llabel]) += Leptonic(nlist_a, nlist_b);
      } 
    }

    if(Nfakemax >= 2){
      for(int f0 = 0; f0 < N-1; f0++){
	for(int f1 = f0+1; f1 < N; f1++){	  
	  for(int i0 = 0; i0 < NID; i0++){
	    for(int i1 = 0; i1 < NID; i1++){
	      Lep* lep0 = nullptr; 
	      if(f0 < Na)
		lep0 = new Lep(list_a[f0]);
	      else
		lep0 = new Lep(list_b[f0-Na]);
	      Lep* lep1 = nullptr; 
	      if(f1 < Na)
		lep1 = new Lep(list_a[f1]);
	      else
		lep1 = new Lep(list_b[f1-Na]);
	      lep0->SetID(LepID(i0));
	      lep1->SetID(LepID(i1));
	      
	      nlist_a.Clear();
	      nlist_b.Clear();
	      nlist.Clear();
	      for(int j = 0; j < N; j++){
		if(j < Na){
		  if(j != f0 && j != f1)
		    nlist_a += list_a[j];
		  if(j == f0)
		    nlist_a += *lep0;
		  if(j == f1)
		    nlist_a += *lep1;
		} else {
		  if(j != f0 && j != f1)
		    nlist_b += list_b[j-Na];
		  if(j == f0)
		    nlist_b += *lep0;
		  if(j == f1)
		    nlist_b += *lep1;
		}
	      }
	      nlist += nlist_a;
	      nlist += nlist_b;
	      
	      llabel = nlist.GetIDLabel();
	      if(label_to_Leptonic.count(llabel) == 0)
		label_to_Leptonic[llabel] = new Leptonic(nlist_a, nlist_b);
	      else
		(*label_to_Leptonic[llabel]) += Leptonic(nlist_a, nlist_b);
	    }
	  }
	} 
      } 
    }   
  }

  int Nc = m_Criteria.GetN();
  const Hadronic& had_S   = dynamic_cast<const Hadronic&>(m_Criteria[1]);
  const Hadronic& had_ISR = dynamic_cast<const Hadronic&>(m_Criteria[2]);
  
  std::map<std::string, Leptonic*>::iterator ilep = label_to_Leptonic.begin();
  while(ilep != label_to_Leptonic.end()){
    string    ilabel = ilep->first;
    Leptonic& ileptonic = *ilep->second;
    ileptonic.SetLabel(label+"-"+ilabel);
    Category icat(ileptonic, had_S, had_ISR, Label());
    for(int i = 3; i < Nc; i++)
      icat.AddGenericBin(dynamic_cast<const GenericBin&>(m_Criteria[i]));
    icat.SetFitBin(m_Template);

    list += icat;

    delete ilep->second;
    
    ilep++;
  }

  return list;
  
}

CategoryList Category::CreateGenericRegions(const string& label, const VD& bin_edges){
  CategoryList list;

  if(m_Criteria.GetN() < 3)
    return list;
  
  int Nbin = bin_edges.size();

  if(Nbin == 0){
    list += *this;
    return list;
  }

  vector<GenericBin*> Bins;
  for(int i = 0; i < Nbin-1; i++)
    Bins.push_back(new GenericBin(bin_edges[i], bin_edges[i+1], label+Form("%d",i)));
  Bins.push_back(new GenericBin(bin_edges[Nbin-1], bin_edges[Nbin-1]-1., label+Form("%d",Nbin-1)));

  int Nc = m_Criteria.GetN();
  const Leptonic& lep     = dynamic_cast<const Leptonic&>(m_Criteria[0]);
  const Hadronic& had_S   = dynamic_cast<const Hadronic&>(m_Criteria[1]);
  const Hadronic& had_ISR = dynamic_cast<const Hadronic&>(m_Criteria[2]);

  for(int i = 0; i < Nbin; i++){
    Category icat(lep, had_S, had_ISR, Label());
    for(int i = 3; i < Nc; i++)
      icat.AddGenericBin(dynamic_cast<const GenericBin&>(m_Criteria[i]));
    icat.AddGenericBin(*Bins[i]);
    icat.SetFitBin(m_Template);
    
    list += icat;
  }
  
  for(int i = 0; i < Nbin; i++)
    delete Bins[i];
  
  return list;
}

CategoryList Category::CreateHadronicSRegions(const vector<const Hadronic*>& had) const {
  CategoryList list;

  if(m_Criteria.GetN() < 3)
    return list;
  
  int Nhad = had.size();

  if(Nhad == 0){
    list += *this;
    return list;
  }

  int Nc = m_Criteria.GetN();
  const Leptonic& lep     = dynamic_cast<const Leptonic&>(m_Criteria[0]);
  const Hadronic& had_S   = dynamic_cast<const Hadronic&>(m_Criteria[1]);
  const Hadronic& had_ISR = dynamic_cast<const Hadronic&>(m_Criteria[2]);

  for(int i = 0; i < Nhad; i++){
    Category icat(lep, *had[i], had_ISR, Label());
    for(int i = 3; i < Nc; i++)
      icat.AddGenericBin(dynamic_cast<const GenericBin&>(m_Criteria[i]));
    icat.SetFitBin(m_Template);

    list += icat;
  }
  
  return list;
}

CategoryList Category::CreateHadronicISRRegions(const vector<const Hadronic*>& had) const {
  CategoryList list;

  if(m_Criteria.GetN() < 3)
    return list;
  
  int Nhad = had.size();
  
  if(Nhad == 0){
    list += *this;
    return list;
  }
  
  int Nc = m_Criteria.GetN();
  const Leptonic& lep     = dynamic_cast<const Leptonic&>(m_Criteria[0]);
  const Hadronic& had_S   = dynamic_cast<const Hadronic&>(m_Criteria[1]);
  const Hadronic& had_ISR = dynamic_cast<const Hadronic&>(m_Criteria[2]);
  
  for(int i = 0; i < Nhad; i++){
    Category icat(lep, had_S, *had[i], Label());
    for(int i = 3; i < Nc; i++)
      icat.AddGenericBin(dynamic_cast<const GenericBin&>(m_Criteria[i]));
    icat.SetFitBin(m_Template);
    
    list += icat;
  }
  
  return list;
}

///////////////////////////////////////////
////////// CategoryList class
///////////////////////////////////////////

CategoryList::CategoryList(){
  m_N = 0;
}

CategoryList::CategoryList(const CategoryList& list){
  m_N = 0;
  int N = list.GetN();
  for(int i = 0; i < N; i++)
    *this += list[i];
}

CategoryList::~CategoryList(){
  Clear();
}

void CategoryList::Clear(){
  for(int i = 0; i < m_N; i++)
    delete m_Cat[i];
    
  m_Cat.clear();
  m_N = 0;
}

int CategoryList::Find(const Category& cat) const {
  for(int i = 0; i < m_N; i++)
    if(*m_Cat[i] == cat)
      return i;

  return -1;
}

CategoryList& CategoryList::operator = (const CategoryList& list){
  Clear();
  int N = list.GetN();
  for(int i = 0; i < N; i++)
    *this += list[i];
  
  return *this;
}
  
int CategoryList::GetN() const {
  return m_N;
}

const Category& CategoryList::operator [] (int i) const {
  return *m_Cat[i];
}

CategoryList& CategoryList::Add(const Category& cat){
  // string label = cat.Label()+"_"+cat.GetLabel();
 
  // if(m_CatMap.count(label) != 0)
  //   return *this;

  // m_CatMap[label] = true;
  
  m_Cat.push_back(new Category(cat));
  m_N++;
  return *this;
}

CategoryList& CategoryList::operator += (const Category& cat){
  return Add(cat);
}

CategoryList& CategoryList::operator += (const CategoryList& cat){
  int N = cat.GetN();
  for(int i = 0; i < N; i++)
    *this += cat[i];
  
  return *this;
}

CategoryList CategoryList::Filter(const string& label) const {
  CategoryList list;

 for(int i = 0; i < m_N; i++)
   if((m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel()).find(label) != std::string::npos)
      list += *m_Cat[i];

  return list;
}

CategoryList CategoryList::Remove(const string& label) const {
  CategoryList list;

 for(int i = 0; i < m_N; i++)
   if((m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel()).find(label) == std::string::npos)
      list += *m_Cat[i];

  return list;
}

CategoryList CategoryList::FilterOR(VS& labels) const {
  CategoryList list;
  int Nl = labels.size();
  for(int i = 0; i < m_N; i++){
    for(int l = 0; l < Nl; l++){
      if((m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel()).find(labels[l]) != std::string::npos){
	cout << "here " << (m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel()) << " " << labels[l] << endl;
	list += *m_Cat[i];
	break;
      }
    }
  }

  return list;
}

CategoryList CategoryList::FilterAND(VS& labels) const {
  CategoryList list;
  int Nl = labels.size();
  for(int i = 0; i < m_N; i++){
    bool match = true;
    for(int l = 0; l < Nl; l++){
      if((m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel()).find(labels[l]) == std::string::npos){
	match = false;
	break;
      }
    }
    if(match)
      list += *m_Cat[i];
  }

  return list;
}

CategoryList CategoryList::RemoveOR(VS& labels) const {
  CategoryList list;
  int Nl = labels.size();
  for(int i = 0; i < m_N; i++){
    bool match = false;
    for(int l = 0; l < Nl; l++){
      if((m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel()).find(labels[l]) != std::string::npos){
	match = true;
	break;
      }
    }
    if(!match)
      list += *m_Cat[i];
  }
  
  return list;
}

CategoryList CategoryList::RemoveAND(VS& labels) const {
  CategoryList list;
  int Nl = labels.size();
  for(int i = 0; i < m_N; i++){
    for(int l = 0; l < Nl; l++){
      if((m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel()).find(labels[l]) == std::string::npos){
	list += *m_Cat[i];
	break;
      }
    }
  }

  return list;
}

void CategoryList::Print() const {
  for(int i = 0; i < m_N; i++)
    std::cout << m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel() << std::endl;
}

CategoryList CategoryList::CreateFitBinRegions(const FitBin& bin) const {
  CategoryList list;
  for(int i = 0; i < m_N; i++)
    list += m_Cat[i]->SetFitBin(bin);

  return list;
}

CategoryList CategoryList::CreateLeptonIDRegions(int NID, int Nfakemax) const {
  CategoryList list;
  for(int i = 0; i < m_N; i++)
    list += m_Cat[i]->CreateLeptonIDRegions(NID, Nfakemax);

  return list;
}

CategoryList CategoryList::CreateGenericRegions(const string& label, const VD& bin_edges) const {
  CategoryList list;
  for(int i = 0; i < m_N; i++)
    list += m_Cat[i]->CreateGenericRegions(label, bin_edges);

  return list;
}

CategoryList CategoryList::CreateHadronicSRegions(const vector<const Hadronic*>& had) const {
  CategoryList list;
  for(int i = 0; i < m_N; i++)
    list += m_Cat[i]->CreateHadronicSRegions(had);

  return list;
}

CategoryList CategoryList::CreateHadronicISRRegions(const vector<const Hadronic*>& had) const {
  CategoryList list;
  for(int i = 0; i < m_N; i++)
    list += m_Cat[i]->CreateHadronicISRRegions(had);

  return list;
}

VC CategoryList::GetCategories() const {
  VC list;

  for(int i = 0; i < m_N; i++)
    list += m_Cat[i]->Label()+"_"+m_Cat[i]->GetLabel();

  return list;
}

///////////////////////////////////////////
////////// CategoryBranch class
///////////////////////////////////////////

CategoryBranch::CategoryBranch(){
  m_Tree = nullptr;
}

CategoryBranch::~CategoryBranch() {}

void CategoryBranch::InitFill(TTree* tree){
  if(!tree)
    return;

  tree->Branch("cat", &m_Cat);
  tree->Branch("bin", &m_Bin);

  m_FitBinBranch.InitFill(tree);

  m_Tree = tree;
}
  
void CategoryBranch::FillCategory(Category& cat){
  m_Cat = cat.Label();
  m_Bin = cat.GetLabel();

  m_FitBinBranch.FillFitBin(cat.GetFitBin());

  if(m_Tree)
    m_Tree->Fill();
}

void CategoryBranch::InitGet(TTree* tree){
  if(!tree)
    return;

  m_CatPtr = 0;
  m_BinPtr = 0;
  tree->SetBranchAddress("cat", &m_CatPtr, &m_b_Cat);
  tree->SetBranchAddress("bin", &m_BinPtr, &m_b_Bin);

  m_FitBinBranch.InitGet(tree);

}

Category CategoryBranch::GetCategory(){
  if(!m_CatPtr || !m_BinPtr)
    return Category(m_Bin,
		    m_FitBinBranch.GetFitBin(),
		    m_Cat);
  else
    return Category(*m_BinPtr,
		    m_FitBinBranch.GetFitBin(),
		    *m_CatPtr);
}
