#include <iostream>
#include <map>

//#include "Criteria.hh"
#include "Leptonic.hh"
#include "Hadronic.hh"
#include "Generic.hh"

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
////////// VS class
///////////////////////////////////////////

VS::VS() {}

VS::VS(const vector<string>& vs){
  for(auto s : vs)
    *this += s;
}

VS::~VS() {}

VS& VS::a(const string& s){
  (*this).push_back(s);
  return *this;
}

VS& VS::operator += (const string& entry){
  this->push_back(entry);
  return *this;
}

VS& VS::operator += (const vector<string>& list){
  for(int i = 0; i < int(list.size()); i++)
    this->push_back(list[i]);
  return *this;
}
 
VS VS::Filter(const string& label) const {
  VS list;

  for(auto s : *this)
    if(s.find(label) != std::string::npos)
      list += s;
  
  return list;
}

VS VS::Remove(const string& label) const {
  VS list;

  for(auto s : *this)
    if(s.find(label) == std::string::npos)
      list += s;
  
  return list;
}

VS VS::FilterOR(VS& labels) const {
  VS list;
  
  for(auto s : *this)
    for(auto l : labels)
      if(s.find(l) != std::string::npos){
	list += s;
	break;
      }
  
  return list;
}

VS VS::FilterAND(VS& labels) const {
  VS list;
  
  for(auto s : *this){
    bool match = true;
    for(auto l : labels)
      if(s.find(l) == std::string::npos){
	match = false;
	break;
      }
    if(match)
      list += s;
  }
  
  return list;
}

VS VS::RemoveOR(VS& labels) const {
  VS list;
  
  for(auto s : *this){
    bool match = false;
    for(auto l : labels)
      if(s.find(l) != std::string::npos){
	match = true;
	break;
      }
    if(!match)
      list += s;
  }
  
  return list;
}

VS VS::RemoveAND(VS& labels) const {
  VS list;
  
  for(auto s : *this)
    for(auto l : labels)
      if(s.find(l) == std::string::npos){
	list += s;
	break;
      }
  
  return list;
}
