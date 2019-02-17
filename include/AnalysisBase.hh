#ifndef AnalysisBase_h
#define AnalysisBase_h

#include <iostream>

#include <TTree.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TH1D.h>
#include <string>

#include "NeventTool.hh"
#include "XsecTool.hh"

using namespace std;

template <class Base>
class AnalysisBase : public Base {

public:
  AnalysisBase(TTree* tree = 0);
  virtual ~AnalysisBase();

  void AddLabel(const string& label);
  void DoSMS(){ m_DoSMS = true; }

  string GetEntry(int entry);

  // analysis functions
  virtual TVector3 GetMET();
  virtual int GetJets(vector<TLorentzVector>& JETs, double pt_cut = -1, double eta_cut = -1);
  virtual int GetJetsBtag(vector<pair<TLorentzVector,bool> >& JETs, double pt_cut = -1, double eta_cut = -1){ return 0; }
  virtual int GetLargeRJets(vector<TLorentzVector>& JETs, double pt_cut = -1, double eta_cut = -1);
  double DeltaPhiMin(const vector<TLorentzVector>& JETs, const TVector3& MET, int N = -1);
  double DeltaPhiMin(const vector<pair<TLorentzVector, bool> >& JETs, const TVector3& MET, int N = -1);

  virtual void GetLeptons(vector<TLorentzVector>& LEPs, vector<int>& IDs,
			  double pt_cut = -1, double eta_cut = -1);
  
  void MomTensorCalc(vector<TLorentzVector>& input, vector<double>& eigenvalues, double pow = 1., bool threeD = true); 

protected:
  bool m_DoSMS;
  
  virtual double GetEventWeight();
  virtual double GetXsec();


private:
  string m_Label;

  NeventTool m_NeventTool;
  XsecTool   m_XsecTool;

  int m_SampleIndex;
  virtual int GetSampleIndex();
  int m_Nsample;
  std::map<int,int>         m_HashToIndex;
  std::map<int,std::string> m_IndexToSample;
  std::map<int,double>      m_IndexToXsec;
  std::map<int,double>      m_IndexToNevent;
  std::map<int,double>      m_IndexToNweight;
  
};

#endif









