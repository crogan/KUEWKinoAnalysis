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

///////////////////////////////////////////
////////// FitBin class
///////////////////////////////////////////

class FitBin {
public:
  // default is to include overflow in last bin for x, but not y
  FitBin(const vector<double>& bin_edges_x = vector<double>(),
	 const vector<double>& bin_edges_y = vector<double>());
  FitBin(const FitBin&);
  
  virtual ~FitBin();

  FitBin& operator = (const FitBin& bin);
  
  FitBin& InitializeHistogram(const std::string& label);
  void Fill(double weight = 1., double X = 0., double Y = 0.);
  
  const vector<double>& GetBinEdgesX() const;
  const vector<double>& GetBinEdgesY() const;

  void WriteHistogram(const std::string& name,
		      const std::string& fold,
		      TFile& file) const;

private:
  int m_NX;
  int m_NY;

  vector<double> m_Edges_X;
  vector<double> m_Edges_Y;
  
  TH1D* m_hist1D;
  TH2D* m_hist2D;
  TH2D* m_hist2Dfine;

  double m_LastBinCenterX;
};

#endif

