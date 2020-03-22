#ifndef BtagSFTool_h
#define BtagSFTool_h

#include <TEfficiency.h>
#include <RooRealVar.h>
#include <RooFormulaVar.h>

using std::vector;

class FormulaBin {

public:
  FormulaBin(double min, double max, const std::string& formula) :
    m_X("x", "x", 0., 1e8), m_Min(min), m_Max(max) {
    m_SF_formula = new RooFormulaVar("formula", formula.c_str(), RooArgList(m_X));
  }
  virtual ~FormulaBin(){
    if(m_SF_formula != nullptr)
      delete m_SF_formula;
  }

  double SF(double pT){
    m_X.setVal(pT);
    return m_SF_formula->getVal();
  }

  double Min() const { return m_Min; }
  double Max() const { return m_Max; }
  
private:
  double m_Min;
  double m_Max;
  RooFormulaVar* m_SF_formula;
  RooRealVar m_X;
  
};

class FormulaBins {

public:
  FormulaBins(){
    m_Nbins = 0;
  }

  virtual ~FormulaBins(){
    for(int i = 0; i < m_Nbins; i++)
      delete m_Bins[i];
  }

  double SF(double pT);

  void AddBin(double min, double max, const std::string& formula);

private:
  int m_Nbins;
  vector<FormulaBin*> m_Bins;
};

class BtagSFTool {
  
public:
  BtagSFTool(){
    for(int i = 0; i < 3; i++){
      m_BtagEff2016[i] = nullptr;
      m_BtagEff2017[i] = nullptr;
      m_BtagEff2018[i] = nullptr;
      for(int j = 0; j < 3; j++)
	for(int k = 0; k < 3; k++)
	  m_SFs[i][j][k] = nullptr;
    }
  }
  
  virtual ~BtagSFTool(){
    for(int i = 0; i < 3; i++){
      if(m_BtagEff2016[i] != nullptr)
	delete m_BtagEff2016[i];
      if(m_BtagEff2017[i] != nullptr)
	delete m_BtagEff2017[i];
      if(m_BtagEff2018[i] != nullptr)
	delete m_BtagEff2018[i];
      for(int j = 0; j < 3; j++)
	for(int k = 0; k < 3; k++)
	  if(m_SFs[i][j][k] != nullptr)
	    delete m_SFs[i][j][k];
    }
  }

  double EFF(double pT, int year, int flavor);
  double SF(double pT, int year, int flavor, int updown = 0);
  
  void BuildMap(const std::string& btagSFfolder);

  void SetEfficiencies(const std::string& rootfile);
  
  void SetSFs(const std::string& csvfile, int year);
  
  // flavor_jet
  // 0 - b
  // 1 - c
  // 2 - other
  double GetWeight(int Njet, double* PT_jet, double* flavor_jet, bool* Btag_jet, int year, int updown = 0);

private:
  TEfficiency* m_BtagEff2016[3];
  TEfficiency* m_BtagEff2017[3];
  TEfficiency* m_BtagEff2018[3];

  FormulaBins* m_SFs[3][3][3]; // [year][flavor][sys]

  void ParseCSV(const std::string& csvfile, int iyear);
  std::string popcomma(std::string& line);
};

#endif






