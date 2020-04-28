#ifndef Criteria_HH
#define Criteria_HH

#include <iostream>
#include <vector>

using std::vector;
using std::string;

///////////////////////////////////////////
////////// Criteria class
///////////////////////////////////////////

class Criteria {
public:
  Criteria(const std::string& label = "none");
  Criteria(const Criteria&);

  virtual ~Criteria();

  const string& Label() const;
  Criteria& SetLabel(const string& label);

  virtual bool operator == (const Criteria& criteria) const = 0;

private:
  std::string m_Label;
};

class CriteriaList {
public:
  CriteriaList();
  CriteriaList(const CriteriaList& list);
  virtual ~CriteriaList();

  void Clear();
  
  CriteriaList& operator = (const CriteriaList& list);
  
  int GetN() const;
  const Criteria& operator [] (int i) const;

  CriteriaList& Add(const Criteria& criteria);
  CriteriaList& operator += (const Criteria& criteria);
  CriteriaList& operator += (const CriteriaList& criteria);
  
private:
  int m_N;
  vector<Criteria*> m_Criteria;
};

///////////////////////////////////////////
////////// SL (string list) class
///////////////////////////////////////////

class SL : public std::vector<std::string> {
public:
  SL(){ }
  virtual ~SL(){ }

  SL& a(const std::string& s){
    (*this).push_back(s);
    return *this;
  }
};

#endif

