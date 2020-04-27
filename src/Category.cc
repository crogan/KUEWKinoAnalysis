#include <iostream>
#include <map>

#include "../include/Category.hh"

///////////////////////////////////////////
////////// Lep class
///////////////////////////////////////////

Lep::Lep(LepFlavor flavor, LepCharge charge,
	 LepID id, LepSource source){
  m_Flavor = flavor;
  m_Charge = charge;
  m_ID = id;
  m_Source = source;
}

Lep::Lep(const Lep& lep){
  m_Flavor = lep.Flavor();
  m_Charge = lep.Charge();
  m_ID = lep.ID();
  m_Source = lep.Source();
}

Lep::~Lep(){ }

// for sorting leptons in category
bool Lep::operator < (const Lep& lep) const {
  if(m_ID < lep.ID())
    return true;
  if(m_ID > lep.ID())
    return false;

  if(m_Flavor < lep.Flavor())
    return true;
  if(m_Flavor > lep.Flavor())
    return false;

  if(m_Charge < lep.Charge())
    return true;
  if(m_Charge > lep.Charge())
    return true;
  
  if(m_Source < lep.Source())
    return true;

  return false;
  
}

// for matching lepton categories
bool Lep::operator == (const Lep& lep) const {
  return (m_Flavor == lep.Flavor() &&
	  m_ID     == lep.ID() &&
	  m_Charge == lep.Charge());
}

LepFlavor Lep::Flavor() const {
  return m_Flavor;
}

LepCharge Lep::Charge() const {
  return m_Charge;
}

LepID Lep::ID() const {
  return m_ID;
}

LepSource Lep::Source() const {
  return m_Source;
}

Lep& Lep::SetFlavor(LepFlavor flavor){
  m_Flavor = flavor;

  return *this;
}

Lep& Lep::SetCharge(LepCharge charge){
  m_Charge = charge;
  
  return *this;
}

Lep& Lep::SetID(LepID id){
  m_ID = id;
  
  return *this;
}

Lep& Lep::SetSource(LepSource source){
  m_Source = source;
  
  return *this;
}

std::string Lep::IDLabel() const {
  if(m_Flavor == kElectron)
    return string("el")+Form("%d",m_ID);
  else
    return string("mu")+Form("%d",m_ID);
}

LepSource GetLepSource(int PDGID, int genPDGID, int momPDGID){
  if(abs(genPDGID) != 11 && abs(genPDGID) != 13)
    return kLF;

  if(PDGID*genPDGID < 0){
    return kQFlip;
  } else if((abs(momPDGID) == abs(PDGID)) ||
	    (abs(momPDGID) == 22) ||
	    (abs(momPDGID) == 23) ||
	    (abs(momPDGID) == 24) ||
	    ((abs(momPDGID) > 1000000) &&
	     (abs(momPDGID) < 3000000))){
    return kSignal;
  } else if(abs(momPDGID) == 15){
    return kTau;
  } else if((abs(momPDGID) == 4) ||
	    (abs(momPDGID) == 5) ||
	    ((abs(momPDGID) > 400) && (abs(momPDGID) < 600)) ||
	    ((abs(momPDGID) > 4000) && (abs(momPDGID) < 6000))){
    return kHF;
  } else {
    return kLF;
  }
}

///////////////////////////////////////////
////////// LepList class
///////////////////////////////////////////

LepList::LepList(){
  m_N = 0;
}

LepList::LepList(const LepList& leps){
  m_N = 0;
  int N = leps.GetN();
  for(int i = 0; i < N; i++)
    *this += leps[i];
}

LepList::~LepList(){
  Clear();
}

void LepList::Clear(){
  
  for(int i = 0; i < m_N; i++)
    delete m_Lep[i];
    
  m_Lep.clear();
  m_N = 0;
}

LepList& LepList::operator = (const LepList& lep){
  Clear();
  int N = lep.GetN();
  for(int i = 0; i < N; i++)
    *this += lep[i];
  
  return *this;
}

int LepList::GetN() const {
  return m_N;
}

const Lep& LepList::operator [] (int i) const {
  // no protection for array out-of-bounds
  return *m_Lep[i];
}

LepList& LepList::Add(const Lep& lep){
  return (*this) += lep;
}

LepList& LepList::operator += (const Lep& lep){
  m_Lep.push_back(new Lep(lep));
  m_N++;
  
  if(m_N == 1)
    return *this;
  
  // keep list ordered
  int index = m_N-2;
  while(index >= 0){
    if(*m_Lep[index+1] < *m_Lep[index]){
      Lep* temp = m_Lep[index+1];
      m_Lep[index+1] = m_Lep[index];
      m_Lep[index] = temp;
    } else {
      break;
    }
    index--;
  }

  return *this;
}

LepList& LepList::operator += (const LepList& leps){
  int N = leps.GetN();
  for(int i = 0; i < N; i++)
    *this += leps[i];
  
  return *this;
}

// for matching hemispheres
bool LepList::operator == (const LepList& leps) const {
  if(m_N != leps.GetN())
    return false;

  for(int i = 0; i < m_N; i++)
    if(!((*this)[i] == leps[i]))
      return false;

  return true;
}

// for sorting hemispheres
bool LepList::operator < (const LepList& leps) const {
  if(m_N < leps.GetN())
    return true;
  if(m_N > leps.GetN())
    return false;

  for(int i = 0; i < m_N; i++){
    if((*this)[i] < leps[i])
      return true;
    if(leps[i] < (*this)[i])
      return false;
  }

  return false;
}

std::string LepList::GetIDLabel() const {
  if(m_N == 0)
    return "0";
  string label = "";
  for(int i = 0; i < m_N; i++)
    label += (*this)[i].IDLabel();

  return label;
}

vector<std::string> LepList::GetFakeLabels(LepSource fake) const {
  std::string label0 = "Fakes_";
  std::string label;
  vector<std::string> labels;
  
  for(int i = 0; i < m_N; i++){
    if((*this)[i].Source() >= fake){
      label = label0;
      label += ((*this)[i].Flavor() == kElectron ? "elf" : "muf");
      label += Form("%d", int((*this)[i].Source()-fake));
      labels.push_back(label);
    }
  }

  return labels;
}

LepList LepList::GetFakes(LepSource fake) const {
  LepList list;
  for(int i = 0; i < m_N; i++){
    if((*this)[i].Source() >= fake){
      Lep lep = (*this)[i];
      lep.SetID(kGold);
      lep.SetCharge(kPos);
      list += lep;
    }
  }

  return list;
}

///////////////////////////////////////////
////////// Criteria abstract base class
///////////////////////////////////////////

Criteria::Criteria(const std::string& label){
  m_Label = label;
}

Criteria::Criteria(const Criteria& criteria){
  m_Label = criteria.Label();
}

Criteria::~Criteria(){ }

const string& Criteria::Label() const {
  return m_Label;
}

Criteria& Criteria::SetLabel(const string& label){
  m_Label = label;

  return *this;
}

///////////////////////////////////////////
////////// CriteriaList class
///////////////////////////////////////////

CriteriaList::CriteriaList(){
  m_N = 0;
}

CriteriaList::CriteriaList(const CriteriaList& list){
  m_N = 0;
  int N = list.GetN();
  for(int i = 0; i < N; i++)
    *this += list[i];
}

CriteriaList::~CriteriaList(){
  Clear();
}

void CriteriaList::Clear(){
  for(int i = 0; i < m_N; i++)
    delete m_Criteria[i];
    
  m_Criteria.clear();
  m_N = 0;
}

CriteriaList& CriteriaList::operator = (const CriteriaList& list){
  Clear();
  int N = list.GetN();
  for(int i = 0; i < N; i++)
    *this += list[i];
  
  return *this;
}
  
int CriteriaList::GetN() const {
  return m_N;
}

const Criteria& CriteriaList::operator [] (int i) const {
  return *m_Criteria[i];
}

CriteriaList& CriteriaList::Add(const Criteria& criteria){
  if(dynamic_cast<const Leptonic*>(&criteria)){
    m_Criteria.push_back(new Leptonic(dynamic_cast<const Leptonic&>(criteria)));
    m_N++;
  }
  if(dynamic_cast<const Hadronic*>(&criteria)){
    m_Criteria.push_back(new Hadronic(dynamic_cast<const Hadronic&>(criteria)));
    m_N++;
  }
  if(dynamic_cast<const GenericBin*>(&criteria)){
    m_Criteria.push_back(new GenericBin(dynamic_cast<const GenericBin&>(criteria)));
    m_N++;
  }
  if(dynamic_cast<const GenericVal*>(&criteria)){
    m_Criteria.push_back(new GenericVal(dynamic_cast<const GenericVal&>(criteria)));
    m_N++;
  }

  return *this;
}

CriteriaList& CriteriaList::operator += (const Criteria& criteria){
  return Add(criteria);
}

CriteriaList& CriteriaList::operator += (const CriteriaList& criteria){
  int N = criteria.GetN();
  for(int i = 0; i < N; i++)
    *this += criteria[i];
  
  return *this;
}

///////////////////////////////////////////
////////// Leptonic class
///////////////////////////////////////////

Leptonic::Leptonic(const LepList& Hem_a, const LepList& Hem_b, const std::string& label)
  : Criteria(label) {
  if(Hem_b < Hem_a){
    m_Lep_a = Hem_b;
    m_Lep_b = Hem_a;
  } else {
    m_Lep_a = Hem_a;
    m_Lep_b = Hem_b;
  }

   m_Nleptonic = 1;
   m_Leptonic.push_back(this);
}

Leptonic::Leptonic(const Leptonic& lep)
  : Criteria(lep) {
  m_Lep_a = lep.GetHemA();
  m_Lep_b = lep.GetHemB();
  int N = lep.GetN();
  m_Nleptonic = 1;
  m_Leptonic.push_back(this);
  
  for(int i = 1; i < N; i++)
    *this += lep[i];
}
    
Leptonic::~Leptonic(){
  for(int i = 1; i < m_Nleptonic; i++)
    delete m_Leptonic[i];
}

// see if there is a match between leptonic systems
bool Leptonic::operator == (const Criteria& val) const {
  const Leptonic* lep = dynamic_cast<const Leptonic*>(&val);
  if(!lep)
    return false;
  
  int N = lep->GetN();
  for(int i = 0; i < m_Nleptonic; i++)
    for(int j = 0; j < N; j++)
      if(((*this)[i].GetHemA() == (*lep)[j].GetHemA()) &&
	 ((*this)[i].GetHemB() == (*lep)[j].GetHemB()))
	return true;
  return false;
}

int Leptonic::GetN() const{
  return m_Nleptonic;
}

const Leptonic& Leptonic::operator [] (int i) const {
  // no check for array-out-of-bounds
  return *m_Leptonic[i];
}

Leptonic& Leptonic::operator += (const Leptonic& leptonic){
  if((m_Lep_a == leptonic.GetHemA()) &&
     (m_Lep_b == leptonic.GetHemB())){
    return *this;
  }
  m_Nleptonic++;
  m_Leptonic.push_back(new Leptonic(leptonic));
  return *this;
}

const LepList& Leptonic::GetHemA() const {
  return m_Lep_a;
}

const LepList& Leptonic::GetHemB() const {
  return m_Lep_b;
}

///////////////////////////////////////////
////////// Hadronic class
///////////////////////////////////////////

Hadronic::Hadronic(int Njet, int Nbjet, int NSV, const std::string& label)
  : Criteria(label) {
  m_Njet  = Njet;
  m_Nbjet = Nbjet;
  m_NSV   = NSV;

  m_Nhadronic = 1;
  m_Hadronic.push_back(this);
}

Hadronic::Hadronic(const Hadronic& had)
  : Criteria(had) {
  m_Njet  = had.GetNjet();
  m_Nbjet = had.GetNbjet();
  m_NSV   = had.GetNSV();
  int N = had.GetN();
  m_Nhadronic = 1;
  m_Hadronic.push_back(this);
  
  for(int i = 1; i < N; i++)
    *this += had[i];
}

Hadronic::~Hadronic(){
  for(int i = 1; i < m_Nhadronic; i++)
    delete m_Hadronic[i];
}

bool Hadronic::operator == (const Criteria& val) const {
  const Hadronic* had = dynamic_cast<const Hadronic*>(&val);
  if(!had)
    return false;
  
  int N = had->GetN();
  for(int i = 0; i < m_Nhadronic; i++){
    for(int j = 0; j < N; j++){
      bool match = 
	(((*this)[i].GetNjet() == (*had)[j].GetNjet()) ||
	 ((((*this)[i].GetNjet() < 0 && (*had)[j].GetNjet() > 0) ||
	   ((*this)[i].GetNjet() > 0 && (*had)[j].GetNjet() < 0)) &&
	  (*had)[j].GetNjet() >= -(*this)[i].GetNjet())) &&
	(((*this)[i].GetNbjet() == (*had)[j].GetNbjet()) ||
	 ((((*this)[i].GetNbjet() < 0 && (*had)[j].GetNbjet() > 0) ||
	   ((*this)[i].GetNbjet() > 0 && (*had)[j].GetNbjet() < 0)) &&
	  (*had)[j].GetNbjet() >= -(*this)[i].GetNbjet())) &&
	(((*this)[i].GetNSV() == (*had)[j].GetNSV()) ||
	 ((((*this)[i].GetNSV() < 0 && (*had)[j].GetNSV() > 0) ||
	   ((*this)[i].GetNSV() > 0 && (*had)[j].GetNSV() < 0)) &&
	  (*had)[j].GetNSV() >= -(*this)[i].GetNSV()));
      if(match)
	return true;
    }
  }

  return false;
}

int Hadronic::GetN() const {
  return m_Nhadronic;
}

const Hadronic& Hadronic::operator [] (int i) const {
  // no check for array-out-of-bounds
  return *m_Hadronic[i];
}

Hadronic& Hadronic::operator += (const Hadronic& hadronic){
  m_Nhadronic++;
  m_Hadronic.push_back(new Hadronic(hadronic));

  return *this;
}

int Hadronic::GetNjet() const {
  return m_Njet;
}

int Hadronic::GetNbjet() const {
  return m_Nbjet;
}

int Hadronic::GetNSV() const {
  return m_NSV;
}

///////////////////////////////////////////
////////// GenericBin class
///////////////////////////////////////////

GenericBin::GenericBin(double bin_min, double bin_max, const std::string& label)
  : Criteria(label) {
  m_Min = bin_min;
  m_Max = bin_max;
}

GenericBin::GenericBin(const GenericBin& bin)
  : Criteria(bin) {
  m_Min = bin.GetMin();
  m_Max = bin.GetMax();
}

GenericBin::~GenericBin(){ }

double GenericBin::GetMin() const {
  return m_Min;
}

double GenericBin::GetMax() const {
  return m_Max;
}

bool GenericBin::operator == (const Criteria& criteria) const {
  const GenericVal* val = dynamic_cast<const GenericVal*>(&criteria);
  if(!val)
    return false;

  return (val->GetVal() >= m_Min) &&
    ((val->GetVal() < m_Max) || (m_Max <= m_Min));
}

///////////////////////////////////////////
////////// GenericVal class
///////////////////////////////////////////

GenericVal::GenericVal(double val, const std::string& label)
  : Criteria(label) {
  m_Val = val;
}

GenericVal::GenericVal(const GenericVal& val)
  : Criteria(val){
  m_Val = val.GetVal();
}

GenericVal::~GenericVal(){ }

double GenericVal::GetVal() const {
  return m_Val;
}

bool GenericVal::operator == (const Criteria& bin) const {
  const GenericVal* val = dynamic_cast<const GenericVal*>(&bin);
  if(!val)
    return false;

  return m_Val == val->GetVal();
}

///////////////////////////////////////////
////////// FitBin class
///////////////////////////////////////////

// default is to include overflow in last bin for x, but not y
FitBin::FitBin(const vector<double>& bin_edges_x,
	       const vector<double>& bin_edges_y){
  m_hist1D = nullptr;
  m_hist2D = nullptr;
  
  if(bin_edges_x.size() < 2){
    m_NX = 1;
    if(bin_edges_x.size() == 0){
      m_Edges_X.push_back(-1.);
      m_Edges_X.push_back( 1.);
    } else {
      m_Edges_X.push_back(bin_edges_x[0]);
      m_Edges_X.push_back(m_Edges_X[0]+1.);
    }
  } else {
    m_Edges_X = bin_edges_x;
    m_NX = m_Edges_X.size() - 1;
  }
  if(bin_edges_y.size() < 2){
    m_NY = 1;
    if(bin_edges_y.size() == 0){
      m_Edges_Y.push_back(-1.);
      m_Edges_Y.push_back( 1.);
    } else {
      m_Edges_Y.push_back(bin_edges_y[0]);
      m_Edges_Y.push_back(m_Edges_Y[0]+1.);
    }
  } else {
    m_Edges_Y = bin_edges_y;
    m_NY = m_Edges_Y.size() - 1;
  }
  
  m_LastBinCenterX = (m_Edges_X[m_NX]+m_Edges_X[m_NX-1])/2.;
}

FitBin::FitBin(const FitBin& bin){
  m_hist1D = nullptr;
  m_hist2D = nullptr;
  
  m_Edges_X = bin.GetBinEdgesX();
  m_Edges_Y = bin.GetBinEdgesY();

  m_NX = m_Edges_X.size() - 1;
  m_NY = m_Edges_Y.size() - 1;

  m_LastBinCenterX = (m_Edges_X[m_NX]+m_Edges_X[m_NX-1])/2.;
}

FitBin::~FitBin(){
  if(m_hist1D)
    delete m_hist1D;
  if(m_hist2D)
    delete m_hist2D;
}

FitBin& FitBin::operator = (const FitBin& bin){
  if(m_hist1D)
    delete m_hist1D;
  if(m_hist2D)
    delete m_hist2D;

  m_hist1D = nullptr;
  m_hist2D = nullptr;
  
  m_Edges_X = bin.GetBinEdgesX();
  m_Edges_Y = bin.GetBinEdgesY();

  m_NX = m_Edges_X.size() - 1;
  m_NY = m_Edges_Y.size() - 1;

  m_LastBinCenterX = (m_Edges_X[m_NX]+m_Edges_X[m_NX-1])/2.;

  return *this;
}

FitBin& FitBin::InitializeHistogram(const std::string& label){
   m_hist2D = (TH2D*) new TH2D(string("h2D_"+label).c_str(),
			       string("h2D_"+label).c_str(),
			       m_NX, &m_Edges_X[0],
			       m_NY, &m_Edges_Y[0]);
   m_hist1D = (TH1D*) new TH1D(string("h1D_"+label).c_str(),
			       string("h1D_"+label).c_str(),
			       m_NX*m_NY, -0.5, m_NX*m_NY-0.5);
   return *this;
}

void FitBin::Fill(double weight, double X, double Y){
  if(m_hist1D == nullptr ||
     m_hist2D == nullptr)
    return;
  
  int iX = m_hist2D->GetXaxis()->FindBin(X);
  int iY = m_hist2D->GetYaxis()->FindBin(Y);
  if(iX <= 0 || iY <= 0 || iY > m_NY)
    return;
  if(iX > m_NX){
    iX -= 1;
    X = m_LastBinCenterX;
  }

  m_hist2D->Fill(X, Y, weight);
  m_hist1D->Fill((iX-1) + (iY-1)*m_NX, weight);
}

const vector<double>& FitBin::GetBinEdgesX() const {
  return m_Edges_X;
}

const vector<double>& FitBin::GetBinEdgesY() const {
  return m_Edges_Y;
}

void FitBin::WriteHistogram(const std::string& name,
			    const std::string& fold,
			    TFile& file) const {
  if(m_hist1D == nullptr ||
     m_hist2D == nullptr)
    return;

  file.cd();
  file.cd(fold.c_str());
  m_hist1D->Write(name.c_str(), TObject::kOverwrite);
  file.cd();
  file.cd(("hist2D/"+fold).c_str());
  m_hist2D->Write(("2D_"+name).c_str(), TObject::kOverwrite);
  file.cd();
}

///////////////////////////////////////////
////////// Category class
///////////////////////////////////////////

Category::Category(const Leptonic& lep,
		   const Hadronic& S_had,
		   const Hadronic& ISR_had){
  m_Criteria += lep;
  m_Criteria += S_had;
  m_Criteria += ISR_had;
}

Category::Category(const Category& cat){
  int N = cat.GetN();

  for(int i = 0; i < N; i++)
    m_Criteria += cat[i];

  m_Template = cat.GetFitBin();
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
    if(i < N-1)
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

bool Category::operator == (const Category& cat) const {
  int N = m_Criteria.GetN();
  if(cat.GetN() < N)
    return false;

  for(int i = 0; i < N; i++)
    if(!(m_Criteria[i] == cat[i]))
      return false;

  return true;
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
  bin->InitializeHistogram(process+"_"+GetLabel());

  return bin;
}

CategoryList Category::CreateLeptonIDRegions(int NID, int Nfakemax){
  CategoryList list;
  
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
      llabel = list_a.GetIDLabel();
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
    Category icat(ileptonic, had_S, had_ISR);
    for(int i = 3; i < Nc; i++)
      icat.AddGenericBin(dynamic_cast<const GenericBin&>(m_Criteria[i]));
    icat.SetFitBin(m_Template);

    list += icat;

    delete ilep->second;
    
    ilep++;
  }

  return list;
  
}

CategoryList Category::CreateGenericRegions(const string& label, const vector<double>& bin_edges){
  CategoryList list;
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
    Category icat(lep, had_S, had_ISR);
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
    Category icat(lep, *had[i], had_ISR);
    for(int i = 3; i < Nc; i++)
      icat.AddGenericBin(dynamic_cast<const GenericBin&>(m_Criteria[i]));
    icat.SetFitBin(m_Template);

    list += icat;
  }
  
  return list;
}

CategoryList Category::CreateHadronicISRRegions(const vector<const Hadronic*>& had) const {
  CategoryList list;
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
    Category icat(lep, had_S, *had[i]);
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

void CategoryList::Print() const {
  for(int i = 0; i < m_N; i++)
    cout << m_Cat[i]->GetLabel() << endl;
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

CategoryList CategoryList::CreateGenericRegions(const string& label, const vector<double>& bin_edges) const {
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
