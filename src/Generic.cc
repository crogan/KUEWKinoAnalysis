#include <iostream>
#include <map>

#include "Generic.hh"

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

///////////////////////////////////////////
////////// FitBin class
///////////////////////////////////////////

// default is to include overflow in last bin for x, but not y
FitBin::FitBin(const vector<double>& bin_edges_x,
	       const vector<double>& bin_edges_y){
  m_hist1D = nullptr;
  m_hist2D = nullptr;
  
  if(bin_edges_x.size() < 2){
    m_NX = 1;
    if(bin_edges_x.size() == 0){
      m_Edges_X.push_back(-1.);
      m_Edges_X.push_back( 1.);
    } else {
      m_Edges_X.push_back(bin_edges_x[0]);
      m_Edges_X.push_back(m_Edges_X[0]+1.);
    }
  } else {
    m_Edges_X = bin_edges_x;
    m_NX = m_Edges_X.size() - 1;
  }
  if(bin_edges_y.size() < 2){
    m_NY = 1;
    if(bin_edges_y.size() == 0){
      m_Edges_Y.push_back(-1.);
      m_Edges_Y.push_back( 1.);
    } else {
      m_Edges_Y.push_back(bin_edges_y[0]);
      m_Edges_Y.push_back(m_Edges_Y[0]+1.);
    }
  } else {
    m_Edges_Y = bin_edges_y;
    m_NY = m_Edges_Y.size() - 1;
  }
  
  m_LastBinCenterX = (m_Edges_X[m_NX]+m_Edges_X[m_NX-1])/2.;
}

FitBin::FitBin(const FitBin& bin){
  m_hist1D = nullptr;
  m_hist2D = nullptr;
  
  m_Edges_X = bin.GetBinEdgesX();
  m_Edges_Y = bin.GetBinEdgesY();

  m_NX = m_Edges_X.size() - 1;
  m_NY = m_Edges_Y.size() - 1;

  m_LastBinCenterX = (m_Edges_X[m_NX]+m_Edges_X[m_NX-1])/2.;
}

FitBin::~FitBin(){
  if(m_hist1D)
    delete m_hist1D;
  if(m_hist2D)
    delete m_hist2D;
}

FitBin& FitBin::operator = (const FitBin& bin){
  if(m_hist1D)
    delete m_hist1D;
  if(m_hist2D)
    delete m_hist2D;

  m_hist1D = nullptr;
  m_hist2D = nullptr;
  
  m_Edges_X = bin.GetBinEdgesX();
  m_Edges_Y = bin.GetBinEdgesY();

  m_NX = m_Edges_X.size() - 1;
  m_NY = m_Edges_Y.size() - 1;

  m_LastBinCenterX = (m_Edges_X[m_NX]+m_Edges_X[m_NX-1])/2.;

  return *this;
}

FitBin& FitBin::InitializeHistogram(const std::string& label){
   m_hist2D = (TH2D*) new TH2D(string("h2D_"+label).c_str(),
			       string("h2D_"+label).c_str(),
			       m_NX, &m_Edges_X[0],
			       m_NY, &m_Edges_Y[0]);
   m_hist1D = (TH1D*) new TH1D(string("h1D_"+label).c_str(),
			       string("h1D_"+label).c_str(),
			       m_NX*m_NY, -0.5, m_NX*m_NY-0.5);
   return *this;
}

void FitBin::Fill(double weight, double X, double Y){
  if(m_hist1D == nullptr ||
     m_hist2D == nullptr)
    return;
  
  int iX = m_hist2D->GetXaxis()->FindBin(X);
  int iY = m_hist2D->GetYaxis()->FindBin(Y);
  if(iX <= 0 || iY <= 0 || iY > m_NY)
    return;
  if(iX > m_NX){
    iX -= 1;
    X = m_LastBinCenterX;
  }

  m_hist2D->Fill(X, Y, weight);
  m_hist1D->Fill((iX-1) + (iY-1)*m_NX, weight);
}

const vector<double>& FitBin::GetBinEdgesX() const {
  return m_Edges_X;
}

const vector<double>& FitBin::GetBinEdgesY() const {
  return m_Edges_Y;
}

void FitBin::WriteHistogram(const std::string& name,
			    const std::string& fold,
			    TFile& file) const {
  if(m_hist1D == nullptr ||
     m_hist2D == nullptr)
    return;

  file.cd();
  file.cd(fold.c_str());
  m_hist1D->Write(name.c_str(), TObject::kOverwrite);
  file.cd();
  file.cd(("hist2D/"+fold).c_str());
  m_hist2D->Write(("2D_"+name).c_str(), TObject::kOverwrite);
  file.cd();
}

