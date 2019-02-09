#ifndef AnalysisBase_h
#define AnalysisBase_h

#include <iostream>

#include <TTree.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TH1D.h>
#include <string>

using namespace std;

template <class Base>
class AnalysisBase : public Base {

public:
  AnalysisBase(TTree* tree = 0);
  virtual ~AnalysisBase();

  void AddLabel(string& label);
  void AddNevent(double nevt);

  virtual Int_t GetEntry(Long64_t entry);

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
  virtual double GetEventWeight();
  double GetXSEC(){ return m_XSEC; }


private:
  int m_CurrentFile;
  int m_DSID;
  string m_Label;
  double m_Nevent;
  double m_XSEC;
  map<string,double> m_IDtoNEVT;
  map<string,double> m_IDtoXSEC;

  map<string,double> m_Label2Nevent;
  map<string,double> m_Label2Nweight;
  map<string,double> m_Label2Nabsweight;
  map<string,double> m_Label2Xsec;

  void NewFile();
  void InitXSECmap();
  void InitMaps();
};

#endif









