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
#include "JSONTool.hh"
#include "PUTool.hh"
#include "BtagSFTool.hh"
#include "JMETool.hh"
#include "SVDiscrTool.hh"

#include "Particle.hh"
#include "Systematics.hh"

using namespace std;

class ParticleList;

template <class Base>
class AnalysisBase : public Base {

public:
  AnalysisBase(TTree* tree = 0);
  virtual ~AnalysisBase();

  void AddLabels(const string& dataset, const string& filetag);
  void AddEventCountFile(const string& rootfile);
  void AddFilterEffFile(const string& rootfile);
  void AddJSONFile(const string& jsonfile);
  void AddPUFolder(const string& pufold);
  void AddBtagFolder(const string& btagfold);
  void AddJMEFolder(const string& jmefold);
  void AddSVDiscrFile(const string& svfile);
  void DoSMS();
  void DoData();
  void DoFastSim();
  void AddSystematics();
  void AddJESSystematics();
  void AddMETSystematics();
  void AddEESSystematics();
  void AddMMSSystematics();
  
  void InitializeHistograms(vector<TH1D*>& histos);
  void BookHistograms(vector<TH1D*>& histos);

  void InitializeSystematics();

  string GetEntry(int entry);

  // event functions
  virtual int GetRunNum();
  virtual int GetLumiNum();
  virtual long GetEventNum();
  
  // analysis functions
  virtual int GetNPV();
  virtual int GetNPUtrue();

  virtual bool GetMETtrigger();
  virtual bool GetMETHTtrigger();
  virtual bool GetMETORtrigger();
  
  virtual TVector3 GetPV(bool& good);
  virtual TVector3 GetMET();
  virtual ParticleList GetJets();
  virtual ParticleList GetJetsMET(TVector3& MET);
  virtual ParticleList GetElectrons();
  virtual ParticleList GetMuons();
  virtual ParticleList GetSVs(const TVector3& PV);

  virtual TVector3 GetGenMET();
  virtual ParticleList GetGenElectrons();
  virtual ParticleList GetGenMuons();
  virtual ParticleList GetGenNeutrinos();
  virtual ParticleList GetGenBosons();
  virtual ParticleList GetGenSparticles();
 
  double DeltaPhiMin(const vector<TLorentzVector>& JETs, const TVector3& MET, int N = -1);
  double DeltaPhiMin(const vector<pair<TLorentzVector, bool> >& JETs, const TVector3& MET, int N = -1);
  
  void MomTensorCalc(vector<TLorentzVector>& input, vector<double>& eigenvalues, double pow = 1., bool threeD = true);

  virtual std::pair<int,int> GetSUSYMasses();

  bool IsSMS(){ return m_DoSMS; }
  bool IsData(){ return m_IsData; }
  bool IsFastSim(){ return m_IsFastSim; }
  
  string GetDataSet(){ return m_DataSet; }
  string GetFileTag(){ return m_FileTag; }
  
protected:
  bool m_DoSMS;
  bool m_IsData;
  bool m_IsFastSim;
  
  virtual double GetEventWeight();
  virtual double GetPUWeight(int updown = 0);
  virtual double GetBtagSFWeight(const ParticleList& jets, int updown = 0, ParticleIDType tag = kMedium);
  virtual double GetXsec();
  virtual bool   IsGoodEvent();

  void SetSystematic(const Systematic& sys);

  string m_DataSet;
  string m_FileTag;

  Systematics m_Systematics;
  
private:

  NeventTool  m_NeventTool;
  XsecTool    m_XsecTool;
  JSONTool    m_JSONTool;
  PUTool      m_PUTool;
  BtagSFTool  m_BtagSFTool;
  JMETool     m_JMETool;
  SVDiscrTool m_SVDiscrTool;

  int m_SampleIndex;
  virtual int GetSampleIndex();
  int m_Nsample;
  std::map<int,int>         m_HashToIndex;
  std::map<int,std::string> m_IndexToSample;
  std::map<int,double>      m_IndexToXsec;
  std::map<int,double>      m_IndexToNevent;
  std::map<int,double>      m_IndexToNweight;

  const Systematic* m_CurSys;
  const Systematic& CurrentSystematic() const;

  Systematics m_DefaultSys;
  Systematics m_JESSys; // jet energy scale systematics
  Systematics m_MMSSys; // muon momentum scale
  Systematics m_EESSys; // electron energy scale
  Systematics m_METSys; // MET systematics
  
};

#endif









