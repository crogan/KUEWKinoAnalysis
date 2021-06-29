#ifndef FitInputEditor_HH
#define FitInputEditor_HH

#include "FitReader.hh"

using std::map;
using std::string;
using std::pair;

///////////////////////////////////////////
////////// FitInputEditor class
///////////////////////////////////////////

class FitInputEditor : public FitReader {
public:
  FitInputEditor(const string& inputfile);

  virtual ~FitInputEditor();

  void WriteFit(const string& OutputFile);

  void SmoothFakes();
  void SmoothQCD();

  void AddShapeSysFakes();
  void AddShapeSysQCD();

  void AddFakeData();
  void AddEmptyData();
  
private:
  TFile* m_OutFile;

  void WriteProc();
  void WriteCat();

  // shape smoothing
  map<string, string> m_FakeGroups;
  map<string, ProcessList> m_FakeGroupLists;
  void InitShapeGroups();

  TH1D* m_hist;
  TH1D* m_hist_tot;
  void SmoothHist(TH1D* hist, TH1D* hist_tot);

  vector<pair<int,double> > m_binN;
  vector<pair<int,double> > m_binN_tot;
  void sortBins(vector<pair<int,double> >& array);

  double m_minN = 10.;
  void replaceBins(int idx = 0, double Prem = 1., double Prem_tot = 1.);

  void checkErrors(TH1D* hist_new, TH1D* hist_old);

  // shape variations
  TH1D* getVariation_RISR(string hname, TH1D* hist, const Category& cat, double varmax);
  TH1D* getVariation_Mperp(string hname, TH1D* hist, const Category& cat, double varmax);
  
};

#endif
