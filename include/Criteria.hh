#ifndef Criteria_HH
#define Criteria_HH

#include <iostream>
#include <vector>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;
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

class VD : public vector<double> {
public:
  VD() {}

  VD(const vector<double>& vd){
    for(auto d : vd)
      *this += d;
  }

  VD& a(double d){
    (*this).push_back(d);
    return *this;
  }
  
  virtual ~VD() {}

  VD& operator += (double d){
    this->push_back(d);
    return *this;
  }

  VD& operator += (const VD& list){
    for(int i = 0; i < int(list.size()); i++)
      this->push_back(list[i]);
  
    return *this;
  }
};


class VS : public vector<string> {
public:
  VS();
  VS(const vector<string>& vs);
  
  virtual ~VS();

  VS& a(const string& s);

  VS& operator += (const string& entry);

  VS& operator += (const vector<string>& list);

  VS Filter(const string& label) const;
  VS Remove(const string& label) const;
  VS FilterOR(VS& labels) const;
  VS FilterAND(VS& labels) const;
  VS RemoveOR(VS& labels) const;
  VS RemoveAND(VS& labels) const;
  
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

