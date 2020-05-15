#include <iostream>
#include <map>

#include "../include/Generic.hh"

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


