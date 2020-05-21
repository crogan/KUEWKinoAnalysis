#include <iostream>
#include <map>

#include "FitBin.hh"

///////////////////////////////////////////
////////// MBins class
///////////////////////////////////////////


MBins::MBins(){
  m_N = 1;
  m_BinEdges.push_back(-1.);
  m_BinEdges.push_back( 1.);
}

MBins::MBins(int Nedge,...){
  if(Nedge <= 2){
    m_N = 1;
    m_BinEdges.push_back(-1.);
    m_BinEdges.push_back( 1.);
    return;
  }

  m_N = Nedge-1;

  va_list valist;
  va_start(valist, Nedge);
  
  for(int i = 0; i < Nedge; i++){ 
    m_BinEdges.push_back( va_arg(valist, double) );
  }
  
  va_end(valist);

}

MBins::MBins(const VD& bin_edges){
  if(bin_edges.size() < 2){
    m_N = 1;
    m_BinEdges.push_back(-1.);
    m_BinEdges.push_back( 1.);
  } else {
    m_BinEdges = bin_edges;
    m_N = m_BinEdges.size() - 1;
  }
}

MBins::MBins(const MBins& bins){
  m_N = bins.NBins();
  m_BinEdges = bins.BinEdges();
}

MBins::~MBins() {}

void MBins::operator = (const MBins& bins){
  m_N = bins.NBins();
  m_BinEdges = bins.BinEdges();
}
  
int MBins::NBins() const {
  return m_N;
}

const VD& MBins::BinEdges() const {
  return m_BinEdges;
}

int MBins::GetBin(double M) const {
  if(M < m_BinEdges[0])
    return -1;

  for(int i = m_N-1; i >= 0; i--)
    if(M >= m_BinEdges[i])
      return i;

  return 0;
}

VS MBins::GetBinLabels() const {
  VS labels;
  for(int i = 0; i < m_N; i++){
    labels += "["+std::to_string(int(m_BinEdges[i]))+",";
    if(i == m_N-1)
      labels[i] += "#infty]";
    else
      labels[i] += std::to_string(int(m_BinEdges[i+1]))+"]";
  }

  return labels;
}
 

///////////////////////////////////////////
////////// RBin class
///////////////////////////////////////////

RBin::RBin(double Rlow, double Rhigh, const MBins& bins){
  m_Rlow  = Rlow;
  m_Rhigh = Rhigh;
  m_Bins  = bins;
}

RBin::RBin(const RBin& bin){
  m_Rlow  = bin.Rlow();
  m_Rhigh = bin.Rhigh();
  m_Bins  = bin.Bins();
}

RBin::~RBin() {}

const MBins& RBin::Bins() const {
  return m_Bins;
}

void RBin::operator = (const RBin& bin){
  m_Rlow  = bin.Rlow();
  m_Rhigh = bin.Rhigh();
  m_Bins  = bin.Bins();
}

double RBin::Rlow() const {
  return m_Rlow;
}
double RBin::Rhigh() const {
  return m_Rhigh;
}
  
int RBin::NBins() const {
  return m_Bins.NBins();
}

const VD& RBin::BinEdges() const {
  return m_Bins.BinEdges();
}

int RBin::GetBin(double M) const {
  return m_Bins.GetBin(M);
}

string RBin::GetRBinLabel() const {
  string label = "["+std::to_string(m_Rlow);
  label.erase(label.find_last_not_of('0')+1, std::string::npos);
  label.erase(label.find_last_not_of('.')+1, std::string::npos);
  label += ","+std::to_string(m_Rhigh);
  label.erase(label.find_last_not_of('0')+1, std::string::npos);
  label.erase(label.find_last_not_of('.')+1, std::string::npos);
  label += "]";

  return label;
}

VS RBin::GetMBinLabels() const {
  return m_Bins.GetBinLabels();
}

///////////////////////////////////////////
////////// FitBin class
///////////////////////////////////////////

FitBin::FitBin(int NRbins,...){
  m_hist1D = nullptr;

  if(NRbins <= 0){
    m_N  = 1;
    m_NR = 1;
    m_RBins.push_back(new RBin(-1, 1., MBins()));
    return;
  }
  
  m_NR = 0;

  va_list valist;
  va_start(valist, NRbins);
  
  for(int i = 0; i < NRbins; i++){ 
    RBin* rbin = dynamic_cast<RBin*>( va_arg(valist, RBin*) );
    if(!rbin){
      std::cout << "DISASTER!!!!" << std::endl;
      break;
    }

    m_RBins.push_back(new RBin(*rbin));
    m_NR++;
  }
  
  va_end(valist);

  if(m_NR == 0){
    m_RBins.push_back(new RBin(-1., 1., MBins()));
  }

  m_N = 0;

  for(int i = 0; i < m_NR; i++)
    m_N += m_RBins[i]->NBins();
  
}

FitBin::FitBin(const vector<RBin>& RBins){
  m_hist1D = nullptr;
  
  m_NR = RBins.size();
  if(m_NR < 1){
    m_NR = 1;
    m_N  = 1;
    m_RBins.push_back(new RBin(-1,1,MBins()));
  } else {
    m_N = 0;
    for(int i = 0; i < m_NR; i++){
      m_RBins.push_back(new RBin(RBins[i]));
      m_N += m_RBins[i]->NBins();
    }
  }
}

FitBin::FitBin(const VD& Rbin_edges,
	       const VD& Mbin_edges){
  m_hist1D = nullptr;
  
  int NR = Rbin_edges.size();
  VD Medges = Mbin_edges;
  
  int NM = Medges.size();
  if(NM < 2){
    if(NM == 0){
      Medges += -1.;
      Medges +=  1.;
    } else {
      Medges += Medges[0] + 1.;
    }
  }

  if(NR < 2){
    if(NR == 0){
      m_RBins.push_back(new RBin(-1., 1., MBins(Medges)));
    } else {
      m_RBins.push_back(new RBin(Rbin_edges[0], Rbin_edges[0] + 1., MBins(Medges)));
    }
    m_NR = 1;
    m_N  = int(Medges.size()-1);
  } else { 
    m_NR = NR - 1;
    for(int i = 0; i < m_NR; i++)
      m_RBins.push_back(new RBin(Rbin_edges[i], Rbin_edges[i+1], MBins(Medges)));
    
    m_N = int(Medges.size()-1)*m_NR;
  }
}

FitBin::FitBin(const FitBin& bin){
  m_hist1D = nullptr;

  m_N  = 0;
  m_NR = bin.NRBins();
  for(int i = 0; i < m_NR; i++){
    m_RBins.push_back(new RBin(bin[i]));
    m_N += m_RBins[i]->NBins();
  }
}

FitBin::~FitBin(){
  Clear();
}

void FitBin::Clear(){
  if(m_hist1D)
    delete m_hist1D;

  m_hist1D = nullptr;

  for(int i = 0; i < m_NR; i++)
    delete m_RBins[i];

  m_RBins.clear();
  m_NR = 0;
  m_N  = 0;
}

FitBin& FitBin::operator = (const FitBin& bin){
  Clear();
  
  m_N  = 0;
  m_NR = bin.NRBins();
  for(int i = 0; i < m_NR; i++){
    m_RBins.push_back(new RBin(bin[i]));
    m_N += m_RBins[i]->NBins();
  }
  
  return *this;
}

int FitBin::NBins() const {
  return m_N;
}

int FitBin::NRBins() const {
  return m_NR;
}

int FitBin::NMBins(int iR) const {
  if(iR < 0 || iR >= m_NR)
    return 0;

  return m_RBins[iR]->NBins();
}

const RBin& FitBin::operator [] (int iR) const {
  if(iR <= 0 || iR >= m_NR)
    iR = 0;

  return *m_RBins[iR];
}

int FitBin::GetBin(double R, double M) const {
  int ibin = 0;
  for(int i = 0; i < m_NR; i++){
    if(R >= m_RBins[i]->Rlow() && R < m_RBins[i]->Rhigh()){
      ibin += m_RBins[i]->GetBin(M);
      return ibin;
    } else {
      ibin += m_RBins[i]->NBins();
    }
  }

  return -1;
}

/////////////////////////////////////////////
FitBin& FitBin::InitializeHistogram(const string& label){
  m_hist1D = (TH1D*) new TH1D(string("h1D_"+label).c_str(),
			      string("h1D_"+label).c_str(),
			      m_N, -0.5, m_N - 0.5);
  m_hist1D->Sumw2();
  
  return *this;
}

void FitBin::Fill(double weight, double M, double R){
  if(m_hist1D == nullptr)
    return;
  
  m_hist1D->Fill(GetBin(R, M), weight);
  // if(GetBin(R, M) < 1)
  //   cout << R << " " << M << endl;
}

void FitBin::WriteHistogram(const string& name,
			    const string& fold,
			    TFile& file) const {
  if(m_hist1D == nullptr)
    return;

  if(!file.IsOpen())
    return;

  file.cd();
  file.cd(fold.c_str());
  m_hist1D->Write(name.c_str(), TObject::kOverwrite);
}

///////////////////////////////////////////
////////// FitBinBranch class
///////////////////////////////////////////


FitBinBranch::FitBinBranch() {}

FitBinBranch::~FitBinBranch() {}

void FitBinBranch::InitFill(TTree* tree){
  if(!tree)
    return;

  tree->Branch("NRBin", &m_NRBin);
  tree->Branch("NMBin", &m_NMBin);
  tree->Branch("RBinEdges", &m_RBinEdges);
  tree->Branch("MBinEdges", &m_MBinEdges);
}

void FitBinBranch::FillFitBin(const FitBin& bin){
  m_NRBin = bin.NRBins();
  m_NMBin.clear();
  m_MBinEdges.clear();
  m_RBinEdges.clear();
  for(int i = 0; i < m_NRBin; i++){
    m_NMBin.push_back(bin[i].NBins());
    if(i ==0) m_RBinEdges.push_back(bin[i].Rlow());
    m_RBinEdges.push_back(bin[i].Rhigh());
    for(int j = 0; j < m_NMBin[i]+1; j++){
      m_MBinEdges.push_back(bin[i].BinEdges()[j]);
    }
  }
}

void FitBinBranch::InitGet(TTree* tree){
  if(!tree)
    return;
  m_NMBinPtr     = 0;
  m_MBinEdgesPtr = 0;
  m_RBinEdgesPtr = 0;
  tree->SetBranchAddress("NRBin", &m_NRBin, &m_b_NRBin);
  tree->SetBranchAddress("NMBin", &m_NMBinPtr, &m_b_NMBin);
  tree->SetBranchAddress("MBinEdges", &m_MBinEdgesPtr, &m_b_MBinEdges);
  tree->SetBranchAddress("RBinEdges", &m_RBinEdgesPtr, &m_b_RBinEdges);
  
}

FitBin FitBinBranch::GetFitBin(){
  if(!m_NMBinPtr ||
     !m_MBinEdgesPtr ||
     !m_RBinEdgesPtr)
    return FitBin();

  vector<RBin> RBins;
  VD Mbins;
  int im = 0;
  for(int r = 0; r < m_NRBin; r++){
    Mbins.clear();
    for(int m = 0; m < m_NMBinPtr->at(r)+1; m++){
      Mbins.push_back(m_MBinEdgesPtr->at(im));
      im++;
    }
    RBins.push_back(RBin(m_RBinEdgesPtr->at(r),
			 m_RBinEdgesPtr->at(r+1),
			 MBins(Mbins)));
  }

  return FitBin(RBins);
}
