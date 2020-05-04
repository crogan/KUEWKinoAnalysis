#ifndef FitBin_HH
#define FitBin_HH

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>
#include <cstdarg>

using std::vector;
using std::string;

///////////////////////////////////////////
////////// MBins class
///////////////////////////////////////////

class MBins {
public:
  MBins();
  MBins(int Nedges,...);
  MBins(const vector<double>& bin_edges);
  MBins(const MBins& bins);

  virtual ~MBins();

  void operator = (const MBins& bins);
  
  int NBins() const;
  const vector<double>& BinEdges() const;

  int GetBin(double M) const;

  vector<string> GetBinLabels() const;
  
private:
  int m_N;
  vector<double> m_BinEdges;
  
};

///////////////////////////////////////////
////////// RBin class
///////////////////////////////////////////

class RBin {
public:
  RBin(double Rlow, double Rhigh, const MBins& bins);
  RBin(const RBin& bin);

  virtual ~RBin();

  void operator = (const RBin& bin);

  double Rlow() const;
  double Rhigh() const;

  const MBins& Bins() const;
  
  int NBins() const;
  const vector<double>& BinEdges() const;

  int GetBin(double M) const;

  string         GetRBinLabel() const;
  vector<string> GetMBinLabels() const;
  
private:
  MBins m_Bins;
  double m_Rlow;
  double m_Rhigh;
  
};

///////////////////////////////////////////
////////// FitBin class
///////////////////////////////////////////

class FitBin {
public:
  FitBin(int NRbins,...); // variable size list of RBin pointers
  FitBin(const vector<RBin>&);
  FitBin(const vector<double>& bin_edges_x = vector<double>(),
	 const vector<double>& bin_edges_y = vector<double>());
  FitBin(const FitBin&);
  
  virtual ~FitBin();

  void Clear();
  
  FitBin& operator = (const FitBin& bin);

  int NBins() const;
  int NRBins() const;
  int NMBins(int iR) const;
  
  const RBin& operator [] (int iR) const;

  int GetBin(double R, double M) const;
  
  FitBin& InitializeHistogram(const std::string& label);
  
  void Fill(double weight = 1., double M = 0., double R = 0.);

  void WriteHistogram(const std::string& name,
		      const std::string& fold,
		      TFile& file) const;
  
private:
  int m_N;
  int m_NR;
  vector<RBin*> m_RBins;
  
  TH1D* m_hist1D;
};

///////////////////////////////////////////
////////// FitBinBranch class
///////////////////////////////////////////

class FitBinBranch {
public:
  FitBinBranch();

  virtual ~FitBinBranch();

  void InitFill(TTree* tree);
  void FillFitBin(const FitBin& bin);

  void InitGet(TTree* tree);
  FitBin GetFitBin();
  
private:
  int m_NRBin;
  vector<double> m_RBinEdges;
  vector<int>    m_NMBin;
  vector<double> m_MBinEdges;

  vector<int>*    m_NMBinPtr;
  vector<double>* m_RBinEdgesPtr;
  vector<double>* m_MBinEdgesPtr;
  TBranch* m_b_NRBin;
  TBranch* m_b_NMBin;
  TBranch* m_b_RBinEdges;
  TBranch* m_b_MBinEdges;
  
};

///////////////////////////////////////////
////////// VD class
///////////////////////////////////////////

class VD : public std::vector<double> {
public:
  VD(){ }
  virtual ~VD(){ }

  VD& a(double v){
    (*this).push_back(v);
    return *this;
  }
};

#endif

