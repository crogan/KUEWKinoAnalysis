#ifndef NtupleBase_h
#define NtupleBase_h

#include "AnalysisBase.hh"

template <class Base>
class NtupleBase : public AnalysisBase<Base> {

public:
  NtupleBase(TTree* tree = 0);
  virtual ~NtupleBase();

  void WriteNtuple(const string& filename);

protected:
  TTree* m_Tree;
  
  TTree* m_EvtTree;
  double m_EvtTotal;
  double m_EvtSample;
  double m_EvtPreselection;
  double m_EvtSelected;

private:
  virtual void InitOutputTree() = 0;
  virtual void FillOutputTree() = 0;
  
  

};

#endif
