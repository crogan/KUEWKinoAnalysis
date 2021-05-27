#ifndef Generic_HH
#define Generic_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <iostream>
#include <vector>

#include "Criteria.hh"

using std::vector;
using std::string;

///////////////////////////////////////////
////////// GenericBin class
///////////////////////////////////////////

class GenericBin : public Criteria {
public:
  GenericBin(double bin_min, double bin_max, const std::string& label = "none");
  GenericBin(const GenericBin&);

  virtual ~GenericBin();

  double GetMin() const;
  double GetMax() const;
  
  bool operator == (const Criteria& bin) const;

private:
  double m_Min;
  double m_Max;
};

class GenericVal : public Criteria {
public:
  GenericVal(double val, const std::string& label = "none");
  GenericVal(const GenericVal&);

  virtual ~GenericVal();

  double GetVal() const;
  
  bool operator == (const Criteria& bin) const;

private:
  double m_Val;
 
};



#endif

