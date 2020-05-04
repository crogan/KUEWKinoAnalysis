#ifndef Criteria_HH
#define Criteria_HH

#include <iostream>
#include <vector>

using std::vector;
using std::string;
using std::pair;

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

class VS : public vector<string> {
public:
  VS() {}

  VS(const vector<string>& vs){
    for(auto s : vs)
      *this += s;
  }
  
  virtual ~VS() {}

  VS& operator += (const string& entry){
    this->push_back(entry);
    return *this;
  }

  VS& operator += (const vector<string>& list){
    for(int i = 0; i < int(list.size()); i++)
      this->push_back(list[i]);
  
    return *this;
  }
};

class VC : public vector<pair<int,string> > {
public:
  VC(){
    m_N = 0;
  }

  virtual ~VC() {}

  VC& operator += (const string& label){
    this->push_back({m_N, label});
    m_N++;

    return *this;
  }
  
private:
  int m_N;
  
};

class SM : public pair<string,VS> {
public:
  SM(const string& prefix){
    this->first = prefix;
  }

  virtual ~SM() {}

  SM& operator += (const string& mass){
    this->second += mass;

    return *this;
  }

  SM& operator += (const VS& masses){
    this->second += masses;

    return *this;
  }
};

// signal masses
class VSM : public vector<SM> {
public:
  VSM(){}

  virtual ~VSM() {}

  VSM& operator += (const SM& sm){
    if(m_SM.count(sm.first) == 0){
      this->push_back(sm);
      m_SM[sm.first] = this->size()-1;
    } else {
      (*this)[m_SM[sm.first]] += sm.second;
    }

    return *this;
  }
  
private:
  std::map<string,int> m_SM;

};

#endif

