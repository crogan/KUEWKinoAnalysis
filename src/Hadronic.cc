#include <iostream>
#include <map>

#include "../include/Hadronic.hh"

///////////////////////////////////////////
////////// Had class
///////////////////////////////////////////


Had::Had(HadFlavor flavor){
  m_Flavor = flavor;
}

Had::Had(const Had& had){
  m_Flavor = had.Flavor();
}

Had::~Had(){ }

  // for sorting hadrons in category
bool Had::operator < (const Had& had) const {
  return m_Flavor >= had.Flavor();
}

// for matching hadron categories
bool Had::operator == (const Had& had) const {
  return m_Flavor == had.Flavor();
}

HadFlavor Had::Flavor() const {
  return m_Flavor;
}

Had& Had::SetFlavor(HadFlavor flavor){
  m_Flavor = flavor;

  return *this;
}
 
std::string Had::IDLabel() const {
  if(m_Flavor == kSV)
    return "sv";
  else if(m_Flavor == kBtag)
    return "b";
  else
    return "j";
}
    
///////////////////////////////////////////
////////// HadList class
///////////////////////////////////////////

HadList::HadList(){
  m_N = 0;
}

HadList::HadList(const HadList& hads){
  m_N = 0;
  int N = hads.GetN();
  for(int i = 0; i < N; i++)
    *this += hads[i];
}

HadList::~HadList(){
  Clear();
}

void HadList::Clear(){
  for(int i = 0; i < m_N; i++)
    delete m_Had[i];
    
  m_Had.clear();
  m_N = 0;
}
  
HadList& HadList::operator = (const HadList& had){
  Clear();
  int N = had.GetN();
  for(int i = 0; i < N; i++)
    *this += had[i];
  
  return *this;
}
  
int HadList::GetN() const {
  return m_N;
}
const Had& HadList::operator [] (int i) const {
  return *m_Had[i];
}

HadList& HadList::Add(const Had& had){
  return (*this) += had;
}

HadList& HadList::operator += (const Had& had){
  m_Had.push_back(new Had(had));
  m_N++;
  
  if(m_N == 1)
    return *this;
  
  // keep list ordered
  int index = m_N-2;
  while(index >= 0){
    if(*m_Had[index+1] < *m_Had[index]){
      Had* temp = m_Had[index+1];
      m_Had[index+1] = m_Had[index];
      m_Had[index] = temp;
    } else {
      break;
    }
    index--;
  }

  return *this;
}

HadList& HadList::operator += (const HadList& hads){
  int N = hads.GetN();
  for(int i = 0; i < N; i++)
    *this += hads[i];
  
  return *this;
}

  // for matching hemispheres
bool HadList::operator == (const HadList& hads) const {
   if(m_N != hads.GetN())
    return false;

  for(int i = 0; i < m_N; i++)
    if(!((*this)[i] == hads[i]))
      return false;

  return true;
}

  // for sorting hemispheres
bool HadList::operator < (const HadList& hads) const {
  if(m_N < hads.GetN())
    return false;
  if(m_N > hads.GetN())
    return true;

  for(int i = 0; i < m_N; i++){
    if((*this)[i] < hads[i])
      return false;
    if(hads[i] < (*this)[i])
      return true;
  }

  return false;
}

std::string HadList::GetIDLabel() const {
  if(m_N == 0)
    return "0";
  string label = "";
  for(int i = 0; i < m_N; i++)
    label += (*this)[i].IDLabel();

  return label;
}
  

///////////////////////////////////////////
////////// Hadronic class
///////////////////////////////////////////

Hadronic::Hadronic(const HadList& Hem_a, const HadList& Hem_b, const std::string& label)
  : Criteria(label) {
  if(Hem_b < Hem_a){
    m_Had_a = Hem_b;
    m_Had_b = Hem_a;
  } else {
    m_Had_a = Hem_a;
    m_Had_b = Hem_b;
  }

  m_Njet  = 0;
  m_Nbjet = 0;
  m_NSV   = 0;
  
  HadList objects;
  objects += Hem_a;
  objects += Hem_b;
  int N = objects.GetN();
  for(int i = 0; i < N; i++){
    if(objects[i].Flavor() == kSV)
      m_NSV++;
    if(objects[i].Flavor() == kJet)
      m_Njet++;
    if(objects[i].Flavor() == kBtag){
      m_Nbjet++;
      m_Njet++;
    }
  }
  
   m_Nhadronic = 1;
   m_Hadronic.push_back(this);
}

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

  if(m_Had_a.GetN() == 0){
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
  } else {
    int N = had->GetN();
    for(int i = 0; i < m_Nhadronic; i++)
      for(int j = 0; j < N; j++)
	if(((*this)[i].GetHemA() == (*had)[j].GetHemA()) &&
	   ((*this)[i].GetHemB() == (*had)[j].GetHemB()))
	  return true;
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

const HadList& Hadronic::GetHemA() const {
  return m_Had_a;
}

const HadList& Hadronic::GetHemB() const {
  return m_Had_b;
}
