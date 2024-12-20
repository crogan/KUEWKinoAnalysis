#ifndef NtupleBase_h
#define NtupleBase_h

#include "AnalysisBase.hh"

template <class Base>
class NtupleBase : public AnalysisBase<Base> {

public:
  NtupleBase(TTree* tree = 0);
  virtual ~NtupleBase();

  void WriteNtuple(const std::string& filename, int ichunk = 1, int nchunch = 1, bool do_slim = false);

protected:
  std::vector<TTree*>     m_Trees;
  std::map<string,vector<TTree*>> m_Label2Tree;
 

private:
  virtual TTree* InitOutputTree(const std::string& sample, bool do_slim = false) = 0;
  virtual void FillOutputTree(TTree* tree, const Systematic& sys = Systematic::Default(), bool do_slim = false) = 0;

  // for event count bookkeeping
  std::vector<std::pair<int,int> > m_masses;
  std::map<std::pair<int,int>,double > m_mapNevent;
  
  

};

#endif
