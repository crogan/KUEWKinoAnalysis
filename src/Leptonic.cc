#include <iostream>
#include <map>

#include "../include/Leptonic.hh"

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
    return false;
  
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
  return string(m_Flavor == kElectron ? "el" : "mu")+Form("%d",m_ID);
  // if(m_Flavor == kElectron)
  //   return string("el")+Form("%d",m_ID)+(m_Charge == kPos ? "p" : "m");
  // else
  //   return string("mu")+Form("%d",m_ID)+(m_Charge == kPos ? "p" : "m");
}

LepSource GetLepSource(int PDGID, int genPDGID, int momPDGID){
  if(abs(genPDGID) != 11 && abs(genPDGID) != 13){
    return kFake;
  } else if((abs(momPDGID) == abs(PDGID)) ||
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

// for sorting hemispheres, < means "more important"
bool LepList::operator < (const LepList& leps) const {
  if(m_N < leps.GetN())
    return false;
  if(m_N > leps.GetN())
    return true;

  for(int i = 0; i < m_N; i++){
    if((*this)[i] < leps[i])
      return false;
    if(leps[i] < (*this)[i])
      return true;
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

