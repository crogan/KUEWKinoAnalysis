#include <iostream>
#include <map>

#include "Hadronic.hh"

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

